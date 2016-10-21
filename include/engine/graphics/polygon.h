#ifndef POLYGON_H
#define POLYGON_H

#include "../core/double3.h"
#include "gl/gl.h"

/*the polygon struct should be used to store all of the info
that is needed between a pair of beginGL() and endGL() calls.*/






struct polygon
{
	/*store the mode that openGL should draw this polygon
	in. The types are..
	GL_POINTS,
	GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
	GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
	GL_QUADS, GL_QUAD_STRIP,
	GL_POLYGON*/
	GLenum mode;

	/*store the reference to a list of vertices, an array of
	the indecies to use and the number of indecies that there
	are to use.*/
	vertex *vertices;
	int *vertex_index;
	int vertex_count;

	/*store the reference to a list of colors*/
	rgb *colors;
	int *color_index;
	int color_count;

	/*store the reference to the list of normals*/
	vertex *normals;
	int *normal_index;
	int normal_count;

	/*there will be as many texcoords as there are
	verticies. Some may have no tex coords, other will. If
	it doesn't, it defaults to -1.*/
	vertex *texcoords;
	int *texcoord_index;

	/*store the index of the material that is used
	by the object when it is being drawn.*/
	int mtl_index;

};



#endif // POLYGON_H
