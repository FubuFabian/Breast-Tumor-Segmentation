#ifndef VTKTHREEVIEWS_H
#define VTKTHREEVIEWS_H

#include <QWidget>
#include <QVTKVolumeSliceWidget.h>

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h> 

#include <vtkEventQtSlotConnect.h>

class VolumeSegmentationWidget;

namespace Ui {
class VTKThreeViews;
}


//!Display volume Slices int three different views
/*!
  This class recieves a vtkImageData representationg a 3D image and displays the
  axial, sagittal and coronal views of the volume
*/
class VTKThreeViews : public QWidget
{
    Q_OBJECT
    
public:

	/**
	Constructor, it initialize the QVTKVolumeSliceWidgets to display the three views  
	[in] vtkImageData containing the 3D image data to display
	*/
    explicit VTKThreeViews(QWidget *parent = 0);

	/**
	Destructor
	**/
    ~VTKThreeViews();
    
    /**
     * \brief set the user picked coordinates when clicking in a viewer
     * @param xPicked
     * @param yPicked
     * @param zPicked
     */
    void setPickedCoordinates(int xPicked, int yPicked, int zPicked);
    
    /**
     * \brief Set the volume Data to display
     * @param volumeData
     */
    void setVolumeData(vtkSmartPointer<vtkImageData> volumeData);
    
    /**
     * \brief Returns the picked coordinates 
     * @return std::vector containing xPicked, yPicke and zPicked
     */
    std::vector<int>* getPickedCoordinates();
    
    /**
     * Connects the widget with a VolumeSegmentationWudget.h
     * @param volumeSegmentation
     */
    void connectWithVolumeSegmentation(VolumeSegmentationWidget* volumeSegmentation);
    
private:
    
	/**
	The QT user interface
	**/
	Ui::VTKThreeViews *ui;

	/**
	Widget that displays the axial view of the 3D image
	**/
	QVTKVolumeSliceWidget * axialDisplayWidget;

	/**
	Widget that displays the sagittal view of the 3D image
	**/
	QVTKVolumeSliceWidget * sagittalDisplayWidget;

	/**
	Widget that displays the coronal view of the 3D image
	**/
	QVTKVolumeSliceWidget * coronalDisplayWidget;
        
        std::vector<int> * pickedCoordinates; ///< PickedCoordinates with the mouse
        
        vtkSmartPointer<vtkEventQtSlotConnect> axialConnections; ///<Connector for the axial viewer
        
        vtkSmartPointer<vtkEventQtSlotConnect> sagittalConnections; ///<Connector for the sagittal viewer
        
        vtkSmartPointer<vtkEventQtSlotConnect> coronalConnections; ///<Connector for the coronal viewer

	
private slots:
    
	/**
	indicate the slice to display with the axialDisplayWidget depending on the position
	of the axialSlider
	**/
    void axialSliderMove(int);

	/**
	indicate the slice to display with the sagittalDisplayWidget depending on the position
	of the sagittalSlider
	**/
    void sagittalSliderMove(int);

	/**
	indicate the slice to display with the coronalDisplayWidget depending on the position
	of the coronalSlider
	**/
    void coronalSliderMove(int);
    
    /**
     * \brief Rotate the axialView camera
     */
    void axialRotate();
    
    /**
     * \brief Flip the axialView image horizontally
     */
    void axialHorizontalFlip();
    
     /**
     * \brief Flip the axialView image vertically
     */
    void axialVerticalFlip();
    
    /**
     * \brief Rotate the sagittalView camera
     */
    void sagittalRotate();
    
    /**
     * \brief Flip the asagittalView image horizontally
     */
    void sagittalHorizontalFlip();
    
    /**
     * \brief Flip the sagittalView image vertically
     */
    void sagittalVerticalFlip();
    
    /**
     * \brief Rotate the coronalView camera
     */
    void coronalRotate();
    
    /**
     * \brief Flip the coronalView image horizontally
     */
    void coronalHorizontalFlip();
    
    /**
     * \brief Flip the coronalView image vertically
     */
    void coronalVerticalFlip();
    
};

#endif // VTKTHREEVIEWS_H
