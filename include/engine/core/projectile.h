#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../core/physical.h"

class projectile : public physical{
private:

    /*mostly for testing, this gives a life to projectiles so
    that they don't keep using memory and collision processing
    cpu cycles forever.*/
    int ticks_left;

	/*store the group shared by all projectiles so they can't
	hit each other.*/
    static int projectile_group_id;

public:

    projectile() :
        ticks_left(100){
        	this->setGroup(projectile_group_id);
    }

    projectile(unsigned int ticks_left) :
        ticks_left(ticks_left){
    }

    ~projectile(){
        #if EVENT_OUTPUT
        cout << " projectile >";
        #endif
    }

    void update(){
        --ticks_left;
        if(!ticks_left) this->kill();
    }

    void addForce(double3 force){}

    float timeLeft(){ return (float)ticks_left * .01f; }
    bool ticksLeft(){ return ticks_left; }

};


int projectile::projectile_group_id = physical::getNextCollisionGroupId("projectiles");

#endif // PROJECTILE_H
