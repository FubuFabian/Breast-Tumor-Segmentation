#ifndef VOLUMERECONSTRUCTIONWIDGET_H
#define VOLUMERECONSTRUCTIONWIDGET_H

#include <QWidget>

#include "mainwindow.h"

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkVolume.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkMath.h>


#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

namespace Ui {
class VolumeReconstructionWidget;
}

//!Has the interaction methods for the user to generate a new volue
/*!
  This class allows the user to choose between a voxel based method or a pixel based method
  to recontruct a volume and set the main volume properties. It allows to change the opacity
  of the generated volume and to changethe colors.
  This class also allows to the user to save the volume in a .mhd and .raw files
*/
class VolumeReconstructionWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit VolumeReconstructionWidget(QWidget *parent = 0);
    ~VolumeReconstructionWidget();

	/** Set the window to display the volume */
    void setMainWindow(MainWindow* mainwindow);

	/** Set the transformation stack for the volume image */
	void setTransformStack(std::vector< vnl_matrix<double> >);

	/** Set the image data stack */
	void setVolumeImageStack(std::vector< vtkSmartPointer<vtkImageData> >);
    
private:
    Ui::VolumeReconstructionWidget *ui;

	/** the main window to call it */
    MainWindow* mainWindow;

    /** The data of each image */
	std::vector< vtkSmartPointer<vtkImageData> > volumeImageStack;

    /** Contains the transformation of each image */
	std::vector< vnl_matrix<double> >  transformStack;

    /** Contains the transformed coords in x of each image pixel */
	std::vector< vnl_matrix<double> > imageCoordsXStack;
    /** Contains the transformed coords in y of each image pixel */
	std::vector< vnl_matrix<double> > imageCoordsYStack;
    /** Contains the transformed coords in z of each image pixel */
	std::vector< vnl_matrix<double> > imageCoordsZStack;

    /** Contains the transformed bounds in x of each image pixel */
	std::vector< vnl_vector<double> > imageBoundsXStack;
    /** Contains the transformed bounds in y of each image pixel */
	std::vector< vnl_vector<double> > imageBoundsYStack;
    /** Contains the transformed bounds in z of each image pixel */
	std::vector< vnl_vector<double> > imageBoundsZStack;

    /** the volume to display */
	vtkSmartPointer<vtkVolume> volume;

    /** Data of the volume */
	vtkSmartPointer<vtkImageData> volumeData;

    /** Main volume properties */
	vtkSmartPointer<vtkVolumeProperty> volumeProperty;

    /** Start of the volume data in the 3D space */
	vnl_vector<double> volumeOrigin;

    /** End of the volume data in the 3D space */
	vnl_vector<double> volumeFinal;

    /** Size of the volume data */
	vnl_vector<double> volumeSize;

    /** Scale of the images */
	vnl_vector<double> scale;
        
    /** The relation between voxel:pixel, example res:1*/    
        int res;

    /**
     * \brief Computes every pixel coord of each image in the 3D space
     */
	void calcImageCoords();

    /**
     * \brief Computes the coords of the images bounds int he 3D space
     */
	void calcImageBounds();

    /**
     * \brief Computes the volume size
     * \param[in] if bool is true it computes it with the imagecoords,
     * else it uses the image bounds
     */
	void calcVolumeSize(bool);

    /**
     * \brief Set the volume opacity
     */
    void setVolumeOpacity();

    /**
     * \brief Set the volume colo transfer function
     */
    void setVolumeColorMap();

    /**
     * \brief Set the display properties of the volume and the rendering method
     */
	void setDisplayProperties(vtkSmartPointer<vtkImageData>);

    /**
     * \brief Display the volume in the 3D scene
     */
	void displayVolume();

private slots:

    /**
     * \brief Saves the volume in a .mhd and .raw file
     */
    void save();

    /**
     * \brief Calls VolumeReconstruction.h to generate the new volume data
     */
    void generate();

    /**
     * \brief Set the resolution of the volume
     */
    void setResolution(int idx);

};

#endif // VOLUMERECONSTRUCTIONWIDGET_H
