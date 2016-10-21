#include "nodemessage.h"

NodeMessage::NodeMessage():
		code(0)
	{
}

NodeMessage::NodeMessage(Node* receiver,unsigned int code,void* data):
		receiver(receiver),
		code(code),
		data(data)
	{
}
