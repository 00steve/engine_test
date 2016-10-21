#ifndef FILE_FOLDER_H
#define FILE_FOLDER_H

#include "../file/file_item.h"

class fileFolder : public fileItem{
private:

	bool scanForChildren(){

		WIN32_FIND_DATA ffd;
		LARGE_INTEGER filesize;
		TCHAR szDir[MAX_PATH];
		size_t length_of_arg;
		string searchPath;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD dwError=0;

		length_of_arg = fullPath.length();

		if (length_of_arg > (MAX_PATH - 3))
		{
			cout << "Directory path is too long.\n";
			return false;
		}

		searchPath = fullPath + "\\*";
		cout << "Target directory is " << searchPath << "\n";

		// Find the first file in the directory.
		hFind = FindFirstFile(searchPath.c_str(), &ffd);
		if (INVALID_HANDLE_VALUE == hFind) {
			cout << "no valid file and shit\n";
			return false;
		}

		// List all the files in the directory with some info about them.

		do {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				cout << " - " << ffd.cFileName << " <DIR>\n";
				cout << "     FOLDER " << fullPath << "\\" << ffd.cFileName << endl;
				this->addFileItem(new fileFolder(string(fullPath + "\\" + ffd.cFileName)));
			} else {
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				cout << " - " << ffd.cFileName << " : " << filesize.QuadPart << " bytes\n";
				//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			}
		} while (FindNextFile(hFind, &ffd) != 0);


		dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES) {
			cout << "something went wrong!\n";
		}

		FindClose(hFind);
		return true;
	}





public:

	fileFolder(string fullPath) :
				fileItem(fullPath)
			{
	}

	~fileFolder(){
		#if DEBUG_DELETES
			cout << " folder >";
		#endif
	}

	string type(){ return "folder"; }

	void draw(){
	}


};



#endif // FILE_FOLDER_H
