#ifndef GL_SHAPE_H
#define GL_SHAPE_H

#include "mesh.h"

/*
using the mesh class, generate various crazy shapes procedurally,
using maths and other smart people stuff.
*/

class glShape{
public:

	static mesh dashedRing(double radius,double thickness,int dashCount,double dashBias){

		return mesh();
	}



	static mesh sphere(double radius,int verticalSections,int horizontalSections){
		mesh m = mesh();

		polygon poly = polygon();
		list<double3> v;
		list<int> i;


		/*generate the bottom point*/
		i.push(v.getCount());
		v.push(double3(0,-radius,0));

		/*generate the intermediate points around the sphere*/
		int y = verticalSections;
		int x = horizontalSections;
		while(y --> 1){
			while(x --> 1){
				//i.push(p.getCount());
				//v.push(double3(sin((x/horizontalSections)*3.14),sin((y/verticalSections)*3.14),cos((z/horizontalSections)*3.14)));
			}//end of creating each point on each band around sphere
		}//end of creating each band of points


		/*generate the top point*/
		i.push(v.getCount());
		v.push(double3(0,radius,0));

		/*build bottom polygon*/

		/*build side polygons*/

		/*build top polygon*/

		poly.mode = GL_POINTS;
		poly.vertex_count = i.getCount();
		poly.vertex_index = &i[0];
		poly.vertices = &v[0];

		m.polygons = &poly;
		m.polygonCount = 1;
		m.vertexCount = v.getCount();
		m.vertices = &v[0];

		return m;
	}


	static void drawBox(double3 d){
		glBegin(GL_TRIANGLE_FAN);
			glVertex3d(d.x*.5,d.y*.5,d.z*.5);
			glVertex3d(d.x*.5,d.y*-.5,d.z*.5);
			glVertex3d(d.x*.5,d.y*-.5,d.z*-.5);
			glVertex3d(d.x*.5,d.y*.5,d.z*-.5);
			glVertex3d(d.x*-.5,d.y*.5,d.z*-.5);
			glVertex3d(d.x*-.5,d.y*.5,d.z*.5);
			glVertex3d(d.x*-.5,d.y*-.5,d.z*.5);
			glVertex3d(d.x*.5,d.y*-.5,d.z*.5);
		glEnd();

		glBegin(GL_TRIANGLE_FAN);


			glVertex3d(d.x*-.5,d.y*-.5,d.z*-.5);
			glVertex3d(d.x*-.5,d.y*.5,d.z*-.5);
			glVertex3d(d.x*.5,d.y*.5,d.z*-.5);
			glVertex3d(d.x*.5,d.y*-.5,d.z*-.5);
			glVertex3d(d.x*.5,d.y*-.5,d.z*.5);
			glVertex3d(d.x*-.5,d.y*-.5,d.z*.5);
			glVertex3d(d.x*-.5,d.y*.5,d.z*.5);

			glVertex3d(d.x*-.5,d.y*.5,d.z*-.5);
		glEnd();

	}

};



#endif // SHAPE_H
