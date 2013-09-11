#ifndef IMAGESEGMENTATIONWIDGET_H
#define IMAGESEGMENTATIONWIDGET_H

#include "mainwindow.h"
#include "QVTKImageWidget.h"

#include <QWidget>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <itkImage.h>
#include <itkCastImageFilter.h>

namespace Ui {
    class ImageSegmentationWidget;
}

class ImageSegmentationWidget : public QWidget
{
    Q_OBJECT

    typedef itk::Image<unsigned char, 2> ImageType;    
    typedef itk::Image<float, 2> FloatImageType;
    typedef itk::CastImageFilter<FloatImageType, ImageType> CastFilterType;
    
public:
    explicit ImageSegmentationWidget(QWidget *parent = 0);
    ~ImageSegmentationWidget();

    void setImage(vtkSmartPointer<vtkImageData> image);
    
    void setMainWindow(MainWindow * mainWindow);
    
private:
    
    Ui::ImageSegmentationWidget *ui;
    
    MainWindow * mainWindow;
    
    vtkSmartPointer<vtkImageData> vtkImage;
    
    FloatImageType::Pointer itkImage;
    
    vtkSmartPointer<vtkImageData> vtkProbabilityImage;
    
    vtkSmartPointer<vtkImageData> vtkRegionGrowingImage;
    
    vtkSmartPointer<vtkImageData> vtkContourImage;
    
    std::vector<ImageType::IndexType> contourPixels;
    
    QVTKImageWidget *displayWidget;
    
    FloatImageType::Pointer probabilityImage;
    
    std::vector<float> intensityProbs;
    
    std::vector<float> textureProbs;
    
    bool intensityProbsFlag;
    
    bool textureProbsFlag;
    
    bool probabilityImageFlag;
    
    bool seedPointFlag;
    
    ImageType::IndexType seedPoint;
    
    FloatImageType::Pointer convertToITKImage(vtkSmartPointer<vtkImageData> vtkImage);
    
    vtkSmartPointer<vtkImageData> convertToVTKImage(FloatImageType::Pointer itkImage);
    

private slots:

    void loadIntensityProbs();

    void loadTextureProbs();

    void computeProbability();

    void segment();

    void save();
    
    /**
    * \brief get the cross point coordinates
    */
    void getCoordinates();
    
    void newSeed();
};

#endif // IMAGESEGMENTATIONWIDGET_H
