#ifndef VOLUMESEGMENTATION_H
#define VOLUMESEGMENTATION_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <itkImage.h>
#include <itkCastImageFilter.h>

//!Segmenta a breast tumor on an ultrasound volume
/*!
 *This class segment a breast tumor in an ultrasound volume using apriori information
 *obtained with probability functions of intensity and texture. It uses a region 
 * growing algorithm to obtain the tumor region from a probability volume 
*/
class VolumeSegmentation 
{

    typedef itk::Image<unsigned char, 3> VolumeType;    
    typedef itk::Image<float, 3> FloatVolumeType;
    typedef itk::CastImageFilter<FloatVolumeType, VolumeType> CastFilterType;
    
public:
    
    
    /**
     * \brief Class constructor
     */
    static VolumeSegmentation *New()
	{
		return new VolumeSegmentation;
	}
    
    /**
     * \brief Class destructor
     */
    ~VolumeSegmentation()
	{
		delete this;
	}

    /**
     * \brief compute the probability volume: 1 highest probablity
     */
	void computeProbabilityVolume();
        
        /**
        * \brief computes the region growing algortithm on the probability volume
        * to obtain the tumor region 
        */
	void computeRegionGrowing();
        
        /**
        * \brief set the ultrasound Volume 
        */
	void setVolume(vtkSmartPointer<vtkImageData> volume);

        /**
        * \brief set the seed for the region growing algorithm 
        * [in] index of the pixel
        */
	void setSeed(VolumeType::IndexType seedPoint);

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
        * \brief returns the probability volume
        */
	vtkSmartPointer<vtkImageData> getProbabilityVolume();
                
        /**
        * \brief returns the region growing volume 
        */
	vtkSmartPointer<vtkImageData> getRegionGrowingVolume();

        /**
        * \brief returns the contour volume 
        */
	vtkSmartPointer<vtkImageData> getContourVolume();

        /**
        * \brief returns the coordinates of the tumor contour 
        */
	std::vector<VolumeType::IndexType> getContourPixels();
    
private:

	FloatVolumeType::Pointer itkVolume; ///< the itk ultrasound volume

    vtkSmartPointer<vtkImageData> vtkVolume; ///<  the vtk ultrasound volume
    
    vtkSmartPointer<vtkImageData> vtkProbabilityVolume; ///<the vtk probability volume

    vtkSmartPointer<vtkImageData> vtkRegionGrowingVolume; ///< the vtk region growing Volume
    
    vtkSmartPointer<vtkImageData> vtkContourVolume; ///< the vtk tumor contour volume
    
    std::vector<VolumeType::IndexType> contourPixels; ///< the tumor contour pixels coordinates
    
    FloatVolumeType::Pointer probabilityVolume; ///< the itk probability volume
    
    std::vector<float> intensityProbs; ///< the intensity probability function
    
    std::vector<float> textureProbs; ///< the texture probability function
    
    VolumeType::IndexType seedPoint; ///< the region growing seed point
    
    /**
    * \brief convert a vtk volume to an itk volume
    * [out] an itk image
    */
    FloatVolumeType::Pointer convertToITKVolume(vtkSmartPointer<vtkImageData> vtkVolume);
    
    /**
    * \brief convert an itk volume to a vtk volume
    * [out] a vtk volume
    */
    vtkSmartPointer<vtkImageData> convertToVTKVolume(FloatVolumeType::Pointer itkVolume);
};

#endif // IMAGESEGMENTATION_H
