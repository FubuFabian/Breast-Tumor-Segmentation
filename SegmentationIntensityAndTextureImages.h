/* 
 * File:   SegmentationIntensityAndTextureImages.h
 * Author: Fabian
 *
 * Created on August 15, 2013, 1:13 PM
 */

#ifndef SEGMENTATIONINTENSITYANDTEXTUREIMAGES_H
#define	SEGMENTATIONINTENSITYANDTEXTUREIMAGES_H

#include <itkImage.h>
#include <itkRescaleIntensityImageFilter.h>


/*!
  This classs Computes the intensity and texture images for breast tumors in
 * ultrasound images. For intensity it uses histogram equalization and 
 * anisotropic gaussian filtering. For texture it uses runlength matrices.
*/
template<class TInputImage>
class SegmentationIntensityAndTextureImages
{
    
public:
    
    typedef TInputImage ImageType;
    
    /**
     * Constructor
     * @return 
     */
    static SegmentationIntensityAndTextureImages * New(){
        return new SegmentationIntensityAndTextureImages;
    }
        
    /**
     * Set input image
     * @param image 
     */
    void setInput(typename ImageType::Pointer image);
    
    /**
     * Computes the intensity image
     * @return intensity Image
     */
    typename ImageType::Pointer getIntensityImage();
    
    /**
     * Computes texture image
     * @return texture image
     */
    typename ImageType::Pointer getTextureImage();
    
private:
    
    /**
     * Input Image
     */
    typename ImageType::Pointer inputImage;
    
    /**
     * Typedef for normilization filter
     */
    typedef typename itk::RescaleIntensityImageFilter<ImageType, ImageType> 
                                                              RescaleFilterType;
    
    
};

#ifndef ITK_MANUAL_INSTANTIATION
#include "SegmentationIntensityAndTextureImages.cpp"
#endif

#endif	/* SEGMENTATIONINTENSITYANDTEXTUREIMAGES_H */

