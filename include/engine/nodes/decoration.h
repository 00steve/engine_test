#ifndef DECORATION_H
#define DECORATION_H

#include "../graphics/drawable.h"
#include "../graphics/mesh.h"
#include "../load/obj.h"

/*a decoration is a simple entity that basically displays
a mesh, but doesn't do anything else.*/


class decoration : public drawable {
private:

	mesh *m;

public:

	decoration(mesh* newMesh) :
		m(newMesh) {
	}

	decoration(string file_name) {

		m = new mesh();
		load_obj* loader = new load_obj(file_name,m);
		while(!loader->IsComplete()){
		}
		m = loader->get_mesh();
	}

	void draw() {
		drawMesh(m);
	}

	void update() {

	}

	mesh*& Mesh(){ return m; }

};








#endif // DECORATION_H
