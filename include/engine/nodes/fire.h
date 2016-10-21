#ifndef FIRE_H
#define FIRE_H

#include "spawner.h"
#include "particles/flame.h"

class fire : public spawner<flame>{
private:
	double intensity;

public:


	void onSpawn(flame *node){
	}

public:

	fire(varMap settings) : spawner<flame>(settings){
		//setSpawnInterval(10);
		intensity = setup::getDouble(settings,"intensity",1);
		setVelocityRandomization(setup::getDouble3(settings,"randomize-velocity",double3(1,1,1))*intensity);
		setInitialVelocity(setup::getDouble3(settings,"velocity",double3(0,.35,0))*intensity);
	}

	~fire(){
		#if DEBUG_DELETES
			cout << " fire >";
		#endif
	}

};

#endif // FIRE_H
