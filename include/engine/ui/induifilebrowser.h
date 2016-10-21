#ifndef INDUI_FILE_BROWSER_H
#define INDUI_FILE_BROWSER_H


#include "../ui/induimod.h"

class IndUIFileBrowser : public IndUIMODULE{
private:



public:


	IndUIFileBrowser(string* title) :
		IndUIMODULE(title){
		}




	void draw(){
		/*draw background*/
		glColor4f(0,0,0,.7f);
		glBegin(GL_POLYGON);
			glVertex3i(0,0,-5);
			glVertex3i(0,*screenSizeY,-5);
			glVertex3i(*screenSizeX,*screenSizeY,-5);
			glVertex3i(*screenSizeX,0,-5);

		glEnd();

	}

};



#endif // INDUI_FILE_BROWSER_H
