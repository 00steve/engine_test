#ifndef FILE_COMPUTER_H
#define FILE_COMPUTER_H

#include "../file/file_drive.h"

class fileComputer : public fileItem{

private:
	bool scanForChildren(){
		cout << "scan computer ::\n";
		DWORD dwSize = MAX_PATH;
		char szLogicalDrives[MAX_PATH] = {0};
		DWORD dwResult = GetLogicalDriveStrings(dwSize,szLogicalDrives);
		if(dwResult == 0 || dwResult > MAX_PATH) return false;

		char* szSingleDrive = szLogicalDrives;
		while(*szSingleDrive) {
			this->addFileItem(new fileDrive(string(szSingleDrive).substr(0,strlen(szSingleDrive)-1)));
			//this->addFileItem(new fileDrive(szSingleDrive));
			szSingleDrive += strlen(szSingleDrive) + 1;
		}

		return true;
	}

public:

	fileComputer() :
			fileItem("")
			{
	}

	~fileComputer(){
		#if DEBUG_DELETES
			cout << " computer >";
		#endif
	}

	string type(){
		return "computer";
	}

	void draw(){
	}



};


#endif // FILE_COMPUTER_H
