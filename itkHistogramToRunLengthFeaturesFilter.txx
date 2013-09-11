/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHistogramToRunLengthFeaturesFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008/10/18 00:22:58 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkHistogramToRunLengthFeaturesFilter_txx
#define _itkHistogramToRunLengthFeaturesFilter_txx

#include "itkHistogramToRunLengthFeaturesFilter.h"

#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace itk {
namespace Statistics {

//constructor
template<class THistogram>
HistogramToRunLengthFeaturesFilter<THistogram>
::HistogramToRunLengthFeaturesFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs( 1 );

  // allocate the data objects for the outputs which are
  // just decorators real types
  for( unsigned int i = 0; i < 10; i++ )
    {
    this->ProcessObject::SetNthOutput( i, this->MakeOutput( i ) );
    }
  
  this->m_RequestedFeature = 0;
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>
::SetInput( const HistogramType *histogram )
{
  this->ProcessObject::SetNthInput( 0, const_cast<HistogramType*>( histogram ) );
}

template<class THistogram>
const typename
HistogramToRunLengthFeaturesFilter<THistogram>::HistogramType *
HistogramToRunLengthFeaturesFilter< THistogram>
::GetInput() const
{
  if ( this->GetNumberOfInputs() < 1 )
    {
    return 0;
    }
  return static_cast<const HistogramType *>( this->ProcessObject::GetInput( 0 ) );
}

template<class THistogram>
typename
HistogramToRunLengthFeaturesFilter<THistogram>::DataObjectPointer
HistogramToRunLengthFeaturesFilter<THistogram>
::MakeOutput( unsigned int itkNotUsed( idx ) )
{
  return static_cast<DataObject*>(MeasurementObjectType::New().GetPointer());
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateData( void )
{
  
  if(this->m_RequestedFeature == 0){
      this->GenerateShortRunEmphasisData();
  }else if(this->m_RequestedFeature == 1){
      this->GenerateLongRunEmphasisData();
  }else if(this->m_RequestedFeature == 2){
      this->GenerateGreyLevelNonuniformityData();
  }else if(this->m_RequestedFeature == 3){
      this->GenerateRunLengthNonuniformityData();
  }else if(this->m_RequestedFeature == 4){
      this->GenerateLowGreyLevelRunEmphasisData();
  }else if(this->m_RequestedFeature == 5){
      this->GenerateHighGreyLevelRunEmphasisData();
  }else if(this->m_RequestedFeature == 6){
      this->GenerateShortRunLowGreyLevelEmphasisData();
  }else if(this->m_RequestedFeature == 7){
      this->GenerateShortRunHighGreyLevelEmphasisData();
  }else if(this->m_RequestedFeature == 8){
      this->GenerateLongRunLowGreyLevelEmphasisData();
  }else if(this->m_RequestedFeature == 9){
      this->GenerateLongRunHighGreyLevelEmphasisData();
  }
  

}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateShortRunEmphasisData( void )
{
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType shortRunEmphasis = NumericTraits<MeasurementType>::Zero;
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
  {
      
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
           inputHistogram->GetIndex( hit.GetInstanceIdentifier() );
    
    double j2 = static_cast<double>( ( index[1] + 1 ) * ( index[1] + 1 ) );
    
    shortRunEmphasis += ( frequency / j2 );
  
  }
  
  shortRunEmphasis /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* shortRunEmphasisOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 0 ) );
  shortRunEmphasisOutputObject->Set( shortRunEmphasis );
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateLongRunEmphasisData( void )
{    
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType longRunEmphasis = NumericTraits<MeasurementType>::Zero;
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
  {
      
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
           inputHistogram->GetIndex( hit.GetInstanceIdentifier() );
    
    double j2 = static_cast<double>( ( index[1] + 1 ) * ( index[1] + 1 ) );
    
    longRunEmphasis += ( frequency * j2 );
  
  }
  
  longRunEmphasis /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* longRunEmphasisOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 1 ) );
  longRunEmphasisOutputObject->Set( longRunEmphasis );
  
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateGreyLevelNonuniformityData( void )
{
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType greyLevelNonuniformity = NumericTraits<MeasurementType>::Zero;
  vnl_vector<double> greyLevelNonuniformityVector(
    inputHistogram->GetSize()[0], 0.0 );
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
  {
      
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
           inputHistogram->GetIndex( hit.GetInstanceIdentifier() );
    
    greyLevelNonuniformityVector[index[0]] += frequency;
  }
  
  greyLevelNonuniformity
    = greyLevelNonuniformityVector.squared_magnitude();
  
  greyLevelNonuniformity /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* greyLevelNonuniformityOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 2 ) );
  greyLevelNonuniformityOutputObject->Set( greyLevelNonuniformity );
  
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateRunLengthNonuniformityData( void )
{
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType runLengthNonuniformity = NumericTraits<MeasurementType>::Zero;
  vnl_vector<double> runLengthNonuniformityVector(
    inputHistogram->GetSize()[1], 0.0 );
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
  {
      
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
           inputHistogram->GetIndex( hit.GetInstanceIdentifier() );
    
    runLengthNonuniformityVector[index[1]] += frequency;
  }
  
  runLengthNonuniformity
    = runLengthNonuniformityVector.squared_magnitude();
  
  runLengthNonuniformity /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* runLengthNonuniformityOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 3 ) );
  runLengthNonuniformityOutputObject->Set( runLengthNonuniformity );
    
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateLowGreyLevelRunEmphasisData(void)
{
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType lowGreyLevelRunEmphasis = NumericTraits<MeasurementType>::Zero;
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
  {
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
      inputHistogram->GetIndex( hit.GetInstanceIdentifier() );

    double i2 = static_cast<double>( ( index[0] + 1 ) * ( index[0] + 1 ) );
    
    lowGreyLevelRunEmphasis += ( frequency / i2 );
  }
  
  lowGreyLevelRunEmphasis /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* lowGreyLevelRunEmphasisOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 4 ) );
  lowGreyLevelRunEmphasisOutputObject->Set( lowGreyLevelRunEmphasis );
  
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateHighGreyLevelRunEmphasisData(void)
{
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType highGreyLevelRunEmphasis = NumericTraits<MeasurementType>::Zero;
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
    {
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
      inputHistogram->GetIndex( hit.GetInstanceIdentifier() );

    double i2 = static_cast<double>( ( index[0] + 1 ) * ( index[0] + 1 ) );
    
    highGreyLevelRunEmphasis += ( frequency * i2 );
  }
  
  highGreyLevelRunEmphasis /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* highGreyLevelRunEmphasisOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 5 ) );
  highGreyLevelRunEmphasisOutputObject->Set( highGreyLevelRunEmphasis );
  
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateShortRunLowGreyLevelEmphasisData(void)
{
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType shortRunLowGreyLevelEmphasis = NumericTraits<MeasurementType>::Zero;
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
  {
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
      inputHistogram->GetIndex( hit.GetInstanceIdentifier() );

    double i2 = static_cast<double>( ( index[0] + 1 ) * ( index[0] + 1 ) );
    double j2 = static_cast<double>( ( index[1] + 1 ) * ( index[1] + 1 ) );
    
    shortRunLowGreyLevelEmphasis += ( frequency / ( i2 * j2 ) );
  }
  
  shortRunLowGreyLevelEmphasis /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* shortRunLowGreyLevelEmphasisOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 6 ) );
  shortRunLowGreyLevelEmphasisOutputObject->Set( shortRunLowGreyLevelEmphasis );
  
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateShortRunHighGreyLevelEmphasisData(void)
{
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType shortRunHighGreyLevelEmphasis = NumericTraits<MeasurementType>::Zero;
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
  {
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
      inputHistogram->GetIndex( hit.GetInstanceIdentifier() );

    double i2 = static_cast<double>( ( index[0] + 1 ) * ( index[0] + 1 ) );
    double j2 = static_cast<double>( ( index[1] + 1 ) * ( index[1] + 1 ) );
    
    shortRunHighGreyLevelEmphasis += ( frequency * i2 / j2 );
  }
  
  shortRunHighGreyLevelEmphasis /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* shortRunHighGreyLevelEmphasisOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 7 ) );
  shortRunHighGreyLevelEmphasisOutputObject->Set( shortRunHighGreyLevelEmphasis );
  
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateLongRunLowGreyLevelEmphasisData(void)
{
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType longRunLowGreyLevelEmphasis = NumericTraits<MeasurementType>::Zero;
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
  {
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
      inputHistogram->GetIndex( hit.GetInstanceIdentifier() );

    double i2 = static_cast<double>( ( index[0] + 1 ) * ( index[0] + 1 ) );
    double j2 = static_cast<double>( ( index[1] + 1 ) * ( index[1] + 1 ) );
    
    longRunLowGreyLevelEmphasis += ( frequency * j2 / i2 );
  }
  
  longRunLowGreyLevelEmphasis /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* longRunLowGreyLevelEmphasisOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 8 ) );
  longRunLowGreyLevelEmphasisOutputObject->Set( longRunLowGreyLevelEmphasis );
  
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
GenerateLongRunHighGreyLevelEmphasisData(void)
{
  const HistogramType * inputHistogram = this->GetInput();

  this->m_TotalNumberOfRuns = static_cast<unsigned long>
     ( inputHistogram->GetTotalFrequency() );
  
  MeasurementType longRunHighGreyLevelEmphasis = NumericTraits<MeasurementType>::Zero;
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  for ( HistogramIterator hit = inputHistogram->Begin();
          hit != inputHistogram->End(); ++hit )
  {
    MeasurementType frequency = hit.GetFrequency();
    if ( frequency == 0 )
      {
      continue;
      }
    MeasurementVectorType measurement = hit.GetMeasurementVector();
    IndexType index =
      inputHistogram->GetIndex( hit.GetInstanceIdentifier() );

    double i2 = static_cast<double>( ( index[0] + 1 ) * ( index[0] + 1 ) );
    double j2 = static_cast<double>( ( index[1] + 1 ) * ( index[1] + 1 ) );
    
    longRunHighGreyLevelEmphasis += ( frequency * i2 * j2 );
  }
  
  longRunHighGreyLevelEmphasis /=
    static_cast<double>( this->m_TotalNumberOfRuns );
  
  MeasurementObjectType* longRunHighGreyLevelEmphasisOutputObject =
    static_cast<MeasurementObjectType*>( this->ProcessObject::GetOutput( 9 ) );
  longRunHighGreyLevelEmphasisOutputObject->Set( longRunHighGreyLevelEmphasis );
  
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetShortRunEmphasisOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 0 ) );
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetLongRunEmphasisOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 1 ) );
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetGreyLevelNonuniformityOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 2 ) );
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetRunLengthNonuniformityOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 3 ) );
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetLowGreyLevelRunEmphasisOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 4 ) );
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetHighGreyLevelRunEmphasisOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 5 ) );
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetShortRunLowGreyLevelEmphasisOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 6 ) );
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetShortRunHighGreyLevelEmphasisOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 7 ) );
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetLongRunLowGreyLevelEmphasisOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 8 ) );
}

template<class THistogram>
const
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementObjectType*
HistogramToRunLengthFeaturesFilter<THistogram>
::GetLongRunHighGreyLevelEmphasisOutput() const
{
  return static_cast<const MeasurementObjectType*>(
    this->ProcessObject::GetOutput( 9 ) );
}

template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetShortRunEmphasis() const
{
  return this->GetShortRunEmphasisOutput()->Get();
}

template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetLongRunEmphasis() const
{
  return this->GetLongRunEmphasisOutput()->Get();
}

template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetGreyLevelNonuniformity() const
{
  return this->GetGreyLevelNonuniformityOutput()->Get();
}

template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetRunLengthNonuniformity() const
{
  return this->GetRunLengthNonuniformityOutput()->Get();
}

template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetLowGreyLevelRunEmphasis() const
{
  return this->GetLowGreyLevelRunEmphasisOutput()->Get();
}
template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetHighGreyLevelRunEmphasis() const
{
  return this->GetHighGreyLevelRunEmphasisOutput()->Get();
}
template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetShortRunLowGreyLevelEmphasis() const
{
  return this->GetShortRunLowGreyLevelEmphasisOutput()->Get();
}
template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetShortRunHighGreyLevelEmphasis() const
{
  return this->GetShortRunHighGreyLevelEmphasisOutput()->Get();
}
template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetLongRunLowGreyLevelEmphasis() const
{
  return this->GetLongRunLowGreyLevelEmphasisOutput()->Get();
}
template<class THistogram>
typename HistogramToRunLengthFeaturesFilter<THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetLongRunHighGreyLevelEmphasis() const
{
  return this->GetLongRunHighGreyLevelEmphasisOutput()->Get();
}

template<class THistogram>
typename HistogramToRunLengthFeaturesFilter< THistogram>::MeasurementType
HistogramToRunLengthFeaturesFilter<THistogram>
::GetFeature( RunLengthFeatureName feature )
{
  switch( feature )
    {
    case ShortRunEmphasis:
      return this->GetShortRunEmphasis();
    case LongRunEmphasis:
      return this->GetLongRunEmphasis();
    case GreyLevelNonuniformity:
      return this->GetGreyLevelNonuniformity();
    case RunLengthNonuniformity:
      return this->GetRunLengthNonuniformity();
    case LowGreyLevelRunEmphasis:
      return this->GetLowGreyLevelRunEmphasis();
    case HighGreyLevelRunEmphasis:
      return this->GetHighGreyLevelRunEmphasis();
    case ShortRunLowGreyLevelEmphasis:
      return this->GetShortRunLowGreyLevelEmphasis();
    case ShortRunHighGreyLevelEmphasis:
      return this->GetShortRunHighGreyLevelEmphasis();
    case LongRunLowGreyLevelEmphasis:
      return this->GetLongRunLowGreyLevelEmphasis();
    case LongRunHighGreyLevelEmphasis:
      return this->GetLongRunHighGreyLevelEmphasis();
    default:
      return 0;
    }
}

template<class THistogram>
void
HistogramToRunLengthFeaturesFilter<THistogram>
::SetRequestedFeature(int feature)
{
    this->m_RequestedFeature = feature;
}

template< class THistogram>
void
HistogramToRunLengthFeaturesFilter< THistogram>::
PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os,indent );
}

} // end of namespace Statistics
} // end of namespace itk


#endif
