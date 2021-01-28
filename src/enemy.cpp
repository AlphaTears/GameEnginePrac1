#include "enemy.h"

//all coments in the player apply here

using std::cout;
using std::endl;

enemy::enemy()
{
	EnemyEntity = nullptr;
	EnemyScenenode = nullptr;


	linearDamping = 0.5f;
	angularDamping = 0.0f;
	

	Vector3 meshBoundingBox(0.0f, 0.0f, 0.0f);
	colShape = NULL;
	dynamicsWorld = NULL;


}

enemy::~enemy()
{

	delete EnemyEntity;
	delete EnemyScenenode;


}


void enemy::createMesh(SceneManager* scnMgr)
{
	
	EnemyEntity = scnMgr->createEntity("fish.mesh");
	EnemyEntity->setCastShadows(false);
	EnemyEntity->setMaterialName("textures/steelhead");
	
}


void enemy::attachToNode(SceneNode* parent)
{
	

	
	EnemyScenenode = parent->createChildSceneNode();
	EnemyScenenode->attachObject(EnemyEntity);
	EnemyScenenode->setScale(20.0f, 20.0f, 20.0f);
	boundingBoxFromOgre();
	

}


void enemy::setScale(float x, float y, float z)
{
	EnemyScenenode->setScale(1.0f, 1.0f, 1.0f);
}


void enemy::setPosition()
{
	x = 0, y = 100, z = -300;

	EnemyScenenode->setPosition(x, y, z);


}

void enemy::setFishanimation()
{

	mFishAnimation = EnemyEntity->getAnimationState("swim");
	mFishAnimation->setLoop(true);
	mFishAnimation->setEnabled(true);
	

}

void enemy::setRotation(Vector3 axis, Radian angle)
{
	Quaternion quat(angle, axis);
	EnemyScenenode->setOrientation(quat);



}




void enemy::boundingBoxFromOgre()
{

	EnemyScenenode->_updateBounds();
	const AxisAlignedBox& b = EnemyScenenode->_getWorldAABB();
	Vector3 temp(b.getSize());
	meshBoundingBox = temp;
	EnemyScenenode->showBoundingBox(false);

}


void enemy::createRigidBody(float mass)
{

	colShape = new btBoxShape(btVector3(meshBoundingBox.x / 2.0f, meshBoundingBox.y / 2.5f, meshBoundingBox.z / 2.0f));


	startTransform.setIdentity();

	Quaternion quat2 = EnemyScenenode->_getDerivedOrientation();
	startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


	Vector3 pos = EnemyScenenode->_getDerivedPosition();
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


void enemy::addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes)
{

	collisionshapes.push_back(colShape);
}

void enemy::addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	this->dynamicsWorld = dynamicsWorld;
	dynamicsWorld->addRigidBody(body);


}

void enemy::update()
{
	Vector3 pos = EnemyScenenode->_getDerivedPosition();

	btVector3 poss(pos.x, pos.y, pos.z);

	btTransform trans;

	btQuaternion rotation = trans.getRotation();



	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();


		EnemyScenenode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		EnemyScenenode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));

		trans.setOrigin(poss);
		trans.setRotation(rotation);

		body->setWorldTransform(trans);
		myMotionState->setWorldTransform(trans);

		//body->applyCentralPushImpulse(btVector3(0,0,100));

	}

}

bool enemy::AImovement()
{

	

	for  ( int i = 0;  i < 2; i++)
	{

		EnemyScenenode->setPosition(x = x , y, z  = z + i);
		
		std::cout << i << std::endl;

	}

	return true;
}

AnimationState* enemy::getanimation()
{
	return mFishAnimation;
}


