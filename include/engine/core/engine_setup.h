#ifndef ENGINE_SETUP_H
#define ENGINE_SETUP_H

#include "var_map.h"


#include "view.h"
#include "event.h"
#include "action.h"
#include "actions/send_message.h"
#include "camera.h"
//#include "../graphics/floating_camera.h"
//#include "../graphics/fpse_camera.h"
//#include "../graphics/attached_camera.h"
//#include "../graphics/fps_camera.h"
#include "../graphics/shader.h"

/**\brief the engine setup header is a static class that builds defined
engine objects based on parameters being passed in in the form
of the varMap class.

All of the functions should have the same basic parameters and
return types.
 - return true if the function is successful
 - return false if the function is full of fail
 - first argument should be a varMap with the settings used to build the object
 - second argument should be a pointer to the type that is being created
*/
class setup{
public:

	static double3 getDouble3(varMap settings,string name,double3 defaultValues){
			//check that string is set
		if(!settings.isSet(name)){
			return defaultValues;
		}
		string str = settings.get<string>(name);
		string tok;
		int i = 0;
		string needle = ",";
		//remove all spaces from string
		str = removeChar(str,' ');
		while(stringCutNext(str,tok,needle) && i < 3){
			if(!stringIsNumber(tok)){
				cout << "is not number\n";
				return defaultValues;
			}
			switch(i){
				case 0: defaultValues.x = stringToNumber<double>(tok); break;
				case 1: defaultValues.y = stringToNumber<double>(tok); break;
				case 2: defaultValues.z = stringToNumber<double>(tok); break;
			}
			++i;
		}
		return defaultValues;
	}
	static double2 getDouble2(varMap settings,string postfix,double2 defaultValues){
		return double2(
			(settings.isSet("x-"+postfix) ? settings.get<double>("x-"+postfix) : defaultValues.x),
			(settings.isSet("y-"+postfix) ? settings.get<double>("y-"+postfix) : defaultValues.y)
		);
	}
	static double getDouble(varMap settings,string name,double defaultValue){
		return double((settings.isSet(name) ? settings.get<double>(name) : defaultValue));
	}

	static double getInt(varMap settings,string name,int defaultValue){
		return int((settings.isSet(name) ? settings.get<double>(name) : defaultValue));
	}

	static string getString(varMap settings,string name,string defaultValue){
		return string((settings.isSet(name) ? settings.get<string>(name) : defaultValue));
	}
	static double3 getColor(varMap settings,string name,double3 defaultValues,double &alpha,bool &hasAlpha){
		//check that string is set
		if(!settings.isSet(name)){
			hasAlpha = false;
			return defaultValues;
		}
		string str = settings.get<string>(name);
		string tok;
		int i = 0;
		string needle = ",";
		//remove all spaces from string
		str = removeChar(str,' ');
		while(stringCutNext(str,tok,needle) && i < 3){
			if(!stringIsNumber(tok)){
				hasAlpha = false;
				return defaultValues;
			}

			switch(i){
				case 0: defaultValues.x = stringToNumber<double>(tok); break;
				case 1: defaultValues.y = stringToNumber<double>(tok); break;
				case 2: defaultValues.z = stringToNumber<double>(tok); break;
			}
			++i;
		}
	}
	static bool getBool(varMap settings,string name,bool defaultValue){
		return (bool)((settings.isSet(name) ? settings.get<bool>(name) : defaultValue));
	}


	static bool addActionsToEvent(node* object,varMap settings,event* eventPtr){
		if(settings.isSet("current-state")){
			eventPtr->performAction(
				new send_message(object,node::root,MESSAGE_ENGINE_SET_STATE,(void*)new string(settings.get<string>("current-state")))
			);
			cout << " - add action: current-state = " << settings.get<string>("current-state") << endl;
		}
		return true;
	}

	/**\brief checks the settings for any of the default event types. These can
	always be added to, but the current list is:

	 - on-context
	 - on-done
	 - on-select
	 - on-window-focus-lost
	**/
	static bool newEvents(node* object,varMap settings,map<event*> &eventsPtr){

		list<string> eventNames = settings.groupNames();
		int eventCount = 0;

		for(int i=0;i<eventNames.getCount();i++){
			/*attempt to load all of the properties that have been
			set for the onclick event.*/
			if(eventNames[i] == "on-select"){
				eventsPtr.push(new event(EVENT_ON_SELECT),"on-select");
				cout << "ADDED EVENT[on-select]\n";
				++eventCount;
				continue;
			}

			/*attempt to load all of the properties that have been
			set for the ondone event.*/
			if(eventNames[i] == "on-done"){
				eventsPtr.push(new event(EVENT_ON_DONE),"on-done");
				cout << "ADDED EVENT[on-done]\n";
				addActionsToEvent(object,settings.getGroup("on-done"),eventsPtr["on-done"]);
				++eventCount;
				continue;
			}
		}
		return eventCount > 0;
	}

	/*
	static bool newCamera(varMap settings,camera* &cameraPtr){
		string type = getString(settings,"type","floating");
		if(type == "floating"){
			cameraPtr = new floating_camera(settings);
		} else if(type == "fpse"){
			cameraPtr = new fpse_camera(settings);
		} else if(type == "attached"){
			cameraPtr = new attached_camera(settings);
		} else if(type == "fps"){
			cameraPtr = new fps_camera(settings);
		} else {
			cameraPtr = NULL;
			cout << "\"" << type << "\" is not a valid camera type. FAILED TO LOAD.\n";
			return false;
		}
		cameraPtr->setFieldOfView(getDouble(settings,"fov",90));
		//set up a controller
		if(settings.isSet("controller")){
			cameraPtr->setController(assetLibrary::getController(settings.get<string>("controller")));
		}

		return true;
	}*/

	static bool newView(string viewname,varMap settings,view* &viewPtr){
		double2 offset = getDouble2(settings,"offset",double2(0,0));

		double2 dimensions = double2(
			(settings.isSet("width") ? settings.get<double>("width") : 1),
			(settings.isSet("height") ? settings.get<double>("height") : 1)
		);

		string sizeMethodName = getString(settings,"size-method","VIEW_SIZEBY_SCALE");
		int sizeMethod;
		if(sizeMethodName == "VIEW_SIZEBY_PIXEL")		sizeMethod = 0;
		else if(sizeMethodName == "VIEW_SIZEBY_SCALE")	sizeMethod = 1;
		else return false;

		string locationName = getString(settings,"location","CENTER");
		int location;
			 if(locationName == "TOP_LEFT")		location = 0;
		else if(locationName == "TOP")			location = 1;
		else if(locationName == "TOP_RIGHT")	location = 2;
		else if(locationName == "LEFT")			location = 3;
		else if(locationName == "CENTER")		location = 4;
		else if(locationName == "RIGHT")		location = 5;
		else if(locationName == "BOTTOM_LEFT")	location = 6;
		else if(locationName == "BOTTOM")		location = 7;
		else if(locationName == "BOTTOM_RIGHT")	location = 8;
		else return false;

		/*build the new view with its initial settings.*/
		viewPtr = new view(viewname,offset,dimensions,sizeMethod,location);

		/*if the view has a default shader set, build that shit*/
		string shaderName = getString(settings,"shader","");
		if(shaderName != ""){
			string fragmentShaderName = "include/assets/shaders/" + shaderName + "_fragment.txt";
			string vertexShaderName = "include/assets/shaders/" + shaderName + "_vertex.txt";
			viewPtr->setShader(new shader(fragmentShaderName,vertexShaderName));
		}

		/*perform extra settings checks that are not required to
		make the view work, but are nice to have sometimes if you
		are cool and stuff. (fuck)*/
		viewPtr->setPreferredCameraName(getString(settings,"camera","N/A"));
		return true;
	}

	static bool newWindow(varMap settings,window* &windowPtr){
		int width = (int) getDouble(settings,"width",800);
		int height = (int) getDouble(settings,"height",600);
		string typeName = getString(settings,"type","WINDOW");
		GLuint type;
		if(typeName == "FULLSCREEN")	type = GLFW_FULLSCREEN;
		else if(typeName == "WINDOW")	type = GLFW_WINDOW;
		else return false;

		windowPtr = new window(width,height,type);
		return true;
	}

	static bool newAsset(varMap settings,void* &object){
		return false;
	}

	static bool newController(varMap settings,controller* &controllerPtr){

		string type = getString(settings,"type","floating");
		if(type == "keyboard-mouse"){
			controllerPtr = new keyboard_mouse(settings);
		} else {
			return false;
		}

		return true;
	}

	/**\brief sets a bunch of settings in ODE depending on what is
	set in the physics part of the properties**/
	static bool physics(varMap* settings){
		if(!settings){
			settings = new varMap();
		}
		//ode::setAutoDisable(getBool(*settings,"auto-disable",true));
		return true;
	}


};



#endif // ENGINE_SETUP_H
