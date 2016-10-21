#ifndef GROUND_H
#define GROUND_H

#include "../core/physical.h"

class ground : public physical{
private:

	GLuint dl;

	mesh *m;

	dGeomID geom;
	dTriMeshDataID trimesh;
	dVector3 *verts;
	int vert_count;
	int *inds;
	int ind_count;
public:

	ground(const string modelFile){
		m = assetLibrary::getMesh(modelFile);
		ode::newMeshSoup(geom,trimesh,m,this,verts,vert_count,inds,ind_count);
		dl = assetLibrary::getDisplayList(modelFile);
		setStatic(true);
		setName(new string("ground_" + numberToString<unsigned long>(getId())));
	}

	~ground(){
		#if DEBUG_DELETES
			cout << " ground >";
		#endif
	}

	void draw(){
		glColor3f(1,1,1);
		glCallList(dl);
	}

	bool setEnabled(const bool enabled){
		return true;
	}

	void addForce(double3 force){}

	bool isEnabled(){ return true; }

	double3 getPosition(){return double3(0,0,0);}
};


#endif // GROUND_H
