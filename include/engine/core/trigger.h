#ifndef TDENGINE_TRIGGER_H
#define TDENGINE_TRIGGER_H

/*a trigger is an node that triggers an
event which sends info like the node that
triggered the trigger and the trigger that sent
the event.


Being expanded to for generalization, a trigger could
be two objects colliding, a certian value range being
left, any number of things. It will be left completely
generic so that triggers can function however they
want as long as they follow the basic ideas below...
- an object or property changes
- an event is sent from one object to one or more other objects
- will be updated every game loop to check if something should happen
*/




class trigger {
public:
    //what should happen when the event is triggered
    //this should be left up to individual implimentations
    virtual void startTrigger(node *n) = 0;

    //what should happen when the event is untriggered (optional)
    //this should be left up to individual implimentations
    virtual void endTrigger(node *n) = 0;

    //check for nodes entering or leaving
    //the trigger area
    virtual void update() = 0;

};

#endif

