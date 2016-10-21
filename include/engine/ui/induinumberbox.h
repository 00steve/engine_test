
#ifndef INDUINUMBERBOX_H
#define INDUINUMBERBOX_H
#include "induiwidget.h"
#include "../graphics/glvectortext.h"

class IndUINumberBox : public IndUIWidget
{
    private:
        //store a reference to the string that is being edited
        string *text;
        double *value;

        //keeps track of what is currently being done to the textbox
        int state;

        //keep track of the index of the text string that chars start
        //and end being selected at
        int selectStart;
        int selectEnd;
        //store the max length that the text can be
        int maxLength;
        //create a text rendering object to show the text
        GLVectorText *glvt;

        //create a list of keys that have already been pressed
        bool *pressed;

        //cut selected text
        void CutSelected() {
            //if the cursor has been placed somewhere in the string
			if(selectEnd > selectStart) {
				//if the end cursor is bigger than the start cursor,
				//that means that a char or more has been selected.
				//cut it out of the string

				int s = selectStart-1;
				int n = selectEnd-selectStart;
				int tl = text->length();
				int c ;
				for( c= s; c+n < tl; c++ ) {
				 (*text)[c] = (*text)[c+n];
				}
				*text = text->substr(0,tl-n);
			}
        }



    public:

    IndUINumberBox(string name,double &value) {
        this->name = name;
        this->value = &value;
        this->text = new string("0.0");
        //set the state to zero by default, which means that nothing
        //is happening
        state = 0;
        //set the text in the text renderer
        glvt = new GLVectorText(this->text,defaultFontSize,width-MARGIN*2);

        //set height to default
        height = defaultFontSize + MARGIN*4;

        //set the maximum length of the field
        maxLength = 40;

        selectStart = -1;
        selectEnd = -1;
        pressed = new bool[325];
        for(int i=0;i<325;i++)
        {
            pressed[i] = false;
        }
    }

    /*update the slide bar based on input from the mouse. The cursorX
    and cursorY are relative to the top left corner of the widget. Click is
    the reference to an array of which buttons on the mouse are pressed
    or not.
    */
    bool update(int cursorX, int cursorY,int **click,bool* &keyboard) {
        //calculate variables needed to determine if the cursor is over the slide box
        switch(state) {
		//hovering, but nothing selected
		case 0:
			if(cursorX < 0 || cursorX > width || cursorY < 0 || cursorY > height) return false;
			//if the cursor is clicked, check to see where the cursor is in the string
			if((*click)[0]) {
				selectStart = (cursorX-MARGIN*2+(glvt->CharacterWidth()+glvt->CharacterSpacing()))/(glvt->CharacterWidth()+glvt->CharacterSpacing());
				selectEnd = selectStart;
				cout << selectStart <<endl;
				cout << "cursor offset: " << cursorX << endl;
				state = 10;
				return true;
			}

			//make sure that the cursor is somewhere in the box
			if(selectStart > 0) {
				//check to see if letter or number keys are being pressed
				for(int i=32;i<97;i++) {
					if(keyboard[i]) {
						if(!pressed[i]) {
							//create a temp variable for the key being pressed
							char kbt = i;
							//if shift key is not being pressed, change the key to lower case
							if(!keyboard[287]) kbt += 32;
							//store that the key was pressed, so it doesn't endlessly add the
							//pressed key to the string
							pressed[i] = true;
							//remove selected chars
							CutSelected();
							//insert the new char at the current index
							text->insert(selectStart-1,&kbt,1);
							//push the cursor forward 1 char so that as the user
							//types, the cursor stays to the right of the char
							//previously entered
							selectEnd = selectStart += 1;

						}
					}//end of if key is being pressed
					else {
						pressed[i] = false;
					}

				}//end of looping through the regular chars
				//check backspace key
				if(keyboard[295]) {
					if(!pressed[295]) {
						pressed[295] = true;
						if(selectStart<selectEnd) CutSelected();
						else if(selectStart > 1) {
							selectEnd = selectStart--;
							CutSelected();
						}
						selectEnd = selectStart;
					}
				}
				else {
					pressed[295] = false;
				}
				//check the left arrow key
				if(keyboard[285]) {
					if(!pressed[285] && selectStart > 1) {
						pressed[285] = true;
						--selectStart;
						--selectEnd;
					}
				}
				else {
					pressed[285] = false;
				}
				//check the right arrow key
				if(keyboard[286]) {
					if(!pressed[286] && selectEnd < (signed int)text->length()+1) {
						pressed[286] = true;
						++selectStart;
						++selectEnd;
					}
				}
				else {
					pressed[286] = false;
				}

			}

			else if(cursorX < 0 || cursorX > width || cursorY < 0 || cursorY > height) return false;

			return true;


		//if the cursor is being dragged
		case 10:
			selectEnd = (cursorX-MARGIN*2+(glvt->CharacterWidth()+glvt->CharacterSpacing()))/(glvt->CharacterWidth()+glvt->CharacterSpacing());
			if((unsigned)selectEnd > text->length()+1) selectEnd = text->length()+1;
			if(!(*click)[0]) {
				cout << "select from " << selectStart << " to " << selectEnd << endl;
				state = 0;
				if(cursorX < 0 || cursorX > width || cursorY < 0 || cursorY > height) return false;

				break;
			}
			return true;
        }//end of switching between states

        return false;
    }

    void Draw(int offsetX,int offsetY) {

        if(selectStart > -1) {
            glColor4f(1,0,0,.6f);
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(offsetX+MARGIN+(selectStart)*(glvt->CharacterWidth()+glvt->CharacterSpacing())+1,offsetY-MARGIN,-.05f);
                glVertex3f(offsetX+MARGIN+(selectStart)*(glvt->CharacterWidth()+glvt->CharacterSpacing())+1,offsetY-height+MARGIN,-.05f);
                glVertex3f(offsetX+MARGIN+(selectEnd)*(glvt->CharacterWidth()+glvt->CharacterSpacing())+3,offsetY-height+MARGIN,-.05f);
                glVertex3f(offsetX+MARGIN+(selectEnd)*(glvt->CharacterWidth()+glvt->CharacterSpacing())+3,offsetY-MARGIN,-.05f);
            glEnd();

        }
        glLineWidth(1);
        glColor3f(0,0,0);
        glBegin(GL_LINE_STRIP);
            glVertex2f(offsetX+MARGIN,offsetY-MARGIN);
            glVertex2f(offsetX+MARGIN,offsetY-height+MARGIN);
            glVertex2f(offsetX+width-MARGIN,offsetY-height+MARGIN);
            glVertex2f(offsetX+width-MARGIN,offsetY-MARGIN);
            glVertex2f(offsetX+MARGIN,offsetY-MARGIN);
        glEnd();
        glvt->OffsetDraw(offsetX+MARGIN*2,offsetY-MARGIN*2);
    }

    bool SetReference(void * value) {
        return false;
    }

    int Height(){return height;}

    bool Active() {
        return state>0;
        //kept the textbox active as long as something was selected. I don't like this.
        // || selectStart!=selectEnd?true:false;
    }

	bool hasChanged(){
		return false;
	}

};


#endif

