#ifndef VTKTHREEVIEWS_H
#define VTKTHREEVIEWS_H

#include <QWidget>
#include <QVTKVolumeSliceWidget.h>

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h> 

#include <vtkEventQtSlotConnect.h>

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
    explicit VTKThreeViews( vtkSmartPointer<vtkImageData> = NULL, QWidget *parent = 0);

	/**
	Destructor
	**/
    ~VTKThreeViews();
    
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
};

#endif // VTKTHREEVIEWS_H
