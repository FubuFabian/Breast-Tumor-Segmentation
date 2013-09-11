/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkUSProbeObjectRepresentation.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __igstkUSProbeObjectRepresentation_h
#define __igstkUSProbeObjectRepresentation_h

#include "igstkMacros.h"
#include "igstkObjectRepresentation.h"
#include "igstkUSProbeObject.h"
#include "igstkStateMachine.h"

namespace igstk
{

/** \class USProbeObjectRepresentation
 * 
 * \brief This class represents a USProbe object. 
 * The representation of the geometric model of the probe is done using
 * VTK implicit functions combined with the Marching Cube algorithm.
 * Boolean operators are also used on the implicit functions to obtain
 * a nice looking shape.
 *
 * \image html  igstkUSProbeObjectRepresentation.png
 *             "US Proble Object Representation State Diagram"
 *
 * \image latex igstkUSProbeObjectRepresentation.eps
 *             "US Proble Object Representation State Diagram"
 *

 *
 *
 * \ingroup ObjectRepresentation
 */

class USProbeObjectRepresentation 
: public ObjectRepresentation
{

public:

  /** Macro with standard traits declarations. */
  igstkStandardClassTraitsMacro( USProbeObjectRepresentation, 
                                 ObjectRepresentation )

public:

  /** Typedefs */
  typedef USProbeObject  USProbeSpatialObjectType;

  /** Return a copy of the current object representation */
  Pointer Copy() const;

  /** Connect this representation class to the spatial object */
  void RequestSetUSProbeObject( const 
                   USProbeSpatialObjectType * USProbeObject );

protected:

  USProbeObjectRepresentation( void );
  virtual ~USProbeObjectRepresentation( void );
  
  USProbeObjectRepresentation(const Self&); //purposely not implemented
  void operator=(const Self&);                      //purposely not implemented


  /** Print object information */
  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const; 

  /** Create the VTK actors */
  void CreateActors();

private:

  /** Internal itkSpatialObject */
  USProbeSpatialObjectType::ConstPointer  
                                               m_USProbeSpatialObject;

  /** update the visual representation with changes in the geometry */
  virtual void UpdateRepresentationProcessing();

  /** Connect this representation class to the spatial object. Only to be
   * called by the State Machine. */
  void SetUSProbeObjectProcessing(); 

  /** Method for performing a null operation during a 
   *  State Machine transition */
  void NoProcessing();

private:

 
  /** Inputs to the State Machine */
  igstkDeclareInputMacro( ValidUSProbeObject );
  igstkDeclareInputMacro( NullUSProbeObject );
  
  /** States for the State Machine */
  igstkDeclareStateMacro( NullUSProbeObject );
  igstkDeclareStateMacro( ValidUSProbeObject );

  USProbeSpatialObjectType::ConstPointer m_USProbeObjectToAdd;

};


} // end namespace igstk

#endif // __igstkUSProbeObjectRepresentation_h
