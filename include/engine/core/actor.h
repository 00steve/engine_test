#ifndef ACTOR_H
#define ACTOR_H

#include "../core/prop.h"
#include "../physics/physical_single_body.h"
#include "../core/controllable.h"

class actor : public physicalSingleBody,public controllable{

protected:

	/**\brief a reference to the prop that the actor uses using the
	primary something something and stuff, fuck.*/
	prop* primaryProp;
	/**\brief this is the other one.*/
	prop* secondaryProp;
	/**\brief used to track the rotation of the actor.

	This assumes that there is an "up" vector on the z axis. So the
	x axis is used to look up and down, and the y axis is used to look
	left and right.*/
	double3 lookRotation;
	/**\brief store a list of nodes that can be interacted with. This
	list should be cleared when something is dissasociated from
	this object.*/
	list<node*> interactiveNodes;
	/**\brief store the node that is the closest node that can be
	interacted with.*/
	node* closestInteractiveNode;

	/**\brief if any of the referenced objects are deleted, remove it from
	the list of objects.*/
	void onReferenceRemoval(node* oldRef){
		if(interactiveNodes.remove((physical*)oldRef)){
			if(closestInteractiveNode == oldRef){
				closestInteractiveNode = NULL;
			}
		}
		node::onReferenceRemoval(oldRef);
	}

	/**\brief determines the closest interactive node and stores it
	in the closestInteractiveNode variable.*/
	bool setClosestInteractiveNode(){
		if(!interactiveNodes.getCount()){
			closestInteractiveNode = NULL;
			return false;
		}
		double minDistance = 99999999;
		double tD;
		for(int i=0;i<interactiveNodes.getCount();i++){
			tD = (position-((physical*)interactiveNodes[i])->getPosition()).length();
			if(minDistance<tD) continue;
			minDistance = tD;
			closestInteractiveNode = interactiveNodes[i];
		}
		return true;
	}

	/**\brief sends the MESSAGE_PRIMARY_INTERACT message to the node
	currently set as the closestInteractiveNode by the setClosestInteractiveNode()
	function that is called every time the actor is updated.*/
	bool primaryInteract(){
		if(!closestInteractiveNode) return false;
		sendMessage(closestInteractiveNode,MESSAGE_PRIMARY_INTERACT,(void*)this);
		return true;
	}

	/**\brief sends the MESSAGE_SECONDARY_INTERACT message to the node
	currently set as the closestInteractiveNode by the setClosestInteractiveNode()
	function that is called every time the actor is updated.*/
	bool secondaryInteract(){
		if(!closestInteractiveNode) return false;
		sendMessage(closestInteractiveNode,MESSAGE_SECONDARY_INTERACT,(void*)this);
		return true;
	}

public:

	actor() :
				lookRotation(double3(0,0,0))
			{
	}

	actor(varMap settings)
			{

	}

	~actor(){
		#if DEBUG_DELETES
			cout << " actor >";
		#endif
	}

	virtual void update(){
		setClosestInteractiveNode();
		physicalSingleBody::update();
		node::update();
	}

	virtual void primaryAction(){}

	virtual void secondaryAction(){}

	virtual bool setPrimaryProp(prop* newProp){
		primaryProp = newProp;
		primaryProp->setActorPosition(&position);
		primaryProp->setActorLookRotation(&lookRotation);
		if(control) newProp->setController(control);
		addDrawable(newProp);
		return false;
	}

	virtual bool setSecondaryProp(prop* newProp){
		return false;
	}

	virtual bool handleMessage(node_message* m){
		switch(m->code){
		case MESSAGE_CAN_INTERACT:
			startUsing((node*)m->sender);
			interactiveNodes.push((node*)m->sender);
			return true;
		case MESSAGE_CANT_INTERACT:
			stopUsing((node*)m->sender);
			interactiveNodes.remove((node*)m->sender);
			return true;
		/*if the pickup message is being sent, it is because
		a pickup is sending the message, along with the prop
		that should be picked up.*/
		case MESSAGE_PICKUP:
			setPrimaryProp((prop*)m->data);
			return true;
		}

		return physical::handleMessage(m);
	}

};

#endif // ACTOR_H
