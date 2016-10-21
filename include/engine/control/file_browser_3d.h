#ifndef FILE_BROWSER_3D_H
#define FILE_BROWSER_3D_H

#include "../core/engine_control.h"
#include "../core/engine_setup.h"
#include "../file/file_computer.h"

class fileBrowser3d : public engine_control{
private:

	fileItem* root;
	int depth;

	void getFolderItems(string* dir,unsigned int depth){
		--depth;
		if(!dir){
			//get all directories of computer
			//if depth > 0, call getFolderItems with path given
		} else {
			//get all folders and files in directory
			//if is folder and depth > 0 call get folder items
		}
	}


public:


	fileBrowser3d(varMap* settings) :
			engine_control(settings),
			depth(3)
			{

		/*see if there is a certain depth that should be reached*/
		if(settings->isSet("depth")){
			depth = setup::getInt(*settings,"depth",1);
			cout << "get depth : " << depth << endl;
		}

		/*set the root directory if it is set. If not, the root
		is the entire computer, which should display all of the
		drives.*/
		if(settings->isSet("root")){
			string root_name = setup::getString(*settings,"root","");
			if(root_name != ""){
				cout << "got root : " << root_name << endl;
				root = new fileFolder(root_name);
			}
			root = NULL;
		}else{
			root = new fileComputer();
		}

		//root->scanForChildren(depth);
		root->refresh(depth);
	}

	~fileBrowser3d(){
		#if DEBUG_DELETES
			cout << " file browser 3d >";
		#endif
	}


	void update(){

	}
};




#endif // FILE_BROWSER_3D_H
