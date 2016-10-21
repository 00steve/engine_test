#ifndef ENGINE_FPS_H
#define ENGINE_FPS_H



#include "../include_list/fps.h"




/**\brief this is a generic class that builds the framework for
a generic fps.

It knows about actors, props, etc. that can interact and do things.
It can also have rules that describe how they interact. That is the
part that is generic. So inheriting classes can have the base of a
working fps engine without any rules.**/
class fps : public engine_control{

	/*physics are set to work at 100 fps, if that changes at some
	point in the future, this whole fps system and everything in
	it will require a major overhaul to take into account different
	fps timings and junk stuff like that shizzy*/
	double physics_tick;
	double physics_tick_left;



protected:

	string mapModelName;
	string skyboxModelName;

	/*stuff to show the map, mostly right now the
	map is a decoration.*/
	ground* mapNode;
	skybox* skyboxNode;

	/*physics type objects*/
	dGeomID plane_geom;

	void setup_physics(){
		ode::init();
		setup::physics(controlSettings->getNewGroup("physics"));
	}

	void set_ground(string modelName){
		mapNode = new ground(modelName);
		addDrawable(mapNode);
		cout << "added ground\n";
	}

	/*
	Goals are what need to be accomplished by the player
	in order to leave the level completed.
	*/
	void set_goals(varMap settings){
	}

	void set_spawn_points(varMap settings){
	}

	/**\brief build all of the rules that control the behavior of the
	fps controller.

	Such as where the player can spawn, where enemies can spawn, what
	constitutes a "win" by the player.**/
	void build_rules(varMap settings){
		cout << "[rules]\n";

		list<string> groups = settings.groupNames();
		string rule;
		while(groups.getCount()){
			rule = groups.pop();
			if(rule == "goals"){
				set_goals(settings.getGroup(rule));
			} else if(rule == "spawn-points"){
				set_spawn_points(settings.getGroup(rule));
			}
		}
	}

	/**\brief depending on the settings that are passed in,
	build a new object and add it to the fps.**/
	drawable* build_object(varMap settings){
		if(!settings.isSet("type")){
			cout << "could not add node to fps: no type set\n";
			return NULL;
		}

		drawable* nNode = NULL;
		string type = settings.get<string>("type");
		double3 offset = setup::getDouble3(settings,"offset",double3(0,0,0));
		bool enabled = setup::getBool(settings,"enabled",false);

		if(type == "crate"){
			physical* c = new crate(offset);
			if(!enabled) c->disableBodies();
			c->setDefaultEnabled(enabled);
			nNode = c;
		} else if(type == "spawner"){
			string stype = setup::getString(settings,"spawn","none");
			if(stype == "crate"){
				nNode = new spawner<crate>(settings);
			}else{
				return NULL;
			}
		} else if(type == "pickup"){
			nNode = new pickup(settings);
		} else if(type == "player") {
			nNode = new fpsPlayer(settings);
		} else if(type == "barrel"){
			nNode = new barrel(settings);
		} else if(type == "fire"){
			nNode = new fire(settings);
		} else if(type == "portal"){
			nNode = new portal(settings);
		} else {
			cout << " OBJECT :: CREATE - invalid object type\n";
		}

		return nNode;
	}

	/**\brief load things specifically needed to make the fps
	simulation work.

	Don't load anything that is needed to make the engine control
	work. This function is used by other controls to build the
	objects in the fps, but use them in a different way.**/
	bool load(varMap *settings){
		controlSettings = settings;
		setup_physics();
		/*load stuff about the map*/
		if(settings->isSet("map.model")){
			set_ground(settings->get<string>("map.model"));
		}
		varMap objectsGroup = settings->getGroup("objects");
		varMap objectGroup;
		list<string> objectGroupNames = objectsGroup.groupNames();
		string cur_label;
		while(objectGroupNames.getCount()){
			cur_label = objectGroupNames.pop();
			objectGroup = objectsGroup.getGroup(cur_label);
			drawable* tmpNode = fps::build_object(objectGroup);
			if(tmpNode){
				addDrawable(tmpNode);
			} else {
				cout << "invalid type:\n";
			}
		}


		/*load the game rules*/
		if(settings->isGroup("rules")){
			build_rules(settings->getGroup("rules"));
		}


		return true;
	}


public:
	fps(varMap* settings) :
			engine_control(settings),
			physics_tick(.01),
			physics_tick_left(0),
			mapNode(NULL)
			{
		fps::load(controlSettings);
		/*load the skybox*/
		/*
		if(controlSettings.isSet("map.skybox")){
			skyboxModelName = controlSettings.get<string>("map.skybox");
			skyboxNode = new skybox(assetLibrary::getDisplayList(skyboxModelName));
			addDrawable(skyboxNode);
		}
		*/
	}

	~fps(){
		ode::destroy();
		#if DEBUG_DELETES
			cout << " fps >";
		#endif
	}

	void update(){
		node::update();
		ode::step();
	}

	static bool load(string filename){
		return true;
	}


};



#endif // ENGINE_FPS_H
