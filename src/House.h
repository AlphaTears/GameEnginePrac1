#pragma once
#ifndef HOUSE_H_

#include <exception>
#include <iostream>
#define HOUSE_H_
#include "Ogre.h"
#include"btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;

class House
{
private:

	SceneNode* HouseScenenode;
	Entity* HouseEntity;
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

	House();

	~House();

	void createMesh(SceneManager* scnMgr);
	void attachToNode(SceneNode* parent);
	void setScale();
	void setRotation(Vector3 axis, Radian angle);
	void setPosition();
	




	void boundingBoxFromOgre();

	void createRigidBody(float mass);
	void addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes);
	void addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	void update();
	



};



#endif // !HOUSE_H_
