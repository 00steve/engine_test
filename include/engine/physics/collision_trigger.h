#ifndef COLLISION_TRIGGER_H
#define COLLISION_TRIGGER_H

#include "../nodes/marker.h"
#include "../core/trigger.h"

/**\brief the collision trigger is an inheritable class that keeps track
of objects that are intersecting a sphere.

It inherits the trigger class with two functions startTrigger and
endTrigger that are not implimented. startTrigger is called when
an object starts intersecting with the sphere and endTrigger is
called when an object stops intersecting with the sphere. Those
two functions need to be implimented by the class that inherits
from the collision_trigger class. **/
class collision_trigger : public trigger,public marker {
private:

    /**\brief store a list of entities that were not in the trigger
    area, but are now in the trigger area*/
    list<physical*> newEntitiesOn;
    /**\brief store a list of entities that are in the trigger area
    and have already had the onEnter function called for them*/
    list<physical*> currentEntitiesOn;
    /**\brief store a list of entities that currently think that they
    can interact with the collision trigger.*/
    list<physical*> entitiesOn;

protected:

	/**\brief if any of the referenced objects are deleted, remove it from
	the list of objects.*/
	void onReferenceRemoval(node* oldRef){
		entitiesOn.remove((physical*)oldRef);
		node::onReferenceRemoval(oldRef);
	}


public:
    collision_trigger(varMap settings) :
			marker(settings){
    }

    ~collision_trigger(){
        #if DEBUG_DELETES
        cout << " collision trigger >";
        #endif
    }

    /**\brief check for nodes entering or leaving the trigger area.

    The collision_trigger::update() function should be called by any
    inheriting classes to make sure the intersecting objects are
    updated.

    The function first clears the list of current entities
    (currentEntitiesOn) that are colliding with the collision trigger
    and the list of new entities (newEntitiesOn) that are colliding
    with the collision trigger. The function checks each of the collisions
    that are currently occurring. If the colliding entity is in the
    list of colliding entities that is maintained between update cycles,
    the entity is added to the currentEntitiesOn list, otherwise it is
    added to the newEntitiesOn list.

    The function next checks the current entity collisions (currentEntitiesOn)
    against the list of colliding entities that persists between update()
    calls. If a persisting entity collision is not found in
    (currentEnttiesOn), it is removed from the persisting entities
    list (entitiesOn) and a message is sent to the no-longer colliding
    entity to tell it that it is no longer colliding.
    **/
    void update(){
        currentEntitiesOn.clear();
        newEntitiesOn.clear();
        collision* cPtr;
        physical* ePtr;
        while((cPtr = physical::collisions.pop())){
            ePtr = cPtr->getCollider();
            if(!entitiesOn.exists(ePtr)){
                startTrigger(ePtr);
                newEntitiesOn.push(ePtr);
            } else {
                currentEntitiesOn.push(ePtr);
            }
        }

        //check to make sure that all of the collisions
        //stored in entitiesOn still exist.
        physical **nPtr;
        while((nPtr = entitiesOn.iterate())){
            if(currentEntitiesOn.exists(*nPtr))  continue;
            if(entitiesOn[entitiesOn.cursorIndex()])
				endTrigger(entitiesOn[entitiesOn.cursorIndex()]);
			entitiesOn.cut(entitiesOn.cursorIndex());
        }//end of looping through existing entities

        //add any entities being used to the list of entities
        //that are overlapping, that are new and stuff
        while(newEntitiesOn.getCount()){
            entitiesOn.push(newEntitiesOn.pop());
        }

		/*this goes last because the node::update function
		calls the recieveMessages function that may delete
		the object and we don't want the object to try and
		do anything after it has been deleted.*/
    	node::update();
    }//end of update()

};


#endif // COLLISION_TRIGGER_H
