#ifndef GERNADE_LAUNCHER_H
#define GERNADE_LAUNCHER_H

#include "../../core/projectile_weapon.h"
#include "../projectiles/collision_gernade.h"

/**\brief uses the projectileWeapon class to build a
gernade launcher, with all projectileWeapon properties
set to a setting resembling what I think a gernade
launcher should be like.**/
class gernadeLauncher : public projectileWeapon{
protected:
	void insertProjectile(){
		sendMessage(getParent()->getParent(),MESSAGE_ADD_DRAWABLE,(void*)(new collisionGernade(getPosition(),getActorLookVector()*velocity)));
	}


public:

	gernadeLauncher(){
		fullyAuto = false;
		maxTotalRounds = 420;
		roundsPerClip = 1;
		reloadTime = 1;
		fireTime = .4;	//300 rounds per minute
		accuracy = 1.434;
		velocity = 215;
		degradeAccurcayByVelocity = 1;

		fillAmmo();
		reload();
	}

};
#endif // GERNADE_LAUNCHER_H
