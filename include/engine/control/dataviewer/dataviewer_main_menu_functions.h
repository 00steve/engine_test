#ifndef DATA_VIEWER_MAIN_MENU_FUNCTIONS_H
#define DATA_VIEWER_MAIN_MENU_FUNCTIONS_H

void dataViewer_LoadDataLog(string fileName,lbr* data){

	if(!data->load(fileName)){
		cout << "COULD NOT LOAD THE LOG FILE, ya dick!\n";
	}
}


void dataViewer_AddDataGraph(string name,lbr* data,varMap settings,lbrGraph* &graph){
	graph = new lbrGraph(name,data,settings);
}





#endif // DATA_VIEWER_MAIN_MENU_FUNCTIONS_H
