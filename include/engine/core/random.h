#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>
#include "double3.h"

double3 randomPointInSphere(double radius){
	double3 res = double3(static_cast <double> (rand()) / static_cast <double> (RAND_MAX) -.5,
						static_cast <double> (rand()) / static_cast <double> (RAND_MAX)-.5,
						static_cast <double> (rand()) / static_cast <double> (RAND_MAX)-.5);
	double depth = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
	return res.normal()*(depth*radius);
}

double3 randomPointInSphere(double3 radius){
	double3 res = double3(static_cast <double> (rand()) / static_cast <double> (RAND_MAX) -.5,
						static_cast <double> (rand()) / static_cast <double> (RAND_MAX)-.5,
						static_cast <double> (rand()) / static_cast <double> (RAND_MAX)-.5).normal();
	double3 depth = double3((static_cast <double> (rand()) / static_cast <double> (RAND_MAX) -.5)*radius.x,
						(static_cast <double> (rand()) / static_cast <double> (RAND_MAX)-.5)*radius.y,
						(static_cast <double> (rand()) / static_cast <double> (RAND_MAX)-.5)*radius.z);
	return double3(res.x*depth.x,res.y*depth.y,res.z*depth.z);
}






#endif // RANDOM_H
