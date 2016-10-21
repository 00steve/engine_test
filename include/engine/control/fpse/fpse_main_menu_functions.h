#ifndef FPSE_MAIN_MENU_FUNCTIONS_H
#define FPSE_MAIN_MENU_FUNCTIONS_H

/*create a list of static functions that can be called from the fpse menu*/

#include "../../core/node.h"
#include "../../ui/indui.h"
#include "../../ui/induifilebrowser.h"

bool fpse_do_nothing(void* data){
	cout << "no action!\n";
	return false;
}

bool fpse_start_fps(void* data){
	node_message* m = new node_message();
	m->code = MESSAGE_ENGINE_SET_TEMP_CONTROLLER;
	m->data = data;
	node::getRoot()->recieveMessage(m);
	return true;
}

bool fpse_exit_engine(void* data){
	node_message* m = new node_message();
	m->code = MESSAGE_ENGINE_EXIT;
	node::getRoot()->recieveMessage(m);
	return true;
}


bool fpse_save_map(void* data){
	return false;
}

bool fpse_map_file_browser(void* callback){
	cout << "Load map\n";

	indui_callback *cb = (indui_callback*) callback;
	if(!cb->indui_instance) return false;


	IndUI* ui = (IndUI*) cb->indui_instance;

	/*add file browser to main menu*/
	IndUIFileBrowser *browser = new IndUIFileBrowser(new string("Select a file"));

    ui->AddModule(browser);


	return true;
}


#endif // FPSE_MAIN_MENU_FUNCTIONS_H
