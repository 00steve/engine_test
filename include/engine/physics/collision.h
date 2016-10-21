#ifndef TD_COLLISION_H
#define TD_COLLISION_H

class physical;

class collision {
private:
    physical *other;
    dContactGeom *contacts;
    double3 *contact_points;
    int contact_count;
public:

    collision(physical* other,dContactGeom* contacts,int contact_count) :
			other(other),
			contacts(contacts),
			contact_count(0)
			{
		//temp just to get the ray collision
		if(contact_count >2) return;

		contact_points = new double3[contact_count];
		while(this->contact_count < contact_count){
			contact_points[this->contact_count].x = contacts[this->contact_count].pos[0];
			contact_points[this->contact_count].y = contacts[this->contact_count].pos[1];
			contact_points[this->contact_count].z = contacts[this->contact_count].pos[2];
			++this->contact_count;
		}
    }

    physical* getCollider(){
        return other;
    }

    int contactCount(){ return contact_count; }

    dContactGeom getContact(int index){ return contacts[index]; }

    dContactGeom* getContacts(){ return contacts; }

    physical* otherPhysical(){return other;}

    double3 getContactPoint(const int index){ return contact_points[index]; }

};


#endif // TD_COLLISION_H
