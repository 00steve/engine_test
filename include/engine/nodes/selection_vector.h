#ifndef SELECTION_VECTOR_H
#define SELECTION_VECTOR_H

#include "../core/physical.h"
#include "../core/depth_click.h"
#include "../core/controller.h"
#include "../core/map.h"
#include <limits>


class selectionVector : public physical{
private:

	depth_click 	*data;
	dGeomID 		vec;
	double			length;
	controller		*control;
	list<physical*>	selected;
	bool 			geomActive;
	int 			state;
	double3			lastCollisionPoint;
	node 			*deferHandler;

	bool selectNode(physical *newNode){
		if(newNode->isSelectable()){
			cout << " - selected object " << newNode->getName() << endl;
			selected.push(newNode);
			sendMessage(newNode,MESSAGE_SELECTED,(void*)NULL);
			startUsing(newNode);
			return true;
		} else {
			cout << " - clicked object " << newNode->getName() << " - Not Selectable\n";
			return false;
		}
	}

	bool deselectNode(physical *oldNode){
		if(selected.exists(oldNode)){
			selected.cutLastSearch();
			sendMessage(oldNode,MESSAGE_DESELECTED,(void*)NULL);
			stopUsing(oldNode);
			return true;
		}
		return false;
	}

	void deselectAllNodes(){
		physical *tmp;
		while((tmp = selected.pop())){
			sendMessage(tmp,MESSAGE_DESELECTED,(void*)NULL);
			stopUsing(tmp);
		}
	}

protected:

	/*if any of the referenced objects are deleted, remove it from
	the list of objects.*/
	void onReferenceRemoval(node* oldRef){
		cout << "[selection vector] - remove reference to " << oldRef->getName() << "\n";
		selected.remove((physical*)oldRef);
	}


public:

	selectionVector(depth_click *data,controller *control) :
				data(data),
				length(1000),
				control(control),
				selected(list<physical*>()),
				geomActive(false),
				state(0),
				deferHandler(NULL)
			{
		setVirtual(true);
        //setStatic(true);
		if(data){
			ode::newStaticRay(vec,data->origin,data->direction,length,this);
		} else {
			vec = dCreateRay (ode::getSpaceId(), length);
			dGeomSetData(vec,(void*)this);
		}
		dGeomRaySetParams( vec, false, false );
		dGeomRaySetClosestHit(vec,true);
		dGeomDisable(vec);
		setName(new string("selection_vector_" + numberToString<unsigned long>(getId())));
	}

	~selectionVector() {
		#if DEBUG_DELETES
			cout << " selection vector >";
		#endif
	}

	void setDepthClick(depth_click *newData){
		data = newData;
	}

	void update(){
		switch(state){
		case 0:
			if(data && data->active){
				geomActive = true;
				dGeomEnable(vec);
				dGeomRaySet(vec,data->origin.x, data->origin.y,	data->origin.z, data->direction.x, data->direction.y, data->direction.z);
				state = 1;
			}
			/*break no matter what so that the physics engine can run at least one time
			and detect if there have been any collisions.*/
			break;
		case 1:
			if(collisions.getCount()){
				collision *c = collisions[dGeomRayGetClosestHit(vec)-1];
				/*if primary_modify is true (ctrl key), add the object
				from the list of selected objects if it is or is not already selected.
				IF secondary_modify is true (shift key), remove the object from the
				list of objects if it is in the list of objects. If neither modifier
				is being pressed, clear the list of selected nodes and select the
				new node.*/
				lastCollisionPoint = double3(c->getContactPoint(0).x,c->getContactPoint(0).y,c->getContactPoint(0).z);
				//cout << "( " << lastCollisionPoint.x << " , " << lastCollisionPoint.y << " , " << lastCollisionPoint.z << " )\n";
				if(control->primary_modify()) {
					if(c->otherPhysical()->isSelectable()){
						cout << "add selected object to selection list\n";
						selectNode(c->otherPhysical());
					} else {
						if(deferHandler){
							sendMessage(deferHandler, MESSAGE_TAKE_CONTROL, (void*)&lastCollisionPoint);
						}
					}
				} else if(control->secondary_modify()) {
					cout << "remove selected object from selection list\n";
					deselectNode(c->otherPhysical());
				} else {
					if(c->otherPhysical()->isSelectable()){
						cout << "replace list with single selected object\n";
						deselectAllNodes();
						selectNode(c->otherPhysical());
					} else {
						if(deferHandler){
							sendMessage(deferHandler, MESSAGE_TAKE_CONTROL, (void*)NULL);
						}
					}
				}

				if(control->secondary_action()){
					cout << "display context menu\n";
					//sendMessage()
				}
				/*either way clear all of the collisions and disable the geom*/
				//delete c;
				collisions.clear();
				dGeomDisable(vec);
			} else {
				/*if there were no collisions, only deselect everything
				if ctrl key is NOT being pressed. If it is, don't do anything*/
				if(!control->primary_modify()){
					deselectAllNodes();
				}
			}

			state = 10;
			break;

		case 10:
			if(data && !data->active){
				state = 0;
			}
			break;
		}

		handleMessages();
	}

	void draw(){
		/*
		if(data){
			double3 xp = data->origin+(data->direction*10);
			double3 nd = xp + data->direction * 1000;
			if(data->active){
				glLineWidth(4);
				glBegin(GL_LINES);
					glColor3f(1,.33f,.33f);
					glVertex3d(xp.x-5,xp.y,xp.z);
					glVertex3d(xp.x+5,xp.y,xp.z);
					glColor3f(.33f,1,.33f);
					glVertex3d(xp.x,xp.y-5,xp.z);
					glVertex3d(xp.x,xp.y+5,xp.z);
					glColor3f(.33f,.33f,1);
					glVertex3d(xp.x,xp.y,xp.z-5);
					glVertex3d(xp.x,xp.y,xp.z+5);

				glEnd();
			}
			glLineWidth(4);
			glBegin(GL_LINES);
				glColor3f(1,0,0);
				glVertex3d(xp.x,xp.y,xp.z);
				glVertex3d(nd.x,nd.y,nd.z);
			glEnd();
		}*/
	}

	bool setEnabled(const bool enabled){
		return true;
	}

	void deferToHandler(node* deferrer){
		deferHandler = deferrer;
	}

	int getSelectedPhysicalCount(){ return selected.getCount(); }

	list<physical*> *selectedPhysicalsRef() { return &selected; }

	double3 getLastCollisionPoint(){ return lastCollisionPoint; }

	depth_click *getClickData(){ return data; }

	void addForce(double3 force){}

	double3 getPosition(){return lastCollisionPoint;}

	bool isEnabled(){ return data && data->active; }
};

#endif // SELECTION_VECTOR_H
