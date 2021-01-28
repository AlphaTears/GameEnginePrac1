



#ifndef PLAYER_H_


#define PLAYER_H_
#include "Ogre.h"
#include"btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;

class Player
{
private:

	SceneNode* ninjaScenenode;
	Entity* ninjaEntity;
	AnimationState* mNinjaAnimationState;
	Vector3 meshBoundingBox;

	btCollisionShape* colShape;
	btCollisionObject* colshapes;
	btRigidBody* body;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btScalar linearDamping;
	btScalar angularDamping;
	float forwardForce, backWardsForce, leftforce, rightForce;
	btDefaultMotionState* myMotionState;
	btTransform startTransform;

public:

	Player();

	~Player();

	void createMesh(SceneManager* scnMgr);
	void attachToNode(SceneNode* parent);
	void setScale(float x, float y, float z);
	void setRotation(Vector3 axis, Radian angle);
	void setPosition(float x, float y, float z);

	void setupNinjaAnimation();
	 AnimationState* getAnimationState();

	 void WalkForward();
	 void NotMoving();
	 void attack();
	 void jumpAnimation();
	 void jumpStopAnimation();
	 void boundingBoxFromOgre();

	 void createRigidBody(float mass);
	 void addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes);
	 void addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	 //void setMass(float mass);
	 void update();
	 void forward();
	 void left();
	 void back();
	 void right();
	 void jump();
	 void setLinearVelocity(btVector3 velocity);
	 
};



#endif // !PLAYER_H_