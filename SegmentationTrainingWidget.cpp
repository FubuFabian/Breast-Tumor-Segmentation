#include "SegmentationTrainingWidget.h"

#include "itkImageToVTKImageFilter.h"
#include "itkVTKImageToImageFilter.h"
#include "SegmentationIntensityAndTextureImages.h"

#include <itkCastImageFilter.h>
#include <itkImageDuplicator.h>
#include <itkCastImageFilter.h>
#include <itkImageFileWriter.h>

#include <vtkImageLuminance.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencilToImage.h>
#include <vtkImageAccumulate.h>
#include <vtkBMPWriter.h>
#include <vtkImageIterator.h>

#include <QString>
#include <QFile>
#include <QTextStream>

SegmentationTrainingWidget::SegmentationTrainingWidget(QWidget *parent) : QWidget(parent)
{
    this->setupUi(this);

    itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
}

SegmentationTrainingWidget::~SegmentationTrainingWidget()
{
    delete this;
}

void SegmentationTrainingWidget::computeProbabilities()
{
    
    
    std::cout<<"Using "<<imageStack.size()<<" for segmentation training"<<std::endl<<std::endl;
    
    std::vector<vnl_vector<double> > intensityProbabilities; 
    std::vector<vnl_vector<double> > textureProbabilities; 
    
    for(int i=0;i<imageStack.size();i++){
       
        if(dataVector.at(i) == 0){
            std::cout<<"No manual segmentation for image: "<<i<<std::endl;
        }else{
        
            FloatImageType::Pointer itkImage = this->convertToITKImage(imageStack.at(i));
        
            SegmentationIntensityAndTextureImages<FloatImageType> * preprocessedImages =
                        SegmentationIntensityAndTextureImages<FloatImageType>::New();
    
            preprocessedImages->setInput(itkImage);
            
            std::cout<<"Obtaining intensity image: "<<i<<std::endl;
            FloatImageType::Pointer itkIntensityImage = preprocessedImages->getIntensityImage();
    
            typedef itk::CastImageFilter<FloatImageType, ImageType> CastFilterType;
            
            CastFilterType::Pointer castIntensityFilter = CastFilterType::New();
            castIntensityFilter->SetInput(itkIntensityImage);
            castIntensityFilter->Update(); 
        
            typedef itk::ImageToVTKImageFilter<ImageType> VTKConverterType;
        
            VTKConverterType::Pointer vtkIntensityConverter = VTKConverterType::New();
            vtkIntensityConverter->GetExporter()->SetInput(castIntensityFilter->GetOutput());
            vtkIntensityConverter->GetImporter()->Update();
    
            vtkSmartPointer<vtkImageData> intensityImage = vtkIntensityConverter->GetOutput();
            
            std::cout<<"Obtaining intensity stencil image: "<<i<<std::endl;
            vtkSmartPointer<vtkImageStencilData> intensityStencilImage = 
                vtkSmartPointer<vtkImageStencilData>::New();
            intensityStencilImage = this->getStencilImage(intensityImage,i);
        
            std::cout<<"Obtaining texture image: "<<i<<std::endl;
            FloatImageType::Pointer itkTextureImage = preprocessedImages->getTextureImage();
    
            CastFilterType::Pointer castTextureFilter = CastFilterType::New();
            castTextureFilter->SetInput(itkTextureImage);
            castTextureFilter->Update(); 
        
            VTKConverterType::Pointer vtkTextureConverter = VTKConverterType::New();
            vtkTextureConverter->GetExporter()->SetInput(castTextureFilter->GetOutput());
            vtkTextureConverter->GetImporter()->Update();
    
            std::cout<<"Obtaining probabilities for image: "<<i<<std::endl;
            vtkSmartPointer<vtkImageData> textureImage = vtkTextureConverter->GetOutput();
            
            std::cout<<"Obtaining texture stencil image: "<<i<<std::endl;
            vtkSmartPointer<vtkImageStencilData> textureStencilImage = 
                vtkSmartPointer<vtkImageStencilData>::New();
            textureStencilImage = this->getStencilImage(textureImage,i);
            
            vtkSmartPointer<vtkImageData> intensityHistogram = 
                    this->getHistogram(intensityStencilImage,intensityImage);
        
            vtkSmartPointer<vtkImageData> textureHistogram = 
                    this->getHistogram(textureStencilImage,textureImage);       
            
            int numberOfBins = textureHistogram->GetNumberOfPoints();
            
            vnl_vector<double> intensityProbability;
            intensityProbability.set_size(numberOfBins);
            intensityProbability.fill(0);
            
            vnl_vector<double> textureProbability;
            textureProbability.set_size(numberOfBins);
            textureProbability.fill(0);                  
            
            for(int x=0;x<numberOfBins;x++){
                
                int* intensityPixel = static_cast<int*>(intensityHistogram->GetScalarPointer(x,0,0));
                intensityProbability.put(x,intensityPixel[0]);
                
                int* texturePixel = static_cast<int*>(textureHistogram->GetScalarPointer(x,0,0));
                textureProbability.put(x,texturePixel[0]);
                
            }
                     
            intensityProbability /= numberOfVoxels;
            intensityProbabilities.push_back(intensityProbability);
            
            textureProbability /= numberOfVoxels;
            textureProbabilities.push_back(textureProbability);
            
        }
        std::cout<<std::endl;
    }
    
    std::cout<<std::endl;
    std::cout<<"Computing intensity mean probabilities"<<std::endl;
    
    this->intensityMeanProbabilities.set_size(256);
    this->intensityMeanProbabilities.fill(0);
    
    for(int i=0;i<intensityProbabilities.size();i++)
    {
        this->intensityMeanProbabilities += intensityProbabilities.at(i);
    }
    
    this->intensityMeanProbabilities /= intensityProbabilities.size();
    
    std::cout<<std::endl;
    std::cout<<"Computing texture mean probabilities"<<std::endl;
    
    this->textureMeanProbabilities.set_size(256);
    this->textureMeanProbabilities.fill(0);
    
    for(int i=0;i<textureProbabilities.size();i++)
    {
        this->textureMeanProbabilities += textureProbabilities.at(i);
    }
    
    this->textureMeanProbabilities /= textureProbabilities.size();

}

void SegmentationTrainingWidget::saveProbabilities()
{

    QString saveIntensityProbabilityFile = QFileDialog::getSaveFileName(
                this, tr("Choose File to Save Intensity Probability"), QDir::currentPath(),tr("Txt (*.txt)"));

    QFile intensityProbabilityFile(saveIntensityProbabilityFile);

    if(!intensityProbabilityFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        std::cout<<"Could not open File to save the intensity probability"<<std::endl;
        return;
    }

    std::cout<<std::endl;
    std::cout<<"Writing intensity probability"<<std::endl;

    QTextStream intensityProbabilityOut(&intensityProbabilityFile);

    for(int i=0;i<intensityMeanProbabilities.size();i++)
    {
        intensityProbabilityOut<<intensityMeanProbabilities.get(i)<<"\n";
    }
    
    intensityProbabilityFile.close();
    
    QString saveTextureProbabilityFile = QFileDialog::getSaveFileName(
                this, tr("Choose File to Save Texture Probability"), QDir::currentPath(),tr("Txt (*.txt)"));

    QFile textureProbabilityFile(saveTextureProbabilityFile);

    if(!textureProbabilityFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        std::cout<<"Could not open File to save the texture probability"<<std::endl;
        return;
    }

    std::cout<<"Writing texture probability"<<std::endl;

    QTextStream textureProbabilityOut(&textureProbabilityFile);

    for(int i=0;i<textureMeanProbabilities.size();i++)
    {
        textureProbabilityOut<<textureMeanProbabilities.get(i)<<"\n";
    }
    
    textureProbabilityFile.close();
}

void SegmentationTrainingWidget::setImage(vtkSmartPointer<vtkImageData> image)
{
    this->workWithStack = false;
    this->image = image;
}


void SegmentationTrainingWidget::
setImageStack(std::vector<vtkSmartPointer<vtkImageData> > imagestack)
{
    this->workWithStack = true;
    this->imageStack = imagestack;
    this->tableWidget->setRowCount(imageStack.size());
    this->dataVector.reserve(imagestack.size());
    this->dataVector.assign(imagestack.size(),0);
}

void SegmentationTrainingWidget::setMainWindow(MainWindow* mainwindow)
{
    this->mainWindow = mainwindow;
}

void SegmentationTrainingWidget::closeEvent( QCloseEvent * event)
{

    this->mainWindow->getDisplayWidget()->segmentationTrainingStyle->clearTracer();
    this->mainWindow->getDisplayWidget()->setPickerFlag();
    this->mainWindow->getDisplayWidget()->initPicker();
    event->accept();
}

void SegmentationTrainingWidget::setPath(vtkSmartPointer<vtkPolyData> data)
{

    int row = this->mainWindow->getDisplayWidget()->getImageDisplayedIndex();

    this->dataVector[row] = data;

    QString str;

    this->tableWidget->setItem(row, 0, new QTableWidgetItem(str.setNum(row)));
    this->tableWidget->setItem(row, 1, new QTableWidgetItem("Ok"));
}


typedef itk::Image<float, 2> FloatImageType;
FloatImageType::Pointer SegmentationTrainingWidget::convertToITKImage(vtkSmartPointer<vtkImageData> vtkImage)
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


vtkSmartPointer<vtkImageStencilData> SegmentationTrainingWidget::getStencilImage(
                                vtkSmartPointer<vtkImageData> vtkImage, int nImage)
{
    
    vtkSmartPointer<vtkPolyDataToImageStencil> dataToStencil = 
            vtkSmartPointer<vtkPolyDataToImageStencil>::New();
    dataToStencil->SetInput(dataVector.at(nImage));
    dataToStencil->SetOutputSpacing( vtkImage->GetSpacing() );
    dataToStencil->SetOutputOrigin( vtkImage->GetOrigin() );
    dataToStencil->SetOutputWholeExtent( vtkImage->GetWholeExtent() );
    dataToStencil->Update();
 
    return dataToStencil->GetOutput();
}

vtkSmartPointer<vtkImageData> SegmentationTrainingWidget::getHistogram(
                                    vtkSmartPointer<vtkImageStencilData> stencil,
                                            vtkSmartPointer<vtkImageData> vtkImage)
{
    vtkSmartPointer<vtkImageAccumulate> imageAccumulate = 
                            vtkSmartPointer<vtkImageAccumulate>::New();
    imageAccumulate->SetStencil(stencil);
    imageAccumulate->SetInput(vtkImage);
    imageAccumulate->Update();
    
    this->numberOfVoxels = imageAccumulate->GetVoxelCount();
    
    return imageAccumulate->GetOutput();
}

