#ifndef IGSTKPOLARISPOINTEROBJECT_H
#define IGSTKPOLARISPOINTEROBJECT_H

/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkPolarisPointerObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "igstkMacros.h"
#include "igstkSpatialObject.h"
#include "itkGroupSpatialObject.h"

namespace igstk
{

/** \class PolarisPointerObject
 *
 * \brief This class represents a PolarisPointer object.
 * \warning For the moment this class does not offer any parameter tuning
 * such as height and width.
 * The actual representation of the class is done via the itkGroupSpatialObject
 *
 * Default representation axis is Z.
 *
 * \ingroup Object
 */

class PolarisPointerObject
: public SpatialObject
{

public:

  /** Macro with standard traits declarations. */
  igstkStandardClassTraitsMacro( PolarisPointerObject, SpatialObject )

protected:

  PolarisPointerObject( void );
  ~PolarisPointerObject( void );

  /** Print object information */
  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const;

private:

  /** Typedefs */
  typedef itk::GroupSpatialObject<3>     PolarisPointerSpatialObjectType;

  PolarisPointerObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  PolarisPointerSpatialObjectType::Pointer m_PolarisPointerSpatialObject;

};

} // end namespace igstk

#endif // IGSTKPOLARISPOINTEROBJECT_H
