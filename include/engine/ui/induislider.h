
#ifndef INDUISLIDER_H
#define INDUISLIDER_H
#include "induiwidget.h"

template <class T>
class IndUISlider : public IndUIWidget {
    private:

        /*the minimun value that the slider can represent*/
        T min;
        /*the maximum value that the slider can represent*/
        T max;
        /*the object that is being updated with the value of
        whatever is selected using the slider*/
        T*& ref;
        T tmp;
        /*what the slider is currently doing*/
        int state;
        /*if the slidebox is being hovered over or is selected*/
        bool hover;

        float offset;

        /*store the last value of the cursor*/
        int lastCX, lastCY;

        /*create a text string to draw the current
        value when the user moves it*/
		GLVectorText *value;
		string *valueString;

		/*a generic function to convert an int to a string*/
		string* toString(T i){
			ostringstream convert;   // stream used for the conversion
			convert << i;      // insert the textual representation of 'Number' in the characters in the stream
			return new string(convert.str()); // set 'Result' to the contents of the stream
		}

    public:

    IndUISlider(T minValue,T maxValue,T* &referenceValue,string name) :
    	min(minValue),
    	max(maxValue),
    	ref(referenceValue)
    {
        this->name = name;
        //set the height of the slider
        height = 20;
        //set the state to zero by default, which means that nothing
        //is happening
        state = 0;
        hover = false;

        /*set up the titlestring*/
        valueString = toString(*referenceValue);
		value = new GLVectorText(valueString,16,0,0);
    }

    /*update the slide bar based on input from the mouse. The cursorX
    and cursorY are relative to the top left corner of the widget. Click is
    the reference to an array of which buttons on the mouse are pressed
    or not.
    */
    bool update(int cursorX, int cursorY,int **click,bool* &keyboard)
    {
    	lastCX = cursorX;
    	lastCY = cursorY;

        //calculate variables needed to determine if the cursor is over the slide box
        float ratio = ((float)*ref-(float)min)/((float)max-(float)min);
        int ih = height - MARGIN*2;
        float barWidth = width - MARGIN*2 - ih;

        switch(state) {
		//if nothing is selected
		case 0: {
			//check if the mouse is over the slide box
			if(cursorX > MARGIN+barWidth*ratio && cursorX < MARGIN+barWidth*ratio+ih && cursorY > MARGIN && cursorY < height - MARGIN) {
				hover = true;
				if((*click)[0]) {
					state = 5;
				}
				break;
			}
			hover = false;
			return false;
		}
		//if the scroll box has been selected
		case 5:
			tmp = *ref;
			offset = cursorX;
			cout << "offset = " << offset << endl;
			//after setting up the scoll box values, set the state to scrolling
			state = 6;
		//if the scoll box is currently selected
		case 6: {
			float minP = (cursorX-offset);
			float spread = max - min;
			T mov = (minP/barWidth) * spread;
			T oref = *ref;
			*ref = tmp+mov;
			if(oref != *ref) changed = true;

			valueString = toString(*ref);

			//check to make sure that it has not passed the min or max values
			if(*ref > max) *ref = max;
			else if(*ref < min) *ref = min;
			//if the mouse stops clicking, change the state back to 0
			if(!(*click)[0]) {
				cout << "done adjusting value = "<< *ref << "\n";
				state = 0;
			}
			return true;
		}



		}//end of switching between states

        return false;
    }


    void Draw(int offsetX,int offsetY)
    {
        float ratio = ((float)*ref-(float)min)/((float)max-(float)min);
        //get the inner height of the bars
        int ih = height - MARGIN*2;
        //get the width of the bar. It is the width of the widget minus the
        //margin on both sides (x2) and the height of the slider box (half
        //for both the left and right side)
        float barWidth = width - MARGIN*2 - ih;


        glColor3f(0,0,0);
        glLineWidth(3);
        glBegin(GL_LINES);
            glVertex2i(offsetX+MARGIN+ih*.5f,offsetY-height*.5);
            glVertex2i(offsetX+width-MARGIN-ih*.5f,offsetY-height*.5);
        glEnd();
        if(hover) glColor3f(1,0,0);

        glBegin(GL_TRIANGLE_FAN);
            glVertex3i(offsetX+MARGIN+barWidth*ratio,offsetY-MARGIN,1);
            glVertex3i(offsetX+MARGIN+barWidth*ratio,offsetY-height+MARGIN,1);
            glVertex3i(offsetX+MARGIN+barWidth*ratio+ih,offsetY-height+MARGIN,1);
            glVertex3i(offsetX+MARGIN+barWidth*ratio+ih,offsetY-MARGIN,1);
        glEnd();



		/*if the slider is being dragged, show the current value so that the
		user knows they are picking what they want.*/
		if(state == 6){
			glColor4f(1,1,1,.8);
			glBegin(GL_TRIANGLE_FAN);
				glVertex3i(lastCX+offsetX,		lastCY+offsetY,		2);
				glVertex3i(lastCX+offsetX-20,	lastCY+offsetY,		2);
				glVertex3i(lastCX+offsetX-20,	lastCY+offsetY-20,	2);
				glVertex3i(lastCX+offsetX,		lastCY+offsetY-20,	2);
			glEnd();
			glPushMatrix();
			glTranslated(0,0,3);
			glColor4f(0,0,0,1);
			value->OffsetDraw(lastCX+offsetX-20,lastCY+offsetY);
			glPopMatrix();
		}

        //cout << name << " " << *rt << endl;
    }


    bool SetReference(void * value)
    {
        //*ref = (T*)value;
        //cout << "setting reference = " << **ref << "\n";
        return false;
    }

    int Height(){return height;}

    bool Active()
    {
        return state>0?true:false;
    }

	bool hasChanged(){
		return !(!changed || (changed = false));
	}

};


#endif
