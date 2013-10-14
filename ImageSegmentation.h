#ifndef IMAGESEGMENTATION_H
#define IMAGESEGMENTATION_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <itkImage.h>
#include <itkCastImageFilter.h>

//!Segmenta a breast tumor on an ultrasound image
/*!
 *This class segment a breast tumor in an ultrasound image using apriori information
 *obtained with probability functions of intensity and texture. It uses a region 
 * growing algorithm to obtain the tumor region from a probability image 
*/
class ImageSegmentation 
{

    typedef itk::Image<unsigned char, 2> ImageType;    
    typedef itk::Image<float, 2> FloatImageType;
    typedef itk::CastImageFilter<FloatImageType, ImageType> CastFilterType;
    
public:
    
    
    /**
     * \brief Class constructor
     */
    static ImageSegmentation *New()
	{
		return new ImageSegmentation;
	}
    
    /**
     * \brief Class destructor
     */
    ~ImageSegmentation()
	{
		delete this;
	}

    /**
     * \brief compute the probability image 1 highest probablity
     */
	void computeProbabilityImage();
        
        /**
        * \brief computes the region growing algortithm on the probability image
        * to obtain the tumor region 
        */
	void computeRegionGrowing();
        
        /**
        * \brief set the ultrasound Image 
        */
	void setImage(vtkSmartPointer<vtkImageData> image);

        /**
        * \brief set the seed for the region growing algorithm 
        * [in] index of the pixel
        */
	void setSeed(ImageType::IndexType seedPoint);

        /**
        * \brief set the probability function for intensity
        * [in]  intensity probability function
        */
	void setIntensityProbabilities(std::vector<float> intensityProbs);
        
        /**
        * \brief set the probability function for texture
        * [in]  texture probability function
        */
	void setTextureProbabilities(std::vector<float> textureProbs);

        /**
        * \brief returns the probability image 
        */
	vtkSmartPointer<vtkImageData> getProbabilityImage();
                
        /**
        * \brief returns the region growing image 
        */
	vtkSmartPointer<vtkImageData> getRegionGrowingImage();

        /**
        * \brief returns the contour image 
        */
	vtkSmartPointer<vtkImageData> getContourImage();

        /**
        * \brief returns the coordinates of the tumor contour 
        */
	std::vector<ImageType::IndexType> getContourPixels();
    
private:

	FloatImageType::Pointer itkImage; ///< the itk ultrasound image

    vtkSmartPointer<vtkImageData> vtkImage; ///<  the vtk ultrasound image
    
    vtkSmartPointer<vtkImageData> vtkProbabilityImage; ///<the vtk probability image

    vtkSmartPointer<vtkImageData> vtkRegionGrowingImage; ///< the vtk region growing image
    
    vtkSmartPointer<vtkImageData> vtkContourImage; ///< the vtk tumor contour image
    
    std::vector<ImageType::IndexType> contourPixels; ///< the tumor contour pixels coordinates
    
    FloatImageType::Pointer probabilityImage; ///< the itk probability image
    
    std::vector<float> intensityProbs; ///< the intensity probability function
    
    std::vector<float> textureProbs; ///< the texture probability function
    
    ImageType::IndexType seedPoint; ///< the region growing seed point
    
    /**
    * \brief convert a vtk image to an itk image
    * [out] an itk image
    */
    FloatImageType::Pointer convertToITKImage(vtkSmartPointer<vtkImageData> vtkImage);
    
    /**
    * \brief convert an itk image to a vtk image
    * [out] a vtk image
    */
    vtkSmartPointer<vtkImageData> convertToVTKImage(FloatImageType::Pointer itkImage);
};

#endif // IMAGESEGMENTATION_H
