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
#include "list.h"
#include <sstream>
#include <iostream>
using namespace std;


#define STRING_IS_SIGNED 1
#define STRING_IS_FLOATING_POINT 2
#define STRING_IS_LONG 4


/**\brief Converts a string to lower case.

The string is passed in by reference, and is changed,
the value of the string is also returned by the function.*/
string stringToLower(string &line);


string convertChar(string line,char from,char to);

template <typename T>
string numberToString ( T Number ) {
	stringstream ss;
	ss << Number;
	return ss.str();
}


string removeChar(string line,char rem);

string removeCharBefore(string line,char rem,char key);

bool lastChar(string line,char key,int &index);

bool getExt(string line,string &ext);

bool getDir(string line,string &dir);

//removes any ocurrances of a specific char that come right
//after another char of a specific type. This is perfect for
//removing extra spaces from a string.
string removeRepeatingChars(string line,char target);

//splits a string where the n-th instance of the splitter is found. For example, if the splitter
//is a ", and the instance is set to 2, the function will skip over the first " and split the string,
//returning before and after split by the index of the next ".
bool splitStringAtChar(string line,string &before, string &after,char splitter,int instance);

bool stringCutNext(string &haystack,string &token,string needle);

//will check each character of both strings to make sure that they
//match, upon the first character that doesn't, it will return the
//current index.
unsigned int stringMatchUntil(const string a,const string b);

//iterates through all of the characters in a string. If any
//non numerical chars or , . -  (at valid places), it will return
//false.
bool stringIsNumber(const string &text);

//checks to see if a string is a number and if it is,
//figures out which type it is.
bool stringIsNumber(const string &text,char &type);

template <typename T>
T stringToNumber ( const string Text ) {
	stringstream ss(Text);
	T result;
	return ss >> result ? result : 0;
}

int bytesToInt(const char* bytes);

short int bytesToShort(const char* bytes);

bool stringGetDouble(string &haystack,double &value,string needle);

bool stringGetDouble3(string &haystack,double3 &value,string needle);

//removes any junk characters from beginning and end of the
//given string. The currently trimmed chars are  (space),tab,
string stringTrim(const string text);


bool isNumberArray(const string text);

bool stringIsNumberList(const string text);

List<double>* stringToNumberList(string value);
#endif // STRING_UTIL_H
