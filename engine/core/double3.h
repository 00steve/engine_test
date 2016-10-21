#ifndef DOUBLE3_H
#define DOUBLE3_H

/*the double3 class is a simple container for storing x,y,z double values
that are necessary for any 3d calculations. It includes basic functions
for performing math operations with double2 and double variables, as well
as some common functions like getting the length of the vector, or cross-
multiplication with other double3 variables.*/

#include <limits>
#include <math.h>

class double3 {
public:

	//store the x,y and z variables
    double x,y,z;

	//double3 constructor, initialize all variables to zero
    //double3();

	//constructor that assigns default values
    //double3(double x,double y,double z);

    	//double3 constructor, initialize all variables to zero
    double3() : x(0),y(0),z(0) {
    }

	//constructor that assigns default values
    double3(double x,double y,double z) :
    	x(x),
    	y(y),
    	z(z) {
    }


    //get the normal of the vector
    double3 normal();

	//convert the double3 to its normalized length and return the value
    double3 normalize();

    /*returns the cross product of two double3 vectors.*/
    double3 operator ^ (const double3 &f);

    /*returns the dot product of two double3 vectors*/
    double	operator * (const double3 &f);

    double3 operator += (const double3 &f);
    double3 operator -= (const double3 &f);
    double3 operator + (const double3 &f);
    double3 operator - (const double3 &f);

    double3 operator *= (const double &d);
    double3 operator * (const double &f);
    double3 operator + (const double &f);
    double3 operator - (const double &f);

    bool operator > (const double &f);
    bool operator < (const double &f);

    double length();

    static double3 infinity;
};



#endif

