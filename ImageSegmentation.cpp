#include "ImageSegmentation.h"
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

#include <math.h>


void ImageSegmentation::computeProbabilityImage()
{
	itkImage = this->convertToITKImage(vtkImage);
        
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

}

void ImageSegmentation::computeRegionGrowing()
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
}

typedef itk::Image<float, 2> FloatImageType;
FloatImageType::Pointer ImageSegmentation::convertToITKImage(vtkSmartPointer<vtkImageData> vtkImage)
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

vtkSmartPointer<vtkImageData> ImageSegmentation::convertToVTKImage(FloatImageType::Pointer itkImage)
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

void ImageSegmentation::setImage(vtkSmartPointer<vtkImageData> image)
{
    this->vtkImage = image;
}

void ImageSegmentation::setSeed(ImageType::IndexType seedPoint)
{
	this->seedPoint = seedPoint;
}

void ImageSegmentation::setIntensityProbabilities(std::vector<float> intensityProbs)
{
	this->intensityProbs = intensityProbs;
}

void ImageSegmentation::setTextureProbabilities(std::vector<float> textureProbs)
{
	this->textureProbs = textureProbs;
}

vtkSmartPointer<vtkImageData> ImageSegmentation::getProbabilityImage()
{
	return this->vtkProbabilityImage;
}

vtkSmartPointer<vtkImageData> ImageSegmentation::getRegionGrowingImage()
{
	return this->vtkRegionGrowingImage;
}

vtkSmartPointer<vtkImageData> ImageSegmentation::getContourImage()
{
	return this->vtkContourImage;
}

typedef itk::Image<unsigned char, 2> ImageType;
std::vector<ImageType::IndexType> ImageSegmentation::getContourPixels()
{
	return this->contourPixels;
}