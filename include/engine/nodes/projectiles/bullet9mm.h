#ifndef BULLET_9MM_H
#define BULLET_9MM_H

#include "bullet.h"
class bullet9mm : public bullet{
public:

	bullet9mm(double3 position, double3 direction) :
		bullet(position,direction,7.5){}

};
#endif // BULLET_9MM_H
