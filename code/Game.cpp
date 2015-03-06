#include "Game.h"

bool HandleContacts(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1) {
   GameObject *o0 = (GameObject *) body0->getUserPointer();
   GameObject *o1 = (GameObject *) body1->getUserPointer();
   o0->getGame()->collission(o0, o1);
   return true;
}

Game::Game() : BaseApplication() {}

Game::~Game()  {}

void Game::reset(){
	score = 0; lastHit = 0;
	gameStart = clock();
	for(GameObject *obj : entities) {
		Coin *coin = dynamic_cast<Coin *>(obj);
		if (coin) { coin->taken = false; coin->rootNode->setVisible(true); }
	}
	oBall->setPosition(Ogre::Vector3(0,0,0));
	oBall->rigidBody->setLinearVelocity(oBall->bDirection * oBall->bSpeed);
}

void Game::createFrameListener() {
	BaseApplication::createFrameListener();
	mTrayMgr->toggleAdvancedFrameStats();

    mScorePanel = mTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "ScorePanel", 200,
    	Ogre::StringVector {"Score", "Time"});
    mScorePanel->show();
}

void Game::collission(GameObject *o0, GameObject *o1) {
	if (o0->kind > o1->kind) {GameObject *tmp = o0; o0 = o1; o1 = tmp;}
	if (o0->kind != K::BALL) return;

	switch(o1->kind){
	case K::WALL:
	case K::OBSTACLE:
		if((clock()-lastHit) > (CLOCKS_PER_SEC*0.15))
			mSndMgr->getSound("sndHit")->play();
		lastHit = clock();
		break;
	case K::PADDLE:
		mSndMgr->getSound("sndPaddle")->play();
		oBall->rigidBody->setLinearVelocity(
				oBall->rigidBody->getLinearVelocity().normalisedCopy() * 250.0);
		break;
	case K::PIT:
		reset();
		break;
	case K::COIN:
		Coin *coin = dynamic_cast<Coin *>(o1);
		if (coin->taken) break;
		mSndMgr->getSound("sndScore")->play();
		coin->taken = true;
		coin->rootNode->setVisible(false);
		score++;
		break;
	}
	std::cout << "o0=" << o0->name << ", o1=" << o1->name << "\n";
}

void Game::createScene(void){
	// Init Bullet
	Ogre::Vector3 gravityVector(0,-9.81,0);
	Ogre::AxisAlignedBox bounds (Ogre::Vector3 (-10000, -10000, -10000), Ogre::Vector3 (10000,  10000,  10000));
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector);
	gContactProcessedCallback = (ContactProcessedCallback) HandleContacts;

	// Init OgreOggSound
	mOggSoundPlugin = new OgreOggSound::OgreOggSoundPlugin();
	Ogre::Root::getSingleton().installPlugin(mOggSoundPlugin);
	mSndMgr = OgreOggSound::OgreOggSoundManager::getSingletonPtr();
	mSndMgr->setSceneManager(mSceneMgr);
	mSndMgr->init();

	mSndMgr->createSound("sndPaddle", "smash.wav", false, false, true);
	mSndMgr->createSound("sndHit", "hit.wav", false, false, true);
	mSndMgr->createSound("sndScore", "bell.wav", false, false, true);

	// Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    mPaddle = new Paddle(this);
    entities.insert(mPaddle);

    oBall = new Ball(this);
    entities.insert(oBall);

    //Create 6 walls
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1000, 1000, 20, 20, true, 1, 5.0, 5.0, Ogre::Vector3::UNIT_X);

    PitPlane* pit = new PitPlane(this, Ogre::Vector3::UNIT_Y, Ogre::Real(-510));
    entities.insert(pit); // Bottom

    Wall *p = new Wall(this, Ogre::Vector3(0.75f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Z), Ogre::Vector3(0, 500, 0));
    entities.insert(p); // Top

    p = new Wall(this, Ogre::Vector3(0.75f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, -100));
    entities.insert(p); // Back

    p = new Wall(this, Ogre::Vector3(0.75f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, 100));
    entities.insert(p); // Front
    
    p = new Wall(this, Ogre::Vector3(1.0f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_Z),  Ogre::Vector3(-375, 0, 0));
    entities.insert(p); // Left

    p = new Wall(this, Ogre::Vector3(1.0f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z), Ogre::Vector3(375, 0, 0));
    entities.insert(p); // Right


    std::vector<Ogre::Vector3> coinPos { Ogre::Vector3(100,100,0), Ogre::Vector3(-100,-100,0) };
    for (Ogre::Vector3 p : coinPos)
    	entities.insert(new Coin(this, p));

    std::vector<Ogre::Vector3> obstaclePos { Ogre::Vector3(-375,-100,0), Ogre::Vector3(355,480,0),
    	Ogre::Vector3(-100,100,0), Ogre::Vector3(100,-100,0) };
    for (Ogre::Vector3 p : obstaclePos)
    	entities.insert(new Obstacle(this, p));

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("OutsideLight");
    light->setPosition(90.0f, 90.0f, 800.0f);

    reset();
}

bool Game::frameStarted(const Ogre::FrameEvent& evt) {
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameEnded(const Ogre::FrameEvent& evt) {
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt){
	if(!BaseApplication::frameRenderingQueued(evt)) return false;
	if (mTrayMgr->isDialogVisible()) return true;

	int elapsedSec = (clock()-gameStart)/CLOCKS_PER_SEC;
	mScorePanel->setParamValue(0, Ogre::StringConverter::toString(score));
	mScorePanel->setParamValue(1, Ogre::StringConverter::toString(elapsedSec));
	for(GameObject *obj : entities) {
		obj->update(evt);
	}
    return true;
}


bool Game::keyPressed( const OIS::KeyEvent& evt ){
	if (evt.key == OIS::KC_LEFT) mPaddle->motion |= 1;
	else if (evt.key == OIS::KC_RIGHT) mPaddle->motion |= 2;
	else BaseApplication::keyPressed(evt);
    return true;
}

bool Game::keyReleased( const OIS::KeyEvent& evt ){
	if (evt.key == OIS::KC_LEFT) mPaddle->motion &= ~1;
	else if (evt.key == OIS::KC_RIGHT) mPaddle->motion &= ~2;
	else BaseApplication::keyReleased(evt);
	return true;
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Game app;
 
        try 
        {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR| MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif
