#include "Game.h"

bool HandleContacts(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1) {
   GameObject *o0 = (GameObject *) body0->getUserPointer();
   GameObject *o1 = (GameObject *) body1->getUserPointer();
   o0->getGame()->collission(o0, o1);
   return true;
}

Game::Game(void) : BaseApplication(), net() {
	remPaddlePos = 0;
	soundOn = true;
	state = GAMEST_MENU;
	mGyroInput = nullptr;
}

Game::~Game() {
	delete mGyroInput;
}

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
	sounds[0] = sounds[1] = sounds[2] = sounds[3] = 0;
	if (soundOn) mSndMgr->getSound("sndBg")->play();
}

void Game::createFrameListener() {
	BaseApplication::createFrameListener();
	mTrayMgr->toggleAdvancedFrameStats();

    mScorePanel = mTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "ScorePanel", 200,
    	Ogre::StringVector {"Score", "Time", "State"});

    mControlPanel = mTrayMgr->createTextBox(OgreBites::TL_CENTER, "ControlPanel", "Mode Controls",  200, 110);

    mScorePanel->show();
}

void Game::collission(GameObject *o0, GameObject *o1) {
	if (o0->kind > o1->kind) {GameObject *tmp = o0; o0 = o1; o1 = tmp;}
	if (o0->kind != K::BALL) return;

	switch(o1->kind){
	case K::WALL:
	case K::OBSTACLE:
		if((clock()-lastHit) > (CLOCKS_PER_SEC*0.15))
			{sounds[0]++; if (soundOn) mSndMgr->getSound("sndHit")->play();}
		lastHit = clock();
		if (oBall->rigidBody->getLinearVelocity().length() > 500.0){
			oBall->rigidBody->setLinearVelocity(oBall->rigidBody->getLinearVelocity());
		}
		else oBall->rigidBody->setLinearVelocity(oBall->rigidBody->getLinearVelocity() * 1.1);
		oBall->rigidBody->setLinearVelocity(oBall->rigidBody->getLinearVelocity()); // just do this if you want it easy
		break;
	case K::PADDLE:
		if (soundOn) mSndMgr->getSound("sndPaddle")->play();
		sounds[1]++;
		oBall->rigidBody->setLinearVelocity(
				oBall->rigidBody->getLinearVelocity().normalisedCopy() * 450.0); 
		// should check here which direction the paddle is moving then move the ball accordingly
		break;
	case K::PIT:
		reset();
		break;
	case K::COIN:
		Coin *coin = dynamic_cast<Coin *>(o1);
		if (coin->taken) break;
		score++;
		if (score == maxScore){
			sounds[3]++;
			// if (soundOn) mSndMgr->getSound("sndScore")->play();
		}
		else {
			sounds[2]++;
			if (soundOn) mSndMgr->getSound("sndScore")->play();
		}
		coin->taken = true;
		coin->rootNode->setVisible(false);
		break;
	}
	//std::cout << "o0=" << o0->name << ", o1=" << o1->name << "\n";
}

void Game::createScene(void){
	mGyroInput = new GyroInput();
	mGyroInput->setEventCallback(this);
	mGyroInput->connect();
	// Init Bullet
	Ogre::Vector3 gravityVector(0,-70,0);
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
	// https://www.freesound.org/people/sirplus/sounds/104831/ (CC license)
	mSndMgr->createSound("sndBg", "slowspyk.ogg", false, true, true);
	mSndMgr->getSound("sndBg")->setVolume(0.2);

	// Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    SinglePlayerState* sp = new SinglePlayerState(this);
    gamestates.push_back(sp);
    sp->createState();

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("OutsideLight");
    light->setPosition(90.0f, 90.0f, 800.0f);

    elapsedSec = 0; score = 0;
}

bool Game::frameStarted(const Ogre::FrameEvent& evt) {
	gamestates[0]->frameStarted(evt);
	return true;
}

bool Game::frameEnded(const Ogre::FrameEvent& evt) {
	gamestates[0]->frameEnded(evt);
	return true;
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt){
	//gamestates[state]->frameRender();
	if(!BaseApplication::frameRenderingQueued(evt)) return false;
	mGyroInput->capture();

	if (mTrayMgr->isDialogVisible()) return true;

	mScorePanel->setParamValue(0, Ogre::StringConverter::toString(score));
	mScorePanel->setParamValue(1, Ogre::StringConverter::toString(elapsedSec));
	mScorePanel->setParamValue(2, Ogre::StringConverter::toString(state));

	if (state == GAMEST_MENU) {
		mControlPanel->setText("1:Single2:Multi");
		mControlPanel->show();
		return true;
	} else {
		if(mTrayMgr->getWidget("ControlPanel"))
			mTrayMgr->destroyWidget("ControlPanel");
	}
    return true;
}

bool Game::keyPressed( const OIS::KeyEvent& evt ){
	gamestates[0]->keyPressed(evt);

	if (state == GAMEST_MENU && (evt.key == OIS::KC_1 || evt.key == OIS::KC_NUMPAD1)) { state = GAMEST_SINGLE; reset(); }
	else if (state == GAMEST_MENU && (evt.key == OIS::KC_2 || evt.key == OIS::KC_NUMPAD2)) state = GAMEST_CONNECT;
	// else if (state == GAMEST_SINGLE && evt.key == OIS::KC_LEFT) { mPaddle1->motion |= 1; mPaddle2->motion |= 1; }
	// else if (state == GAMEST_SINGLE && evt.key == OIS::KC_RIGHT) { mPaddle1->motion |= 2; mPaddle2->motion |= 2; }
	else if (evt.key == OIS::KC_M){
		if (!soundOn) { soundOn = true; mSndMgr->getSound("sndBg")->play(); }
		else if (soundOn) { soundOn = false; mSndMgr->getSound("sndBg")->pause(); }
	}
	else BaseApplication::keyPressed(evt);
    return true;
}

bool Game::keyReleased( const OIS::KeyEvent& evt ){
	gamestates[0]->keyReleased(evt);
	// if (state == GAMEST_SINGLE && evt.key == OIS::KC_LEFT) { mPaddle1->motion &= ~1; mPaddle2->motion &= ~1; }
	// else if (state == GAMEST_SINGLE && evt.key == OIS::KC_RIGHT) { mPaddle1->motion &= ~2; mPaddle2->motion &= ~2; }
	// else 
		BaseApplication::keyReleased(evt);
	return true;
}

void Game::gyroMoved(int dev, double x, double y, double raw_x, double raw_y) {

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
