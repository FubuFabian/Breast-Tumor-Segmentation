/* 
 * File:   itkScalarToTextureRLImageFilter.txx
 * Author: Fabian
 *
 * Created on July 29, 2013, 4:09 PM
 */

#ifndef ITKSCALARTOTEXTURERLIMAGEFILTER_TXX
#define	ITKSCALARTOTEXTURERLIMAGEFILTER_TXX

#include "itkScalarToTextureRLImageFilter.h"

#include <itkMirrorPadImageFilter.h>
#include <itkRegionOfInterestImageFilter.h>
#include <itkImage.h>
#include <itkSmartPointer.h>

#include <vector>
#include <math.h>
#include <iostream>

namespace itk {

template <class TInputImage, class TMaskImage, class TOutputImage>
ScalarToTextureRLImageFilter<TInputImage, TMaskImage, TOutputImage>
::ScalarToTextureRLImageFilter()
{
    this->m_RegionSize.Fill( 13 );
    this->m_MaskImage = NULL;
    this->m_NumberofBins = 16;
    this->m_PixelMaxValue = 255;
    this->m_PixelMinValue = 0;
    this->m_TextureFeature = 0;
    this->m_MeansFlag = true;
    
}

template<class TInputImage, class TMaskImage, class TOutputImage>
void
ScalarToTextureRLImageFilter<TInputImage, TMaskImage, TOutputImage>
::GenerateData()
{
    
  this->AllocateOutputs();
  
  ProgressReporter progress( this, 0, 
        this->GetInput()->GetRequestedRegion().GetNumberOfPixels(), 100 );
  
  typedef MirrorPadImageFilter<InputImageType,InputImageType> PadFilterType;
  typename PadFilterType::Pointer padFilter = PadFilterType::New();
  
  int boundSize = floor((double)this->m_RegionSize[1]/2);
  
  RegionSizeType bound;
  bound.Fill(boundSize);
  
  padFilter->SetPadBound(bound);
  padFilter->SetInput(this->GetInput());
  padFilter->Update();
  
  typename InputImageType::Pointer padImage = padFilter->GetOutput();
  
//  typedef Statistics::ScalarImageToCooccurrenceMatrixFilter<InputImageType>
//          GLCMGeneratorType;
//  typename GLCMGeneratorType::Pointer glcmGenerator = GLCMGeneratorType::New();
//  
//  glcmGenerator->SetNumberOfBinsPerAxis(this->m_NumberofBins);
//  glcmGenerator->SetPixelValueMinMax(this->m_PixelMinValue,m_PixelMaxValue);
  
  typename RunLengthFilterType::Pointer runLengthFilter = RunLengthFilterType::New();
  
  unsigned int dim = this->GetInput()->GetLargestPossibleRegion().GetImageDimension();
  typename OffsetVectorType::Pointer offsets = OffsetVectorType::New();
  this->SetOffsets(*offsets, dim);
  
  runLengthFilter->SetNumberOfBinsPerAxis(this->m_NumberofBins);
  runLengthFilter->SetPixelValueMinMax(this->m_PixelMinValue,this->m_PixelMaxValue);
  runLengthFilter->SetOffsets(offsets);
    

//  typedef typename GLCMGeneratorType::HistogramType HistogramType;
//  typedef Statistics::HistogramToTextureFeaturesFilter<HistogramType> 
//          TextureFeaturesType;
//  typename TextureFeaturesType::Pointer textureFeatures;
  
  typename RunLengthFilterType::FeatureValueVectorPointer features;
  
  if(this->m_MeansFlag){
        features = runLengthFilter->GetFeatureMeans();
  }else{
        features = runLengthFilter->GetFeatureStandardDeviations();
  }
  
  typedef RegionOfInterestImageFilter<InputImageType,InputImageType> ExtractFilterType;
  typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
  
  typename InputImageType::RegionType region;
  typename InputImageType::IndexType regionIndex;
    
  region.SetSize(this->m_RegionSize);
  
  extractFilter->SetInput(padImage);
  
  ImageRegionIteratorType itPad(padImage, padImage->GetLargestPossibleRegion());
  itPad.GoToBegin();

  ImageRegionIteratorType itOut(this->GetOutput(), 
          this->GetOutput()->GetLargestPossibleRegion());
  itOut.GoToBegin();
  
  runLengthFilter->FastCalculationsOn();
  
  if(this->m_TextureFeature == 0){
      runLengthFilter->SetRequestedFeature(0);
  }else if(this->m_TextureFeature == 1){
      runLengthFilter->SetRequestedFeature(1);
  }else if(this->m_TextureFeature == 2){
      runLengthFilter->SetRequestedFeature(2);
  }else if(this->m_TextureFeature == 3){
      runLengthFilter->SetRequestedFeature(3);
  }else if(this->m_TextureFeature == 4){
      runLengthFilter->SetRequestedFeature(4);
  }else if(this->m_TextureFeature == 5){
      runLengthFilter->SetRequestedFeature(5);
  }else if(this->m_TextureFeature == 6){
      runLengthFilter->SetRequestedFeature(6);
  }else if(this->m_TextureFeature == 7){
      runLengthFilter->SetRequestedFeature(7);
  }else if(this->m_TextureFeature == 8){
      runLengthFilter->SetRequestedFeature(8);
  }else if(this->m_TextureFeature == 9){
      runLengthFilter->SetRequestedFeature(9);
  }
  
  while(!itPad.IsAtEnd()){

        regionIndex = itPad.GetIndex();
        region.SetIndex(regionIndex);
        
        if(padImage->GetLargestPossibleRegion().IsInside(region)){ 
            
            extractFilter->SetRegionOfInterest(region);
            extractFilter->UpdateLargestPossibleRegion();
            
            runLengthFilter->SetInput(extractFilter->GetOutput());
            runLengthFilter->UpdateLargestPossibleRegion();
                
            this->GetOutput()->SetPixel(itOut.GetIndex(),features->ElementAt(0));
            
            ++itOut;
            
            progress.CompletedPixel();
        }
        
        ++itPad;
  }
  
}

template<class TInputImage, class TMaskImage, class TOutputImage>
void
ScalarToTextureRLImageFilter<TInputImage, TMaskImage, TOutputImage>
::SetOffsets(OffsetVectorType &offsets, unsigned int dim) const
{
      
    if(dim<3){
    
      OffsetType offset1;
      offset1[0] = 1;
      offset1[1] = 0;
  
   
      OffsetType offset2;
      offset2[0] = 1;
      offset2[1] = -1;
  
      OffsetType offset3;
      offset3[0] = 0;
      offset3[1] = -1;
    
      OffsetType offset4;
      offset4[0] = -1;
      offset4[1] = -1;
      
      offsets.reserve(4);
      offsets.InsertElement(0,offset1);
      offsets.InsertElement(1,offset2);
      offsets.InsertElement(2,offset3);
      offsets.InsertElement(3,offset4);
      
  }else{
      
      OffsetType offset1;
      offset1[0] = 1; offset1[1] = 0; offset1[2] = 1;
   
   
      OffsetType offset2;
      offset2[0] = 1; offset2[1] = 0; offset2[2] = 0;
  
      OffsetType offset3;
      offset3[0] = 1; offset3[1] = 0; offset3[2] = -1;
    
      OffsetType offset4;
      offset4[0] = 1; offset4[1] = 1; offset4[2] = 1;
      
      OffsetType offset5;
      offset5[0] = 1; offset5[1] = 1; offset5[2] = 0;
      
      OffsetType offset6;
      offset6[0] = 1; offset6[1] = 1; offset6[2] = -1;
      
      OffsetType offset7;
      offset7[0] = 0; offset7[1] = 1; offset7[2] = 1;
      
      OffsetType offset8;
      offset8[0] = 0; offset8[1] = 1; offset8[2] = 0;
      
      OffsetType offset9;
      offset9[0] = 0; offset9[1] = 1; offset9[2] = -1;
      
      OffsetType offset10;
      offset10[0] = -1; offset10[1] = 1; offset10[2] = 1;
      
      OffsetType offset11;
      offset11[0] = -1; offset11[1] = 1; offset11[2] = 0;
      
      OffsetType offset12;
      offset12[0] = -1; offset12[1] = 1; offset12[2] = -1;
      
      OffsetType offset13;
      offset13[0] = 0; offset13[1] = 0; offset13[2] = 1;
      
      offsets.reserve(13);
      offsets.InsertElement(0,offset1);
      offsets.InsertElement(1,offset2);
      offsets.InsertElement(2,offset3);
      offsets.InsertElement(3,offset4);
      offsets.InsertElement(4,offset5);
      offsets.InsertElement(5,offset6);
      offsets.InsertElement(6,offset7);
      offsets.InsertElement(7,offset8);
      offsets.InsertElement(8,offset9);
      offsets.InsertElement(9,offset10);
      offsets.InsertElement(10,offset11);
      offsets.InsertElement(11,offset12);
      offsets.InsertElement(12,offset13);
  }
    
}

template<class TInputImage, class TMaskImage, class TOutputImage>
void
ScalarToTextureRLImageFilter<TInputImage, TMaskImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );

  os << indent << "Region size: "
    << this->m_RegionSize << std::endl;
  os << indent << "Number of Bins: "
    << this->m_NumberofBins << std::endl;
  os << indent << "Maximum Pixel Value: "
    << this->m_PixelMaxValue << std::endl;
  os << indent << "Minimum Pixel Value: "
    << this->m_PixelMinValue << std::endl;
  
  if(this->m_TextureFeature == 0){
     os << indent << "Texture Feature: Energy"<<std::endl;
  }else if(this->m_TextureFeature == 1){
     os << indent << "Texture Feature: Entropy"<<std::endl;
  }else if(this->m_TextureFeature == 2){
     os << indent << "Texture Feature: Correlation"<<std::endl;
  }else if(this->m_TextureFeature == 3){
     os << indent << "Texture Feature: Inverse Different Moment"<<std::endl;
  }else if(this->m_TextureFeature == 4){
     os << indent << "Texture Feature: Inertia"<<std::endl;
  }else if(this->m_TextureFeature == 5){
     os << indent << "Texture Feature: Cluster Shade"<<std::endl;
  }else if(this->m_TextureFeature == 6){
     os << indent << "Texture Feature: Cluster Prominence"<<std::endl;
  }else if(this->m_TextureFeature == 7){
     os << indent << "Texture Feature: Haralick Correlation"<<std::endl;
  }
  
}

}// end namespace itk

#endif	/* ITKSCALARTOTEXTURERLIMAGEFILTER_TXX */

