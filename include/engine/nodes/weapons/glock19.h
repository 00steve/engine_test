#ifndef GLOCK19_H
#define GLOCK19_H

#include "../../core/projectile_weapon.h"
#include "../projectiles/bullet9mm.h"

/**\brief creates the engine version of the glock 19 gun using the
projectileWeapon class.


**\ingroup weapons
**/
class glock19 : public projectileWeapon{
protected:

	void insertProjectile(){
		sendMessage(getParent()->getParent(),MESSAGE_ADD_DRAWABLE,(void*)(new bullet9mm(getPosition(),getActorLookVector()*velocity)));
	}

public:

	glock19(){
		fullyAuto = false;
		maxTotalRounds = 200;
		roundsPerClip = 15;
		reloadTime = 1;
		fireTime = .25;
		accuracy = 1.01;
		velocity = 375;
		degradeAccurcayByVelocity = 1;

		fillAmmo();
		reload();
	}
};


#endif // GLOCK19_H
