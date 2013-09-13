#ifndef IMAGESEGMENTATION_H
#define IMAGESEGMENTATION_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <itkImage.h>
#include <itkCastImageFilter.h>

class ImageSegmentation 
{

    typedef itk::Image<unsigned char, 2> ImageType;    
    typedef itk::Image<float, 2> FloatImageType;
    typedef itk::CastImageFilter<FloatImageType, ImageType> CastFilterType;
    
public:
    static ImageSegmentation *New()
	{
		return new ImageSegmentation;
	}
    ~ImageSegmentation()
	{
		delete this;
	}

	void computeProbabilityImage();

	void computeRegionGrowing();

	void setImage(vtkSmartPointer<vtkImageData> image);

	void setSeed(ImageType::IndexType seedPoint);

	void setIntensityProbabilities(std::vector<float> intensityProbs);

	void setTextureProbabilities(std::vector<float> textureProbs);

	vtkSmartPointer<vtkImageData> getProbabilityImage();

	vtkSmartPointer<vtkImageData> getRegionGrowingImage();

	vtkSmartPointer<vtkImageData> getContourImage();

	std::vector<ImageType::IndexType> getContourPixels();
    
private:

	FloatImageType::Pointer itkImage;

    vtkSmartPointer<vtkImageData> vtkImage;
    
    vtkSmartPointer<vtkImageData> vtkProbabilityImage;

    vtkSmartPointer<vtkImageData> vtkRegionGrowingImage;
    
    vtkSmartPointer<vtkImageData> vtkContourImage;
    
    std::vector<ImageType::IndexType> contourPixels;
    
    FloatImageType::Pointer probabilityImage;
    
    std::vector<float> intensityProbs;
    
    std::vector<float> textureProbs;
    
    ImageType::IndexType seedPoint;
    
    FloatImageType::Pointer convertToITKImage(vtkSmartPointer<vtkImageData> vtkImage);
    
    vtkSmartPointer<vtkImageData> convertToVTKImage(FloatImageType::Pointer itkImage);
};

#endif // IMAGESEGMENTATION_H
