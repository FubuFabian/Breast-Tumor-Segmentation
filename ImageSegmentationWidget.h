#ifndef IMAGESEGMENTATIONWIDGET_H
#define IMAGESEGMENTATIONWIDGET_H

#include "mainwindow.h"
#include "QVTKImageWidget.h"
#include "ImageSegmentation.h"

#include <QWidget>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <itkImage.h>

namespace Ui {
    class ImageSegmentationWidget;
}

class ImageSegmentationWidget : public QWidget
{
    Q_OBJECT
    
public:

	typedef itk::Image<unsigned char, 2> ImageType;

    explicit ImageSegmentationWidget(QWidget *parent = 0);
    ~ImageSegmentationWidget();

    void setImage(vtkSmartPointer<vtkImageData> image);
    
    void setMainWindow(MainWindow * mainWindow);
	    
    /**
    * \brief get the cross point coordinates
    */
    void getCoordinates();
    
private:
    
    Ui::ImageSegmentationWidget *ui;

	ImageSegmentation * imageSegmentation;
    
    MainWindow * mainWindow;
    
    vtkSmartPointer<vtkImageData> vtkProbabilityImage;
    
    vtkSmartPointer<vtkImageData> vtkRegionGrowingImage;
    
    vtkSmartPointer<vtkImageData> vtkContourImage;
    
    std::vector<ImageType::IndexType> contourPixels;
    
    QVTKImageWidget *displayWidget;
    
    std::vector<float> intensityProbs;
    
    std::vector<float> textureProbs;
    
    bool intensityProbsFlag;
    
    bool textureProbsFlag;
    
    bool probabilityImageFlag;
    
    bool seedPointFlag;
    
	ImageType::IndexType seedPoint;

private slots:

    void loadIntensityProbs();

    void loadTextureProbs();

    void computeProbability();

    void segment();

    void save();
    
    void newSeed();
};

#endif // IMAGESEGMENTATIONWIDGET_H
