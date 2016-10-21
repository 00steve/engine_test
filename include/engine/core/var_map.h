#ifndef VAR_MAP_H
#define VAR_MAP_H
#include "map.h"
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <string>
#include "string_util.h"

/*the settings class stores any settings that can be configured in the engine.*/


class varMapEntry {

public:
	string classType;
	void* value;
	/*used to start grouping things together*/
	varMapEntry* parent;

	varMapEntry() :
		classType(""),
		value(NULL),
		parent(NULL) {
	}

	varMapEntry(void* value,string classType) :
		classType(classType),
		value(value),
		parent(NULL) {
	}


};




class varMap {
private:
	/*keep a map that translates the name of a setting
	into an engine_settings_nugget that contains the type
	and value of a nugget of setting.*/
	map<varMapEntry> settings;

	/*keeps track of if a file has been loaded to set all
	of the settings.*/
	string* loadedFile;

	static bool nextLineChunk(string &line,string &chunk,char &del,list<char> chars) {
		unsigned int index = line.length();
		if(!index) return false;
		unsigned int i;
		int x = chars.getCount();
		while(x --> 0) {
			i = line.find(chars[x]);
			if(i < index) index = i;
		}
		if(index < line.length()) {
			chunk = string(line.substr(0,index));
			del = line[index];
			line = string(line.substr(index+1));
			return true;
		}
		chunk = line;
		line = "";
		return true;
	}

	/*recursive function to load all of the properties out
	of an opened file. If an object is found that starts on
	a line, the file and newly created child map will be
	sent to a new parse file function until the object is
	finished reading.*/
	static bool parseFile(ifstream &file,map<varMapEntry> &m,string domain) {
		string line;
		string key;
		string value;
		string type;
		string chunk;
		string last_chunk;
		string last_value = "";
		string last_type = "";
		string last_key = "";
		/*if a string is surrounded by " quotes, the string will be marked as
		a literal, which means that the rest of its contents up until the next
		" should be treated as one chunk, no matter what is in it.*/
		bool literal = false;
		bool was_literal = false;
		/*the current reserved character. There aren't too many, but this
		stores the reference to the most recent one*/
		char rchar;

		/*when a value is stored, we first need to figure out which type it is.
		This is the variable that will be used to do that.*/
		char num_type;

		/*the list of characters that are used
		to split the entire file string into different
		chunks.*/
		list<char> reserved_chars = list<char>();
		reserved_chars.push(' ');
		reserved_chars.push('[');
		reserved_chars.push(']');
		reserved_chars.push('#');

		getline(file, line);
		while (!file.eof()) {
			/*a bunch of functions to clean the string up and
			get it ready for being parsed*/
			line = stringTrim(line);
			//line = convertChar(line,'{','[');
			//line = convertChar(line,'}',']');
			line = removeCharBefore(line,' ','[');
			line = convertChar(line,'\t',' ');
			line = removeRepeatingChars(line,' ');

			type = "";
			rchar = ' ';
			while(nextLineChunk(line,chunk,rchar,reserved_chars)) {
				if(literal) {
					rchar = ' ';
				}
				switch(rchar) {
				case '#':
					line = "";
					type = "comment";
					break;

				case ' ':
					if(key == "") {
						key = chunk;
						value = "";
					}
					else {
						if(chunk[0] == '"') {
							literal = true;
						}
						if(chunk[chunk.length()-1] == '"') {
							literal = false;
							was_literal = true;
						}
						value += chunk + string(1,rchar);
					}
					type = "key/value";
					break;

				case '[':
					if(chunk == "" && type == "" && last_chunk != ""){
						chunk = last_chunk;
					}
					//cout << " - [object] - " << chunk << endl;
					type = "object";
					line = stringTrim(line);
					/*if a new chunk is starting, recursively call the parse file
					and pass it in the name of the chunk and its parent name*/
					parseFile(file,m,domain+chunk+".");
					break;

				case ']':
					//cout << " - [end object]\n";
					type = "endobject";
					return true;
					break;
				}

				if(!literal && last_key == key && last_key != "" && last_type == "key/value") {
					value = stringTrim(value);
					if(was_literal) {
						was_literal = false;
						value = value.substr(1,value.length()-2);
					}
					/*based on the way that all of this shit works, there is an
					extra char at the end of every value that is to be saved. It
					needs to be removed. If it isn't, any strings will have an
					extra space or " at the end and numbers won't be evaluated as
					numbers.*/
					value = value.substr(0,value.length()-1);

					cout << "SAVE: " << domain << last_key << " = " << value << endl;
					key = domain + key;
					if(stringIsNumber(value,num_type)) {
						num_type = 'D';
						switch(num_type) {
							case 'U': push<unsigned int>(m,new unsigned int(stringToNumber<unsigned int>(value)),key); break;
							case 'I': push<int>(m,new int(stringToNumber<int>(value)),key); break;
							case 'F': push<float>(m,new float(stringToNumber<float>(value)), key); break;
							case 'D': push<double>(m,new double(stringToNumber<double>(value)), key); break;
						}
						//cout << "   - NUMBER (" << num_type << ")\n";
					}
					else if(value == "true"){
						push<bool>(m,new bool(true),key);
					}else if(value == "false"){
						push<bool>(m,new bool(false),key);
					}
					else {
						push<string>(m,new string(value),key);
						//cout << "   - STRING\n";
					}
					key = "";
					value = "";
				}
				last_key = key;
				last_value = value;
				last_chunk = chunk;
				last_type = type;
			}//end of looping through chunks in line

			getline(file, line);
		}
		return true;
	}

public:

	varMap() :
		settings(map<varMapEntry>()),
		loadedFile(NULL) {
	}

	template<typename T>
	bool add(T* value,string name){
		settings.push(varMapEntry((void*) value,(typeid(T)).name()),name);
		return true;
	}

	/*new engine settings can be mapped dynamically*/
	template<typename T>
	static bool push(map<varMapEntry> &m,T* value,string name) {
		m.push(varMapEntry((void*) value,(typeid(T)).name()),name);
		return true;
	}

	static bool load(string fileName,varMap &m) {
		m = varMap();
		ifstream file(fileName.c_str());
		if (!file) return false;

		m.loadedFile = new string(fileName);
		parseFile(file,m.settings,"");

        return true;
	}

	template<typename T>
	T& operator [](const string name) {
		varMapEntry entry = settings[name];
		return *((T*) entry.value);
	}

	template<typename T>
	T& get(const string name) {
		varMapEntry entry = settings[name];
		return *((T*) entry.value);
	}

	template<typename T>
	list<T> getAll(const string name) {
		list<varMapEntry*> items = list<varMapEntry*>();
		list<T> values;
		string t_type = (typeid(T)).name();
		items = settings.all(name);
		for(int i=0;i<items.getCount();i++) {
			if(t_type != items[i]->classType) continue;
			values.push(*((T*)items[i]->value));
		}
		return values;
	}

	varMap getGroup(const string name) {
		varMap child = varMap();
		child.settings = settings.children(name);
		return child;
	}

	varMap* getNewGroup(const string name) {
		varMap *child = new varMap();
		child->settings = settings.children(name);
		return child;
	}

	/*return a list of all root level
	names that are in the current map.*/
	list<string> groupNames(){
		list<mapNode<varMapEntry>*> items = settings.childrenList("");
		list<string> names = list<string>();
		string junk,tmpName;
		int i = items.getCount();
		while(i --> 0){
			if(!splitStringAtChar(items[i]->key,tmpName, junk,'.',1)) continue;
			if(names.exists(tmpName)) continue;
			names.push(tmpName);
		}

		return names;
	}

	list<string> childrenNames(){
		list<mapNode<varMapEntry>*> items = settings.childrenList("");
		list<string> names = list<string>();
		string junk,tmpName;
		int i = items.getCount();
		while(i --> 0){
			if(splitStringAtChar(items[i]->key,tmpName, junk,'.',1)) continue;
			names.push(items[i]->key);
		}

		return names;
	}

	list<string> descendantNames(){
		list<mapNode<varMapEntry>*> items = settings.childrenList("");
		list<string> names = list<string>();
		int i = items.getCount();
		while(i --> 0){
			names.push(items[i]->key);
		}

		return names;
	}



	double3 getDouble3(string name,double3 defaultValues){
			//check that string is set
		if(!isSet(name)){
			cout << "property name not found!\n";
			return defaultValues;
		}
		string str = get<string>(name);
		string tok;
		int i = 0;
		string needle = ",";
		//remove all spaces from string
		str = removeChar(str,' ');
		while(stringCutNext(str,tok,needle) && i < 3){
			if(!stringIsNumber(tok)){
				cout << "is not number\n";
				return defaultValues;
			}
			switch(i){
				case 0: defaultValues.x = stringToNumber<double>(tok); break;
				case 1: defaultValues.y = stringToNumber<double>(tok); break;
				case 2: defaultValues.z = stringToNumber<double>(tok); break;
			}
			++i;
		}
		return defaultValues;
	}
	double2 getSeperatedDouble2(string postfix,double2 defaultValues){
		return double2(
			(isSet("x-"+postfix) ? get<double>("x-"+postfix) : defaultValues.x),
			(isSet("y-"+postfix) ? get<double>("y-"+postfix) : defaultValues.y)
		);
	}

	double2 getDouble2(string name,double2 defaultValues){
			//check that string is set
		if(!isSet(name)){
			cout << "property name not found!\n";
			return defaultValues;
		}
		string str = get<string>(name);
		string tok;
		int i = 0;
		string needle = ",";
		//remove all spaces from string
		str = removeChar(str,' ');
		while(stringCutNext(str,tok,needle) && i < 2){
			if(!stringIsNumber(tok)){
				cout << "is not number\n";
				return defaultValues;
			}
			switch(i){
				case 0: defaultValues.x = stringToNumber<double>(tok); break;
				case 1: defaultValues.y = stringToNumber<double>(tok); break;
			}
			++i;
		}
		return defaultValues;
	}


	double getDouble(string name,double defaultValue){
		return double((isSet(name) ? get<double>(name) : defaultValue));
	}
	double getInt(string name,int defaultValue){
		return int((isSet(name) ? get<double>(name) : defaultValue));
	}
	string getString(string name,string defaultValue){
		return string((isSet(name) ? get<string>(name) : defaultValue));
	}

	/**\brief given a string of comma deliniated values, return a double3 of
	the first 3 values

	The alpha and boolean hasAlpha argument values will be updated with the
	value and true if a fourth value is found. If no fourth value is found,
	hasAlpha will return false.**/
	double3 getColor(string name,double3 defaultValues,double &alpha,bool &hasAlpha){
		//check that string is set
		if(!isSet(name)){
			hasAlpha = false;
			return defaultValues;
		}
		string str = get<string>(name);
		string tok;
		int i = 0;
		string needle = ",";
		//remove all spaces from string
		str = removeChar(str,' ');
		while(stringCutNext(str,tok,needle) && i < 3){
			if(!stringIsNumber(tok)){
				hasAlpha = false;
				return defaultValues;
			}

			switch(i){
				case 0: defaultValues.x = stringToNumber<double>(tok); break;
				case 1: defaultValues.y = stringToNumber<double>(tok); break;
				case 2: defaultValues.z = stringToNumber<double>(tok); break;
			}
			++i;
		}
	}
	bool getBool(string name,bool defaultValue){
		return (bool)((isSet(name) ? get<bool>(name) : defaultValue));
	}


	void print(){ settings.print(); }

	bool isLoaded(){ return (loadedFile != NULL); }

	int getCount() { return settings.getCount(); }

	bool isSet(string key) { return settings.isSet(key); }

	bool isGroup(string groupName){
		list<mapNode<varMapEntry>*> items = settings.childrenList("");
		string junk,tmpName;
		int i = items.getCount();
		while(i --> 0){
			if(!splitStringAtChar(items[i]->key,tmpName, junk,'.',1)) continue;
			if(tmpName != groupName) continue;
			return true;
		}
		return false;
	}

};




#endif // ENGINE_SETTINGS_H
