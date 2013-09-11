#ifndef SEGMENTATIONTRAININGWIDGET_H
#define SEGMENTATIONTRAININGWIDGET_H

#include <QWidget>

#include "mainwindow.h"
#include "ui_SegmentationTrainingWidget.h"

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageStencilData.h>

#include <itkImage.h>
#include <vnl/vnl_vector.h>


/*!
  This classs Computes the intensity and texture probabilities for breast tumors
 * in ultrasound images. 
*/
class SegmentationTrainingWidget : public QWidget, private Ui::SegmentationTrainingWidget
{
    Q_OBJECT

public:
    
    typedef itk::Image<unsigned char, 2> ImageType;    
    typedef itk::Image<float, 2> FloatImageType;
    
    /**
     * Constructor
     * @param parent
     */
    explicit SegmentationTrainingWidget(QWidget *parent = 0);
    ~SegmentationTrainingWidget();
    
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
    
    /** Set the window to display the crop images */
    void setMainWindow(MainWindow* mainwindow);

    /** Set traced path by the tracer*/
    void setPath(vtkSmartPointer<vtkPolyData> data);

private:
    
    /** \brief User interface*/
    Ui::SegmentationTrainingWidget *ui;
    
    /** \brief a flag to work with multiple images*/
    bool workWithStack;

    /** \brief an Array of vtkImageData to work */
    std::vector< vtkSmartPointer<vtkImageData> > imageStack;
        
    /** \brief a vnl_matrix to store the center pf the sphere */
    vnl_matrix<double> centers;
    
    /** the main window to call it */
    MainWindow* mainWindow;
    
    /** \brief the vtkImageData to work */
    vtkSmartPointer<vtkImageData> image;
    
    /** \brief a Vector that has the traced points*/
    std::vector< vtkSmartPointer<vtkPolyData> > dataVector;
    
    /** \brief Manual segmentation for each tumor*/
    vtkSmartPointer<vtkPolyData> data;
    
    /** \brief number of voxels in each segmentation*/
    int numberOfVoxels;
    
    /** \brief Mean probabilities for intensity*/
    vnl_vector<double> intensityMeanProbabilities;
    
    /** \brief Mean probabilities for teture*/
    vnl_vector<double> textureMeanProbabilities;
    
    virtual void closeEvent( QCloseEvent * event);
    
    /** \brief Convert vtk image ti itk image*/
    FloatImageType::Pointer convertToITKImage(vtkSmartPointer<vtkImageData> vtkImage);

    /**
     * Get the pixels inside the manual segmented area
     * @param vtkImage
     * @param nImage number of image in the stack
     * @return 
     */
    vtkSmartPointer<vtkImageStencilData> getStencilImage(
                                vtkSmartPointer<vtkImageData> vtkImage, int nImage);
    
    /**
     * Computes the histogram of the pixels inside the manual segmented area
     * @param stencil
     * @param image
     * @return 
     */
    vtkSmartPointer<vtkImageData> getHistogram(
                                    vtkSmartPointer<vtkImageStencilData> stencil,
                                            vtkSmartPointer<vtkImageData> image);

private slots:

    /**
     * Computes the intensity and texture probabilities
     */
    void computeProbabilities();

    /**
     * Save the probabilities in .txt files
     */
    void saveProbabilities();
};

#endif // SEGMENTATIONTRAININGWIDGET_H
