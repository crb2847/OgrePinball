#include "Game.h"

bool HandleContacts(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1) {
   GameObject *o0 = (GameObject *) body0->getUserPointer();
   GameObject *o1 = (GameObject *) body1->getUserPointer();
   o0->getGame()->collission(o0, o1);
   return true;
}

Game::Game(void) : BaseApplication(), net() {
	remPaddlePos = 0;
	state = GAMEST_WAIT;
}

Game::~Game() {}

void Game::reset(){
	elapsedSec = 0;
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
    	Ogre::StringVector {"Score", "Time", "State"});
    mScorePanel->show();
}

void Game::collission(GameObject *o0, GameObject *o1) {
	if (o0->kind > o1->kind) {GameObject *tmp = o0; o0 = o1; o1 = tmp;}
	if (o0->kind != K::BALL) return;

	switch(o1->kind){
	case K::WALL:
	case K::OBSTACLE:
		if((clock()-lastHit) > (CLOCKS_PER_SEC*0.15))
			if (soundOn) mSndMgr->getSound("sndHit")->play();
		lastHit = clock();
		break;
	case K::PADDLE:
		if (soundOn) mSndMgr->getSound("sndPaddle")->play();
		oBall->rigidBody->setLinearVelocity(
				oBall->rigidBody->getLinearVelocity().normalisedCopy() * 250.0);
		break;
	case K::PIT:
		reset();
		break;
	case K::COIN:
		Coin *coin = dynamic_cast<Coin *>(o1);
		if (coin->taken) break;
		if (soundOn) mSndMgr->getSound("sndScore")->play();
		coin->taken = true;
		coin->rootNode->setVisible(false);
		score++;
		break;
	}
	//std::cout << "o0=" << o0->name << ", o1=" << o1->name << "\n";
}

void Game::createScene(void){
	// Init Bullet
	Ogre::Vector3 gravityVector(0,-30.81,0);
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

    mPaddle1 = new Paddle(this,1);
    entities.insert(mPaddle1);

    mPaddle2 = new Paddle(this,2);
    entities.insert(mPaddle2);

    //mMyPaddle = server ? mPaddle1 : mPaddle2;

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


    std::vector<Ogre::Vector3> coinPos { Ogre::Vector3(100,100,0), Ogre::Vector3(-100,400,0), Ogre::Vector3(-100,150,0), Ogre::Vector3(-50,200,0), Ogre::Vector3(-250,300,0), Ogre::Vector3(280,400,0), Ogre::Vector3(-250,0,0), Ogre::Vector3(-300,100,0), Ogre::Vector3(-300,-400,0), Ogre::Vector3(-100,-300,0), Ogre::Vector3(0,-350,0), Ogre::Vector3(100,-270,0), Ogre::Vector3(200,-340,0)};
    for (Ogre::Vector3 p : coinPos)
    	entities.insert(new Coin(this, p));

    std::vector<Ogre::Vector3> obstaclePos { Ogre::Vector3(-350,-250,0), Ogre::Vector3(355,480,0),
    	Ogre::Vector3(-100,50,0), Ogre::Vector3(100,-150,0) };
    for (Ogre::Vector3 p : obstaclePos)
    	entities.insert(new Obstacle(this, p));

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("OutsideLight");
    light->setPosition(90.0f, 90.0f, 800.0f);

    reset();
}

bool Game::frameStarted(const Ogre::FrameEvent& evt) {
	if (state != GAMEST_SERVER) return true;
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameEnded(const Ogre::FrameEvent& evt) {
	if (state != GAMEST_SERVER) return true;
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt){
	if(!BaseApplication::frameRenderingQueued(evt)) return false;
	if (mTrayMgr->isDialogVisible()) return true;

	if (state == GAMEST_SERVER) elapsedSec = (clock()-gameStart)/CLOCKS_PER_SEC;
	mScorePanel->setParamValue(0, Ogre::StringConverter::toString(score));
	mScorePanel->setParamValue(1, Ogre::StringConverter::toString(elapsedSec));
	mScorePanel->setParamValue(2, Ogre::StringConverter::toString(state));

	if (state == GAMEST_WAIT) {
		bool r_server;
		if (net.connect(&r_server)) {
			state = r_server ? GAMEST_SERVER : GAMEST_CLIENT;
			reset();
		} else {
			return true;
		}
	}

	NetworkData_t netout, netin;
	int ci = 0;

	for(GameObject *obj : entities) {
		obj->update(evt);
		Coin *coin = dynamic_cast<Coin *>(obj);
		if (coin) netout.coins[ci++] = coin->taken;
	}

	netout.paddle1Pos = mPaddle1->rootNode->getPosition().x;
	netout.paddle2Pos = mPaddle2->rootNode->getPosition().x;
	netout.score = score;
	//netout.score2 = score2;
	netout.ballX = oBall->rootNode->getPosition().x;
	netout.ballY = oBall->rootNode->getPosition().y;
	netout.ballRotation = oBall->rootNode->getOrientation();
	netout.type = NET_UPDATE;
	netout.time = elapsedSec;
	net.write(&netout);

	while (net.read(&netin)) {
		if (netin.type != NET_UPDATE) continue;
		if (state == GAMEST_SERVER) {
			mPaddle2->setPosition(Ogre::Vector3(netin.paddle2Pos, -490, 0.0));
		} else {
			mPaddle1->setPosition(Ogre::Vector3(netin.paddle1Pos, -490, 0.0));
			oBall->setPosition(Ogre::Vector3(netin.ballX, netin.ballY,0));
			oBall->setRotation(netin.ballRotation);
			score = netin.score;
			elapsedSec = netin.time;
			ci = 0;
			for(GameObject *obj : entities) {
				Coin *coin = dynamic_cast<Coin *>(obj);
				if (!coin) continue;
				coin->taken = netin.coins[ci];
				coin->rootNode->setVisible(!netin.coins[ci]);
				ci++;
			}
		}
	}

    return true;
}

bool Game::keyPressed( const OIS::KeyEvent& evt ){
	if (state == GAMEST_SERVER && evt.key == OIS::KC_LEFT) mPaddle1->motion |= 1;
	else if (state == GAMEST_CLIENT && evt.key == OIS::KC_LEFT) mPaddle2->motion |= 1;
	else if (state == GAMEST_SERVER && evt.key == OIS::KC_RIGHT) mPaddle1->motion |= 2;
	else if (state == GAMEST_CLIENT && evt.key == OIS::KC_RIGHT) mPaddle2->motion |= 2;
	else if (evt.key == OIS::KC_M){
		if (!soundOn) soundOn = true;
		else if (soundOn) soundOn = false;
	}
	else BaseApplication::keyPressed(evt);
    return true;
}

bool Game::keyReleased( const OIS::KeyEvent& evt ){
	if (state == GAMEST_SERVER && evt.key == OIS::KC_LEFT) mPaddle1->motion &= ~1;
	else if (state == GAMEST_CLIENT && evt.key == OIS::KC_LEFT) mPaddle2->motion &= ~1;
	else if (state == GAMEST_SERVER && evt.key == OIS::KC_RIGHT) mPaddle1->motion &= ~2;
	else if (state == GAMEST_CLIENT && evt.key == OIS::KC_RIGHT) mPaddle2->motion &= ~2;
	else BaseApplication::keyReleased(evt);
	return true;
}

 
#ifdef __cplusplus
extern "C" {
#endif
 
int main(int argc, char *argv[]) {
	bool server = (argc > 1) ? atoi(argv[1]) : false;
	Game app;
	try  {
		app.go();
	} catch( Ogre::Exception& e ) {
		std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
	}
	return 0;
}
 
#ifdef __cplusplus
}
#endif
