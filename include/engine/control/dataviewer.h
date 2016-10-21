#ifndef DATA_VIEWER_CONTROL_H
#define DATA_VIEWER_CONTROL_H

#include "../core/engine_control.h"
#include "../ui/induimod.h"
#include "../ui/induidock.h"
#include "../ui/induifilebrowser.h"
#include "../ui/induicontextmenu.h"

#include "../ui/induitextbox.h"
#include "../ui/induinumberbox.h"
#include "../ui/induibutton.h"
#include "../ui/induidropdown.h"


#include "dataviewer/lbrgraph.h"


#include "dataviewer/dataviewer_main_menu_functions.h"


class dataViewer : public engine_control{
private:

	lbr data;
	list<lbrGraph*> graphs;

	void loadHistory(varMap settings){
		if(settings.isSet("lastLogFile")){
			cout << "has last log file : load it!\n";
			dataViewer_LoadDataLog(settings.getString("lastLogFile",""),&data);
		}
		if(settings.isGroup("openGraphs")){
            varMap views = settings.getGroup("openGraphs");
            list<string> viewNames = views.groupNames();
            for(int i=0;i<viewNames.getCount();i++){
				cout << viewNames[i] << endl;
				lbrGraph* newGraph = NULL;
				dataViewer_AddDataGraph(viewNames[i],&data,views.getGroup(viewNames[i]),newGraph);
				graphs.push(newGraph);
				addDrawable((drawable*)newGraph);
            }
		}
	}

public:

	dataViewer(varMap* settings) :
			engine_control(settings)
			{
		loadHistory(settings->getGroup("history"));
	}

	void update(){
		node::update();
		draw();
	}


};


#endif
