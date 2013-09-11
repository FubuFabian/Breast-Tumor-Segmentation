/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkPolarisPointerObject.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "igstkPolarisPointerObject.h"

namespace igstk
{

/** Constructor */
PolarisPointerObject::PolarisPointerObject():m_StateMachine(this)
{
  m_PolarisPointerSpatialObject = PolarisPointerSpatialObjectType::New();
  this->RequestSetInternalSpatialObject( m_PolarisPointerSpatialObject );
}

/** Destructor */
PolarisPointerObject::~PolarisPointerObject()
{
}

/** Print object information */
void PolarisPointerObject
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace igstk

