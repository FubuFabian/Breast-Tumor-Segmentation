#ifndef vtkScribbleInteractorStyle_H
#define vtkScribbleInteractorStyle_H

// VTK
#include <vtkImageTracerWidget.h>
#include <vtkInteractorStyleImage.h> // superclass
#include <vtkSmartPointer.h>

// Boost
//#include <boost/signals2/signal.hpp>

class vtkImageActor;

class vtkScribbleInteractorStyle : public vtkInteractorStyleImage
{
public:
  // Constructors
  static vtkScribbleInteractorStyle* New();
  vtkTypeMacro(vtkScribbleInteractorStyle, vtkInteractorStyleImage);
  vtkScribbleInteractorStyle();

  // A signal to indicate that something has changed.
  //boost::signals2::signal<void (vtkPolyData*, bool)> StrokeUpdated;

  // Connect the tracer to the interactor, etc.
  void InitializeTracer(vtkImageActor* imageActor);

private:

  void ClearTracer();

  // Update the selection when the EndInteraction event is fired.
  void CatchWidgetEvent(vtkObject* caller, long unsigned int eventId, void* callData);

  // The widget which does most of the work.
  vtkSmartPointer<vtkImageTracerWidget> Tracer;

};

#endif
