#include "VolumeSegmentationWidget.h"
#include "ui_VolumeSegmentationWidget.h"

#include <vtkBMPWriter.h>


VolumeSegmentationWidget::VolumeSegmentationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeSegmentationWidget)
{
    ui->setupUi(this);
    
    ui->probabilityBtn->setEnabled(false);
    ui->segmentBtn->setEnabled(false);
    ui->saveBtn->setEnabled(false);
    ui->newSeedBtn->setEnabled(false);
    
    this->intensityProbsFlag = false;
    this->textureProbsFlag = false;
    this->seedPointFlag = false;
    this->probabilityVolumeFlag = false;
    
    this->vtkProbabilityVolume = vtkSmartPointer<vtkImageData>::New();
    this->vtkRegionGrowingVolume = vtkSmartPointer<vtkImageData>::New();
    this->vtkContourVolume = vtkSmartPointer<vtkImageData>::New();

	this->volumeSegmentation = VolumeSegmentation::New();
}

VolumeSegmentationWidget::~VolumeSegmentationWidget()
{
    delete ui;
    delete displayWidget;
}

void VolumeSegmentationWidget::loadIntensityProbs()
{

    QString intensityProbsFilename = QFileDialog::getOpenFileName(
                                                this, tr("Load Intensity Probs File"), 
                                        QDir::currentPath(), tr("Txt (*.txt *.doc)"));

    std::cout<<std::endl;
    std::cout<<"Loading Intensity Probs File"<<std::endl;

    if (!intensityProbsFilename.isEmpty())
    {

        QFile file(intensityProbsFilename);
        if (!file.open(QIODevice::ReadOnly)){
            std::cout<<"Could not open Intensity Probs File"<<std::endl;
             return;
         }
        
        QTextStream stream(&file);
        QString line;

        while (!stream.atEnd())
        {
            line = stream.readLine();

            this->intensityProbs.push_back(line.toFloat());
     
        }
        file.close();        
    }
    
	volumeSegmentation->setIntensityProbabilities(intensityProbs);

    this->intensityProbsFlag = true;
    
    if(textureProbsFlag)
        ui->probabilityBtn->setEnabled(true);
}

void VolumeSegmentationWidget::loadTextureProbs()
{

    QString textureProbsFilename = QFileDialog::getOpenFileName(
                                                this, tr("Load Texture Probs File"), 
                                        QDir::currentPath(), tr("Txt (*.txt *.doc)"));

    std::cout<<std::endl;
    std::cout<<"Loading Texture Probs File"<<std::endl;

    if (!textureProbsFilename.isEmpty())
    {

        QFile file(textureProbsFilename);
        if (!file.open(QIODevice::ReadOnly)){
            std::cout<<"Could not open Texture Probs File"<<std::endl;
             return;
         }
        
        QTextStream stream(&file);
        QString line;

        while (!stream.atEnd())
        {
            line = stream.readLine();

            this->textureProbs.push_back(line.toFloat());
     
        }
        file.close();        
    }
    
	volumeSegmentation->setTextureProbabilities(textureProbs);

    this->textureProbsFlag = true;
    
    if(intensityProbsFlag)
        ui->probabilityBtn->setEnabled(true);
}

void VolumeSegmentationWidget::computeProbability()
{

    this->volumeSegmentation->computeProbabilityVolume();

    this->vtkProbabilityVolume = volumeSegmentation->getProbabilityVolume();
    
    this->displayWidget = this->mainWindow->getDisplayWidget();
    this->displayWidget->setAndDisplayVolume(vtkProbabilityVolume);
    
    this->probabilityVolumeFlag = true;

    if(seedPointFlag)
        ui->segmentBtn->setEnabled(true);
}

void VolumeSegmentationWidget::segment()
{
    this->volumeSegmentation->setSeed(seedPoint);

    this->volumeSegmentation->computeRegionGrowing();

    this->vtkRegionGrowingVolume = volumeSegmentation->getRegionGrowingVolume();
    this->vtkContourVolume = volumeSegmentation->getContourVolume();
    this->contourPixels = volumeSegmentation->getContourPixels();
    
    this->displayWidget->setAndDisplayVolume(vtkContourVolume);
    
    ui->newSeedBtn->setEnabled(true);
    ui->saveBtn->setEnabled(true);
}

void VolumeSegmentationWidget::save()
{	
    QString probabilityVolumeFile = QFileDialog::getSaveFileName(
                this, tr("Save Probability Volume"),QDir::currentPath(),tr("BMP (*.bmp)"));

    std::string str1 = std::string(probabilityVolumeFile.toAscii().data());
    const char * saveProbabilityVolumeFile = str1.c_str();

    std::cout<<"Saving Probability volume in file: "<<std::endl<<std::endl;
    std::cout<<saveProbabilityVolumeFile<<std::endl<<std::endl;

    vtkSmartPointer<vtkBMPWriter> probabilityVolumeWriter = vtkSmartPointer<vtkBMPWriter>::New();
    probabilityVolumeWriter->SetFileName(saveProbabilityVolumeFile);
    probabilityVolumeWriter->SetInput(this->vtkProbabilityVolume);

    try{
	probabilityVolumeWriter->Write();
    }catch( std::exception& e){
        std::cout<<e.what()<<std::endl;
    }
    
    QString regionGrowingVolumeFile = QFileDialog::getSaveFileName(
                this, tr("Save RegionGrowing Volume"),QDir::currentPath(),tr("BMP (*.bmp)"));

    std::string str2 = std::string(regionGrowingVolumeFile.toAscii().data());
    const char * saveRegionGrowingVolumeFile = str2.c_str();

    std::cout<<"Saving RegionGrowing volume in file: "<<std::endl<<std::endl;
    std::cout<<saveRegionGrowingVolumeFile<<std::endl<<std::endl;

    vtkSmartPointer<vtkBMPWriter> regionGrowingVolumeWriter = vtkSmartPointer<vtkBMPWriter>::New();
    regionGrowingVolumeWriter->SetFileName(saveRegionGrowingVolumeFile);
    regionGrowingVolumeWriter->SetInput(this->vtkRegionGrowingVolume);

    try{
	regionGrowingVolumeWriter->Write();
    }catch( std::exception& e){
        std::cout<<e.what()<<std::endl;
    }
    
    QString contourVolumeFile = QFileDialog::getSaveFileName(
                this, tr("Save Contour Volume"),QDir::currentPath(),tr("BMP (*.bmp)"));

    std::string str3 = std::string(contourVolumeFile.toAscii().data());
    const char * saveContourVolumeFile = str3.c_str();

    std::cout<<"Saving Contour Volume in file: "<<std::endl<<std::endl;
    std::cout<<saveContourVolumeFile<<std::endl<<std::endl;

    vtkSmartPointer<vtkBMPWriter> contourVolumeWriter = vtkSmartPointer<vtkBMPWriter>::New();
    contourVolumeWriter->SetFileName(saveContourVolumeFile);
    contourVolumeWriter->SetInput(this->vtkContourVolume);

    try{
	contourVolumeWriter->Write();
    }catch( std::exception& e){
        std::cout<<e.what()<<std::endl;
    }
    
    QString saveContourPixelsFile = QFileDialog::getSaveFileName(
                this, tr("Choose File to Save Contour ixels"), QDir::currentPath(),tr("Txt (*.txt)"));

    QFile contourPixelsFile(saveContourPixelsFile);

    if(!contourPixelsFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        std::cout<<"Could not open File to save the contour pixels "<<std::endl;
        return;
    }

    std::cout<<std::endl;
    std::cout<<"Writing contour pixels"<<std::endl;

    QTextStream contourPixelsOut(&contourPixelsFile);

    for(unsigned int i=0;i<contourPixels.size();i++)
    {
        contourPixelsOut<<contourPixels.at(i)[0]<<" "<<contourPixels.at(i)[1]<<"\n";
    }
    
    contourPixelsFile.close();
}

void VolumeSegmentationWidget::setVolume(vtkSmartPointer<vtkImageData> volume)
{
	volumeSegmentation->setVolume(volume);
}

void VolumeSegmentationWidget::setMainWindow(MainWindow* mainWindow)
{
    this->mainWindow = mainWindow;
}

void VolumeSegmentationWidget::setVTKThreeViews(VTKThreeViews* threeViews)
{
    this->threeViews = threeViews;
}

void VolumeSegmentationWidget::getCoordinates()
{
    std::vector<int>* pickedCoordinates = threeViews->getPickedCoordinates(); 
    
    this->seedPoint[0] = pickedCoordinates->at(0);
    this->seedPoint[1] = pickedCoordinates->at(1);
    this->seedPoint[2] = pickedCoordinates->at(2);
    
    QString str;

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(str.setNum(seedPoint[0])));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(str.setNum(seedPoint[1])));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(str.setNum(seedPoint[2])));
    //std::cout << "Seed Point -> (x = " << seedPoint[0] << ", y = " << seedPoint[1] << ")" << std::endl;
    
    this->seedPointFlag = true;
    
    if(probabilityVolumeFlag)
        ui->segmentBtn->setEnabled(true);

}
    
void VolumeSegmentationWidget::newSeed()
{
    this->seedPoint[0] = 0;
    this->seedPoint[1] = 0;

    this->displayWidget->setAndDisplayVolume(vtkProbabilityVolume);
    
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(0));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(0));
    
    ui->segmentBtn->setEnabled(false);
    ui->saveBtn->setEnabled(false);
    
}