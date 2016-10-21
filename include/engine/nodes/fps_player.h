#ifndef FPS_PLAYER_H
#define FPS_PLAYER_H

#include "../core/actor.h"
#include "../physics/contact_settings.h"


class fpsPlayer : public actor,public contact_settings{
private:

	static GLUquadricObj* 	pQuadric;
	dSurfaceParameters*		surface;
	varMap settings;

    double radius;
	double height;

	/*right now there is only one camera that can be
	returned, it is the head_cam, so it is called
	head_cam, don't like it? fuck yaself!*/
	camera *head_cam;
	double3 cameraPosition;

	bool isPrimaryInteracting;

	void setup_physics(){
        ode::newCapsule(body,geom,height-(radius*2),radius,.1,this);
        //dBodySetAutoDisableLinearThreshold (body, .5);
        //dBodySetAutoDisableAngularThreshold (body, .06);
        dBodySetAutoDisableSteps (body, 25);
        //make sure that physics object can not rotate on any axii
        dBodySetMaxAngularSpeed(body,0);

		//setup the contact properties
		surface = new dSurfaceParameters();
		surface->mode = dContactBounce | dContactSoftCFM;// | dContactSlip1 | dContactSlip2;// | dContactFDir1;// ;
		surface->mu = 2.0f;
		surface->bounce = .1;
		surface->bounce_vel = .1;
		surface->soft_cfm = 0.005;
		surface->slip1 = .1;
		surface->slip2 = .1;
	}

public:

	fpsPlayer(varMap settings) :
				settings(settings),
				cameraPosition(double3(0,0,0)),
				isPrimaryInteracting(false)
			{
		radius = .35;
		height = 2;
		position = setup::getDouble3(settings,"offset",double3(0.1f,100,0));
		//do stuff with the settings
		string cameraName = settings.getString("camera","none");
        if(cameraName != "none"){
			node* camera = node::getByName(cameraName);
			if(camera){
				sendMessage(camera,MESSAGE_SET_POSITION_REFERENCE,(void*)&cameraPosition);
				sendMessage(camera,MESSAGE_SET_ROTATION_REFERENCE,(void*)&lookRotation);
			}
        }

        string controllerName = settings.getString("controller","none");
		if(controllerName != "none"){
			node* c = node::getByName(controllerName);
			if(c){
                control = (controller*)c;
			}
		}

		setup_physics();
        dBodySetPosition(body,position.x,position.y,position.z);
        //ode::glMatrix(body,gl_matrix);

        setName(settings.isSet("id") ?
			new string(settings.getString("id","")) :
			new string("fpsplayer_" + numberToString<unsigned long>(getId()))
		);


		//setPrimaryProp(new glock19());



        cout << "FPS_PLAYER name : " << getName() << endl;
	}

	~fpsPlayer(){
		#if DEBUG_DELETES
			cout << " fps player >";
		#endif
	}

	void update(){

		/*get control*/
		if(control){

			this->lookRotation.x -= control->turn_direction().y*.01f;
			this->lookRotation.y -= control->turn_direction().x*.01f;
			if(fabs(lookRotation.x) > .9f) lookRotation.x = fabs(lookRotation.x)/lookRotation.x * .9f;

			if(collisions.getCount()){
				/*add force along direction vector*/
				double3 moveamount = double3(
					(sinf(lookRotation.y)*control->move_direction().y + cosf(lookRotation.y)*control->move_direction().x),
					0,
					(cosf(lookRotation.y)*control->move_direction().y - sin(lookRotation.y)*control->move_direction().x)
				);
				dBodyEnable(body);
				addForce(moveamount*10);
			}

			/*call primary interaction function once per button press*/
			if(control->primary_interaction()){
				if(!isPrimaryInteracting){
					primaryInteract();
					isPrimaryInteracting = true;
				}
			}else{
				isPrimaryInteracting = false;
			}

		}
		cameraPosition = position;
		cameraPosition.y += .5;
		actor::update();
        collisions.clearAndDelete();
	}

	void draw(){
        glPushMatrix();
            //glMultMatrixf(gl_matrix);

			glColor3f(0,1,0);
			glBegin(GL_LINES);
				glVertex3f(0,0,height*-.5);
				glVertex3f(0,0,height*.5);
			glEnd();

			glTranslated(0,0,-radius*2);
			gluCylinder(fpsPlayer::pQuadric,radius,radius,height-radius*2,30,1);
			glColor3f(1,0,0);
			gluSphere(fpsPlayer::pQuadric,radius,30,30);
			glColor3f(0,0,1);
			glTranslated(0,0,height-radius*2);
			gluSphere(fpsPlayer::pQuadric,radius,30,30);

        glPopMatrix();



	}

    virtual dSurfaceParameters* getSurfaceParameters(dGeomID g){
    	if(g == geom){
			return surface;
    	}
        return NULL;
    }



};



GLUquadricObj* fpsPlayer::pQuadric = gluNewQuadric();


#endif // FPS_PLAYER_H
