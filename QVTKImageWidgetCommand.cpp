 //
//  OnMouseMovePtr.cpp
//  US_Probe_Calibration
//
//  Created by Zian Fanti on 07/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//
#ifndef QVTKIMAGEWIDGETCOMMAND_CPP
#define	QVTKIMAGEWIDGETCOMMAND_CPP

#include "QVTKImageWidgetCommand.h"
//#include "QVTKImageWidget.h"
//#include "QVTKVolumeSliceWidget.h"

#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkVariant.h>
#include <vtkMath.h>
#include <vtkCommand.h>
#include <vtkImageViewer2.h>

#include<sstream>


template<class Widget>
QVTKImageWidgetCommand<Widget>::QVTKImageWidgetCommand() 
{ 
    this->qVTKImageWidgetFlag = true;
    this->qVTKVolumeSliceWidgetFlag = false; 
}

template<class Widget>
QVTKImageWidgetCommand<Widget>::~QVTKImageWidgetCommand()
{
  this->ImageWidget = NULL;
  this->Picker = NULL;
  this->Annotation = NULL;
}

template<class Widget>
void QVTKImageWidgetCommand<Widget>::SetPicker(vtkSmartPointer<vtkPropPicker> picker)
{
  this->Picker = picker;
}

template<class Widget>
void QVTKImageWidgetCommand<Widget>::SetAnnotation(vtkSmartPointer<vtkCornerAnnotation> annotation)
{
  this->Annotation = annotation;
}

template<class Widget>
void QVTKImageWidgetCommand<Widget>::SetImageWidget(WidgetType* imageWidget)
{
  this->ImageWidget = imageWidget;
}

template<class Widget>
void QVTKImageWidgetCommand<Widget>::Execute(vtkObject* caller, unsigned long event, void *vtkNotUsed(callData))
{
  vtkSmartPointer<vtkImageViewer2> Viewer = ImageWidget->getImageViewer();

  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
          Viewer->GetRenderWindow()->GetInteractor();
  
  // Get a shortcut to the pixel data.
  vtkSmartPointer<vtkImageData> imageData = Viewer->GetInput();
      
  // if the mouse is moving inside the viewer
  if (event == vtkCommand::MouseMoveEvent)
    {
                          
      int* windowPosition = interactor->GetEventPosition();

      int xWindow = windowPosition[0];
      int yWindow = windowPosition[1];
      int zWindow = windowPosition[2];

      this->Picker->Pick(xWindow, yWindow, zWindow, Viewer->GetRenderer());

      double* imPos = this->Picker->GetPickPosition();
      int xImagePosition = vtkMath::Round(imPos[0]);
      int yImagePosition = vtkMath::Round(imPos[1]);
      int zImagePosition = Viewer->GetSlice();
      
      
      int* extent = imageData->GetExtent();
      int* dimension = imageData->GetDimensions();   
      
      int center[2];
      center[0] = floor(dimension[0]/2);
      center[1] = floor(dimension[1]/2);
      
      if(qVTKVolumeSliceWidgetFlag){
          xImagePosition = (xImagePosition - extent[0] + center[0]);
          yImagePosition = (yImagePosition - extent[2] + center[1]);
      }
      
      
      int xClipPosition = (xImagePosition - extent[0]);
      int yClipPosition = (dimension[1]-1) - (yImagePosition - extent[2]);
	
      //  int yClipPosition = (yImagePosition );
      int zClipPosition = zImagePosition - extent[4];
      
      
      typename std::stringstream xx;
          xx << xImagePosition;
          typename  std::stringstream yy;
          yy << yImagePosition;
          typename  std::stringstream zz;
          zz << zImagePosition;
      
      std::string message = "Location: ( "
              + vtkVariant(xClipPosition).ToString() + ", "
              + vtkVariant(yClipPosition).ToString() + ", "
              + vtkVariant(zClipPosition).ToString() + ")";
      
      if(qVTKVolumeSliceWidgetFlag)
          this->ImageWidget->setPickedCoordinates(xImagePosition,yImagePosition);
      else if(qVTKImageWidgetFlag)
          this->ImageWidget->setPickedCoordinates(xClipPosition,yClipPosition);
      
      // for display pixel value when mouse move
      // We have to handle different number of scalar components.
      switch (imageData->GetNumberOfScalarComponents())
        {
        case 1:
          {
            float pixelData = imageData->GetScalarComponentAsFloat(xImagePosition,
                                                                   yImagePosition,
                                                                   zImagePosition, 0);
          typename  std::stringstream p;
          p << pixelData;
          message += " - Pixel Value: (" + p.str() + ")";                       
            
            break;
          }
        
        case 3:
          {
            float pixel_r = imageData->GetScalarComponentAsFloat(xImagePosition,
                                                                 yImagePosition,
                                                                 zImagePosition, 0);
            float pixel_g = imageData->GetScalarComponentAsFloat(xImagePosition,
                                                                 yImagePosition,
                                                                 zImagePosition, 1);
            float pixel_b = imageData->GetScalarComponentAsFloat(xImagePosition,
                                                                 yImagePosition,
                                                                 zImagePosition, 2);
          typename std::stringstream r;
          r << pixel_r;
          typename  std::stringstream g;
          g << pixel_g;
          typename  std::stringstream b;
          b << pixel_b;
            
            message += " - Pixel Value: (" + r.str() + "," + g.str() + "," + b.str() + ")";    
            
            break;
          }          
        }

      
      this->Annotation->SetText(3, message.c_str());
      interactor->Render();

      vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
      style->OnMouseMove();
    }

}

template<class Widget>
void QVTKImageWidgetCommand<Widget>::setQVTKImageWidgetFlagOn()
{
    this->qVTKImageWidgetFlag = true;
    this->qVTKVolumeSliceWidgetFlag = false; 
}
  
template<class Widget>
void QVTKImageWidgetCommand<Widget>::setQVTKVolumeSliceWidgetFlagOn()
{
    this->qVTKImageWidgetFlag = false;
    this->qVTKVolumeSliceWidgetFlag = true; 
}



#endif
