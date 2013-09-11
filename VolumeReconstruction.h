#ifndef VOLUMERECONSTRUCTION_H
#define VOLUMERECONSTRUCTION_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPlane.h>

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

#include <math.h>
#include <vector>

#endif // VOLUMERECONSTRUCTION_H
 
using namespace std;

//!Generate a new volume
/*!
  This class generate a new volume data using a voxel based method with the previously loaded data.
  It requiers the images data, the tracker data and the estimated parameters from a calibration.
  The method implemented a nearest pixel interpolation.
*/
class VolumeReconstruction
{

public:

    /**
     * \brief Constructor
     */
	static VolumeReconstruction *New()
	{
			return new VolumeReconstruction;
	} 

    /**
     * \brief Set the size of the volume data
     */
	void setVolumeSize(vnl_vector<double>);

    /**
     * \brief Set the volume data orgin in the 3D scene
     */
	void setVolumeOrigin(vnl_vector<double>);

    /**
     * \brief Set the image bounds
     */
    void setImageBoundsStack(std::vector< vnl_vector<double> >, std::vector< vnl_vector<double> >,
                              std::vector< vnl_vector<double> >);

    /**
     * \brief Set image data stack to generate the volume
     */
    void setVolumeImageStack(std::vector< vtkSmartPointer< vtkImageData> >);

    /**
     * \brief Set the transformation for each image used in the reconstruction
     */
    void setTransformStack(std::vector< vnl_matrix<double> >);

    /**
     * \brief Set the scale of the images
     */
    void setScale(vnl_vector<double>);
    
    /**
     * \brief Set the resolution of the volume
     */
    void setResolution(int);

    /**
     * \brief Returns the new volume data with the voxel based method
     */
	vtkSmartPointer<vtkImageData> generateVolume();

private:

     /** Size of the volume */
	vnl_vector<double> volumeSize;

    /** Where the volume data begins in the 3D scene */
	vnl_vector<double> volumeOrigin;

    /** Stacks for the image Bounds in x */
    std::vector< vnl_vector<double> > imageBoundsXStack;
    /** Stacks for the image Bounds in Y */
    std::vector< vnl_vector<double> > imageBoundsYStack;
    /** Stacks for the image Bounds in Z */
    std::vector< vnl_vector<double> > imageBoundsZStack;

    /** The stack of images data */
    std::vector< vtkSmartPointer< vtkImageData> > volumeImageStack;

    /** Contains the transformation for each image */
    std::vector< vnl_matrix<double> > transformStack;

    /** scale of the images */
    vnl_vector<double> scale;

    /** The plane equation for each image */
	std::vector< vtkSmartPointer<vtkPlane> > imagePlaneStack;

    /** the maximun distance found in the volume */
	double maxDistance;

        /** The resolution of the volume*/
        int resolution;

    /**
     * \brief Compute the plane equation for each image
     */
	void calcImagePlane();


    /**
     * \brief Computes the maximun distance in the volume
     */
	double calcMaxDistance();


    /**
     * \brief Computes the voxel value using three lineal interpolation
     */
	double calcVoxelValue(std::vector< vnl_vector<double> >, vnl_vector<double>, vnl_vector<double>);

};
