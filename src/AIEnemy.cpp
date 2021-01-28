#include "AIEnemy.h"


using std::cout;
using std::endl;

AIEnemy::AIEnemy()
{
	AIEnemyEntity = nullptr;
	AIEnemyScenenode = nullptr;
	
	
	

	linearDamping = 0.5f;
	angularDamping = 0.0f;


	Vector3 meshBoundingBox(0.0f, 0.0f, 0.0f);
	colShape = NULL;
	dynamicsWorld = NULL;


}

AIEnemy::~AIEnemy()
{

	delete AIEnemyEntity;
	delete AIEnemyScenenode;


}


void AIEnemy::createMesh(SceneManager* scnMgr)
{

	AIEnemyEntity = scnMgr->createEntity("robot.mesh");
	AIEnemyEntity->setCastShadows(false);
	AIEnemyEntity->setMaterialName("textures/PaperEnemyTexture");

}


void AIEnemy::attachToNode(SceneNode* parent)
{



	AIEnemyScenenode = parent->createChildSceneNode();
	AIEnemyScenenode->attachObject(AIEnemyEntity);
	boundingBoxFromOgre();


}


void AIEnemy::setScale()
{
	AIEnemyScenenode->setScale(5.0f, 5.0f, 5.0f);
}


void AIEnemy::setPosition()
{
	x = -700, y = 50, z = -300;

	AIEnemyScenenode->setPosition(x, y, z);


}

void AIEnemy::setAIEnemyanimation()
{

	mEnemyAnimationState = AIEnemyEntity->getAnimationState("Walk");
	mEnemyAnimationState->setLoop(true);
	mEnemyAnimationState->setEnabled(true);


}

void AIEnemy::setRotation(Vector3 axis, Radian angle)
{
	Quaternion quat(angle, axis);
	AIEnemyScenenode->setOrientation(quat);



}




void AIEnemy::boundingBoxFromOgre()
{

	AIEnemyScenenode->_updateBounds();
	const AxisAlignedBox& b = AIEnemyScenenode->_getWorldAABB();
	Vector3 temp(b.getSize());
	meshBoundingBox = temp;
	AIEnemyScenenode->showBoundingBox(false);

}


void AIEnemy::createRigidBody(float mass)
{

	colShape = new btBoxShape(btVector3(meshBoundingBox.x / 2.0f, meshBoundingBox.y / 2.5f, meshBoundingBox.z / 2.0f));


	startTransform.setIdentity();

	Quaternion quat2 = AIEnemyScenenode->_getDerivedOrientation();
	startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


	Vector3 pos = AIEnemyScenenode->_getDerivedPosition();
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


void AIEnemy::addToCollisionShapes(btAlignedObjectArray<btCollisionShape*>& collisionshapes)
{

	collisionshapes.push_back(colShape);
}

void AIEnemy::addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	this->dynamicsWorld = dynamicsWorld;
	dynamicsWorld->addRigidBody(body);


}

void AIEnemy::update()
{
	Vector3 pos = AIEnemyScenenode->_getDerivedPosition();

	btVector3 poss(pos.x, pos.y, pos.z);

	btTransform trans;

	btQuaternion rotation = trans.getRotation();



	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();


		AIEnemyScenenode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		AIEnemyScenenode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));

		trans.setOrigin(poss);
		trans.setRotation(rotation);

		body->setWorldTransform(trans);
		myMotionState->setWorldTransform(trans);

		//body->applyCentralPushImpulse(btVector3(0,0,100));

	}

}

bool AIEnemy::AImovement()
{



	for (int i = 0; i < 2; i++)
	{

		AIEnemyScenenode->setPosition(x = x, y, z = z + i);

		std::cout << i << std::endl;

	}

	return true;
}

void AIEnemy::SetAIIdleAniamtion()
{

	mEnemyAnimationState = AIEnemyEntity->getAnimationState("Idle");
	mEnemyAnimationState->setLoop(true);
	mEnemyAnimationState->setEnabled(true);
}

SceneNode* AIEnemy::getSceneNode()
{
	return AIEnemyScenenode;
}

AnimationState* AIEnemy::getanimationState()
{
	return mEnemyAnimationState;
}

