#include "varmap.h"

VarMapEntry::~VarMapEntry(){
	//cout << " - destry varmap::entry\n";
	//if(value) delete dynamic_cast<classType>(value);
}

VarMap::~VarMap(){
    //cout << "destroy varmap\n";

}
