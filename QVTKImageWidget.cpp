//
//  QVTKImageWidget.cpp
//  US_Probe_Calibration
//
//  Created by Zian Fanti on 01/12/11.
//  Copyright (c) 2011 __UNAM__. All rights reserved.
//

#include "QVTKImageWidget.h"
#include "QVTKImageWidgetCommand.h"
#include "ChangeVolumePropertiesWidget.h"
#include "CheckCalibrationErrorWidget.h"
#include "SegmentationTrainingWidget.h"

#include <QSize.h>
#include <QBoxLayout>
#include <QString>

#include <itkImage.h>
#include <itkImageFileReader.h>

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include <vtkCornerAnnotation.h>
#include <vtkPropPicker.h>
#include <vtkTextProperty.h>

#include <vtkImageActor.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageFlip.h>

#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkMath.h>
#include <vtkMetaImageReader.h>

#include <vtkCallbackCommand.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkImageTracerWidget.h>
#include <vtkBMPReader.h>

QVTKImageWidget::QVTKImageWidget(QWidget *parent) : QWidget(parent)
{
    qvtkWidget = new QVTKWidget(this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(qvtkWidget);
    this->setLayout(layout);

    // by default 
    this->isImageStackLoaded = false;
    this->isVolumeImageStackLoaded = false;
    this->imageDisplayedIndex = 0;


    // create the essentials vtk objects to display the images    
    this->imageViewer = vtkSmartPointer<vtkImageViewer2>::New();

    // to display data in the corners of the image viewer
    this->cornerAnnotation = vtkSmartPointer< vtkCornerAnnotation >::New();
    
    this->pickerFlag = true;
    
    this->checkCalibrationErrorFlag = false;
    
    this->segmentationTrainingFlag = false;
    
    this->volumeLoaded = false;
    
    this->threeViews = new VTKThreeViews();
    
    this->propertiesChanger = new ChangeVolumePropertiesWidget();
    this->propertiesChanger->setQVTKImageWidget(this);
}



QVTKImageWidget::~QVTKImageWidget()
{
    //	renderWindow->Finalize();
    this->itkImage = NULL;
    this->vtkImage = NULL;
    this->qvtkWidget = NULL;
    this->imageViewer = NULL;
}


void QVTKImageWidget::setAndDisplayImage(QString imageFilename)
{
    // reads a vtkImage for display purposes
    vtkSmartPointer <vtkImageReader2Factory> readerFactory =
            vtkSmartPointer <vtkImageReader2Factory>::New();

    vtkSmartPointer <vtkImageReader2> reader =
            readerFactory->CreateImageReader2(imageFilename.toAscii().data());

    reader->SetFileName(imageFilename.toAscii().data());
    reader->Update();

    this->setAndDisplayImage(reader->GetOutput());

    readerFactory = NULL;
    reader = NULL;

}


void QVTKImageWidget::setAndDisplayImage(vtkSmartPointer<vtkImageData> image)
{
    // if the image is already defined 
    itkImage = NULL;
    rgbItkImage = NULL;
    vtkImage = NULL;

    this->vtkImage = image;

    this->setImageProperties(true);

    this->displayImage(vtkImage);
}


void QVTKImageWidget::setAndDisplayMultipleImages(QStringList filenames)
{
    if (this->imageStack.size() > 0)
        {
            this->imageStack.clear();
            for (uint i = 0; i < imageStack.size(); i++)
                {
                    imageStack.at(i) = NULL;
                }
        }

    this->imageStack.reserve(filenames.size());

    for (int i = 0; i < filenames.size(); i++)
        {
            vtkSmartPointer<vtkImageReader2Factory> readerFactory =
                    vtkSmartPointer<vtkImageReader2Factory>::New();

            vtkSmartPointer<vtkImageReader2> reader =
                    readerFactory->CreateImageReader2(filenames.at(i).toAscii().data());
            reader->SetFileName(filenames.at(i).toAscii().data());
            reader->Update();

            vtkSmartPointer<vtkImageData> image = reader->GetOutput();
            this->imageStack.push_back(image);

            readerFactory = NULL;
            reader = NULL;

        }

    isImageStackLoaded = true;

    displayImage(imageStack.at(imageDisplayedIndex));

}


void QVTKImageWidget::
setAndDisplayMultipleImages(std::vector<vtkSmartPointer<vtkImageData> > imageStack)
{
    if (imageStack.size() > 0)
        {
            this->imageStack.clear();
            this->imageStack = imageStack;
            displayImage(imageStack.at(imageDisplayedIndex));
        }
    isImageStackLoaded = true;
}

void QVTKImageWidget::setAndDisplayVolumeImages(QStringList imageFilenames, QString rotationFilename, 
												QString translationFilename, QString calibrationFilename)
{
    if (this->volumeImageStack.size() > 0)
        {
            this->volumeImageStack.clear();
            for (uint i = 0; i < volumeImageStack.size(); i++)
                {
                    volumeImageStack.at(i) = NULL;
                }
        }

    this->volumeImageStack.reserve(imageFilenames.size());

	std::cout<<std::endl;
	std::cout<<"Loading 2D Images"<<std::endl;
    for (int i = 0; i < imageFilenames.size(); i++)
    {
        vtkSmartPointer<vtkImageReader2Factory> readerFactory =
			vtkSmartPointer<vtkImageReader2Factory>::New();

        vtkSmartPointer<vtkImageReader2> reader =
            readerFactory->CreateImageReader2(imageFilenames.value(i).toAscii().data());
        
		reader->SetFileName(imageFilenames.at(i).toAscii().data());
        reader->Update();

        vtkSmartPointer<vtkImageData> image = reader->GetOutput();
		vtkSmartPointer<vtkImageFlip> flipYFilter = vtkSmartPointer<vtkImageFlip>::New();
		
		flipYFilter->SetFilteredAxis(1); 
		flipYFilter->SetInput(image);
		flipYFilter->Update();

		image = flipYFilter->GetOutput();
		image->SetScalarTypeToUnsignedChar();
		image->Update();
        
        this->volumeImageStack.push_back(image);

        readerFactory = NULL;
        reader = NULL;
	}

	std::cout<<std::endl;
	std::cout<<"Loading Rotation Data"<<std::endl;
    if (!rotationFilename.isEmpty())
    {

        QFile file(rotationFilename);
        if (!file.open(QIODevice::ReadOnly))
            return;

        QTextStream stream(&file);
        QString line;

        // declare a rotation matrix
        this->volumeDataRotations.set_size(volumeImageStack.size(), 4);
        int idx = 0;
        while (!stream.atEnd())
        {
             line = stream.readLine();
             QStringList lineList = line.split(" ");

             for (int j = 0; j < lineList.size(); j++)
             {
                 volumeDataRotations.put(idx, j, lineList.value(j).toDouble());
             }

             idx++;
         }
                file.close();
     }

	std::cout<<std::endl;
	std::cout<<"Loading Translation Data"<<std::endl;
        
     if (!translationFilename.isEmpty())
     {

         QFile file(translationFilename);
         if (!file.open(QIODevice::ReadOnly))
            return;

         QTextStream stream(&file);
         QString line;

         // declare a translation matrix
         this->volumeDataTranslations.set_size(volumeImageStack.size(), 3);

         int idx = 0;
         while (!stream.atEnd())
         {
               line = stream.readLine();
               QStringList lineList = line.split(" ");

               for (int j = 0; j < lineList.size(); j++)
               {
                  volumeDataTranslations.put(idx, j, lineList.value(j).toDouble());
               }

               idx++;
                    //std::cout << line.toAscii().data() << std::endl;
          }
          
		  file.close(); // when your done.
        }

	 std::cout<<std::endl;
	 std::cout<<"Loading Calibration Data"<<std::endl;
	 if (!calibrationFilename.isEmpty())
     {

         QFile file(calibrationFilename);
         if (!file.open(QIODevice::ReadOnly))
            return;

         QTextStream stream(&file);
         QString line;

         // declare a translation matrix
         this->volumeDataCalibration.reserve(8);


         while (!stream.atEnd())
         {
               line = stream.readLine();           
			   volumeDataCalibration.push_back(line.toDouble());
          }
          
		  file.close(); // when your done.
        }      

    isVolumeImageStackLoaded = true;

    displayVolumeImages(volumeImageStack, volumeDataRotations, volumeDataTranslations, volumeDataCalibration);

}


void QVTKImageWidget::setAndDisplayVolume(QString volumeFilename)
{


    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName(volumeFilename.toAscii().data());
    reader->Update();

    volumeData = reader->GetOutput();

    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();

    vtkSmartPointer<vtkVolumeRayCastCompositeFunction> rayCastFunction =
                vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();

    volumeScalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeScalarOpacity->AddPoint(0,0.00);
    volumeScalarOpacity->AddPoint(127,1.00);
    volumeScalarOpacity->AddPoint(255,0.00);
    volumeScalarOpacity->Update();
    volumeProperty->SetScalarOpacity(volumeScalarOpacity);

    vtkSmartPointer<vtkColorTransferFunction> volumeColor = vtkSmartPointer<vtkColorTransferFunction>::New();
    volumeColor->AddRGBPoint(0,0.0,0.0,0.0);
    volumeColor->AddRGBPoint(64,0.25,0.25,0.25);
    volumeColor->AddRGBPoint(128,0.5,0.5,0.5);
    volumeColor->AddRGBPoint(192,0.75,0.75,0.75);
    volumeColor->AddRGBPoint(255,1.0,1.0,1.0);
    volumeProperty->SetColor(volumeColor);

    vtkSmartPointer<vtkVolumeRayCastCompositeFunction> compositeFunction =
            vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();

    vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
    volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    volumeMapper->CroppingOff();
    volumeMapper->SetInput(volumeData);

    volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetOrigin(0,0,0);
    volume->SetProperty(volumeProperty);
    volume->Update();
    
    threeViews->setVolumeData(volumeData);
    threeViews->show();
    
    propertiesChanger->show();

    this->displayVolume(volume);

}

void QVTKImageWidget::setAndDisplayVolume(vtkSmartPointer<vtkImageData> volumeData)
{

    this->volumeData = volumeData;

    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();

    vtkSmartPointer<vtkVolumeRayCastCompositeFunction> rayCastFunction =
                vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();

    volumeScalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeScalarOpacity->AddPoint(0,0.00);
    volumeScalarOpacity->AddPoint(127,1.00);
    volumeScalarOpacity->AddPoint(255,0.00);
    volumeScalarOpacity->Update();
    volumeProperty->SetScalarOpacity(volumeScalarOpacity);

    vtkSmartPointer<vtkColorTransferFunction> volumeColor = vtkSmartPointer<vtkColorTransferFunction>::New();
    volumeColor->AddRGBPoint(0,0.0,0.0,0.0);
    volumeColor->AddRGBPoint(64,0.25,0.25,0.25);
    volumeColor->AddRGBPoint(128,0.5,0.5,0.5);
    volumeColor->AddRGBPoint(192,0.75,0.75,0.75);
    volumeColor->AddRGBPoint(255,1.0,1.0,1.0);
    volumeProperty->SetColor(volumeColor);

    vtkSmartPointer<vtkVolumeRayCastCompositeFunction> compositeFunction =
            vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();

    vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
    volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    volumeMapper->CroppingOff();
    volumeMapper->SetInput(volumeData);

    volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetOrigin(0,0,0);
    volume->SetProperty(volumeProperty);
    volume->Update();

    threeViews->setVolumeData(this->volumeData);
    threeViews->show();

    propertiesChanger->show();
    
    this->displayVolume(volume);

}

void QVTKImageWidget::displayImage(vtkImageData *image)
{
    imageViewer->SetInput(image);
    imageViewer->GetRenderer()->ResetCamera();

    // make qt-vtk connection
    qvtkWidget->SetRenderWindow(imageViewer->GetRenderWindow());

    // disable interpolation, so we can see each pixel
    imageActor = imageViewer->GetImageActor();
    imageActor->InterpolateOff();

    if(this->pickerFlag){
        
        this->initPicker();
        
    }else if(this->checkCalibrationErrorFlag){
        
        this->checkCalibrationErrorStyle->clearTracer();
        this->startTracer();	
        
    }else if(this->segmentationTrainingFlag){
        this->segmentationTrainingStyle->clearTracer();
        this->startTracer();
    }
}

void QVTKImageWidget::initPicker()
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
    imageViewer->GetRenderer()->AddViewProp(cornerAnnotation);

	    //listen to MouseMoveEvents invoked by the interactor's style
    vtkSmartPointer<QVTKImageWidgetCommand<QVTKImageWidget> > callback =
            vtkSmartPointer<QVTKImageWidgetCommand<QVTKImageWidget> >::New();
    callback->SetImageWidget(this);
    callback->SetAnnotation(cornerAnnotation);
    callback->SetPicker(propPicker);
    callback->setQVTKImageWidgetFlagOn();

    vtkSmartPointer<vtkInteractorStyleImage> imageStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    imageViewer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(imageStyle);

    imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);


    // render image viewer
    imageViewer->Render();
}

void QVTKImageWidget::startTracer()
{
    
  if(this->checkCalibrationErrorFlag){
   
      this->checkCalibrationErrorStyle = 
              vtkSmartPointer< vtkTracerInteractorStyle<CheckCalibrationErrorWidget> >::New();
      this->checkCalibrationErrorStyle->setCallerWidget(this->calibrationErrorWidget);
      imageViewer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(this->checkCalibrationErrorStyle);
      
      this->checkCalibrationErrorStyle->initTracer(imageActor);
      this->checkCalibrationErrorStyle->setAutoCloseOff();
      
  }else if(this->segmentationTrainingFlag){
   
      this->segmentationTrainingStyle = 
              vtkSmartPointer< vtkTracerInteractorStyle<SegmentationTrainingWidget> >::New();
      this->segmentationTrainingStyle->setCallerWidget(this->segmentationTrainingWidget);
      imageViewer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(this->segmentationTrainingStyle);
      
      this->segmentationTrainingStyle->initTracer(imageActor);
      this->segmentationTrainingStyle->setAutoCloseOn();
      
  }  
  
  imageViewer->GetRenderer()->ResetCamera();
  imageViewer->Render();

}

void QVTKImageWidget::displayVolumeImages(std::vector< vtkSmartPointer<vtkImageData> > volumeImageStack,
                         vnl_matrix<double> volumeDataRotations, vnl_matrix<double> volumeDataTranslations,
						 std::vector<double> volumeDataCalibration)
{

    if (this->volumeImageActorStack.size() > 0)
        {
            this->volumeImageActorStack.clear();
            for (uint i = 0; i < volumeImageActorStack.size(); i++)
                {
                    volumeImageActorStack.at(i) = NULL;
                }
        }

    this->volumeImageActorStack.reserve(volumeImageStack.size());

    //Creating Image Actors
	std::cout<<std::endl;
	std::cout<<"Calculating Transformation Matrix for images "<<std::endl;

    for(int i=0; i < volumeImageStack.size(); i++)
    {	
		vnl_matrix<double> imageTransform = this->computeTransformation(volumeDataRotations.get_row(i), 
			volumeDataTranslations.get_row(i), volumeDataCalibration);

		vtkSmartPointer<vtkMatrix4x4> vtkImageTransform = vtkSmartPointer<vtkMatrix4x4>::New();

		for (int k=0; k < 4; k++){
			for(int j=0; j < 4; j++){
				vtkImageTransform->SetElement(k,j,imageTransform[k][j]);
			}
		}
	
		vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
		transform->SetMatrix(vtkImageTransform);
        
		vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();

        actor->SetInput(volumeImageStack.at(i));
        actor->SetUserTransform(transform);
		
		scale.set_size(2);
		scale.put(0,volumeDataCalibration[6]);
		scale.put(1,volumeDataCalibration[7]);
		actor->SetScale(scale[0],scale[1],1);

        volumeImageActorStack.push_back(actor);
		transformStack.push_back(imageTransform);

    }

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(183.0/255.0,197.0/255.0,253.0/255.0);

    //Adding Image Actors to renderer
	std::cout<<std::endl;
	std::cout<<"Adding images to 3D scene"<<std::endl;
	for(int i=0; i < volumeImageActorStack.size(); i++){
        renderer->AddActor(volumeImageActorStack.at(i));
	}

    renwin = vtkSmartPointer<vtkRenderWindow>::New();
    renwin->AddRenderer(renderer);

    qvtkWidget->SetRenderWindow(renwin);
	std::cout<<std::endl;
	std::cout<<"Displaying 3D scene"<<std::endl<<std::endl;
    renwin->Render();

	//prueba();

}

vnl_matrix<double> QVTKImageWidget::computeTransformation(vnl_vector<double> quaternion, vnl_vector<double> translation,
																	 std::vector<double> calibration)
{

    double x = calibration[0];
    double y = calibration[1];
    double z = calibration[2];
    double a = calibration[3];
    double b = calibration[4];
    double c = calibration[5];

    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

    vnl_quaternion<double> rTpQuat(c, b, a);
    vnl_matrix<double> rTp = rTpQuat.rotation_matrix_transpose_4();
    rTp = rTp.transpose();
    rTp.put(0, 3, x);
    rTp.put(1, 3, y);
    rTp.put(2, 3, z);

    vnl_quaternion<double> tTrQuat(quaternion[1], quaternion[2], quaternion[3], quaternion[0]);
    vnl_matrix<double> tTr = tTrQuat.rotation_matrix_transpose_4();
    tTr = tTr.transpose();
    tTr.put(0, 3, translation[0]);
    tTr.put(1, 3, translation[1]);
    tTr.put(2, 3, translation[2]);

    vnl_matrix<double> tTp = tTr*rTp;

    return tTp;
	

}

void QVTKImageWidget::displaySelectedImage(int idx)
{
    if (!imageStack.empty())
        {
            if (idx >= 0 && idx < (int) imageStack.size())
                {
                    imageDisplayedIndex = idx;
                    displayImage(imageStack[imageDisplayedIndex]);
                }
        }
}

void QVTKImageWidget::displayVolume(vtkSmartPointer<vtkVolume> volume)
{
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(183.0/255.0,197.0/255.0,253.0/255.0);
    renderer->AddVolume(volume);
    renderer->GetActiveCamera()->SetFocalPoint(volume->GetCenter());
    renderer->GetActiveCamera()->Roll(90);
    
    double cameraPos[3];
    cameraPos[0] = volume->GetCenter()[0] + volume->GetMaxXBound() + 30;
    cameraPos[1] = volume->GetCenter()[1];
    cameraPos[2] = volume->GetCenter()[2];
    
    renderer->GetActiveCamera()->SetPosition(cameraPos);

    renwin = vtkSmartPointer<vtkRenderWindow>::New();
    renwin->AddRenderer(renderer);
    
    qvtkWidget->SetRenderWindow(renwin);
    std::cout<<std::endl;
    std::cout<<"Displaying volume"<<std::endl<<std::endl;
    renwin->Render();
    
    //MainWindow->setVolumeLoaded(true);
    
    this->volumeLoaded = true;
}

void QVTKImageWidget::setImageProperties(bool verbose)
{
    this->numDimensions = this->vtkImage->GetDataDimension();
    this->pixelType = this->vtkImage->GetScalarTypeAsString();
    this->imageType = this->vtkImage->GetNumberOfScalarComponents();

    int* dim = vtkImage->GetDimensions();
    this->imageWidth = dim[0];
    this->imageHeight = dim[1];
    std::cout << imageWidth << "," << imageHeight << "," << dim[2] << std::endl;

    if (verbose)
        {
            std::cout << "Pixels type: " << pixelType << std::endl;
            std::cout << "Image type: " << imageType << std::endl;
            std::cout << "Num of Dimensions: " << numDimensions << std::endl;
        }
}

void QVTKImageWidget::setVolumeOpacity(vtkSmartPointer<vtkPiecewiseFunction> opacity)
{
	volumeProperty->SetScalarOpacity(opacity);

    renwin->Render();

}


QVTKWidget* QVTKImageWidget::getQVTKWidget()
{
    return this->qvtkWidget;
}


vtkSmartPointer<vtkImageViewer2> QVTKImageWidget::getImageViewer()
{
    return this->imageViewer;
}


std::vector< vtkSmartPointer<vtkImageData> > QVTKImageWidget::getImageStack()
{
    return this->imageStack;
}

std::vector< vtkSmartPointer<vtkImageData> > QVTKImageWidget::getVolumeImageStack()
{
    return this->volumeImageStack;
}

std::vector< vnl_matrix<double> > QVTKImageWidget::getTransformStack()
{
    return this->transformStack;
}

int QVTKImageWidget::getXPicked()
{
    return this->xPicked;
}


int QVTKImageWidget::getYPicked()
{
    return this->yPicked;
}

vnl_vector<double> QVTKImageWidget::getTransformScale()
{
	return this->scale;
}

void QVTKImageWidget::setXPicked(int xPosition)
{
    this->xPicked = xPosition;
}


void QVTKImageWidget::setYPicked(int yPosition)
{
    this->yPicked = yPosition;
}

void QVTKImageWidget::setPickedCoordinates(int xPosition, int yPosition)
{
    this->xPicked = xPosition;
    this->yPicked = yPosition;
}

void QVTKImageWidget::setVolumeData(vtkSmartPointer<vtkImageData> volumeData)
{
    this->volumeData = volumeData;
}

QString QVTKImageWidget::getPixelType()
{
    return QString(pixelType.c_str());
}


QString QVTKImageWidget::getImageType()
{
    if (imageType == 1)
        {
            return QString("grayscale");
        }
    else if (imageType == 3)
        {
            return QString("rgb");
        }
    else
        {
            return QString("-");
        }
}


QString QVTKImageWidget::getNumOfDimesions()
{
    QString out;
    out.setNum(numDimensions);
    return out;
}


int QVTKImageWidget::getImageDisplayedIndex()
{
    return imageDisplayedIndex;
}

vtkSmartPointer<vtkImageData> QVTKImageWidget::getVolume()
{
    return volumeData;
}

VTKThreeViews* QVTKImageWidget::getVTKThreeViews()
{
    return threeViews;
}

bool QVTKImageWidget::getVolumeLoaded()
{
    return volumeLoaded;
}

void QVTKImageWidget::setCalibrationErrorWidget(CheckCalibrationErrorWidget* calibrationErrorWidget)
{
	this->calibrationErrorWidget = calibrationErrorWidget;
}

void QVTKImageWidget::setSegmentationTrainingWidget(SegmentationTrainingWidget* segmentationTrainingWidget)
{
	this->segmentationTrainingWidget = segmentationTrainingWidget;
}

void QVTKImageWidget::setPickerFlag()
{
    this->pickerFlag = true;
    this->checkCalibrationErrorFlag = false;
    this->segmentationTrainingFlag = false;
}

void QVTKImageWidget::setCheckCalibrationErrorFlag()
{
    this->pickerFlag = false;
    this->checkCalibrationErrorFlag = true;
    this->segmentationTrainingFlag = false;
}

void QVTKImageWidget::setSegmentationTrainingFlag()
{
    this->pickerFlag = false;
    this->checkCalibrationErrorFlag = false;
    this->segmentationTrainingFlag = true;
}

