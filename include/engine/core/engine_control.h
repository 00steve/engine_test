
#ifndef ENGINE_CONTROL_H
#define ENGINE_CONTROL_H

#include <iostream>
#include <fstream>
using namespace std;
#include "window.h"

#include "view.h"
#include "camera.h"
#include "loader.h"
#include "../load/obj.h"
#include "../graphics/drawable.h"
#include "../core/asset_library.h"
#include "../core/depth_click.h"
#include "../core/engine_setup.h"

/**\brief The base class that can be used as a glorified control loop for the engine.

It automatically loads a bunch of default objects from the config file like controllers,
cameras, views, etc. (in that order).
*/

class engine_control : public drawable{
private:

	/*private variables. Used internally by a control and should not
	be messed with by any inheriting classes.*/


protected:
	/*based on the settings*/
	varMap* controlSettings;

	/*create a map of the views to store them by
	name so that they can be called by name. Store
	a list that keeps all of them in an index so
	that they can be called in succession without
	having to look up each one.*/
	map<view*> 		views;
	list<view*> 	viewList;

	map<camera*> 		cameras;
	map<mesh*> 			meshes;
	map<controller*>	controllers;

	view* 			activeView;
	camera* 		activeCamera;
	loader* 		activeLoader;

	int 			currentViewIndex;

	/**\brief get the click where the cursor is in the screen, combined
	with the active camera and active view to work backwards from 2d screen space to 3d world
	space to tell us where the click is*/
	depth_click* getDepthClick(controller* control) {
		if(!activeView || !activeCamera) return NULL;
		return depth_click::get(control->cursor_position(), activeCamera,
								activeView->getOffset(), activeView->getDimensions(),
								activeView->getAspectRatio());
	}

	/**\brief Goddamn, that's got a long name, doesn't it? Either way,
	it is verbose so you know exactly what the fuck is happening.

	It will set the camera that will be used by any view that A:
	doesn't have a camera set, and B: the camera's name matches
	the name of the camera that is preferred by the current
	view. This should be called after all cameras and all views
	have been loaded. It is pointless and computer saddening to
	run before those things are done because it uses nested loops
	and loops make the cpu sad.*/
	bool setEmptyViewPreferredCameras() {
		currentViewIndex = views.getCount();
		/*set a boolean if the default camera has been set, otherwise this
		the camera.isSet('default') function needs to run every time it is
		checked, which is bullshit. It'll probably need to run more often
		than not, so define that shit here.*/
		bool defaultCamera = cameras.isSet("default");
		if(defaultCamera){
			cout << "Default camera has been added from the config file and shit!\n";
		}
		string wantedCamera;
		while(currentViewIndex --> 0) {
			cout << "check view index : " << currentViewIndex << endl;
			if(viewList[currentViewIndex]->getCamera()) continue;
			wantedCamera = viewList[currentViewIndex]->getPreferredCameraName();
			cout << " -  wants camera : " << wantedCamera << " -> ";
			/*if the view can find the camera that it would prefer, set the
			camera and continue on to the next view.*/
			if(cameras.isSet(wantedCamera)) {
				viewList[currentViewIndex]->setCamera(cameras[wantedCamera]);
				activeCamera = cameras[wantedCamera];
				cout << "got that fucking fuck face\n";
				continue;
			}
			/*if the current view doesn't have a specific camera that it would
			like to use from the config file, use the default camera if it is
			set. If it is not set, return from the function ,because some shit
			has gone wrong.*/
			if(!defaultCamera) {
				cout << "no default camera, the view doesn't know what it should do, ya bitch\n";
				return false;
			}
			/*set the view to use the default camera*/
			viewList[currentViewIndex]->setCamera(cameras["default"]);
			activeCamera = cameras["default"];
			cout << "using default camera\n";
		}//end of looping through the current view
		return true;
	}//end of the set view cameras function
	//functions to add all of a certain type of element
	bool addAllViews(){
		varMap settings = controlSettings->getGroup("views");
		list<string> viewNames = settings.groupNames();
		cout << "Add views\n";
		for(int i=0;i<viewNames.getCount();i++){
			cout << " - Add view : " << viewNames[i] << "\n";
			view* newView = NULL;
			if(!setup::newView(viewNames[i],settings.getGroup(viewNames[i]),newView)) continue;
			cout << "add new view : " << viewNames[i] << endl;
			views.push(newView,viewNames[i]);
			viewList.push(newView);
			this->startUsing((node*)newView);
		}
		/*check to see if a view has not been set and if a
		view with the name default has been set.*/
		if(views["default"] && !controlSettings->isSet("view")){
			cout << " A default view is set, you terrier fucking hermamphrodities\n";
			activeView = views["default"];
		}
		cout << "Done adding views\n";
		return true;
	}

	/**\brief Given a group of settings, the controller will take every property in the
	settings that it understands and use them to set up a viewport*/
	bool addView(string viewName){
		varMap settings = controlSettings->getGroup("views");
		list<string> viewNames = settings.groupNames();
		for(int i=0;i<viewNames.getCount();i++){
			if(viewName != viewNames[i]) continue;
			cout << "add view with name : " << viewName << "\n";
			view* newView = NULL;
			if(!setup::newView(viewName,settings.getGroup(viewName),newView)) return false;
			views.push(newView,viewName);
			viewList.push(newView);
			this->startUsing((node*)newView);
			return true;
		}
		return false;
	}

	bool associateControllerToCamera(varMap cameraSettings, camera* camera){
		string controllerName = cameraSettings.getString("controller","default");
		if(controllers.isSet(controllerName)){
			camera->setController(controllers.getLastCheck());
			cout << "found that shit controller for that fuck camera\n";
			return true;
		}
		cout << "couldn't find controller " << controllerName << endl;
		return false;
	}

	/*
	bool addAllCameras(){
		varMap settings = controlSettings->getGroup("cameras");
		list<string> cameraNames = settings.groupNames();
		camera* newCamera = NULL;
		varMap cameraSettings;

		for(int i=0;i<cameraNames.getCount();i++){
			cameraSettings = settings.getGroup(cameraNames[i]);
			if(!setup::newCamera(cameraSettings,newCamera)) continue;
			associateControllerToCamera(cameraSettings,newCamera);
			cameras.push(newCamera,cameraNames[i]);
			cout << "Added camera : " << cameraNames[i] << endl;
			addChild(newCamera);
		}
		return true;
	}*/

	/*
	bool addCamera(string cameraName){
		varMap settings = controlSettings->getGroup("cameras");
		list<string> cameraNames = settings.groupNames();
		varMap cameraSettings;
		for(int i=0;i<cameraNames.getCount();i++){
			if(cameraName != cameraNames[i]) continue;
			camera* newCamera = NULL;
			cameraSettings = settings.getGroup(cameraName);
			if(!setup::newCamera(cameraSettings,newCamera)) return false;
			associateControllerToCamera(cameraSettings,newCamera);

			cameras.push(newCamera,cameraName);
			addChild(newCamera);
			return true;
		}
		return false;
	}
		*/
	/*
	bool addModel(string fileName){
	//check that the filename actually exists, what point is
		//trying to load something that doesn't exist?
		ifstream fileExists(fileName);
		if (!fileExists) {
			cout << "file not a valid file\n";
			return false;
		}
		//get the file extension to determine the type of
		//file that should be loaded, in conjunction with the
		/type of class that is expected.
        string ext;
        if(!getExt(fileName,ext)){
			cout << "couldn't get file name extension\n";
			return false;
        }

		if(ext == "obj"){
			mesh* tmpMesh = meshes.push(new mesh(),fileName);
			activeLoader = new load_obj(fileName,tmpMesh);
		} else {
			return false;
		}

		thread::waitForAll();
		return true;
	}*/

	bool addAllControllers(){
		if(!controlSettings->isGroup("controllers")) {
			return false;
		}
		varMap settings = controlSettings->getGroup("controllers");
		list<string> controllerNames = settings.groupNames();
		controller* newController = NULL;
		for(int i=0;i<controllerNames.getCount();i++){
			cout << "Added controller : " << controllerNames[i] << endl;
			if(!setup::newController(settings.getGroup(controllerNames[i]),newController)) continue;
            controllers.push(newController,controllerNames[i]);
			addChild(newController);
		}
		return true;
	}

	bool loadAsset(string fileName){
		return assetLibrary::getAsset(fileName);
	}

	bool activateView(string viewName){
		if(!views.isSet(viewName)) return false;
		activeView = views[viewName];
		return true;
	}

	static IndUI* setupUI(){
		IndUI* ui = new IndUI(node::windowProperties.width,
						node::windowProperties.height,
						&input::mousePositionX,
						&input::mousePositionY,
						&input::mouseButton,input::keys);
		cout << " - built ui\n";
		return ui;
	}

	virtual void private_init(){
		/*set the parent of the control to whatever the root node
		is, most likely the engine.*/
		setParent(node::root);

		/*make sure that some settings are even loaded*/
		if(!controlSettings) return;
		/*if there is a settings property, use that instead of
		whatever is in the stupid file currently being used.*/
		if(controlSettings->isSet("settings")){
			string settingsFile = controlSettings->get<string>("settings");
            if(!varMap::load(settingsFile,*controlSettings)){
				/*try to load settings from the default directory*/
				settingsFile = "include/assets/settings/" + settingsFile + ".est";
				if(!varMap::load(settingsFile,*controlSettings)){
					cout << "failed to load external settings : " << settingsFile << "\n";
					return;
				}
            }
			cout << "loaded external settings : " << settingsFile << endl;
		}//end of if external settings are set.

		addAllControllers();
		addAllViews();
		//addAllCameras();
		setEmptyViewPreferredCameras();
	}

	/*if any of the referenced objects are deleted, remove it from
	the list of objects.*/
	void onReferenceRemoval(node* oldRef){

		if(cameras.isSet(oldRef->getName())){
			cameras.removeLastNode();
		}

	}

	float f;

public:

	engine_control(varMap* settings) :
		controlSettings(settings),
		activeView(NULL),
		activeCamera(NULL),
		activeLoader(NULL) {
		private_init();
		f = 0;
	}

	~engine_control(){
		#if DEBUG_DELETES
			cout << "engine_control->";
		#endif
	}

	/*the draw function is called by the engine to update the
	screen. The default draw function for any control draws each
	view, in the opposite order they were added to the view list,
	using each views current camera. This means that whatever view
	is created last is drawn on top of all other views that already
	have been added.
	This can be overridden for some crazy stupid ass reason, but
	any logical person shouldn't need to, except for some crazy
	reason that sidesteps the entire paradigm the engine is centered
	around.*/
	virtual void windowDraw() {
		//glClearColor(++f,0,0,1);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//currentViewIndex = viewList.getCount();
		//while(currentViewIndex --> 0){
		//	viewList[currentViewIndex]->draw((drawable*)this);
		//}
		glfwSwapBuffers();
	}

	/**\brief this is the implimentation of the drawable class. By default
	it just draws all of the children of the control.

	This can, of course be overridden by anyone fool-hardy enough to give it
	a foolishly foolish go.*/
	virtual void draw() {

		for(int i=0 ; i<getDrawables().getCount() ; i++){
			this->getDrawables()[i]->draw();
        }
	}

	/**\brief The update() function is not implimented. It must be implimented
	by any classes inheriting from the engine_control class.

	**/
	virtual void update(){
		node::update();
	}

	list<view*> getViews(){
		list<mapNode<view*>*> viewNodes = views.childrenList("");
		list<view*> viewList;
		//for(int i=0;i<viewNodes.getCount();i++){
		//	viewList.push(viewNodes[i]->item);
		//}
		return viewList;
	}

	controller* getController(string name){
		if(controllers.exists(name)){
			return controllers.getLastCheck();
		}
		return NULL;
	}

	camera* setActiveCamera(string viewName,camera* cam){
		if(views[viewName]){
			views[viewName]->setCamera(cam);
		}
		return activeCamera = cam;
	}

	camera* getActiveCamera(){ return activeCamera; }
	view*	getActiveView(){ return activeView; }

};




#endif // ENGINE_STATE_H
