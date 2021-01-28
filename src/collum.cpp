#include "Collum.h"



using std::cout;
using std::endl;

Collum::Collum()
{
	CollumEntity = nullptr;
	CollumScenenode = nullptr;


	linearDamping = 0.5f;
	angularDamping = 0.0f;


	Vector3 meshBoundingBox(0.0f, 0.0f, 0.0f);
	colShape = NULL;
	dynamicsWorld = NULL;


}

Collum::~Collum()
{

	delete CollumEntity;
	delete CollumScenenode;


}


void Collum::createMesh(SceneManager* scnMgr)
{
	CollumEntity = scnMgr->createEntity("column.mesh");
	CollumEntity->setCastShadows(false);
	CollumEntity->setMaterialName("textures/BumpyMetal");
}


void Collum::attachToNode(SceneNode* parent)
{
	CollumScenenode = parent->createChildSceneNode();
	CollumScenenode->attachObject(CollumEntity);
	CollumScenenode->setScale(1.0f, 1.0f, 1.0f);
	boundingBoxFromOgre();
	


}


void Collum::setScale(float x, float y, float z)
{
	CollumScenenode->setScale(x, y, z);
}


void Collum::setPosition(float x, float y, float z)
{
	

	CollumScenenode->setPosition(x, y, z);


}

void Collum::setRotation(Vector3 axis, Radian angle)
{
	Quaternion quat(angle, axis);
	CollumScenenode->setOrientation(quat);



}




void Collum::boundingBoxFromOgre()
{

	CollumScenenode->_updateBounds();
	const AxisAlignedBox& b = CollumScenenode->_getWorldAABB();
	Vector3 temp(b.getSize());
	meshBoundingBox = temp;
	CollumScenenode->showBoundingBox(false);

}


void Collum::createRigidBody(float mass)
{

	colShape = new btBoxShape(btVector3(meshBoundingBox.x / 2.0f, meshBoundingBox.y / 2.5f, meshBoundingBox.z / 2.0f));


	startTransform.setIdentity();

	Quaternion quat2 = CollumScenenode->_getDerivedOrientation();
	startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


	Vector3 pos = CollumScenenode->_getDerivedPosition();
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


void Collum::addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes)
{

	collisionshapes.push_back(colShape);
}

void Collum::addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	this->dynamicsWorld = dynamicsWorld;
	dynamicsWorld->addRigidBody(body);


}



void Collum::update()
{
	Vector3 pos = CollumScenenode->_getDerivedPosition();

	btVector3 poss(pos.x, pos.y, pos.z);

	btTransform trans;

	btQuaternion rotation = trans.getRotation();



	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();


		CollumScenenode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		CollumScenenode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));

		trans.setOrigin(poss);
		trans.setRotation(rotation);

		body->setWorldTransform(trans);
		myMotionState->setWorldTransform(trans);

		//body->applyCentralPushImpulse(btVector3(0,0,100));

	}
}



