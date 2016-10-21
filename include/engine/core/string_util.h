/*
String utils is a group of functions that I commonly use to manipulate or
get information from strings that are not built into the string class.

numberToString() and stringToNumber() were stolen from
http://www.cplusplus.com/forum/articles/9645/.

*/


#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <string.h>
#include "double3.h"
#include <sstream>

#define STRING_IS_SIGNED 1
#define STRING_IS_FLOATING_POINT 2
#define STRING_IS_LONG 4


/**\brief converts an ascii string to lowercase alpha characters.

Any ascii values between 64 and 91 (A-Z) are shifted by 32
to (a-z).**/
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

template <typename T>
string numberToString ( T Number ) {
	stringstream ss;
	ss << Number;
	return ss.str();
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

/**\brief finds the last instance of a character in a string.

The function returns true if at least 1 instance of the character
is found, and false if no instances of the character is found. The
argument &index is given the index of the string that the character
is found at.**/
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

/**\brief removes any occurances of a specific char that come right
after another char of a specific type.

This is perfect for removing extra spaces from a string.**/
string removeRepeatingChars(string line,char target) {
    int x = line.length();
    while(x --> 1) {
        if(line[x] != target || line[x-1] != target) continue;
		line = line.erase(x-1,1);
	}
	return line;
}

/**\brief splits a string where the n-th instance of the splitter is found.

For example, if the splitter is a ", and the instance is set to 2, the function
will skip over the first " and split the string, returning before and after split
by the index of the next ".**/
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

/**\brief will check each character of both strings to make sure that they
match.

Upon the first character that doesn't, it will return the current index.**/
unsigned int stringMatchUntil(const string a,const string b){
	unsigned int index = 0;
	while(a.length() > index && b.length() > index && a[index] == b[index]){
		++index;
	}
	return index;
}

/**\brief iterates through all of the characters in a string.

If any non numerical chars or , . -  (at valid places), it will return false.**/
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
		//if a negitive sign is the first character
		} else if(text[ind] == 45 && ind == 0){
			continue;
		}
		return false;
	}
	return true;
}

/**\brief checks to see if a string is a number and if it is,
figures out which type it is.**/
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

template <typename T>
T stringToNumber ( const string Text ) {
	stringstream ss(Text);
	T result;
	return ss >> result ? result : 0;
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
        if(!stringCutNext(haystack,tmpVal,needle)) return false;
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

/**\brief removes any junk characters from beginning and end of the
given string.

The currently trimmed chars are  (space),tab,**/
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



#endif // STRING_UTIL_H
