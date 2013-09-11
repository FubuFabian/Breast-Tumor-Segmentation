#ifndef SPHEREFUNCTION_H
#define SPHEREFUNCTION_H

#endif // SPHEREFUNCTION_H

#include <vnl/vnl_least_squares_function.h>
#include <vnl/vnl_cost_function.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>
#include <vector>

//!Ogeneral equation of the sphere
/*!
  This class is a vnl_least_squares_function that represents the general equation of a sphere.
  It is used as the function to estimate the center and radius of a sphere
*/
class SphereFunction : public vnl_least_squares_function
{

public:

    SphereFunction
       (vnl_matrix<double> * points);

    virtual ~SphereFunction();

	/** \brief The general equation of a sphere */
    virtual void f(vnl_vector<double> const &p, vnl_vector<double> &fx);

private:

	/** \brief Surface Points */
    vnl_matrix<double> * _data;

};

