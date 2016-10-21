#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

/*build a class the lets any other object set, use, and remove
a controller from itself.*/

#include "../core/controller.h"
#include "../input/keyboard_mouse.h"



class controllable{
protected:

	controller *control;

public:

	controllable():
		control(NULL){}

	controllable(controller *control) :
			control(control)
			{
	}

	virtual controller* setController(controller *control){
		return this->control = control;
	}

	virtual bool unsetController(){ return !control || !(control = NULL); }

	controller* getController(){ return control; }

};





#endif // CONTROLLABLE_H
