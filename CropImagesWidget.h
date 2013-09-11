#ifndef CROPIMAGESWIDGET_H
#define CROPIMAGESWIDGET_H

#include "ui_CropImagesWidget.h"
#include "mainwindow.h"

#include <QWidget>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

namespace Ui {
class CropImagesWidget;
}

//!Crop images
/*!
  This class crop one or multiple images deppendng on which range of the 
  ultrasound machine was used. It can crop images in ranges 4, 5, 6 and 8.
  Images can be saved in a folder or used to calibrate the US probe. 
*/
class CropImagesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CropImagesWidget(QWidget *parent = 0);
    ~CropImagesWidget();

     /**
     * \brief Set this stack of vtkImageData 
     * \param[in] a std Vector of vtkImageData
     */
    void setImageStack(std::vector< vtkSmartPointer<vtkImageData> > imageStack);
    
    /**
     * \brief Set this vtkImageData 
     * \param[in] a smart Pointer of vtkImageData
     */
    void setImage(vtkSmartPointer<vtkImageData> image);
    
    
    /** 
	 * \brief Set the window to display the crop images 
	 */
    void setMainWindow(MainWindow* mainwindow);

private:
    Ui::CropImagesWidget *ui;

	/** \brief if there are multiple images to work with */
	bool workWithStack;
    
    /** \brief an Array of vtkImageData to work */
    std::vector< vtkSmartPointer<vtkImageData> > imageStack;
	 
    /** the main window to call it */
    MainWindow* mainWindow;

	 /** \brief the vtkImageData to work */
    vtkSmartPointer<vtkImageData> image;

	/** \brief the cropped image */
	vtkSmartPointer<vtkImageData> cropImage;

	/** \brief the cropped imageStack */
    std::vector< vtkSmartPointer<vtkImageData> > cropStack;
    
    /** \brief Crop ultrasound image depnding of the depth type*/
    vtkSmartPointer<vtkImageData> cropProbeImage(vtkSmartPointer<vtkImageData> image, int depthType);

private slots:

	/** \brief calls the crop method when the crop buttom is clicked */
    void crop();

	/** \brief Save the cropped images in a folder*/
    void save();

};

#endif // CROPIMAGESWIDGET_H
