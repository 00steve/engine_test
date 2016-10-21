#ifndef INDUIMOD_H
#define INDUIMOD_H


#include "../graphics/glvectortext.h"
#include "induiwidget.h"

class IndUIMODULE {
protected:

    //store the background image that should be used for the drag area
    static GLuint *dragBackgroundImage;

	static double title_color_r;
	static double title_color_g;
	static double title_color_b;

    //store the default text size
    static float defaultTextSize;

    static int *screenSizeX;
    static int *screenSizeY;

    //create variables to store default info about all of the modules' designs
    static int defaultWidth;
    static int defaultDragAreaHeight;

    /*store pointers to ints that stay updated with the
    mouse's position*/
    int *mX;
    int *mY;
    /*the reference to an array of bools that holds
    which buttons have been clicked on the mouse.*/
    int **mC;
    bool *kb;

    /*set variables specific to the module*/
    int cursorOffsetX,cursorOffsetY;
    /*the module's offset from the origin lower left corner of
    the screen.*/
    int offsetX,offsetY;
    /*the total width and height of the module*/
    int width,height;
    int dragAreaHeight;
    /*what the module is doing*/
    int state;
    /*if the module can be moved around the screen*/
    bool draggable;
    bool dragging;
    /*cursor is over the module*/
    bool active;
    /*store list of widgets that are in this module*/
    IndUIWidget **widgets;
    int widgetCount;
    int currentWidget;

    int toff,boff;

    /*if the module should be drawn or calculated*/
    bool visible;

    /*store an object for writing the title*/
    GLVectorText *title;
    string *titleString;

    /*a function that allows the indUImodule class to perform any actions
    on the widget that has just been added*/
    virtual void SetupAddedWidget(IndUIWidget *newWidget){
        //newWidget->SetWidth(width);
    }


public:

    IndUIMODULE(string* title){
        //set module to be draggable by default
        draggable = true;
        dragging = false;
        //set active to false until the mouse goes over it
        active = false;
        state = 0;
        /*set the daufault position of the module*/
        offsetX = 0;
        offsetY = 0;

        width = defaultWidth;
        dragAreaHeight = defaultDragAreaHeight;

        height = 20;

        widgetCount = 0;
        widgets = new IndUIWidget*[widgetCount];
        currentWidget = -1;

        visible = true;

        this->titleString = title;
        this->title = new GLVectorText(this->titleString,defaultTextSize);

    }


    bool AddWidget(IndUIWidget *w){
        //set settings in the widget
        SetupAddedWidget(w);
        IndUIWidget **tmp = new IndUIWidget*[widgetCount+1];
        memcpy(tmp,widgets,sizeof(IndUIWidget *)*widgetCount);//copy info from the old list to the new one
        widgets = tmp;//store the temp list as the new list
        widgets[widgetCount++] = w;
        height += w->Height();
        return true;
    }

    static void setScreenSize(int *x,int *y){
        screenSizeX = x;
        screenSizeY = y;
    }

    void setInput(int *mouseX,int *mouseY,int **mouseClick,bool* &keyboard){
        mX = mouseX;
        mY = mouseY;
        mC = mouseClick;
        kb = keyboard;
    }

	void setOffset(int newX,int newY){
		offsetX = newX;
		offsetY = newY;
	}

    /*draw an empty box*/
    virtual void draw() {
        //draw the name
        /*draw a shadow before drawing the box*/
        glColor4f(0,0,0,.1f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3i(offsetX-6,offsetY+6,-1);
            glVertex3i(offsetX-6,offsetY-height-6,-1);
            glVertex3i(offsetX+width+6,offsetY-height-6,-1);
            glVertex3i(offsetX+width+6,offsetY+6,-1);
        glEnd();


        glColor4f(.2f,.2f,.2f,1);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2i(offsetX,offsetY);
            glVertex2i(offsetX,offsetY-height);
            glVertex2i(offsetX+width,offsetY-height);
            glVertex2i(offsetX+width,offsetY);
        glEnd();
        glTranslatef(0,0,.1f);
        int off = offsetY-dragAreaHeight;


        for(int i=0;i<widgetCount;i++) {
            widgets[i]->Draw(offsetX,off);
            off -= widgets[i]->Height();
        }

        glColor4f(title_color_r,title_color_g,title_color_b,1);
        glLineWidth(2);
        glPointSize(2);
        title->OffsetDraw(offsetX+4,offsetY-4);


    }

    virtual void update(){
		switch(state){
		//if the mouse is not even over the module
		case 0:
			//check for mouse going over the drag zone (and not clicking) or over any sub modules
			if(*mX > offsetX && *mX< offsetX + width && *mY < offsetY && !(*mC)[0]) {
				if(*mY > offsetY - dragAreaHeight) {
					state = 1;
					cout << "state = 1 entered drag area\n";
				}
				else if(*mY > offsetY - height) {
					state = 10;
					cout << "state = 10 entered widget area\n";
				}
			}
			break;
		//if the mouse is over the drag zone
		case 1:
			//check to see if the mouse left the drag zone.
			if(offsetX > *mX || offsetX + width < *mX || offsetY < *mY) {
				state = 0;
				cout << "state = 0 left module area\n";
				break;
			}
			//check to see if state went over to the widget portion of the module
			if(offsetY - dragAreaHeight > *mY) {
				cout << "state = 10 entered widget area\n";
				state = 10;
			}
			//check to see if the mouse has been clicked. If it is, set dragging to true
			if((*mC)[0]) {
				state = 5;
				cout << "state = 5 start dragging\n";
			}

			break;
		//setup variables needed for when the module is being dragged
		case 5:
			cursorOffsetX = *mX-offsetX;
			cursorOffsetY = *mY-offsetY;
			state = 6;
			//exclue the break so that it runs the case 6 code right away
		//if the module is being dragged. Check for the mouse unclicking
		case 6:
			offsetX = *mX - cursorOffsetX;
			offsetY = *mY - cursorOffsetY;
			//check to make sure that the module is not going out of the screen
			if(offsetX < 0) offsetX = 0;
			else if(offsetX+width > *screenSizeX) offsetX = *screenSizeX - width;
			if(offsetY - height < 0) offsetY = height;//*screenSizeY +
			else if(offsetY > *screenSizeY) offsetY = *screenSizeY;

			//check to see if the mouse is still being clicked
			if(!(*mC)[0]) {
				if(*mX > offsetX && *mX< offsetX + width && *mY < offsetY) {
					if(*mY > offsetY - dragAreaHeight) {
						state = 1;
						cout << "state = 1 entered drag area\n";
						break;
					}
					else if(*mY > offsetY - height) {
						state = 10;
						cout << "state = 10 entered widget area\n";
						break;
					}
				}
				//if mouse is not over the drag area or widget area, default to
				//being out of box
				cout << "state = 0 out of module\n";
				state = 0;
			}//end of if mouse is not being clicked any more
			break;

		//if the cursor is over the widget part of the module
		case 10:
			if(currentWidget > -1) {
				//update the widget until it returns false
				if(!widgets[currentWidget]->update(*mX-offsetX,*mY-boff,mC,kb) && !widgets[currentWidget]->Active()) {
				currentWidget = -1;
				}
			}
			else {
				//figure out which widget should be updated based on the
				//mouse position
				toff = offsetY-dragAreaHeight;
				for(int i=0;i<widgetCount;i++) {
					//figure out where the bottom of the current widget is
					boff = toff-widgets[i]->Height();
					//check if the mouse is within this widget
					if(*mY > boff && *mY < toff) {
						widgets[i]->update(*mX-offsetX,*mY-boff,mC,kb);
						currentWidget = i;
						break;
					}
					//set the top offset to what the bottom of the last widget was
					toff = boff;
				}

				//if the mouse is not being clicked, check to see if the cursor
				//has left the area of the modules.
				if(!(*mC)[0]) {
					//check to see if the mouse left the widget zone
					if(offsetX > *mX || offsetX + width < *mX || offsetY - height >  *mY) {
						state = 0;
						cout << "state = 0 left module area\n";
						break;
					}
					//check to see if state went over to the widget portion of the module
					if(offsetY - dragAreaHeight < *mY) {
						state = 1;
						cout << "state = 1 entered drag area\n";
					}
				}//end of if mouse is not being clicked
				break;
            }//end of if there is a widget currently running
        }//end of the switch between module states
    }//end of update function

    /*return the first widget with a name that matches
     the function*/
    IndUIWidget *GetWidget(string name){
        int i = 0;
        while(i < widgetCount)
        {
            if(widgets[i]->Name() == name)
            {
                return widgets[i];
            }
            ++i;
        }
        return NULL;
    }

    void Visible(bool isVisible){visible = isVisible;}
    bool Visible(){return visible;}

    bool IsActive(){return state==0?false:true;}
    int MouseX(){return *mX;}
    int MouseY(){return *mY;}
    bool LeftClick(){return (bool)((*mC)[0]);}

	int GetWidgetCount(){ return widgetCount; }

};

int IndUIMODULE::defaultWidth = 200;
float IndUIMODULE::defaultTextSize = 16;
int* IndUIMODULE::screenSizeX;
int* IndUIMODULE::screenSizeY;
int IndUIMODULE::defaultDragAreaHeight = 20;
GLuint* IndUIMODULE::dragBackgroundImage = NULL;
double IndUIMODULE::title_color_r = 1;
double IndUIMODULE::title_color_g = 1;
double IndUIMODULE::title_color_b = 1;
#endif
