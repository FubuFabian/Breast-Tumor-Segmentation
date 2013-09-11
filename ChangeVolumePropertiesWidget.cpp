#include "ChangeVolumePropertiesWidget.h"
#include "ui_ChangeVolumePropertiesWidget.h"

#include <vtkSmartPointer.h>
#include <vtkPiecewiseFunction.h>

ChangeVolumePropertiesWidget::ChangeVolumePropertiesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeVolumePropertiesWidget)
{
    ui->setupUi(this);

	ui->lowerThresholdSlider->setTickInterval(1);
	ui->lowerThresholdSlider->setRange(0,255);
	
	ui->upperThresholdSlider->setTickInterval(1);
	ui->upperThresholdSlider->setRange(0,255);
	ui->upperThresholdSlider->setValue(255);

	ui->opacitySlider->setTickInterval(1);
	ui->opacitySlider->setRange(0,100);
	ui->opacitySlider->setValue(100);

	ui->lowerThresholdDisplay->setText("0");
	ui->upperThresholdDisplay->setText("255");

	volumeScalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	opacityVal = 1.0;
	
	QVTKImage = true;
}

ChangeVolumePropertiesWidget::~ChangeVolumePropertiesWidget()
{
    delete ui;
}

void ChangeVolumePropertiesWidget::changeLowerThreshold(int tickValue)
{
	

	if (tickValue<ui->upperThresholdSlider->value()){
		lowerThreshVal = tickValue;
		upperThreshVal = ui->upperThresholdSlider->value();
	}else{
		lowerThreshVal = ui->upperThresholdSlider->value();
		upperThreshVal = tickValue;
	}
		
	ui->lowerThresholdDisplay->setText(QString::number(lowerThreshVal));
	ui->upperThresholdDisplay->setText(QString::number(upperThreshVal));

	middlePoint = lowerThreshVal + floor((upperThreshVal - lowerThreshVal)*0.5);
	
	volumeScalarOpacity->RemoveAllPoints();
	volumeScalarOpacity->AddPoint(lowerThreshVal,0.0);
	volumeScalarOpacity->AddPoint(middlePoint,opacityVal);
	volumeScalarOpacity->AddPoint(upperThreshVal,0.0);
    volumeScalarOpacity->Update();
	
	if(QVTKImage)
		qvtkImageWidget->setVolumeOpacity(volumeScalarOpacity);
	else
		igstkImageRepresentation->changeVolumeOpacity(volumeScalarOpacity);
}

void ChangeVolumePropertiesWidget::changeUpperThreshold(int tickValue)
{
	if (tickValue<ui->lowerThresholdSlider->value()){
		lowerThreshVal = tickValue;
		upperThreshVal = ui->lowerThresholdSlider->value();
	}else{
		lowerThreshVal = ui->lowerThresholdSlider->value();
		upperThreshVal = tickValue;
	}

	ui->lowerThresholdDisplay->setText(QString::number(lowerThreshVal));
	ui->upperThresholdDisplay->setText(QString::number(upperThreshVal));

	middlePoint = lowerThreshVal + floor((upperThreshVal - lowerThreshVal)*0.5);
 
    volumeScalarOpacity->RemoveAllPoints();
	volumeScalarOpacity->AddPoint(lowerThreshVal,0.0);
	volumeScalarOpacity->AddPoint(middlePoint,opacityVal);
	volumeScalarOpacity->AddPoint(upperThreshVal,0.0);
    volumeScalarOpacity->Update();

	if(QVTKImage)
		qvtkImageWidget->setVolumeOpacity(volumeScalarOpacity);
	else
		igstkImageRepresentation->changeVolumeOpacity(volumeScalarOpacity);
}


void ChangeVolumePropertiesWidget::changeOpacity(int tickValue)
{

	opacityVal = tickValue*0.01;

    volumeScalarOpacity->RemoveAllPoints();
	volumeScalarOpacity->AddPoint(lowerThreshVal,0.0);
	volumeScalarOpacity->AddPoint(middlePoint,opacityVal);
	volumeScalarOpacity->AddPoint(upperThreshVal,0.0);
    volumeScalarOpacity->Update();

	if(QVTKImage)
		qvtkImageWidget->setVolumeOpacity(volumeScalarOpacity);
	else
		igstkImageRepresentation->changeVolumeOpacity(volumeScalarOpacity);
}

void ChangeVolumePropertiesWidget::setQVTKImageWidget(QVTKImageWidget *qvtkImageWidget)
{
	this->qvtkImageWidget = qvtkImageWidget;
	QVTKImage =  true;
}

void ChangeVolumePropertiesWidget::
setigstkImageSpatialObjectVolumeRepresentation(
	igstk::ImageSpatialObjectVolumeRepresentation<igstk::USImageObject>::Pointer igstkImageRepresentation)
{
	this->igstkImageRepresentation = igstkImageRepresentation;
	QVTKImage =  false;
}
