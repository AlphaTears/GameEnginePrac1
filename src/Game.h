#pragma once
//ogre libraries
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"

//bullet physics libraries implementation
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

#include "Player.h"
#include  "enemy.h"
#include "Collum.h"
#include "House.h"
#include "AIEnemy.h"
#include "Penguin.h"
#include "Jaiqua.h"

using namespace Ogre;
using namespace OgreBites;

class Game : public ApplicationContext, public InputListener
{
private:

    Player* player;
    enemy* Enemy;
    Collum* collum;
    Collum* collum2;
    House* house;
    AIEnemy* aienemy;
    Penguin* penguin;
    Jaiqua* jaiqua;


    SceneManager* scnMgr;

    SceneNode* camNode;
    btRigidBody* bodyC;
    btDefaultMotionState* CMotionState;

    // configuration for the collision
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    //keeps track of the collision shapes make sure to use the collision shapes.
    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    float x, y, z; 
    float xc, yc, zc;
    float playerMov = 10;
    AnimationState* animationacces;
    bool upDown, DownDown, leftDown, rightDown,shiftDown;

    Vector3 mDirection;
    Vector3 mDestination;
    Real mDistance;
    Real mWalkSpd;
    std::deque<Ogre::Vector3> mWalkList;

    
public:
	Game();
	virtual ~Game();

    

	void setup();
    void setupEnemy();
    void setupPlayer();
    void setupHouse();
    void setupMovingEnemy();
    void setupPenguin();
    void setupJaiqua();

	void setupCamera();

	//void setupBoxMesh();
    void setupFloor();
    void setupFloo2();
    void setupFloo3();
    void setupFloo4();

    void setupWall();
    void setupwall2();
    void setupwall3();
    void setupwall4();
    void setupwall5();
    void setupwall6();
    void setupCollum();
    void setupCollum2();
    bool nextlocation();
	void setupLights();

	bool keyPressed(const KeyboardEvent& evt);
    bool keyReleased(const KeyboardEvent& evt);
    bool mouseMoved(const MouseMotionEvent& evt);


	bool frameStarted (const FrameEvent &evt);
	bool frameEnded(const FrameEvent &evt);
    bool frameRenderingQueued(const FrameEvent& evt);

    //tried making the camera move using physics 
    //void CamUpdate();
    //void CamForward();

   

  void bulletInit();
};
