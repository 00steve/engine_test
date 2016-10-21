#ifndef DRAW_MESH_H
#define DRAW_MESH_H

#include "../graphics/mesh.h"



void drawMesh(mesh *m) {
	int current_polygon = 0;
	int current_vertex = 0;
	int vertex_index;
	int normal_index;
	int texcoord_index;
	//int texture_id;

	//cout << "Normals in mesh " << m->name << " : " << m->normalCount << endl;
	while(current_polygon < m->polygonCount) {
		//cout << "bind texture : " << (*m->materials)[m->polygons[current_polygon].mtl_index]->diffuse_id << endl;
		glBindTexture(GL_TEXTURE_2D,(*m->materials)[m->polygons[current_polygon].mtl_index]->diffuse_id);
		glBegin(m->polygons[current_polygon].mode);
		current_vertex = 0;
		vertex_index = 0;
		texcoord_index = 0;
		while(current_vertex < m->polygons[current_polygon].vertex_count) {
			vertex_index = m->polygons[current_polygon].vertex_index[current_vertex];
			normal_index = m->polygons[current_polygon].normal_index[current_vertex];
			texcoord_index = m->polygons[current_polygon].texcoord_index[current_vertex];
			//glColor3d(.5,.5,.5);
			if(m->polygons[current_polygon].normal_count > current_vertex) {
				glNormal3d(m->normals[normal_index].x,m->normals[normal_index].y,m->normals[normal_index].z);
			}
			glTexCoord2d(m->polygons[current_polygon].texcoords[texcoord_index].x,m->polygons[current_polygon].texcoords[texcoord_index].y);
			glVertex3d(m->polygons[current_polygon].vertices[vertex_index].x,
					   m->polygons[current_polygon].vertices[vertex_index].y,
					   m->polygons[current_polygon].vertices[vertex_index].z);
			++current_vertex;

		}

		glEnd();
		++current_polygon;
	}
	mesh *child;
	int child_index = 0;

	while((child = m->getChild(child_index++))) {
		drawMesh(child);
	}
}


GLuint generate_display_list(mesh *m){
	GLuint l = glGenLists(1);
	glNewList(l,GL_COMPILE);
	drawMesh(m);
	glEndList();
	return l;
}

#endif // DRAW_MESH_H
