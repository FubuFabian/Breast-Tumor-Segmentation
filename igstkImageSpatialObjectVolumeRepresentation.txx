/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    $RCSfile: igstkImageSpatialObjectVolumeRepresentation.txx,v $
  Language:  C++
  Date:      $Date: 2007/04/30 15:09:55 $
  Version:   $Revision: 1.9 $

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __igstkImageSpatialObjectVolumeRepresentation_txx
#define __igstkImageSpatialObjectVolumeRepresentation_txx

#include "igstkImageSpatialObjectVolumeRepresentation.h"
#include "stdio.h"


namespace igstk
{

/** Constructor */
template < class TImageSpatialObject >
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::ImageSpatialObjectVolumeRepresentation():m_StateMachine(this)
{
  // We create the image spatial object
  m_ImageSpatialObject = NULL;

  this->RequestSetSpatialObject( m_ImageSpatialObject );

  // Create classes for displaying images
  m_ImageActor = vtkVolume::New();
  this->AddActor( m_ImageActor );
  m_ImageData  = NULL;
  m_ImageData2  = NULL;  

  // Create the observer to VTK image events 
  m_VTKImageObserver = VTKImageObserver::New();

  m_MinThreshold = 0;
  //Eric
//  m_MaxThreshold = 2048;
//  m_ShiftBy = 1024;
  m_MaxThreshold = 255;
  m_ShiftBy = 0;


  m_ShiftScale = vtkImageShiftScale::New();
  m_VolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
  m_VolumeMapper   = vtkSmartPointer<vtkVolumeRayCastMapper>::New();

  igstkAddInputMacro( ValidImageSpatialObject );
  igstkAddInputMacro( NullImageSpatialObject  );
  igstkAddInputMacro( EmptyImageSpatialObject  );
  igstkAddInputMacro( ConnectVTKPipeline );

  igstkAddStateMacro( NullImageSpatialObject );
  igstkAddStateMacro( ValidImageSpatialObject );
 
  igstkAddTransitionMacro( NullImageSpatialObject, NullImageSpatialObject,
                           NullImageSpatialObject,  No );
  igstkAddTransitionMacro( NullImageSpatialObject, EmptyImageSpatialObject,
                           NullImageSpatialObject,  No );
  igstkAddTransitionMacro( NullImageSpatialObject, ValidImageSpatialObject,
                           ValidImageSpatialObject,  SetImageSpatialObject );
  
  igstkAddTransitionMacro( ValidImageSpatialObject, NullImageSpatialObject,
                           NullImageSpatialObject,  No ); 
  igstkAddTransitionMacro( ValidImageSpatialObject, EmptyImageSpatialObject,
                           NullImageSpatialObject,  No ); 
  igstkAddTransitionMacro( ValidImageSpatialObject, ValidImageSpatialObject,
                           ValidImageSpatialObject,  No ); 
  
  igstkAddTransitionMacro( NullImageSpatialObject, ConnectVTKPipeline,
                           NullImageSpatialObject, No );
  igstkAddTransitionMacro( ValidImageSpatialObject, ConnectVTKPipeline,
                           ValidImageSpatialObject, ConnectVTKPipeline );

  igstkSetInitialStateMacro( NullImageSpatialObject );

  m_StateMachine.SetReadyToRun();
} 

/** Destructor */
template < class TImageSpatialObject >
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::~ImageSpatialObjectVolumeRepresentation()  
{

  // This deletes also the m_ImageActor
  this->DeleteActors();

  if(m_OpacityTransferFunction)
    {
    m_OpacityTransferFunction->Delete();
    }
  if(m_ColorTransferFunction)
    {
    m_ColorTransferFunction->Delete();
    }
  if(m_VolumeProperty)
    {
    m_VolumeProperty->Delete();
    }
  if(m_VolumeMapper)
    {
    m_VolumeMapper->Delete();
    }
}

/** Overloaded DeleteActor function */
template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::DeleteActors( )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::DeleteActors called...\n");
  
  this->Superclass::DeleteActors();
  
  m_ImageActor = NULL;

}
 
/** Set the Image Spatial Object */
template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::RequestSetImageSpatialObject( const ImageSpatialObjectType * image )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::RequestSetImageSpatialObject called...\n");
  
  m_ImageSpatialObjectToAdd = image;

  if( !m_ImageSpatialObjectToAdd )
    {
    m_StateMachine.PushInput( m_NullImageSpatialObjectInput );
//    printf("NullImageSpatialObjectInput\n");
    }
  else 
    {
    if( m_ImageSpatialObjectToAdd->IsEmpty() )
      {
      m_StateMachine.PushInput( m_ValidImageSpatialObjectInput );
//    printf("EmptyImageSpatialObjectInput\n");
      }
    else
      {
      m_StateMachine.PushInput( m_ValidImageSpatialObjectInput );
//    printf("ValidImageSpatialObjectInput\n");
      }
    }
  
  m_StateMachine.ProcessInputs();
}


//Eric
/** Set the Image Spatial Object */
template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::RequestSetImageSpatialObject( )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::RequestSetImageSpatialObject called...\n");

  m_StateMachine.PushInput( m_ValidImageSpatialObjectInput );
  m_StateMachine.ProcessInputs();
}
//


/** Null Operation for a State Machine Transition */
template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::NoProcessing()
{
}

/** Set the Image Spatial Object */
template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::SetImageSpatialObjectProcessing()
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::SetImageSpatialObjectProcessing called...\n");

  // We create the image spatial object
//////  m_ImageSpatialObject = m_ImageSpatialObjectToAdd;

//////  m_ImageSpatialObject->AddObserver( VTKImageModifiedEvent(), 
//////                                     m_VTKImageObserver );

//////  this->RequestSetSpatialObject( m_ImageSpatialObject );
  
  // This method gets a VTK image data from the private method of the
  // ImageSpatialObject and stores it in the representation by invoking the
  // private SetImage method.
  
//////  this->m_ImageSpatialObject->RequestGetVTKImage();

//////  if( this->m_VTKImageObserver->GotVTKImage() ) 
    {
//    m_ImageData = this->m_VTKImageObserver->GetVTKImage();
//    m_ImageData2 = this->m_VTKImageObserver->GetVTKImage();
//    m_ImageData = m_ImageData2;
	m_ImageData=m_ImageSpatialObjectToAdd->volumeData;
    if( m_ImageData )
      {
      //printf("\nUpdating image. ");
      m_ImageData->Update();
      //printf("Done.\n");
		int dims[3];
		m_ImageData->GetDimensions(dims);
		unsigned long sizevol=m_ImageData->GetActualMemorySize();
		printf("Vol dimensions: %d %d %d\n",dims[0],dims[1],dims[2]);
		printf("Vol size: %d KB\n",sizevol);
      }
    }

  //m_ShiftScale->SetInput( m_ImageData );
  m_VolumeMapper->SetInput( m_ImageData );
  m_ImageActor->SetMapper(m_VolumeMapper);
  m_ImageActor->SetProperty(m_VolumeProperty);
}


/** Print Self function */
template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}


/** Update the visual representation in response to changes in the geometric
 * object */
template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::UpdateRepresentationProcessing()
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                       ::UpdateRepresentationProcessing called...\n");
  if( m_ImageData )
    {
    m_ShiftScale->SetInput( m_ImageData );
    m_VolumeMapper->SetInput( m_ShiftScale->GetOutput() ); // FIXME
    }
}


/** Create the vtk Actors */
template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::CreateActors()
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::CreateActors called...\n");

  // to avoid duplicates we clean the previous actors
  this->DeleteActors();

  m_ImageActor = vtkVolume::New();
  this->AddActor( m_ImageActor );

    vtkSmartPointer<vtkPiecewiseFunction> m_OpacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    m_OpacityTransferFunction->AddPoint(0,0.00);
    int opacityPoint = 127;
    m_OpacityTransferFunction->AddPoint(opacityPoint,1.00);
    m_OpacityTransferFunction->AddPoint(255,0.00);
    m_OpacityTransferFunction->Update();
     m_VolumeProperty->SetScalarOpacity(m_OpacityTransferFunction);

    vtkSmartPointer<vtkColorTransferFunction> m_ColorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    m_ColorTransferFunction->AddRGBPoint(0,0.0,0.0,0.0);
    m_ColorTransferFunction->AddRGBPoint(64,0.25,0.25,0.25);
    m_ColorTransferFunction->AddRGBPoint(128,0.5,0.5,0.5);
    m_ColorTransferFunction->AddRGBPoint(192,0.75,0.75,0.75);
    m_ColorTransferFunction->AddRGBPoint(255,1.0,1.0,1.0);
     m_VolumeProperty->SetColor(m_ColorTransferFunction);

    vtkSmartPointer<vtkVolumeRayCastCompositeFunction> compositeFunction =
            vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();

    vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
    m_VolumeMapper->SetVolumeRayCastFunction(compositeFunction);
    m_VolumeMapper->CroppingOff();
    m_VolumeMapper->SetInput(m_ImageData);
  
  /*
  //m_ShiftScale = vtkImageShiftScale::New();
  //m_ShiftScale->SetInput( m_ImageData );
  //m_ShiftScale->SetShift( m_ShiftBy );
  //m_ShiftScale->SetOutputScalarTypeToUnsignedShort();

  //m_VolumeMapper = vtkVolumeTextureMapper3D::New();
  //m_VolumeMapper->SetInput(m_ShiftScale->GetOutput());

  //m_OpacityTransferFunction = vtkPiecewiseFunction::New();
  //m_OpacityTransferFunction->AddPoint(0, 0.0);
  //if( m_MinThreshold > 0 )
  //  {
  //  m_OpacityTransferFunction->AddPoint(m_MinThreshold, 0.0);//0.05
  //  }
////  m_OpacityTransferFunction->AddPoint(m_MaxThreshold, 0.2);//0.1
////  m_OpacityTransferFunction->AddPoint(m_MaxThreshold+1, 0.2);
//  m_OpacityTransferFunction->AddPoint(m_MaxThreshold, VolumeOpacity);
//  m_OpacityTransferFunction->AddPoint(m_MaxThreshold+1, VolumeOpacity);

  //m_OpacityTransferFunction->AddPoint(m_MinThreshold, 0.0);
  m_OpacityTransferFunction->AddPoint(m_MaxThreshold*(2.0/3.0), VolumeOpacity);
  m_OpacityTransferFunction->AddPoint(m_MaxThreshold, 0.5*VolumeOpacity);

  m_ColorTransferFunction = vtkColorTransferFunction::New();
  m_ColorTransferFunction->AddRGBPoint(0, 0, 0, 1);
  m_ColorTransferFunction->AddRGBPoint(m_MaxThreshold/2, 0, 1, 0);
  m_ColorTransferFunction->AddRGBPoint(m_MaxThreshold, 1, 0, 0);
 
  m_ColorTransferFunction->AddRGBPoint(m_MaxThreshold/4, 1, 0, 0);
  m_ColorTransferFunction->AddRGBPoint(m_MaxThreshold/2, 0, 0, 1);
  m_ColorTransferFunction->AddRGBPoint(m_MaxThreshold/4*3, 0, 1, 0);


  m_GrayScaleTransferFunction=vtkPiecewiseFunction::New();
  m_GrayScaleTransferFunction->AddPoint(0,0.0);
  m_GrayScaleTransferFunction->AddPoint(255,1.0);
  
    m_VolumeProperty = vtkVolumeProperty::New();
  //m_VolumeProperty->SetColor(m_ColorTransferFunction);
  m_VolumeProperty->SetColor(m_GrayScaleTransferFunction);
  m_VolumeProperty->SetScalarOpacity(m_OpacityTransferFunction);*/
  

  igstkPushInputMacro( ConnectVTKPipeline );
  m_StateMachine.ProcessInputs(); 
}

template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::changeVolumeOpacity(vtkSmartPointer<vtkPiecewiseFunction> opacity)
{
	m_VolumeProperty->SetScalarOpacity(opacity);
}


/** Create a copy of the current object representation */
template < class TImageSpatialObject >
typename ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >::Pointer
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::Copy() const
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::Copy called...\n");

  Pointer newOR = 
         ImageSpatialObjectVolumeRepresentation<TImageSpatialObject>::New();
  newOR->SetColor(this->GetRed(),this->GetGreen(),this->GetBlue()); //FIXME
  newOR->SetOpacity(this->GetOpacity());
  newOR->RequestSetImageSpatialObject(m_ImageSpatialObject);

  return newOR;
}

template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::SetImage( const vtkImageData * image )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::SetImage called...\n");

  // This const_cast<> is needed here due to the lack of 
  // const-correctness in VTK 
  m_ImageData = const_cast< vtkImageData *>( image );
}


template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::ConnectVTKPipelineProcessing(){
  m_ImageActor->SetMapper(m_VolumeMapper);
  m_ImageActor->SetProperty(m_VolumeProperty);
  m_ImageActor->SetVisibility( 1 );
  m_ImageActor->SetPickable( 0 );
}

//Eric
template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::UpdateVTKPipelineProcessing(){
//  m_ImageActor->SetMapper(m_VolumeMapper);
  m_ImageActor->SetProperty(m_VolumeProperty);
//  m_ImageActor->SetVisibility( 1 );
//  m_ImageActor->SetPickable( 0 );
}

} // end namespace igstk

#endif
