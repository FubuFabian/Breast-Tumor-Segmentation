#ifndef ESTIMATESPHEREFROMPOINTS_H
#define ESTIMATESPHEREFROMPOINTS_H

#endif // ESTIMATESPHEREFROMPOINTS_H

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

//!ObtEstimate a sphere center and radius
/*!
  This class estimate a sphere using the general ecuation of the sphere (SphereFunction.h)
  I creates a non linear equation system using several points on the surface of the sphere
  and solve it using vnl_levenberg_marquardt.h.
*/
class EstimateSphereFromPoints
{

public:

	/** \brief Constructor */
    static EstimateSphereFromPoints *New()
    {
        return new EstimateSphereFromPoints;
    }


	/** \brief Set the manualy selected surface points */
    void setPoints(vnl_matrix<double>);

	/** \brief Return the estimated Sphere */
    vnl_vector<double> getSphere();

	/** \brief Estimate Sphere using levenberg-mardquart */
    void estimateSphere();

private:

	/** \brief The manualy selected surface Points */
    vnl_matrix<double> points;

	/** \brief The estimated sphere */
    vnl_vector<double> sphere;

};
