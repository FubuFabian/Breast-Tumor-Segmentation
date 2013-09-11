#ifndef CHECKCALIBRATIONERRORWIDGET_H
#define CHECKCALIBRATIONERRORWIDGET_H

#include "ui_CheckCalibrationErrorWidget.h"
#include "mainwindow.h"

#include <QWidget>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>

//!Check the calibration error
/*!
  This class Check the calibration Error by estimating the center and the radius of a tracked sphere
  using EstimateSpehereFromPoints.h and meassures the distance between the tracked center and the estimated
  center as the calibration error. It allows the user to select points on the surface of the sphere from
  each image manualy. The error can be saved in a .txt file.
*/
class CheckCalibrationErrorWidget : public QWidget, private Ui::CheckCalibrationErrorWidget
{
    Q_OBJECT
    
public:
       
    /** Constructor */
    CheckCalibrationErrorWidget(QWidget* parent = 0);
    
	
    /** Destructor */
    virtual ~CheckCalibrationErrorWidget();

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
    
    Ui::CheckCalibrationErrorWidget *ui;


    /** \brief a flag to work with multiple images*/
    bool workWithStack;

    /** \brief an Array of vtkImageData to work */
    std::vector< vtkSmartPointer<vtkImageData> > imageStack;
        
    /** \brief a vnl_matrix to store the center pf the sphere */
    vnl_matrix<double> centers;
    
    /** \brief a vnl_matrix to store the translations of each image given by the tracker */
    vnl_matrix<double> translations;
    
    
    /** \brief a vnl_matrix to store the rotations of each image given by the tracker */
    vnl_matrix<double> rotations;
    float rotations_2[7][4];

    /** The angles and translation estimated */
    std::vector<double> calibrationData;
    
    /** the main window to call it */
    MainWindow* mainWindow;
    
    /** \brief the vtkImageData to work */
    vtkSmartPointer<vtkImageData> image;

    /** \brief a Vector that has the traced points*/
    std::vector< vtkSmartPointer<vtkPoints> > pointsVector;

    /** \brief a Vector that contain the calibration Error */	
    vnl_vector<double> error;

    //vtkSmartPointer<vtkPoints> points;
    virtual void closeEvent( QCloseEvent * event);

    /** \brief Transform the vectorPoints to its world coordinates */
    vnl_matrix<double> transformPoints();

private slots:

    /** \brief load the tracked Center of the sphere*/
    void loadCenter();

    /** \brief load the Rotations for each image */
    void loadRotations();

    /** \brief load the Translations for each image */
    void loadTranslations();

    /** \brief load the Calibration to check */
    void loadCalibration();
    
    /** \brief Cehck the calibration error */
    void checkError();
    
    /** \brief Save the calibration error in a txt file */
    void saveError();
};

#endif // CHECKCALIBRATIONERRORWIDGET_H
