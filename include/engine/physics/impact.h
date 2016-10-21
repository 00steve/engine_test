#ifndef IMPACT_H
#define IMPACT_H

/*stores information about one object colliding with another,
such as where the collision happened, the amount of force that
the collider applied and the velocity vector of the object that
collided with it.*/


struct impact{

	double3 position;
	double3 velocity;
	double mass;

	impact(double3 position,double3 velocity,double mass);

};



#endif // IMPACT_H
