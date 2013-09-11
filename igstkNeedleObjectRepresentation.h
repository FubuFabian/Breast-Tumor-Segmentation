#ifndef __igstkNeedleObjectRepresentation_h
#define __igstkNeedleObjectRepresentation_h


#include "igstkMacros.h"
#include "igstkObjectRepresentation.h"
#include "igstkNeedleObject.h"
#include "igstkStateMachine.h"

namespace igstk
{

/** \class NeedleObjectRepresentation
 *
 * \brief This class represents a Needle object.
 * The representation of the geometric model of the needle is done using
 * VTK implicit functions combined with the Marching Cube algorithm.
 * Boolean operators are also used on the implicit functions to obtain
 * a nice looking shape.
 *

 */

class NeedleObjectRepresentation
: public ObjectRepresentation
{

public:

  /** Macro with standard traits declarations. */
  igstkStandardClassTraitsMacro( NeedleObjectRepresentation,
                                 ObjectRepresentation )

public:

  /** Typedefs */
  typedef NeedleObject  NeedleSpatialObjectType;

  /** Return a copy of the current object representation */
  Pointer Copy() const;

  /** Connect this representation class to the spatial object */
  void RequestSetNeedleObject( const
                   NeedleSpatialObjectType * NeedleObject );

protected:

  NeedleObjectRepresentation( void );
  virtual ~NeedleObjectRepresentation( void );

  NeedleObjectRepresentation(const Self&); //purposely not implemented
  void operator=(const Self&);                      //purposely not implemented


  /** Print object information */
  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const;

  /** Create the VTK actors */
  void CreateActors();

private:

  /** Internal itkSpatialObject */
  NeedleSpatialObjectType::ConstPointer
                                               m_NeedleSpatialObject;

  /** update the visual representation with changes in the geometry */
  virtual void UpdateRepresentationProcessing();

  /** Connect this representation class to the spatial object. Only to be
   * called by the State Machine. */
  void SetNeedleObjectProcessing();

  /** Method for performing a null operation during a
   *  State Machine transition */
  void NoProcessing();

private:


  /** Inputs to the State Machine */
  igstkDeclareInputMacro( ValidNeedleObject );
  igstkDeclareInputMacro( NullNeedleObject );

  /** States for the State Machine */
  igstkDeclareStateMacro( NullNeedleObject );
  igstkDeclareStateMacro( ValidNeedleObject );

  NeedleSpatialObjectType::ConstPointer m_NeedleObjectToAdd;

};


} // end namespace igstk

#endif // NEEDLEOBJECTREPRESENTATION_H
