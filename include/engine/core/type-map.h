#ifndef TYPEMAP_H
#define TYPEMAP_H

#include "map.h"
#include <typeinfo>

/*this class translates types into a readable format that can be used
to translate things and stuff when the typeid is called.*/
class typeMap{
private:
	static map<string> dictionary;

public:

	template<class T>
	static void define(string name){
		string compName = (typeid(T)).name();
		dictionary.push(name,compName);
	}

	template<class T>
	static string translate(){
		string compName = (typeid(T)).name();
		if(dictionary.isSet(compName)){
			return dictionary[compName];
		} else {
			return "";
		}
	}


};

map<string> typeMap::dictionary = map<string>();


#endif // TYPEMAP_H
