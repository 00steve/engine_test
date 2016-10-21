
#ifndef INDUICHECKBOX_H
#define INDUICHECKBOX_H
#include "induiwidget.h"

class IndUICheckbox : public IndUIWidget
{
    private:
        /*the object that is being updated with the value of
        whatever is selected using the slider*/
        bool*& ref;

        /*what the slider is currently doing*/
        int state;
        /*if the slidebox is being hovered over or is selected*/
        bool hover;

        float offset;

    public:

    IndUICheckbox(bool* &referenceValue,string name) : ref(referenceValue)
    {
        this->name = name;
        //set the height of the slider
        height = 20;
        //set the state to zero by default, which means that nothing
        //is happening
        state = 0;
        hover = false;
    }

    /*update the slide bar based on input from the mouse. The cursorX
    and cursorY are relative to the top left corner of the widget. Click is
    the reference to an array of which buttons on the mouse are pressed
    or not.
    */
    bool Update(int cursorX, int cursorY,int **click)
    {


        //calculate variables needed to determine if the cursor is over the slide box
        int ih = height - MARGIN*2;
        //float barWidth = width - MARGIN*2 - ih;

        switch(state)
        {
            //if nothing is selected
            case 0:
            {
                //check if the mouse is over the slide box
                if(cursorX > MARGIN && cursorX < MARGIN+ih && cursorY > MARGIN && cursorY < height - MARGIN)
                {
                    hover = true;
                    if((*click)[0])
                    {
                        *ref = !*ref;
                        state = 5;
                        cout << "reverse " << name << " checkbox = " <<  (*ref?"true" : "false") << "\n";
                    }
                    return false;
                }

                hover = false;
                return false;
            }

            //if the box has been clicked
            case 5:
                if(!(*click)[0]) state = 0;
                return true;
        }//end of switching between states

        return false;
    }


    void Draw(int offsetX,int offsetY)
    {
        //get the inner height of the bars
        int ih = height - MARGIN*2;
        //get the width of the bar. It is the width of the widget minus the
        //margin on both sides (x2) and the height of the slider box (half
        //for both the left and right side)
        //float barWidth = width - MARGIN*2 - ih;

        glColor3f(0,0,0);

        if(hover) glColor3f(1,0,0);
        glLineWidth(1);
        glBegin(GL_LINE_STRIP);
            glVertex3i(offsetX+MARGIN,offsetY-MARGIN,1);
            glVertex3i(offsetX+MARGIN,offsetY-height+MARGIN,1);
            glVertex3i(offsetX+MARGIN+ih,offsetY-height+MARGIN,1);
            glVertex3i(offsetX+MARGIN+ih,offsetY-MARGIN,1);
            glVertex3i(offsetX+MARGIN,offsetY-MARGIN,1);
        glEnd();

        if(*ref)
        {
            glLineWidth(3);
            glBegin(GL_LINES);
                glVertex3i(offsetX+MARGIN,offsetY-MARGIN,1);
                glVertex3i(offsetX+MARGIN+ih,offsetY-height+MARGIN,1);
                glVertex3i(offsetX+MARGIN,offsetY-height+MARGIN,1);
                glVertex3i(offsetX+MARGIN+ih,offsetY-MARGIN,1);
                //glVertex3i(offsetX+MARGIN,offsetY-MARGIN,1);
            glEnd();
        }

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

};


#endif

