#ifndef BULLET_H
#define BULLET_H

#include "../../core/projectile.h"

class bullet : public projectile{
private:

    dGeomID ray;
    double3 direction;
    double3 position;
    double3 normal;
    double velocity;
    double mass;

    bool has_closest_collision;
    double3 closest_collision;
    int contact_count;

public:

    bullet(double3 position,double3 direction,float mass) :
        direction(direction),
        position(position),
        normal(direction.normal()),
        velocity(direction.length()),
        mass(mass),
        has_closest_collision(false),
        closest_collision(double3(0,0,0))
    {
        ode::newStaticRay(ray,position,normal,velocity*.01,this);

        dGeomRaySetParams( ray, false, false );
        dGeomRaySetClosestHit(ray,true);
        setVirtual(true);
    }

    ~bullet(){
        #if DEBUG_DELETES
        cout << "delete bullet >";
        #endif
        ode::removeGeom(ray);
    }


    void update(){
        position+=direction*.01f;
        dGeomRaySet(ray,position.x,position.y,position.z,normal.x,normal.y,normal.z);
		dGeomRaySetLength(ray,direction.length()*.01f);
        if(collisions.getCount()){
            collision *c = collisions[dGeomRayGetClosestHit(ray)-1];
            if(c->contactCount()){
				impact *i = new impact(c->getContactPoint(0),direction,1);
                sendMessage(c->otherPhysical(),MESSAGE_PROJECTILE_IMPACT,(void*)i);
            }
            this->kill();
        } else {
        	/*only call default projectile update if there are no collisions. This
        	will check to see if the bullet has existed for however many ticks. if
        	it has, it is removed so bullets don't go flying off forever.*/
			projectile::update();
        }
    }

    void draw(){
        glLineWidth(6);
        glColor4f(1,.5,0,.8f);
        glBegin(GL_LINES);
            glNormal3f(1,1,1);
            glVertex3d(position.x,position.y,position.z);
            glVertex3d(position.x+direction.x*.01f,position.y+direction.y*.01f,position.z+direction.z*.01f);
        glEnd();
    }

    double3 getPosition(){ return position; }
	bool isEnabled(){ return true; }
	bool setEnabled(const bool enabled){
		return true;
	}

};

#endif // BULLET_H

