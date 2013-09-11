#ifndef CHANGEVOLUMEPROPERTIESWIDGET_H
#define CHANGEVOLUMEPROPERTIESWIDGET_H

#include <QWidget>
#include "QVTKImageWidget.h"
#include "igstkImageSpatialObjectVolumeRepresentation.h"
#include "igstkUSImageObject.h"

namespace Ui {
class ChangeVolumePropertiesWidget;
}

//!Changes volume properties
/*!
  This class changes the opacity propertie of a displayed volume
*/
class ChangeVolumePropertiesWidget : public QWidget
{
    Q_OBJECT
    
public:

	/**
	Constructor
	**/
    explicit ChangeVolumePropertiesWidget(QWidget *parent = 0);
    
	/**
	Destructor
	**/
	~ChangeVolumePropertiesWidget();

	/**
	set the QVTKImageWidget qhere the volume is displayed
	[in] a pointer to the QVTKImageWidget
	**/
	void setQVTKImageWidget(QVTKImageWidget *);

	void setigstkImageSpatialObjectVolumeRepresentation(
		igstk::ImageSpatialObjectVolumeRepresentation<igstk::USImageObject>::Pointer igstkImageRepresentation);
    
private:

	/**
	The user Interface
	**/
    Ui::ChangeVolumePropertiesWidget *ui;

	/**
	The QVTKImageWidget where the volume is displayed
	**/
	QVTKImageWidget * qvtkImageWidget;

	igstk::ImageSpatialObjectVolumeRepresentation<igstk::USImageObject>::Pointer igstkImageRepresentation;


	/**
	Upper threshold value for the opacity property.
	The values above this will not be display.
	**/
	int upperThreshVal;

	/**
	Lower threshold value for the opacity property.
	The values bellow this will not be display.
	**/
	int lowerThreshVal;

	/**
	Middle point between the lower and upper thresholds.
	This point has the highest opacity value
	**/
	int middlePoint;

	/**
	Highest opacity value
	**/
	double opacityVal;

	/**
	Picewisefunction representing the opacity of the volume
	**/
	vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity;

	bool QVTKImage;

private slots:

	/**
	Change the threshold values when the lowerThresholdSlider is moved
	**/
    void changeLowerThreshold(int);

	/**
	Change the threshold values when the upperThresholdSlider is moved
	**/
    void changeUpperThreshold(int);

	/**
	Change the highest opacity values whe the opacitySlider is moved
	**/
    void changeOpacity(int);
};

#endif // CHANGEVOLUMEPROPERTIESWIDGET_H
