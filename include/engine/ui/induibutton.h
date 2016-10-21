#ifndef INDUI_BUTTON_H
#define INDUI_BUTTON_H

#include "induiwidget.h"
#include "../graphics/glvectortext.h"

class IndUIButton : public IndUIWidget {
private:
	int state;
	string *label;
	GLVectorText *glLabel;
	indui_function func;
public:

	IndUIButton(string label,indui_function &func) :
		state(0),
		label(new string(label)),
		func(func){

		height = defaultFontSize + MARGIN*4;
		glLabel = new GLVectorText(this->label,buttonFontSize);
		glLabel->setAlignment(0);

	}

    bool update(int cursorX, int cursorY,int **click,bool* &keyboard){
		//cout << "bt " << cursorX << "," << cursorY << endl;
		switch(state){
		case 0:
			if(cursorX > MARGIN && cursorX < width-MARGIN && cursorY > MARGIN && cursorY < height-MARGIN){
				state = 1;
			}
			break;
		case 1:
			if(*click[0]){
				indui_function func2 = *func;
				func2(NULL);
				state = 5;
			}
			break;
		case 5:
			if(!*click[0]){
				cout << "click end.\n";
				state = 0;
			}
		}
		return false;
	}


    void Draw(int offsetX,int offsetY) {
		setGLForText();
		glLabel->OffsetDraw(offsetX+(width*.5),offsetY-MARGIN*2);
		switch(state){
		case 0:
		case 1:
			setGLForButton();
			glBegin(GL_TRIANGLE_FAN);
				glVertex3i(offsetX+MARGIN,offsetY-MARGIN,0);
				glVertex3i(offsetX+MARGIN,offsetY-height+MARGIN,0);
				glVertex3i(offsetX+width-MARGIN,offsetY-height+MARGIN,0);
				glVertex3i(offsetX+width-MARGIN,offsetY-MARGIN,0);
			glEnd();
			break;
		case 5:
			setGLForActiveButton();
			glBegin(GL_TRIANGLE_FAN);
				glVertex3i(offsetX+MARGIN,offsetY-MARGIN,0);
				glVertex3i(offsetX+MARGIN,offsetY-height+MARGIN,0);
				glVertex3i(offsetX+width-MARGIN,offsetY-height+MARGIN,0);
				glVertex3i(offsetX+width-MARGIN,offsetY-MARGIN,0);
			glEnd();
			break;
		}



    }


	bool hasChanged(){
		return !(!changed || (changed = false));
	}


};





#endif // INDUI_BUTTON_H
