/*the dll class is an interface that allows an operating system to load dlls*/
#ifndef SYNENGINE_DLL_H
#define SYNENGINE_DLL_H
#include <windows.h>

class eDLL {
private:

	HINSTANCE hInstLibrary;//the windows reference to the dll
	char* filename;

public:
	eDLL(const char* filename)
		{
		this->filename = (char*)filename;//store the name of the dll file
		hInstLibrary = LoadLibrary(filename);//attempt to load the current .dll
		if(!hInstLibrary) cout << "Failed to load " << filename << "\n";
		}
	~eDLL()
		{
		FreeLibrary(hInstLibrary);
		delete filename;
		}
	bool IsLoaded(){return (bool) GetModuleHandle(filename);}
	HINSTANCE Lib(){return hInstLibrary;}
	const char* FileName(){return filename;}
};


template <typename classX> static classX GetDllValue(eDLL* dll,const char* function)
    {
    typedef classX (*GetClass)();
    GetClass newClass = (GetClass)::GetProcAddress(dll->Lib(),function);
    classX temp = newClass();
    if(temp) return temp;
    return NULL;
    };

#endif
