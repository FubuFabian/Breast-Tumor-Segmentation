#ifndef VTKTRACERINTERACTORSTYLE_H
#define VTKTRACERINTERACTORSTYLE_H

#include <vtkImageTracerWidget.h>
#include <vtkInteractorStyleImage.h>
#include <vtkSmartPointer.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>


#include <QWidget>

//class CheckCalibrationErrorWidget;


template<class Widget> 
class vtkTracerInteractorStyle : public vtkInteractorStyleImage
{

public:
    
    typedef Widget WidgetType;

    static vtkTracerInteractorStyle* New(){
        return new vtkTracerInteractorStyle;
    }
    
    vtkTypeMacro(vtkTracerInteractorStyle, vtkInteractorStyleImage);
    
    vtkTracerInteractorStyle();

    void initTracer(vtkSmartPointer<vtkImageActor> imageActor);
    
    void setAutoCloseOn();
    
    void setAutoCloseOff();
    
    void clearTracer();

    void setCallerWidget(WidgetType * callerWidget);

private:

    
    void catchEvent(vtkObject* caller, long unsigned int eventId, void* callData);

    vtkSmartPointer<vtkImageTracerWidget> tracer;

//	CheckCalibrationErrorWidget* calibrationErrorWidget;

    WidgetType * caller;
    
    vtkImageData * image;
};

#ifndef VTK_MANUAL_INSTANTIATION
#include "vtkTracerInteractorStyle.cpp"
#endif

#endif // VTKTRACERINTERACTORSTYLE_H
