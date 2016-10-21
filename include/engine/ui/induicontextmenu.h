#ifndef INDUI_CONTEXT_MENU_H
#define INDUI_CONTEXT_MENU_H

#include "induimod.h"

class IndUICONTEXTMENU : public IndUIMODULE {
protected:


public:

    IndUICONTEXTMENU(string* title) : IndUIMODULE(title){
    	draggable = false;
    }

    ~IndUICONTEXTMENU(){

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

};

#endif // INDUI_CONTEXT_MENU_H
