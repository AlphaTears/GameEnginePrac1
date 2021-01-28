#pragma once
#pragma once
#ifndef COLLUM_H_

#include <exception>
#include <iostream>
#define COLLUM_H_
#include "Ogre.h"
#include"btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;

class Collum
{
private:

	SceneNode* CollumScenenode;
	Entity* CollumEntity;
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

	Collum();

	~Collum();

	void createMesh(SceneManager* scnMgr);
	void attachToNode(SceneNode* parent);
	void setScale(float x, float y, float z);
	void setRotation(Vector3 axis, Radian angle);
	void setPosition(float x, float y, float z);





	void boundingBoxFromOgre();

	void createRigidBody(float mass);
	void addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes);
	void addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	//void setMass(float mass);
	void update();
	



};



#endif // !COLLUM_H_