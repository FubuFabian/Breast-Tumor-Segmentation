#include "PolarisConfigurationWidget.h"
#include "ui_PolarisConfigurationWidget.h"

#include <QFileDialog>

PolarisConfigurationWidget::PolarisConfigurationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PolarisConfigurationWidget)
{
    ui->setupUi(this);

	referenceRomFile = "Data/Reference.rom";
	probeRomFile = "Data/USProbe.rom";
	needleRomFile = "Data/Needle.rom";
	pointerRomFile = "Data/Pointer.rom";

	probeCalibrationFile = "Data/USProbeCalibrationParameters.txt";
	needleCalibrationFile = "Data/NeedleCalibrationParameters.txt";
	pointerCalibrationFile = "Data/PointerCalibrationParameters.txt"; 

	ui->referenceRomText->setText(referenceRomFile);
	ui->probeRomText->setText(probeRomFile);
	ui->needleRomText->setText(needleRomFile);
	ui->pointerRomText->setText(pointerRomFile);

	ui->probeCalibrationText->setText(probeCalibrationFile);
	ui->needleCalibrationText->setText(needleCalibrationFile);
	ui->pointerCalibrationText->setText(pointerCalibrationFile);

	ui->polarisPort->setCurrentIndex(2);
	quit = false;
}

PolarisConfigurationWidget::~PolarisConfigurationWidget()
{
    delete ui;
}

void PolarisConfigurationWidget::chooseReferenceRom()
{
	referenceRomFile = QFileDialog::getOpenFileName(this, tr("Open Reference Tool Rom"),
        QDir::currentPath(),tr("Rom Files (*.rom)"));

	ui->referenceRomText->setText(referenceRomFile);

}

void PolarisConfigurationWidget::chooseProbeRom()
{
	probeRomFile = QFileDialog::getOpenFileName(this, tr("Open Ultrasound Probe Tool Rom"),
        QDir::currentPath(),tr("Rom Files (*.rom)"));

	ui->probeRomText->setText(probeRomFile);
}

void PolarisConfigurationWidget::chooseNeedleRom()
{
	needleRomFile = QFileDialog::getOpenFileName(this, tr("Open Needle Tool Rom"),
        QDir::currentPath(),tr("Rom Files (*.rom)"));

	ui->needleRomText->setText(needleRomFile);

}

void PolarisConfigurationWidget::choosePointerRom()
{
	pointerRomFile = QFileDialog::getOpenFileName(this, tr("Open Pointer Tool Rom"),
        QDir::currentPath(),tr("Rom Files (*.rom)"));

	ui->pointerRomText->setText(pointerRomFile);
}

void PolarisConfigurationWidget::chooseProbeCalibration()
{
	probeCalibrationFile = QFileDialog::getOpenFileName(this, tr("Open Probe Calibration File"),
        QDir::currentPath(),tr("Text (*.txt)"));

	ui->probeCalibrationText->setText(probeCalibrationFile);
}

void PolarisConfigurationWidget::chooseNeedleCalibration()
{
	needleCalibrationFile = QFileDialog::getOpenFileName(this, tr("Open Needle Calibration File"),
        QDir::currentPath(),tr("Text (*.txt)"));

	ui->needleCalibrationText->setText(needleCalibrationFile);
}

void PolarisConfigurationWidget::choosePointerCalibration()
{
	pointerCalibrationFile = QFileDialog::getOpenFileName(this, tr("Open Pointer Calibration File"),
        QDir::currentPath(),tr("Text (*.txt)"));

	ui->pointerCalibrationText->setText(pointerCalibrationFile);
}

void PolarisConfigurationWidget::ok()
{
	this->hide();
	quit = true;
}

bool PolarisConfigurationWidget::hasQuitted()
{
	return quit;
}

std::vector<QString> PolarisConfigurationWidget::getFiles()
{
	std::vector<QString> files;
	files.reserve(7);

	files.push_back(referenceRomFile);
	files.push_back(probeRomFile);
	files.push_back(needleRomFile);
	files.push_back(pointerRomFile);
	files.push_back(probeCalibrationFile);
	files.push_back(needleCalibrationFile);
	files.push_back(pointerCalibrationFile);

	return files;
}

int PolarisConfigurationWidget::getPort()
{
	return ui->polarisPort->currentIndex();
}