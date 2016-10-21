#ifndef ENGINE_PROJECTILE_WEAPON_H
#define ENGINE_PROJECTILE_WEAPON_H


#include "../core/prop.h"

/**\brief a generic class that handles launching some other node
at some speed in some direction.

Much of the same code will be written over and over again when
making rocket launchers, ak-47s, gattling guns, and even arms
that throw things. They should all be able to inherit this class
to do most of the work without having to rewrite code over and
over again.**/
class projectileWeapon : public prop{
private:

	int roundsLeft;
	int roundsInClip;
	double reloadTimeLeft;
	double fireTimeLeft;

	bool reloading;
	bool firing;
	bool triggerPulled;

protected:

	bool fullyAuto;
	/*the number of rounds that the weapon (or more accurately)
	the player can hold.*/
	int maxTotalRounds;
	/*the number of projectiles that each clip can hold, the
	number of projectiles that can be fired before a reload
	needs to happen.*/
	int roundsPerClip;
	/*time it takes to replace a clip after it has been emptied*/
	double reloadTime;
	/*the time it takes to fire a single projectile*/
	double fireTime;
	/*measured in angle of randomness of projection*/
	double accuracy;
	/*the meters per second that the projectile leaves the weapon*/
	double velocity;
	/*the amount accuracy decreases as velocity increases*/
	double degradeAccurcayByVelocity;

	virtual void insertProjectile()=0;

public:

	projectileWeapon() :
			roundsLeft(0),
			roundsInClip(0),
			reloadTimeLeft(0),
			fireTimeLeft(0),
			reloading(false),
			firing(false),
			triggerPulled(false)
			{
	}

	~projectileWeapon(){
		#if DEBUG_DELETES
			cout << " projectile weapon >";
		#endif
	}

	/**\brief make the weapon fire.

	It will check to make sure that it is not reloading, or already
	firing, and that the projectile_weapon node has a non-null parent
	that the projectile_weapon can add a projectile to. If everything
	is good to go, it will call the insertProjectile() function which
	is inherited by any classes using this class. They can determine
	how or what type of projectile they want to insert into the world.**/
	void fire(){
		if(reloading || firing || !getParent()) return;
		if(triggerPulled && !fullyAuto) return;
		firing = true;
		fireTimeLeft = fireTime;
		cout << "bang!\n";
		insertProjectile();
		--roundsInClip;
		if(!roundsInClip){
			reload();
		}
	}

	/**\brief sets the amount of ammo in the weapon to a value
	equal to the maxTotalRounds property.**/
	void fillAmmo(){
		roundsLeft = maxTotalRounds;
	}

	void reload(){
		reloadTimeLeft = reloadTime;
		roundsInClip = roundsPerClip;
		if(roundsInClip > roundsLeft) roundsInClip = roundsLeft;
		roundsLeft -= roundsInClip;
		reloading = true;
	}

	/**\brief handles the basic functions of the projectile weapon,
	such as firing, reloading, etc.

	Any inheriting class should call projectile_weapon::update() so
	it doesn't have to write all of its same logic all over again.**/
	void update(){
		if(reloading){
			reloadTimeLeft -= node::tick();
			if(reloadTimeLeft <= 0){
				cout << "done reloading\n";
				reloading = false;
			}
		}
		if(firing){
			fireTimeLeft -= node::tick();
			if(fireTimeLeft <=0){
				cout << "done firing\n";
				firing = false;
			}
		}
		if(control){
			if(control->primary_action()){
				fire();
				triggerPulled = true;
			} else {
				triggerPulled = false;
			}
		}
		handleMessages();
	}


};





#endif // ENGINE_PROJECTILE_WEAPON_H
