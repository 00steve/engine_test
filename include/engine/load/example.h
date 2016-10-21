/*an implimentation of the load class that does nothing,
but displays the way a load class should be set up.
*/


#ifndef EXAMPLE_LOAD_H
#define EXAMPLE_LOAD_H
#include "../core/load.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../core/string_util.h"


class load_example : public load {
private:



    /*if for some reason the custom_load function didn't
    work out for the best, set some properties of the class
    and print some letters.*/
    void failed() {
    	Completed();
        isSuccessful = false;
        cout << "couldn't load\n";
    }

    void custom_load(string file_name) {
        cout << "load::example(" << file_name << ")\n";

        //create a file stream
        ifstream file;
        string line;
        //attempt to open the file to read
        file.open(file_name.c_str());
        if(!file.good()) {
			cout << "file doesn't exist, bitch\n";
            failed();
            return;
        }

        while (!file.eof()) {
            /*get next line*/
            getline(file, line);
        }

        cout << "finished loading " << file_name << "\n";
        Completed();

    }

    bool isSuccessful;

public:

    load_example(string file_name) :
        load(file_name),
        isSuccessful(false){
        start();
    }


    virtual bool successful(){
        return isSuccessful;
    }

	/*replace this with whatever object has been
	built by the custom_load function.*/
    void* get_example(){
        return void*;
    }



};

#endif // BMP_LOAD_H

