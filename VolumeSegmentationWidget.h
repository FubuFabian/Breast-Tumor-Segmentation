#ifndef VOLUMESEGMENTATIONWIDGET_H
#define VOLUMESEGMENTATIONWIDGET_H

#include "mainwindow.h"
#include "QVTKImageWidget.h"
#include "VolumeSegmentation.h"
#include "VTKThreeViews.h"

#include <QWidget>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <itkImage.h>

namespace Ui {
    class VolumeSegmentationWidget;
}

//!User interface for volume segmentation
/*!
 *This class is the user interface for segmenting a breast tumor from
 *3D ultrasound image using the class VolumeSegmentation.h
*/
class VolumeSegmentationWidget : public QWidget
{
    Q_OBJECT
    
public:
    
    typedef itk::Image<unsigned char, 3> VolumeType; ///< itk 3D image type

    /**
     * \brief class contructor
     * @param parent
     */
    explicit VolumeSegmentationWidget(QWidget *parent = 0);
    ~VolumeSegmentationWidget();

    /**
     * \brief set the ultrasoun image
     * @param a vtk image
     */
    void setVolume(vtkSmartPointer<vtkImageData> volume);
    
    /**
     * \brief Set the calling main window
     * @param mainWindow
     */
    void setMainWindow(MainWindow * mainWindow);
    
    void setVTKThreeViews(VTKThreeViews* threeViews);
	   
    
private:
    
    Ui::VolumeSegmentationWidget *ui; ///< the user interface

    VolumeSegmentation * volumeSegmentation; ///< has all the functions for volume segmentation
    
    MainWindow * mainWindow; ///< the calling main window
    
    VTKThreeViews* threeViews;
    
    vtkSmartPointer<vtkImageData> vtkProbabilityVolume; ///< the probability volume
    
    vtkSmartPointer<vtkImageData> vtkRegionGrowingVolume; ///< the region growing volume
    
    vtkSmartPointer<vtkImageData> vtkContourVolume; ///< the tumor contour volume
    
    std::vector<VolumeType::IndexType> contourPixels; ///< the tumor contour pixels coordinates
    
    QVTKImageWidget *displayWidget; ///< the display widget
    
    std::vector<float> intensityProbs; ///< the intensity probability function
    
    std::vector<float> textureProbs; ///< the texture probability function
    
    bool intensityProbsFlag; ///< indicates if the intensity probability function is loaded
    
    bool textureProbsFlag; ///< indicates if the texture probability function is loaded
    
    bool probabilityVolumeFlag; ///< indicates if the probability volume has been computed
    
    bool seedPointFlag; ///< indicates if the seed piont has been selected
    
    VolumeType::IndexType seedPoint;  ///< the region growing seed point

private slots:

    /**
     * \brief load the intensity probability function
     */
    void loadIntensityProbs();

    /**
     * \brief load the texture probability function
     */
    void loadTextureProbs();
    
    /**
     * \brief calls the VolumeSegmentation.h function to compute the proability volume
     */
    void computeProbability();

    /**
     * \brief calls the VolumeSegmentation.h function to segment the volume
     */
    void segment();

    /**
     * \brief saves the probability volume, region growing volume, contour volume
     * and the contour coordinates
     */
    void save();
    
    
    /**
     * \brief choose a new seed point
     */
    void newSeed();
    
    /**
    * \brief get the cross point coordinates
    */
    void getCoordinates();
};

#endif // IMAGESEGMENTATIONWIDGET_H
