#include "Jaiqua.h"
using std::cout;
using std::endl;

//all coments in the player apply here

Jaiqua::Jaiqua()
{
	JaiquaEntity = nullptr;
	JaiquaScenenode = nullptr;


	linearDamping = 0.5f;
	angularDamping = 0.0f;


	Vector3 meshBoundingBox(0.0f, 0.0f, 0.0f);
	colShape = NULL;
	dynamicsWorld = NULL;


}

Jaiqua::~Jaiqua()
{

	delete JaiquaEntity;
	delete JaiquaScenenode;


}


void Jaiqua::createMesh(SceneManager* scnMgr)
{

	JaiquaEntity = scnMgr->createEntity("jaiqua.mesh");
	JaiquaEntity->setCastShadows(false);
	JaiquaEntity->setMaterialName("textures/blue_jaiqua");

}


void Jaiqua::attachToNode(SceneNode* parent)
{



	JaiquaScenenode = parent->createChildSceneNode();
	JaiquaScenenode->attachObject(JaiquaEntity);
	JaiquaScenenode->setScale(10.0f, 10.0f, 10.0f);
	boundingBoxFromOgre();


}


void Jaiqua::setPosition()
{
	x = 0, y = 10, z = -1000;

	JaiquaScenenode->setPosition(x, y, z);


}

void Jaiqua::setRotation(Vector3 axis, Radian angle)
{
	Quaternion quat(angle, axis);
	JaiquaScenenode->setOrientation(quat);



}




void Jaiqua::boundingBoxFromOgre()
{

	JaiquaScenenode->_updateBounds();
	const AxisAlignedBox& b = JaiquaScenenode->_getWorldAABB();
	Vector3 temp(b.getSize());
	meshBoundingBox = temp;
	JaiquaScenenode->showBoundingBox(false);

}


void Jaiqua::createRigidBody(float mass)
{

	colShape = new btBoxShape(btVector3(meshBoundingBox.x / 2.0f, meshBoundingBox.y / 2.5f, meshBoundingBox.z / 2.0f));


	startTransform.setIdentity();

	Quaternion quat2 = JaiquaScenenode->_getDerivedOrientation();
	startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


	Vector3 pos = JaiquaScenenode->_getDerivedPosition();
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


void Jaiqua::addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes)
{

	collisionshapes.push_back(colShape);
}

void Jaiqua::addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	this->dynamicsWorld = dynamicsWorld;
	dynamicsWorld->addRigidBody(body);


}

void Jaiqua::update()
{
	Vector3 pos = JaiquaScenenode->_getDerivedPosition();

	btVector3 poss(pos.x, pos.y, pos.z);

	btTransform trans;

	btQuaternion rotation = trans.getRotation();



	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();


		JaiquaScenenode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		JaiquaScenenode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));

		trans.setOrigin(poss);
		trans.setRotation(rotation);

		body->setWorldTransform(trans);
		myMotionState->setWorldTransform(trans);

		//body->applyCentralPushImpulse(btVector3(0,0,100));

	}

}