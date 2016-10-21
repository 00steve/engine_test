#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../core/double3.h"
#include "../core/action.h"
#include "../core/event.h"
#include "../core/node.h"

/*the controller interface lets the programmer set custom ways to perform the default actions of
the engine. There are default controls that can be mapped out.*/
class controller : public node{
protected:


    /*allow up to 6 axii of variable from -1 to 1 input.*/
    float axis[6];

    /*the button that pauses the game or brings up the menu.*/
    bool menu;

    /*if there is a help button*/
    bool help;

    /*allow up to 6 action buttons that are on or off.*/
    bool action[8];


public:

	controller(string* name){
		if(name){
			setName(name);
		}else{
			setName(new string("controller_" + numberToString<unsigned long>(getId())));
		}
	}

    virtual void update() = 0;

    virtual double2 turn_direction() = 0;

    virtual double2 move_direction() = 0;

    virtual bool primary_interaction() = 0;

    virtual bool secondary_interaction() = 0;

    virtual bool primary_action() = 0;

    virtual bool secondary_action() = 0;

    virtual int2 cursor_position() = 0;

	/*a input with the ability to modify what is going on. This would
	be most important for the keyboard, possibly the SHIFT/TAB/ALT keys*/
    virtual bool primary_modify() = 0;
    virtual bool secondary_modify() = 0;

	/* AFAIK, this is deprecated, it does do something simple and
	unperfect, though. It uses the depth buffer to determine where
	a click actually occured in the real 3d space.

    double3 cursor_position_3d(){
    	int2 pos = cursor_position();
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;
		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
		glGetDoublev( GL_PROJECTION_MATRIX, projection );
		glGetIntegerv( GL_VIEWPORT, viewport );
		winX = (float)pos.x;
		winY = (float)pos.y;
		glReadPixels( pos.x, pos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
		gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
		return double3(posX, posY, posZ);
    }
    */

    bool button(int index){ return action[index]; }
};




#endif // CONTROLLER_H
