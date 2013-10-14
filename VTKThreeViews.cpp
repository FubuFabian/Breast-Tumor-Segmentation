#include "VTKThreeViews.h"
#include "ui_VTKThreeViews.h"
#include "VolumeSegmentationWidget.h"

#include <QVBoxLayout>

#include <vtkMetaImageReader.h>
#include <vtkImageData.h>
#include <vtkEventQtSlotConnect.h>


VTKThreeViews::VTKThreeViews(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VTKThreeViews)
{
	ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);

	this->axialDisplayWidget = new QVTKVolumeSliceWidget();
        this->axialDisplayWidget->setVTKThreeViews(this);
        
	QVBoxLayout * axialLayout = new QVBoxLayout;
	axialLayout->setContentsMargins(0, 0, 0, 0);
	axialLayout->setSpacing(0);
	axialLayout->QLayout::addWidget(axialDisplayWidget);
	ui->axialDisplay->setLayout(axialLayout);

	this->sagittalDisplayWidget = new QVTKVolumeSliceWidget();
        this->sagittalDisplayWidget->setVTKThreeViews(this);

	QVBoxLayout * sagittalLayout = new QVBoxLayout;
	sagittalLayout->setContentsMargins(0, 0, 0, 0);
	sagittalLayout->setSpacing(0);
	sagittalLayout->QLayout::addWidget(sagittalDisplayWidget);
	ui->sagittalDisplay->setLayout(sagittalLayout);

	this->coronalDisplayWidget = new QVTKVolumeSliceWidget();
        this->coronalDisplayWidget->setVTKThreeViews(this);
              

	QVBoxLayout * coronalLayout = new QVBoxLayout;
	coronalLayout->setContentsMargins(0, 0, 0, 0);
	coronalLayout->setSpacing(0);
	coronalLayout->QLayout::addWidget(coronalDisplayWidget);
	ui->coronalDisplay->setLayout(coronalLayout);
        
        this->pickedCoordinates = new std::vector<int>;
        this->pickedCoordinates->reserve(3);
        this->pickedCoordinates->assign(3,0);
    
}

VTKThreeViews::~VTKThreeViews()
{
    delete ui;
}

void VTKThreeViews::axialSliderMove(int slice)
{
	axialDisplayWidget->displayVolumeSlice(slice);
}

void VTKThreeViews::axialRotate()
{
    this->axialDisplayWidget->rotateCamera();
}

void VTKThreeViews::axialHorizontalFlip()
{
    this->axialDisplayWidget->setHorizontalFlipFlag();
}

void VTKThreeViews::axialVerticalFlip()
{
    this->axialDisplayWidget->setVerticalFlipFlag();
}

void VTKThreeViews::sagittalSliderMove(int slice)
{
	sagittalDisplayWidget->displayVolumeSlice(slice);
}

void VTKThreeViews::sagittalRotate()
{
    this->sagittalDisplayWidget->rotateCamera();
}

void VTKThreeViews::sagittalHorizontalFlip()
{
    this->sagittalDisplayWidget->setHorizontalFlipFlag();
}

void VTKThreeViews::sagittalVerticalFlip()
{
    this->sagittalDisplayWidget->setVerticalFlipFlag();
}

void VTKThreeViews::coronalSliderMove(int slice)
{
	coronalDisplayWidget->displayVolumeSlice(slice);	
}

void VTKThreeViews::coronalRotate()
{
    this->coronalDisplayWidget->rotateCamera();
}

void VTKThreeViews::coronalHorizontalFlip()
{
    this->coronalDisplayWidget->setHorizontalFlipFlag();
}

void VTKThreeViews::coronalVerticalFlip()
{
    this->coronalDisplayWidget->setVerticalFlipFlag();
}

void VTKThreeViews::setPickedCoordinates(int xPicked, int yPicked, int zPicked)
{
    this->pickedCoordinates->at(0) = xPicked;
    this->pickedCoordinates->at(1) = yPicked;
    this->pickedCoordinates->at(2) = zPicked;
}

std::vector<int>* VTKThreeViews::getPickedCoordinates()
{
    return pickedCoordinates;
}

void VTKThreeViews::setVolumeData(vtkSmartPointer<vtkImageData> volumeData)
{
    int size[3];
	
	volumeData->GetDimensions(size);
	
	size[0] = size[0]-1;
	size[1] = size[1]-1;
	size[2] = size[2]-1;

	int centerSlice[3];
	centerSlice[0] = floor(size[0]*0.5);
	centerSlice[1] = floor(size[1]*0.5);
	centerSlice[2] = floor(size[2]*0.5);

	ui->sagittalSlider->setRange(0,size[0]);
	ui->coronalSlider->setRange(0,size[1]);
	ui->axialSlider->setRange(0,size[2]);

	ui->sagittalSlider->setTickInterval(1);
	ui->coronalSlider->setTickInterval(1);
	ui->axialSlider->setTickInterval(1);

	ui->sagittalSlider->setValue(centerSlice[0]);
	ui->coronalSlider->setValue(centerSlice[1]);
	ui->axialSlider->setValue(centerSlice[2]);

	sagittalDisplayWidget->setVolumeData(volumeData);
	coronalDisplayWidget->setVolumeData(volumeData);
	axialDisplayWidget->setVolumeData(volumeData);

	sagittalDisplayWidget->setSagittalView();
	coronalDisplayWidget->setCoronalView();
	axialDisplayWidget->setAxialView();

	sagittalDisplayWidget->displayVolumeSlice(centerSlice[0]);
	coronalDisplayWidget->displayVolumeSlice(centerSlice[1]);
	axialDisplayWidget->displayVolumeSlice(centerSlice[2]);
}

void VTKThreeViews::connectWithVolumeSegmentation(VolumeSegmentationWidget* volumeSegmentation)
{
    
    axialConnections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    
    axialConnections->Connect(axialDisplayWidget->getQVTKWidget()->GetRenderWindow()->GetInteractor(),
                           vtkCommand::LeftButtonPressEvent,
                           volumeSegmentation,
                           SLOT(getCoordinates()));
    
    sagittalConnections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    
    sagittalConnections->Connect(sagittalDisplayWidget->getQVTKWidget()->GetRenderWindow()->GetInteractor(),
                           vtkCommand::LeftButtonPressEvent,
                           volumeSegmentation,
                           SLOT(getCoordinates()));
    
    coronalConnections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    
    coronalConnections->Connect(coronalDisplayWidget->getQVTKWidget()->GetRenderWindow()->GetInteractor(),
                           vtkCommand::LeftButtonPressEvent,
                           volumeSegmentation,
                           SLOT(getCoordinates()));
        
}



