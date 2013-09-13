/* 
 * File:   itkHistogramEqualizationFilter.txx
 * Author: Fabian
 *
 * Created on July 29, 2013, 4:09 PM
 */

#ifndef ITKHISTOGRAMEQUALIZATIONFILTER_TXX
#define	ITKHISTOGRAMEQUALIZATIONFILTER_TXX

#include "itkHistogramEqualizationFilter.h"

#include <itkMirrorPadImageFilter.h>
#include <itkScalarImageToCooccurrenceMatrixFilter.h>
#include <itkHistogramToTextureFeaturesFilter.h>
#include <itkExtractImageFilter.h>
#include <itkImage.h>

#include <vector>
#include <math.h>

namespace itk {

template <class TInputImage, class TMaskImage, class TOutputImage>
HistogramEqualizationFilter<TInputImage, TMaskImage, TOutputImage>
::HistogramEqualizationFilter()
{
    this->m_RegionSize.Fill( 13 );
    this->m_MaskImage = NULL;
    this->m_HistogramSize.SetSize(1);
    this->m_HistogramSize[0] = 256;
}

template<class TInputImage, class TMaskImage, class TOutputImage>
void
HistogramEqualizationFilter<TInputImage, TMaskImage, TOutputImage>
::GenerateData()
{
    
  this->AllocateOutputs();

  ProgressReporter progress( this, 0, 
        this->GetInput()->GetRequestedRegion().GetNumberOfPixels(), 100 );
  
  typename HistogramFilterType::Pointer histogramFilter = HistogramFilterType::New();
  histogramFilter->SetInput(this->GetInput());
  histogramFilter->SetAutoMinimumMaximum(true);
  histogramFilter->SetMarginalScale(10);
  histogramFilter->SetHistogramSize(this->m_HistogramSize);
  histogramFilter->Update();
  
  typedef typename HistogramFilterType::HistogramType LocalHistogramType;
  const LocalHistogramType * histogram = histogramFilter->GetOutput();
  
  typename LocalHistogramType::ConstIterator itHist = histogram->Begin();
  
  std::vector<int> cumulativeVector;
  cumulativeVector.reserve(this->m_HistogramSize[0]);
  
  cumulativeVector[0] = itHist.GetFrequency();
  ++itHist;
  
  for(unsigned int i=1;i<this->m_HistogramSize[0];i++)
  {
      int frequency = itHist.GetFrequency();
      cumulativeVector[i] = frequency + cumulativeVector[i-1];
      ++itHist;
  }  
  
  std::vector<float> equalizedVector;
  equalizedVector.reserve(this->m_HistogramSize[0]);
  
  int nPixels = this->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels();
  
  for(unsigned int i=0;i<this->m_HistogramSize[0];i++)
  {
      float temp = 
      (double)(cumulativeVector[i]-cumulativeVector[0])/(nPixels-cumulativeVector[0]);
      equalizedVector[i] = floor(temp*(this->m_HistogramSize[0]-1));
  }
  

  ImageRegionIteratorType itIn(this->GetInput(),
          this->GetInput()->GetLargestPossibleRegion());
  itIn.GoToBegin();

  ImageRegionIteratorType itOut(this->GetOutput(), 
          this->GetOutput()->GetLargestPossibleRegion());
  itOut.GoToBegin();
    
  while(!itIn.IsAtEnd())
  {
      this->GetOutput()->SetPixel(itOut.GetIndex(),
              equalizedVector[this->GetInput()->GetPixel(itIn.GetIndex())]);
      ++itOut;
      ++itIn;
      progress.CompletedPixel();
  }      
  
  

  
}

template<class TInputImage, class TMaskImage, class TOutputImage>
void
HistogramEqualizationFilter<TInputImage, TMaskImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );

  os << indent << "Region size: "
    << this->m_RegionSize << std::endl;
  os << indent << "Histogram size: "
    << this->m_HistogramSize[0] << std::endl;
  
}

}// end namespace itk

#endif	/* ITKHistogramEqualizationFILTER_TXX */

