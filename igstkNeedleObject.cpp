/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkNeedleObject.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "igstkNeedleObject.h"

namespace igstk
{

/** Constructor */
NeedleObject::NeedleObject():m_StateMachine(this)
{
  m_NeedleSpatialObject = NeedleSpatialObjectType::New();
  this->RequestSetInternalSpatialObject( m_NeedleSpatialObject );
}

/** Destructor */
NeedleObject::~NeedleObject()
{
}

/** Print object information */
void NeedleObject
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace igstk

