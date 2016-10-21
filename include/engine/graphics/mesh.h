#ifndef MESH_H
#define MESH_H

#include "../core/quaternion.h"
#include "../graphics/drawable.h"
#include "../graphics/polygon.h"
#include "../graphics/material.h"
#include "../core/map.h"
#define dSINGLE
#include <ode/ode.h>

/*the mesh class stores information about a group of polygons. It
inherits the node<> template so that it can have child meshes. This
is useful for something like a character. The torso would be the
main mesh, and each limb would be a child.*/




class mesh {
protected:

	static void break_into_triangles(polygon &p,list<int> &tri,int offset){
		int i=0;
		if(p.mode == GL_TRIANGLES){
			while(i < p.vertex_count){
				tri.push(p.vertex_index[i]+offset);
				++i;
			}
		} else {
			while(i+2 < p.vertex_count){
				tri.push(p.vertex_index[0]+offset);
				tri.push(p.vertex_index[i+1]+offset);
				tri.push(p.vertex_index[i+2]+offset);
				++i;
			}
		}

	}

	/*given a mesh, place all of its polygons into a mesh list*/
	static void append_triangle_list(mesh* m,list<vertex> &v,list<int> &tri){

		int i;
		int ind_offset = v.getCount();
		for(i=0;i<m->polygonCount;i++){
			break_into_triangles(m->polygons[i],tri,ind_offset);
		}

		i = 0;
		while(i<m->vertexCount){
			v.push(m->vertices[i]);
			++i;
		}
		for(int i=0;i<m->children.getCount();i++){
            append_triangle_list(m->children[i],v,tri);
		}
	}


public:
	/*the orientation of the mesh. This is the
	offset and rotation from the origin of where
	drawing starts.*/
	double3 		offset;
	quaternion 		rotation;
	/*the main list of colors and vertices being used by
	any polygons in the mesh.*/
	vertex			*vertices;
	int				vertexCount;
	double3			*colors;
	double3			*normals;
	int				normalCount;
	/*the individual shapes being drawn by openGL. Each one
	is whatever happens between a single pair of glBegin() and
	glEnd() calls.*/
	polygon			*polygons;
	int				polygonCount;
	string 			name;
	/*store the texture information*/
	vertex 			*texcoords;
	int				texcoordCount;

	/*store children of the mesh*/
	list<mesh*> children;

	map<int> *material_index;
	list<material*> *materials;


	mesh* parent;

	mesh() :
		vertices(NULL),
		vertexCount(0),
		colors(NULL),
		normals(NULL),
		normalCount(0),
		polygons(NULL),
		polygonCount(0),
		texcoords(NULL),
		texcoordCount(0),
		children(list<mesh*>()),
		material_index(new map<int>()),
		materials(new list<material*>()),
		parent(NULL) {
	}

	void addChild(mesh* newNode){
		newNode->parent 			= this;
		newNode->material_index 	= this->material_index;
		newNode->materials 			= this->materials;
		children.push(newNode);
	}

	list<mesh*> getChildren(){return children;}

	material* addMaterial(material* newMaterial){
		materials->push(newMaterial);
		material_index->push(materials->getCount()-1,newMaterial->name);
		return materials->last();
	}

	material* &getMaterial(string name){
		return (*materials)[(*material_index)[name]];
	}

	list<material*> *getMaterials(){ return materials; }

	int getMaterialIndex(string key){
		return(*material_index)[key];
	}

	mesh* getChild(const int index){
		if(index == children.getCount()) return NULL;
		return children[index];
	}

	void genTriangleLists(dVector3 *&vert,int &vert_count,int *&indices,int &index_count){
		list<vertex> v;
		list<int> inds;
		mesh::append_triangle_list(this,v,inds);

		vert_count = v.getCount();
		vert = new dVector3[vert_count]();
        for(int i=0;i<vert_count;i++){
			vert[i][0] = (float)v[i].x;
			vert[i][2] = (float)v[i].z;
			vert[i][1] = (float)v[i].y;
        }

        index_count = inds.getCount();
        indices = new int[index_count]();
        for(int i=0;i<index_count;i++){
			indices[i] = inds[i];
        }
	}

};




#endif // MESH_H
