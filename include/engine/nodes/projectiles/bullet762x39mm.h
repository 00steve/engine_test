
#ifndef BULLET_762x39MM_H
#define BULLET_762x39MM_H

#include "bullet.h"

class bullet762x39mm : public bullet{
public:

	bullet762x39mm(double3 position, double3 direction) :
		bullet(position,direction,8){}

};
#endif // BULLET_9MM_H
