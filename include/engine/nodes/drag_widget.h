#ifndef DRAG_WIDGET_H
#define DRAG_WIDGET_H

#include "../core/physical.h"
#include "../core/controllable.h"
#include "../nodes/selection_vector.h"
#include <ode/objects.h>

/*the drag widget should be created whenever a physical
is selected. The physical that is selected is sent to the
dragWidget's constructor and stored as the "target". When
the selection vector*/





class dragWidget : public physical, controllable {
private:

	list<physical*> *targets;
	list<bool> targetsDisabled;
	physical *tmp_target;

	int lastTargetCount;
	physical *lastNode;

	double3 centerOfTargets;

	bool hasControl;

	double3 dragStart;
	double3 dragEnd;
	double3 dragOffset;
	double3 lastDragOffset;
	selectionVector *selection;
	double selectionDepth;

	depth_click 	*data;

	bool enablePhysicals;


	void calculateCenterOfTargets() {
		centerOfTargets = double3(0,0,0);
		int i = 0;
		while(i < targets->getCount()){
			centerOfTargets = (centerOfTargets*i + (*targets)[i]->getPosition()) / (i+1);
			++i;
		}
	}

	void enableTargets(){
		int i = targets->getCount();
		while( i --> 0 ){
			(*targets)[i]->setEnabled(true);
		}
	}

	void disableTargets(){

	}

public:
	dragWidget(list<physical*> *targets,controller *control) :
			controllable(control),
			targets(targets),
			lastTargetCount(0),
			lastNode(NULL),
			hasControl(false),
			dragOffset(double3(0,0,0)),
			selection(NULL),
			data(NULL),
			enablePhysicals(false)
			{
		calculateCenterOfTargets();
		setName(new string("drag_widget_" + numberToString<unsigned long>(getId())));
	}

	~dragWidget() {
		#if DEBUG_DELETES
			cout << " drag_widget >";
		#endif
	}


	void update() {
		handleMessages();
		collisions.clear();
		if(lastTargetCount != targets->getCount() || lastNode != targets->last()){
			lastTargetCount = targets->getCount();
			lastNode = targets->last();
			calculateCenterOfTargets();
		}

        if(hasControl){

			int i = targets->getCount();
			while( i --> 0 ){
				if((*targets)[i]->isEnabled())
					(*targets)[i]->addForce(double3(0,9.81,0));
			}

			data = selection->getClickData();
			dragEnd = data->origin + data->direction * selectionDepth;
			dragOffset = dragEnd-dragStart;
			dragStart = dragEnd;

			/*if dragoffset is > 0, it means that anything that is selected should be
			moved the amount of the offset and stuff, and shit... forrealz*/
			if(dragOffset.length()){
				int i = targets->getCount();
				while( i --> 0 ){
					(*targets)[i]->shiftPosition(dragOffset);
					(*targets)[i]->setEnabled(true);
				}
			}
			lastDragOffset = dragOffset;

			if(!control->primary_action()){
				/*return objects to have default dampening.*/
				if(lastDragOffset.length()){
					int i = targets->getCount();
					while( i --> 0 ){
						(*targets)[i]->setVelocity(lastDragOffset*100);
						(*targets)[i]->setEnabled(true);
					}
				}

				sendMessage(this,MESSAGE_GIVE_CONTROL,(void*)NULL);
			}
        }
	}

	bool setEnabled(const bool enabled){
		return true;
	}

	bool handleMessage(node_message* m) {
		int i;
        switch(m->code) {
		case MESSAGE_TAKE_CONTROL:
			hasControl = true;
			startUsing(m->sender);
			selection = (selectionVector*) m->sender;
			dragStart = selection->getLastCollisionPoint();
			data = selection->getClickData();
			selectionDepth = (data->origin - dragStart).length();
			//cout << "distance from dat drag : " << selectionDepth << endl;

            enableTargets();

			i = targets->getCount();
			while( i --> 0 ){
				(*targets)[i]->setKinematic();
			}

			//cout << "drag that shit\n";
			return true;
        case MESSAGE_GIVE_CONTROL:
            hasControl = false;
            stopUsing(m->sender);
            selection = NULL;
            //dragOffset = NULL;
            //cout << "draggin that shit got did\n";

			i = targets->getCount();
			while( i --> 0 ){
				(*targets)[i]->setDynamic();
			}

            return true;
        }
		return physical::handleMessage(m);
	}



	void draw() {
		glDisable(GL_TEXTURE_2D);
		glLineWidth(5);
		glBegin(GL_LINES);
			glColor3f(0,0,1);
			glVertex3d(centerOfTargets.x,centerOfTargets.y-10,centerOfTargets.z);
			glVertex3d(centerOfTargets.x,centerOfTargets.y+10,centerOfTargets.z);
			glColor3f(0,1,0);
			glVertex3d(centerOfTargets.x-10,centerOfTargets.y,centerOfTargets.z);
			glVertex3d(centerOfTargets.x+10,centerOfTargets.y,centerOfTargets.z);
			glColor3f(1,0,0);
			glVertex3d(centerOfTargets.x,centerOfTargets.y,centerOfTargets.z-10);
			glVertex3d(centerOfTargets.x,centerOfTargets.y,centerOfTargets.z+10);
        glEnd();
        glEnable(GL_TEXTURE_2D);

	}


	bool isSelectable() {
		return false;
	}

	void addForce(double3 force){}

	double3 getPosition(){ return centerOfTargets; }

	bool isEnabled(){ return hasControl; }
};





#endif // DRAG_WIDGET_H
