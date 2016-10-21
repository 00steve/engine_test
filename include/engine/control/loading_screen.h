#ifndef LOADING_SCREEN_CONTROL_H
#define LOADING_SCREEN_CONTROL_H

#include "../core/engine_control.h"
#include "../nodes/decoration.h"
#include "loading_screen/loading_screen_progress.h"
#include "loading_screen/progress_load_bar.h"
#include "../core/conditions/is_more_than.h"
#include "../core/actions/send_message.h"

class loadingScreen : public engine_control{
private:

	mesh* background;
	mesh* foreground;
	string foregroundMeshName;
	string backgroundMeshName;
	decoration* foregroundDecoration;
	decoration* backgroundDecoration;

	double currentProgress;
	double completeProgress;

	/*this is the flag that is passed to the
	event. When the flag changes, the event is
	fired.*/
	bool doneLoading;
	event done;


	loading_screen_progress* progressWidget;


public:

	loadingScreen(varMap* settings) :
		engine_control(settings),
		background(NULL),
		foreground(NULL),
		currentProgress(0),
		completeProgress(1),
		doneLoading(false),
		progressWidget(NULL) {

		if(controlSettings->isSet("background")){
			backgroundMeshName = controlSettings->get<string>("background");
			if(!loadAsset(controlSettings->get<string>("background"))){
				cout << "failed to load background file\n";
				return;
			} else {
				background = meshes[backgroundMeshName];
				backgroundDecoration = new decoration(background);
				addDrawable(backgroundDecoration);
			}
		}

		if(controlSettings->isSet("foreground")){
			foregroundMeshName = controlSettings->get<string>("foreground");
			if(!loadAsset(foregroundMeshName)){
				cout << "failed to load background file\n";
				return;
			} else {
				foreground = meshes[foregroundMeshName];
				foregroundDecoration = new decoration(foreground);
				addDrawable(foregroundDecoration);
			}
		}

		varMap progressDisplaySettings = controlSettings->getGroup("progress-display");
		/*setup a widget for the loading screen
		to show progress*/
		if(progressDisplaySettings.isSet("type")){
			string progressType = progressDisplaySettings.get<string>("type");
			cout << "has progress display: " << progressType << endl;

			varMap progressSettings = controlSettings->getGroup("progress-display");
			if(progressType == "load-bar"){
				progressWidget = new progress_load_bar(&currentProgress,progressSettings);
				addDrawable((drawable*)progressWidget);
			}

		}

		map<event*> events = map<event*>();

		/*builds common event types for the calling object
		using the settings to return a map of events.*/
		if(!setup::newEvents(this,*controlSettings,events)){
			cout << "ERROR: the loading screen requires the on-done event to do things, son!\n";
			return;
		}

		/*new event that fires when the controller is finished
		doing whatever it does.*/
		if(events.isSet("on-done")){
			//done = events["on-done"];
			events["on-done"]->fireWhen(new isMoreThan(&currentProgress,&completeProgress));
			if(!node::getParent()){

				cout << "loading screen doesn't have a parent, yo\n";
			}
			/*this is being set manually now, code will have to be added to the setup::newEvents function
			to check for actions that should happen when an event fires.*/
			//events["on-done"]->performAction(new send_message(this,getParent(),MESSAGE_ENGINE_EXIT,(void*)"main-menu"));
			addChild(events["on-done"]);
		}

	}

	void update(){
		currentProgress += .0025;
		if(progressWidget) progressWidget->update();

		node::update();
	}

};


#endif // LOADING_SCREEN_CONTROL_H
