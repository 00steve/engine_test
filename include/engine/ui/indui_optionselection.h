#ifndef INDUI_OPTION_SELECTION_H
#define INDUI_OPTION_SELECTION_H


class optionSelection : public IndUIWidget{

private:

	list<optionSelection*> children;
	void* data;

	string *label;
	GLVectorText *glLabel;

	int childrenOffsetX;

	int childOver;

	int height;

	bool clicking;



public:

	optionSelection(string* label) :
		children(list<optionSelection*>()),
		data(NULL),
		label(label),
		childrenOffsetX(0),
		childOver(-1),
		height(defaultFontSize+MARGIN*2),
		clicking(false){
		glLabel = new GLVectorText(this->label,defaultFontSize);
		glLabel->Draw();
	}

	optionSelection* add(optionSelection* newChild){
		children.push(newChild);
		calculateWidth();
		return newChild;
	}



	optionSelection* get(string label){
		int i = children.getCount();
		while( i --> 0){
			if(*children[i]->label != label) continue;
			return children[i];
		}
		return NULL;
	}

	bool update(int cursorX, int cursorY,int **click,bool* &keyboard){
		int cC = ((cursorY-MARGIN) / (height+MARGIN)) -1;

		if(cC > -1 && cC < children.getCount()){
			childOver = cC;

			if(!clicking && (*click[0])){
				clicking = true;
				children[cC]->onClick();
				#if DEBUG_UI
				cout << "Clicked child : " << cC << endl;
				#endif
			}
		} else {
			childOver = -1;
		}

		if(!(*click[0])){
			clicking = false;
		}

		return false;
	}

	void Draw(int offsetX,int offsetY){

		glLineWidth(1);

		setGLForText();
		glLabel->OffsetDraw(offsetX+MARGIN*2,offsetY-MARGIN*2);

		int i = 0;
		int cOffsetY = offsetY;
		while( i < children.getCount()){
			cOffsetY -= height+MARGIN;
			children[i]->Draw(offsetX,cOffsetY);
			++i;
		}

		if(!i) return;

		if(childOver > -1){
			setGLForButton();
			glBegin(GL_TRIANGLE_FAN);
				glVertex3i(offsetX,offsetY-(height+MARGIN)*(childOver+1)-MARGIN,0);
				glVertex3i(offsetX,offsetY-(height+MARGIN)*(childOver+2),0);
				glVertex3i(offsetX+width,offsetY-(height+MARGIN)*(childOver+2),0);
				glVertex3i(offsetX+width,offsetY-(height+MARGIN)*(childOver+1)-MARGIN,0);
			glEnd();
		}

		setGLForActiveButton();
		glBegin(GL_TRIANGLE_FAN);
			glVertex3i(offsetX,offsetY,0);
			glVertex3i(offsetX,cOffsetY-height-MARGIN*2,0);
			glVertex3i(offsetX+width,cOffsetY-height-MARGIN*2,0);
			glVertex3i(offsetX+width,offsetY,0);
		glEnd();

	}

	int childCount(){ return children.getCount(); }

	bool hasChanged(){ return false; }

	GLVectorText* getLabel(){ return glLabel; }

	virtual bool onClick(){
		return false;
	}

	int calculateWidth(){
		int winW = 0;
		int i = children.getCount();
		int labW;
		while( i --> 0 ){
			//children[i]->getLabel()->Draw();
			labW = children[i]->getLabel()->getWidth();
			if(labW > winW) winW = labW;
		}
		width = winW + MARGIN*4;
		return width;
	}
};


#endif // INDUI_OPTION_SELECTION_H
