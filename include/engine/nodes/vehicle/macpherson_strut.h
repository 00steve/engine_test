#ifndef MACPHERSON_STRUT_H
#define MACPHERSON_STRUT_H

#include "../../core/physical.h"
#include "../../core/var_map.h"
#include "../../graphics/gl_shape.h"

/**\brief the macPherson strut class is an ODE implimentation of the
MacPherson strut used in vehicles (mostly cars).


**/

#define MACPHERSON_STRUT_DRAW_LINES false


class macPhersonStrut : public physical{
private:


	dBodyID 	LCABody;
	dGeomID 	LCAGeom;
    float 		LCAGLMatrix[16];
    double3 	LCAPosition;
    double3 	LCASize;
    bool 		LCAActive;
	dJointID	LCAtoChassis;
	dJointID	LCAtoHub;

    dBodyID 	TABody;
    dGeomID 	TAGeom;
    float		TAGLMatrix[16];
    double3 	TAPosition;
    double3 	TASize;
    bool		TAActive;
    dJointID 	TAtoChassis;
    dJointID	TAtoOther;


	void buildHub(varMap settings){
	}


	void buildLowerControlArm(varMap settings){
		double3 chassisOffset = settings.getDouble3("chassis-lower-control-arm-offset",double3(0,0,0));
		double3 hubOffset = settings.getDouble3("hub-lower-control-arm-offset",double3(0,0,0));
		cout << " - chassis offset ( " << chassisOffset.x << ", " << chassisOffset.y << ", " << chassisOffset.z << ")\n";
		cout << " - hub offset     ( " << hubOffset.x << ", " << hubOffset.y << ", " << hubOffset.z << ")\n";
		LCASize = (chassisOffset-hubOffset).abs()+.1;
		double3 center = (chassisOffset + hubOffset) *.5;
		cout << " - LCA center     ( " << center.x << ", " << center.y << ", " << center.z << ")\n";

		ode::newBox(LCABody,LCAGeom,LCASize,.1,this);
		dBodySetPosition(LCABody,center.x,center.y,center.z);
        dBodyEnable(LCABody);

	}

	void buildTrailingArm(varMap settings){
		double3 chassisOffset = settings.getDouble3("chassis-trailing-arm-offset",double3(0,0,0));
		double3 otherOffset = settings.getDouble3("hub-trailing-arm-offset",double3(0,0,0));
		bool hubConnected = true;
		if(!otherOffset.length()){
			otherOffset = settings.getDouble3("lower-control-arm-trailing-arm-offset",double3(0,0,0));
            hubConnected = !otherOffset.length();
		}
		cout << " - chassis offset ( " << chassisOffset.x << ", " << chassisOffset.y << ", " << chassisOffset.z << ")\n";
		cout << " - other offset   ( " << otherOffset.x << ", " << otherOffset.y << ", " << otherOffset.z << ")\n";
		double length = (chassisOffset-otherOffset).length();
		cout << " - trailing arm length : " << length << endl;
		TASize = double3(length,.05,.05);
		double3 center = (chassisOffset + otherOffset) *.5;
		cout << " - trailing arm center( " << center.x << ", " << center.y << ", " << center.z << ")\n";

		ode::newBox(TABody,TAGeom,TASize,.1,this);
		dBodySetPosition(TABody,center.x,center.y,center.z);

		double yAngle = atan2(chassisOffset.z-otherOffset.z,chassisOffset.x-otherOffset.x);
		cout << " - trailing arm y angle : " << yAngle << endl;

		dBodySetRotation(TABody,float3x3::yRotation(yAngle).toDMatrix3());
		dBodySetTorque(TABody,0,0,0);


        dBodyEnable(TABody);
        if(hubConnected){

        } else {
			ode::newBallJoint(LCABody,TABody,TAtoOther,1,otherOffset,double3(0,0,0),double3(0,0,0));
        }

	}


public:


	macPhersonStrut(physical* chassis,varMap settings){
		setGroup(chassis->getGroup());
		buildHub(settings);
		buildLowerControlArm(settings);
		buildTrailingArm(settings);

	}

	~macPhersonStrut(){
	}

	void update(){
        if((LCAActive = dBodyIsEnabled(LCABody))){
            ode::glMatrix(LCABody,LCAGLMatrix);
            const dReal *tPos = dBodyGetPosition(LCABody);
            LCAPosition.x = tPos[0];
            LCAPosition.y = tPos[1];
            LCAPosition.z = tPos[2];
        }

        if((TAActive = dBodyIsEnabled(TABody))){
            ode::glMatrix(TABody,TAGLMatrix);
            const dReal *tPos = dBodyGetPosition(TABody);
            TAPosition.x = tPos[0];
            TAPosition.y = tPos[1];
            TAPosition.z = tPos[2];
        }



		physical::update();
	}

	void draw(){
        glPushMatrix();
            glMultMatrixf(LCAGLMatrix);
            if(LCAActive){
				glColor3f(1,1,1);
            } else {
				glColor3f(.5,.5,.5);
            }
            #if MACPHERSON_STRUT_DRAW_LINES
				glBegin(GL_LINES);
					glVertex3d(LCASize.x*-.5,0,0);
					glVertex3d(LCASize.x*.5,0,0);
				glEnd();
            #else
				glShape::drawBox(LCASize);
            #endif
        glPopMatrix();

        glPushMatrix();
            glMultMatrixf(TAGLMatrix);
            if(TAActive){
				glColor3f(1,1,1);
            } else {
				glColor3f(.5,.5,.5);
            }
            #if MACPHERSON_STRUT_DRAW_LINES
				glBegin(GL_LINES);
					glVertex3d(TASize.x*-.5,0,0);
					glVertex3d(TASize.x*.5,0,0);
				glEnd();
            #else
				glShape::drawBox(TASize);
            #endif
        glPopMatrix();


	}


	bool isEnabled(){return false;}
	bool setEnabled(bool enabled){return enabled;}
	double3 getPosition(){return double3(0,0,0);}

	/**\brief Return a given dBodyID depending on the index set.

	0 = Hub
	1 = Lower Control Arm
	2 = Trailing Arm
	**/
	dBodyID getBody(int index){
		return NULL;
	}

};

#endif // MACPHERSON_STRUT_H
