#include "Penguin.h"
using std::cout;
using std::endl;

//all coments in the player apply here

Penguin::Penguin()
{
	penguinEntity = nullptr;
	penguinScenenode = nullptr;


	linearDamping = 0.5f;
	angularDamping = 0.0f;


	Vector3 meshBoundingBox(0.0f, 0.0f, 0.0f);
	colShape = NULL;
	dynamicsWorld = NULL;


}

Penguin::~Penguin()
{

	delete penguinEntity;
	delete penguinScenenode;


}


void Penguin::createMesh(SceneManager* scnMgr)
{

	penguinEntity = scnMgr->createEntity("penguin.mesh");
	penguinEntity->setCastShadows(false);
	penguinEntity->setMaterialName("textures/penguin");

}


void Penguin::attachToNode(SceneNode* parent)
{



	penguinScenenode = parent->createChildSceneNode();
	penguinScenenode->attachObject(penguinEntity);
	penguinScenenode->setScale(10.0f, 10.0f, 10.0f);
	boundingBoxFromOgre();


}


void Penguin::setPosition()
{
	x = -600, y = 100, z = -2000;

	penguinScenenode->setPosition(x, y, z);


}

void Penguin::setRotation(Vector3 axis, Radian angle)
{
	Quaternion quat(angle, axis);
	penguinScenenode->setOrientation(quat);



}




void Penguin::boundingBoxFromOgre()
{

	penguinScenenode->_updateBounds();
	const AxisAlignedBox& b = penguinScenenode->_getWorldAABB();
	Vector3 temp(b.getSize());
	meshBoundingBox = temp;
	penguinScenenode->showBoundingBox(false);

}


void Penguin::createRigidBody(float mass)
{

	colShape = new btBoxShape(btVector3(meshBoundingBox.x / 2.0f, meshBoundingBox.y / 2.5f, meshBoundingBox.z / 2.0f));


	startTransform.setIdentity();

	Quaternion quat2 = penguinScenenode->_getDerivedOrientation();
	startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


	Vector3 pos = penguinScenenode->_getDerivedPosition();
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


void Penguin::addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes)
{

	collisionshapes.push_back(colShape);
}

void Penguin::addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	this->dynamicsWorld = dynamicsWorld;
	dynamicsWorld->addRigidBody(body);


}

void Penguin::update()
{
	Vector3 pos = penguinScenenode->_getDerivedPosition();

	btVector3 poss(pos.x, pos.y, pos.z);

	btTransform trans;

	btQuaternion rotation = trans.getRotation();



	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();


		penguinScenenode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		penguinScenenode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));

		trans.setOrigin(poss);
		trans.setRotation(rotation);

		body->setWorldTransform(trans);
		myMotionState->setWorldTransform(trans);

		//body->applyCentralPushImpulse(btVector3(0,0,100));

	}

}

