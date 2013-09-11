/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkUSProbeObjectRepresentation.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "igstkUSProbeObjectRepresentation.h"

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkImplicitBoolean.h>
#include <vtkSampleFunction.h>
#include <vtkMarchingContourFilter.h>
#include <vtkCylinderSource.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>


namespace igstk
{ 

/** Constructor */
USProbeObjectRepresentation
::USProbeObjectRepresentation():m_StateMachine(this)
{
  // We create the ellipse spatial object
  m_USProbeSpatialObject = NULL;
  this->RequestSetSpatialObject( m_USProbeSpatialObject );
  
  igstkAddInputMacro( ValidUSProbeObject );
  igstkAddInputMacro( NullUSProbeObject  );

  igstkAddStateMacro( NullUSProbeObject   );
  igstkAddStateMacro( ValidUSProbeObject  );

  igstkAddTransitionMacro( NullUSProbeObject, 
                           NullUSProbeObject, 
                           NullUSProbeObject,  No );
  igstkAddTransitionMacro( NullUSProbeObject, 
                           ValidUSProbeObject, 
                           ValidUSProbeObject,  
                           SetUSProbeObject );
  igstkAddTransitionMacro( ValidUSProbeObject, 
                           NullUSProbeObject, 
                           NullUSProbeObject,  No ); 
  igstkAddTransitionMacro( ValidUSProbeObject, 
                           ValidUSProbeObject, 
                           ValidUSProbeObject,  No ); 

  igstkSetInitialStateMacro( NullUSProbeObject );

  m_StateMachine.SetReadyToRun();
} 

/** Destructor */
USProbeObjectRepresentation::~USProbeObjectRepresentation()  
{
  this->DeleteActors();
}

/** Set the USProbeal Spatial Object */
void USProbeObjectRepresentation
::RequestSetUSProbeObject( const USProbeSpatialObjectType * 
                                                            USProbe )
{
  m_USProbeObjectToAdd = USProbe;
  if( !m_USProbeObjectToAdd )
    {
    m_StateMachine.PushInput( m_NullUSProbeObjectInput );
    m_StateMachine.ProcessInputs();
    }
  else
    {
    m_StateMachine.PushInput( m_ValidUSProbeObjectInput );
    m_StateMachine.ProcessInputs();
    }
}

/** Set the USProbe Spatial Object */
void USProbeObjectRepresentation::NoProcessing()
{
}

/** Set the USProbe Spatial Object */
void USProbeObjectRepresentation::SetUSProbeObjectProcessing()
{
  m_USProbeSpatialObject = m_USProbeObjectToAdd;
  this->RequestSetSpatialObject( m_USProbeSpatialObject );
} 

/** Print Self function */
void USProbeObjectRepresentation
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}

/** Update the visual representation in response to changes in the geometric
 * object */
void USProbeObjectRepresentation::UpdateRepresentationProcessing()
{
  igstkLogMacro( DEBUG, "UpdateRepresentationProcessing called ....\n");
}

/** Create the vtk Actors */
void USProbeObjectRepresentation::CreateActors()
{
  // to avoid duplicates we clean the previous actors
  this->DeleteActors();
   
  //Create image plane
  vtkPolyDataMapper* imageCubeMapper = vtkPolyDataMapper::New();
  vtkActor* imageCubeActor = vtkActor::New();

  vtkCubeSource* imageCube = vtkCubeSource::New();
  imageCube->SetCenter(20,-20,0);
  imageCube->SetXLength(40);
  imageCube->SetYLength(40);
  imageCube->SetZLength(4);
	
  imageCubeActor->GetProperty()->SetColor(0.5,0.5,0.67);
  imageCubeActor->GetProperty()->SetOpacity(this->GetOpacity());

  imageCubeMapper->SetInputConnection(imageCube->GetOutputPort());
  imageCubeActor->SetMapper( imageCubeMapper );

  this->AddActor( imageCubeActor );
  imageCubeMapper->Delete();
  imageCube->Delete(); 

  //Create probe body
  vtkPolyDataMapper* bodyCube1Mapper = vtkPolyDataMapper::New();
  vtkActor* bodyCube1Actor = vtkActor::New();

  vtkCubeSource* bodyCube1 = vtkCubeSource::New();
  bodyCube1->SetCenter(20,2,0);
  bodyCube1->SetXLength(56);
  bodyCube1->SetYLength(4);
  bodyCube1->SetZLength(23);
	
  bodyCube1Actor->GetProperty()->SetColor(0.4,0.4,0.4);
  bodyCube1Actor->GetProperty()->SetOpacity(this->GetOpacity());

  bodyCube1Mapper->SetInputConnection(bodyCube1->GetOutputPort());
  bodyCube1Actor->SetMapper( bodyCube1Mapper );

  this->AddActor( bodyCube1Actor );
  bodyCube1Mapper->Delete();
  bodyCube1->Delete();

  vtkPolyDataMapper* bodyCube2Mapper = vtkPolyDataMapper::New();
  vtkActor* bodyCube2Actor = vtkActor::New();

  vtkCubeSource* bodyCube2 = vtkCubeSource::New();
  bodyCube2->SetCenter(20,17,0);
  bodyCube2->SetXLength(60);
  bodyCube2->SetYLength(30);
  bodyCube2->SetZLength(25);
	
  bodyCube2Actor->GetProperty()->SetColor(0.8,0.8,0.8);
  bodyCube2Actor->GetProperty()->SetOpacity(this->GetOpacity());

  bodyCube2Mapper->SetInputConnection(bodyCube2->GetOutputPort());
  bodyCube2Actor->SetMapper( bodyCube2Mapper );

  this->AddActor( bodyCube2Actor );
  bodyCube2Mapper->Delete();
  bodyCube2->Delete();

  vtkPolyDataMapper* bodyCube3Mapper = vtkPolyDataMapper::New();
  vtkActor* bodyCube3Actor = vtkActor::New();

  vtkCubeSource* bodyCube3 = vtkCubeSource::New();
  bodyCube3->SetCenter(20,34,0);
  bodyCube3->SetXLength(42);
  bodyCube3->SetYLength(4);
  bodyCube3->SetZLength(23);
	
  bodyCube3Actor->GetProperty()->SetColor(0.85,0.85,0.85);
  bodyCube3Actor->GetProperty()->SetOpacity(this->GetOpacity());

  bodyCube3Mapper->SetInputConnection(bodyCube3->GetOutputPort());
  bodyCube3Actor->SetMapper( bodyCube3Mapper );

  this->AddActor( bodyCube3Actor );
  bodyCube3Mapper->Delete();
  bodyCube3->Delete();

  vtkPolyDataMapper* bodyCube4Mapper = vtkPolyDataMapper::New();
  vtkActor* bodyCube4Actor = vtkActor::New();

  vtkCubeSource* bodyCube4 = vtkCubeSource::New();
  bodyCube4->SetCenter(20,60,0);
  bodyCube4->SetXLength(34);
  bodyCube4->SetYLength(52);
  bodyCube4->SetZLength(20);
	
  bodyCube4Actor->GetProperty()->SetColor(0.9,0.9,0.9);
  bodyCube4Actor->GetProperty()->SetOpacity(this->GetOpacity());

  bodyCube4Mapper->SetInputConnection(bodyCube4->GetOutputPort());
  bodyCube4Actor->SetMapper( bodyCube4Mapper );

  this->AddActor( bodyCube4Actor );
  bodyCube4Mapper->Delete();
  bodyCube4->Delete();

  vtkPolyDataMapper* bodyCylinder1Mapper = vtkPolyDataMapper::New();
  vtkActor* bodyCylinder1Actor = vtkActor::New();

  vtkCylinderSource* bodyCylinder1 = vtkCylinderSource::New();
  bodyCylinder1->SetCenter(20,89,0);
  bodyCylinder1->SetHeight(6);
  bodyCylinder1->SetRadius(10);

  bodyCylinder1Actor->GetProperty()->SetColor(0.2,0.388,0.592);
  bodyCylinder1Actor->GetProperty()->SetOpacity(this->GetOpacity());
  //bodyCylinder1Actor->SetOrigin(20,-89,0);
  //bodyCylinder1Actor->SetOrientation(0,0,0);

  bodyCylinder1Mapper->SetInputConnection(bodyCylinder1->GetOutputPort());
  bodyCylinder1Actor->SetMapper( bodyCylinder1Mapper );

  this->AddActor( bodyCylinder1Actor );
  bodyCylinder1Mapper->Delete();
  bodyCylinder1->Delete();

  vtkPolyDataMapper* bodyCylinder2Mapper = vtkPolyDataMapper::New();
  vtkActor* bodyCylinder2Actor = vtkActor::New();

  vtkCylinderSource* bodyCylinder2 = vtkCylinderSource::New();
  bodyCylinder2->SetCenter(20,104.5,0);
  bodyCylinder2->SetHeight(25);
  bodyCylinder2->SetRadius(7.5);

  bodyCylinder2Actor->GetProperty()->SetColor(0.9,0.9,0.9);
  bodyCylinder2Actor->GetProperty()->SetOpacity(this->GetOpacity());
  //bodyCylinder2Actor->SetOrigin(20,0,104.5);
  //bodyCylinder2Actor->SetOrientation(90,0,0);

  bodyCylinder2Mapper->SetInputConnection(bodyCylinder2->GetOutputPort());
  bodyCylinder2Actor->SetMapper( bodyCylinder2Mapper );

  this->AddActor( bodyCylinder2Actor );
  bodyCylinder2Mapper->Delete();
  bodyCylinder2->Delete();

}

/** Create a copy of the current object representation */
USProbeObjectRepresentation::Pointer
USProbeObjectRepresentation::Copy() const
{
  Pointer newOR = USProbeObjectRepresentation::New();
  newOR->SetColor(this->GetRed(),this->GetGreen(),this->GetBlue());
  newOR->SetOpacity(this->GetOpacity());
  newOR->RequestSetUSProbeObject(m_USProbeSpatialObject);

  return newOR;
}


} // end namespace igstk
