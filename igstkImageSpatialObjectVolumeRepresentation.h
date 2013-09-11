/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    $RCSfile: igstkImageSpatialObjectVolumeRepresentation.h,v $
  Language:  C++
  Date:      $Date: 2007/02/09 23:34:22 $
  Version:   $Revision: 1.7 $

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __igstkImageSpatialObjectVolumeRepresentation_h
#define __igstkImageSpatialObjectVolumeRepresentation_h

#include "igstkObjectRepresentation.h"
#include "igstkImageSpatialObject.h"
#include "igstkStateMachine.h"

#include "vtkImageShiftScale.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeTextureMapper3D.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkSmartPointer.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeRayCastMapper.h"

namespace igstk
{

/** \class ImageSpatialObjectVolumeRepresentation
 * 
 * \brief This class represents an image object. The parameters of the object
 * are ... The image object is rendered in a VTK scene using the
 * ... object.
 * 
 * \ingroup ObjectRepresentation
 */

template < class TImageSpatialObject >
class ImageSpatialObjectVolumeRepresentation : public ObjectRepresentation
{

public:

  /** Macro with standard traits declarations. */
  igstkStandardTemplatedClassTraitsMacro( 
                                       ImageSpatialObjectVolumeRepresentation, \
                                       ObjectRepresentation )

public:

  typedef TImageSpatialObject                      ImageSpatialObjectType;

  typedef typename ImageSpatialObjectType::ConstPointer 
                                           ImageSpatialObjectConstPointer;

  /** Return a copy of the current object representation */
  Pointer Copy() const;

  /** Connect this representation class to the spatial object */
  void RequestSetImageSpatialObject( const ImageSpatialObjectType * 
                                                ImageSpatialObject );
  void RequestSetImageSpatialObject();  
  /** Set methods. Need State machine logic */
  igstkSetMacro( MinThreshold, unsigned );
  igstkGetMacro( MinThreshold, unsigned );
  igstkSetMacro( MaxThreshold, unsigned );
  igstkGetMacro( MaxThreshold, unsigned );
  igstkSetMacro( ShiftBy,      unsigned );
  igstkGetMacro( ShiftBy,      unsigned );

  vtkVolumeProperty * RequestGetVolumeProperty()
    { return m_VolumeProperty; }
  void RequestSetVolumeProperty( vtkVolumeProperty * vp )
    { m_VolumeProperty = vp; }

  void SetImageData(vtkImageData *ImageDataInput){
//	  m_ImageData2=ImageDataInput ;
	  m_ImageData=ImageDataInput ;
  }

  void UpdateVTKPipelineProcessing();

  /** Type used for representing the slice number */
  typedef unsigned int SliceNumberType;

  /** Print the object information in a stream. */
  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const; 

  /** Observer macro that will received a event with an image as payload and
   * will store it internally. This will be the receptor of the event sent by
   * the ImageSpatialObject when an image is requested. */
  igstkObserverMacro( VTKImage, VTKImageModifiedEvent, 
                      EventHelperType::VTKImagePointerType );
  
  float VolumeOpacity;

  void changeVolumeOpacity(vtkSmartPointer<vtkPiecewiseFunction> opacity);

protected:

  /** Constructor */
  ImageSpatialObjectVolumeRepresentation();
  
  /** Destructor */
  ~ImageSpatialObjectVolumeRepresentation();

  /** Overloaded function to delete actors */
  void DeleteActors();

  /** Create the VTK actors for displaying geometry */
  void CreateActors();

private:

  ImageSpatialObjectVolumeRepresentation(const Self&); //not implemented
  void operator=(const Self&);   //purposely not implemented

  /** Internal itkSpatialObject */
  ImageSpatialObjectConstPointer         m_ImageSpatialObject;
  ImageSpatialObjectConstPointer         m_ImageSpatialObjectToAdd;

  /** VTK classes that support display of an image */
  vtkPiecewiseFunction *                 m_OpacityTransferFunction;
  vtkColorTransferFunction *             m_ColorTransferFunction;
  vtkPiecewiseFunction *				 m_GrayScaleTransferFunction;

  vtkImageShiftScale *                   m_ShiftScale;
  vtkSmartPointer<vtkVolumeRayCastMapper>             m_VolumeMapper;
  vtkSmartPointer<vtkVolumeProperty>                    m_VolumeProperty;

  vtkImageData *                         m_ImageData;
  vtkImageData *                         m_ImageData2;
  vtkVolume *                            m_ImageActor;
  
  unsigned                               m_ShiftBy;
  unsigned                               m_MinThreshold;
  unsigned                               m_MaxThreshold;

  /** Update the visual representation with changes in the geometry */
  virtual void UpdateRepresentationProcessing();

  /** Null operation for State Machine transition */
  void NoProcessing();

  /** Connect this representation class to the spatial object. Only to be
   * called by the State Machine. */
  void SetImageSpatialObjectProcessing();

  /** Sets the vtkImageData from the spatial object. This method MUST be
   * private in order to prevent unsafe access from the VTK image layer. */
  void SetImage( const vtkImageData * image );
  
  /** Attempt to set the Slice Number. */
  void AttemptSetSliceNumberProcessing();

  /** Actually set the Slice Number. */
  void SetSliceNumberProcessing();

  /** Actually set the Slice Orientation. */
  void SetOrientationProcessing();
      
  /** Reports the minimum and maximum slice numbers on the current
   *  orientation */
  void ReportSliceNumberBoundsProcessing();

  /** Connect VTK pipeline */
  void ConnectVTKPipelineProcessing();
    

  /** Observer to the VTK image events */
  typename VTKImageObserver::Pointer   m_VTKImageObserver;

private:

  /** Inputs to the State Machine */
  igstkDeclareInputMacro( ValidImageSpatialObject );
  igstkDeclareInputMacro( NullImageSpatialObject );
  igstkDeclareInputMacro( EmptyImageSpatialObject );
  igstkDeclareInputMacro( ConnectVTKPipeline );
  
  /** States for the State Machine */
  igstkDeclareStateMacro( NullImageSpatialObject );
  igstkDeclareStateMacro( ValidImageSpatialObject );

};

} // end namespace igstk

#ifndef IGSTK_MANUAL_INSTANTIATION
#include "igstkImageSpatialObjectVolumeRepresentation.txx"
#endif


#endif // __igstkImageSpatialObjectRepresentation_h
