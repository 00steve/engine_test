#ifndef KEYBOARD_MOUSE_H
#define KEYBOARD_MOUSE_H
#include "../core/input.h"
#include "../core/controller.h"

class keyboard_mouse : public controller{
private:

    /*store the amount of movement that is happening with the
    input keys*/
    double2 m;
    /*store the look rotation of the turn_direction variable*/
    double2 l;

public:

    /*If no pad is set, it automatically chooses
    the first one.*/
    keyboard_mouse():
    	controller(NULL),
        m(double2(0,0)),
        l(double2(0,0)){
    }

    keyboard_mouse(varMap settings) :
				controller(settings.isSet("id") ? new string(settings.getString("id","")) : NULL)
			{
		cout << getName() << endl;
    }

    void update(){
        input::updateMouse();
        m.x = input::keys['A']-input::keys['D'];
        m.y = input::keys['W']-input::keys['S'];
    }

    double2 move_direction(){
        return m;
    }

    double2 turn_direction(){
        return double2(input::mouseSmoothMovement.x,input::mouseSmoothMovement.y);
    }

    bool primary_interaction(){
        return input::keys['Q'];
    }

    bool secondary_interaction(){
        return input::keys['E'];
    }

    bool primary_action(){
        return (input::mouseButton[0]);
    }

    bool secondary_action(){
        return (input::mouseButton[1]);
    }

	bool primary_modify(){
		return (input::keys[289]);
	}

	bool secondary_modify(){
		return (input::keys[287]);
	}

    int2 cursor_position(){
		return input::mousePosition;
	}

};

#endif // KEYBOARD_MOUSE_H
