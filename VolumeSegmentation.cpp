#include "VolumeSegmentation.h"
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


void VolumeSegmentation::computeProbabilityVolume()
{
	itkVolume = this->convertToITKVolume(vtkVolume);
        
    SegmentationIntensityAndTextureImages<FloatVolumeType> * preprocessedVolumes =
                        SegmentationIntensityAndTextureImages<FloatVolumeType>::New();
    
    preprocessedVolumes->setInput(itkVolume);
    preprocessedVolumes->setTimeStep(0.0625);
            
    std::cout<<std::endl<<"Obtaining intensity Volume"<<std::endl;
    FloatVolumeType::Pointer itkIntensityVolume = preprocessedVolumes->getIntensityImage();
            
    CastFilterType::Pointer castIntensityFilter = CastFilterType::New();
    castIntensityFilter->SetInput(itkIntensityVolume);
    castIntensityFilter->Update(); 
    
    VolumeType::Pointer itkIntensityCastVolume = castIntensityFilter->GetOutput();
    
    std::cout<<"Obtaining texture Volume"<<std::endl<<std::endl;
    FloatVolumeType::Pointer itkTextureVolume = preprocessedVolumes->getTextureImage();
    
    CastFilterType::Pointer castTextureFilter = CastFilterType::New();
    castTextureFilter->SetInput(itkTextureVolume);
    castTextureFilter->Update(); 
    
    VolumeType::Pointer itkTextureCastVolume = castTextureFilter->GetOutput();
    
    FloatVolumeType::Pointer itkProbabilityVolume = FloatVolumeType::New();
    itkProbabilityVolume->SetRegions(itkVolume->GetLargestPossibleRegion());
    itkProbabilityVolume->Allocate();
    
    typedef itk::ImageRegionIterator<VolumeType>  VolumeRegionIteratorType;
    typedef itk::ImageRegionIterator<FloatVolumeType>  FloatVolumeRegionIteratorType;
    
    VolumeRegionIteratorType itIntensity(itkIntensityCastVolume,
          itkIntensityCastVolume->GetLargestPossibleRegion());
    itIntensity.GoToBegin();


    VolumeRegionIteratorType itTexture(itkTextureCastVolume,
          itkTextureCastVolume->GetLargestPossibleRegion());
    itTexture.GoToBegin(); 
    
    FloatVolumeRegionIteratorType itOut(itkProbabilityVolume, 
          itkProbabilityVolume->GetLargestPossibleRegion());
    itOut.GoToBegin();
    
    while(!itOut.IsAtEnd())
    {
        float intensityValue = intensityProbs.at(itIntensity.Value());
        float textureValue = textureProbs.at(itTexture.Value());
        
        float probability = intensityValue*textureValue;
        
        itkProbabilityVolume->SetPixel(itOut.GetIndex(),probability);
        
        ++itOut;
        ++itIntensity;
        ++itTexture;
    }     
    
    typedef itk::RescaleIntensityImageFilter<FloatVolumeType, FloatVolumeType> 
                                                              RescaleFilterType;
    RescaleFilterType::Pointer normalizeFilter = RescaleFilterType::New();
    normalizeFilter->SetOutputMaximum(255);
    normalizeFilter->SetOutputMinimum(0);
    normalizeFilter->SetInput(itkProbabilityVolume);
    normalizeFilter->Update();   
    
    this->probabilityVolume = normalizeFilter->GetOutput();
    
    this->vtkProbabilityVolume = this->convertToVTKVolume(probabilityVolume);

}

void VolumeSegmentation::computeRegionGrowing()
{
    this->contourPixels.clear();
    
    typedef itk::ConnectedThresholdImageFilter<FloatVolumeType,FloatVolumeType> RegionGrowingType;
    RegionGrowingType::Pointer regionGrowing = RegionGrowingType::New();
    
    regionGrowing->SetInput(probabilityVolume);
    
    FloatVolumeType::SizeType volumeSize;
    volumeSize = probabilityVolume->GetLargestPossibleRegion().GetSize();
    
    seedPoint[1] = volumeSize[1] - seedPoint[1]; 
    
    regionGrowing->SetSeed(seedPoint);
    regionGrowing->SetReplaceValue(255);
    
    typedef itk::NeighborhoodIterator<FloatVolumeType> NeighborhoodIterator;
    NeighborhoodIterator::RadiusType radius;
    radius.Fill(1);
    
    FloatVolumeType::RegionType seedRegion;
    
    FloatVolumeType::RegionType::IndexType seedRegionIndex;
    seedRegionIndex[0] = seedPoint[0] - 1;
    seedRegionIndex[1] = seedPoint[1] - 1;
    seedRegionIndex[2] = seedPoint[2] - 1;
    seedRegion.SetIndex(seedRegionIndex);
    
    FloatVolumeType::RegionType::SizeType seedRegionSize;
    seedRegionSize.Fill(3); 
    
    NeighborhoodIterator it(radius,probabilityVolume,seedRegion);
    
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
    
    typedef itk::GrayscaleFillholeImageFilter<FloatVolumeType,FloatVolumeType> FillHolesType;
    FillHolesType::Pointer fillHoles = FillHolesType::New();
    fillHoles->SetInput(regionGrowing->GetOutput());
    fillHoles->Update();
    
    this->vtkRegionGrowingVolume = this->convertToVTKVolume(fillHoles->GetOutput());
 
    typedef itk::BinaryContourImageFilter <FloatVolumeType, FloatVolumeType >
                                                binaryContourVolumeFilterType;
 
    binaryContourVolumeFilterType::Pointer binaryContourFilter
                                        = binaryContourVolumeFilterType::New ();
    binaryContourFilter->SetInput(fillHoles->GetOutput());
    binaryContourFilter->SetForegroundValue(255);
    binaryContourFilter->SetBackgroundValue(0);
    binaryContourFilter->SetFullyConnected(false);
    binaryContourFilter->Update();
    
    
    FloatVolumeType::Pointer contourVolume = binaryContourFilter->GetOutput();
    
    typedef itk::ImageRegionIterator<FloatVolumeType>  FloatVolumeRegionIteratorType;
    
    FloatVolumeRegionIteratorType itContour(contourVolume, 
          contourVolume->GetLargestPossibleRegion());
    itContour.GoToBegin();
    
    VolumeType::IndexType pixel;
    
    while(!itContour.IsAtEnd())
    {
        if(itContour.Value()!=0){
            pixel[0] = itContour.GetIndex()[0];
            pixel[1] = itContour.GetIndex()[1];
            pixel[2] = itContour.GetIndex()[2];
            
            this->contourPixels.push_back(pixel);
        }
        ++itContour;
    }
    
    typedef itk::ImageDuplicator<FloatVolumeType> DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(this->itkVolume);
    duplicator->Update();
    
    FloatVolumeType::Pointer itkContourVolume = duplicator->GetOutput();
    
    for(unsigned int i=0;i<contourPixels.size();i++){
        itkContourVolume->SetPixel(contourPixels.at(i),255);
    }
        
    this->vtkContourVolume = this->convertToVTKVolume(itkContourVolume);  
}

typedef itk::Image<float, 3> FloatVolumeType;
FloatVolumeType::Pointer VolumeSegmentation::convertToITKVolume(vtkSmartPointer<vtkImageData> vtkVolume)
{
    int numberOfScalars = vtkVolume->GetNumberOfScalarComponents();
    
    typedef itk::VTKImageToImageFilter<VolumeType> ITKConverterType;
    ITKConverterType::Pointer itkConverter = ITKConverterType::New();
    
    if(numberOfScalars==3){
        vtkSmartPointer<vtkImageLuminance> volumeLuminance = vtkSmartPointer<vtkImageLuminance>::New();
        volumeLuminance->SetInput(vtkVolume);
        volumeLuminance->Update();
        itkConverter->SetInput(volumeLuminance->GetOutput());
    }else{
        itkConverter->SetInput(vtkVolume);
    }

    itkConverter->Update();
    
    typedef itk::ImageDuplicator<VolumeType> DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(itkConverter->GetOutput());
    duplicator->Update();
        
    typedef itk::CastImageFilter<VolumeType,FloatVolumeType> CastVolumeType;
    CastVolumeType::Pointer castVolume = CastVolumeType::New();
    castVolume->SetInput(duplicator->GetOutput());
    castVolume->Update();
    
    return castVolume->GetOutput();    
}

vtkSmartPointer<vtkImageData> VolumeSegmentation::convertToVTKVolume(FloatVolumeType::Pointer itkVolume)
{  
    CastFilterType::Pointer castProbabilityFilter = CastFilterType::New();
    castProbabilityFilter->SetInput(itkVolume);
    castProbabilityFilter->Update(); 
    
    typedef itk::ImageToVTKImageFilter<VolumeType> VTKConverterType;
    VTKConverterType::Pointer vtkConverter = VTKConverterType::New();
    vtkConverter->SetInput(castProbabilityFilter->GetOutput());
    vtkConverter->Update();
    
    vtkSmartPointer<vtkImageData> tempVolume = vtkSmartPointer<vtkImageData>::New();
    tempVolume->DeepCopy(vtkConverter->GetOutput());
    
    return tempVolume;
}

void VolumeSegmentation::setVolume(vtkSmartPointer<vtkImageData> volume)
{
    this->vtkVolume = volume;
}

void VolumeSegmentation::setSeed(VolumeType::IndexType seedPoint)
{
	this->seedPoint = seedPoint;
}

void VolumeSegmentation::setIntensityProbabilities(std::vector<float> intensityProbs)
{
	this->intensityProbs = intensityProbs;
}

void VolumeSegmentation::setTextureProbabilities(std::vector<float> textureProbs)
{
	this->textureProbs = textureProbs;
}

vtkSmartPointer<vtkImageData> VolumeSegmentation::getProbabilityVolume()
{
	return this->vtkProbabilityVolume;
}

vtkSmartPointer<vtkImageData> VolumeSegmentation::getRegionGrowingVolume()
{
	return this->vtkRegionGrowingVolume;
}

vtkSmartPointer<vtkImageData> VolumeSegmentation::getContourVolume()
{
	return this->vtkContourVolume;
}

typedef itk::Image<unsigned char, 3> VolumeType;
std::vector<VolumeType::IndexType> VolumeSegmentation::getContourPixels()
{
	return this->contourPixels;
}