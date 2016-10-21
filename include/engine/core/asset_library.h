/*
the asset library is used by engine to load and manage assets. It is
static so it can be accessed from anywhere. The whole point of it is
that a model or sound file, etc. can only be loaded once and can be
accessed from anywhere in the project.

*/

#ifndef ASSET_LIBRARY_H
#define ASSET_LIBRARY_H

#include "../core/map.h"
#include "../core/thread_safe_list.h"

#include "loader.h"
#include "../load/obj.h"
//#include "../load/bmp.h"
#include "../load/image.h"

#include "../core/controller.h"
#include "../input/keyboard_mouse.h"

#include "../core/string_util.h"
#include "../graphics/draw_mesh.h"

class mesh;
class texture;

class assetLibrary{
private:

	/*all files and the pointer they reference to in each of
	the asset type lists.*/
	static map<void*> directory;

	/*lists of loaded assets by type*/
	static list<mesh> meshes;
	static list<texture> textures;
	static list<varMap> settings;
	static list<GLuint> display_lists;
	static list<node*> node_types;

	static controller* kbm;

	static loader* getFileLoader(const string fileName){
		loader* loader = NULL;
		/*check that the filename actually exists, what point is
		trying to load something that doesn't exist?*/
		ifstream fileExists(fileName.c_str());
		if (!fileExists) {
			cout << "file not a valid file\n";
			return loader;
		}
		/*get the file extension to determine the type of
		file that should be loaded, in conjunction with the
		type of class that is expected.*/
        string ext;
        if(!getExt(fileName,ext)){
			cout << "couldn't get file name extension\n";
			return loader;
        }
        ext = stringToLower(ext);


		if(ext == "obj"){
			meshes.push(mesh());
			mesh* tmpMesh = &meshes.last();
			loader = new load_obj(fileName,tmpMesh);
			directory.push((void*)&meshes.last(),fileName);
		}
		else if(ext=="bmp" || ext=="png" || ext=="jpg"){
			textures.push(texture());
			texture* tmpTexture = &textures.last();
			loader = new load_image(fileName,tmpTexture);
			directory.push((void*)&textures.last(),fileName);
		}
		/*this is the part of the code where any node should be able
		to call the dll of another node type to load that nodes
		definition into the program, and it should be able to call it
		again after that.*/
		else if(ext=="dll"){
			cout << "DON'T KNOW WHAT TO DO HERE, MOTHERFUCKER!\n";
/*
			HINSTANCE hdll = NULL;
			engine_control* new_control = NULL;
			typedef engine_control* (*pvFunctv)(varMap*);
			hdll = LoadLibrary(string(fileName).c_str());
			if(hdll == NULL){
				cout << "could'nt load dll "+fileName+"\n";
				return false;
			} else {
				cout << "[DLL] "+fileName+" []\n";
			}
			pvFunctv CreateFoo = (pvFunctv)GetProcAddress(hdll,"GetNode");
			new_control = static_cast<engine_control*>(CreateFoo(currentStateSettings.getNewGroup("control")));
			setCurrentControl(new_control);
*/
		}
		return loader;
	}

	static void loadAll(string fileName){
		list<dependency*> dependencies = list<dependency*>();
		dependency *cD = new dependency();
		dependency *cC = new dependency();
		loader* fLoader = NULL;
		cD->object_reference = NULL;
		cD->file = fileName;
		dependencies.push(cD);
		while(dependencies.getCount()){
			cD = dependencies.pop();
			if(!directory.exists(cD->file)){
				fLoader = getFileLoader(cD->file);
				if(!fLoader) continue;
				fLoader->nonThreadedStart();
				if(!fLoader->wasSuccessful()) continue;
				cD->load_data = fLoader->getData();
				/*loop through all of the dependencies that have
				been found for the file. Check to see if the file
				has been loaded, if not, add it to the list of
				dependencies to load*/
				while(fLoader->getDependencies()->getCount()){
					cC = fLoader->getDependencies()->pop();
					dependencies.push(cC);
				}
			}//end of if data needs to be loaded
			if(cD->object_reference){
				((loader*)cD->object_reference)->integrateDependency(cD);
			}

		}//end of looping through dependencies
	}

	static void loadFile(string fileName){
		//start new thread to load file and
		//all dependencies
		loadAll(fileName);
	}


public:

	/**\brief this function should be called when the engine has been destroyed to
	remove any memory from things that have been loaded.**/
	static void destroy(){
		if(display_lists.getCount()){
			glDeleteLists(display_lists[0],display_lists.getCount());
		}
	}

	/**\brief loads an a file into the asset library if it has not already
	been loaded.**/
	static bool getAsset(const string fileName){
		if(!directory.exists(fileName)){
			loadFile(fileName);
		}
		return directory.exists(fileName);
	}

	static mesh* getMesh(const string fileName){
		if(!directory.exists(fileName)){
			loadFile(fileName);
		}
		return (mesh*) directory[fileName];
	}

	static GLuint getDisplayList(const string fileName){

		if(!directory.exists(fileName+"_dl")){
			mesh* m = getMesh(fileName);
			directory.push((void*)generate_display_list(m),fileName + "_dl");
		}
		return (GLuint) directory[fileName+"_dl"];
	}

	static controller* getController(const string controllerType){
		if(controllerType == "keyboard_mouse"){
			if(!kbm){
				kbm = new keyboard_mouse();
			}
			return kbm;
		}
		return NULL;
	}

	void* operator [] (const string fileName){
		return directory[fileName];
	}

    template <typename t> static t* load(string fileName,varMap* settings){

		HINSTANCE hdll = NULL;
		t* newNode = NULL;
		typedef t* (*pvFunctv)(varMap*);
		hdll = LoadLibrary(string(fileName).c_str());
		if(hdll == NULL){
			cout << "could'nt load dll "+fileName+"\n";
			return NULL;
		} else {
			cout << "[DLL] "+fileName;
		}
		pvFunctv CreateFoo = (pvFunctv)GetProcAddress(hdll,"NewNode");
		newNode = static_cast<t*>(CreateFoo(settings));
		cout << " - created\n";
		return newNode;
    }


};


/*declare all of the objects*/
map<void*> 			assetLibrary::directory 		= map<void*>();
list<mesh> 			assetLibrary::meshes 			= list<mesh>();
list<texture> 		assetLibrary::textures 			= list<texture>();
list<varMap>		assetLibrary::settings			= list<varMap>();
list<GLuint> 		assetLibrary::display_lists		= list<GLuint>();
list<node*> 		assetLibrary::node_types		= list<node*>();

controller*			assetLibrary::kbm				= NULL;


#endif // ASSET_LIBRARY_H

