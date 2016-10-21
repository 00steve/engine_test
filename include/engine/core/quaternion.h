#ifndef QUATERNION_H
#define QUATERNION_H

/*my custom quaternion class. I wanted to know how they work and
wanted to use my own utility classes to build it.

The getForwardVector, getUpVector and getRightVector were pretty
much ripped from Nic Foster at http://nic-gamedev.blogspot.com/
2011/11/quaternion-math-getting-local-axis.html
*/




#include <math.h>
#include "double3.h"
#include "double4x4.h"

class quaternion {
private:

    double x,y,z,w;
    double magnitude;

    /*if the magnitude of the quaternion falls out of
    the acceptable range, renormalize it to make it a
    unit quaternion.*/
    void normalize() {
        if((magnitude = x*x+y*y+z*z+w*w) > 1.01) {
            //cout << "needed normalizing\n";
            magnitude = sqrt(magnitude);
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
            w /= magnitude;
        }

    }

public:
    quaternion() :
        x(0),
        y(0),
        z(0),
        w(1),
        magnitude(1) {
    }

    /*generate a quaternion from an offset*/
    quaternion(double3 offset) :
        x(offset.x),
        y(offset.y),
        z(offset.z),
        w(1) {
        normalize();
    }

    /*create a new quaternion with all of the values
    already set.*/
    quaternion(double x,double y,double z,double w){
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
        normalize();
    }

    /*create a new quaternion from a unit vector
    rotation.*/
    quaternion(double3 axis,double angle){
        //axis is a unit vector

        w  = cosf( angle/2);
        x = axis.x * sinf( angle/2 );
        y = axis.y * sinf( angle/2 );
        z = axis.z * sinf( angle/2 );
        normalize();
    }

    /*multiply this quaternion by another quaternion. This is
    NON-COMMUTATIVE, which means Q1 x Q2 != Q2 x Q1. To rotate
    PLAYER_ROT using another quaternion, the code would look
    like FINAL_ROATAION = TURN_AMOUNT * PLAYER_ROT*/
    quaternion operator *(const quaternion &b){
        quaternion result;
        result.w = (b.w * w) - (b.x * x) - (b.y * y) - (b.z * z);
        result.x = (b.w * x) + (b.x * w) + (b.y * z) - (b.z * y);
        result.y = (b.w * y) + (b.y * w) + (b.z * x) - (b.x * z);
        result.z = (b.w * z) + (b.z * w) + (b.x * y) - (b.y * x);
        result.normalize();
        //return new quaterion
        return(result);
    }

    /*conjugate the current quaternion.*/
    quaternion conjugate(){
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    /*return conjugate copy of current quaternion*/
    quaternion getConjugate(){
        return quaternion(-x,-y,-z,w);
    }

    /*define vector multiplication operator*/
    double3 operator *(double3 v){
        quaternion vf = quaternion(v);
        //apply multiplications
        vf = vf * getConjugate();
        vf = *this * vf;
        return double3(vf.x, vf.y, vf.z).normalize();
    }

    /*allow a quaternion to have a double3 added and return a
    new double3 that combines the quaternion's offset and
    the double3*/
    double3 operator +(double3 v){
        double3 t = double3(x,y,z).normalize();
        return t+v;
    }

    /*converts the quaternion to a 4x4 rotation matrix. It can
    be used by openGL to rotate something*/
    float* toMatrix(float* matrix){
        // First row
        matrix[0]	= 1.0f - 2.0f * ( y * y + z * z );
        matrix[1]	= 2.0f * (x * y + z * w);
        matrix[2]	= 2.0f * (x * z - y * w);
        matrix[3]	= 0.0f;
        // Second row
        matrix[4]	= 2.0f * ( x * y - z * w );
        matrix[5]	= 1.0f - 2.0f * ( x * x + z * z );
        matrix[6]	= 2.0f * (z * y + x * w );
        matrix[7]	= 0.0f;
        // Third row
        matrix[8]	= 2.0f * ( x * z + y * w );
        matrix[9]	= 2.0f * ( y * z - x * w );
        matrix[10]	= 1.0f - 2.0f * ( x * x + y * y );
        matrix[11]	= 0.0f;
        // Fourth row
        matrix[12]	= 0;
        matrix[13]	= 0;
        matrix[14]	= 0;
        matrix[15]	= 1.0f;
        return matrix;
    }


	float* toRotationMatrix(float* matrix){
        matrix[0]=1;
        matrix[1]=0;//4
        matrix[2]=0;//8
        matrix[3]=0;

        matrix[4]=0;//1
        matrix[5]=1;
        matrix[6]=0;//9
        matrix[7]=0;

        matrix[8]=0;//2
        matrix[9]=0;//6
        matrix[10]=1;
        matrix[11]=0;
		double3 forw = getForwardVector();
        matrix[12]= -forw.x * 57.29578f;
        matrix[13]= -forw.y * 57.29578f;
        matrix[14]= -forw.z * 57.29578f;
        matrix[15]=1;

        return matrix;
	}


    /*gets the quaternion representation of the difference between
    this quaternion and another quaternion.*/
    quaternion getDelta(quaternion q){
        //x = -x;y = -y;z = -z;//conjugate
        //quaternion result = *this * q;
        //x = -x;y = -y;z = -z;//conjugate
        return getConjugate() * q;
    }

    double3 getForwardVector(){
        return double3( 2 * (x * z + w * y), 2 * (y * x - w * x), 1 - 2 * (x * x + y * y));
    }

    double3 getUpVector(){
        return double3( 2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x));
    }

    double3 getRightVector(){
        return double3( 1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y));
    }

    quaternion interpolate(quaternion q,double t){
        return quaternion(x*(1-t)+q.x*t, y*(1-t)+q.y*t, z*(1-t)+q.z*t, w*(1-t)+q.w*t);
    }

    void print(){
        cout << "(" << x << "," << y << "," << z << "," << w << ")\n";

    }

	/*some static functions that generate quaternions in
	default orientations*/
    static quaternion backward(){
        return quaternion(0,1,0,0);
    }
    static quaternion down(){
        return quaternion(.7068,0,0,.7068);
    }
    static quaternion up(){
        return quaternion(-.7068,0,0,.7068);
    }
    static quaternion right(){
        return quaternion(0,-.7068,0,.7068);
    }
    static quaternion left(){
        return quaternion(0,.7068,0,.7068);
    }

    static quaternion fromMatrix(float *r){
        return quaternion();
    }

	/*some static functions to generate quaternions at
	arbitrary rotations. They are generally used multipled
	with existing quaternions to rotate them.*/
    static quaternion rotateX(double x){
        return quaternion(sin(x/2),0,0,cos(x/2));
    }
    static quaternion rotateY(double y){
        return quaternion(0,sin(y/2),0,cos(y/2));
    }
    static quaternion rotateZ(double z){
        return quaternion(0,0,sin(z/2),cos(z/2));
    }
};
#endif

