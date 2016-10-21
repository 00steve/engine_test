#ifndef FORCE_SPHERE_H
#define FORCE_SPHERE_H

#include "../physics/collision_trigger.h"

/**\brief applies a force to objects that intersect with a spherical area.

**/
class forceSphere : collision_trigger{
private:


protected:

	double radius;


	int falloff_method;

	/*the amount of foce applied to an object at an infinitely
	small distance to the center of the force sphere. This has
	a falloff from the center to the edge, which does its own
	shit to figure out how much of the available force should be
	applied.*/
	double force;

public:

	forceSphere(){
		setVirtual(true);
	}

	~forceSphere(){
		#if DEBUG_DELETES
			cout << " force sphere >";
		#endif
	}


	void update(){
		collision_trigger::update();
	}

};



#endif // FORCE_SPHERE_H
