/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkUSProbeObject.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "igstkUSProbeObject.h"

namespace igstk
{ 

/** Constructor */
USProbeObject::USProbeObject():m_StateMachine(this)
{
  m_USProbeSpatialObject = USProbeSpatialObjectType::New();
  this->RequestSetInternalSpatialObject( m_USProbeSpatialObject );
} 

/** Destructor */
USProbeObject::~USProbeObject()  
{
}

/** Print object information */
void USProbeObject
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace igstk
