#ifndef SCENE3D_H
#define SCENE3D_H

#include <iostream>

#include "igstkPolarisTracker.h"
#include "igstkPolarisTrackerTool.h"

#include "igstkTransformObserver.h"

#include "igstkAxesObject.h"
#include "igstkUSProbeObject.h"
#include "igstkNeedleObject.h"
#include "igstkPolarisPointerObject.h"
#include "igstkUSImageObject.h"

#include "igstkAxesObjectRepresentation.h"
#include "igstkUSProbeObjectRepresentation.h"
#include "igstkNeedleObjectRepresentation.h"
#include "igstkPolarisPointerObjectRepresentation.h"
#include "igstkImageSpatialObjectVolumeRepresentation.h"

#include "Scene3DWidget.h"

using namespace  std;

//!3D sCene
/*!
  This class has the main objects and configuration to create a virtual representation of  a needle biopsy.
*/
class Scene3D
{

public:

	typedef igstk::PolarisTrackerTool         TrackerToolType;
	typedef TrackerToolType::TransformType    TransformType;
	typedef igstk::TransformObserver          ObserverType;

	/** \brief Class constructor*/
    static Scene3D *New()
    {
        return new Scene3D;
    }

	/** \brief Initialize the 3D scene, creates all the scene objects*/
    void init3DScene();

	/** \brief Configure the Polaris tracker
	* \param[in] ROM files*/
	void configTracker(std::string, std::string, std::string, std::string, QString, QString, QString, int);
	
	/** \brief Start Tracking*/
	void startTracking();

	/** \brief Initialize the tracker logger*/
	void initLogger();

	/** \brief Add an ultrasound volume to the scene*/
	void addVolumeToScene(std::string);

	/** \bried Add elements to the for views ins the Scene3DWidget*/
	void addFourViews();

	/** \bried Set fourViews to false*/
	void removeFourViews();

private:

	bool volumeLoaded; ///<Indicates if a volume is loaded
	bool configTrackerFlag; ///<Indicates of the tracker is configure
	bool fourViews; ///<Indicates if the four views are active
	TransformType identityTransform; ///<Transformation for the tracked objects

	igstk::AxesObject::Pointer   referenceAxes; ///<Refrence axes object
	igstk::USProbeObject::Pointer usProbe; ///<Ultrasound Probe object
    igstk::NeedleObject::Pointer needle; ///<Biopsy needle object
	igstk::PolarisPointerObject::Pointer pointer; ///<Polaris pointer object
	igstk::USImageObject::Pointer usVolume; ///<Ultrasound volume object

	TrackerToolType::Pointer referenceTool; ///<Tool for the reference axes
	TrackerToolType::Pointer ultrasoundProbeTool; ///<Tool to track the us probe
	TrackerToolType::Pointer needleTool; ///<Tool to track the biopsy needle
	TrackerToolType::Pointer pointerTool; ///<Tool to track the polaris pointer

	igstk::PolarisTracker::Pointer tracker; ///<Tracker object

	ObserverType::Pointer coordSystemAObserverReferenceTool; ///<Oberver for the reference tool events
	ObserverType::Pointer coordSystemAObserverUltrasoundProbe; ///<Oberver for the ultrasound probe events
	ObserverType::Pointer coordSystemAObserverNeedle; ///<Oberver for the needle events
	ObserverType::Pointer coordSystemAObserverPointer; ///<Oberver for the ponter events

	Scene3DWidget * scene3DWidget; ///<User inteface

	igstk::AxesObjectRepresentation::Pointer referenceAxesRepresentation; ///<Representation of Reference Axes Object
	igstk::USProbeObjectRepresentation::Pointer usProbeRepresentation; ///<Representation of US Probe Object
	igstk::NeedleObjectRepresentation::Pointer needleRepresentation; ///<Representation of Needle Object
	igstk::PolarisPointerObjectRepresentation::Pointer pointerRepresentation; ///<Representation of Pointer Object 
	igstk::ImageSpatialObjectVolumeRepresentation<igstk::USImageObject>::Pointer usVolumeRepresentation; ///<Representation of US Volume Object


};
#endif // SCENE3D_H
















