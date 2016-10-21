#ifndef MATERIAL_H
#define MATERIAL_H

#include "../core/double3.h"
#include <string.h>

class material{
public:

	string name;
	/**/
	string map_diffuse;

	rgb color_diffuse;

	GLuint diffuse_id;
};


#endif // MATERIAL_H
