/* 
 * File:   itkHistogramEqualizationFilter.h
 * Author: Fabian
 *
 * Created on July 29, 2013, 4:06 PM
 */

#ifndef ITKHISTOGRAMEQUALIZATIONFILTER_H
#define	ITKHISTOGRAMEQUALIZATIONFILTER_H

#include "itkImageToImageFilter.h"
#include <itkImageRegionConstIterator.h>
#include <itkImageToHistogramFilter.h>

namespace itk {

/** \class HistogramEqualizationFilter. This class generates a conventional
 * histogram equalization.
 * 
 */
template<class TInputImage, class TMaskImage = Image<unsigned char, 
  ::itk::GetImageDimension<TInputImage>::ImageDimension>, 
  class TOutputImage = TInputImage>
class ITK_EXPORT HistogramEqualizationFilter :
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef HistogramEqualizationFilter                      Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>   Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;

  /** Standard New method. */
  itkNewMacro( Self );

  /** ImageDimension constants */
  itkStaticConstMacro( ImageDimension, unsigned int,
                       TInputImage::ImageDimension );

  /** Some convenient typedefs. */
  typedef float                                   RealType;
  typedef TInputImage                             InputImageType;
  typedef TMaskImage                              MaskImageType;
  typedef TOutputImage                            OutputImageType;
  
  typedef typename Statistics::ImageToHistogramFilter<InputImageType> 
  HistogramFilterType;
  

  /** Runtime information support. */
  itkTypeMacro( HistogramEqualizationFilter,
                ImageToImageFilter );

  typedef ImageRegionConstIterator<InputImageType>
  ImageRegionIteratorType;
  typedef typename ImageRegionIteratorType::SizeType RegionSizeType;
  
  itkSetMacro( RegionSize, RegionSizeType );
  itkGetConstMacro( RegionSize, RegionSizeType );
  

  void SetMaskImage( const MaskImageType *mask )
    {
    this->SetNthInput( 1, const_cast<MaskImageType *>( mask ) ); 
    }
  const MaskImageType* GetMaskImage() const
    {
    return static_cast<MaskImageType*>( const_cast<DataObject *>
      ( this->ProcessObject::GetInput( 1 ) ) ); 
    }  
  void SetInput1( const TInputImage *input )
    {
    this->SetInput( input ); 
    }  
  void SetInput2( const TMaskImage *mask )
    {
    this->SetMaskImage( mask );
    }  
  void SetHistogramSize(typename HistogramFilterType::HistogramSizeType size)
    {
    this->m_HistogramSize = size;  
    }


protected:
  HistogramEqualizationFilter();
  ~HistogramEqualizationFilter() {};
  void PrintSelf( std::ostream& os, Indent indent ) const;

  void GenerateData();

private:
  HistogramEqualizationFilter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  RegionSizeType                       m_RegionSize;
  typename MaskImageType::Pointer      m_MaskImage;
  typename HistogramFilterType::HistogramSizeType m_HistogramSize;

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkHistogramEqualizationFilter.txx"
#endif

#endif	/* ITKHISTOGRAMEQUALIZATIONFILTER_H */

