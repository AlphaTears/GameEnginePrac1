#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"

/* Bullet3 Physics */
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

    //// collision configuration.
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    ///keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
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

	void setupLights();

	bool keyPressed(const KeyboardEvent& evt);
    bool keyReleased(const KeyboardEvent& evt);
    bool mouseMoved(const MouseMotionEvent& evt);


	bool frameStarted (const FrameEvent &evt);
	bool frameEnded(const FrameEvent &evt);
    bool frameRenderingQueued(const FrameEvent& evt);

    //void CamUpdate();
    //void CamForward();
    bool nextlocation();

  void bulletInit();
};
