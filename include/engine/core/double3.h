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
    double3() : x(0),y(0),z(0) {
    }

	//constructor that assigns default values
    double3(double x,double y,double z) :
    	x(x),
    	y(y),
    	z(z) {
    }

    //get the normal of the vector
    double3 normal() {
        double length = sqrt(x*x+y*y+z*z);
        return double3(x / length , y / length, z / length );
    }

	double3 abs(){
		return double3(fabs(x),fabs(y),fabs(z));
	}

	//convert the double3 to its normalized length and return the value
    double3 normalize() {
        double length = sqrt(x*x+y*y+z*z);
        x /= length; y /= length; z /= length;
        return *this;
    }

    /*returns the cross product of two double3 vectors.*/
    double3 operator ^ (const double3 &f) 	{ return double3(y*f.z-z*f.y,z*f.x-x*f.z,x*f.y-y*f.x);}

    /*returns the dot product of two double3 vectors*/
    double	operator * (const double3 &f)	{ return x*f.x + y*f.y + z*f.z; }

    double3 operator += (const double3 &f) 	{ x += f.x; y += f.y; z += f.z; return *this; }
    double3 operator -= (const double3 &f) 	{ x -= f.x; y -= f.y; z -= f.z; return *this; }
    double3 operator + (const double3 &f) 	{ return double3(x+f.x,y+f.y,z+f.z); }
    double3 operator - (const double3 &f) 	{ return double3(x-f.x,y-f.y,z-f.z); }

    double3 operator *= (const double &d) 	{ x *= d; y *= d; z *= d; return *this; }
    double3 operator * (const double &f) 	{ return double3(x*f,y*f,z*f); }
    double3 operator + (const double &f) 	{ return double3(x+f,y+f,z+f); }
    double3 operator - (const double &f) 	{ return double3(x-f,y-f,z-f); }

    double3 operator * (const int &i) 	{ return double3(x*(double)i,y*(double)i,z*(double)i); }
    double3 operator / (const int &i) 	{ return double3(x/(double)i,y/(double)i,z/(double)i); }
    double3 operator + (const int &i) 	{ return double3(x+(double)i,y+(double)i,z+(double)i); }
    double3 operator - (const int &i) 	{ return double3(x-(double)i,y-(double)i,z-(double)i); }

    bool operator > (const double &f) {
        if(x*x+y*y+z*z > f*f) return true;
        return false;
    }
    bool operator < (const double &f) {
        if(x*x+y*y+z*z < f*f) return true;
        return false;
    }

    double length() { return sqrt(x*x+y*y+z*z); }

    static double3 infinity;
};

/*double3 initialized to the largest
double values that the system supports*/
double3 double3::infinity = double3(std::numeric_limits<double>::max(),std::numeric_limits<double>::max(),std::numeric_limits<double>::max());

/*given two double3 values, it returns the greater of each double value
as a new double3*/
inline double3 double3_greater(const double3 &a,const double3 &b) {
    return double3((a.x>b.x?a.x:b.x),(a.y>b.y?a.y:b.y),(a.z>b.z?a.z:b.z));
}

/*given two double3 values, it returns the lesser of each double value
as a new double3*/
inline double3 double3_lesser(const double3 &a,const double3 &b) {
    return double3((a.x<b.x?a.x:b.x),(a.y<b.y?a.y:b.y),(a.z<b.z?a.z:b.z));
}

/*define different types that double3 can be called*/

typedef double3 vertex;
typedef double3 rgb;
typedef double3 normal;

#endif

