#ifndef CRATE_H
#define CRATE_H

#include "../physics/physical_single_body.h"

class crate : public physicalSingleBody{
private:

    mesh *m;
	GLuint dl;
    double3 last_collision;
	int life;

    void buildBox() {
        ode::newBox(body,geom,double3(1,1,1),1,this);
        dBodySetAutoDisableLinearThreshold (body, .5);
        dBodySetAutoDisableAngularThreshold (body, .06);
        dBodySetAutoDisableSteps (body, 25);
        dBodyEnable(body);
    }

	void init(){
        buildBox();
        dBodySetPosition(body,position.x,position.y,position.z);
        ode::glMatrix(body,gl_matrix);
        dl = assetLibrary::getDisplayList("include/assets/models/crate_lo_z/crate_low_z.obj");
        setName(new string("crate_" + numberToString<unsigned long>(getId())));
        life = 3000;
	}


public:
    crate(double3 position) {
        this->position = position;
		init();
    }

    crate(double3 position,double3 velocity){
		this->position = position;
		init();
		dBodySetLinearVel(body,velocity.x,velocity.y,velocity.z);
    }

	~crate(){
		#if DEBUG_DELETES
			cout << " crate >";
		#endif
	}


	/**\brief the crate implimentation of the draw method.

	This updates the projection matrix with the oriention of the
	cube that represents the crate, and calls the draw list that was
	created using the crate model.**/
	void draw() {
        glPushMatrix();
            glMultMatrixf(gl_matrix);
			glColor3f(1,1,1);
			glCallList(dl);
        glPopMatrix();
	}
};




#endif // CRATE_H
