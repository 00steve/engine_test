
#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "../core/camera.h"

class fps_camera : public camera {
protected:
    double3 *offset;
    double3 *rotation;
    float fov;
    GLfloat *m;
    void internal_update(){

    }



public:

    fps_camera(varMap settings) :
				camera(settings.isSet("id") ? new string(settings.getString("id","")) : NULL),
				offset(new double3(0,0,0)),
				rotation(new double3(0,0,0)) {
    }

    ~fps_camera(){
    }

	virtual bool handleMessage(node_message* m){
		switch(m->code){
		case MESSAGE_SET_POSITION_REFERENCE:
			offset = (double3*) m->data;
			return true;
		case MESSAGE_SET_ROTATION_REFERENCE:
			rotation = (double3*) m->data;
			return true;
		}

		return camera::handleMessage(m);
	}


    void translateView(float aspectRatio){

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective( 45.0, aspectRatio, near_clip, far_clip );
        gluLookAt(offset->x, 							    	//cameara position x
                offset->y, 								    	//cameara position y
                offset->z, 								    	//cameara position z
                offset->x+sin(rotation->y)*cos(rotation->x),	//look position x
                offset->y+sin(rotation->x),  				    //look position y
                offset->z+cos(rotation->y)*cos(rotation->x),  	//look position z
                -sin(rotation->y)*sin(rotation->x),				//up rotation x
                cos(rotation->x),								//up rotation y
                -cos(rotation->y)*sin(rotation->x));			//up rotation z
    }

	double3 getPosition(){ return *offset; }

};


#endif // FPS_CAMERA_H


