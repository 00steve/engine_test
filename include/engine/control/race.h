#ifndef RACE_H
#define RACE_H

#include "../core/engine_control.h"
#include "../nodes/car.h"

class race : public engine_control{
private:

	list<vehicle*> vehicles;

	/*test shit to get this fucking shit working and fuck*/
	dGeomID groundGeom;
	controller *kbm;



	void setup_physics(varMap* settings){
		ode::init();
		setup::physics(settings->getNewGroup("physics"));

		//setup stupid fake ground
		addDrawable(new infinite_plane(double3(0,1,0),-5));
	}


public:
	race(varMap* settings):
			engine_control(settings)
			{

		setup_physics(settings);


		varMap vehicleGroupList = settings->getGroup("vehicles");
		list<string> vehicleNames = vehicleGroupList.groupNames();
		varMap vehicle;
		varMap vehicleSettings;
		string vehicleType;


		for(int i=0;i<vehicleNames.getCount();i++){
			vehicle = vehicleGroupList.getGroup(vehicleNames[i]);
			cout << "vehicle name : " << vehicleNames[i] << endl;
			if(vehicle.getString("settings","") != ""){
				if(varMap::load(vehicle.getString("settings",""),vehicleSettings)){
					vehicleType = vehicle.getString("type","");
					cout << "- loaded vehicle settings for type : " << vehicleType << "\n";
					if(vehicleType == "car"){
						vehicles.push(new car(vehicleSettings));
						addDrawable(vehicles[vehicles.getCount()-1]);
					} else {
						cout << "- what am I supposed to do wit this shit? [invalid type]\n";
					}
				} else {
					cout << "- couldn't load your shitty setings from file " << vehicle.getString("settings","[No string]") << endl;
				}
			}
		}

		kbm = new keyboard_mouse();







	}

	~race(){
		ode::destroy();
		#if DEBUG_DELETES
			cout << " race >";
		#endif
	}


	void update(){
		node::update();
		if(kbm->primary_interaction()){
			ode::step();
		}
	}


};
#endif // RACE_H
