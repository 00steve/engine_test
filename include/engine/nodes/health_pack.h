#ifndef HEALTHPACK_H
#define HEALTHPACK_H

#include "../core/prop.h"

class healthPack : public prop {

public:

	healthPack(){


	}

	~healthPack(){
        #if DEBUG_DELETES
        cout << " heatlh pack -> ";
        #endif
	}


	bool handleMessage(node_message *m){
		switch(m->code){
		case MESSAGE_PRIMARY_INTERACT:
			cout << "pickup health pack\n";
			delete this;
			return true;
		}
		return prop::handle_message(m);
	}


};




#endif
