#ifndef ENGINE_BARREL_H
#define ENGINE_BARREL_H

#include "../physics/physical_single_body.h"


class barrel : public physicalSingleBody {
private:


	GLuint dl;

	void setup_physics(){
		ode::newCylinder(body,geom,1.33,.5,1,this);
        dBodySetAutoDisableLinearThreshold (body, .5);
        dBodySetAutoDisableAngularThreshold (body, .06);
        dBodySetAutoDisableSteps (body, 25);
		setPosition(position);
	}

	void load_settings(varMap settings){
		position = settings.getDouble3("offset",double3(0,0,0));
	}

public:

	barrel(varMap settings){
		load_settings(settings);
		setup_physics();
        dl = assetLibrary::getDisplayList("include/assets/models/explodey_barrel/explodey_barrel.obj");
        setName(new string("explody_barrel_" + numberToString<unsigned long>(getId())));
	}


    void draw(){
        glPushMatrix();
            glMultMatrixf(gl_matrix);
			glCallList(dl);
        glPopMatrix();
    }

};



#endif // ENGINE_BARREL_H
