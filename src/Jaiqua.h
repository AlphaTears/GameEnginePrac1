#pragma once
#ifndef JAIQUA_H_

#include <exception>
#include <iostream>
#define JAIQUA_H_
#include "Ogre.h"
#include"btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;

class Jaiqua
{
private:

	SceneNode* JaiquaScenenode;
	Entity* JaiquaEntity;
	//AnimationState* PenguinAnimationState;
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

	Jaiqua();

	~Jaiqua();

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



#endif // !JAIQUA_H_
