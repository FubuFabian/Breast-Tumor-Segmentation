#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "ProbeCalibrationWidget.h"
#include "CheckCalibrationErrorWidget.h"
#include "VolumeReconstructionWidget.h"
#include "CropImagesWidget.h"
#include "Scene3D.h"
#include "PivotCalibration.h"
#include "SegmentationTrainingWidget.h"
#include "ImageSegmentationWidget.h"
#include "VolumeSegmentationWidget.h"
#include "VTKThreeViews.h"

#include <QVBoxLayout>
#include <vtkEventQtSlotConnect.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->setAttribute(Qt::WA_DeleteOnClose);

  this->displayWidget = new QVTKImageWidget();

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->QLayout::addWidget(displayWidget);
  ui->imageWidget->setLayout(layout);

  // hide the slider andonly show if an image stack is load
  ui->imageSlider->hide();
  // create the connections 
  Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  
  this->imageLoaded = false;
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::addImages()
{

    
    
  std::cout<<"Loading 2D Images"<<std::endl;
  this->imagesFilenames = QFileDialog::getOpenFileNames(this, tr("Open Images"),
	  QDir::currentPath(),tr("Image Files (*.png *.jpg *.bmp)"));
  if (!imagesFilenames.isEmpty())
    {
      if (imagesFilenames.size() == 1)
        {
          ui->imageSlider->hide();

          addLogText("Loading: <b>" + imagesFilenames.first() + "</b>");
          displayWidget->setAndDisplayImage(imagesFilenames.first());

          addLogText("Image Type: <b>" + displayWidget->getImageType() + "</b>");
          addLogText("Pixel Type: <b>" + displayWidget->getPixelType() + "</b>");
          addLogText("Num of Dimensions: <b>" + displayWidget->getNumOfDimesions() + "</b>");
          
          this->imageLoaded = true;
        }
      else
        {
          // if selected multiple files
          ui->imageSlider->show();
          ui->imageSlider->setTickInterval(1);
          ui->imageSlider->setRange(0, imagesFilenames.size() - 1);

          this->displayWidget->setAndDisplayMultipleImages(imagesFilenames);
          for (int i = 0; i < imagesFilenames.size(); i++)
            {
              addLogText("Loading: <b>" + imagesFilenames.at(i) + "</b>");
            }
          
          this->imageLoaded = true;
        }
    }
  else
    {
      return;
    }
}

void MainWindow::openVolumeData()
{

	std::cout<<"Loading Volume Data"<<std::endl;

    this->volumeImagesFilenames = QFileDialog::getOpenFileNames(this, tr("Open Volume Images"), 
		QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
	
    this->volumeRotationData  = QFileDialog::getOpenFileName(this, tr("Open Volume Rotation Data"), 
		QDir::currentPath(), tr("Txt (*.txt *.doc)"));
	
    this->volumeTranslationData  = QFileDialog::getOpenFileName(this, tr("Open Volume Translation Data"), 
		QDir::currentPath(), tr("Txt (*.txt *.doc)"));
	
	this->volumeCalibrationData  = QFileDialog::getOpenFileName(this, tr("Open Volume Calibration Data"), 
		QDir::currentPath(), tr("Txt (*.txt *.doc)"));
    
	if (!volumeImagesFilenames.isEmpty())
      {
            this->displayWidget->setAndDisplayVolumeImages(volumeImagesFilenames, 
				volumeRotationData, volumeTranslationData, volumeCalibrationData);

      }
    else
      {
        return;
      }

}

void MainWindow::openVolume()
{


	std::cout<<"Loading Volume"<<std::endl;

    this->volumeFilename = QFileDialog::getOpenFileName(this, tr("Open Volume .mhd"),
        QDir::currentPath(), tr("Volume Files (*.mhd)"));
    
	this->displayWidget->setAndDisplayVolume(volumeFilename);

}

void MainWindow::probeCalibration()
{

    std::cout<<"Probe Calibration"<<std::endl;

      if (!displayWidget->getImageStack().empty())
      {
      ProbeCalibrationWidget* probeCalibration = new ProbeCalibrationWidget();

      if (displayWidget->isImageStackLoaded)
        probeCalibration->setImageStack(displayWidget->getImageStack());
      else
        probeCalibration->setImage(displayWidget->getImageViewer()->GetInput());

	  displayWidget->setPickerFlag();
      // get left mouse pressed with high priority
      Connections->Connect(displayWidget->getQVTKWidget()->GetRenderWindow()->GetInteractor(),
                           vtkCommand::LeftButtonPressEvent,
                           probeCalibration,
                           SLOT(getCoordinates()));

      probeCalibration->setMainWindow(this);
      probeCalibration->show();
    }
  else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load images before calibrate the probe");
      errorMessage.exec();
    }
}

void MainWindow::pivotCalibration()
{
	std::cout<<"Pivot Calibration"<<std::endl;

	PivotCalibration *app = new PivotCalibration();
	app->Show();

	while( !app->HasQuitted() )
	{
		Fl::wait(0.001);
		igstk::PulseGenerator::CheckTimeouts();
	}

}

void MainWindow::checkCalibrationError()
{

    std::cout<<"Check Calibration Error"<<std::endl<<std::endl;

    if (!displayWidget->getImageStack().empty())
    {
      CheckCalibrationErrorWidget* checkCalibrationWidget = new CheckCalibrationErrorWidget();

      if (displayWidget->isImageStackLoaded)
        checkCalibrationWidget->setImageStack(displayWidget->getImageStack());
      else
        checkCalibrationWidget->setImage(displayWidget->getImageViewer()->GetInput());

	    displayWidget->setCalibrationErrorWidget(checkCalibrationWidget);
	  	displayWidget->setCheckCalibrationErrorFlag();
		displayWidget->startTracer();
	  
      checkCalibrationWidget->setMainWindow(this); 
      checkCalibrationWidget->show();
    }
    else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load images before checking the calibration");
      errorMessage.exec();
    }
}

void MainWindow::volumeReconstruction()
{

	std::cout<<"VOLUME RECONSTRUCTION"<<std::endl<<std::endl;
    if (!displayWidget->getVolumeImageStack().empty())
      {
       VolumeReconstructionWidget * volumeReconstruction = new VolumeReconstructionWidget();
		
		if (displayWidget->isVolumeImageStackLoaded){
           volumeReconstruction->setVolumeImageStack(displayWidget->getVolumeImageStack());
		   volumeReconstruction->setTransformStack(displayWidget->getTransformStack());
		}

        volumeReconstruction->setMainWindow(this);
        volumeReconstruction->show();
      }
    else
      {
        QErrorMessage errorMessage;
        errorMessage.showMessage(
            "No volume data loaded, </ br> please load data before reconstruct the volume");
        errorMessage.exec();
      }
	
}

void MainWindow::display3DScene()
{

	Scene3D* scene3D = new Scene3D();
	scene3D->init3DScene();
}


void MainWindow::cropImages()
{
    std::cout<<"Image Cropping"<<std::endl;

    if (!displayWidget->getImageStack().empty())
    {
      CropImagesWidget* cropImages = new CropImagesWidget();

      if (displayWidget->isImageStackLoaded)
        cropImages->setImageStack(displayWidget->getImageStack());
      else
        cropImages->setImage(displayWidget->getImageViewer()->GetInput());

      cropImages->setMainWindow(this);
      cropImages->show();
    }
    else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load images before crop images");
      errorMessage.exec();
    }
}

void MainWindow::segmentationTraining()
{
    std::cout<<"Segmentation Training"<<std::endl<<std::endl;

    if (!displayWidget->getImageStack().empty())
    {
      SegmentationTrainingWidget* segmentationTrainingWidget = new SegmentationTrainingWidget();

      if (displayWidget->isImageStackLoaded)
        segmentationTrainingWidget->setImageStack(displayWidget->getImageStack());
      else
          
        segmentationTrainingWidget->setImage(displayWidget->getImageViewer()->GetInput());

	displayWidget->setSegmentationTrainingWidget(segmentationTrainingWidget);
        displayWidget->setSegmentationTrainingFlag();
        displayWidget->startTracer();
	  
        segmentationTrainingWidget->setMainWindow(this); 
        segmentationTrainingWidget->show();
    }
    else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load images before checking the calibration");
      errorMessage.exec();
    }
}

void MainWindow::segmentImage()
{
    std::cout<<"Segmentation for 2D images"<<std::endl<<std::endl;
    
    
    if(this->imageLoaded)
    {
        ImageSegmentationWidget * imageSegmentationWidget = new ImageSegmentationWidget();
        
        displayWidget->setPickerFlag();
        imageSegmentationWidget->setImage(displayWidget->getImageViewer()->GetInput());
        
        Connections->Connect(displayWidget->getQVTKWidget()->GetRenderWindow()->GetInteractor(),
                           vtkCommand::LeftButtonPressEvent,
                           imageSegmentationWidget,
                           SLOT(getCoordinates()));
        
        imageSegmentationWidget->setMainWindow(this);
        imageSegmentationWidget->show();
        
    }else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load a image before segmentation");
      errorMessage.exec();
    }

}

void MainWindow::segmentVolume()
{
    std::cout<<"Segmentation for 2D images"<<std::endl<<std::endl;
    
    
    if(displayWidget->getVolumeLoaded())
    {
        VolumeSegmentationWidget * volumeSegmentationWidget = new VolumeSegmentationWidget();
        
        volumeSegmentationWidget->setVolume(displayWidget->getVolume());
        VTKThreeViews* threeViews = displayWidget->getVTKThreeViews();
        
        threeViews->connectWithVolumeSegmentation(volumeSegmentationWidget);
        volumeSegmentationWidget->setVTKThreeViews(threeViews);
        
        volumeSegmentationWidget->setMainWindow(this);
        volumeSegmentationWidget->show();
        
        
//        displayWidget->setPickerFlag();
//        imageSegmentationWidget->setImage(displayWidget->getImageViewer()->GetInput());
//        
//        Connections->Connect(displayWidget->getQVTKWidget()->GetRenderWindow()->GetInteractor(),
//                           vtkCommand::LeftButtonPressEvent,
//                           imageSegmentationWidget,
//                           SLOT(getCoordinates()));
//        
//        imageSegmentationWidget->setMainWindow(this);
//        imageSegmentationWidget->show();
    }else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load a image before segmentation");
      errorMessage.exec();
    }

}

void MainWindow::displaySelectedImage(int idx)
{
  this->displayWidget->displaySelectedImage(idx);
}


QVTKImageWidget* MainWindow::getDisplayWidget()
{
  return this->displayWidget;
}


void MainWindow::addLogText(QString str)
{
  ui->textEdit->append(str);

  // move the cursor to the end of the last line
  QTextCursor cursor = ui->textEdit->textCursor();
  cursor.movePosition(QTextCursor::End);
  ui->textEdit->setTextCursor(cursor);
}


void MainWindow::print()
{
  this->addLogText("estoy cachando el evento");
}
