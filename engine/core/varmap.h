#ifndef VAR_MAP_H
#define VAR_MAP_H

#include "map.h"
#include "list.h"
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <string>
#include "stringutil.h"

/*the settings class stores any settings that can be configured in the engine.*/
using namespace std;

class VarMapEntry {

public:
	string classType;
	void* value;
	/*used to start grouping things together*/
	VarMapEntry* parent;

	VarMapEntry() :
		classType(""),
		value(NULL),
		parent(NULL) {
	}

	~VarMapEntry();

	VarMapEntry(void* value,string classType) :
		classType(classType),
		value(value),
		parent(NULL) {
	}

};


#define GROUP_START_BRACKET '{'
#define GROUP_END_BRACKET '}'
#define LIST_START_BRACKET '['
#define LIST_END_BRACKET ']'

class VarMap {
private:
	/*keep a map that translates the name of a setting
	into an engine_settings_nugget that contains the type
	and value of a nugget of setting.*/
	map<VarMapEntry> settings;

	/*keeps track of if a file has been loaded to set all
	of the settings.*/
	string* loadedFile;

    /**\brief The name of the VarMap.

    If the VarMap is obtained using the GetGroup() function, the name of the group
    is given to the VarMap. Otherwise it isn't set or something, deal with it. It might
    change in the future, but probably not, depending on if I feel like it.*/
	string name;

	static bool NextLineChunk(string &line,string &chunk,char &del,List<char> chars) {
		unsigned int index = line.length();
		if(!index) return false;
		unsigned int i;
		int x = chars.GetCount();
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

	/**\brief A recursive function to load all of the properties out
	of an opened file.

	If an object is found that starts on a line, the file and newly created child map will be
	sent to a new parse file function until the object is finished reading.*/
	static bool ParseFile(ifstream &file,map<VarMapEntry> &m,string domain,char objectOrList) {
	    /* if list characters are used, start building a list of var map entries,
        or end it. */
	    List<VarMapEntry> currentListItems;
	    int next_id = 0;

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
		char rchar = ' ';

		/*when a value is stored, we first need to figure out which type it is.
		This is the variable that will be used to do that.*/
		char num_type;

		List<char> reserved_chars = List<char>();
		reserved_chars.Push(' ');
		reserved_chars.Push('#');
		reserved_chars.Push(GROUP_START_BRACKET);
		reserved_chars.Push(GROUP_END_BRACKET);
		reserved_chars.Push(LIST_START_BRACKET);
		reserved_chars.Push(LIST_END_BRACKET);


		while (!file.eof()) {
			getline(file, line);
			/*a bunch of functions to clean the string up and
			get it ready for being parsed*/
			line = stringTrim(line);
			line = removeCharBefore(line,' ',GROUP_START_BRACKET);
			line = convertChar(line,'\t',' ');
			line = removeRepeatingChars(line,' ');
			type = "";
			while(NextLineChunk(line,chunk,rchar,reserved_chars)) {
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
					} else {
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
				case GROUP_START_BRACKET:
					if(chunk == "" && type == "" && last_chunk != "" && objectOrList != 'l'){
						chunk = last_chunk;
					}
					//if is a list, and there is no name set, give it the next
					//value. This lets you create lists of random shit without
					//specifying a name for every fucking little piece of shit
					//each line above is the same length, I think that is cool
					if(objectOrList == 'l' && chunk == ""){
                        chunk = numberToString<int>(next_id++);
					}
					//cout << " - [object] - " << chunk << endl;
					type = "object";
					line = stringTrim(line);
					ParseFile(file,m,domain+chunk+".",'o');
					break;
				case GROUP_END_BRACKET:
					//cout << " - [end object]\n";
					type = "endobject";
					return true;
					break;
				case LIST_START_BRACKET:
					if(chunk == "" && type == "" && last_chunk != ""){
						chunk = last_chunk;
					}
					//cout << " - [list] - " << chunk << endl;
					type = "list";
					line = stringTrim(line);
                    ParseFile(file,m,domain+chunk+".",'l');
                    break;
                case LIST_END_BRACKET:
					//cout << " - [end list]\n";
					type = "endlist";
					return true;
					break;

				}//end of if is special character switch statement


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

					//cout << "SAVE: " << domain << last_key << " = " << value << endl;
					key = domain + key;
					if(stringIsNumberList(value)){
                        List<double> *values = stringToNumberList(value);
                        Push<List<double> >(m,values,key);
					} else if(stringIsNumber(value,num_type)) {
						if(stringToNumber<int>(value) == stringToNumber<double>(value)){
							num_type = 'I';
						} else {
							num_type = 'D';
						}
						//overwrite num type no matter what, will load as a double,
						//otherwise for values like 1 or 85, we would need to specifically
						//tell it that the value is a d for some reason.
						num_type = 'D';
						switch(num_type) {
							case 'U': Push<unsigned int>(m,new unsigned int(stringToNumber<unsigned int>(value)),key); break;
							case 'I': Push<int>(m,new int(stringToNumber<int>(value)),key); break;
							case 'F': Push<float>(m,new float(stringToNumber<float>(value)), key); break;
							case 'D': Push<double>(m,new double(stringToNumber<double>(value)), key); break;
						}
						//cout << "   - NUMBER (" << num_type << ")\n";
					} else if(value == "true") {
						Push<bool>(m,new bool(true),key);
					} else if(value == "false") {
						Push<bool>(m,new bool(false),key);
					} else {
						Push<string>(m,new string(value),key);
						//cout << "   - STRING " << value << endl;
					}

					//cout << "Key : " << key << " = " << value << endl;
					key = "";
					value = "";
				}
				last_key = key;
				last_value = value;
				last_chunk = chunk;
				last_type = type;
			}//end of looping through chunks in line

		}
		return true;
	}

public:

	VarMap() :
		settings(map<VarMapEntry>()),
		loadedFile(NULL),
		name("") {
	}

	~VarMap();

	template<typename T>
	bool Add(T* value,string name){
		settings.push(VarMapEntry((void*) value,(typeid(T)).name()),name);
		return true;
	}

	/*new engine settings can be mapped dynamically*/
	template<typename T>
	static bool Push(map<VarMapEntry> &m,T* value,string name) {
		m.push(VarMapEntry((void*) value,(typeid(T)).name()),name);
		return true;
	}

	static bool Load(string fileName,VarMap &m){
		m = VarMap();
		ifstream file(fileName.c_str());
		if (!file.is_open()){
			cout << "couldn't load file " << fileName << endl;
			return false;
		}
		m.loadedFile = new string(fileName);
		ParseFile(file,m.settings,"",'o');
        return true;
	}

	template<typename T>
	T& operator [](const string name) {
		VarMapEntry entry = settings[name];
		return *((T*) entry.value);
	}

	template<typename T>
	T& get(const string name) {
		VarMapEntry entry = settings[name];
		return *((T*) entry.value);
	}

	List<string> GetStringList(const string name){
		List<string> values;
		string haystack = get<string>(name);
		string token = "";
		while(stringCutNext(haystack,token,",")){
			values.Push(token);
		}
		return values;
	}

	template<typename T>
	List<T> GetAll(const string name) {
		List<VarMapEntry*> items = List<VarMapEntry*>();
		List<T> values;
		string t_type = (typeid(T)).name();
		items = settings.all(name);
		for(int i=0;i<items.GetCount();i++) {
			if(t_type != items[i]->classType) continue;
			values.Push(*((T*)items[i]->value));
		}
		return values;
	}

	VarMap GetGroup(const string name) {
		VarMap child = VarMap();
		child.settings = settings.children(name);
		child.name = name;
		return child;
	}

	VarMap* getNewGroup(const string name) {
		VarMap *child = new VarMap();
		child->settings = settings.children(name);
		child->name = name;
		return child;
	}

	/*return a list of all root level
	names that are in the current map.*/
	List<string> GroupNames(){
		List<mapNode<VarMapEntry>*> items = settings.childrenList("");
		List<string> names = List<string>();
		string junk,tmpName;
		int i = items.GetCount();
		while(i --> 0){
			if(!splitStringAtChar(items[i]->key,tmpName, junk,'.',1)) continue;
			if(names.Exists(tmpName)) continue;
			names.Push(tmpName);
		}
		return names;
	}

	void Print(){ settings.print(); }

	bool IsLoaded(){ return (loadedFile != NULL); }

	int GetCount() { return settings.getCount(); }

	bool IsSet(string key) { return settings.isSet(key); }

    string Name(){ return name; }
};




#endif // ENGINE_SETTINGS_H
