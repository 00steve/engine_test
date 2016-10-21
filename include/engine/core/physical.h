/*the physical class provides an extension to any class
that wants to store properties about how it should collide
or interact with other physical objects in the ODE simulation.*/

#ifndef PHYSICAL_H
#define PHYSICAL_H

#include "../core/node.h"

//#include "../physics/collision.h"
#include "../physics/impact.h"
//#include "../physics/contact_settings.h"

#include <ode/objects.h>

#define PROJECTILE_GROUP 1
#define EXPLOSION_GROUP 2

#define GOOD_GUYS 10
#define BAD_GUYS 11
#define OTHER_GUYS 12


class physical : public drawable {
private:

    /*the maximum number of  contact points that can be created
    between this physical object's bodies and any other physical
    object's bodies.*/
    int maxContactPoints;

    /*if the object is set as virtual, it will not generate
    collision contact points between it and any other physicals
    that it is colliding with. Making something virtual is great
    when you want to see if things intersect without colliding
    them.*/
    bool virt;

    bool stat;

	bool grouped;

	bool selected;
	bool selectable;

    /*if the int is in a group of like bodies, they should not
    generate collision data. This is useful because you could
    set all projectiles in the engine to one group, so they
    will not interact with each other, or set all of the bodies
    in a vehicle to one group so that they won't interact with
    each other. Each physical that is created is automatically
    given its own group (it's unique node id is used).*/
    unsigned int group;

    unsigned int contact_mode;

	bool shouldDefaultEnabled;

	/*easy helper properties to store collision groups so they
	don't have to be a compiler something or another.*/
	static map<int> collision_groups;
	static int next_collision_group_id;

protected:

    /*store a list of nodes that this node has collided with.
    This will be performed by the physics engine.*/
    list<collision*>  collisions;

	static int getNextCollisionGroupId(string name){
		collision_groups.push(next_collision_group_id,name);
		return next_collision_group_id++;
	}

public:

    physical() :
        maxContactPoints(4),
        virt(false),
        stat(false),
        grouped(false),
        selected(false),
        selectable(true),
        group(0),
        contact_mode(dContactBounce | dContactSoftCFM),
        shouldDefaultEnabled(false) {
    }

    ~physical(){
        #if DEBUG_DELETES
        cout << " physical >";
        #endif

		collisions.clearAndDelete();

    }

    /**\brief sets the contact parameters to use for contact
    points between this body and other bodies in the physics
    engine.**/
    void setContactMode(unsigned int mode){
        contact_mode = mode;
    }

    void update(){
        node::update();
        collisions.clearAndDelete();
    }

    void addCollision(collision *c){
        collisions.push(c);
    }

    node* getNode(){
        return this;
    }

	string getClass(){ return "physical"; }

    int getMaxContactPoints(){
        return maxContactPoints;
    }

    /*check if an object is virtual*/
    bool isVirtual(){ return virt; }

    /*check if an object is static*/
    bool isStatic(){ return stat; }

	/**\brief check if a physical object is enabled or not.

	This is a general type function, so it is left up to things
	implementing it to decide if they are enabled or not.**/
	virtual bool isEnabled() = 0;

    /**\brief if an object is virtual it will not generate contact
    hinges that apply forces to it or what is colliding
    with it.**/
    bool setVirtual(bool setVirt){ return virt = setVirt; }

    /**\brief set if the object should not collide with itself or
    other objects of its type.**/
    bool setStatic(bool setStat){ return stat = setStat; }

	/**\brief "should" set the object and any bodies it is comprised
	of to have infinite mass.**/
	virtual void setKinematic(){}

	/**\brief should remove the infinite mass from any bodies that
	the object is comprised of:

	basically, set them back to what they were before they went
	kinematic**/
	virtual void setDynamic(){}

	virtual bool setEnabled(const bool enabled) = 0;

	virtual void setLinearDampening(double scale){}
	virtual void setAngularDampening(double scale){}

    virtual double3 getPosition()=0;

	/**\brief implimentable function that lets an inheriting class
	determine how it should work. It should shift the position of
	the object by the amount provided in world space.*/
	virtual void shiftPosition(const double3 offset){}

    /**\brief removes the geom or geoms from a physical
    and attaches them to a body.*/
    virtual void glueTo(dBodyID body,double3 offset){}

    /**\brief takes two bodies and attaches them together
    with a joint. If too much force is applied to this
    joint, it will break.*/
    virtual void attachTo(dBodyID body,double3 offset,double strength){}

    /**\brief add forces to any bodies in the physical*/
    virtual void addForce(const double3 force){};

	/**\brief ignoring what the current velocity or other
	forces are acting on the object, set its velocity however
	the inheriting clas wishes to the amount set in
	world space*/
	virtual void setVelocity(const double3 velocity){};

    /**\brief returns a body that is being updated in the physics
    engine that the physical object is using.

    By default, no body is returned. It is up to the inheriting
    class to determine if a body can be returned or what index
    any bodies should have.**/
    virtual dBodyID getBody(unsigned int index){
        return NULL;
    }

	/**\brief returns the number of bodies that the physical object
	has.

	It defaults to zero so that the program doesn't die, but should
	be overwritten by any inheriting classes.**/
	virtual unsigned int getBodyCount(){
		return 0;
	}

    /**\brief returns a joint that is being updated in the physics
    engine that the physical object is using.

    By default, no joint is returned. It is up to the inheriting
    class to determine if a joint can be returned or what index
    any joints should have.**/
    virtual dJointID getJoint(unsigned int index){
        return NULL;
    }

	bool isGrouped(){
		return grouped;
	}

	/*if the object can be selected. This allows an object to exist
	in the physics engine, that can be selected by the selectionVector,
	but can be discarded if the user should not be able to select it.*/
	virtual bool isSelectable(){
		return selectable;
	}

    unsigned int getGroup(){
        return group;
    }

    unsigned int setGroup(unsigned int newGroup){
        return group = newGroup;
    }

    /**\brief return the contact settings of a particular geom that
    is part of this physical object.

    By default nothing is returned. This can be overridden so that
    it returns any settings based on the geom id that is passed to
    it. This allows the programmer to let physicals return a different
    contact_setting for each contact part of it. */
    virtual dSurfaceParameters* getSurfaceParameters(dGeomID){
        return NULL;
    }

	bool handleMessage(node_message* m){
        switch(m->code){
		case MESSAGE_SELECTED:
			selected = true;
			selectable = false;
			return true;
		case MESSAGE_DESELECTED:
			selected = false;
			selectable = true;
			return true;
		case MESSAGE_SET_VELOCITY:
			return true;
		case MESSAGE_SET_ORIENTATION:
			return true;
		case MESSAGE_SET_ROTATION:
			return true;
        }
        return drawable::handleMessage(m);
	}

	virtual void disableBodies(){}

	/**\brief this is a generic function that returns if the physical
	object is enabled or not.

	If it is, things should collide against it and its bodies should still
	be active in the simulation. If this is set to false, none of the bodies
	that constitutes the physical should be enabled.*/
	virtual bool defaultEnabled(){ return shouldDefaultEnabled; };

	virtual void setDefaultEnabled(const bool enabled){
		shouldDefaultEnabled = enabled;
	}


};


map<int> physical::collision_groups = map<int>();
/*start it at 100 for some reason, it doesn't matter*/
int physical::next_collision_group_id = 100;

#endif // PHYSICAL_H
