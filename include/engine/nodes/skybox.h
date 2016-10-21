/*the skybox is special in the fact that it takes a shortcut in
rendering. It should be drawn before anything else, then it will
automatically clear the depth buffer so everything gets drawn
in front of it.*/


#ifndef SKYBOX_H
#define SKYBOX_H
#include "../graphics/drawable.h"

class skybox : public drawable{
private:
	GLuint dl;
public:

	skybox(GLuint newDl) :
		dl(newDl) {
	}

	void draw() {
		//float mat[16];
		//glPushMatrix();
			//glGetFloatv(GL_MODELVIEW_MATRIX, mat);
			//glTranslatef(-mat[12],-mat[13],-mat[14]);
			//drawMesh(m);
			glCallList(dl);
		//glPopMatrix();
		//glClear(GL_DEPTH_BUFFER_BIT);
	}

	void update() {

	}

	//mesh*& Mesh(){ return m; }
};



#endif // SKYBOX_H
