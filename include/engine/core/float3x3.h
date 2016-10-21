#ifndef FLOAT3x3_H
#define FLOAT3x3_H

#include "double3.h"

/*a generic matrix class with ambitions to output arrays of
3x4 doubles that are compatible with ODE physics engine*/
class float3x3{
private:

	float m[9];

public:


	float* toDMatrix3(){
		float *n;
		n=new float[12]();
		n[0]=m[0];	n[1]=m[1];	n[2]=m[2];	n[3]=0;
		n[4]=m[3];	n[5]=m[4];	n[6]=m[5];	n[7]=0;
		n[8]=m[6];	n[9]=m[7];	n[10]=m[8];	n[11]=0;
		return n;
	}


	static float3x3 identity(){
		float3x3 n;
		n.m[0] = 1;	n.m[1] = 0;	n.m[2] = 0;
		n.m[3] = 0;	n.m[4] = 1;	n.m[5] = 0;
		n.m[6] = 0;	n.m[7] = 0;	n.m[8] = 1;
		return n;
	}

	static float3x3 xRotation(float r){
		float3x3 n;
		n.m[0] = 1;	n.m[1] = 0;			n.m[2] = 0;
		n.m[3] = 0;	n.m[4] = cos(r);	n.m[5] = sin(r);
		n.m[6] = 0;	n.m[7] = -sin(r);	n.m[8] = cos(r);
		return n;
	}

	static float3x3 yRotation(float r){
		float3x3 n;
		n.m[0] = cos(r);	n.m[1] = 0;			n.m[2] = -sin(r);
		n.m[3] = 0;			n.m[4] = 1;			n.m[5] = 0;
		n.m[6] = sin(r);	n.m[7] = -sin(r);	n.m[8] = cos(r);
		return n;
	}

	static float3x3 zRotation(float r){
		float3x3 n;
		n.m[0] = cos(r);	n.m[1] = sin(r);	n.m[2] = 0;
		n.m[3] = -sin(0);	n.m[4] = cos(r);	n.m[5] = 0;
		n.m[6] = 0;			n.m[7] = 0;			n.m[8] = 1;
		return n;
	}


};





#endif // DOUBLE3x4_H
