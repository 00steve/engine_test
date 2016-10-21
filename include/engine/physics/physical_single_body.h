#ifndef SINGLE_BODY_OBJECT_H
#define SINGLE_BODY_OBJECT_H

#include "../core/physical.h"


class physicalSingleBody : public physical{
protected:

    dBodyID body;
    dGeomID geom;
    float gl_matrix[16];
	double3 position;
	bool active;

	/*variables to tell the object the angle of the
	floor. 0 radians is flat (facing directly upwards). 1.57
	radians is a wall, and then everything inbetween.*/
	bool trackFloor;
	double floorAngle;

public:

	physicalSingleBody() :
		trackFloor(false),
		floorAngle(0)
	{
	}

	~physicalSingleBody()
	{
		#if DEBUG_DELETES
			cout << " single body >";
		#endif
		ode::removeGeom(geom);
		ode::removeBody(body);
		delete gl_matrix;
	}


	void update(){
        if((active = dBodyIsEnabled(body))){
            ode::glMatrix(body,gl_matrix);
            const dReal *tPos = dBodyGetPosition(body);
            position.x = tPos[0];
            position.y = tPos[1];
            position.z = tPos[2];
        }
		physical::update();
	}

	/*bool handleMessage(node_message* m){
        switch(m->code){
        case MESSAGE_PROJECTILE_IMPACT:
            cout << this->getName() << " hit with bullet!\n";
            return true;
        }
        return physical::handleMessage(m);
	}*/

	bool handleMessage(node_message* m){
		switch(m->code){
		case MESSAGE_PROJECTILE_IMPACT:
			impact* i = (impact*)m->data;
			addForceAtPoint(i->velocity * i->mass * .25f, i->position);
			return true;
		}
		return physical::handleMessage(m);
	}

	virtual void setKinematic(){
		dBodySetKinematic(body);
	}
	virtual void setDynamic(){
		dBodySetDynamic(body);
	}

	void setLinearDampening(double scale){
		//dBodySetLinearDamping(body,scale);
	}

	void setAngularDampening(double scale){
		dBodySetAngularDamping(body,scale);
	}

	void disableBodies(){
		dBodyDisable(body);
	}

    double3 getPosition(){ return position; }

	void setPosition(double3 newPosition){
		dBodySetPosition(body,newPosition.x,newPosition.y,newPosition.z);
	}

	void shiftPosition(const double3 offset){
		dBodySetPosition(body, position.x+offset.x, position.y+offset.y, position.z+offset.z);

		ode::glMatrix(body,gl_matrix);
		const dReal *tPos = dBodyGetPosition(body);
		position.x = tPos[0];
		position.y = tPos[1];
		position.z = tPos[2];
	}

	/**\brief adds a force at the COG of an object.

	The force is added at the COG(center of gravity)
	of the object. This force causes acceleration, but
	not rotational change because it is acting on the
	center of gravity. If you want to simulate push or
	some type of force acting on an object that could
	cause velocity or rotational change, use the
	addForceAtPoint() function**/
	void addForce(const double3 force){
		dBodyAddForce(body,force.x,force.y,force.z);
	}

	/**\brief adds a force to the object at a specific point.

	This is useful for a collision or simulating a force acting
	on an object somewhere on the surface.**/
	void addForceAtPoint(const double3 force,const double3 origin){
		dBodyAddForceAtPos(body,force.x,force.y,force.z,origin.x,origin.y,origin.z);
		setEnabled(true);
	}

	void setVelocity(const double3 velocity){
		dBodySetLinearVel(body,velocity.x,velocity.y,velocity.z);
	}

	bool isEnabled(){ return dBodyIsEnabled(body); }
	bool setEnabled(const bool enabled){
		if(enabled){
			dBodyEnable(body);
		} else {
			dBodyDisable(body);
		}
		return enabled;
	}

};

#endif // SINGLE_BODY_OBJECT_H
