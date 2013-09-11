#include "EstimateSphereFromPoints.h"
#include "SphereFunction.h"

#include <vnl/algo/vnl_levenberg_marquardt.h>
#include <iostream>
#include <math.h>
#include <QErrorMessage>


void EstimateSphereFromPoints::setPoints(vnl_matrix<double> points)
{
    this->points = points;
}

vnl_vector<double> EstimateSphereFromPoints::getSphere()
{
    return sphere;
}

void EstimateSphereFromPoints::estimateSphere()
{

	double nPoints = points.rows();

	std::cout<<std::endl;
	std::cout<<"Number of surface points "<<nPoints<<std::endl;		

	double pCenterX = points.get_column(0).mean();
	double pCenterY = points.get_column(1).mean();
	double pCenterZ = points.get_column(2).mean();

	std::cout<<std::endl;
	std::cout<<"Initial values for center "<<pCenterX<<", "<<pCenterY<<", "<<pCenterZ<<std::endl;

	vnl_vector<double> dist;
	dist.set_size(points.rows());

	for(int i=0; i<points.rows(); i++)
	{
		vnl_vector<double> sPoint = points.get_row(i);
		dist[i] = sqrt(pow((pCenterX - sPoint[0]),2) + pow((pCenterY - sPoint[1]),2) + pow((pCenterZ - sPoint[2]),2));
	}

	double pRadius = dist.mean();

	std::cout<<std::endl;
	std::cout<<"Initial values for radius "<<pRadius<<std::endl<<std::endl;

	vnl_vector<double> x(4);
	x.put(0,pCenterX);
	x.put(1,pCenterY);
	x.put(2,pCenterZ);
	x.put(3,pRadius);

	SphereFunction sphereFunc(&points);

	vnl_levenberg_marquardt LM(sphereFunc);
    LM.set_verbose(false);
    
    LM.set_f_tolerance(1e-6);
    LM.set_x_tolerance(1e-6);

    LM.set_max_function_evals(100*nPoints);
    
    bool okOptimization = false;
    
    try
    {
         okOptimization = LM.minimize(x);
    }
    catch (std::exception& e)
    {
		std::cout<<"Excepcion"<<std::endl;
    }

	LM.diagnose_outcome(std::cout);
	std::cout<<std::endl<<"Sphere = " << x << std::endl;

	sphere.set_size(4);
	sphere = x;
}

