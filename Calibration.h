 /*
 * File:   CalibrationCalc
 * Author: Fabian Torres
 * 
 * Created on 19/january/2012, 0:38
 */

#include "SinglePointTargetUSCalibrationParametersEstimator.h"
#include "RANSAC.h"


//!Implements LSQRRecepies methods
/*!
  This classs have the calibration methods implemented in LSQRRecipes to calibrate an
  Ultra Sound Probe with a cross wire phantom.
*/
class Calibration
{


	public:

        typedef lsqrRecipes::SingleUnknownPointTargetUSCalibrationParametersEstimator::DataType DataType;

        ///Constructor of the class
		static Calibration *New()
		{
			return new Calibration;
		} 
	
        ///insert the rotation matrix of an image to transformations
		void InsertTransformations(vnl_matrix<double> rotationMatrix, vnl_vector<double> translation);
	
        ///clear transformations
		void ClearTransformations();
	
        ///insert the crosswire point of an image to imagePoints
		void InsertImagePoints(double p[2]);
	
        ///clear imagePoints
		void ClearImagePoints();
		
        ///estimate calibration parameters
		bool Calibrate();

        std::vector<double> getEstimatedUSCalibrationParameters();

    private:

        std::vector<lsqrRecipes::Frame> transformations;

        std::vector<lsqrRecipes::Point2D> imagePoints;///<contains the crosswire point in all images

        std::vector< DataType > data;///<contain the data of all images

        std::vector<double> estimatedUSCalibrationParameters;//contain the estimated calibration parameters


};
