/* 
 * File:   CalibrationCalc
 * Author: Fabian Torres
 * 
 * Created on 19/january/2012, 0:38
 */

#include "Calibration.h"

void Calibration::InsertTransformations(vnl_matrix<double> rotationMatrix, vnl_vector<double> translation)
{
	std::cout<<std::endl;
	std::cout<<"Rotation Matrix:"<<std::endl;
	rotationMatrix.print(std::cout);
	std::cout<<std::endl;
	std::cout<<"Traslation: "<<translation[0]<<", "<<translation[1]<<", "<<translation[2]<<std::endl;
	std::cout<<std::endl;
	
	lsqrRecipes::Frame f;

	f.setRotationMatrix(rotationMatrix);
	f.setTranslation(translation);
	transformations.push_back(f);

	if(transformations.empty())
		std::cout<<"Transformations is empty"<<std::endl<<std::endl;
  

}

void Calibration::ClearTransformations()
{
	transformations.clear();
	if(!transformations.empty())
		std::cout<<"Transformations is not clear"<<std::endl<<std::endl;
		
}

void Calibration::InsertImagePoints(double p[2])
{

	lsqrRecipes::Point2D point;
	
	point[0]=p[0];
	point[1]=p[1];
	std::cout<<"Image point: "<<p[0]<<", "<<p[1]<<std::endl<<std::endl;
	imagePoints.push_back(p);
	
	if(imagePoints.empty())
		std::cout<<"Imagepoints is empty"<<std::endl<<std::endl;
}

void Calibration::ClearImagePoints()
{
	imagePoints.clear();
	if(!imagePoints.empty())
		std::cout<<"Imagepoints is not clear"<<std::endl<<std::endl;
}

bool Calibration::Calibrate()
{
	DataType dataElement;

	std::cout<<"Calculating calibration parameters"<<std::endl<<std::endl;
	size_t n = transformations.size();

	for(int i=0; i<n; i++) {
		dataElement.T2 = transformations[i];
		dataElement.q = imagePoints[i];
		data.push_back(dataElement);
	}

	double maxDistanceBetweenPoints = 1.0;
	lsqrRecipes::SingleUnknownPointTargetUSCalibrationParametersEstimator 
		usCalibration(maxDistanceBetweenPoints);

    //std::vector<double> estimatedUSCalibrationParameters;
  
	usCalibration.setLeastSquaresType(lsqrRecipes::SingleUnknownPointTargetUSCalibrationParametersEstimator::ANALYTIC);
	usCalibration.leastSquaresEstimate(data, estimatedUSCalibrationParameters);

	if(estimatedUSCalibrationParameters.size() == 0)
		std::cout<<"DEGENERATE CONFIGURATION\n\n\n";
	else {
		std::cout<<"t1[x,y,z]:\n";
		std::cout<<"\t["<<estimatedUSCalibrationParameters[0]<<", "<<estimatedUSCalibrationParameters[1];
		std::cout<<", "<<estimatedUSCalibrationParameters[2]<<"]\n";
		std::cout<<"t3[x,y,z]:\n";
		std::cout<<"\t["<<estimatedUSCalibrationParameters[3]<<", "<<estimatedUSCalibrationParameters[4];
		std::cout<<", "<<estimatedUSCalibrationParameters[5]<<"]\n";
		std::cout<<"omega[z,y,x]:\n";
		std::cout<<"\t["<<estimatedUSCalibrationParameters[6]<<", "<<estimatedUSCalibrationParameters[7];
		std::cout<<", "<<estimatedUSCalibrationParameters[8]<<"]\n";
		std::cout<<"m[x,y]:\n";
		std::cout<<"\t["<<estimatedUSCalibrationParameters[9];
		std::cout<<", "<<estimatedUSCalibrationParameters[10]<<"]\n";
	}	
	
	return 1;
}

std::vector<double> Calibration::getEstimatedUSCalibrationParameters()
{
    return estimatedUSCalibrationParameters;
}
