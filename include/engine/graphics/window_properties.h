#ifndef WINDOW_PROPERTIES_H
#define WINDOW_PROPERTIES_H

#include "../core/view.h"

struct window_properties{

	int *width;
	int *height;

	list<node*> views;

};



#endif // WINDOW_PROPERTIES_H
