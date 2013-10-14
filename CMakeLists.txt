CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(BreastTumorSegmentation)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

FIND_PACKAGE(ITK)
IF(ITK_FOUND)
	INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
	MESSAGE(FATAL_ERROR "Cannot build US Probe Calibration without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

FIND_PACKAGE (VTK)
IF (VTK_FOUND)
   INCLUDE(${USE_VTK_FILE})
ELSE(VTK_FOUND)
	MESSAGE(FATAL_ERROR "Cannot build US Probe Calibration without VTK.  Please set ITK_DIR.")   
ENDIF(VTK_FOUND)

FIND_PACKAGE(LSQRRecipes)
IF(LSQRRecipes_FOUND)
    INCLUDE(${LSQRRecipes_USE_FILE})
ELSE(LSQRRecipes_FOUND)
	MESSAGE(FATAL_ERROR "Cannot build US Probe Calibration without LSQRRecipes.")   
ENDIF(LSQRRecipes_FOUND)

FIND_PACKAGE(IGSTK)
IF(IGSTK_FOUND)
	INCLUDE(${IGSTK_USE_FILE})
ELSE(IGSTK_FOUND)
	MESSAGE(FATAL_ERROR "Cannot build US Probe Calibration without IGSTK.  Please set IGSTK_DIR.")
ENDIF(IGSTK_FOUND)

FIND_PACKAGE(FLTK)
 IF (FLTK_FOUND)
  INCLUDE_DIRECTORIES (${FLTK_INCLUDE_DIR})
ELSE (FLTK_FOUND)
  MESSAGE( FATAL_ERROR "This application requires FLTK. One of these components is missing. Please verify configuration")
ENDIF (FLTK_FOUND)

# Find QT 
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE( ${QT_USE_FILE} )

# support for out-of-source build
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${IGSTK_SOURCE_DIR}/Examples/TransformReaders
  ${IGSTK_SOURCE_DIR}/Examples/TransformWriters
)


 
# Set your files and resources here
SET(AppSrcs main.cpp mainwindow.cpp QVTKImageWidget.cpp QVTKImageWidgetCommand.cpp 
    ProbeCalibrationWidget.cpp Calibration.cpp VolumeReconstructionWidget.cpp 
    VolumeReconstruction.cpp CropImagesWidget.cpp Scene3D.cpp Scene3DWidget.cpp
    PolarisTracker.cpp igstkNeedleObject.cpp igstkNeedleObjectRepresentation.cpp
    igstkUSProbeObject.cpp igstkUSProbeObjectRepresentation.cpp
    igstkPolarisPointerObject.cpp igstkPolarisPointerObjectRepresentation.cpp
    CheckCalibrationErrorWidget.cpp vtkTracerInteractorStyle.cpp
    EstimateSphereFromPoints.cpp SphereFunction.cpp igstkUSImageObject.cpp 
    igstkImageSpatialObjectVolumeRepresentation.txx QVTKVolumeSliceWidget.cpp
    VTKThreeViews.cpp ChangeVolumePropertiesWidget.cpp
    PivotCalibration.cpp PolarisConfigurationWidget.cpp SegmentationTrainingWidget.cpp
    SegmentationIntensityAndTextureImages.cpp ImageSegmentation.cpp
    ImageSegmentationWidget.cpp VolumeSegmentation.cpp
    VolumeSegmentationWidget.cpp)
    
SET(AppHeaders mainwindow.h QVTKImageWidget.h QVTKImageWidgetCommand.h 
    ProbeCalibrationWidget.h Calibration.h VolumeReconstructionWidget.h
    VolumeReconstruction.h CropImagesWidget.h Scene3D.h Scene3DWidget.h
    PolarisTracker.h igstkNeedleObject.h igstkNeedleObjectRepresentation.h
    igstkUSProbeObject.h igstkUSProbeObjectRepresentation.h
    igstkPolarisPointerObject.h igstkPolarisPointerObjectRepresentation.h
    CheckCalibrationErrorWidget.h vtkTracerInteractorStyle.h
    EstimateSphereFromPoints.h SphereFunction.h igstkUSImageObject.h 
    igstkImageSpatialObjectVolumeRepresentation.h QVTKVolumeSliceWidget.h
    VTKThreeViews.h ChangeVolumePropertiesWidget.h
    PivotCalibration.h PolarisConfigurationWidget.h SegmentationTrainingWidget.h
    itkScalarToTextureRLImageFilter.h itkScalarImageToRunLengthFeaturesFilter.h 
    itkScalarImageToRunLengthMatrixFilter.h itkHistogramToRunLengthFeaturesFilter.h 
    itkHistogramEqualizationFilter.h SegmentationIntensityAndTextureImages.h
    itkImageToVTKImageFilter.h itkVTKImageToImageFilter.h ImageSegmentation.h
    ImageSegmentationWidget.h VolumeSegmentation.h VolumeSegmentationWidget.h)
    
SET(AppUI mainwindow.ui ProbeCalibrationWidget.ui VolumeReconstructionWidget.ui 
    CropImagesWidget.ui Scene3DWidget.ui CheckCalibrationErrorWidget.ui
    VTKThreeViews.ui ChangeVolumePropertiesWidget.ui PolarisConfigurationWidget.ui
    SegmentationTrainingWidget.ui ImageSegmentationWidget.ui VolumeSegmentationWidget.ui)

SET(AppUIFLTK PivotCalibrationUI.fl)

FLTK_WRAP_UI(BreastTumorSegmentation ${AppUIFLTK}) 
 
# for generate qt aditional files 
QT4_WRAP_UI(UISrcs ${AppUI})
QT4_WRAP_CPP(MOCSrcs ${AppHeaders} )
 
 
# Packaging source code in same files 
SOURCE_GROUP("Resources" FILES
  ${AppUI}
  ${AppUIFLTK}
)
 
SOURCE_GROUP("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
  ${AppUIFLTK}
)
 
ADD_EXECUTABLE(BreastTumorSegmentation ${AppSrcs} ${AppHeaders} ${UISrcs} ${MOCSrcs} ${AppUIFLTK})

TARGET_LINK_LIBRARIES(BreastTumorSegmentation QVTK IGSTK ${VTK_LIBRARIES} ${ITK_LIBRARIES} ${FLTK_LIBRARIES} TransformReaders TransformWriters LSQRRecipes)

