#ifndef ENGINE_H


#define INPUT_OUTPUT false
#define EVENT_OUTPUT true
#define DEBUG_DELETES true
#define DEBUG_ASSOCIATIONS false
#define DEBUG_INPUT false
#define DEBUG_LOAD true
#define DEBUG_UI false
#define DEBUG_MESSAGES false

#include <iostream>
using namespace std;

#include "core/window.h"
#include "core/asset_library.h"
#include "core/node.h"
#include "physics/ode.h"
#include "core/engine_setup.h"
#include "ui/indui.h"
#include "core/timer.h"
#include "core/engine_control.h"

/*controls depend on the engine setup utility*/
#include "control/loading_screen.h"
//#include "control/menu_screen.h"
#include "control/fps.h"
//#include "control/fpse.h"
//#include "control/race.h"
//#include "control/dataviewer.h"




/**\brief The engine class is the foundation that everything is added to.

It keeps track of assets, builds the window, runs the simulation, captures user
input, etc. It also acts as a container to hide all of the nitty gritty from anyone
using it.

A class of the type engine_control can be set as the default state of the engine.
From there different engine controls can be used to swap between different modes in
the engine. There are several engine controls that come with the default installation
of engine. Things like a menu, loading screen, first person shooter, etc..
**/
class engine : public node {
private:
	/*basic */
	list<engine_control*> backStates;

	timer *time;

	/*major parts of the engine*/
	window *mainWindow;
	varMap settings;
	bool exit_flag;

	int breakfree;

	string currentStateName;
	string currentSettingsFile;

	engine_control *previousControl;
	engine_control *currentControl;

	void setCurrentControl(engine_control* newControl) {
		currentControl = newControl;
		currentControl->setParent(this);
		/*load views into window*/
		list<view*> controlViews = newControl->getViews();
		cout << " - new control has " << controlViews.getCount() << " views\n";
		for(int i=0;i<controlViews.getCount();i++){
			mainWindow->addView(controlViews[i]);
		}
	}

	void setDefaultOpenGLSettings() {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		quaternion lightAngle = quaternion::left();
		GLfloat lightpos[] = {0, 1, 1, 0};
		glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
		glPointSize(5);

	}

	/**\brief Much like the setCurrentState() function, this function
	takes the current state and replaces it with a new state...

	Except in this case, the new state is a fully populated
	state class, so no work needs to be done other than telling
	the engine to use the new state class.**/
	bool setTempControl(engine_control *control){
		if(previousControl) return false;
		previousControl = currentControl;
		currentControl = control;
		return true;
	}

	bool unsetTempControl(){
		if(!previousControl) return false;
		currentControl = previousControl;
		previousControl = NULL;
		return true;
	}

	/*using the config file settings, the engine builds the current
	state and any objects required by it*/
	bool setCurrentState(string stateName) {
		engine::currentStateName = stateName;
		/*if statename is "exit", set the exit flag to true and
		return from the function.*/
		if(engine::currentStateName == "exit") return true;
		/*if a current control is set, dereference it and delete
		it from the engine.*/
		if(currentControl){
			currentControl->setParent(NULL);
			delete currentControl;
			currentControl = NULL;
		}
		/*get the default state settings, if none are found, return false*/
		varMap currentStateSettings = settings.getGroup(engine::currentStateName);
		if(currentStateSettings.getCount() < 1){
			cout << "failed to load default engine state : " << engine::currentStateName << "\n";
			return false;
		}
		if(!currentStateSettings.isSet("control.type")) {
			cout << "no control type set, engine init exiting.\n";
			return false;
		}
		/*set up the default engine control. This is what the user will see
		when the engine initially loads.*/
		string controlType = currentStateSettings.get<string>("control.type");

		engine_control* new_control = assetLibrary::load<engine_control>(
			string("include/assets/library/engine.control."+controlType+".dll"),
			new varMap()
		);
		setCurrentControl(new_control);
		return true;
	}

	/**\brief Adds the current state to a stack of available states.

	This is a dumb function that basically allows the programmer to push a
	new state on top of the current one. The current one loses focus and is
	not updated or drawn, but when the *new* current state is popped, the old
	current state returns to the forefront of the engine. This is nice if you
	want to pause a state and pop up a menu or do something else.*/
	bool pushState(string stateName){
		if(currentControl){
			backStates.push(currentControl);
			currentControl = NULL;
		}
		return setCurrentState(stateName);
	}

	/**\brief Removes the current state from the stack of states.

	This allows you to return from a state that has been pushed onto the
	state stack. If there are no states left to pop, false is returned,
	otherwise the last state on the stack is removed from the stack and
	set as the current state.

	If nothing else is referencing the current state that is being replaced
	from the stack, it will be lost forever.*/
	bool popState(){
		if(currentControl == backStates.last()){
			backStates.pop();
		}
		if(!backStates.getCount()) return false;

		return (currentControl = backStates.last());
	}

public:

	engine() :
			backStates(list<engine_control*>()),
			mainWindow(NULL),
			settings(varMap()),
			exit_flag(false),
			currentStateName("default"),
			currentSettingsFile("include/assets/settings/default.est"),
			previousControl(NULL),
			currentControl(NULL) {
		setName(new string("engine_" + numberToString<unsigned long>(getId())));
		breakfree = 0;
	}

	~engine(){
		#if DEBUG_DELETES
			cout << " engine >";
		#endif
		assetLibrary::destroy();
	}

	bool init() {
		node::root = this;
		time = new timer();
		/*check to see if any settings have been set, if not,
		attempt to load the default settings. If not settings are set
		and no default settings can be loaded, return false.*/
		if(!settings.isLoaded() && !varMap::load(currentSettingsFile,settings)) return false;
		/*set the static settings for the loader class*/
		/*
		if(settings.isSet("default.directories.textures")){
			loader::settings.add<string>(
				new string(settings.get<string>("default.directories.textures")),"default.directories.textures");
		} else {
			loader::settings.add<string>( new string("include/assets/textures/"),"default.directories.textures");
		}
		if(settings.isSet("default.directories.models")){
			loader::settings.add<string>(
				new string(settings.get<string>("default.directories.models")),"default.directories.models");
		} else {
			loader::settings.add<string>( new string("include/assets/textures/"),"default.directories.models");
		}*/
		/*load the default window settings from the config file. If a
		window cannot be build, the init function returns false.*/
		varMap windowSettings = settings.getGroup("windows.default");
		if(windowSettings.getCount() > 0) {
			cout << "build window\n";
			if(!setup::newWindow(windowSettings,mainWindow)) {
				cout << "failed to set up default window\n";
				return false;
			}
		}
		setDefaultOpenGLSettings();
		/*store windows properties in the node::windowProperties struct
		so that it can be used by object that need information about the
		size of the window like IndUI.*/
		node::windowProperties.width = &mainWindow->width;
		node::windowProperties.height = &mainWindow->height;
		input::init();
		if(!setCurrentState(engine::currentStateName)){
			cout << " - couldn't set a new engine state\n";
			return false;
		}
		if(!currentControl){
			cout << " - new control failed to build\n";
			return false;
		}
		addChild(currentControl);
		return true;
	}

	void update(){
		if(time->tick()){
			currentControl->update();
			handleMessages();
			++breakfree;
		}
		currentControl->windowDraw();
	}

	bool run() {
		while(!exit_flag && mainWindow->isOpen() && currentControl && breakfree < 100) {

			update();
		}
		return true;
	}

	/**\brief handles all of the messages that are applicable to the
	engine class.

	MESSAGE_ENGINE_SET_STATE : allows another node to send a new engine
	control to be switched to.

	MESSAGE_ENGINE_SET_TEMP_CONTROLLER : allows another node to send a
	engine control to temporarily switch to. This allows the state of
	the current control to be saved while a new controller is used.

	MESSAGE_ENGINE_UNSET_TEMP_CONTROLLER : tells the engine to switch
	back to the controller that was running before MESSAGE_ENGINE_SET_TEMP_CONTROLLER
	was called.

	MESSAGE_ENGINE_EXIT : tells the engine that the executable should
	end. The user has exited the game, something has happened that it
	should no longer be running.
	**/
	bool handleMessage(node_message* m){
		cout << "engine recieved message\n";
		switch(m->code){
		case MESSAGE_ENGINE_SET_STATE:
			cout << "engine: change state\n";
			setCurrentState(*((string*) m->data));
			return true;
		case MESSAGE_ENGINE_SET_TEMP_CONTROLLER:
			cout << "engine: set temp controller\n";
			return setTempControl((engine_control*) m->data);
		case MESSAGE_ENGINE_UNSET_TEMP_CONTROLLER:
			cout << "engine: unset temp controller\n";
			return unsetTempControl();
		case MESSAGE_ENGINE_EXIT:

			exit_flag = true;
			return true;
		}
		return node::handleMessage(m);
	}

	window* getWindow(){ return mainWindow; }

};


#define ENGINE_H
#endif // ENGINE_H
