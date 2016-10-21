/*this class holds everything that is needed to be known about
a texture, like the rgba values at each pixel, the link to opengl,
some other shit I can't think of at this very moment, and more
amazing things, all for 4 easy payments of $29.95!*/


#ifndef TEXTURE_H
#define TEXTURE_H

#include "../graphics/gl_structs.h"
#include "../core/int2.h"
#include <GL/glfw.h>
#include <GL/glu.h>
#include "FreeImage.h"

class texture{
public:

	/*store all of the pixel information about the image*/
	rgba* p;
	/*store the dimensions of the rgba object 2d array*/
	int2 s;
	/*store the openGL id of the texture*/
	GLuint id;


	texture() :
		p(NULL) {
	}

	texture(GLuint id,int width,int height) :
		p(NULL),
		s(int2(width,height)),
		id(id){

	}

	/*stores the size of the texture, as well as sets the
	size of the array that */
	void setSize(int2 newSize){
		s = newSize;
		p = new rgba[s.x*s.y]();
	}

	rgba& pixel(int x,int y){
		return p[y*s.x+x];
	}


	GLuint addToOpenGL(){
		GLuint id;
		glGenTextures(1,&id);
		glBindTexture(GL_TEXTURE_2D,id);
		//gluBuild1DMipmaps(GL_TEXTURE_2D,4,s.x,s.y,GL_RGBA,GL_UNSIGNED_INT,(void*) p);

		return id;
	}

	int getWidth(){ return s.x; }
	int getHeight(){ return s.y; }



};



#endif // TEXTURE_H
