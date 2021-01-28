#pragma once
#ifndef ENEMY_H_

#include <exception>
#include <iostream>
#define ENEMY_H_
#include "Ogre.h"
#include"btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;

class enemy
{
private:

	SceneNode* EnemyScenenode;
	Entity* EnemyEntity;
	AnimationState* mEnemyAnimationState;
	Vector3 meshBoundingBox;
	AnimationState* mFishAnimation;


	btCollisionShape* colShape;
	btCollisionObject* colshapes;
	btRigidBody* body;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btScalar linearDamping;
	btScalar angularDamping;
	btDefaultMotionState* myMotionState;
	btTransform startTransform;
	int x, y, z;
public:

	enemy();

	~enemy();

	void createMesh(SceneManager* scnMgr);
	void attachToNode(SceneNode* parent);
	void setScale(float x, float y, float z);
	void setRotation(Vector3 axis, Radian angle);
	void setPosition();
	void setFishanimation();
	

	

	void boundingBoxFromOgre();
	
	void createRigidBody(float mass);
	void addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes);
	void addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	//void setMass(float mass);
	void update();
	bool AImovement();
	AnimationState* getanimation();
	

};



#endif // !ENEMY_H_