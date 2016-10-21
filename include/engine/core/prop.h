#ifndef PROP_H
#define PROP_H
#include "../core/controllable.h"

class prop : public drawable,public controllable{
private:

	/*set references to the actor that is using the prop,
	so that it knows where it is and the rotation it is at*/
	double3 *actorPosition;
	double3 *actorLookRotation;

public:

	prop() :
			actorPosition(NULL),
			actorLookRotation(NULL){
	}

	~prop(){
		#if DEBUG_DELETES
			cout << " prop >";
		#endif
	};


	void draw(){}


	void update(){
		node::update();
	}

	void setActorPosition(double3 *positionReference){
		actorPosition = positionReference;
	}

	void setActorLookRotation(double3 *lookRotationReference){
		actorLookRotation = lookRotationReference;
	}

	double3 getActorPosition(){ return *actorPosition; }
	double3 getPosition(){
		return *actorPosition + getActorLookVector();
	}
	double3 getActorLookRotations(){ return *actorLookRotation; }

	/**\brief gets the vector equivilent of where the actor is looking.

	The returned value is calculated from the getActorLookRotation() x
	and y values.**/
	double3 getActorLookVector(){
		//return double3(
		//				sinf(actorLookRotation->y),
		//				sinf(actorLookRotation->x),
		//				cosf(actorLookRotation->y));
		return double3(
						sinf(actorLookRotation->y)*cosf(actorLookRotation->x),
						sinf(actorLookRotation->x),
						cosf(actorLookRotation->y)*cosf(actorLookRotation->x));
	}
};


#endif // PROP_H
