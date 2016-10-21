#ifndef FLAME_H
#define FLAME_H

#include "../particle.h"

class flame : public particle{
public:

	flame(double3 position,double3 velocity) :
				particle(double3(0,.15f,0),.998f,position,velocity)
			{
	}

	flame(double3 position,double3 velocity,double dampening) :
				particle(double3(0,.15f,0),dampening,position,velocity)
			{
	}

	~flame(){
		#if DEBUG_DELETES
			cout << " flame >";
		#endif
	}

};

#endif // FLAME_H
