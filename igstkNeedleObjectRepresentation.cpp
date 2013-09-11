/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkNeedleObjectRepresentation.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "igstkNeedleObjectRepresentation.h"

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
NeedleObjectRepresentation
::NeedleObjectRepresentation():m_StateMachine(this)
{
  // We create the ellipse spatial object
  m_NeedleSpatialObject = NULL;
  this->RequestSetSpatialObject( m_NeedleSpatialObject );

  igstkAddInputMacro( ValidNeedleObject );
  igstkAddInputMacro( NullNeedleObject  );

  igstkAddStateMacro( NullNeedleObject   );
  igstkAddStateMacro( ValidNeedleObject  );

  igstkAddTransitionMacro( NullNeedleObject,
                           NullNeedleObject,
                           NullNeedleObject,  No );
  igstkAddTransitionMacro( NullNeedleObject,
                           ValidNeedleObject,
                           ValidNeedleObject,
                           SetNeedleObject );
  igstkAddTransitionMacro( ValidNeedleObject,
                           NullNeedleObject,
                           NullNeedleObject,  No );
  igstkAddTransitionMacro( ValidNeedleObject,
                           ValidNeedleObject,
                           ValidNeedleObject,  No );

  igstkSetInitialStateMacro( NullNeedleObject );

  m_StateMachine.SetReadyToRun();
}

/** Destructor */
NeedleObjectRepresentation::~NeedleObjectRepresentation()
{
  this->DeleteActors();
}

/** Set the Needleal Spatial Object */
void NeedleObjectRepresentation
::RequestSetNeedleObject( const NeedleSpatialObjectType *
                                                            Needle )
{
  m_NeedleObjectToAdd = Needle;
  if( !m_NeedleObjectToAdd )
    {
    m_StateMachine.PushInput( m_NullNeedleObjectInput );
    m_StateMachine.ProcessInputs();
    }
  else
    {
    m_StateMachine.PushInput( m_ValidNeedleObjectInput );
    m_StateMachine.ProcessInputs();
    }
}

/** Set the Needle Spatial Object */
void NeedleObjectRepresentation::NoProcessing()
{
}

/** Set the Needle Spatial Object */
void NeedleObjectRepresentation::SetNeedleObjectProcessing()
{
  m_NeedleSpatialObject = m_NeedleObjectToAdd;
  this->RequestSetSpatialObject( m_NeedleSpatialObject );
}

/** Print Self function */
void NeedleObjectRepresentation
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}

/** Update the visual representation in response to changes in the geometric
 * object */
void NeedleObjectRepresentation::UpdateRepresentationProcessing()
{
  igstkLogMacro( DEBUG, "UpdateRepresentationProcessing called ....\n");
}

/** Create the vtk Actors */
void NeedleObjectRepresentation::CreateActors()
{
    this->DeleteActors();

	//Create the tip of the needle
	vtkPolyDataMapper* tipSphereMapper = vtkPolyDataMapper::New();
    vtkActor* tipSphereActor = vtkActor::New();

    vtkSphereSource* tipSphere = vtkSphereSource::New();
    tipSphere->SetRadius(1.0);
    tipSphere->SetCenter(0,0,0);

	tipSphereActor->GetProperty()->SetColor(1.0,0.0,0.0);
    tipSphereActor->GetProperty()->SetOpacity(this->GetOpacity());

    tipSphereMapper->SetInputConnection(tipSphere->GetOutputPort());
    tipSphereActor->SetMapper( tipSphereMapper );

    this->AddActor( tipSphereActor );
    tipSphereMapper->Delete();
	tipSphere->Delete();

	vtkPolyDataMapper* tipCylinderMapper = vtkPolyDataMapper::New();
    vtkActor* tipCylinderActor = vtkActor::New();

    vtkCylinderSource* tipCylinder = vtkCylinderSource::New();
    tipCylinder->SetRadius(0.6);
	tipCylinder->SetHeight(45);
    tipCylinder->SetCenter(0,23,0);

	tipCylinderActor->GetProperty()->SetColor(0.8,0.8,0.8);
    tipCylinderActor->GetProperty()->SetOpacity(this->GetOpacity());

    tipCylinderMapper->SetInputConnection(tipCylinder->GetOutputPort());
    tipCylinderActor->SetMapper( tipCylinderMapper );

	this->AddActor( tipCylinderActor );
	tipCylinderMapper->Delete();
    tipCylinder->Delete();

	//Create the body of the needle
	vtkPolyDataMapper* bodyCylinder1Mapper = vtkPolyDataMapper::New();
    vtkActor* bodyCylinder1Actor = vtkActor::New();

    vtkCylinderSource* bodyCylinder1 = vtkCylinderSource::New();
    bodyCylinder1->SetRadius(4.5);
	bodyCylinder1->SetHeight(8);
    bodyCylinder1->SetCenter(0,49,0);

	bodyCylinder1Actor->GetProperty()->SetColor(0.325,0.658,0.584);
    bodyCylinder1Actor->GetProperty()->SetOpacity(this->GetOpacity());

    bodyCylinder1Mapper->SetInputConnection(bodyCylinder1->GetOutputPort());
    bodyCylinder1Actor->SetMapper( bodyCylinder1Mapper );

	this->AddActor( bodyCylinder1Actor );
	bodyCylinder1Mapper->Delete();
    bodyCylinder1->Delete();

	vtkPolyDataMapper* bodyCylinder2Mapper = vtkPolyDataMapper::New();
    vtkActor* bodyCylinder2Actor = vtkActor::New();

    vtkCylinderSource* bodyCylinder2 = vtkCylinderSource::New();
    bodyCylinder2->SetRadius(8);
	bodyCylinder2->SetHeight(87);
    bodyCylinder2->SetCenter(0,96.5,0);

	bodyCylinder2Actor->GetProperty()->SetColor(0.490,0.701,0.768);
    bodyCylinder2Actor->GetProperty()->SetOpacity(this->GetOpacity());

    bodyCylinder2Mapper->SetInputConnection(bodyCylinder2->GetOutputPort());
    bodyCylinder2Actor->SetMapper( bodyCylinder2Mapper );

	this->AddActor( bodyCylinder2Actor );
	bodyCylinder2Mapper->Delete();
    bodyCylinder2->Delete();

	vtkPolyDataMapper* bodyCubeMapper = vtkPolyDataMapper::New();
    vtkActor* bodyCubeActor = vtkActor::New();

	vtkCubeSource* bodyCube = vtkCubeSource::New();
	bodyCube->SetCenter(0,142,0);
	bodyCube->SetYLength(2);
	bodyCube->SetXLength(32);
	bodyCube->SetZLength(18);
	
	bodyCubeActor->GetProperty()->SetColor(0.247,0.462,0.545);
    bodyCubeActor->GetProperty()->SetOpacity(this->GetOpacity());

    bodyCubeMapper->SetInputConnection(bodyCube->GetOutputPort());
    bodyCubeActor->SetMapper( bodyCubeMapper );

	this->AddActor( bodyCubeActor );
	bodyCubeMapper->Delete();
    bodyCube->Delete();
	
}


/** Create a copy of the current object representation */
NeedleObjectRepresentation::Pointer
NeedleObjectRepresentation::Copy() const
{
  Pointer newOR = NeedleObjectRepresentation::New();
  newOR->SetColor(this->GetRed(),this->GetGreen(),this->GetBlue());
  newOR->SetOpacity(this->GetOpacity());
  newOR->RequestSetNeedleObject(m_NeedleSpatialObject);

  return newOR;
}


} // end namespace igstk


