#include "QVTKVolumeSliceWidget.h"

#include <QSize.h>
#include <QBoxLayout>
#include <QString>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>


const double QVTKVolumeSliceWidget::axialElements[16] = {
	-1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 
};

const double QVTKVolumeSliceWidget::coronalElements[16] = {
    -1, 0, 0, 0,
    0, 0, 1, 0,
    0,1, 0, 0,
    0, 0, 0, 1 
};

const double QVTKVolumeSliceWidget::sagittalElements[16] = {
    0, 0,1, 0,
    -1, 0, 0, 0,
    0,1, 0, 0,
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

	this->setAxialView();
}

QVTKVolumeSliceWidget::~QVTKVolumeSliceWidget()
{

}

void QVTKVolumeSliceWidget::setVolumeData(vtkSmartPointer<vtkImageData> volumeData)
{
	this->volumeData = volumeData;

	volumeData->GetSpacing(spacing);
	volumeData->GetOrigin(origin);
}

void QVTKVolumeSliceWidget::displayVolumeSlice(int slice)
{
	double position[3];

	if(axialView){
		position[0] = origin[0];
		position[1] = origin[1];
		position[2] = origin[2] + spacing[2]*slice;		
	}else if(coronalView){
		position[0] = origin[0];
		position[1] = origin[1] + spacing[1]*slice;
		position[2] = origin[2];
	}else if(sagittalView){
		position[0] = origin[0] + spacing[0]*slice;
		position[1] = origin[1];
		position[2] = origin[2];
	}
	
	
	resliceAxes->SetElement(0, 3, position[0]);
	resliceAxes->SetElement(1, 3, position[1]);
	resliceAxes->SetElement(2, 3, position[2]);

	reslicer->SetInput(volumeData);
	reslicer->SetOutputDimensionality(2);
	reslicer->SetResliceAxes(resliceAxes);
	reslicer->SetInterpolationModeToLinear();

	viewer->SetInput(reslicer->GetOutput());
    viewer->GetRenderer()->ResetCamera();

    qvtkWidget->SetRenderWindow(viewer->GetRenderWindow());

	imageActor = viewer->GetImageActor();
	imageActor->InterpolateOff();

	viewer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(imageStyle);

	viewer->Render();
}

void QVTKVolumeSliceWidget::setAxialView()
{
	resliceAxes->DeepCopy(axialElements);
	axialView = true;
	coronalView = false;
	sagittalView = false;
}

void QVTKVolumeSliceWidget::setCoronalView()
{
	resliceAxes->DeepCopy(coronalElements);
	axialView = false;
	coronalView = true;
	sagittalView = false;
}

void QVTKVolumeSliceWidget::setSagittalView()
{
	resliceAxes->DeepCopy(sagittalElements);
	axialView = false;
	coronalView = false;
	sagittalView = true;
}