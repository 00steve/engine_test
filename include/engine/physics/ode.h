#ifndef DEngine_ode_H
#define DEngine_ode_H

#define dSINGLE
#include <ode/ode.h>
#include "../physics/ode_utils.h"
#include "../core/double3.h"
#include "../graphics/gl_structs.h"
#include "collision.h"
#include "../core/physical.h"
#include "../core/float3x3.h"





class ode {

private:
	static bool initialized;

    static dWorldID worldId;
    static dSpaceID spaceId;
    static dJointGroupID contactGroup;

	static double stepSize;

    // this is called by dSpaceCollide when two objects in space are
    // potentially colliding.

    static void nearCallback (void *data, dGeomID o1, dGeomID o2) {

        /*see if any of the objects that are colliding have info about
        them that can be checked.*/
        physical* nA = (physical*)dGeomGetData(o1);
        physical* nB = (physical*)dGeomGetData(o2);
        /*if both either of the geoms doesn't have a physical, or both
        physicals are static or both physicals are virtual, bail from
        the collision check.

        20150924 - If one of the geoms is static or virtual and the other
        geom is also static or virtual, don't count the collision*/
        if(!nA || !nB
			|| (nA->isStatic() && nB->isStatic())
			//|| ((nA->isStatic() || nA->isVirtual()) && (nB->isStatic() || nB->isVirtual()))
		){
			return;
		}

		/*check if either of the objects is not grouped, and if they are both
		grouped, make sure they are not in the same group*/
        if(!nA->isGrouped() || !nB->isGrouped() || nA->getGroup() != nB->getGroup()){

            /*check to see if the two physical objects contain contact information
            that should be used to determine how the contact point behaves*/
            dSurfaceParameters* csA = nA->getSurfaceParameters(o1);
            dSurfaceParameters* csB = nB->getSurfaceParameters(o2);
            dContact contact;
            if(!csA && !csB){ //if neither geom has surface properties
                contact.surface.mode = dContactBounce | dContactSoftCFM;// | dContactFDir1;//dContactSlip1  | dContactSlip2 ;
                contact.surface.mu = 2;
                contact.surface.bounce = 0.2;
                contact.surface.bounce_vel = 0.6;
                contact.surface.soft_cfm = 0.005;
            } else if(csA && !csB){ //if only geom A has surface properties
                contact.surface = *csA;
            } else if(csB && !csA){ //if only geom B has surface properties
                contact.surface = *csB;
            } else{  //if both geoms have surface properties
            	cout << "warning, no logic implimented if both objects have surface properties. Fix this shit!\n";
            }
            dContactGeom contacts[8];
            if (int numc = dCollide (o1,o2,8,contacts,sizeof(dContactGeom))) {
                dBodyID b1 = dGeomGetBody(o1);
                dBodyID b2 = dGeomGetBody(o2);
                /*make sure that both physicals are not virtual before
                generating contact points*/
                if(!nA->isVirtual() && !nB->isVirtual()){
                    for(int i=0;i<numc;i++){
                        contact.geom = contacts[i];
                        dJointID c = dJointCreateContact (worldId,contactGroup,&contact);
                        dJointAttach (c,b1,b2);
                    }//end of looping through the collision points of the two bodies
                }//end of if both physicals are not virtual
                nA->addCollision(new collision(nB,contacts,numc));
                nB->addCollision(new collision(nA,contacts,numc));
            }//end of if both geoms have a physical that they are attached to
        }//end of if the two geometries collided
        else {
			cout << "neither geom is grouped\n";
        }
    }


	static int dTriCallback(dGeomID triMesh,dGeomID obj,int triangleIndex){
		cout << "TRIANGLE COLLISION!";
		return 1;
	}


public:

    static void init(){
    	if(initialized) return;
		initialized = true;

		stepSize = .01;

        #if EVENT_OUTPUT
            cout << "CREATE physics\n";
        #endif
        dInitODE();

        worldId = dWorldCreate();
        //dQuadTreeSpaceCreate (dSpaceID space, dVector3 Center, dVector3 Extents, int Depth);
        spaceId = dHashSpaceCreate(0);
        contactGroup = dJointGroupCreate(0);
/*
        dWorldSetGravity (worldId, 0, -9.81,0 );
        dWorldSetERP (worldId, 0.2);	//the springiness of everything
        dWorldSetCFM (worldId, 1e-5);//3e-5 (old) //the softness of everything
        dWorldSetAutoDisableFlag(worldId,true);
        dWorldSetQuickStepNumIterations (worldId, 10);
        dWorldSetContactMaxCorrectingVel (worldId, 5);
        dWorldSetAutoDisableLinearThreshold(worldId,0.000001f);
        dWorldSetAutoDisableAngularThreshold(worldId,0.0000001f);
        dWorldSetAutoDisabool ode::initialized = false;
double ode::stepSize;
dWorldID ode::worldId;
dSpaceID ode::spaceId;
dJointGroupID ode::contactGroup;
bleTime(worldId,.25);
        */
        dWorldSetGravity (worldId, 0, -9.81,0 );
        dWorldSetERP (worldId, 0.10);	//the springiness of everything
        dWorldSetCFM (worldId, 0.02);//3e-5 (old) //the softness of everything
        dWorldSetAutoDisableFlag(worldId,true);
        dWorldSetQuickStepNumIterations (worldId, 10);
        dWorldSetContactMaxCorrectingVel (worldId, 5);
        dWorldSetAutoDisableLinearThreshold(worldId,0.000001f);
        dWorldSetAutoDisableAngularThreshold(worldId,0.0000001f);
        dWorldSetAutoDisableTime(worldId,.25);

    }

    static void destroy(){
    	if(!initialized) return;
    	initialized = false;
        #if EVENT_OUTPUT
            cout << "DESTROY physics\n";
        #endif
        dJointGroupDestroy (contactGroup);
        dSpaceDestroy(spaceId);
        dWorldDestroy(worldId);
        dCloseODE();
    }

    static void step(){
        // remove all contact joints
        dJointGroupEmpty (contactGroup);

        // find collisions and add contact joints
        dSpaceCollide (spaceId,0,&nearCallback);
        // step the simulation
        dWorldQuickStep(worldId,stepSize);

    }

	static void setAutoDisable(bool autoDisable){
		dWorldSetAutoDisableFlag(worldId,autoDisable);
	}

    /*builds a box of a set size with a set density
    and adds the body to the world and the geom
    to the space, then returns a reference to that
    body.

    The last variable is a reference to the node that the
    box will be a part of. This is required so that when
    collisions are performed, the engine can determine what
    to do with the body. If it is set to null, no callback
    will be performed.*/
    static void newBox(dBodyID &body,dGeomID &geom,double3 dimensions,double density,void* data){
        body = dBodyCreate(worldId);
        dMass m;
        dMassSetZero(&m);
        dMassSetBox(&m, density, dimensions.x, dimensions.y, dimensions.z);
        dBodySetMass(body, &m);
        geom = dCreateBox(spaceId,dimensions.x,dimensions.y,dimensions.z);
        dGeomSetBody (geom, body);
        dGeomSetData(geom,data);
    }

    /*does the same things as newBox, but with a sphere, it creates a sphere
    of the given radius with teh given density equally distributed throughout
    the sphere. IF a node is passed in, it is stored in the body for future
    reference.*/
    static void newSphere(dBodyID &body,dGeomID &geom,double radius,double density,void* data){
        body = dBodyCreate(worldId);
        dMass m;
        dMassSetZero(&m);
        dMassSetSphere(&m, density, radius);
        dBodySetMass(body, &m);
        geom = dCreateSphere(spaceId,radius);
        dGeomSetBody (geom, body);
        dGeomSetData(geom,data);
    }

    /*generates a capsule with the given height and radius, with the mass
    that was argued.*/
    static void newCapsule(dBodyID &body,dGeomID &geom,double height,double radius,double density, void* data){
        body = dBodyCreate(worldId);
        dMass m;
        dMassSetZero(&m);
        dMassSetCapsule(&m,density,2,radius,height);
        geom = dCreateCapsule(spaceId,radius,height);
        dGeomSetBody(geom,body);
        dMatrix3 rot;
        dRSetIdentity(rot);
        dRFromAxisAndAngle(rot,1,0,0,1.57f);
        dGeomSetRotation(geom,rot);
        dGeomSetData(geom,data);
    }

    /*generates a cylinder given the height and radius.*/
    static void newCylinder(dBodyID &body,dGeomID &geom,double length,double radius,double mass, void* data){
        body = dBodyCreate(worldId);
        dMass m;
        dMassSetZero(&m);
        dMassSetCylinder(&m,mass,3,radius,length);
        geom = dCreateCylinder(spaceId,radius,length);
        dGeomSetBody(geom,body);
        dMatrix3 rot;
        dRSetIdentity(rot);
        dRFromAxisAndAngle(rot,1,0,0,1.57f);
        dGeomSetRotation(geom,rot);


        dGeomSetData(geom,data);
    }

    static void newMeshSoup(dGeomID &geom,dTriMeshDataID &triMesh,mesh *m,void* data,dVector3 *&vertices,int &vert_count, int *&indices,int &index_count){
		m->genTriangleLists(vertices,vert_count,indices,index_count);
        triMesh = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildSimple(triMesh,(dReal*)vertices,vert_count,
									(dTriIndex*)indices,index_count);
        geom = dCreateTriMesh(spaceId, triMesh, NULL, NULL, NULL);
        dGeomSetData(geom, data);
        dGeomSetPosition(geom, 0, 0, 0);
        //dGeomTriMeshSetCallback(geom,&dTriCallback);

    }

    static void newStaticSphere(dGeomID &geom,double radius,void* data){
        geom = dCreateSphere(spaceId,radius);
        dGeomSetData(geom,data);
    }

    static void newStaticPlane(dGeomID &geom,double3 direction,double offset,void* data){
        direction.normalize();
        geom = dCreatePlane (spaceId,direction.x,direction.y,direction.z,offset);
        dGeomSetData(geom,data);
    }

    static void newStaticRay(dGeomID &geom,double3 position,double3 normal,double length,void* data){
        geom = dCreateRay (spaceId, length);
        dGeomRaySet(geom,position.x,position.y,position.z,normal.x,normal.y,normal.z);
        //dGeomRaySetLength(geom,length);
        dGeomSetData(geom,data);

    }

    /*creates a joint that holds two bodies together. It can be broken and constrains
    the bodies in whatever way is needed. The two bodies are the bodies that will be
    attached together with the joint. The strength is the amount of force that is
    required to break the joint. The orienation is the desired resting rotation of the
    joint.*/
    static void newBallJoint(dBodyID &body_a,dBodyID &body_b,dJointID &joint,double strength,double3 anchor,double3 orientation,double3 rotation){
        joint = dJointCreateBall(worldId,NULL);
        dJointAttach(joint,body_a,body_b);
        dJointSetBallAnchor (joint, anchor.x, anchor.y, anchor.z);
        //dJointSetFixed (joint);
    }

	static void newSliderJoint(dBodyID &body_a,dBodyID &body_b,dJointID &joint,double stiffness,double3 anchor,double3 orientation){
		joint = dJointCreateSlider(worldId,NULL);
		dJointAttach(joint,body_a,body_b);
		dJointSetSliderAxis(joint,0,1,0);
		dJointSetSliderParam(joint,dParamBounce,.0001);
		dJointSetSliderParam(joint,dParamCFM,.5);
	}

    static void offsetCOG(dBodyID &body,double3 offset){
        //dMass m;
        //dBodyGetMass(body,&m);
        //dMassTranslate(&m,offset.x,offset.y,offset.z);
        //dBodySetMass(body,&m);
    }

    static void removeGeom(dGeomID &geom){
        dSpaceRemove (spaceId, geom);
    }

    static void removeBody(dBodyID &body){
        dBodyDestroy(body);
    }

    //static void newMesh(dGeomID &geom,
    static void glMatrix(dBodyID body,float *matrix){
        const dReal *R = dBodyGetRotation(body);
        const dReal *pos = dBodyGetPosition(body);
        matrix[0]=R[0];
        matrix[1]=R[4];
        matrix[2]=R[8];
        matrix[3]=0;
        matrix[4]=R[1];
        matrix[5]=R[5];
        matrix[6]=R[9];
        matrix[7]=0;
        matrix[8]=R[2];
        matrix[9]=R[6];
        matrix[10]=R[10];
        matrix[11]=0;
        matrix[12]=pos[0];
        matrix[13]=pos[1];
        matrix[14]=pos[2];
        matrix[15]=1;
    }

    static void glRotationMatrix(dBodyID body,float *matrix){
        const dReal *R = dBodyGetRotation(body);
        matrix[0]=R[0];
        matrix[1]=R[1];//4
        matrix[2]=R[2];//8
        matrix[3]=0;

        matrix[4]=R[4];//1
        matrix[5]=R[5];
        matrix[6]=R[6];//9
        matrix[7]=0;

        matrix[8]=R[8];//2
        matrix[9]=R[9];//6
        matrix[10]=R[10];
        matrix[11]=0;

        matrix[12]=0;
        matrix[13]=0;
        matrix[14]=0;
        matrix[15]=1;
    }

    static void glTranslationMatrix(dBodyID body,float *matrix){
        const dReal *pos = dBodyGetPosition(body);
        matrix[0]=1;
        matrix[1]=0;//4
        matrix[2]=0;//8
        matrix[3]=0;

        matrix[4]=0;//1
        matrix[5]=1;
        matrix[6]=0;//9
        matrix[7]=0;

        matrix[8]=0;//2
        matrix[9]=0;//6
        matrix[10]=1;
        matrix[11]=0;

        matrix[12]=-pos[0];
        matrix[13]=-pos[1];
        matrix[14]=-pos[2];
        matrix[15]=1;
    }

    static dWorldID getWorldId(){ return worldId; }
    static dSpaceID getSpaceId(){ return spaceId; }

    static bool isInitialized(){ return initialized; }

	static double getStepSize(){return stepSize;}

};

bool ode::initialized = false;
double ode::stepSize;
dWorldID ode::worldId;
dSpaceID ode::spaceId;
dJointGroupID ode::contactGroup;


#endif // 3DEngine_ode_H
