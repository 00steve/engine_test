#ifndef FILE_DRIVE_H
#define FILE_DRIVE_H

#include "../file/file_folder.h"

/*this class is just a placeholder in case drives are
ever treated differently than folders. For all intents and
purposes, right now, a drive is a folder.*/


class fileDrive : public fileFolder{
private:

public:

	fileDrive(string fullPath) :
			fileFolder(fullPath)
			{
	}

};


#endif
