#ifndef CAR_H
#define CAR_H

#include "../core/vehicle.h"
#include "vehicle/macpherson_strut.h"

class car : public vehicle{
private:

	list<drawable*> drawableChildren;


	dBodyID chassisBody;
	dGeomID chassisGeom;

	bool buildChassis(varMap settings){
		ode::newBox(chassisBody,chassisGeom,double3(5,4,13),1,this);
		return true;
	}

	/**/
	bool addSuspension(varMap settings){
		string type = settings.getString("type","[set a type, moron]");

		if(type == "macpherson"){
			cout << "MacPherson strut\n";
			addDrawable(new macPhersonStrut(this,settings));
			return true;
		}

		cout << "I don't know how to build a " << type << " suspension... tough shit, son.\n";
		return false;
	}

public:

	car(varMap settings){
		physical::setGroup(physical::getNextCollisionGroupId("strut"));
		cout << "-------------------------\n-------build a car-------\n-------------------------\n";
		buildChassis(settings.getGroup("chassis"));
		addSuspension(settings.getGroup("front_left_suspension"));
		addSuspension(settings.getGroup("front_right_suspension"));
		//addSuspension(settings.getGroup("rear_left_suspension"));
		//addSuspension(settings.getGroup("rear_right_suspension"));

		cout << "-------------------------\n\n";
	}
	~car(){

	}

	void draw(){
		drawableChildren = getDrawables();
		for(int i=0;i<drawableChildren.getCount();i++){
			drawableChildren[i]->draw();
		}



	}
	bool isEnabled(){return false;}
	bool setEnabled(bool enabled){return enabled;}
	double3 getPosition(){return double3(0,0,0);}

	dBodyID getBody(unsigned int index){
		if(index == 0){
			return chassisBody;
		}
        return NULL;
    }

};
#endif // CAR_H
