#ifndef NODE_H
#define NODE_H

#include "list.h"
#include "ordered_list.h"
#include "input.h"
#include "node_message.h"
#include "../graphics/window_properties.h"
#include "string_util.h"
#include "../ui/induicontextmenu.h"


/**\brief the most basic building block of this game engine.

The node has basic and almost universally needed functions that should
be inherited by any objects that exist in the game engine. This class
takes care of garbage collection, naming of individual objects,
storing of heirarchial information, etc.**/
class node {
private:

	node* parent;
	list<node*> children;
	orderedList<node_message*> unhandled_messages;
	node_message* last_message;

	/** \store the number of nodes that are referencing this
	node. Every time an node starts using another node for
	any reason, it should call the using function and pass the
	node being used as the parameter. When an node is done
	using another node, it should call the notUsing function
	to decrement the other node's referenced count**/
	list<node*> references;
	list<node*> dependents;
	//unsigned int dependent_count;
	/*if the keep alive flag is set to true, the node should not
	be deleted even if it has no references.*/
	bool keep_alive;

	/*can be set by any children classes, but by default is
	set to the node id. This should not be used to reference
	a node for obvious reasons. If you don't know what the main
	reason is, it is because if you make, I don't know, like 200
	nodes, then you make one called 'node201' and you have
	an object 'node1398' that references 'node201' and tells it
	to do something, but then later on you add some new nodes of
	some type before you create 'node201', then 'node1398' will
	be referencing the wrong node. Yes, that was a long explination,
	just I guess don't reference things unless you specifically
	know the name, otherwise your shit is probably going to die
	a horrible firey screamingly painful death.*/
	string *name;

	/*store a list of pointers to every node that currently exists.
	This list is used to run through to check if the node should be
	killed or not.*/
	static list<node*> global_list;

	/*store a static list of ids, so only one node ever has each id
	and when any node is created, the id is moved to the next node.*/
	unsigned long id;
	static unsigned long next_id;

	/*keep a list of ids of fallen (dead) objects so that they can be
	reused.*/
	static list<unsigned long> available_ids;

	/*if the node is braindead, it should not be updated, but it should
	still be drawn. It will only still be drawn if it is stll enabled.*/
	bool should_update;

	/*length of seconds between updates of all nodes, should be synchronized
	so this is available for nodes to use via the tick() function*/
	static double t_tick;

    /** \brief private initialization of the node class. This handles
	internal housekeeping, such as setting the unique node id.
     * \return null
     */
	void private_init(){
		/*set the node id. If there are any available ids, use one
		of them, otherwise grab a new one.*/
		if(available_ids.getCount()){
			id = available_ids.pop();
		} else {
			id = node::next_id++;
		}
		name = new string();
		/*add the node to the global list of nodes in the system*/
		global_list.push(this);
	}

	/** \brief call this when this node is using another node.

	It adds the other node to the list of references, which is a list
	of pointers to the nodes that this node is referencing.**/
	void start_using(node* newNode) {
		#if DEBUG_ASSOCIATIONS
			cout << "[ASSOCIATION]" << *this->name << " now references " << newNode->getName() << endl;
		#endif
        references.push(newNode);
        newNode->dependents.push(this);
	}

	/**\brief call this when this node is done referencing another node.

	If the other node is no longer being referenced by any nodes, it should kill
	itself, because nobody loves it.**/
	void stop_using(node* oldNode) {
		#if DEBUG_ASSOCIATIONS
			cout << "[DISASSOCIATION] " << *this->name << " no longer references " << oldNode->getName() << endl;
		#endif
		references.remove(oldNode);
		onReferenceRemoval(oldNode);
		oldNode->dependents.remove(this);
	}

protected:

	/*accessable list of any properties that might be need*/
	static window_properties windowProperties;

	virtual void onStartUpdating(){}
	virtual void onStopUpdating(){}

	/**\brief the default node reference removal hander.

	Removes any messages that reference the deleted node.**/
	virtual void onReferenceRemoval(node* oldRef){
		for(int i=0;i<unhandled_messages.getCount();i++){
			if(unhandled_messages[i]->sender == oldRef){
				//unhandled_messages.cut(i);
				cout << "found that sumnabitch sent a message before dying!\n";
			}
		}
	}

	/**\brief stores the node in the node directory with the given name.

	If the name already exists, the function returns false because each
	name needs to be unique**/
	bool setName(string *name){
		this->name = name;
		return true;
	}

	virtual bool appendToContextMenu(IndUICONTEXTMENU* menu){
		return false;
	}

public:
	static node* root;

	/**\brief the only way to construct a new node is to give it a
	parent node.

	If there is no parent, null should be passed in.**/
	node() :
			parent(NULL),
			children(list<node*>()),
			unhandled_messages(orderedList<node_message*>()),
			last_message(NULL),
			references(list<node*>()),
			dependents(list<node*>()),
			name(NULL),
			should_update(true)
			{
		private_init();
	}

	node(node* parent) :
			parent(parent),
			children(list<node*>()),
			unhandled_messages(orderedList<node_message*>()),
			last_message(NULL),
			references(list<node*>()),
			dependents(list<node*>()),
			//dependent_count(0),
			name(NULL),
			should_update(true)
			{
		private_init();
	}

	/**\brief the node destructor.

	This kills any references that this node may be making to other nodes
	and does other things to deallocate the memory it is using.

	It will be automatically called last when any inheriting class has
	the delete method called on it.**/
	virtual ~node() {
		#if DEBUG_DELETES
			cout << " node \"" << (name ? *name : "[node]")  << "\" DELETED\n";
		#endif
		dissassociate(this);
		if(this->parent) {
			this->parent->removeChild(this);
		}
		available_ids.push(this->id);
		delete last_message;
	}

	/**\brief create the default node update function.

	Pretty much anything that wants to consider itself a node should be
	updated (in whatever frequency the programmer wants), but it should
	be updated which is the important part of the statement, therefore,
	henceforth, every node shall have an update statement.

	Any child who wishes to override this function, should still probably
	call node::update() either before or after it has done its business.
	**/
	virtual void update(){
		if(should_update){
			for(int i=0 ; i<children.getCount(); i++){
				if(!children[i]->should_update) continue;
				children[i]->update();
			}
			handleMessages();
		}
	}

	/**\brief sets a reference to the parent node of the current node.**/
	node* setParent(node* parent) {
		this->parent = parent;
		return parent;
	}

	/**\brief adds a child to the children list.

	This node also sets itself as the new childs parent using the child's setParent
	function. If the new child already has a parent set, the old parent has the
	removeChild() function called to remove the child from itself.**/
	node* addChild(node* child) {
		if(child->parent) child->parent->removeChild(child);
		child->setParent((node*)this);
		return children.push(child);
	}

	/**\brief removes a child from the parent. If the child does not
	exist, false is returned.**/
	bool removeChild(node* child) {
		child->setParent(NULL);
		//stop_using(child);
		return children.remove(child);
	}

	/**\brief gets the reference to a child that the node contains.**/
	node* getChild(int index) {
		if(index < children.getCount()) return children[index];
		return NULL;
	}

	/**\brief gets the reference to the list that contains all of the
	node's children.**/
	list<node*>* getChildren() {
		return &children;
	}

	/**\brief gets the parent of this node. If a parent is not set, it
	returns NULL.**/
	node* getParent(){
		return this->parent;
	}

	/**\brief gets the distinct ID of the node.

	IDs are recycled. If a node is deleted, its ID is moved to a list
	of available IDs.**/
	unsigned long getId(){return id;}

	/**\brief gets the (should be) distinct name of the node.**/
	string getName(){return *name;}

	/**\brief gets the string representation of the specific
	node implimentation.

	This should be overridden by any inheriting classes.**/
	virtual string getClass(){return "node"; }

	/**\brief tells the node that the code in the update() function
	should begin running.**/
	void startUpdating(){
		should_update = true;
		onStartUpdating();
	}

	/**\brief tells the node that the code in the update() function
	should stop running.**/
	void stopUpdating(){
		should_update = false;
		onStopUpdating();
	}

	/**\brief tell this node that another node is now referencing it.

	If another node is using this node, the other node should call this
	function on THIS node to tell THIS node that it is being used by
	something. When the other node is done using THIS node, it should
	call the stopUsing function to let this node know that it is not
	needed anymore.**/
	void startUsing(node *other){
		start_using(other);
	}

	/**\brief tell this node that a different node is done referencing it**/
	void stopUsing(node *other){
		stop_using(other);
	}

	/**\brief sends a message to any other node.

	The message should use a defined MESSAGE_ compiler variable. The message is
	immediately sent to any other node via the recieveMessage() function where it
	is added to the other node's queue of messages to go through when it is the
	other node's turn to update.**/
	virtual bool sendMessage(node* recipient,unsigned int message_code,void* data){
		if(!recipient){
			cout << "{AN INVALID RECIPIENT WAS SET, message could not be sent!}\n";
			return false;
		}
		node_message* m = new node_message();
		m->sender = this;
		m->code = message_code;
		m->data = data;
		recipient->recieveMessage(m);
		return true;
	}

	/**\brief adds a message to the queue of messages to read when it is this nodes
	turn to update**/
	virtual void recieveMessage(node_message* m){
		unhandled_messages.push(m);
	}

	/**\brief the default node function for handling each message that the
	node has recieved.

	If an inheriting node has any specific functionality
	associated with recieving messages, it should override this implimentation.

	The function returns FALSE because it doesn't actually handle messages in
	any way.**/
	virtual bool handleMessage(node_message* m){
		return false;
	}

	/**\brief loops through all of the messages that are queued up in the node and
	calls the handleMessage() function for each one.

	If the function is able to successfully loop through all of the messages, it will
	return true, otherwise the program will probably crash because something crazy
	happened.**/
	virtual bool handleMessages(){
		while(unhandled_messages.getCount()){
			last_message = unhandled_messages.unshift();
			#if DEBUG_MESSAGES
				if(!handleMessage(last_message)){
					cout << "[Message with id " << last_message->code << " could not be handled]\n";
				}
			#else
				handleMessage(last_message);
			#endif
		}
		return true;
	}

	virtual string type(){
		return "node";
	}

	/**\brief deletes the node.

	This causes a cascading effect of all of this node's destructors
	to call, eventually leading to nodes ~node() destructor, which
	removes any references to and from this node so nothing tries to
	reference it after it stops existing.**/
	virtual void kill() {
		delete this;
	}

	virtual IndUICONTEXTMENU* getContextMenu(){
		IndUICONTEXTMENU* menu = new IndUICONTEXTMENU(name);
		appendToContextMenu(menu);
		if(menu->GetWidgetCount() > 0) return menu;
		//delete menu;
		return NULL;
	}

	/**\brief removes the node from any associations it has with other nodes.

	If the node has any dependents, it should tell the dependent node
	to stop using it, and it should stop using any nodes that it is
	referencing.**/
	static bool dissassociate(node* oldNode){
		while(oldNode->dependents.getCount()){
			#if DEBUG_ASSOCIATIONS
				cout << " - remove reference from dependent : " << oldNode->dependents[0]->getName() << endl;
			#endif
			(oldNode->dependents[0])->stopUsing(oldNode);
		}
		while(oldNode->references.getCount()){
			#if DEBUG_ASSOCIATIONS
				cout << " - remove reference to : " << oldNode->references[0]->getName() << endl;
			#endif
			oldNode->stopUsing(oldNode->references[0]);
		}
		return true;
	}

	/*get the total number of nodes in the application.*/
	static int globalCount() {
		return global_list.getCount();
	}

	/**\brief get the amount of time (milliseconds) that has ellapsed since
	the last time the engine was updated.**/
	static double tick(){
		return t_tick;
	}

	/**\brief get the root node of the engine. This is the node that is controlling
	all of the other nodes and whatnot.**/
	static node* getRoot(){
		return root;
	}

	/**\brief returns the first node found with a given name.**/
	static node* getByName(string name){
		//loop through all nodes to look for one
		for(int i=0;i<global_list.getCount();i++){
			if(global_list[i]->getName() != name) continue;
			return global_list[i];
		}
		return NULL;
	}


};

window_properties node::windowProperties = window_properties();
list<node*> node::global_list = list<node*>();
node* node::root = NULL;
unsigned long node::next_id = 0;
list<unsigned long> node::available_ids = list<unsigned long>();
double node::t_tick = .01;
#endif // NODE_H
