#include "assetlibrary.h"

AssetLibrary::AssetLibrary(){

    rootDirectory = "engine/assets/";
    loadedFiles = map<void*>();
}



VarMap AssetLibrary::LoadSettings(Node* loader,string fileName){
	if(loadedFiles.exists(fileName)) {
		cout << "found existing load of " << fileName << endl;
		return *((VarMap*)loadedFiles[fileName]);
	}
	return LoadSettingsFile(loader,fileName);
}

VarMap AssetLibrary::LoadSettingsFile(Node* loader,string fileName){
	fileName = rootDirectory+"settings/"+fileName;
	cout << "load settings file " << fileName << endl;
	VarMap newMap;
	if(!VarMap::Load(fileName,newMap)){
		cout << "couldn't load settings\n";
		return newMap;
	}
	loadedFiles.push((void*)&newMap,fileName);
	return newMap;

}

template <class T>
T AssetLibrary::Load(Node* loader,string fileName,VarMap settings){
    eDLL* dll = new eDLL(fileName.c_str());
    if(loadedFiles.isSet(fileName)){
        dll = (eDLL*) loadedFiles.getLastCheck();
    } else {
		if(!dll->IsLoaded()){
			delete dll;
			cout << " - couldn't load the dll for some reason\n";
			return NULL;
		}
		loadedFiles.push((void*)dll,fileName);
    }
    string funcName = string("Build");
	T object = GetDllValue<T>(dll,funcName.c_str());
	object->GlobalsRef(Node::GlobalsRef());
	object->GlobalRequestsRef(loader->GlobalRequestsRef());
	object->AssetLibraryRef(loader->AssetLibraryRef());
	cout << " - static node asset library : " << loader->AssetLibraryRef() << endl;
	settings.Print();
	cout << "ON LOAD set settings of node\n";
	object->Settings(settings);
	return object;
}

Node* AssetLibrary::LoadCustom(Node* loader,string settingsName,VarMap settings){
	if(!settings.IsSet("type")) return NULL;
	string fileName = rootDirectory + "extensions/engine." + settingsName
		+ "." + settings.get<string>("type") + ".dll";
	return Load<Node*>(loader,fileName,settings);
}




/*
EngineControl* AssetLibrary::LoadEngineControl(Node* loader,VarMap settings){
	string type = settings.get<string>("type");
	string fileName = rootDirectory + "extensions/engine.control." + type + ".dll";
	eDLL* dll = NULL;
	if(loadedFiles.isSet(fileName)){
		dll = (eDLL*)loadedFiles.getLastCheck();
	} else {
		dll = new eDLL(fileName.c_str());
		if(!dll->IsLoaded()){
			delete dll;
			cout << " - couldn't load the dll for some reason\n";
			return NULL;
		}
		loadedFiles.push((void*)dll,fileName);
	}
    string funcName = string("BuildEngineControl");
	EngineControl* ec = GetDllValue<EngineControl*>(dll,funcName.c_str());
	ec->GlobalsRef(loader->GlobalsRef());
	ec->GlobalRequestsRef(loader->GlobalRequestsRef());
	ec->AssetLibraryRef(loader->AssetLibraryRef());
	cout << " - loaded engine control asset library ref " << loader->AssetLibraryRef() << " from " << &ec->Assets() << endl;
	ec->Settings(settings);
	return ec;
}


View* AssetLibrary::LoadView(Node* loader,VarMap settings){
    if(!settings.IsSet("type")){
        cout << "invalid settings argument. No \"type\" set.\n";
        return NULL;
    }
    string fileName = rootDirectory + "extensions/engine.view." + settings.get<string>("type") + ".dll";
    return Load<View*>(loader,fileName,settings);
}




Camera* AssetLibrary::LoadCamera(Node* loader,VarMap settings){
    if(!settings.IsSet("type")){
        cout << "invalid settings argument. No \"type\" set.\n";
        return NULL;
    }
    string fileName = rootDirectory + "extensions/engine.camera." + settings.get<string>("type") + ".dll";
    return Load<Camera*>(loader,fileName,settings);
}


*/

