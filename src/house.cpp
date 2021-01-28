#include "House.h"



using std::cout;
using std::endl;

House::House()
{
	HouseEntity = nullptr;
	HouseScenenode = nullptr;


	linearDamping = 0.5f;
	angularDamping = 0.0f;


	Vector3 meshBoundingBox(0.0f, 0.0f, 0.0f);
	colShape = NULL;
	dynamicsWorld = NULL;


}

House::~House()
{

	delete HouseEntity;
	delete HouseScenenode;


}


void House::createMesh(SceneManager* scnMgr)
{

	HouseEntity = scnMgr->createEntity("tudorhouse.mesh");
	HouseEntity->setCastShadows(false);
	HouseEntity->setMaterialName("textures/houseTexture");

}


void House::attachToNode(SceneNode* parent)
{



	HouseScenenode = parent->createChildSceneNode();
	HouseScenenode->attachObject(HouseEntity);
	boundingBoxFromOgre();


}


void House::setScale()
{
	HouseScenenode->setScale(0.7f, 0.7f, 0.7f);
}


void House::setPosition()
{
	x = 0, y = 350, z = -2000;

	HouseScenenode->setPosition(x, y, z);


}



void House::setRotation(Vector3 axis, Radian angle)
{
	Quaternion quat(angle, axis);
	HouseScenenode->setOrientation(quat);



}




void House::boundingBoxFromOgre()
{

	HouseScenenode->_updateBounds();
	const AxisAlignedBox& b = HouseScenenode->_getWorldAABB();
	Vector3 temp(b.getSize());
	meshBoundingBox = temp;
	HouseScenenode->showBoundingBox(false);

}


void House::createRigidBody(float mass)
{

	colShape = new btBoxShape(btVector3(meshBoundingBox.x / 2.0f, meshBoundingBox.y / 2.5f, meshBoundingBox.z / 2.0f));


	startTransform.setIdentity();

	Quaternion quat2 = HouseScenenode->_getDerivedOrientation();
	startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


	Vector3 pos = HouseScenenode->_getDerivedPosition();
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


void House::addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes)
{

	collisionshapes.push_back(colShape);
}

void House::addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	this->dynamicsWorld = dynamicsWorld;
	dynamicsWorld->addRigidBody(body);


}

void House::update()
{
	Vector3 pos = HouseScenenode->_getDerivedPosition();

	btVector3 poss(pos.x, pos.y, pos.z);

	btTransform trans;

	btQuaternion rotation = trans.getRotation();



	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();


		HouseScenenode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		HouseScenenode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));

		trans.setOrigin(poss);
		trans.setRotation(rotation);

		body->setWorldTransform(trans);
		myMotionState->setWorldTransform(trans);

		//body->applyCentralPushImpulse(btVector3(0,0,100));

	}

}