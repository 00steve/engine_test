#ifndef LOAD_H
#define LOAD_H
#include "thread.h"
#include "map.h"
#include "../load/dependency.h"
#include "../core/var_map.h"

class loader{
private:

    /*this needs to be implimented by whatever
    class is inheriting this so it can perform
    its own custom load of whatever file.*/
    virtual void custom_load(string file_name) {
        cout << "load::custom_load() says \"override me, bitch!\"\n";
    };

    /*this starts the thread that runs the loading of whatever
    file or object that needs to be loaded.

    This function was called by a new thread, so it is reconfubulating
    the object that was passed in, into something of the load class
    type. That way, it knows which functions can be called.*/
    static void static_load(void* args) {
        loader* dyn_obj = (loader*) args;
        dyn_obj->custom_load(dyn_obj->file_name);

    }

    /*add a new thread that runs the static_load
    function*/
    thread* load_thread;

    /*store the name of the file that is being loaded*/
    string file_name;

	bool isComplete;

	bool isSuccessful;

	/*store whatever the hell is loaded by a load inheriter.
	Whatever is using this bad boy should know what it is
	getting into, beucase if it is the wrong class, it will
	fucking kill every living thing in the program.*/
	void* data;

	/*keep a list of all dependencies found when the object
	is loaded.*/
	list<dependency*> dependencies;

protected:

    /*if for some reason the custom_load function didn't
    work out for the best, set some properties of the class
    and print some letters.*/
    void failed() {
        isSuccessful = false;
        cout << "Couldn't load\n";
    }

    void succeeded(){
    	isSuccessful = true;
    	//cout << "Loaded like a rolllizard\n";
    }

	void finished() {
		/*if an asset has no dependencies left load,
		say that it is complete and remove the thread
		that it is running in and stuff, for fuck's sake@!*/
		//thread::removeThread(load_thread);
	}

public:


	static varMap settings;


    /*basically a shell to call the custom_load
    implimentation from a new thread.*/
    loader(string file_name) :
        file_name(file_name),
        isComplete(false),
        isSuccessful(false),
        data(NULL),
        dependencies(list<dependency*>()) {
    }

    loader(string file_name,void* data) :
        file_name(file_name),
        isComplete(false),
        isSuccessful(false),
        data(data),
        dependencies(list<dependency*>()) {
    }

    void start() {
        load_thread = new thread(loader::static_load,this);
    }

    void nonThreadedStart(){
		custom_load(file_name);
    }

    /**/
    bool IsComplete() {
    	return isComplete;
    }

    bool wasSuccessful() {
    	return isSuccessful;
	}


	/*function used by the asset library to load shit*/
	static loader* loadFile(string fileName){
		return NULL;
	}


	virtual void* getData() = 0;


	/*add file that needs to be loaded to make this file work
	correctly*/
    void add_dependency(dependency* newDependency) {
    	newDependency->object_reference = (void*)this;
    	dependencies.push(newDependency);
	}


	/*class that must be implimented by any inheriting classes,
	mostly to force anyone writing derived classes to have
	awareness that they can do this.*/
	virtual void integrateDependency(dependency* depends) = 0;

	list<dependency*>* getDependencies(){return &dependencies; }

};

varMap loader::settings = varMap();
#endif // IMPORT_H
