
#ifndef CAMERA_H
#define CAMERA_H

#include "../core/double3.h"
#include "../core/quaternion.h"
#include "../core/node.h"
#include "../core/int2.h"
#include "../core/controller.h"
#include "../core/var_map.h"
#include "../core/controllable.h"

class camera : public node,public controllable{
private:

    bool pauseState;

protected:

    /*store a reference to a controller that is used to
    control the camera. If none, is set, it won't have
    any control. Any control it might has should be programmed
    in any cameras that impliment this class.*/
	//controller *control;

    /*to customize the way that a camera behaves, impliment
    the internal_update. This will only run if the camera is
    not paused.*/
    virtual void internal_update() = 0;

    float fieldOfView;

	double near_clip;
	double far_clip;

public:
    camera(string* cameraName) :
				pauseState(false),
				fieldOfView(45),
				near_clip(.1),
				far_clip(1500)
			{

		if(cameraName){
			setName(cameraName);
		}else{
			setName(new string("camera_" + numberToString<unsigned long>(getId())));
		}
		cout << "add camera : " << getName() << endl;
    }

	~camera(){
		#if DEBUG_DELETES
			cout << " camera >";
		#endif
	}

    /*generic update routine that
    lets the camera update any of its internal
    values*/
    void update(){
        /*if the camera is paused, it shouldn't update
        it should just keep doing what it has been doing
        until it is un-paused.*/
        if(paused()) return;
        if(control) control->update();
        internal_update();
		handleMessages();
    }

	virtual bool handleMessage(node_message* m){
		return node::handleMessage(m);
	}

    /*perform any openGL setup functions that
    need to be run before the view can be
    drawn. It requires the width and height of
    the viewport so that it can set everything
    properly.

    I chose to have the programmer set it every
    time they want to draw a view so that a
    camera can be used with multiple views ( I
    have no idea why you would) but it should be
    covered just in case.*/
    virtual void translateView(float aspectRatio) = 0;

    float getDrawDistance(){
        return far_clip;
    }

	float getFieldOfView(){ return fieldOfView; }

    void pause(){
        pauseState = true;
    }

    void resume(){
        pauseState = false;
    }

    bool paused(){ return pauseState; }

	float setFieldOfView(float newFieldOfView){
		return fieldOfView = newFieldOfView;
	}


	void setClippingPlanes(double near, double far){

	}

	/**/

	GLdouble* getModelviewMatrix(){
		GLdouble *m = new GLdouble[16]();
		glGetDoublev( GL_MODELVIEW_MATRIX, m );
		return m;
	}

	GLdouble* getProjectionMatrix(){
		GLdouble *m = new GLdouble[16]();
		glGetDoublev(GL_PROJECTION_MATRIX, m);
		return m;
	}

	virtual double3 getPosition()=0;

};


#endif
