#ifndef ASSET_DEPENDENCY_H
#define ASSET_DEPENDENCY_H

#include "../core/loader.h"

struct dependency{

	/*store type type of dependency this thing is. It is completely
	dependent(haha) on the object being loaded that is saying it needs
	whatever is being loaded. If a 3d model needs a texture, it could set
	the id to 1 or 158498, and when the texture has been loaded, the loader
	loading the 3d model will call the finalize function and pass this
	object so the 3d model loader will know if it is id 1 or 158498 that it
	is a texture and should know what to do with it.*/
	int type;

	/*store some data about the dependency, again this is a wildcard available
	to the calling load object.*/
	void* load_data;

	/*a secondary data source that can store any object the caller requires*/
	void* ref_data;

	/*store the object that has been loaded so that it can be used by the
	caller when it is passed back to the finilize function.*/
	void* object_reference;

	/*store the file that should be loaded.*/
	string file;

};
#endif // DEPENDENCY_H
