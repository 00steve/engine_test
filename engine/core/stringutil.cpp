#include "stringutil.h"


string stringToLower(string &line){
    int l = line.size();
	while(l --> 0){
		if(line[l] < 91 && line[l] > 64){
			line[l] += 32;
		}
	}
	return line;
}

string convertChar(string line,char from,char to) {
    int x = line.length();
    while(x --> 1) {
        if(line[x] != from) continue;
		line[x] = to;
	}
	return line;
}

string removeChar(string line,char rem) {
    int x = line.length();
    while(x --> 1) {
        if(line[x] != rem) continue;
		line = line.erase(x,1);
	}
	return line;
}

string removeCharBefore(string line,char rem,char key) {
    int x = line.length();
    while(x --> 1) {
        if(line[x] != key || line[x-1] != rem) continue;
		line = line.erase(x-1,1);
	}
	return line;
}

bool lastChar(string line,char key,int &index){
	index = line.length();
	while(index --> 0){
		if(line[index] != key) continue;
		return true;
	}
	return false;
}

bool getExt(string line,string &ext){
	int endIndex;
	if(!lastChar(line,'.',endIndex)) return false;
	ext = line.substr(endIndex+1);
	return true;
}

bool getDir(string line,string &dir){
	int endIndex;
	if(!lastChar(line,'/',endIndex) && !lastChar(line,'\\',endIndex)) return false;
	dir = line.substr(0,endIndex+1);
	return true;
}

string removeRepeatingChars(string line,char target) {
    int x = line.length();
    while(x --> 1) {
        if(line[x] != target || line[x-1] != target) continue;
		line = line.erase(x-1,1);
	}
	return line;
}

bool splitStringAtChar(string line,string &before, string &after,char splitter,int instance) {
    unsigned int x = 0;
    int c = 0;
    while(x < line.length()) {
        if(line[x++] != splitter) continue;
		if(++c != instance) continue;
		before = line.substr(0,x-1);
		after = line.substr(x);
		return true;
	}
	return false;
}

/*Given a string, the chunk that should be loped off, and the thing
that should separate the two parts, do the following:
- change haystack to everything after the needle (this is a reference so the variable fed in will be changed)
- change token to everything before the needle (this is a reference so the variable fed in will be changed)
If the needle is not found, true is returned and the entire haystack is returned as token, also the haystack
is set to an empty string. If the needle is found (and the haystack
and token are set), TRUE will be returned.
*/
bool stringCutNext(string &haystack,string &token,string needle){
    unsigned int index;
    if(haystack.length() == 0) return false;
    if((index = haystack.find(needle)) == string::npos){
        token = haystack;
        haystack = "";
        return true;
    }
    token = haystack.substr(0, index);
    haystack.erase(0, index + needle.length());
    return true;
}

unsigned int stringMatchUntil(const string a,const string b){
	unsigned int index = 0;
	while(a.length() > index && b.length() > index && a[index] == b[index]){
		++index;
	}
	return index;
}

bool stringIsNumber(const string &text){
	int ind = text.length();
	bool periods = false;
	while(ind --> 0){
		if(text[ind]>47 && text[ind]<58){
			continue;
		//if a period is somewhere in the number
		} else if(text[ind] == 46 && !periods){
			periods = true;
			continue;
		//if a negative sign is the first character
		} else if(text[ind] == 45 && ind == 0){
			continue;
		}
		return false;
	}
	return true;
}

bool stringIsNumber(const string &text,char &type){
	int ind = text.length();
	bool isFloating = false;

	unsigned int props = 0;

	while(ind --> 0){
		//if the ascii value is between 0 and 9.
		if(text[ind]>47 && text[ind]<58){
			continue;
		//if a period is somewhere in the number
		//and no other period has been found
		} else if(text[ind] == 46 && !isFloating){
			props += STRING_IS_FLOATING_POINT;
			isFloating = true;
			continue;
		//if a negitive sign is the first character
		} else if(text[ind] == 45 && ind == 0){
			props += STRING_IS_SIGNED;
			continue;
		}
		return false;
	}

	switch(props)
	{
		case 0: type = 'U'; break;
		case 1: type = 'I'; break;
		case 2: type = 'F'; break;
		case 3: type = 'F'; break;
	}

	return true;
}

int bytesToInt(const char* bytes) {
	return (bytes[3] << 24) + (bytes[2] << 16) + (bytes[1] << 8) + bytes[0];
}

short int bytesToShort(const char* bytes) {
	return (bytes[1] << 8) + bytes[0];
}

bool stringGetDouble(string &haystack,double &value,string needle){
    string tmpVal = "";
    while(tmpVal == ""){
        if(!stringCutNext(haystack,tmpVal," ")) return false;
    }
    value = stringToNumber<double>(tmpVal);
    return true;
}

bool stringGetDouble3(string &haystack,double3 &value,string needle){
    if(!stringGetDouble(haystack,value.x,needle)) return false;
    if(!stringGetDouble(haystack,value.y,needle)) return false;
    if(!stringGetDouble(haystack,value.z,needle)) return false;
    return true;
}

string stringTrim(const string text){
	int s = 0;
	int e = text.length();
	int o_s = s;
	int o_e = e;
	while(s == o_s && s < e){
		s += (text[s] == 32 || text[s] == 9);
		++o_s;
	}
	while(e == o_e && e != s){
		e -= (text[e] == 32 || text[e] == 9);
		--o_e;
	}
	if(s > e){
		return "";
	}
	return text.substr(s, e-s);
}

//checks if the string is a comma delimited list of numbers of any sort
//even if it is an empty list, isNumberArray don't give a fuck.
bool stringIsNumberList(string text){
    string token;
    return stringCutNext(text,token,",")
		&& stringIsNumber(token)
		&& stringCutNext(text,token,",")
		&& stringIsNumber(token);
}

List<double>* stringToNumberList(string value){
    string token;
    List<double>* values = new List<double>();
    while(stringCutNext(value,token,",")){
        values->Push(stringToNumber<double>(token));
    }
    return values;
}
