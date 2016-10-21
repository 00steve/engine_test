
#ifndef ENGINE_FPSE_H
#define ENGINE_FPSE_H

/*this is a class that allows you to edit the config file (.est) for
a first person shooter, by placing objects and doing other things that
are cool and stuff.*/


#include "fpse/fpse_main_menu_functions.h"

#include "../core/var_map.h"
//#include "../include_list/fps.h"

#include "../ui/induimod.h"
#include "../ui/induidock.h"
#include "../ui/induifilebrowser.h"
#include "../ui/induicontextmenu.h"

#include "../ui/induitextbox.h"
#include "../ui/induinumberbox.h"
#include "../ui/induibutton.h"
#include "../ui/induidropdown.h"
#include "../nodes/selection_vector.h"
#include "../nodes/drag_widget.h"

class fpse : public fps{
private:

	/*editor state settings*/
	bool 				pauseGame;
	bool 				pausePhysics;
	bool 				unsavedChanges;
	depth_click			*dClick;
	controller 			*control;
	selectionVector		*selectVector;
	bool 				selected;
	bool				physicsWidgetEnabled;
	physical			*physicsWidget;


	/*objects specific to the editor that are used by
	it to make things happen*/
	IndUI 				*ui;

	IndUIMODULE			*menuDock;
	string 				*menuDockTitle;
	string 				filePulldownName;
	string 				*filePulldownValue;
	IndUIDropdown		*filePulldown;
	IndUIDropdown		*editPulldown;
	IndUIButton			*testButton;

	IndUIMODULE			*physicalMenu;
	string				*physicalMenuTitle;
	IndUINumberBox		*xPosTBox,*yPosTBox,*zPosTBox;
	double3				*emptyPhysicalPosition;

	IndUICONTEXTMENU	*contextMenu;


	list<IndUIWidget*> 	widgets;

	/*store the name of the current settings file being
	edited and the varMap that represents it. This */
	string 				mapSettingsFilename;
	varMap 				*mapSettings;

	/*store a copy of the fps engine control that
	will be set as the current state of the engine
	whenever the user wants to test the map.*/
	engine_control		*testfps;

	double3 *spawneroffset;

	void setup_physics(){
		ode::init();
		cout << "setup default physics\n";
		if(mapSettings){
			cout << " - map has settings\n";
			if(mapSettings->isSet("physics")){
				cout << " - map has physics in settings\n";
				setup::physics(mapSettings->getNewGroup("physics"));
			}
		} else {
			setup::physics(NULL);
		}
	}

	void setup_ui(){

		cout << " - setup menu\n";
		ui = engine_control::setupUI();

		menuDockTitle = new string("Main menu");
		menuDock = new IndUIDock(menuDockTitle,UI_DOCK_EDGE_TOP,NULL,NULL );

		//add file menu
		filePulldownName = "File";
		filePulldownValue = new string("file stuff");

		ui->AddModule(menuDock);

		//add dropdown (for testing)
		filePulldown = new IndUIDropdown("File");
        filePulldown->addOption("Save");
        filePulldown->addOption("Save As");
		filePulldown->addOption(new optionFunction(new string("Open"),fpse_map_file_browser,new indui_callback(ui,NULL)));
        filePulldown->addOption("Clear");
        filePulldown->addOption("Exit",fpse_exit_engine);
        menuDock->AddWidget(filePulldown);


		editPulldown = new IndUIDropdown("Edit");
		editPulldown->addOption("Undo");
		editPulldown->addOption("Redo");
		menuDock->AddWidget(editPulldown);

		//FIGURE OUT WHY THE FUCK INDUIBUTTONS DONT WORK IN THE MENU DOCK
		testButton = new IndUIButton("Start and other things -- this stuff is the same length",fpse_start_fps);

		menuDock->AddWidget(testButton);

		/*
		testPulldown = new IndUIDropdown("Test");
		menuDock->addWidget(testPulldown);
		*/

		//set up the physical menu this menu should be linked to whichever physical
		//node is selected. If no physical node is selected, it should be hidden.

		physicalMenuTitle = new string("Physical properties");
		physicalMenu = new IndUIMODULE(physicalMenuTitle);
		physicalMenu->setOffset(100,100);

		emptyPhysicalPosition = new double3(0,0,0);
		xPosTBox = new IndUINumberBox(string("X position"),emptyPhysicalPosition->x);
		yPosTBox = new IndUINumberBox(string("Y position"),emptyPhysicalPosition->y);
		zPosTBox = new IndUINumberBox(string("Z position"),emptyPhysicalPosition->z);
		physicalMenu->AddWidget(xPosTBox);
		physicalMenu->AddWidget(yPosTBox);
		physicalMenu->AddWidget(zPosTBox);
		//ui->AddModule(physicalMenu);


		//set the indui to show up in the default view
		if(views["default"]) views["default"]->setUI(ui);

		//setup the selection vector object
		selectVector = new selectionVector(dClick,control);
		addDrawable(selectVector);

		//setup the drag widget
		physicsWidget = new dragWidget(selectVector->selectedPhysicalsRef(),control);
		addDrawable(physicsWidget);
		selectVector->deferToHandler(physicsWidget);

	}

	/*given the active camera of the view that is being used
	by the engine control, the controller assigned to the camera
	if there is one.*/
	void setup_controller(){
		control = new keyboard_mouse();
	}

	bool load_map(string filename){

		varMap tmpSettings;
		varMap::load(filename,tmpSettings);

		mapSettings = &tmpSettings;
		mapSettingsFilename = filename;

		load(mapSettings);

		return true;
	}

	/*used to build a context menu for an item when right-clicked, based
	on the type that is clicked.*/
	bool display_context_menu(){
		return false;
	}

public:
	fpse(varMap* settings) :
				fps(settings),
				pauseGame(false),
				pausePhysics(false),
				unsavedChanges(false),
				dClick(NULL),
				control(NULL),
				selectVector(NULL),
				selected(false),
				physicsWidgetEnabled(false),
				physicsWidget(NULL),
				//physicalMenu(NULL),
				//physicalMenuTitle(NULL),
				contextMenu(NULL),
				widgets(list<IndUIWidget*>()),
				mapSettings(NULL)
			{
		cout << "setup FPSE node " << this << "\n";
		setup_controller();
		setup_physics();
		setup_ui();

		/*hard code test of map load*/
		load_map("include/assets/settings/01_the_beach.est");

		setName(new string("fpse_" + numberToString<unsigned long>(getId())));


	}

	~fpse(){
		#if DEBUG_DELETES
			cout << " fpse >";
		#endif
	}


	/*overwrite normal update function to update whatever
	we feel like whenever we feel like updating it, ya fuckin
	fuck fuckeruuuuuuuuU!*/
	void update(){

		if(!ui->IsActive()){
			if(!pauseGame){
				//fps::update();
			}
			if(!pauseGame) node::update();
			if(!pausePhysics) ode::step();

			/*if the primary controller action is true,
			the left mouse button is being clicked. If it is
			not already being clicked*/
			control->update();
			if(control->primary_action()){
				dClick = getDepthClick(control);
				if(dClick){
					dClick->active = true;
					selected = true;
					selectVector->setDepthClick(dClick);
				}
			} else if(selected){
				selected = false;
				if(dClick) dClick->active = false;
				selectVector->setDepthClick(dClick);
			}

			if(control->secondary_action() && !contextMenu){
				dClick = getDepthClick(control);
				if(dClick){
					dClick->active = true;
					selected = true;
					selectVector->setDepthClick(dClick);

				}
			}


		}
		ui->update();
	}

	bool loadMap(string filename){
		return load_map(filename);
	}

};



#endif // ENGINE_FPS_H
