#pragma once
#ifndef PENGUIN_H_

#include <exception>
#include <iostream>
#define PENGUIN_H_
#include "Ogre.h"
#include"btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;

class Penguin
{
private:

	SceneNode* penguinScenenode;
	Entity* penguinEntity;
	AnimationState* PenguinAnimationState;
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

	Penguin();

	~Penguin();

	void createMesh(SceneManager* scnMgr);
	void attachToNode(SceneNode* parent);
	
	void setRotation(Vector3 axis, Radian angle);
	void setPosition();


	void boundingBoxFromOgre();

	void createRigidBody(float mass);
	void addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes);
	void addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	void update();


};



#endif // !PENGUIN_H_