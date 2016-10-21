#ifndef ODE_FORCE_H
#define ODE_FORCE_H

#include "node.h"


/*the force class contains an object that lasts for a
certain amount of time with a radius of affection, and
a falloff amount and an actual force. Any of these
variables can change over time or not. The generic
force class is constant and never goes away.*/

class force {
public:


    /*called to apply force to any bodies that
    should be effected by the force.*/
    virtual void applyForce(dBodyID body) = 0;


};






#endif // FORCE_H
