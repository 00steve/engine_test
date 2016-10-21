#ifndef MARKER_H
#define MARKER_H

/*a marker is a static object that marks a location.
It can be dragged around and displayed in different ways.

If should be used to place objects that should, themselves,
not be seen, like a spawner, unless for some crazy reason
 the spawner should look like stuff, like if there is a
 teleporter, that should be placed somewhere nad thingimbobs
 pop out of if, then that should be displayed.
*/


#include "../core/physical.h"

class marker : public physical{
private:

	static GLUquadricObj* pQuadric;
	bool enabled;

    /**\brief store the shape that should be collided against other shapes*/
    dGeomID geom;

	int slices;

protected:

	double radius;
	double3 position;


	void build_geom(){
        ode::newStaticSphere(geom,radius,this);
        dGeomSetPosition(geom,position.x,position.y,position.z);
        setStatic(true);
        setVirtual(true);
		slices = 10+radius;
	}


public:

	marker(varMap settings){
    	radius = setup::getDouble(settings,"radius",1);
        position = setup::getDouble3(settings,"offset",double3(0,0,0));
        enabled = setup::getBool(settings,"enabled",true);
        build_geom();
        setName(new string("marker_" + numberToString<unsigned long>(getId())));
	}

	marker(const double3 position,const double radius) :
			enabled(true),
			radius(radius)
			{
		this->position = position;
		build_geom();
		setName(new string("marker_" + numberToString<unsigned long>(getId())));
	}

	~marker(){
		#if DEBUG_DELETES
			cout << " marker >";
		#endif
		ode::removeGeom(geom);
	}

	/*
	void draw(){
		glPointSize(radius*2);
		glBegin(GL_POINTS);
			glVertex3d(position.x,position.y,position.z);
		glEnd();
	}*/

    void draw(){
        glPushMatrix();
            glColor4f(1,0,0,.5);
            glTranslated(position.x,position.y,position.z);
            glPushAttrib(GL_POLYGON_BIT);
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            gluSphere(marker::pQuadric,radius,slices,slices);
            glPopAttrib();
        glPopMatrix();
    }


	/**\brief fucking fuck that needs to be from physical class
	to allow the editor to move stuff around and things*/
	void shiftPosition(const double3 offset){
		position += offset;
		dGeomSetPosition(geom,position.x,position.y,position.z);
	}

	double3 getPosition(){return position;}

	bool isEnabled(){ return enabled; }
	bool setEnabled(const bool enabled){ return this->enabled = enabled; }




};


GLUquadricObj* marker::pQuadric = gluNewQuadric();

#endif // MARKER_H
