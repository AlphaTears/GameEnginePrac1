﻿
#include "Player.h"
#include "Game.h"

Player::Player() 
{
	ninjaEntity = nullptr;
	ninjaScenenode = nullptr;


	linearDamping = 0.5f;
	angularDamping = 0.0f;
	forwardForce = -200.0f, backWardsForce = 200.0;

	Vector3 meshBoundingBox(0.0f, 0.0f, 0.0f);
	colShape = NULL;
	dynamicsWorld = NULL;
	

}

Player::~Player() 
{

	delete ninjaEntity;
	delete ninjaScenenode;


}


void Player::createMesh(SceneManager* scnMgr) 
{
	ninjaEntity = scnMgr->createEntity("ninja.mesh");
	ninjaEntity->setCastShadows(false);

}


void Player::attachToNode(SceneNode* parent) 
{
	ninjaScenenode = parent->createChildSceneNode();
	ninjaScenenode->attachObject(ninjaEntity);
	ninjaScenenode->setScale(1.0f,1.0f,1.0f);
	boundingBoxFromOgre();



}


void Player::setScale(float x, float y, float z) 
{
	ninjaScenenode->setScale(1.0f, 1.0f, 1.0f);
}


void Player::setPosition(float x, float y, float z)
{
	ninjaScenenode->setPosition(x, y, z);


}

void Player::setRotation(Vector3 axis, Radian angle) 
{
	Quaternion quat(angle, axis);
	ninjaScenenode->setOrientation(quat);



}





void Player::setupNinjaAnimation()
{
	
	
	mNinjaAnimationState = ninjaEntity->getAnimationState("Idle1");
	mNinjaAnimationState->setLoop(true);
	mNinjaAnimationState->setEnabled(true);
	mNinjaAnimationState->setWeight(1);
	
	
}

AnimationState* Player::getAnimationState() 
{

	return mNinjaAnimationState;
}


void Player::WalkForward() 
{

	mNinjaAnimationState = ninjaEntity->getAnimationState("Walk");
	mNinjaAnimationState->setLoop(true);
	mNinjaAnimationState->setEnabled(true);
	mNinjaAnimationState->setWeight(1);

}



void Player::NotMoving() 
{

	mNinjaAnimationState = ninjaEntity->getAnimationState("Idle1");
	mNinjaAnimationState->setLoop(true);
	mNinjaAnimationState->setEnabled(true);
	mNinjaAnimationState->setWeight(1);


}

void Player::attack() 
{


	mNinjaAnimationState = ninjaEntity->getAnimationState("Attack1");
	mNinjaAnimationState->setLoop(true);
	mNinjaAnimationState->setEnabled(true);
	mNinjaAnimationState->setWeight(1);

}

void Player::jumpAnimation()
{


	mNinjaAnimationState = ninjaEntity->getAnimationState("Jump");
	mNinjaAnimationState->setLoop(true);
	mNinjaAnimationState->setEnabled(true);
	mNinjaAnimationState->setWeight(1);


}


void Player::boundingBoxFromOgre() 
{

	ninjaScenenode->_updateBounds();
	const AxisAlignedBox& b = ninjaScenenode->_getWorldAABB();
	Vector3 temp(b.getSize());
	meshBoundingBox = temp;
	ninjaScenenode->showBoundingBox(false);

}


void Player::createRigidBody(float mass) 
{

	colShape = new btBoxShape(btVector3(meshBoundingBox.x / 2.0f, meshBoundingBox.y / 25.0f , meshBoundingBox.z / 2.0f));

	
	startTransform.setIdentity();

	Quaternion quat2 = ninjaScenenode->_getDerivedOrientation();
	startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


	Vector3 pos = ninjaScenenode->_getDerivedPosition();
	startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	btScalar Mass(mass);

	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		colShape->calculateLocalInertia(mass, localInertia);
	}

	myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	body = new btRigidBody(rbInfo);

	body->setDamping(linearDamping, angularDamping);

	body->setUserPointer((void*)this);
	

}


void Player::addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes) 
{

	collisionshapes.push_back(colShape);
}

void Player::addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld) 
{
	this->dynamicsWorld = dynamicsWorld;
	dynamicsWorld->addRigidBody(body);


}

void Player::update()
{
	//Vector3 pos = ninjaScenenode->_getDerivedPosition();
	
	//btVector3 poss(pos.x, pos.y, pos.z);

	btTransform trans;

	btQuaternion rotation = trans.getRotation();

	

	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();


		ninjaScenenode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		ninjaScenenode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
		
		//code to stop the player to rotate inside the other objects, working slighly bad


		//trans.setOrigin(poss);
		trans.setRotation(rotation);

		body->setWorldTransform(trans);
		myMotionState->setWorldTransform(trans);

		//body->applyCentralPushImpulse(btVector3(0,0,100));*/
		
	}	

}


void Player::forward() 
{
	btVector3 fwd(0.0f, 0.0f, forwardForce);
	btVector3 push;

	btTransform trans;

	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();

		push = quatRotate(orientation, fwd);


		body->activate();

		body->applyCentralForce(push);
	}



}


void Player::back()
{
	btVector3 fwd(0.0f, 0.0f, backWardsForce);
	btVector3 push;

	btTransform trans;

	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();

		push = quatRotate(orientation, fwd);


		body->activate();

		body->applyCentralForce(push);
	}



}



void Player::right()
{
	btVector3 fwd(backWardsForce, 0.0f, 0.0f);
	btVector3 push;

	btTransform trans;

	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();

		push = quatRotate(orientation, fwd);


		body->activate();

		body->applyCentralForce(push);
	}



}

void Player::jump()
{
	btVector3 fwd(0.0f, backWardsForce, 0.0f);
	btVector3 push;

	btTransform trans;

	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();

		push = quatRotate(orientation, fwd);


		body->activate();

		body->applyCentralForce(push);
	}


}



void Player::setLinearVelocity(btVector3 velocity)
{


	body->setLinearVelocity(velocity);


}




void Player::left()
{
	btVector3 fwd(forwardForce, 0.0f, 0.0f);
	btVector3 push;

	btTransform trans;

	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();

		push = quatRotate(orientation, fwd);


		body->activate();

		body->applyCentralForce(push);
	}



}




