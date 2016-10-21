#ifndef SEND_MESSAGE_H
#define SEND_MESSAGE_H

#include "../action.h"

class send_message : public action{

	node* sender;
	node* reciever;
	unsigned int code;
	void* data;


public:


	send_message(node* sender,node* reciever,unsigned int code,void* data) :
		sender(sender),
		reciever(reciever),
		code(code),
		data(data){
	}

	void run(){
		if(sender){
			sender->sendMessage(reciever,code,data);
		}
	}

};

#endif // SEND_MESSAGE_H
