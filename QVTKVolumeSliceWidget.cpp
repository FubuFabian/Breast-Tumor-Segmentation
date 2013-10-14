#include "QVTKVolumeSliceWidget.h"
#include "QVTKImageWidgetCommand.h"
#include "VTKThreeViews.h"

#include <QSize.h>
#include <QBoxLayout>
#include <QString>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPropPicker.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkCamera.h>
#include <vtkImageFlip.h>


const double QVTKVolumeSliceWidget::axialElements[16] = {
    
1, 0, 0, 0,
0, 1, 0, 0,
0, 0, 1, 0,
0, 0, 0, 1 
        
};

const double QVTKVolumeSliceWidget::sagittalElements[16] = {
    
0, 0,-1, 0,
1, 0, 0, 0,
0,-1, 0, 0,
0, 0, 0, 1    
            
};

const double QVTKVolumeSliceWidget::coronalElements[16] = {
    
1, 0, 0, 0,
0, 0, 1, 0,
0,-1, 0, 0,
0, 0, 0, 1
        
};

const double QVTKVolumeSliceWidget::obliqueElements[16] = {
    
1, 0, 0, 0,
0, 0.866025, -0.5, 0,
0, 0.5, 0.866025, 0,
0, 0, 0, 1 
        
};


QVTKVolumeSliceWidget::QVTKVolumeSliceWidget(QWidget *parent) : QWidget(parent)
{
    qvtkWidget = new QVTKWidget(this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(qvtkWidget);
    this->setLayout(layout);

	this->resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
	this->reslicer = vtkSmartPointer<vtkImageReslice>::New();
	this->viewer = vtkSmartPointer<vtkImageViewer2>::New();
	this->imageActor = vtkSmartPointer<vtkImageActor>::New();
	this->imageStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
        this->horizontalFlipFlag = false;
        this->verticalFlipFlag = false;

	this->setAxialView();
        
        cornerAnnotation = vtkSmartPointer< vtkCornerAnnotation >::New();
        
        this->slice = 0;
}

QVTKVolumeSliceWidget::~QVTKVolumeSliceWidget()
{

}

void QVTKVolumeSliceWidget::setVolumeData(vtkSmartPointer<vtkImageData> volumeData)
{
	this->volumeData = volumeData;

	volumeData->GetSpacing(spacing);
	volumeData->GetOrigin(origin);
        volumeData->GetDimensions(dimensions);
        
}

void QVTKVolumeSliceWidget::displayVolumeSlice(int slice)
{
	double position[3];
        this->slice = slice;
        
	if(axialView){
		position[0] = origin[0] + spacing[0] * 0.5 * (dimensions[0]);		
		position[1] = origin[1] + spacing[1] * 0.5 * (dimensions[1]);	
                position[2] = origin[2] + spacing[2] * slice;
	}else if(sagittalView){
		position[0] = origin[0] + spacing[0] * slice;
                position[1] = origin[1] + spacing[1] * 0.5 * (dimensions[1]);
                position[2] = origin[2] + spacing[2] * 0.5 * (dimensions[2]);
	}else if(coronalView){

                position[0] = origin[0] + spacing[0] * 0.5 * (dimensions[0]);
		position[1] = origin[1] + spacing[1] * slice;
                position[2] = origin[2] + spacing[2] * 0.5 * (dimensions[2]);
	}	
	
	resliceAxes->SetElement(0, 3, position[0]);
	resliceAxes->SetElement(1, 3, position[1]);
	resliceAxes->SetElement(2, 3, position[2]);
        
	reslicer->SetInput(volumeData);
	reslicer->SetOutputDimensionality(2);
	reslicer->SetResliceAxes(resliceAxes);
	reslicer->SetInterpolationModeToLinear();
        
        vtkSmartPointer<vtkImageData> sliceImage = reslicer->GetOutput();
        
        if(horizontalFlipFlag)
            sliceImage = this->imageHorizontalFlip(sliceImage);
        
        if(verticalFlipFlag)
            sliceImage = this->imageVerticalFlip(sliceImage);
                    
	viewer->SetInput(sliceImage);
        viewer->GetRenderer()->ResetCamera();

        qvtkWidget->SetRenderWindow(viewer->GetRenderWindow());

	imageActor = viewer->GetImageActor();
	imageActor->InterpolateOff();

        
        this->initPicker();

}

void QVTKVolumeSliceWidget::setAxialView()
{
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	resliceAxes->DeepCopy(axialElements);
	axialView = true;
	coronalView = false;
	sagittalView = false;
        obliqueView = false;
}

void QVTKVolumeSliceWidget::setCoronalView()
{
	resliceAxes->DeepCopy(coronalElements);
	axialView = false;
	coronalView = true;
	sagittalView = false;
        obliqueView = false;
}

void QVTKVolumeSliceWidget::setSagittalView()
{
	resliceAxes->DeepCopy(sagittalElements);
	axialView = false;
	coronalView = false;
	sagittalView = true;
        obliqueView = false;
}

void QVTKVolumeSliceWidget::setObliqueView()
{
	resliceAxes->DeepCopy(obliqueElements);
	axialView = false;
	coronalView = false;
	sagittalView = false;
        obliqueView = true;
}



void QVTKVolumeSliceWidget::initPicker()
{
	// Picker to pick pixels
    vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
    propPicker->PickFromListOn();

    // Give the picker a prop to pick
    propPicker->AddPickList(imageActor);

    // Annotate the image with mouse over pixel information
    cornerAnnotation->SetLinearFontScaleFactor(2);
    cornerAnnotation->SetNonlinearFontScaleFactor(1);
    cornerAnnotation->SetMaximumFontSize(15);
    cornerAnnotation->GetTextProperty()->SetColor(1, 0, 0);
    viewer->GetRenderer()->AddViewProp(cornerAnnotation);

	    //listen to MouseMoveEvents invoked by the interactor's style
    vtkSmartPointer<QVTKImageWidgetCommand<QVTKVolumeSliceWidget> > callback =
            vtkSmartPointer<QVTKImageWidgetCommand<QVTKVolumeSliceWidget> >::New();
    callback->SetImageWidget(this);
    callback->SetAnnotation(cornerAnnotation);
    callback->SetPicker(propPicker);
    callback->setQVTKVolumeSliceWidgetFlagOn();

    vtkSmartPointer<vtkInteractorStyleImage> imageStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    viewer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(imageStyle);

    imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);


    // render image viewer
    viewer->Render();
}

void QVTKVolumeSliceWidget::setXPicked(int xPosition)
{
    this->xPicked = xPosition;
}


void QVTKVolumeSliceWidget::setYPicked(int yPosition)
{
    this->yPicked = yPosition;
}

void QVTKVolumeSliceWidget::setPickedCoordinates(int xPosition, int yPosition)
{   
    
    int* dimensions = volumeData->GetDimensions();
    
    if(axialView){
        
        if(horizontalFlipFlag)
            this->xPicked = (dimensions[0] - 1) - xPosition; 
        else
            this->xPicked = xPosition;

        
        if(verticalFlipFlag)
            this->yPicked = (dimensions[1] - 1) - yPosition;
        else
            this->yPicked = yPosition;
        

        this->zPicked = slice;
        
    }else if(sagittalView){
        
        this->xPicked = slice;
        
        if(horizontalFlipFlag)
            this->yPicked = (dimensions[1] - 1) - xPosition;
        else
            this->yPicked = xPosition;
        
        if(verticalFlipFlag)
            this->zPicked = yPosition;
        else
            this->zPicked = (dimensions[2] - 1) - yPosition;
        
    }else if(coronalView){
        
        if(horizontalFlipFlag)
            this->xPicked = (dimensions[0] - 1) - xPosition;
        else
            this->xPicked = xPosition;
        
        this->yPicked = slice;
        
        if(verticalFlipFlag)
            this->zPicked = yPosition;
        else
            this->zPicked = (dimensions[2] - 1) - yPosition;
        
    }
    
    this->threeViews->setPickedCoordinates(xPicked,yPicked,zPicked);

}

vtkSmartPointer<vtkImageViewer2> QVTKVolumeSliceWidget::getImageViewer()
{
    return this->viewer;
}

void QVTKVolumeSliceWidget::rotateCamera()
{
    this->viewer->GetRenderer()->GetActiveCamera()->Roll(-90);
    this->viewer->Render();
}

vtkSmartPointer<vtkImageData> QVTKVolumeSliceWidget::imageHorizontalFlip(vtkSmartPointer<vtkImageData> image)
{
    vtkSmartPointer<vtkImageFlip> flipFilter = vtkSmartPointer<vtkImageFlip>::New();
		
    flipFilter->SetFilteredAxis(0); 
    flipFilter->SetInput(image);
    flipFilter->Update();
    
    return flipFilter->GetOutput();

}

vtkSmartPointer<vtkImageData> QVTKVolumeSliceWidget::imageVerticalFlip(vtkSmartPointer<vtkImageData> image)
{
    vtkSmartPointer<vtkImageFlip> flipFilter = vtkSmartPointer<vtkImageFlip>::New();
		
    flipFilter->SetFilteredAxis(1); 
    flipFilter->SetInput(image);
    flipFilter->Update();
    
    return flipFilter->GetOutput();
}

void QVTKVolumeSliceWidget::setHorizontalFlipFlag()
{
    if(horizontalFlipFlag)
        this->horizontalFlipFlag = false;
    else
        this->horizontalFlipFlag = true;
    
    this->displayVolumeSlice(slice);
}

void QVTKVolumeSliceWidget::setVerticalFlipFlag()
{
    if(verticalFlipFlag)
        this->verticalFlipFlag = false;
    else
        this->verticalFlipFlag = true;
    
    this->displayVolumeSlice(slice);
}

void QVTKVolumeSliceWidget::setVTKThreeViews(VTKThreeViews * threeViews)
{
    this->threeViews = threeViews;
}

QVTKWidget* QVTKVolumeSliceWidget::getQVTKWidget()
{
    return this->qvtkWidget;
}