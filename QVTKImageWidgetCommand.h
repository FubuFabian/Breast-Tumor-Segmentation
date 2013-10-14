//
//  OnMouseMovePtr.h
//  US_Probe_Calibration
//
//  Created by Zian Fanti on 07/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef US_Probe_Calibration_OnMouseMovePtr_h
#define US_Probe_Calibration_OnMouseMovePtr_h

#include <vtkCommand.h>
#include <vtkPropPicker.h>
#include <vtkCornerAnnotation.h>
#include <vtkSmartPointer.h>

//!Interaction with mouse
/*!
  The mouse motion callback, to pick the image and recover pixel values
*/
template<class Widget>
class QVTKImageWidgetCommand : public vtkCommand
{
public:
    
  typedef Widget WidgetType;

  static QVTKImageWidgetCommand *New()
  {
        return new QVTKImageWidgetCommand;
  }
  
  vtkTypeMacro(QVTKImageWidgetCommand, vtkCommand);

  QVTKImageWidgetCommand();

  ~QVTKImageWidgetCommand();

  void SetPicker(vtkSmartPointer<vtkPropPicker> picker);

  void SetAnnotation(vtkSmartPointer<vtkCornerAnnotation> annotation);

  /**
   * \brief Set the 2d image widget related to this 2d event manager
   * \param[in] viewer QVTKImageWidget target 2D image
   */
  void SetImageWidget(WidgetType* imageWidget);

  /**
   *
   */
  void Execute(vtkObject *, unsigned long vtkNotUsed(event), void *);
  
  /**
   * \brief Set the qVTKImageWidgetFlag to true
   **/
  void setQVTKImageWidgetFlagOn();
  
  /**
   * \brief Set the qVTKVolumeSliceWidgetFlag to true
   **/
  void setQVTKVolumeSliceWidgetFlagOn(); 

private:

  /** Pointer to the picker */
  vtkSmartPointer<vtkPropPicker> Picker;

  /** Pointer to the annotation */
  vtkSmartPointer<vtkCornerAnnotation> Annotation;

  /** The widget related to the mouse events */
  WidgetType* ImageWidget;
  
  /** Indicates that the calling widget is a QVTKImageWidget */
  bool qVTKImageWidgetFlag;
  
  /** Indicates that the calling widget is a QVTKVolumeSliceWidget */
  bool qVTKVolumeSliceWidgetFlag; 
  
  
};

#ifndef VTK_MANUAL_INSTANTIATION
#include "QVTKImageWidgetCommand.cpp"
#endif

#endif
