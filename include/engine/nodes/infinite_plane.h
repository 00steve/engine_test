#ifndef	INFINITE_PLANE_H
#define INFINITE_PLANE_H

#include "../core/physical.h"

class infinite_plane : public physical{
private:
    dGeomID geom;
    double offset;
public:
    infinite_plane(double3 normal,double offset) :
    	offset(offset){
        ode::newStaticPlane(geom,normal,offset,this);
        setStatic(true);
    }


	void draw(){
		int x=1000,y,i = 100;
		while(x > -1000){
			y = 1000;
			while(y > -1000){
				glColor4d(1,1,1,1);
				glLineWidth(3);
				glBegin(GL_LINES);
					glVertex3f(x,offset,-1000);
					glVertex3f(x,offset,1000);
					glVertex3f(-1000,offset,y);
					glVertex3f(1000,offset,y);
				glEnd();
				y -= i;
			}
			x -= i;
		}
	}


	bool isEnabled(){return false;}
	bool setEnabled(bool enabled){return enabled;}
	double3 getPosition(){return double3(0,0,0);}

};








#endif
