
#ifndef INDUIWIDGET_H
#define INDUIWIDGET_H

class IndUIWidget{
    protected:
        //if the widget can be changed by user input
        bool enabled;
        //this is set when the mouse is over this widget
        bool active;
        //if the widget has been changed by user input
        bool changed;
        //the height of the widget (needed by the module to
        //set the drawing position of relative widgets
        int height;
        //set the width 0f the widget
        int width;
        static int defaultWidth;

        //set a static font size
        static float defaultFontSize;
        static float buttonFontSize;

        //the spacing that should be used between things in the ui
        static int MARGIN;

		static double highlight_r;
		static double highlight_g;
		static double highlight_b;
		static double font_r;
		static double font_g;
		static double font_b;
		static double button_r;
		static double button_g;
		static double button_b;
		static double active_button_r;
		static double active_button_g;
		static double active_button_b;


        string name;

        void setGLForText(){
			glLineWidth(1);
			glColor4f(font_r,font_g,font_b,1);
        }

        void setGLForButton(){
			glColor4f(button_r,button_g,button_b,1);
        }

        void setGLForActiveButton(){
			glColor4f(active_button_r,active_button_g,active_button_b,1);
        }



    public:

        IndUIWidget() {
            enabled = true;
            active = false;
            changed = false;
            //width = defaultWidth;
            name = "";
        }

        /*create a virtual draw function that accepts the offsets
        of where to draw everything.*/
        virtual void Draw(int offsetX,int offsetY) = 0;

        /*the virtual function tasks the implimenter with doing whatever
        it needs to do when the mouse is over it. If a slider needs to be
        dragged or a number needs to be increased or text needs to be
        modified, etc. it should be done by the widget.

        If the widget's update function returns true, the module will
        automatically keep updating the widget until it returns false.*/
        virtual bool update(int mouseX,int mouseY,int **mouseClick,bool* &keyboard) = 0;

        /*sets the internal value of the widget. This is why widget is a
        template class. It needs to know what type of widget any inheriting
        classes are, so that the value of any widget can be set without knowing
        its type. It hould return bool if setting the value worked and false if it
        didn't*/
        //template <typename T> bool SetReference(T value) {cout << "default template setreference\n";}
        virtual bool SetReference(void* value ) { cout << "SetReference not overridden\n";return false;}

		/*lets a widget set a callback function that is called when the
		value is updated*/

        virtual void SetWidth(int width) {
            this->width = width;
        }

        int Height(){return height;}

		int Width(){return width;}

        string Name(){return name;}

        virtual bool Active(){return active;}

		virtual bool hasChanged() = 0;

		static int getDefaultFontSize(){ return defaultFontSize; }
		static int getMargin(){ return MARGIN; }
};


int IndUIWidget::defaultWidth = 200;
float IndUIWidget::defaultFontSize = 16;
float IndUIWidget::buttonFontSize = 12;
int IndUIWidget::MARGIN = 4;
double IndUIWidget::highlight_r = .5f;
double IndUIWidget::highlight_g = .7;
double IndUIWidget::highlight_b = 1;
double IndUIWidget::font_r = 1;
double IndUIWidget::font_g = 1;
double IndUIWidget::font_b = 1;
double IndUIWidget::button_r = .1f;
double IndUIWidget::button_g = .3f;
double IndUIWidget::button_b = .45f;
double IndUIWidget::active_button_r = .05f;
double IndUIWidget::active_button_g = .15f;
double IndUIWidget::active_button_b = .225f;
#endif

