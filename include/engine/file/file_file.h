#ifndef FILE_FILE_H
#define FILE_FILE_H

#include "../file/file_item.h"

class fileFile : public fileItem{
private:

	bool scanForChildren(){
		return false;
	}


public:

	fileFile(string fullPath) :
			fileItem(fullPath)
			{
	}

	string type(){ return "folder"; }

	void draw(){
	}


};



#endif // FILE_FOLDER_H

