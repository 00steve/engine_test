

#ifndef GLVECTORTEXT_H
#define GLVECTORTEXT_H



class GLVectorText{
    private:
        //the origin of where the text should start drawing from
        float offsetX,offsetY;

        //the alignment of the text 1 = left align, 0 = centered, -1 = right aligned
        int alignment;

        //the text that should be drawn, stores a copy of the string being referenced
        string* &text;

		//if a background polygon should be drawn behind the text
		bool solidBackground;


        //store the default font size of the vector text so it can keep track of it
        //on its own.
        static float defaultFontSize;

        //store the width of the string
        float width;
        float maxWidth;
        //where to draw the current character from
        float drawX;
        float drawY;
        //it keeps track of the current char being drawn
        unsigned int cursor;

        float size;

    public:
        /*create a constructor passing a string pointer to it.*/
        GLVectorText(string* &text) : text(text)
        {
            //store the pointer in the class
            //this->text = text;
            //set default properties of the text
            alignment = 1;
            //set default offset
            offsetX = 0;
            offsetY = 0;
            //set the size to the default size
            size = defaultFontSize;
            width = text->length()*size*.6f;
        }

        GLVectorText(string* &text,float fontSize) : text(text)
        {
            //store the pointer in the class
            //this->text = text;
            //set default properties of the text
            alignment = 1;
            //set default offset
            offsetX = 0;
            offsetY = 0;
            //set the font size to the size that was passed in
            size = fontSize;
        }

        GLVectorText(string* &text,float fontSize,float maxWidth) : text(text)
        {
            //store the pointer in the class
            //this->text = text;
            //set default properties of the text
            alignment = 1;
            //set default offset
            this->maxWidth = maxWidth;
            //set the font size to the size that was passed in
            size = fontSize;
        }

        GLVectorText(string* &text,float fontSize,float offsetX,float offsetY) : text(text)
        {
            //store the pointer in the class
            //this->text = text;
            //set default properties of the text
            alignment = 1;
            //set default offset
            this->offsetX = offsetX;
            this->offsetY = offsetY;
            //set the font size to the size that was passed in
            size = fontSize;
        }





        /*draws text on a 2d plane. The unit of measurements sent to it should
        be pixels, using an isometric view set to the pixel size of the drawing
        window.*/
        void Draw()
        {
            //find total width of the text
            width = text->length()*size*.6f;
            //find where to start drawing the text
            drawX = offsetX + (width*alignment*.5f) - (width *.5f);
            drawY = offsetY;
            //reset the cursor to the first char
            cursor = 0;
            float w = size*.5f;
            float h = size;


            //glLineWidth(size*.12f);
            while(cursor < text->length()) {
				switch((*text)[cursor]) {
				case '0':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
					glEnd();
					break;
				case '1':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.3f*h+drawY);
						glVertex2f(.5f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
					glEnd();
				   glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case '2':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.3f*h+drawY);
						glVertex2f(.3f*w+drawX,-.1f*h+drawY);
						glVertex2f(.7f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.25f*h+drawY);
						glVertex2f(.9f*w+drawX,-.4f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case '3':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.7f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.3f*h+drawY);
						glVertex2f(.9f*w+drawX,-.7f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.2f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case '4':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.2f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case '5':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.6f*h+drawY);
						glVertex2f(.9f*w+drawX,-.7f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case '6':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.3f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.2f*h+drawY);
						glVertex2f(.1f*w+drawX,-.4f*h+drawY);
						glVertex2f(.1f*w+drawX,-.8f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.8f*h+drawY);
						glVertex2f(.9f*w+drawX,-.6f*h+drawY);
						glVertex2f(.8f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case '7':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.2f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case '8':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case '9':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.8f*h+drawY);
						glVertex2f(.9f*w+drawX,-.6f*h+drawY);
						glVertex2f(.9f*w+drawX,-.2f*h+drawY);
						glVertex2f(.7f*w+drawX,-.1f*h+drawY);
						glVertex2f(.3f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.2f*h+drawY);
						glVertex2f(.1f*w+drawX,-.4f*h+drawY);
						glVertex2f(.2f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;

				case 'A':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.5f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.8f*w+drawX,-.6f*h+drawY);
						glVertex2f(.2f*w+drawX,-.6f*h+drawY);
					glEnd();
					break;
				case 'B':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.7f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.25f*h+drawY);
						glVertex2f(.9f*w+drawX,-.35f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case 'C':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.35f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.35f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.35f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'D':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.65f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.35f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.65f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
					glEnd();
					break;
				case 'E':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case 'F':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case 'G':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.3f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.3f*h+drawY);
						glVertex2f(.1f*w+drawX,-.7f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
						glVertex2f(.5f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case 'H':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case 'I':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'J':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.5f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.75f*h+drawY);
						glVertex2f(.35f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'K':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'L':
					glBegin(GL_LINES);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'M':
					glBegin(GL_LINES);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.2f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.7f*h+drawY);
						glVertex2f(.8f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'N':
					glBegin(GL_LINES);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
					glEnd();
					break;
				case 'O':
					glBegin(GL_LINES);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.35f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.3f*h+drawY);
						glVertex2f(.1f*w+drawX,-.7f*h+drawY);
						glVertex2f(.35f*w+drawX,-.9f*h+drawY);
						glVertex2f(.65f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.7f*h+drawY);
						glVertex2f(.9f*w+drawX,-.3f*h+drawY);
						glVertex2f(.65f*w+drawX,-.1f*h+drawY);
						glVertex2f(.35f*w+drawX,-.1f*h+drawY);
					glEnd();
					break;
				case 'P':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.7f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.25f*h+drawY);
						glVertex2f(.9f*w+drawX,-.35f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case 'Q':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.35f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.3f*h+drawY);
						glVertex2f(.1f*w+drawX,-.7f*h+drawY);
						glVertex2f(.35f*w+drawX,-.9f*h+drawY);
						glVertex2f(.65f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.7f*h+drawY);
						glVertex2f(.9f*w+drawX,-.3f*h+drawY);
						glVertex2f(.65f*w+drawX,-.1f*h+drawY);
						glVertex2f(.35f*w+drawX,-.1f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.5f*w+drawX,-.6f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'R':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.7f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.25f*h+drawY);
						glVertex2f(.9f*w+drawX,-.35f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.4f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'S':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.3f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.25f*h+drawY);
						glVertex2f(.1f*w+drawX,-.35f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'T':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'U':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.3f*h+drawY);
						glVertex2f(.1f*w+drawX,-.7f*h+drawY);
						glVertex2f(.35f*w+drawX,-.9f*h+drawY);
						glVertex2f(.65f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.7f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
					glEnd();
					break;
				case 'V':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
					glEnd();
					break;
				case 'W':
					glBegin(GL_LINES);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.2f*w+drawX,-.9f*h+drawY);
						glVertex2f(.5f*w+drawX,-.3f*h+drawY);
						glVertex2f(.8f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
					glEnd();
					break;
				case 'X':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'Y':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.5f*w+drawX,-.5f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'Z':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'a':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.45f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.95f*w+drawX,-.95f*h+drawY);
					glEnd();
					break;
				case 'b':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.05f*w+drawX,-.95f*h+drawY);
					glEnd();
					break;
				case 'c':
					glBegin(GL_LINE_STRIP);

						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case 'd':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.95f*w+drawX,-.95f*h+drawY);
					glEnd();
					break;
				case 'e':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.9f*w+drawX,-.7f*h+drawY);
						glVertex2f(.1f*w+drawX,-.7f*h+drawY);
					glEnd();
					break;
				case 'f':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
						glVertex2f(.5f*w+drawX,-.25f*h+drawY);
						glVertex2f(.65f*w+drawX,-.15f*h+drawY);
						glVertex2f(.75f*w+drawX,-.1f*h+drawY);
						glVertex2f(.9f*w+drawX,-.1f*h+drawY);
					glEnd();
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case 'g':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.45f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.95f*h+drawY);
						glVertex2f(.7f*w+drawX,-1.1f*h+drawY);
						glVertex2f(.3f*w+drawX,-1.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-1.0f*h+drawY);
					glEnd();
					break;
				case 'h':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'i':
					glBegin(GL_LINES);
						glVertex2f(.5f*w+drawX,-.5f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.55f*w+drawX,-.4f*h+drawY);
						glVertex2f(.45f*w+drawX,-.4f*h+drawY);
						glVertex2f(.45f*w+drawX,-.3f*h+drawY);
						glVertex2f(.55f*w+drawX,-.3f*h+drawY);
						glVertex2f(.55f*w+drawX,-.4f*h+drawY);
					glEnd();
					break;
				case 'j':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.95f*h+drawY);
						glVertex2f(.5f*w+drawX,-1.1f*h+drawY);
						glVertex2f(.3f*w+drawX,-1.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-1.0f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.65f*w+drawX,-.4f*h+drawY);
						glVertex2f(.75f*w+drawX,-.4f*h+drawY);
						glVertex2f(.75f*w+drawX,-.3f*h+drawY);
						glVertex2f(.65f*w+drawX,-.3f*h+drawY);
						glVertex2f(.65f*w+drawX,-.4f*h+drawY);
					glEnd();
					break;
				case 'k':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.8f*w+drawX,-.35f*h+drawY);
						glVertex2f(.1f*w+drawX,-.6f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'l':
					glBegin(GL_LINES);
						glVertex2f(.5f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'm':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.6f*h+drawY);
						glVertex2f(.2f*w+drawX,-.5f*h+drawY);
						glVertex2f(.4f*w+drawX,-.5f*h+drawY);
						glVertex2f(.5f*w+drawX,-.6f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.5f*w+drawX,-.6f*h+drawY);
						glVertex2f(.6f*w+drawX,-.5f*h+drawY);
						glVertex2f(.8f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.6f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);

					glEnd();
					break;
				case 'n':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.45f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.7f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.65f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.7f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;
				case 'o':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
					glEnd();
					break;

				case 'p':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.45f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.05f*w+drawX,-1.1f*h+drawY);
					glEnd();
					break;

				case 'q':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.45f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.1f*w+drawX,-.65f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.65f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-1.1f*h+drawY);
					glEnd();
					break;
				case 'r':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.45f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.7f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.65f*w+drawX,-.5f*h+drawY);
						glVertex2f(.8f*w+drawX,-.6f*h+drawY);
					glEnd();
					break;
				case 's':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
						glVertex2f(.3f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.55f*h+drawY);
						glVertex2f(.1f*w+drawX,-.6f*h+drawY);
						glVertex2f(.3f*w+drawX,-.7f*h+drawY);
						glVertex2f(.7f*w+drawX,-.7f*h+drawY);
						glVertex2f(.9f*w+drawX,-.8f*h+drawY);
						glVertex2f(.9f*w+drawX,-.85f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);

					glEnd();
					break;

				case 't':
					glBegin(GL_LINES);
						glVertex2f(.5f*w+drawX,-.1f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;

				case 'u':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.75f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.75f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;

				case 'v':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.5f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case 'w':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.3f*w+drawX,-.9f*h+drawY);
						glVertex2f(.5f*w+drawX,-.5f*h+drawY);
						glVertex2f(.7f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);

					glEnd();
					break;
				case 'x':
					glBegin(GL_LINES);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
					glEnd();
					break;

				case 'y':
				  glBegin(GL_LINE_STRIP);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
					glEnd();
					glBegin(GL_LINE_STRIP);
						glVertex2f(.9f*w+drawX,-.9f*h+drawY);
						glVertex2f(.9f*w+drawX,-1.0f*h+drawY);
						glVertex2f(.8f*w+drawX,-1.1f*h+drawY);
						glVertex2f(.1f*w+drawX,-1.1f*h+drawY);
					glEnd();
					break;

				case '-':
					glBegin(GL_LINES);
						glVertex2f(.9f*w+drawX,-.5f*h+drawY);
						glVertex2f(.1f*w+drawX,-.5f*h+drawY);
					glEnd();
					break;
				case '.':
					glBegin(GL_LINE_STRIP);
						glVertex2f(.4f*w+drawX,-.93f*h+drawY);
						glVertex2f(.5f*w+drawX,-.93f*h+drawY);
						glVertex2f(.5f*w+drawX,-.8f*h+drawY);
						glVertex2f(.4f*w+drawX,-.8f*h+drawY);
						glVertex2f(.4f*w+drawX,-.93f*h+drawY);
					glEnd();
					break;

				case ' ':
					break;
				default:
					glBegin(GL_LINES);
						glVertex2f(.4f*w+drawX,-.4f*h+drawY);
						glVertex2f(.6f*w+drawX,-.6f*h+drawY);
						glVertex2f(.6f*w+drawX,-.4f*h+drawY);
						glVertex2f(.4f*w+drawX,-.6f*h+drawY);
						glVertex2f(.4f*w+drawX,-.6f*h+drawY);
					glEnd();
			}//end of switching between different characters

			//increment the cursor
			++cursor;
			//add width of cursor to draw x
			drawX += w + w*.2f;
            }//end of looping through each character
        }

        /*calls the draw function, but adjusts adds an offset to the text before
        drawing it.*/
        void OffsetDraw(float x,float y)
        {
            float oX = offsetX;
            float oY = offsetY;
            offsetX += x;
            offsetY += y;
            Draw();
            offsetX = oX;
            offsetY = oY;
        }

        /*return the width of each character*/
        float CharacterWidth()
        {
            return size*.5f;
        }
        /*return the amount of spacing between each character*/
        float CharacterSpacing()
        {
            return size*.1f;
        }

		void setAlignment(int align){
			alignment = align;
		}

		float getWidth(){
			return width;
		}

		float getHeight(){
			return size;
		}
};

//set the default font size to 12 (pixels)
float GLVectorText::defaultFontSize = .3;
#endif
