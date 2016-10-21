#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include "../core/engine_control.h"
#include "../graphics/glvectortext.h"

/*include freetype headers for testing to see
what type of kick ass shit I can get going.*/
//#include <zlib.h>
//#include <ft2build.h>
//#include FT_FREETYPE_H



class menuScreen : public engine_control{
private:

	list<string*> optionNames;
	list<GLVectorText*> optionTexts;

public:

	menuScreen(varMap* settings) :
		engine_control(settings),
		optionNames(list<string*>()),
		optionTexts(list<GLVectorText*>()){

		/*get menu items*/
		varMap optionsGroup = controlSettings->getGroup("options");
		varMap optionGroup;
		list<string> optionGroupNames = optionsGroup.groupNames();
		cout << optionGroupNames.getCount() << " group names\n";
		string cur_label;
		while(optionGroupNames.getCount()){
			cur_label = optionGroupNames.pop();
			optionGroup = optionsGroup.getGroup(cur_label);
			if(optionGroup.isSet("label")){
				cur_label = optionGroup.get<string>("label");
			}
			optionNames.push(new string(cur_label));
			optionTexts.push(new GLVectorText(optionNames[optionNames.getCount()-1]));
		}
	}

	void draw(){

		glLineWidth(4);

		double3 pos = double3(0,0,0);
		double3 offset = double3(0,-.4,0);

		for(int i=0;i<optionTexts.getCount();i++){
			glColor3d(1,1,1);
			optionTexts[i]->OffsetDraw(pos.x,pos.y);
			glColor3d(0.25,0,0);
			glBegin(GL_TRIANGLE_FAN);
				glVertex3d(pos.x,pos.y,-.1);
				glVertex3d(pos.x+optionTexts[i]->getWidth(),pos.y,-.1);
				glVertex3d(pos.x+optionTexts[i]->getWidth(),pos.y-optionTexts[i]->getHeight(),-.1);
				glVertex3d(pos.x,pos.y-optionTexts[i]->getHeight(),-.1);
			glEnd();
			pos += offset;
		}

	}

	void update(){

	}

};



#endif // MENU_SCREEN_H
