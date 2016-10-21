#ifndef NODE_MESSAGE_H
#define NODE_MESSAGE_H

#define MESSAGE_UNDEFINED 			0
/*node level functions 10-29*/
#define MESSAGE_FINISHED 			10
#define MESSAGE_STARTED 			11

/*drawable level functions 30-49*/
#define MESSAGE_ADD_DRAWABLE 		30
#define MESSAGE_ADD_PHYSICAL 		31

#define MESSAGE_REMOVE_DRAWABLE 	40
#define MESSAGE_REMOVE_PHYSICAL 	41

#define MESSAGE_SET_CAMERA 				50 //mostly used by views, but it's here and stuff for things
#define MESSAGE_SET_TARGET 				51 //mostly used by cameras that point at things, but whatevs
#define MESSAGE_SET_POSITION_REFERENCE 	52
#define MESSAGE_SET_ROTATION_REFERENCE 	53
#define MESSAGE_ADD_VIEW                54//used by the window object to set a view to use
#define MESSAGE_ENABLE_VIEW             55
#define MESSAGE_DISABLE_VIEW            56
#define MESSAGE_SET_RENDER_SUBJECT      57//used by views to set what node should be drawn

#define MESSAGE_SELECTED 			110
#define MESSAGE_DESELECTED 			111

#define MESSAGE_TAKE_CONTROL 		175
#define MESSAGE_GIVE_CONTROL 		176
#define MESSAGE_PICKUP				179
#define MESSAGE_DROP				180
#define MESSAGE_CAN_INTERACT		181
#define MESSAGE_CANT_INTERACT		182

#define MESSAGE_PRIMARY_INTERACT	290
#define MESSAGE_SECONDARY_INTERACT	291
#define MESSAGE_TERTIARY_INTERACT	292

/*engine commands, the engine sets a reference
to itself as node::root, so any engine functions
can be called from any other node.*/
#define MESSAGE_ENGINE_SET_STATE 				300
#define MESSAGE_ENGINE_EXIT 					350
#define MESSAGE_ENGINE_SET_TEMP_CONTROLLER 		351
#define MESSAGE_ENGINE_UNSET_TEMP_CONTROLLER 	352
#define MESSAGE_ENGINE_SET_VIEW_CAMERA 			353

/*collision and damage stuff*/
#define MESSAGE_PROJECTILE_IMPACT	 			600
#define MESSAGE_BLUNT_IMPACT					601

#define MESSAGE_SET_ENABLED 		710
#define MESSAGE_SET_DISABLED 		711

/*physics engine stuff*/
#define MESSAGE_SET_VELOCITY 		920
#define MESSAGE_SET_ORIENTATION 	921
#define MESSAGE_SET_ROTATION 		922
#define MESSAGE_SET_PHYSICS_SPACE   923
#define MESSAGE_REGISTER_PHYSICS_OFFER      924
#define MESSAGE_REGISTER_PHYSICS_REQUEST    925 //should send a var map of physics objects to create
#define MESSAGE_REGISTER_PHYSICS_FINISHED   926 //should return a map that has corresponding physics objects

/*glfw stuff*/
#define MESSAGE_GET_GLFW_WINDOW		1000

class Node;

class NodeMessage{
public:

	NodeMessage();
	NodeMessage(Node* receiver,unsigned int code,void* data);

	Node* sender;
	Node* receiver;
	unsigned int code;
	void* data;

};


#endif // NODE_MESSAGE_H
