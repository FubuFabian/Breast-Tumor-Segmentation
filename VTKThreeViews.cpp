#include "VTKThreeViews.h"
#include "ui_VTKThreeViews.h"

#include <QVBoxLayout>
#include <vtkMetaImageReader.h>
#include <vtkImageData.h>


VTKThreeViews::VTKThreeViews( vtkSmartPointer<vtkImageData> volumeData, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VTKThreeViews)
{

	ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);

	this->axialDisplayWidget = new QVTKVolumeSliceWidget();

	QVBoxLayout * axialLayout = new QVBoxLayout;
	axialLayout->setContentsMargins(0, 0, 0, 0);
	axialLayout->setSpacing(0);
	axialLayout->QLayout::addWidget(axialDisplayWidget);
	ui->axialDisplay->setLayout(axialLayout);

	this->sagittalDisplayWidget = new QVTKVolumeSliceWidget();

	QVBoxLayout * sagittalLayout = new QVBoxLayout;
	sagittalLayout->setContentsMargins(0, 0, 0, 0);
	sagittalLayout->setSpacing(0);
	sagittalLayout->QLayout::addWidget(sagittalDisplayWidget);
	ui->sagittalDisplay->setLayout(sagittalLayout);

	this->coronalDisplayWidget = new QVTKVolumeSliceWidget();

	QVBoxLayout * coronalLayout = new QVBoxLayout;
	coronalLayout->setContentsMargins(0, 0, 0, 0);
	coronalLayout->setSpacing(0);
	coronalLayout->QLayout::addWidget(coronalDisplayWidget);
	ui->coronalDisplay->setLayout(coronalLayout);

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

VTKThreeViews::~VTKThreeViews()
{
    delete ui;
}

void VTKThreeViews::axialSliderMove(int slice)
{
	axialDisplayWidget->displayVolumeSlice(slice);
}

void VTKThreeViews::sagittalSliderMove(int slice)
{
	sagittalDisplayWidget->displayVolumeSlice(slice);
}


void VTKThreeViews::coronalSliderMove(int slice)
{
	coronalDisplayWidget->displayVolumeSlice(slice);	
}

