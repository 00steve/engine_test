#ifndef INDUI_DROPDOWN_H
#define INDUI_DROPDOWN_H

#include "induiwidget.h"
#include "../graphics/glvectortext.h"


#include "indui_optionselection.h"
#include "indui_optionfunction.h"



class IndUIDropdown : public IndUIWidget{
private:

	int state;
	string *label;

	optionSelection* options;

	GLVectorText *glLabel;

protected:

	static bool onClick(IndUIDropdown* dropDown,optionSelection* selection){ return false; }


public:

	IndUIDropdown(string title) :
		state(0),
		label(new string(title)),
		options(new optionSelection(label)){
		height = defaultFontSize + MARGIN*4;
		glLabel = options->getLabel();
		glLabel->Draw();
		width = glLabel->getWidth() + MARGIN*4;
	}


	/*adds labels to */
	void addOption(string label){
		options->add(new optionSelection(new string(label)));
		width = options->calculateWidth();
	}


	void addOption(string label,indui_function &func){
		optionSelection *tmpOption = new optionFunction(new string(label),func);
		options->add(tmpOption);
		width = options->calculateWidth();
	}


	void addOption(optionFunction *newFunction){
		options->add(newFunction);
		width = options->calculateWidth();
	}


    bool update(int cursorX, int cursorY,int **click,bool* &keyboard){
		switch(state){
		case 0:
			if(cursorX > MARGIN && cursorX < width-MARGIN && cursorY > MARGIN && cursorY < height-MARGIN){
				state = 1;
			}
			break;
		case 1:
			if(**click){
				#if DEBUG_UI
				cout << "open menu!\n";
				#endif
				state = 5;
			}
			break;
		case 5:
			if((cursorX < 0 || cursorX > width || cursorY < 0 || cursorY > (height+MARGIN) * options->childCount()) && **click){
				#if DEBUG_UI
				cout << "leave menu.\n";
				#endif
				state = 0;
			} else {
				options->update(cursorX,cursorY,click,keyboard);
			}
			return true;
			break;
		}
		return false;
	}

    void Draw(int offsetX,int offsetY) {

		switch(state){
		case 0:
		case 1:


			setGLForText();
			glLabel->OffsetDraw(offsetX+MARGIN*2,offsetY-MARGIN*2);

			setGLForButton();
			glBegin(GL_TRIANGLE_FAN);
				glVertex3i(offsetX+MARGIN,offsetY-MARGIN,0);
				glVertex3i(offsetX+MARGIN,offsetY-height+MARGIN,0);
				glVertex3i(offsetX+width-MARGIN,offsetY-height+MARGIN,0);
				glVertex3i(offsetX+width-MARGIN,offsetY-MARGIN,0);
			glEnd();

			break;
		case 5:

			options->Draw(offsetX,offsetY);

			setGLForActiveButton();
			glBegin(GL_TRIANGLE_FAN);
				glVertex3i(offsetX,offsetY,0);
				glVertex3i(offsetX,offsetY-height,0);
				glVertex3i(offsetX+width,offsetY-height,0);
				glVertex3i(offsetX+width,offsetY,0);
			glEnd();


			break;
		}



    }

	bool hasChanged(){
		return !(!changed || (changed = false));
	}



};



#endif // INDUI_DROPDOWN_H
