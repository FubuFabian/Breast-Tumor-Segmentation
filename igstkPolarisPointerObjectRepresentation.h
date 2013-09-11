#ifndef IGSTKPOLARISPOINTEROBJECTREPRESENTATION_H
#define IGSTKPOLARISPOINTEROBJECTREPRESENTATION_H

#include "igstkMacros.h"
#include "igstkObjectRepresentation.h"
#include "igstkPolarisPointerObject.h"
#include "igstkStateMachine.h"

namespace igstk
{

/** \class PolarisPointerObjectRepresentation
 *
 * \brief This class represents a PolarisPointer object.
 * The representation of the geometric model of the PolarisPointer is done using
 * VTK implicit functions combined with the Marching Cube algorithm.
 * Boolean operators are also used on the implicit functions to obtain
 * a nice looking shape.
 *

 */

class PolarisPointerObjectRepresentation
: public ObjectRepresentation
{

public:

  /** Macro with standard traits declarations. */
  igstkStandardClassTraitsMacro( PolarisPointerObjectRepresentation,
                                 ObjectRepresentation )

public:

  /** Typedefs */
  typedef PolarisPointerObject  PolarisPointerSpatialObjectType;

  /** Return a copy of the current object representation */
  Pointer Copy() const;

  /** Connect this representation class to the spatial object */
  void RequestSetPolarisPointerObject( const
                   PolarisPointerSpatialObjectType * PolarisPointerObject );

protected:

  PolarisPointerObjectRepresentation( void );
  virtual ~PolarisPointerObjectRepresentation( void );

  PolarisPointerObjectRepresentation(const Self&); //purposely not implemented
  void operator=(const Self&);                      //purposely not implemented


  /** Print object information */
  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const;

  /** Create the VTK actors */
  void CreateActors();

private:

  /** Internal itkSpatialObject */
  PolarisPointerSpatialObjectType::ConstPointer
                                               m_PolarisPointerSpatialObject;

  /** update the visual representation with changes in the geometry */
  virtual void UpdateRepresentationProcessing();

  /** Connect this representation class to the spatial object. Only to be
   * called by the State Machine. */
  void SetPolarisPointerObjectProcessing();

  /** Method for performing a null operation during a
   *  State Machine transition */
  void NoProcessing();

private:


  /** Inputs to the State Machine */
  igstkDeclareInputMacro( ValidPolarisPointerObject );
  igstkDeclareInputMacro( NullPolarisPointerObject );

  /** States for the State Machine */
  igstkDeclareStateMacro( NullPolarisPointerObject );
  igstkDeclareStateMacro( ValidPolarisPointerObject );

  PolarisPointerSpatialObjectType::ConstPointer m_PolarisPointerObjectToAdd;

};


} // end namespace igstk

#endif // IGSTKPOLARISPOINTEROBJECTREPRESENTATION_H
