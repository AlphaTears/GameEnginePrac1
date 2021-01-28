

#include <exception>
#include <iostream>

#include "Game.h"
#include "enemy.h"
#include "Collum.h"
using std::cout;
using std::endl;

Game::Game() : ApplicationContext("OgreTutorialApp")
{
    //defining all the variables created within our Game.h
    dynamicsWorld = NULL;
    
     
    upDown = false;
    DownDown = false;
    leftDown = false;
    rightDown = false;
    shiftDown = false;


    mDistance = 0;
    mWalkSpd = 70;
    mDirection = Vector3::ZERO;
    mDestination = Vector3::ZERO;
    mWalkList.push_back(Ogre::Vector3(-800.0, 0, 50.0));
    mWalkList.push_back(Ogre::Vector3(-100.0, 0, -200.0));
    mWalkList.push_back(Ogre::Vector3(-600, 0, -250.0));
    mWalkList.push_back(Ogre::Vector3(-800, 0, -250.0));
    mWalkList.push_back(Ogre::Vector3(-1000, 0, -250.0));
}


Game::~Game()
{
    //cleanup in the reverse order of creation/initialization

    ///Start of the cleaning of our games
    //removing the rigid bodies first

    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
      btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
    	btRigidBody* body = btRigidBody::upcast(obj);

    	if (body && body->getMotionState())
    	{
    		delete body->getMotionState();
    	}

    	dynamicsWorld->removeCollisionObject(obj);
    	delete obj;
    }

    //deleting all the collision shapes in the world
    for (int j = 0; j < collisionShapes.size(); j++)
    {
    	btCollisionShape* shape = collisionShapes[j];
    	collisionShapes[j] = 0;
    	delete shape;
    }

    //delete everything related to physics
    delete dynamicsWorld;
    delete solver;
    delete overlappingPairCache;
    delete dispatcher;
    delete collisionConfiguration;

    //This line isn't completely necessary as it will be deleted automtically.
    collisionShapes.clear();
}


void Game::setup()
{
    
    ApplicationContext::setup();

    addInputListener(this);

    // setting a pointer to the already created root
    Root* root = getRoot();
    scnMgr = root->createSceneManager();

    // regestering our scn to the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);
    //starting bullet
    bulletInit();
    //seting up the camera
    setupCamera();
    //setting every object in our world
    setupFloor();
    setupFloo4();
    setupFloo2();
    setupFloo3();
    setupWall();
    setupwall3();
    setupwall4();
    setupwall5();
    setupwall2();
    setupwall6();
    setupCollum();
    setupCollum2();
    setupPlayer();
    setupPenguin();
    setupJaiqua();
    setupMovingEnemy();
    //setting the house after everything cause sometimes it bugs with the player
    setupHouse();
    //setting up the lights 
    setupLights();

    
    
    //setting up an enemy its here cause it was for a test
    setupEnemy();

    //creating entities for the walking paths of our robot 
    Ogre::Entity* ent;
    Ogre::SceneNode* node;

    ent = scnMgr->createEntity("knot.mesh");
    node = scnMgr->getRootSceneNode()->createChildSceneNode(
        Ogre::Vector3(0, -10.0, 25.0));
    node->attachObject(ent);
    node->setScale(0.1, 0.1, 0.1);

    ent = scnMgr->createEntity("knot.mesh");
    node = scnMgr->getRootSceneNode()->createChildSceneNode(
        Ogre::Vector3(550.0, -10.0, 50.0));
    node->attachObject(ent);
    node->setScale(0.1, 0.1, 0.1);

    ent = scnMgr->createEntity("knot.mesh");
    node = scnMgr->getRootSceneNode()->createChildSceneNode(
        Ogre::Vector3(-100.0, -10.0, -200.0));
    node->attachObject(ent);
    node->setScale(0.1, 0.1, 0.1);


}

void Game::setupEnemy()
{

    float mass = 1.0f;
    SceneNode* sceneRoot = scnMgr->getRootSceneNode();
    Vector3 axis(0.0, 0.0, 0.0);
    Radian angle(Degree(25.0));

    

    Enemy = new enemy();
    Enemy->createMesh(scnMgr);
    Enemy->attachToNode(sceneRoot);
    Enemy->setRotation(axis, angle);
    Enemy->setPosition();
    Enemy->createRigidBody(mass);
    Enemy->setFishanimation();

    Enemy->addToCollisionShapes(collisionShapes);
    Enemy->addToDynamicsWorld(dynamicsWorld);

}

void Game::setupCamera()
{


    xc = 0, yc = 200, zc = 600;


    // creating the camera
    Camera* cam = scnMgr->createCamera("myCam");

    //Seting up the camera
    cam->setNearClipDistance(5);
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(xc, yc, zc);
    camNode->attachObject(cam);

    //setting up the camera for the camera
    Viewport* vp = getRenderWindow()->addViewport(cam);
    vp->setBackgroundColour(ColourValue(0, 0, 0));

    // link the camera and view port.
    cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    

}

bool Game::nextlocation() 
{
    //code used to check the nextlocations which we created for the enemy AI 
    if (mWalkList.empty())
        return false;

    mDestination = mWalkList.front();
    mWalkList.pop_front();
    mDirection = mDestination - aienemy->getSceneNode()->getPosition();
    mDistance = mDirection.normalise();

    return true;

}

void Game::bulletInit()
{
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    collisionConfiguration = new btDefaultCollisionConfiguration();

     ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
     dispatcher = new btCollisionDispatcher(collisionConfiguration);

     ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
     overlappingPairCache = new btDbvtBroadphase();

     ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
     solver = new btSequentialImpulseConstraintSolver;

     dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

     dynamicsWorld->setGravity(btVector3(0, -20, 0));
}


void Game::setupFloor()
{


    // Creating a plane object
    Plane plane(Vector3::UNIT_Y, 0);

    // giving defenitions to our plane such as width and length + how many squares goes into, in this case it a 5*5 
    MeshManager::getSingleton().createPlane(
            "ground", RGN_DEFAULT,
            plane,
            1500, 1500, 20, 20,
            true,
            1, 5, 5,
            Vector3::UNIT_Z);

    // the needed ground entity 
    Entity* groundEntity = scnMgr->createEntity("ground");

   
    //Turning off shadows + giving it a material from the examples notepad
    //Then creating the sceneNode
    groundEntity->setCastShadows(false);
    groundEntity->setMaterialName("Examples/Rockwall");
    
   
    SceneNode* thisSceneNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    thisSceneNode->attachObject(groundEntity);
    

   
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(groundShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
  

    Vector3 pos = thisSceneNode->_getDerivedPosition();

    
    groundTransform.setOrigin(btVector3(pos.x, pos.y-50.0, pos.z));

    Quaternion quat2 = thisSceneNode->_getDerivedOrientation();
    groundTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

    
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        groundShape->calculateLocalInertia(mass, localInertia);

    
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

   

    dynamicsWorld->addRigidBody(body);
}

void Game::setupFloo2()
{
   
    Plane plane(Vector3::UNIT_Y, 0);
    MeshManager::getSingleton().createPlane(
        "ground4", RGN_DEFAULT,
        plane,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);

    Entity* groundEntity4 = scnMgr->createEntity("ground4");

    groundEntity4->setCastShadows(false);

    
    groundEntity4->setMaterialName("Examples/Rockwall");

    
    SceneNode* thisSceneNode4 = scnMgr->getRootSceneNode()->createChildSceneNode();
    thisSceneNode4->attachObject(groundEntity4);
    thisSceneNode4->setPosition(0, 0, -1500);

   
    btCollisionShape* groundShape4 = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(groundShape4);

    btTransform groundTransform4;
    groundTransform4.setIdentity();
    

    Vector3 pos = thisSceneNode4->_getDerivedPosition();

   
    groundTransform4.setOrigin(btVector3(pos.x, pos.y - 50.0, pos.z));

    Quaternion quat2 = thisSceneNode4->_getDerivedOrientation();
    groundTransform4.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

    
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        groundShape4->calculateLocalInertia(mass, localInertia);

    
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform4);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape4, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

   

    
    dynamicsWorld->addRigidBody(body);
}

void Game::setupFloo3()
{
    
    Plane plane(Vector3::UNIT_Y, 0);

    
    MeshManager::getSingleton().createPlane(
        "ground2", RGN_DEFAULT,
        plane,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);

   
    Entity* groundEntity3 = scnMgr->createEntity("ground2");

    
    groundEntity3->setCastShadows(false);

    
    groundEntity3->setMaterialName("Examples/Rockwall");

    
    SceneNode* thisSceneNode3 = scnMgr->getRootSceneNode()->createChildSceneNode();
    thisSceneNode3->attachObject(groundEntity3);
    thisSceneNode3->setPosition(-1500, 0, -1500);

    
    btCollisionShape* groundShape3 = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(groundShape3);

    btTransform groundTransform3;
    groundTransform3.setIdentity();
   

    Vector3 pos = thisSceneNode3->_getDerivedPosition();

    
    groundTransform3.setOrigin(btVector3(pos.x, pos.y - 50.0, pos.z));

    Quaternion quat2 = thisSceneNode3->_getDerivedOrientation();
    groundTransform3.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

    
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        groundShape3->calculateLocalInertia(mass, localInertia);

   
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform3);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape3, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    

    
    dynamicsWorld->addRigidBody(body);
}

void Game::setupFloo4()
{
    
    Plane plane(Vector3::UNIT_Y, 0);

   
    MeshManager::getSingleton().createPlane(
        "ground1", RGN_DEFAULT,
        plane,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);

    
    Entity* groundEntity1 = scnMgr->createEntity("ground1");

   
    groundEntity1->setCastShadows(false);

    
    groundEntity1->setMaterialName("Examples/Rockwall");

    
    SceneNode* thisSceneNode1 = scnMgr->getRootSceneNode()->createChildSceneNode();
    thisSceneNode1->attachObject(groundEntity1);
    thisSceneNode1->setPosition(-1500, 0, 0);

    
    btCollisionShape* groundShape1 = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(groundShape1);

    btTransform groundTransform1;
    groundTransform1.setIdentity();
   

    Vector3 pos = thisSceneNode1->_getDerivedPosition();

    
    groundTransform1.setOrigin(btVector3(pos.x, pos.y - 50.0, pos.z));

    Quaternion quat2 = thisSceneNode1->_getDerivedOrientation();
    groundTransform1.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

    
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        groundShape1->calculateLocalInertia(mass, localInertia);

   
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform1);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape1, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    
    dynamicsWorld->addRigidBody(body);
}


void Game::setupWall()
{

    Quaternion q(Degree(90), Vector3::UNIT_Z);
    Quaternion r(Degree(90), Vector3::UNIT_X);
    q = q * r;

  
    Plane Wall(Vector3::UNIT_Y, 0);

  
    MeshManager::getSingleton().createPlane(
        "Wall", RGN_DEFAULT,
        Wall,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);

    
    Entity* WallEntity = scnMgr->createEntity("Wall");

    
    WallEntity->setCastShadows(false);

    
    WallEntity->setMaterialName("Examples/Rockwall");

    
    SceneNode* WallSceneNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    WallSceneNode->attachObject(WallEntity);
    WallSceneNode->setPosition(0, 300, -2000);
    WallSceneNode->rotate(q);
    WallSceneNode->yaw(Degree(90));
    
    btCollisionShape* WallShape = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(WallShape);

    btTransform WallTransform;
    WallTransform.setIdentity();
   

    Vector3 pos = WallSceneNode->_getDerivedPosition();

   
    WallTransform.setOrigin(btVector3(pos.x, pos.y - 50.0, pos.z));

    Quaternion quat2 = WallSceneNode->_getDerivedOrientation();
    WallTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

    
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        WallShape->calculateLocalInertia(mass, localInertia);

   
    btDefaultMotionState* myMotionState = new btDefaultMotionState(WallTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, WallShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    
    
    dynamicsWorld->addRigidBody(body);
}

void Game::setupwall2()
{
    Quaternion q(Degree(-90), Vector3::UNIT_Z);
    Quaternion r(Degree(90), Vector3::UNIT_X);
    

   
    Plane Wall5(Vector3::UNIT_Y, 0);

   
    MeshManager::getSingleton().createPlane(
        "Wall5", RGN_DEFAULT,
        Wall5,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);


    Entity* WallEntity5 = scnMgr->createEntity("Wall5");

    
    WallEntity5->setCastShadows(false);

   
    WallEntity5->setMaterialName("Examples/Rockwall");

   
    SceneNode* WallSceneNode5 = scnMgr->getRootSceneNode()->createChildSceneNode();
    WallSceneNode5->attachObject(WallEntity5);
    WallSceneNode5->setPosition(-2250, 300, -1500);
    WallSceneNode5->rotate(q);
    WallSceneNode5->yaw(Degree(90));
    
    btCollisionShape* WallShape5 = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(WallShape5);

    btTransform WallTransform5;
    WallTransform5.setIdentity();
    

    Vector3 pos = WallSceneNode5->_getDerivedPosition();

    
    WallTransform5.setOrigin(btVector3(pos.x, pos.y - 50.0, pos.z));

    Quaternion quat2 = WallSceneNode5->_getDerivedOrientation();
    WallTransform5.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

    
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        WallShape5->calculateLocalInertia(mass, localInertia);

    
    btDefaultMotionState* myMotionState = new btDefaultMotionState(WallTransform5);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, WallShape5, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

   
    dynamicsWorld->addRigidBody(body);


}

void Game::setupwall3()
{
    Quaternion q(Degree(90), Vector3::UNIT_Z);
    Quaternion r(Degree(90), Vector3::UNIT_X);
    q = q * r;

  
    Plane Wall1(Vector3::UNIT_Y, 0);

    
    MeshManager::getSingleton().createPlane(
        "Wall1", RGN_DEFAULT,
        Wall1,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);

    
    Entity* WallEntity1 = scnMgr->createEntity("Wall1");

    
    WallEntity1->setCastShadows(false);

    
    WallEntity1->setMaterialName("Examples/Rockwall");

    
    SceneNode* WallSceneNode1 = scnMgr->getRootSceneNode()->createChildSceneNode();
    WallSceneNode1->attachObject(WallEntity1);
    WallSceneNode1->setPosition(-1500, 300, -2000);
    WallSceneNode1->rotate(q);
    WallSceneNode1->yaw(Degree(90));
    
    btCollisionShape* WallShape1 = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(WallShape1);

    btTransform WallTransform1;
    WallTransform1.setIdentity();
   

    Vector3 pos = WallSceneNode1->_getDerivedPosition();

    
    WallTransform1.setOrigin(btVector3(pos.x, pos.y - 50.0, pos.z));

    Quaternion quat2 = WallSceneNode1->_getDerivedOrientation();
    WallTransform1.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

    
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        WallShape1->calculateLocalInertia(mass, localInertia);

   
    btDefaultMotionState* myMotionState = new btDefaultMotionState(WallTransform1);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, WallShape1, localInertia);
    btRigidBody* body5 = new btRigidBody(rbInfo);

   
    dynamicsWorld->addRigidBody(body5);




}

void Game::setupwall4()
{

    Quaternion q(Degree(90), Vector3::UNIT_Z);
    Quaternion r(Degree(90), Vector3::UNIT_X);
   

  
    Plane Wall4(Vector3::UNIT_Y, 0);

    
    MeshManager::getSingleton().createPlane(
        "Wall4", RGN_DEFAULT,
        Wall4,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);

   
    Entity* WallEntity4 = scnMgr->createEntity("Wall4");

    
    WallEntity4->setCastShadows(false);

   
    WallEntity4->setMaterialName("Examples/Rockwall");

    
    SceneNode* WallSceneNode4 = scnMgr->getRootSceneNode()->createChildSceneNode();
    WallSceneNode4->attachObject(WallEntity4);
    WallSceneNode4->setPosition(750, 300, 0);
    WallSceneNode4->rotate(q);
    WallSceneNode4->yaw(Degree(90));
    
    btCollisionShape* WallShape4 = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(WallShape4);

    btTransform WallTransform4;
    WallTransform4.setIdentity();
    

    Vector3 pos = WallSceneNode4->_getDerivedPosition();

   
    WallTransform4.setOrigin(btVector3(pos.x, pos.y - 50.0, pos.z));

    Quaternion quat2 = WallSceneNode4->_getDerivedOrientation();
    WallTransform4.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

  
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        WallShape4->calculateLocalInertia(mass, localInertia);

    
    btDefaultMotionState* myMotionState = new btDefaultMotionState(WallTransform4);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, WallShape4, localInertia);
    btRigidBody* body5 = new btRigidBody(rbInfo);

   
    dynamicsWorld->addRigidBody(body5);


}

void Game::setupwall5()
{
    Quaternion q(Degree(90), Vector3::UNIT_Z);
    Quaternion r(Degree(45), Vector3::UNIT_X);
    Quaternion d(Degree(10), Vector3::UNIT_Y);
    

    
    Plane Wall2(Vector3::UNIT_Y, 0);

    
    MeshManager::getSingleton().createPlane(
        "Wall2", RGN_DEFAULT,
        Wall2,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);

    
    Entity* WallEntity2 = scnMgr->createEntity("Wall2");

    
    WallEntity2->setCastShadows(false);

   
    WallEntity2->setMaterialName("Examples/Rockwall");

    
    SceneNode* WallSceneNode2 = scnMgr->getRootSceneNode()->createChildSceneNode();
    WallSceneNode2->attachObject(WallEntity2);
    WallSceneNode2->setPosition(750, 300, -1500);
    WallSceneNode2->rotate(q);
    WallSceneNode2->yaw(Degree(90));
    
    btCollisionShape* WallShape1 = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(WallShape1);

    btTransform WallTransform2;
    WallTransform2.setIdentity();
    

    Vector3 pos = WallSceneNode2->_getDerivedPosition();

   
    WallTransform2.setOrigin(btVector3(pos.x, pos.y - 50.0, pos.z));

    Quaternion quat2 = WallSceneNode2->_getDerivedOrientation();
    WallTransform2.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

   
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        WallShape1->calculateLocalInertia(mass, localInertia);

    
    btDefaultMotionState* myMotionState = new btDefaultMotionState(WallTransform2);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, WallShape1, localInertia);
    btRigidBody* body5 = new btRigidBody(rbInfo);

  
    dynamicsWorld->addRigidBody(body5);


}

void Game::setupwall6()
{

    Quaternion q(Degree(-90), Vector3::UNIT_Z);
    Quaternion r(Degree(90), Vector3::UNIT_X);


    
    Plane Wall6(Vector3::UNIT_Y, 0);

    
    MeshManager::getSingleton().createPlane(
        "Wall6", RGN_DEFAULT,
        Wall6,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);

    
    Entity* WallEntity6 = scnMgr->createEntity("Wall6");

    
    WallEntity6->setCastShadows(false);

    
    WallEntity6->setMaterialName("Examples/Rockwall");

    
    SceneNode* WallSceneNode6 = scnMgr->getRootSceneNode()->createChildSceneNode();
    WallSceneNode6->attachObject(WallEntity6);
    WallSceneNode6->setPosition(-2250, 300, 0);
    WallSceneNode6->rotate(q);
    WallSceneNode6->yaw(Degree(90));
   
    btCollisionShape* WallShape6 = new btBoxShape(btVector3(btScalar(750.), btScalar(50.), btScalar(750.)));

    collisionShapes.push_back(WallShape6);

    btTransform WallTransform6;
    WallTransform6.setIdentity();
   

    Vector3 pos = WallSceneNode6->_getDerivedPosition();

    
    WallTransform6.setOrigin(btVector3(pos.x, pos.y - 50.0, pos.z));

    Quaternion quat2 = WallSceneNode6->_getDerivedOrientation();
    WallTransform6.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));


    btScalar mass(0.);

    
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        WallShape6->calculateLocalInertia(mass, localInertia);

    
    btDefaultMotionState* myMotionState = new btDefaultMotionState(WallTransform6);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, WallShape6, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    
    dynamicsWorld->addRigidBody(body);

}



bool Game::frameStarted(const Ogre::FrameEvent &evt)
{
    

    

    
    ApplicationContext::frameStarted(evt);

  if (this->dynamicsWorld != NULL)
  {

      if (upDown) 
      {
          player->forward();
      }
      if (DownDown)
      {
          player->back();
      }
      if (leftDown)
      {
          player->left();
      }
      if (rightDown)
      {
          player->right();
      }if (shiftDown)
      {
          player->jump();
      }

      // Bullet can work with a fixed timestep

      // Or a variable one, however, under the hood it uses a fixed timestep
      // then interpolates between them.
          

     dynamicsWorld->stepSimulation((float)evt.timeSinceLastFrame, 10);

     int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

     // update positions of all objects
     for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
     {
         btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
         btRigidBody* body = btRigidBody::upcast(obj);
         btTransform trans;
        
         if (body && body->getMotionState())
         {
            body->getMotionState()->getWorldTransform(trans);

            /* https://oramind.com/ogre-bullet-a-beginners-basic-guide/ */
            void *userPointer = body->getUserPointer();
            if (userPointer && !player)
            {
              btQuaternion orientation = trans.getRotation();
              Ogre::SceneNode* sceneNode = static_cast<Ogre::SceneNode*>(userPointer);
              
              sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
              sceneNode->setOrientation(Ogre::Quaternion(orientation.getX(), orientation.getY(), orientation.getZ(), orientation.getW()));
            }

          }
          else
          {
            trans = obj->getWorldTransform();
          }

     }
     if (numManifolds > 0)
     {
         
         for (int i = 0; i < numManifolds; i++)
         {
             btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
             const btCollisionObject* obA = contactManifold->getBody0();
             const btCollisionObject* obB = contactManifold->getBody1();

             int numContacts = contactManifold->getNumContacts();

             

             if (numContacts > 0)
             {
                 for (int k = 0; k < numContacts; k++)
                 {
                     btManifoldPoint& pt = contactManifold->getContactPoint(k);
                     if (pt.getDistance() < 0.f)
                     {
                         // point of contact between two objects this part of the code isnt working as it lacks varibable and defenitions
                         const btVector3& ptA = pt.getPositionWorldOnA();
                         const btVector3& ptB = pt.getPositionWorldOnB();
                         const btVector3& normalOnB = pt.m_normalWorldOnB;

                       
                         
                         
                     }
                 }
             }
         }
     }

     
     

     player->update();
    
     Enemy->update();
     penguin->update();
     collum->update();
     //Enemy->AImovement(); Ai testing 
   }
  return true;
}


bool Game::frameEnded (const Ogre::FrameEvent &evt)
{
  if (this->dynamicsWorld != NULL)
  {

 

     

       dynamicsWorld->stepSimulation((float)evt.timeSinceLastFrame, 10);
       
  }
  return true;
}

void Game::setupLights()
{
    // Setup the light for the game
    scnMgr->setAmbientLight(ColourValue(0, 0, 0));
    scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

    // Adding a light which is focused in one area
   Light* spotLight = scnMgr->createLight("SpotLight");

    // Configure
    spotLight->setDiffuseColour(1, 1, 1.0);
    spotLight->setSpecularColour(0.5, 0.5, 0.5);
    spotLight->setType(Light::LT_SPOTLIGHT);
    spotLight->setSpotlightRange(Degree(35), Degree(50));


    // givinng the Spotlight its one node
    SceneNode* spotLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    spotLightNode->setDirection(-1, -1, 0);
    spotLightNode->setPosition(Vector3(200, 200, 0));

    // Attaching the light to the node
    spotLightNode->attachObject(spotLight);
    spotLight->setCastShadows(false);

    // creating light which illuminates in a specific direction
    Light* directionalLight = scnMgr->createLight("DirectionalLight");
    
    // Configuring the colour and type
    directionalLight->setType(Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(ColourValue(1, 1, 1));
    directionalLight->setSpecularColour(ColourValue(0.5, 0.5, 0.5));

    // Setup a scene node for the directional lightnode.
    SceneNode* directionalLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    directionalLightNode->attachObject(directionalLight);
    directionalLightNode->setDirection(Vector3(0, -1, 1));
    directionalLight->setCastShadows(false);

    // Create another type of lighting in our scene 
    Light* pointLight = scnMgr->createLight("PointLight");

    // Configuring the colour of that light 
    pointLight->setType(Light::LT_POINT);
    pointLight->setDiffuseColour(1, 1, 1);
    pointLight->setSpecularColour(0.5, 0.5, 0.5);

    // Node object 
    SceneNode* pointLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    // Putting it at a specific position
    pointLightNode->setPosition(Vector3(0, 150, 250));

    // Adding it to our nodes.
    pointLightNode->attachObject(pointLight);
    pointLight->setCastShadows(false);
}

bool Game::keyPressed(const KeyboardEvent& evt)
{
   
    if (evt.keysym.sym == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }

    //SDL KEYS were used in where for testing purposes.

    switch (evt.keysym.sym)
    {
    case SDLK_UP:
        upDown = true;
        player->WalkForward();
        cout << z << endl;
        camNode->setPosition(xc, yc, zc = zc - 5);
        break;
    case SDLK_DOWN:

        DownDown = true;
        player->WalkForward();
        camNode->setPosition(xc, yc, zc = zc + 5);
        break;

    case SDLK_LEFT:
        leftDown = true;
        player->WalkForward();
        camNode->setPosition(xc = xc - 5, yc, zc );
        break;

    case SDLK_RIGHT:
        rightDown = true;
        player->WalkForward();
        camNode->setPosition(xc = xc + 5 , yc, zc);
        break;
    case SDLK_SPACE:
        player->attack();
        
        break;
    case SDLK_LSHIFT:
        shiftDown = true;
        player->jumpAnimation();
        break;
    default:
        break;

    }

    return true;
}

bool Game::keyReleased(const KeyboardEvent& evt) 
{



    switch (evt.keysym.sym)
    {
    case SDLK_UP:
        upDown = false;
        player->setLinearVelocity(btVector3(0, 0, 0));
        player->NotMoving();

        break;
    case SDLK_DOWN:

        DownDown = false;
        player->NotMoving();
        player->setLinearVelocity(btVector3(0,0,0));
        break;

    case SDLK_LEFT:
        leftDown = false;
        player->NotMoving();
        player->setLinearVelocity(btVector3(0, 0, 0));
        break;

    case SDLK_RIGHT:
        rightDown = false;
        player->NotMoving();
        player->setLinearVelocity(btVector3(0, 0, 0));
        break;
    case SDLK_SPACE:
        player->NotMoving();
        break;
    case SDLK_LSHIFT:
        shiftDown = false;
        
        break;


    default:
        break;
    }


    return true;
}

bool Game::mouseMoved(const MouseMotionEvent& evt)
{
	return true;
}


void Game::setupPlayer() 
{

    //creating all the variables needed for the player and playing each one of the functions so that our player will be shown in the redenring window.
    float mass = 1.0f;

    Real* time;
    
    x = 0, y = 10, z = 200; 
    SceneNode* sceneRoot = scnMgr->getRootSceneNode();
    Vector3 axis(0.0, 0.0, 0.0);
    Radian angle(Degree(25.0));
    

    //functions given to the player related to graphics
    player = new Player();
    player->createMesh(scnMgr);
    player->attachToNode(sceneRoot);
    player->setRotation(axis, angle);
    player->setPosition(x, y, z);
    player->setupNinjaAnimation();
   //functions given to the player related to physics
    player->createRigidBody(mass);
    player->addToCollisionShapes(collisionShapes);
    player->addToDynamicsWorld(dynamicsWorld);
    
}

void Game::setupHouse()
{

   

    float massH = 0.0f;
    SceneNode* sceneRootH = scnMgr->getRootSceneNode();
    Vector3 axisH(0.0, 0.0, 0.0);
    Radian angleH(Degree(25.0));

    house = new House;
    house->createMesh(scnMgr);
    house->attachToNode(sceneRootH);
    house->setRotation(axisH, angleH);
    house->setPosition();
    house->setScale();
    house->createRigidBody(massH);
    house->addToCollisionShapes(collisionShapes);
    house->addToDynamicsWorld(dynamicsWorld);



}

void Game::setupMovingEnemy()
{

    float massA = 0.0f;
    SceneNode* sceneRootA = scnMgr->getRootSceneNode();
    Vector3 axisA(0.0, 0.0, 0.0);
    Radian angleA(Degree(25.0));

    aienemy = new AIEnemy;
    aienemy->createMesh(scnMgr);
    aienemy->attachToNode(sceneRootA);
    aienemy->setRotation(axisA, angleA);
    aienemy->setPosition();
    aienemy->setScale();
   

}

void Game::setupPenguin()
{
    float massp = 1.0f;
    SceneNode* sceneRootp = scnMgr->getRootSceneNode();
    Vector3 axisp(0.0, 0.0, 0.0);
    Radian anglep(Degree(25.0));

    penguin = new Penguin;
    penguin->createMesh(scnMgr);
    penguin->attachToNode(sceneRootp);
    penguin->setRotation(axisp, anglep);
    penguin->setPosition();
    penguin->createRigidBody(massp);

    penguin->addToCollisionShapes(collisionShapes);
    penguin->addToDynamicsWorld(dynamicsWorld);
}

void Game::setupJaiqua()
{

    float massj = 1.0f;
    SceneNode* sceneRootj = scnMgr->getRootSceneNode();
    Vector3 axisj(0.0, 0.0, 0.0);
    Radian anglej(Degree(25.0));

    
   
    jaiqua = new Jaiqua;
    jaiqua->createMesh(scnMgr);
    jaiqua->attachToNode(sceneRootj);
    jaiqua->setRotation(axisj, anglej);
    jaiqua->setPosition();
    jaiqua->createRigidBody(massj);

    jaiqua->addToCollisionShapes(collisionShapes);
    jaiqua->addToDynamicsWorld(dynamicsWorld);

}

void Game::setupCollum()
{
    int x, y, z;
    x = -200, y = 100, z = -200;

    float massC = 0.0f;
    SceneNode* sceneRootC = scnMgr->getRootSceneNode();
    Vector3 axisC(0.0, 0.0, 0.0);
    Radian angleC(Degree(25.0));

    collum = new Collum;
    collum->createMesh(scnMgr);
    collum->attachToNode(sceneRootC);
    collum->setRotation(axisC, angleC);
    collum->setPosition(x ,y ,z);
    collum->setScale(1.0f, 1.0f, 1.0f);
    collum->createRigidBody(massC);
    collum->addToCollisionShapes(collisionShapes);
    collum->addToDynamicsWorld(dynamicsWorld);

    

}

void Game::setupCollum2()
{
    int x, y, z;
    x = 200, y = 100, z = -200;

    float massC2 = 0.0f;
    SceneNode* sceneRootC2 = scnMgr->getRootSceneNode();
    Vector3 axisC2(0.0, 0.0, 0.0);
    Radian angleC2(Degree(25.0));

    collum2 = new Collum;
    collum2->createMesh(scnMgr);
    collum2->attachToNode(sceneRootC2);
    collum2->setRotation(axisC2, angleC2);
    collum2->setPosition(x, y, z);
    collum2->setScale(1.0f, 1.0f, 1.0f);

    collum2->createRigidBody(massC2);
    collum2->addToCollisionShapes(collisionShapes);
    collum2->addToDynamicsWorld(dynamicsWorld);




}

bool Game::frameRenderingQueued(const FrameEvent& evt)
{
    //adding time to our different animations used
    player->getAnimationState()->addTime(evt.timeSinceLastFrame);
    Enemy->getanimation()->addTime(evt.timeSinceLastFrame);
    aienemy->getanimationState()->addTime(evt.timeSinceLastFrame);

    //giving our Ai enemy the logics that needs to be called each time.
    if (mDirection == Ogre::Vector3::ZERO) 
    {
        if (nextlocation())
        {
            aienemy->setAIEnemyanimation();

        }
    }
    else
    {
        Ogre::Real move = mWalkSpd * evt.timeSinceLastFrame;
        mDistance -= move;

        if (mDistance <= 0.0)
        {
            aienemy->getSceneNode()->setPosition(mDestination);
            mDirection = Ogre::Vector3::ZERO;

            if (nextlocation())
            {
                Ogre::Vector3 src = aienemy->getSceneNode()->getOrientation() * Ogre::Vector3::UNIT_X;

                if ((1.0 + src.dotProduct(mDirection)) < 0.0001)
                {
                    aienemy->getSceneNode()->yaw(Ogre::Degree(180));
                }
                else
                {
                    Ogre::Quaternion quat = src.getRotationTo(mDirection);
                    aienemy->getSceneNode()->rotate(quat);
                }

            }
            else
            {
                aienemy->SetAIIdleAniamtion();

            }
        }
        else
        {
            aienemy->getSceneNode()->translate(move * mDirection);
        }
    }


    

    return true;
}







