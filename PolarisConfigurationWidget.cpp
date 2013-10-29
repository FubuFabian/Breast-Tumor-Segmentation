#include "PolarisConfigurationWidget.h"
#include "ui_PolarisConfigurationWidget.h"

#include <QFileDialog>
#include <QTextStream>

#include <iostream>

PolarisConfigurationWidget::PolarisConfigurationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PolarisConfigurationWidget)
{
    ui->setupUi(this);


	ui->referenceRomText->setText("Data/Reference.rom");
	ui->probeRomText->setText("Data/USProbe.rom");
	ui->needleRomText->setText("Data/Needle.rom");
	ui->pointerRomText->setText("Data/Pointer.rom");

	ui->probeCalibrationText->setText("Data/USProbeCalibrationParameters.txt");
	ui->needleCalibrationText->setText("Data/NeedleCalibrationParameters.txt");
	ui->pointerCalibrationText->setText("Data/PointerCalibrationParameters.txt");

	ui->polarisPort->setCurrentIndex(0);
	quit = false;
}

PolarisConfigurationWidget::~PolarisConfigurationWidget()
{
    delete ui;
}

void PolarisConfigurationWidget::chooseReferenceRom()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Reference Tool Rom"),
        QDir::currentPath(),tr("Rom Files (*.rom)"));

	ui->referenceRomText->setText(filename);

}

void PolarisConfigurationWidget::chooseProbeRom()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Ultrasound Probe Tool Rom"),
        QDir::currentPath(),tr("Rom Files (*.rom)"));

	ui->probeRomText->setText(filename);
}

void PolarisConfigurationWidget::chooseNeedleRom()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Needle Tool Rom"),
        QDir::currentPath(),tr("Rom Files (*.rom)"));

	ui->needleRomText->setText(filename);

}

void PolarisConfigurationWidget::choosePointerRom()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Pointer Tool Rom"),
        QDir::currentPath(),tr("Rom Files (*.rom)"));

	ui->pointerRomText->setText(filename);
}

void PolarisConfigurationWidget::chooseProbeCalibration()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Probe Calibration File"),
        QDir::currentPath(),tr("Text (*.txt)"));

	ui->probeCalibrationText->setText(filename);
}

void PolarisConfigurationWidget::chooseNeedleCalibration()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Needle Calibration File"),
        QDir::currentPath(),tr("Text (*.txt)"));

	ui->needleCalibrationText->setText(filename);
}

void PolarisConfigurationWidget::choosePointerCalibration()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Pointer Calibration File"),
        QDir::currentPath(),tr("Text (*.txt)"));

	ui->pointerCalibrationText->setText(filename);
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

	files.push_back(ui->referenceRomText->text());
	files.push_back(ui->probeRomText->text());
	files.push_back(ui->needleRomText->text());
	files.push_back(ui->pointerRomText->text());
	files.push_back(ui->probeCalibrationText->text());
	files.push_back(ui->needleCalibrationText->text());
	files.push_back(ui->pointerCalibrationText->text());

	return files;
}

int PolarisConfigurationWidget::getPort()
{
	return ui->polarisPort->currentIndex();
}

void PolarisConfigurationWidget::loadConfig()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Polaris Configuration File"),
        QDir::currentPath(),tr("Text (*.txt)"));
    
    std::vector<QString> filenames;
    filenames.reserve(7);
    int port;
    
    if (!filename.isEmpty())
    {

        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
            return;

        QTextStream stream(&file);
        QString line;
        
        line = stream.readLine();
        port = line.toInt();

        while (!stream.atEnd())
        {
             line = stream.readLine();
             filenames.push_back(line);
         }
             file.close();
     }
    
    
	ui->referenceRomText->setText(filenames[0]);
	ui->probeRomText->setText(filenames[1]);
	ui->needleRomText->setText(filenames[2]);
	ui->pointerRomText->setText(filenames[3]);

	ui->probeCalibrationText->setText(filenames[4]);
	ui->needleCalibrationText->setText(filenames[5]);
	ui->pointerCalibrationText->setText(filenames[6]);

	ui->polarisPort->setCurrentIndex(port);  
    
}

void PolarisConfigurationWidget::saveConfig()
{
    
    QString filename = QFileDialog::getSaveFileName(
                this, tr("Choose Directory to Save Configuration"), QDir::currentPath(),tr("Txt (*.txt)"));

    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        std::cout<<"Could not open File to save the configuration"<<std::endl;
        return;
    }

	std::cout<<std::endl;
	std::cout<<"Writing Polaris Configuration"<<std::endl;

	QTextStream out(&file);
        
        out<<ui->polarisPort->currentIndex()<<"\n";
	out<<ui->referenceRomText->text()<<"\n";
	out<<ui->probeRomText->text()<<"\n";
	out<<ui->needleRomText->text()<<"\n";
	out<<ui->pointerRomText->text()<<"\n";
	out<<ui->probeCalibrationText->text()<<"\n";
	out<<ui->needleCalibrationText->text()<<"\n";
	out<<ui->pointerCalibrationText->text();


	file.close();
    
}