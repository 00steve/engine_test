/*IndUI is a class that lets the programer build a user interface
that only relies on openGL and nothing else.*/

#ifndef INDUI_H
#define INDUI_H
#include <string.h>

typedef bool (&indui_function)(void*);

/*create data type to hold info being sent to
any functions that are called by the indui function.
This should be a pointer to the struct that holds the
return value as wall.*/
struct indui_callback{
	/*hold reference to indui*/
	void* indui_instance;
	/*the object that is calling the function*/
	void* caller;
	/*this is what is returned from the function*/
	void* value;

	indui_callback(void* indui_instance,void* value) :
		indui_instance(indui_instance),
		value(value){
	}
};






#include "../graphics/glvectortext.h"
#include "induimod.h"



class IndUI
{
private:

    //screen width and height and aspect ratio of the screen
    int *sSX, *sSY;
    int *mPX, *mPY;
    int **mB;
    bool *kB;

    float aspectRatio;

    /*let the ui store a list of modules that are in it*/
    IndUIMODULE **modules;
    int moduleCount;
    int active;


    void init(){

        //calculate the aspect ratio of the screen
        aspectRatio = (float)*sSY / (float)*sSX;

        /*set up the array that holds the modules*/
        moduleCount = 0;
        active = false;

        IndUIMODULE::setScreenSize(sSX,sSY);
    }


public:

    IndUI(int *mousePositionX,int *mousePositionY,int **mouseButton,bool* &keyboard) :
    	sSX(0),
    	sSY(0),
    	mPX(mousePositionX),
    	mPY(mousePositionY),
    	mB(mouseButton),
    	kB(keyboard) {
		init();
    }

    /*the only info required by IndUI is the pixel resolution of
    the window.*/
    IndUI(int *screenSizeX,int *screenSizeY,int *mousePositionX,int *mousePositionY,int **mouseButton,bool* &keyboard) :
    	sSX(screenSizeX),
    	sSY(screenSizeY),
    	mPX(mousePositionX),
    	mPY(mousePositionY),
    	mB(mouseButton),
    	kB(keyboard) {
		init();
    }


    /*updates the menu system*/
    void update(){
        active = false;
        /*check to see if the mouse is over any of the modules*/
        int i = 0;
        while(i < moduleCount) {
            if(modules[i]->Visible()) {
                modules[i]->update();
                if(modules[i]->IsActive()) {
                    active = true;
                    break;
                }
            }
            ++i;
        }
    }


    /*draws all of the user interface modules*/
    void draw(){

        /*reset the graphics for drawing a menu*/
        glPushClientAttrib(GL_MATRIX_MODE | GL_MODELVIEW | GL_TEXTURE_BIT | GL_TEXTURE_2D);
        glPushMatrix();

			glDisable(GL_TEXTURE_2D);
            glClear(GL_DEPTH_BUFFER_BIT);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, *sSX,0,*sSY,  -10.0f, 10.0f);

            glMatrixMode(GL_MODELVIEW);

			int i = 0;
			while(i < moduleCount) {
				if(modules[i]->Visible()) {
					glColor4f(0,0,0,.99f);
					modules[i]->draw();
				}
				++i;
			}


        glPopMatrix();
        glPopClientAttrib();



    }


    /*add a module to the list of modules*/
    void AddModule(IndUIMODULE *newMod){
    	newMod->setInput(mPX,mPY,mB,kB);
        IndUIMODULE **tmp = new IndUIMODULE*[moduleCount+1];
        memcpy(tmp,modules,sizeof(IndUIMODULE *)*moduleCount);//copy info from the old list to the new one
        modules = tmp;//store the temp list as the new list
        modules[moduleCount++] = newMod;

    }

	/*returns if any of the modules are
	currently active. If they are, nothing important
	should be happening in the engine.*/
    bool IsActive(){return active;}

    /*sets new pointers to the size and width of the view*/
    void setScreenSize(int *screen_width,int *screen_height){
		sSX = screen_width;
		sSY = screen_height;
    }
};





bool BuildTextTexture()
{
    /*array to hold all of the bit patterns for text rendering.
    This provides a 256x256 pixal 1 channel image that holds each
    ascii character in a 16x16 pixel area. This gives us room for
    256 characters (16 on each line)*/
    //char *img = new char[65536];




    return true;
};




#endif
