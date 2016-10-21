
#include "double3.h"




    //get the normal of the vector
double3 double3::normal() {
        double length = sqrt(x*x+y*y+z*z);
        return double3(x / length , y / length, z / length );
    }

	//convert the double3 to its normalized length and return the value
double3 double3::normalize() {
        double length = sqrt(x*x+y*y+z*z);
        x /= length; y /= length; z /= length;
        return *this;
    }

    /*returns the cross product of two double3 vectors.*/
    double3 double3::operator ^ (const double3 &f) 	{ return double3(y*f.z-z*f.y,z*f.x-x*f.z,x*f.y-y*f.x);}

    /*returns the dot product of two double3 vectors*/
    double	double3::operator * (const double3 &f)	{ return x*f.x + y*f.y + z*f.z; }

    double3 double3::operator += (const double3 &f) 	{ x += f.x; y += f.y; z += f.z; return *this; }
    double3 double3::operator -= (const double3 &f) 	{ x -= f.x; y -= f.y; z -= f.z; return *this; }
    double3 double3::operator + (const double3 &f) 	{ return double3(x+f.x,y+f.y,z+f.z); }
    double3 double3::operator - (const double3 &f) 	{ return double3(x-f.x,y-f.y,z-f.z); }

    double3 double3::operator *= (const double &d) 	{ x *= d; y *= d; z *= d; return *this; }
    double3 double3::operator * (const double &f) 	{ return double3(x*f,y*f,z*f); }
    double3 double3::operator + (const double &f) 	{ return double3(x+f,y+f,z+f); }
    double3 double3::operator - (const double &f) 	{ return double3(x-f,y-f,z-f); }

    bool double3::operator > (const double &f) {
        if(x*x+y*y+z*z > f*f) return true;
        return false;
    }
    bool double3::operator < (const double &f) {
        if(x*x+y*y+z*z < f*f) return true;
        return false;
    }

    double double3::length() { return sqrt(x*x+y*y+z*z); }


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

