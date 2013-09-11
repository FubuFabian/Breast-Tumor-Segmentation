/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkUSProbeObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __igstkUSProbeObject_h
#define __igstkUSProbeObject_h

#include "igstkMacros.h"
#include "igstkSpatialObject.h"
#include "itkGroupSpatialObject.h"

namespace igstk
{

/** \class USProbeObject
 * 
 * \brief This class represents a USProbe object. 
 * \warning For the moment this class does not offer any parameter tuning
 * such as height and width.
 * The actual representation of the class is done via the itkGroupSpatialObject 
 *
 * Default representation axis is Z.
 *
 * \ingroup Object
 */

class USProbeObject 
: public SpatialObject
{

public:

  /** Macro with standard traits declarations. */
  igstkStandardClassTraitsMacro( USProbeObject, SpatialObject )

protected:

  USProbeObject( void );
  ~USProbeObject( void );

  /** Print object information */
  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const; 

private:
 
  /** Typedefs */
  typedef itk::GroupSpatialObject<3>     USProbeSpatialObjectType;

  USProbeObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  USProbeSpatialObjectType::Pointer m_USProbeSpatialObject;

};

} // end namespace igstk

#endif // __igstkUSProbeObject_h
