#ifndef FLOATING_CAMERA_H
#define FLOATING_CAMERA_H

#include "../core/camera.h"

class floating_camera : public camera {
    protected:
        quaternion rotation;
        double3 offset;
        float fov;
        GLfloat m[16];
        double3 angle;

        void internal_update(){
        	if(!control) return;
        	control->update();
            //camera angle
            angle.x -= (control->turn_direction().y*.0155f);
            angle.y -= (control->turn_direction().x*.0155f);
            if(fabs(angle.x)>1.5f) angle.x = angle.x / fabs(angle.x) *1.5;
            //camera position
            double2 mov = control->move_direction();
            mov = mov.normalize()*.005f;
            offset = offset + double3(
								mov.y*sin(angle.y)*cos(angle.x)+mov.x*cos(angle.y)*cos(angle.x),
								mov.y*sin(angle.x),
								mov.y*cos(angle.y)*cos(angle.x)-mov.x*sin(angle.y)*cos(angle.x) );
        }

        void setup(){
            angle.y = 3.14f;
        }
    public:

		floating_camera(varMap settings) :
			camera(settings.isSet("id") ? new string(settings.getString("id","")) : NULL),
			offset(settings.getDouble3("offset",double3(0,0,0))) {
            setup();
		}


        void translateView(float aspectRatio){
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective( fieldOfView, aspectRatio, near_clip, far_clip );
            gluLookAt(offset.x, 							//cameara position x
					offset.y, 								//cameara position y
					offset.z, 								//cameara position z
					offset.x+sin(angle.y)*cos(angle.x),		//look position x
					offset.y+sin(angle.x),  				//look position y
					offset.z+cos(angle.y)*cos(angle.x),  	//look position z
					-sin(angle.y)*sin(angle.x),				//up angle x
					cos(angle.x),							//up angle y
					-cos(angle.y)*sin(angle.x));			//up angle z

        }


		double3 getPosition(){ return offset; }
};


#endif // FLOATING_CAMERA_H
