#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include "../graphics/drawable.h"

class fileItem : public drawable{
private:

	list<fileItem*> fileItemChildren;
	virtual bool scanForChildren() = 0;

protected:

	string fullPath;
	string localPath;
	bool upToDate;

	fileItem* addFileItem(fileItem* child){
		this->addDrawable(child);
		return fileItemChildren.push(child);
	}


public:

	fileItem(string fullPath) :
			fullPath(fullPath)
			{
		upToDate = false;
	}

	~fileItem(){
		#if DEBUG_DELETES
			cout << " file_item >";
		#endif
	}

	virtual void draw() = 0;

	/*function that utilizes the private scanForChildren()
	function that all implimentation classes need to define
	to check for children down to a certain level of recursion.*/
	void refresh(int depth){
		if(!depth) return;
		--depth;
        if(!upToDate){
			scanForChildren();
			upToDate = true;
        }
		for(int i=0;i<fileItemChildren.getCount();i++){
			fileItemChildren[i]->refresh(depth);
		}
	}
};



#endif // FOLDER_H
