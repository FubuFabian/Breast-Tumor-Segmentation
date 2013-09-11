#include "ImageSegmentationWidget.h"
#include "ui_ImageSegmentationWidget.h"
#include "SegmentationIntensityAndTextureImages.h"
#include "itkImageToVTKImageFilter.h"
#include "itkVTKImageToImageFilter.h"

#include <itkImageRegionIterator.h>
#include <itkImageDuplicator.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkNeighborhoodIterator.h>
#include <itkGrayscaleFillholeImageFilter.h>
#include <itkBinaryContourImageFilter.h>

#include <vtkImageLuminance.h>
#include <vtkBMPWriter.h>

#include <math.h>


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
    
    this->textureProbsFlag = true;
    
    if(intensityProbsFlag)
        ui->probabilityBtn->setEnabled(true);
}

void ImageSegmentationWidget::computeProbability()
{
    
    
    this->itkImage = this->convertToITKImage(vtkImage);
        
    SegmentationIntensityAndTextureImages<FloatImageType> * preprocessedImages =
                        SegmentationIntensityAndTextureImages<FloatImageType>::New();
    
    preprocessedImages->setInput(itkImage);
            
    std::cout<<std::endl<<"Obtaining intensity image"<<std::endl;
    FloatImageType::Pointer itkIntensityImage = preprocessedImages->getIntensityImage();
            
    CastFilterType::Pointer castIntensityFilter = CastFilterType::New();
    castIntensityFilter->SetInput(itkIntensityImage);
    castIntensityFilter->Update(); 
    
    ImageType::Pointer itkIntensityCastImage = castIntensityFilter->GetOutput();
    
    std::cout<<"Obtaining texture image"<<std::endl<<std::endl;
    FloatImageType::Pointer itkTextureImage = preprocessedImages->getTextureImage();
    
    CastFilterType::Pointer castTextureFilter = CastFilterType::New();
    castTextureFilter->SetInput(itkTextureImage);
    castTextureFilter->Update(); 
    
    ImageType::Pointer itkTextureCastImage = castTextureFilter->GetOutput();
    
    FloatImageType::Pointer itkProbabilityImage = FloatImageType::New();
    itkProbabilityImage->SetRegions(itkImage->GetLargestPossibleRegion());
    itkProbabilityImage->Allocate();
    
    typedef itk::ImageRegionIterator<ImageType>  ImageRegionIteratorType;
    typedef itk::ImageRegionIterator<FloatImageType>  FloatImageRegionIteratorType;
    
    ImageRegionIteratorType itIntensity(itkIntensityCastImage,
          itkIntensityCastImage->GetLargestPossibleRegion());
    itIntensity.GoToBegin();


    ImageRegionIteratorType itTexture(itkTextureCastImage,
          itkTextureCastImage->GetLargestPossibleRegion());
    itTexture.GoToBegin(); 
    
    FloatImageRegionIteratorType itOut(itkProbabilityImage, 
          itkProbabilityImage->GetLargestPossibleRegion());
    itOut.GoToBegin();
    
    while(!itOut.IsAtEnd())
    {
        float intensityValue = intensityProbs.at(itIntensity.Value());
        float textureValue = textureProbs.at(itTexture.Value());
        
        float probability = intensityValue*textureValue;
        
        itkProbabilityImage->SetPixel(itOut.GetIndex(),probability);
        
        ++itOut;
        ++itIntensity;
        ++itTexture;
    }     
    
    typedef itk::RescaleIntensityImageFilter<FloatImageType, FloatImageType> 
                                                              RescaleFilterType;
    RescaleFilterType::Pointer normalizeFilter = RescaleFilterType::New();
    normalizeFilter->SetOutputMaximum(255);
    normalizeFilter->SetOutputMinimum(0);
    normalizeFilter->SetInput(itkProbabilityImage);
    normalizeFilter->Update();   
    
    this->probabilityImage = normalizeFilter->GetOutput();
    
    this->vtkProbabilityImage = this->convertToVTKImage(probabilityImage);
    
    this->displayWidget = this->mainWindow->getDisplayWidget();
    this->displayWidget->setAndDisplayImage(vtkProbabilityImage);
    
    this->probabilityImageFlag = true;

    if(seedPointFlag)
        ui->segmentBtn->setEnabled(true);
}

void ImageSegmentationWidget::segment()
{

    this->contourPixels.clear();
    
    typedef itk::ConnectedThresholdImageFilter<FloatImageType,FloatImageType> RegionGrowingType;
    RegionGrowingType::Pointer regionGrowing = RegionGrowingType::New();
    
    regionGrowing->SetInput(probabilityImage);
    
    FloatImageType::SizeType imageSize;
    imageSize = probabilityImage->GetLargestPossibleRegion().GetSize();
    
    seedPoint[1] = imageSize[1] - seedPoint[1]; 
    
    regionGrowing->SetSeed(seedPoint);
    regionGrowing->SetReplaceValue(255);
    
    typedef itk::NeighborhoodIterator<FloatImageType> NeighborhoodIterator;
    NeighborhoodIterator::RadiusType radius;
    radius.Fill(1);
    
    FloatImageType::RegionType seedRegion;
    
    FloatImageType::RegionType::IndexType seedRegionIndex;
    seedRegionIndex[0] = seedPoint[0] - 1;
    seedRegionIndex[1] = seedPoint[1] - 1;
    seedRegion.SetIndex(seedRegionIndex);
    
    FloatImageType::RegionType::SizeType seedRegionSize;
    seedRegionSize.Fill(3); 
    
    NeighborhoodIterator it(radius,probabilityImage,seedRegion);
    
    float accum = 0;
    for (unsigned int i = 0; i < it.Size(); ++i){    
        accum += it.GetPixel(i);  
    }
    
    float seedRegionMean = accum/(float)it.Size();
    
    float upperThreshold = seedRegionMean + seedRegionMean*0.55;
    float lowerThreshold = seedRegionMean - seedRegionMean*0.55;
    
    regionGrowing->SetUpper(upperThreshold);
    regionGrowing->SetLower(lowerThreshold);

    regionGrowing->Update();
    
    typedef itk::GrayscaleFillholeImageFilter<FloatImageType,FloatImageType> FillHolesType;
    FillHolesType::Pointer fillHoles = FillHolesType::New();
    fillHoles->SetInput(regionGrowing->GetOutput());
    fillHoles->Update();
    
    this->vtkRegionGrowingImage = this->convertToVTKImage(fillHoles->GetOutput());
 
    typedef itk::BinaryContourImageFilter <FloatImageType, FloatImageType >
                                                binaryContourImageFilterType;
 
    binaryContourImageFilterType::Pointer binaryContourFilter
                                        = binaryContourImageFilterType::New ();
    binaryContourFilter->SetInput(fillHoles->GetOutput());
    binaryContourFilter->SetForegroundValue(255);
    binaryContourFilter->SetBackgroundValue(0);
    binaryContourFilter->SetFullyConnected(false);
    binaryContourFilter->Update();
    
    
    FloatImageType::Pointer contourImage = binaryContourFilter->GetOutput();
    
    typedef itk::ImageRegionIterator<FloatImageType>  FloatImageRegionIteratorType;
    
    FloatImageRegionIteratorType itContour(contourImage, 
          contourImage->GetLargestPossibleRegion());
    itContour.GoToBegin();
    
    ImageType::IndexType pixel;
    
    while(!itContour.IsAtEnd())
    {
        if(itContour.Value()!=0){
            pixel[0] = itContour.GetIndex()[0];
            pixel[1] = itContour.GetIndex()[1];
            
            this->contourPixels.push_back(pixel);
        }
        ++itContour;
    }
    
    typedef itk::ImageDuplicator<FloatImageType> DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(this->itkImage);
    duplicator->Update();
    
    FloatImageType::Pointer itkContourImage = duplicator->GetOutput();
    
    for(unsigned int i=0;i<contourPixels.size();i++){
        itkContourImage->SetPixel(contourPixels.at(i),255);
    }
        
    this->vtkContourImage = this->convertToVTKImage(itkContourImage);  
    
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
    this->vtkImage = image;
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

typedef itk::Image<float, 2> FloatImageType;
FloatImageType::Pointer ImageSegmentationWidget::convertToITKImage(vtkSmartPointer<vtkImageData> vtkImage)
{
    int numberOfScalars = vtkImage->GetNumberOfScalarComponents();
    
    typedef itk::VTKImageToImageFilter<ImageType> ITKConverterType;
    ITKConverterType::Pointer itkConverter = ITKConverterType::New();
    
    if(numberOfScalars==3){
        vtkSmartPointer<vtkImageLuminance> imageLuminance = vtkSmartPointer<vtkImageLuminance>::New();
        imageLuminance->SetInput(vtkImage);
        imageLuminance->Update();
        itkConverter->SetInput(imageLuminance->GetOutput());
    }else{
        itkConverter->SetInput(vtkImage);
    }

    itkConverter->Update();
    
    typedef itk::ImageDuplicator<ImageType> DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(itkConverter->GetOutput());
    duplicator->Update();
        
    typedef itk::CastImageFilter<ImageType,FloatImageType> CastImageType;
    CastImageType::Pointer castImage = CastImageType::New();
    castImage->SetInput(duplicator->GetOutput());
    castImage->Update();
    
    return castImage->GetOutput();    
}

vtkSmartPointer<vtkImageData> ImageSegmentationWidget::convertToVTKImage(FloatImageType::Pointer itkImage)
{
    
    CastFilterType::Pointer castProbabilityFilter = CastFilterType::New();
    castProbabilityFilter->SetInput(itkImage);
    castProbabilityFilter->Update(); 
    
    typedef itk::ImageToVTKImageFilter<ImageType> VTKConverterType;
    VTKConverterType::Pointer vtkConverter = VTKConverterType::New();
    vtkConverter->SetInput(castProbabilityFilter->GetOutput());
    vtkConverter->Update();
    
    vtkSmartPointer<vtkImageData> tempImage = vtkSmartPointer<vtkImageData>::New();
    tempImage->DeepCopy(vtkConverter->GetOutput());
    
    return tempImage;
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