
/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkPolarisPointerObjectRepresentation.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "igstkPolarisPointerObjectRepresentation.h"

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkImplicitBoolean.h>
#include <vtkSampleFunction.h>
#include <vtkMarchingContourFilter.h>
#include <vtkCylinderSource.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkConeSource.h>

namespace igstk
{

/** Constructor */
PolarisPointerObjectRepresentation
::PolarisPointerObjectRepresentation():m_StateMachine(this)
{
  // We create the ellipse spatial object
  m_PolarisPointerSpatialObject = NULL;
  this->RequestSetSpatialObject( m_PolarisPointerSpatialObject );

  igstkAddInputMacro( ValidPolarisPointerObject );
  igstkAddInputMacro( NullPolarisPointerObject  );

  igstkAddStateMacro( NullPolarisPointerObject   );
  igstkAddStateMacro( ValidPolarisPointerObject  );

  igstkAddTransitionMacro( NullPolarisPointerObject,
                           NullPolarisPointerObject,
                           NullPolarisPointerObject,  No );
  igstkAddTransitionMacro( NullPolarisPointerObject,
                           ValidPolarisPointerObject,
                           ValidPolarisPointerObject,
                           SetPolarisPointerObject );
  igstkAddTransitionMacro( ValidPolarisPointerObject,
                           NullPolarisPointerObject,
                           NullPolarisPointerObject,  No );
  igstkAddTransitionMacro( ValidPolarisPointerObject,
                           ValidPolarisPointerObject,
                           ValidPolarisPointerObject,  No );

  igstkSetInitialStateMacro( NullPolarisPointerObject );

  m_StateMachine.SetReadyToRun();
}

/** Destructor */
PolarisPointerObjectRepresentation::~PolarisPointerObjectRepresentation()
{
  this->DeleteActors();
}

/** Set the PolarisPointeral Spatial Object */
void PolarisPointerObjectRepresentation
::RequestSetPolarisPointerObject( const PolarisPointerSpatialObjectType *
                                                            PolarisPointer )
{
  m_PolarisPointerObjectToAdd = PolarisPointer;
  if( !m_PolarisPointerObjectToAdd )
    {
    m_StateMachine.PushInput( m_NullPolarisPointerObjectInput );
    m_StateMachine.ProcessInputs();
    }
  else
    {
    m_StateMachine.PushInput( m_ValidPolarisPointerObjectInput );
    m_StateMachine.ProcessInputs();
    }
}

/** Set the PolarisPointer Spatial Object */
void PolarisPointerObjectRepresentation::NoProcessing()
{
}

/** Set the PolarisPointer Spatial Object */
void PolarisPointerObjectRepresentation::SetPolarisPointerObjectProcessing()
{
  m_PolarisPointerSpatialObject = m_PolarisPointerObjectToAdd;
  this->RequestSetSpatialObject( m_PolarisPointerSpatialObject );
}

/** Print Self function */
void PolarisPointerObjectRepresentation
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}

/** Update the visual representation in response to changes in the geometric
 * object */
void PolarisPointerObjectRepresentation::UpdateRepresentationProcessing()
{
  igstkLogMacro( DEBUG, "UpdateRepresentationProcessing called ....\n");
}

/** Create the vtk Actors */
void PolarisPointerObjectRepresentation::CreateActors()
{
    this->DeleteActors();

	//Create the tip of the Pointer 
	vtkPolyDataMapper* tipConeMapper = vtkPolyDataMapper::New();
	vtkActor* tipConeActor = vtkActor::New();

	vtkConeSource* tipCone = vtkConeSource::New();
	tipCone->SetCenter(0,0,2);
	tipCone->SetHeight(4);
	tipCone->SetRadius(1.5);
	tipCone->SetDirection(0,0,-1);

	tipConeActor->GetProperty()->SetColor(0.8,0.8,0.8);
	tipConeActor->GetProperty()->SetOpacity(this->GetOpacity());
	tipConeActor->SetOrigin(0,0,0);


	tipConeMapper->SetInputConnection(tipCone->GetOutputPort());
	tipConeActor->SetMapper(tipConeMapper);

	this->AddActor(tipConeActor);
	tipConeMapper->Delete();
	tipCone->Delete();

	vtkPolyDataMapper* tipCylinderMapper = vtkPolyDataMapper::New();
	vtkActor* tipCylinderActor = vtkActor::New();

	vtkCylinderSource* tipCylinder = vtkCylinderSource::New();
	tipCylinder->SetCenter(0,0,73);
	tipCylinder->SetHeight(138);
	tipCylinder->SetRadius(1.5);

	tipCylinderActor->GetProperty()->SetColor(0.8,0.8,0.8);
	tipCylinderActor->GetProperty()->SetOpacity(this->GetOpacity());
	tipCylinderActor->SetOrigin(0,0,73);
	tipCylinderActor->SetOrientation(90,0,0);

	tipCylinderMapper->SetInputConnection(tipCylinder->GetOutputPort());
	tipCylinderActor->SetMapper(tipCylinderMapper);

	this->AddActor(tipCylinderActor);
	tipCylinderMapper->Delete();
	tipCylinder->Delete();

	//Create body of the Pointer
	vtkPolyDataMapper* bodyCube1Mapper = vtkPolyDataMapper::New();
    vtkActor* bodyCube1Actor = vtkActor::New();

    vtkCubeSource* bodyCube1 = vtkCubeSource::New();
    bodyCube1->SetCenter(0,0,199.5);
    bodyCube1->SetYLength(6);
    bodyCube1->SetXLength(9);
    bodyCube1->SetZLength(115);

    bodyCube1Actor->GetProperty()->SetColor(0.0,0.0,0.0);
    bodyCube1Actor->GetProperty()->SetOpacity(this->GetOpacity());

    bodyCube1Mapper->SetInputConnection(bodyCube1->GetOutputPort());
    bodyCube1Actor->SetMapper( bodyCube1Mapper );

    this->AddActor( bodyCube1Actor );
    bodyCube1Mapper->Delete();
    bodyCube1->Delete();

	vtkPolyDataMapper* bodyCube2Mapper = vtkPolyDataMapper::New();
    vtkActor* bodyCube2Actor = vtkActor::New();

    vtkCubeSource* bodyCube2 = vtkCubeSource::New();
    bodyCube2->SetCenter(15,0,277.98);
    bodyCube2->SetYLength(6);
    bodyCube2->SetXLength(9);
    bodyCube2->SetZLength(60);

    bodyCube2Actor->GetProperty()->SetColor(0.0,0.0,0.0);
    bodyCube2Actor->GetProperty()->SetOpacity(this->GetOpacity());
	bodyCube2Actor->SetOrigin(15,0,277.98);
	bodyCube2Actor->SetOrientation(0,30,0);

    bodyCube2Mapper->SetInputConnection(bodyCube2->GetOutputPort());
    bodyCube2Actor->SetMapper( bodyCube2Mapper );

    this->AddActor( bodyCube2Actor );
    bodyCube2Mapper->Delete();
    bodyCube2->Delete();

	vtkPolyDataMapper* bodyCube3Mapper = vtkPolyDataMapper::New();
    vtkActor* bodyCube3Actor = vtkActor::New();

    vtkCubeSource* bodyCube3 = vtkCubeSource::New();
    bodyCube3->SetCenter(-10.6,0,262.6);
    bodyCube3->SetYLength(6);
    bodyCube3->SetXLength(9);
    bodyCube3->SetZLength(30);

    bodyCube3Actor->GetProperty()->SetColor(0.0,0.0,0.0);
    bodyCube3Actor->GetProperty()->SetOpacity(this->GetOpacity());
	bodyCube3Actor->SetOrigin(-10.6,0,262.6);
	bodyCube3Actor->SetOrientation(0,-45,0);

    bodyCube3Mapper->SetInputConnection(bodyCube3->GetOutputPort());
    bodyCube3Actor->SetMapper( bodyCube3Mapper );

    this->AddActor( bodyCube3Actor );
    bodyCube3Mapper->Delete();
    bodyCube3->Delete();

}


/** Create a copy of the current object representation */
PolarisPointerObjectRepresentation::Pointer
PolarisPointerObjectRepresentation::Copy() const
{
  Pointer newOR = PolarisPointerObjectRepresentation::New();
  newOR->SetColor(this->GetRed(),this->GetGreen(),this->GetBlue());
  newOR->SetOpacity(this->GetOpacity());
  newOR->RequestSetPolarisPointerObject(m_PolarisPointerSpatialObject);

  return newOR;
}


} // end namespace igstk


