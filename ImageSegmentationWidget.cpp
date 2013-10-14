#include "ImageSegmentationWidget.h"
#include "ui_ImageSegmentationWidget.h"

#include <vtkBMPWriter.h>


ImageSegmentationWidget::ImageSegmentationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageSegmentationWidget)
{
    ui->setupUi(this);
    
    ui->probabilityBtn->setEnabled(false);
    ui->segmentBtn->setEnabled(false);
    ui->saveBtn->setEnabled(false);
    ui->newSeedBtn->setEnabled(false);
    
    this->intensityProbsFlag = false;
    this->textureProbsFlag = false;
    this->seedPointFlag = false;
    this->probabilityImageFlag = false;
    
    this->vtkProbabilityImage = vtkSmartPointer<vtkImageData>::New();
    this->vtkRegionGrowingImage = vtkSmartPointer<vtkImageData>::New();
    this->vtkContourImage = vtkSmartPointer<vtkImageData>::New();

	this->imageSegmentation = ImageSegmentation::New();
}

ImageSegmentationWidget::~ImageSegmentationWidget()
{
    delete ui;
    delete displayWidget;
}

void ImageSegmentationWidget::loadIntensityProbs()
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
    
	imageSegmentation->setIntensityProbabilities(intensityProbs);

    this->intensityProbsFlag = true;
    
    if(textureProbsFlag)
        ui->probabilityBtn->setEnabled(true);
}

void ImageSegmentationWidget::loadTextureProbs()
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
    
	imageSegmentation->setTextureProbabilities(textureProbs);

    this->textureProbsFlag = true;
    
    if(intensityProbsFlag)
        ui->probabilityBtn->setEnabled(true);
}

void ImageSegmentationWidget::computeProbability()
{

    this->imageSegmentation->computeProbabilityImage();

    this->vtkProbabilityImage = imageSegmentation->getProbabilityImage();
    
    this->displayWidget = this->mainWindow->getDisplayWidget();
    this->displayWidget->setAndDisplayImage(vtkProbabilityImage);
    
    this->probabilityImageFlag = true;

    if(seedPointFlag)
        ui->segmentBtn->setEnabled(true);
}

void ImageSegmentationWidget::segment()
{
    this->imageSegmentation->setSeed(seedPoint);

    this->imageSegmentation->computeRegionGrowing();

    this->vtkRegionGrowingImage = imageSegmentation->getRegionGrowingImage();
    this->vtkContourImage = imageSegmentation->getContourImage();
    this->contourPixels = imageSegmentation->getContourPixels();
    
    this->displayWidget->setAndDisplayImage(vtkContourImage);
    
    ui->newSeedBtn->setEnabled(true);
    ui->saveBtn->setEnabled(true);
}

void ImageSegmentationWidget::save()
{	
    QString probabilityImageFile = QFileDialog::getSaveFileName(
                this, tr("Save Probability Image"),QDir::currentPath(),tr("BMP (*.bmp)"));

    std::string str1 = std::string(probabilityImageFile.toAscii().data());
    const char * saveProbabilityImageFile = str1.c_str();

    std::cout<<"Saving Probability image in file: "<<std::endl<<std::endl;
    std::cout<<saveProbabilityImageFile<<std::endl<<std::endl;

    vtkSmartPointer<vtkBMPWriter> probabilityImageWriter = vtkSmartPointer<vtkBMPWriter>::New();
    probabilityImageWriter->SetFileName(saveProbabilityImageFile);
    probabilityImageWriter->SetInput(this->vtkProbabilityImage);

    try{
	probabilityImageWriter->Write();
    }catch( std::exception& e){
        std::cout<<e.what()<<std::endl;
    }
    
    QString regionGrowingImageFile = QFileDialog::getSaveFileName(
                this, tr("Save RegionGrowing Image"),QDir::currentPath(),tr("BMP (*.bmp)"));

    std::string str2 = std::string(regionGrowingImageFile.toAscii().data());
    const char * saveRegionGrowingImageFile = str2.c_str();

    std::cout<<"Saving RegionGrowing image in file: "<<std::endl<<std::endl;
    std::cout<<saveRegionGrowingImageFile<<std::endl<<std::endl;

    vtkSmartPointer<vtkBMPWriter> regionGrowingImageWriter = vtkSmartPointer<vtkBMPWriter>::New();
    regionGrowingImageWriter->SetFileName(saveRegionGrowingImageFile);
    regionGrowingImageWriter->SetInput(this->vtkRegionGrowingImage);

    try{
	regionGrowingImageWriter->Write();
    }catch( std::exception& e){
        std::cout<<e.what()<<std::endl;
    }
    
    QString contourImageFile = QFileDialog::getSaveFileName(
                this, tr("Save Contour Image"),QDir::currentPath(),tr("BMP (*.bmp)"));

    std::string str3 = std::string(contourImageFile.toAscii().data());
    const char * saveContourImageFile = str3.c_str();

    std::cout<<"Saving Contour image in file: "<<std::endl<<std::endl;
    std::cout<<saveContourImageFile<<std::endl<<std::endl;

    vtkSmartPointer<vtkBMPWriter> contourImageWriter = vtkSmartPointer<vtkBMPWriter>::New();
    contourImageWriter->SetFileName(saveContourImageFile);
    contourImageWriter->SetInput(this->vtkContourImage);

    try{
	contourImageWriter->Write();
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

void ImageSegmentationWidget::setImage(vtkSmartPointer<vtkImageData> image)
{
	imageSegmentation->setImage(image);
}

void ImageSegmentationWidget::setMainWindow(MainWindow* mainWindow)
{
    this->mainWindow = mainWindow;
}

void ImageSegmentationWidget::getCoordinates()
{
    std::cout<<std::endl;
    this->seedPoint[0] = mainWindow->getDisplayWidget()->getXPicked();
    this->seedPoint[1] = mainWindow->getDisplayWidget()->getYPicked();
    
    QString str;

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(str.setNum(seedPoint[0])));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(str.setNum(seedPoint[1])));
    std::cout << "Seed Point -> (x = " << seedPoint[0] << ", y = " << seedPoint[1] << ")" << std::endl;
    
    this->seedPointFlag = true;
    
    if(probabilityImageFlag)
        ui->segmentBtn->setEnabled(true);

}
    
void ImageSegmentationWidget::newSeed()
{
    this->seedPoint[0] = 0;
    this->seedPoint[1] = 0;

    this->displayWidget->setAndDisplayImage(vtkProbabilityImage);
    
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(0));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(0));
    
    ui->segmentBtn->setEnabled(false);
    ui->saveBtn->setEnabled(false);
    
}