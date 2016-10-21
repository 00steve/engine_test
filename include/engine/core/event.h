#ifndef EVENT_H
#define EVENT_H

#define EVENT_ON_DONE 10
#define EVENT_ON_SELECT 100
#define EVENT_ON_CONTEXT 101
#define EVENT_ON_WINDOW_FOCUS_LOST 200

#include "list.h"
#include "condition.h"
#include "node.h"
#include "action.h"

class event : public node{
private:
	//static events
	int eventType;


	condition* trueCondition;
	action* trueAction;

public:

	event(){
	}

	event(int eventType) :
		eventType(eventType),
		trueCondition(NULL),
		trueAction(NULL) {
	}

	~event(){
		cout << "kill event\n";
	}

	void fireWhen(condition* newCondition){
		trueCondition = newCondition;
	}

	void performAction(action* newAction){
		this->trueAction = newAction;
	}

	void update(){
		if(trueCondition && trueCondition->isMet() && trueAction){
			trueAction->run();
		}
	}

};





#endif // EVENT_H
