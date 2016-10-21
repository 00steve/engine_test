#ifndef ATTACHED_CAMERA_H
#define ATTACHED_CAMERA_H

#include "../core/camera.h"

/*can be attached to any object. It is
attached at the moment of creation and should cease to
exist when it is no longer needed.

It's orientation is derived from the PHYSICAL object
that it is attached to. It uses an offset relative to
the local space of the object it is attached to, then
it uses the rotation and position of the attached
object to translate everything to match the object it
is attached to.
*/

class attached_camera : public camera {
private:

	/*store the body that is being referenced.*/
	dBodyID *body;

	/*store the rotation and position of the body
	so they exist between the update() and draw()
	calles.*/
	float gl_t_matrix[16];
	float gl_r_matrix[16];

	/*store the offset of the camera relative to
	the body that it is attached to.*/
	double3 offset;

	void internal_update(){
		if(!body) return;
		//ode::glTranslationMatrix(*body,gl_t_matrix);
		//ode::glRotationMatrix(*body,gl_r_matrix);
	}


	double a;

public:

	attached_camera(varMap settings) :
				camera(settings.isSet("id") ? new string(settings.getString("id","")) : NULL),
				body(NULL),
				offset(settings.getDouble3("offset",double3(0,0,0)))
			{
		//do stuff to set up the camera
		a = 0;
	}


	void update(){
		a+=.1f;
		internal_update();
        handleMessages();
	}


	virtual bool handleMessage(node_message* m){
		switch(m->code){
		case MESSAGE_SET_TARGET:
			body = (dBodyID*) m->data;
			return true;
		}
		return camera::handleMessage(m);
	}


	void translateView(float aspectRatio){
		if(!body) return;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective( fieldOfView, aspectRatio, near_clip, far_clip );
		glTranslated(offset.x,offset.y,offset.z);
		glRotated(180,0,1,0);
		glRotated(a,0,1,0);
		glMultMatrixf(gl_t_matrix);
	}

	double3 getPosition(){ return offset; }

};


#endif // ATTACHED_CAMERA_H
