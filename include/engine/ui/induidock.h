#ifndef INDUI_DOCK_H
#define INDUI_DOCK_H

/*the indui module is a version of the induimodule that is docked
to part of the screen and can't be dragged around*/

#include "induimod.h"

#define UI_DOCK_EDGE_TOP 		0
#define UI_DOCK_EDGE_BOTTOM		1
#define UI_DOCK_EDGE_LEFT		2
#define UI_DOCK_EDGE_RIGHT		3


class IndUIDock : public IndUIMODULE{
private:

	int dock_edge;

	int *width;
	int *height;


	bool horizontal;

public:

	IndUIDock(string* &title,int dock_edge,int* width,int* height) :
		IndUIMODULE(title),
		dock_edge(dock_edge),
		width(width),
		height(height){

        //set module to be draggable by default
        draggable = false;
        dragging = false;
        //set active to false until the mouse goes over it
        active = false;
        state = 0;
        /*set the daufault position of the module*/

		switch(dock_edge){
		case UI_DOCK_EDGE_TOP:
			if(!width) this->width = screenSizeX;
			if(!height) this->height = new int(IndUIWidget::getDefaultFontSize()+IndUIWidget::getMargin()*4);
			horizontal = true;
			offsetY = *screenSizeY;
			break;
		case UI_DOCK_EDGE_RIGHT:

			break;
		case UI_DOCK_EDGE_LEFT:
			if(!width) this->width = new int(defaultWidth);
			if(!height) this->height = screenSizeY;
			horizontal = false;
			offsetY = *screenSizeY;
			break;
		case UI_DOCK_EDGE_BOTTOM:
			this->height = new int(IndUIWidget::getDefaultFontSize()+IndUIWidget::getMargin()*4);
			offsetY = *this->height;
			break;
		}

        dragAreaHeight = 0;


        widgetCount = 0;
        widgets = new IndUIWidget*[widgetCount];
        currentWidget = -1;

        visible = true;
	}


    void update(){

		switch(state){
		//if the mouse is not even over the module
		case 0:
			//check for mouse going over the drag zone (and not clicking) or over any sub modules
			if(*mX > offsetX && *mX< offsetX + *width && *mY < offsetY && !(*mC)[0]) {
				if(*mY > offsetY - *height) {
					state = 10;
					//cout << "state = 10 entered widget area\n";
				}
			}
			break;

		//if the cursor is over the widget part of the module
		case 10:
			if(currentWidget > -1) {
				//update the widget until it returns false
				if(!widgets[currentWidget]->update(*mX-toff,offsetY-*mY,mC,kb) && !widgets[currentWidget]->Active()) {
				currentWidget = -1;
				}
			}
			else {
				//figure out which widget should be updated based on the
				//mouse position
				toff = offsetX;
				for(int i=0;i<widgetCount;i++) {
					//figure out where the bottom of the current widget is
					boff = toff+widgets[i]->Width();
					//check if the mouse is within this widget
					if(*mX > toff && *mX < boff) {
						widgets[i]->update(*mX-toff,offsetY-*mY,mC,kb);
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
					if(offsetX > *mX || offsetX + *width < *mX || offsetY - *height >  *mY) {
						state = 0;
						//cout << "state = 0 left module area\n";
						break;
					}
				}//end of if mouse is not being clicked
				break;
            }//end of if there is a widget currently running
        }//end of the switch between module states
    }//end of update function



	void draw(){
        glColor4f(0,0,0,1);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3i(0							,*IndUIMODULE::screenSizeY			,-1);
            glVertex3i(0							,(*IndUIMODULE::screenSizeY)-*height	,-1);
            glVertex3i(*IndUIMODULE::screenSizeX	,(*IndUIMODULE::screenSizeY)-*height	,-1);
            glVertex3i(*IndUIMODULE::screenSizeX	,*IndUIMODULE::screenSizeY			,-1);
        glEnd();


        glTranslatef(0,0,.1f);


        int off;
        switch(dock_edge){
		case UI_DOCK_EDGE_TOP:
			off = offsetX;
			for(int i=0;i<widgetCount;i++) {
				widgets[i]->Draw(off,*IndUIMODULE::screenSizeY);
				off += widgets[i]->Width();
			}
			break;
		case UI_DOCK_EDGE_LEFT:
			off = offsetY;

			for(int i=0;i<widgetCount;i++) {
				widgets[i]->Draw(offsetX,off);
				off -= widgets[i]->Height();
			}
			break;
        }


	}

};



#endif // INDUI_DOCK_H
