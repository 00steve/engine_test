#ifndef SPAWNER_H
#define SPAWNER_H

#include "../nodes/marker.h"
#include "../graphics/gl_shape.h"
#include "../core/random.h"

template <typename nodeType>
class spawner : public marker{
private:

	varMap settings;

	bool use_total_spawns;
	bool use_period;

	/*counts either total spans or period, either way
	it counts down one tick on the counter every time
	the update is called. When the counter hits 0, a
	drawable is spawned and the counter resets at the
	iteration value.*/
	int counter;
	int interval;

	/*whenever the counter reaches 0, the spawns left
	variable counts down one. It stops spawning after
	the spawns_left has reaced 0. If the number of
	spawns is time based, it is generated based on the
	starting number of ticks and the tick interval
	between spawns.*/
	int spawns_left;
	mesh m;

	/*the amount that the velocity of the spawner at
	the creation of a spawnee is inherited by the
	spawnee. 0 is spawnee starts out with zero speed,
	1, is spawnee is created with 100% of the velocity
	of the spawner. negitive values shoot the spawnee
	out in the opposite direction of the spawner, etc.*/
	double velocity_multiplier;
	double3 spawn_velocity;

	/*the amount the velocity is randomized.*/
	double3 randomize_velocity;

protected:

	/**\brief an overridable function provied from the
	spawner parent class to allow any inheriting classes
	the chance to interact with what they spawn before their
	spawn is let loose into the world.

	The spawn is sent as a node, so any interaction should be
	through the sendMessage() function of the node() class,
	unless you're feeling like a hunk and want to chance casting
	the new node as something specific.

	Actually, I was lying. I forgot that this class was a
	template class so this function could dynamically decide
	which type of object it was dealing with. With that said,
	it does nothing by default, but can be overwritten by any
	inheriting classes and stuff can be done with it, whatever
	the template class type allows.**/
	virtual void onSpawn(nodeType *node){
	}

public:

	spawner(double3 position,double radius,int interval,int totalSpawns,double velocityMultiplier) :
				marker(position,radius),
				interval(interval),
				spawns_left(totalSpawns),
				velocity_multiplier(velocityMultiplier)
			{
		cout << "built a spawner and shit\n";
	}

	spawner(varMap settings) :
				marker(settings),
				settings(settings)
			{
		interval = setup::getInt(settings,"interval",1000);
		cout << "spawner :: interval = " << interval << endl;
		spawns_left = setup::getInt(settings,"total-spawns",(setup::getInt(settings,"duration",-interval)/interval));
		cout << "spawner :: spawns left = " << spawns_left << endl;
		velocity_multiplier = setup::getDouble(settings,"inherit-velocity",0);
		cout << "spawner :: spawner velocity multiplier = " << velocity_multiplier << endl;
		cout << "spawns left : " << spawns_left << endl;
		spawn_velocity = setup::getDouble3(settings,"velocity",double3(0,0,0));
		cout << "spawner :: initial velocity = " << spawn_velocity.x << "," << spawn_velocity.y << "," << spawn_velocity.z << "\n";
		randomize_velocity = setup::getDouble3(settings,"randomize-velocity",double3(0,0,0));
		cout << "spawner :: velocity rand() = " << randomize_velocity.x << "," << randomize_velocity.y << "," << randomize_velocity.z << "\n";
		counter = 0;
		/*setup shape that is drawn by the engine*/
		m = glShape::sphere(10,10,10);
	}

	~spawner(){
		#if DEBUG_DELETES
			cout << " spawner >";
		#endif
	}

	/**\brief update the spawner.

	Depending on how the spawner is set up, it will count down or
	up through its timers and generate things when they should be
	generated.

	When it is finished, it kills itself**/
	void update(){
		--counter;
		while(counter < 1){
			counter += interval;
			/*calculate random point in sphere within spawn radius*/
			nodeType *n = new nodeType(position+randomPointInSphere(radius),spawn_velocity+randomPointInSphere(randomize_velocity));
            sendMessage(getParent(),MESSAGE_ADD_DRAWABLE,(void*)n);
            onSpawn(n);
			if(spawns_left == -1) continue;
			if(--spawns_left < 1){
				kill();
				return;
			}
		}
		marker::update();
	}

	/**\brief allow the interval between spawns to be set at
	runtime.

	This is essential for any classes inheriting the spawner
	class that have generic settings that may be different from
	the default spawner setting, but are common enough that forcing
	the user to put the new setting in the config file is stuff.**/
	void setSpawnInterval(int newInterval){
		interval = newInterval;
	}

	void setVelocityRandomization(double3 amount){
		randomize_velocity = amount;
	}

	void setVelocityRandomization(double amount){
		randomize_velocity = double3(amount,amount,amount);
	}

	void setInitialVelocity(double3 velocity){
		spawn_velocity = velocity;
	}

	void setSpawnsLeft(int spawnsLeft){
		spawns_left = spawnsLeft;
	}

	int intervalBeforeNextSpawn(){ return counter; }

};




#endif // SPAWNER_H
