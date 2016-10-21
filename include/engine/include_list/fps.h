#ifndef INCLUDE_LIST_FPS_H
#define INCLUDE_LIST_FPS_H

/*this is a file that breaks out the loading of anything needed by the
fps engine control into a seperate file so it is easier to manage what
needs to be loaded, or so that engine controls like the fps editor can
reference this same file and when a new object type is added, it will
be referenced by anything that needs to reference things that the fps
needs to reference.*/





/*nodes, different types of objects that have been
programmed to be used in this engine control. Include whichever ones
are needed. (All are included by default)*/

#include "../nodes/decoration.h"
#include "../nodes/skybox.h"
#include "../nodes/crate.h"
#include "../nodes/infinite_plane.h"
#include "../nodes/ground.h"
#include "../ai/n_path.h"
#include "../nodes/spawner.h"
#include "../nodes/marker.h"
#include "../nodes/fps_player.h"
#include "../nodes/barrel.h"

/*include props used by actors
, guns, ammo, health, anything an actor can use*/
//#include "../nodes/health_pack.h"
#include "../nodes/weapons/ak47.h"
#include "../nodes/weapons/glock19.h"



/*include interactive items or things that allow
the player to interact with the world*/
#include "../nodes/pickup.h"

/*include particles, shit that makes shit look not like
shit and stuff.*/
#include "../nodes/fire.h"


/*include vehicles.*/
#include "../physics/portal.h"
#include "../core/vehicle.h"

#endif // INCLUDE_LIST_FPS_H
