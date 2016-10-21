#ifndef PICKUP_H
#define PICKUP_H

#include "../physics/collision_trigger.h"

class pickup : public collision_trigger{
private:

	prop* item;

public:

	pickup(varMap settings) :
			collision_trigger(settings)
			{

        string type = setup::getString(settings,"spawn","");
		if(type == "health-pack") {
			//item = new healthPack();
		} else if(type == "ak47"){
			item = new ak47();
		} else {
			item = NULL;
		}

        setName(new string("pickup_" + numberToString<unsigned long>(getId())));

	}


	~pickup(){
		#if DEBUG_DELETES
			cout << " pickup >";
		#endif
	}


	bool handleMessage(node_message *msg){
		switch(msg->code){
		case MESSAGE_PRIMARY_INTERACT:
			sendMessage(msg->sender,MESSAGE_PICKUP,(void*)item);
			delete this;
			return true;
		}
		return collision_trigger::handleMessage(msg);
	}

	/**\brief sends message to other node that the pickup can be
	used.**/
    void startTrigger(node *n){
    	startUsing(n);
		sendMessage(n,MESSAGE_CAN_INTERACT,this);
    }

	/**\brief sends message to the other node that the pickup can
	no longer be used.**/
    void endTrigger(node *n){
    	stopUsing(n);
		sendMessage(n,MESSAGE_CANT_INTERACT,this);
    }


};






#endif // PICKUKP_H
