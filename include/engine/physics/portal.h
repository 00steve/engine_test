
#ifndef PORTAL_H
#define PORTAL_H

#include "../physics/collision_trigger.h"

class portal : public collision_trigger{
private:

	list<node*> transportedPhysicals;
	double3 offset;
	double3 *origin;
	bool onContact;

protected:

	/*if any of the referenced objects are deleted, remove it from
	the list of objects.*/
	void onReferenceRemoval(node* oldRef){
		transportedPhysicals.remove((physical*)oldRef);
		collision_trigger::onReferenceRemoval(oldRef);
	}


	void transportNode(physical *node){
		transportedPhysicals.push(node);
		(node)->shiftPosition(*origin+offset);
	}


public:

	portal(varMap settings) : collision_trigger(settings){

		offset = double3(10,0,0);
		origin = new double3(0,0,0);
		onContact = true;
	}

	~portal(){
		#if DEBUG_DELETES
			cout << " portal >";
		#endif
	}

	/**\brief sends message to other node that the pickup can be
	used.**/
    void startTrigger(node *n){
    	/*if the object has already been transported but is still in the
    	collision area of the physical, disable it until the object has
    	left the collision area.*/
    	startUsing(n);
		//transportedPhysicals
		if(onContact){
			transportNode((physical*)n);
		}
		else {
			sendMessage(n,MESSAGE_CAN_INTERACT,this);
		}
    }

	/**\brief sends message to the other node that the pickup can
	no longer be used.**/
    void endTrigger(node *n){
    	stopUsing(n);
    	transportedPhysicals.remove((physical*)n);
		sendMessage(n,MESSAGE_CANT_INTERACT,this);
    }

    bool handleMessage(node_message* m){
		switch(m->code){
		case MESSAGE_PRIMARY_INTERACT:
			transportNode((physical*)m->sender);
			return true;

		}
		return collision_trigger::handleMessage(m);
    }
};




#endif // PORTAL_H
