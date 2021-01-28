
#pragma once
#ifndef AIENEMY_H_

#include <exception>
#include <iostream>
#define AIENEMY_H_
#include "Ogre.h"
#include"btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;

class AIEnemy
{
private:

	

	SceneNode* AIEnemyScenenode;
	Entity* AIEnemyEntity;
	AnimationState* mEnemyAnimationState;
	Vector3 meshBoundingBox;
	



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

	AIEnemy();

	~AIEnemy();

	void createMesh(SceneManager* scnMgr);
	void attachToNode(SceneNode* parent);
	void setScale();
	void setRotation(Vector3 axis, Radian angle);
	void setPosition();
	void setAIEnemyanimation();




	void boundingBoxFromOgre();

	void createRigidBody(float mass);
	void addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes);
	void addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	//void setMass(float mass);
	void update();
	bool AImovement();
	void SetAIIdleAniamtion();

	SceneNode* getSceneNode();
	AnimationState* getanimationState();

};



#endif // !ENEMY_H_