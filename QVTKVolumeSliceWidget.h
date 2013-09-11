#ifndef QVTKVOLUMESLICEWIDGET_H
#define QVTKVOLUMESLICEWIDGET_H

#endif // QVTKVOLUMESLICEWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QVTKWidget.h>

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>

//!Display volume Slices
/*!
  This class provides the funtions to display on a vtkImageViewer2 a slice of a volume in
  the defined view (axial, sagittal or coronal)
*/

class QVTKVolumeSliceWidget : public QWidget {
    Q_OBJECT    

public:

	/**
	Constructor
	**/
	QVTKVolumeSliceWidget(QWidget *parent = 0);

	/**
	Destructor
	**/
	virtual ~QVTKVolumeSliceWidget();

	/**
	Sets a vtkImageData as the volumeData and obtains the needed parameters for slicing
	[in] vtkImageData that contains the volume to slice
	**/
	void setVolumeData(vtkSmartPointer<vtkImageData>); 

	/**
	Displays the selected slice in a vtkImageViewer2
	[in] int that indicates the slice number to display
	**/
	void displayVolumeSlice(int);

	/**
	Sets the axial plane as the slicing plane, DEFAULT
	**/
	void setAxialView();

	/**
	Sets the coronal plane as the slicing plane
	**/
	void setCoronalView();

	/**
	Sets the coronal plane as the slicing plane
	**/
	void setSagittalView();

private:

	/**
	QVTKWidget that contains the rendererWindow of the 
	**/
	QVTKWidget * qvtkWidget;

	/**
	Image Viewer that render the sliced image
	**/
	vtkSmartPointer<vtkImageViewer2> viewer;

	/**
	vtkImageData that contains the data of the volume to slice
	**/
	vtkSmartPointer<vtkImageData> volumeData;

	/**
	Matrix containing the rotation of the slice plane
	**/
	vtkSmartPointer<vtkMatrix4x4> resliceAxes;

	/**
	Reslicer that slice the volume and gives the sliced image as an output
	**/
	vtkSmartPointer<vtkImageReslice> reslicer;

	/**
	ImageActor of the sliced image
	**/
	vtkSmartPointer<vtkImageActor> imageActor;

	/**
	Interactor style of the viewer, it has the basic interaction with a 2D image
	**/
	vtkSmartPointer<vtkInteractorStyleImage> imageStyle;

	/**
	Data for the axial plane
	**/
	static const double axialElements[16];

	/**
	Data for the coronal plane
	**/
	static const double coronalElements[16];

	/**
	Data for the sagittal plane
	**/
	static const double sagittalElements[16];

	/**
	Indicates if the volume is going to be sliced in the axial plane
	**/
	bool axialView;

	/**
	Indicates if the volume is going to be sliced in the coronal plane
	**/
	bool coronalView;

	/**
	Indicates if the volume is going to be sliced in the sagital plane
	**/
	bool sagittalView;

	/**
	Contains the voxel size of the volumeData
	**/
	double spacing[3];

	/**
	Contains the origin of the volumeData
	**/
	double origin[3];

};