#ifndef PARTICLE_H
#define PARTICLE_H

/**\brief an object that follows a generic set of rules to
move around and be displayed for a short period of time, then
cease to exist.

A perfect example of something that should be a particle is a
flame of a fire, or a blob of smoke, or a spark, etc. Particles
are basicallly eye candy (like neutrinos) and don't really interact
with anything, unless some class inherits from the particle and
decides to give it bastard children with a physical class, then
the particles could start interacting with other physicals and *that
could get hella-interesting.

*by hella-interesting, I mean it will probably slow your computer to
a crawl unless you are a boy genious with the brain of 1000 donkeys (
20 men). Don't judge me, I did the math.**/
class particle : public drawable{
private:

	/*store a list of all particles that exist. When there
	are too many, remove the oldest ones.*/
	static orderedList<particle*> all_particles;

	static int max_particle_count;

	/*the amount that gavity influcences the acceleration of
	the particle. Things that rise would be something like
	0,-.1,0*/
	double3 gravity_influence;
	double air_dampening;

	void init(){
		//loop through the list of all particles and remove
		//them until there are only as many as are allowed
		//by the max_particle_count variable
		//cout << "particle count : " << all_particles.getCount() << endl;
		while(all_particles.getCount() >= max_particle_count){
			delete all_particles.unshift();
		}

		//add the new particle to the list of all particles
		//this list is kept so that particles can be removed
		//from the system when there are too many of them.
		all_particles.push(this);

		state = 1;
		ticks = 0;
		fadeInForTickCount = 100;
		fadeOutStartTickCount = 300;
		fadeOutForTickCount = 100;

	}

	int ticks;


protected:

	double3 position;
	double3 velocity;

	double3 camAngle;

	int state;

	int groupForTickCount;
	int shrinkForTickCount;
	int fadeOutStartTickCount;
	int fadeOutForTickCount;
	int fadeInForTickCount;

	double alpha;

public:

	particle(double3 gravity_influence,double air_dampening,double3 position,double3 velocity) :
				gravity_influence(gravity_influence),
				air_dampening(air_dampening),
				position(position),
				velocity(velocity)
			{
		init();
	}

	~particle(){
		all_particles.cut(all_particles.getItemIndex(this));
		#if DEBUG_DELETES
			cout << " particle >";
		#endif
	}



	void update(){
		velocity = (gravity_influence *.01 + velocity) * air_dampening;
		position += velocity *.01;
		//cout << "particle";
		/*get angle based on distance between camera and particle*/
		//camAngle = position - getCurrentCameraPosition();


		++ticks;

		switch(state){
		case 1://fade in
			if(fadeInForTickCount <= ticks){
				state = 2;
				ticks = 0;
			} else {
				alpha = (float)ticks / (float)fadeInForTickCount;
			}
			break;
		case 2://solid
			if(fadeOutStartTickCount - fadeInForTickCount - ticks < 1){
                state = 3;
                ticks = 0;
			}
			break;
		case 3://fade out

			if(fadeOutForTickCount <= ticks){
				return delete this;
			} else {
				alpha = 1 - ((float)ticks / (float)fadeOutForTickCount);
			}
		}

		drawable::update();
	}

	void draw(){

		glPushMatrix();
			double3 ofs = (getCurrentCameraPosition()-position).normalize();

			double ata = atan2(ofs.x,ofs.z);
			double bta = atan2(ofs.y*ofs.y,ofs.x*ofs.x+ofs.z*ofs.z);
			glTranslated(position.x,position.y,position.z);
			glRotated(ata*360/6.28f,0,1,0);
			glRotated(bta*360/6.28f,1,0,0);

			glColor4f(1,1,1,alpha);
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLE_FAN);

				glVertex3f(-.5,.5,0);
				glVertex3f(-.5,-.5,0);
				glVertex3f(.5,-.5,0);
				glVertex3f(.5,.5,0);


			glEnd();

			glEnable(GL_TEXTURE_2D);
		glPopMatrix();

	}

};


orderedList<particle*> particle::all_particles = orderedList<particle*>();
int particle::max_particle_count = 10;
#endif // PARTICLE_H
