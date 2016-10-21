#ifndef INDUILIST_H
#define INDUILIST_H


#include "induiwidget.h"

template <class T>
class IndUIList : public IndUIWidget
{
    private:

        /*store an array of references of any object type*/
        T** objects;
        int objectCount;
        int objectsSize;

        /*store an array of the object names*/
        string *objectNames;
        /*store an object for writing the title*/
        GLVectorText *vecText;
        string *objectName;

        /*what the slider is currently doing*/
        int state;
        /*if the slidebox is being hovered over or is selected*/
        bool hover;

        //store how tall the selectbox should be
        float offset;

        //store the current line that should be highlighted
        float lineHighlight;

    public:

    IndUIList(string name,int height)
    {
        this->name = name;
        this->height = height;
        //set the state to zero by default, which means that nothing
        //is happening
        state = 0;
        hover = false;

        objectCount = 0;
        objectsSize = 0;
        //objectNames = new string[];
        this->objectName = new string("Hello");
        vecText = new GLVectorText(this->objectName,defaultFontSize);

    }

    /*update the slide bar based on input from the mouse. The cursorX
    and cursorY are relative to the top left corner of the widget. Click is
    the reference to an array of which buttons on the mouse are pressed
    or not.
    */
    bool update(int cursorX, int cursorY,int **click,bool* &keyboard)
    {


        //calculate variables needed to determine if the cursor is over the slide box
        //float barWidth = width - MARGIN*2;

        switch(state)
        {
            //if nothing is selected
            case 0:
                //get and store the highlight line
                lineHighlight = cursorY/defaultFontSize;
                cout << lineHighlight << endl;
            break;
        }//end of switching between states

        return false;
    }


    void Draw(int offsetX,int offsetY)
    {
        //get the inner height of the bars
        //int ih = height - MARGIN*2;
        //get the width of the bar. It is the width of the widget minus the
        //margin on both sides (x2) and the height of the slider box (half
        //for both the left and right side)
        //float barWidth = width - MARGIN*2 - ih;

        glColor3f(0,0,0);
        glLineWidth(1);
        glBegin(GL_LINE_STRIP);
            glVertex2i(offsetX+MARGIN,offsetY-MARGIN);
            glVertex2i(offsetX+MARGIN,offsetY-height+MARGIN);
            glVertex2i(offsetX+width-MARGIN,offsetY-height+MARGIN);
            glVertex2i(offsetX+width-MARGIN,offsetY-MARGIN);
            glVertex2i(offsetX+MARGIN,offsetY-MARGIN);
        glEnd();
        if(hover) glColor3f(1,0,0);

        //draw all of the items that are listed
        for(int i=0;i<objectCount;i++)
        {
            this->objectName = &objectNames[i];
            vecText->OffsetDraw(offsetX+MARGIN*2,offsetY-MARGIN*2);
            offsetY -= defaultFontSize;
        }

        //cout << name << " " << *rt << endl;
    }


    bool SetReference(void * value)
    {
        //*ref = (T*)value;
        //cout << "setting reference = " << **ref << "\n";
        return false;
    }


    bool AddItem(T *item,string name)
    {
        if(objectCount == objectsSize)
        {
            objectsSize = objectsSize * 2 + 1;

            T **tempObs = new T*[objectsSize];
            memcpy(tempObs,objects,sizeof(T*)*objectCount);//copy info from the old list to the new one
            objects = tempObs;//store the temp list as the new list

            string *tempStr = new string[objectsSize];
            memcpy(tempStr,objectNames,sizeof(string)*objectCount);
            objectNames = tempStr;
        }
        objectNames[objectCount] = name;
        objects[objectCount++] = item;
        return true;
    }

    int Height(){return height;}

    bool Active()
    {
        return state>0?true:false;
    }

};






#endif
