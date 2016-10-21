#ifndef AK47_H
#define AK47_H

#include "../../core/projectile_weapon.h"
#include "../projectiles/bullet762x39mm.h"

/**\brief uses the projectileWeapon class to build an
ak47, with all projectileWeapon properties set to a setting
resembling an ak47 as closely as possible.**/
class ak47 : public projectileWeapon{
protected:

	void insertProjectile(){
		sendMessage(getParent()->getParent(),MESSAGE_ADD_DRAWABLE,(void*)(new bullet762x39mm(getPosition(),getActorLookVector()*velocity)));
	}


public:

	ak47(){
		fullyAuto = true;
		maxTotalRounds = 420;
		roundsPerClip = 30;
		reloadTime = 1;
		fireTime = .2;	//300 rounds per minute
		accuracy = 1.04;
		velocity = 715;
		degradeAccurcayByVelocity = 1;

		fillAmmo();
		reload();
	}



};

#endif // AK47_H
