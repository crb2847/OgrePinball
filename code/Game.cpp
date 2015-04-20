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
	sounds[0] = sounds[1] = sounds[2] = 0;
	if (soundOn) mSndMgr->getSound("sndBg")->play();
}

void Game::createFrameListener() {
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

  mInputManager = OIS::InputManager::createInputSystem( pl );

  mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
  mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

  mMouse->setEventCallback(this);
  mKeyboard->setEventCallback(this);

  //Set initial mouse clipping size
  windowResized(mWindow);

  //Register as a Window listener
  Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

  mRoot->addFrameListener(this);
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
		break;
	case K::PADDLE:
		if (soundOn) mSndMgr->getSound("sndPaddle")->play();
		sounds[1]++;
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
		sounds[2]++;
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
	// https://www.freesound.org/people/sirplus/sounds/104831/ (CC license)
	mSndMgr->createSound("sndBg", "slowspyk.ogg", false, true, true);
	mSndMgr->getSound("sndBg")->setVolume(0.2);

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

    elapsedSec = 0; score = 0;

    //initalize CEGUI components
    {
	    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	    CEGUI::Font::setDefaultResourceGroup("Fonts");
	    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme"); 

	    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	    sheet = wmgr.createWindow("DefaultWindow", "OgrePinball/Sheet");

	    //the start menu
	    menu = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Menu");
	    menu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	    menu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));
	    sheet->addChild(menu);

	    //the one-player game button
	    CEGUI::Window *onePlayer = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/OnePlayer");
	    onePlayer->setText("Single Player Game");
	    onePlayer->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    onePlayer->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.02,0)));
	    menu->addChild(onePlayer);

	    //the two-player game button
	    CEGUI::Window *twoPlayer = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/TwoPlayer");
	    twoPlayer->setText("Multiplayer Player Game");
	    twoPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    twoPlayer->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.22,0)));
	    menu->addChild(twoPlayer);

	    //the settings button
	    CEGUI::Window *settings = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Settings");
	    settings->setText("Settings");
	    settings->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    settings->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.42,0)));
	    menu->addChild(settings);

	    //the how to play button
	    CEGUI::Window *howToPlay = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/HowToPlay");
	    howToPlay->setText("How To Play");
	    howToPlay->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    howToPlay->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.62,0)));
	    menu->addChild(howToPlay);

	    //the quit button
	    CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "Raquetball/Menu/Quit");
	    quit->setText("Quit");
	    quit->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    quit->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.82,0)));
	    menu->addChild(quit);

	    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
	}
}

bool Game::frameStarted(const Ogre::FrameEvent& evt) {
	if (state != GAMEST_SERVER && state != GAMEST_SINGLE) return true;
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameEnded(const Ogre::FrameEvent& evt) {
	if (state != GAMEST_SERVER && state != GAMEST_SINGLE) return true;
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt){
	if(!BaseApplication::frameRenderingQueued(evt)) return false;

	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	//Need to inject timestamps to CEGUI System.
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	if (state == GAMEST_SERVER || state == GAMEST_SINGLE)
		elapsedSec = (clock()-gameStart)/CLOCKS_PER_SEC;


	if (state == GAMEST_MENU) {


		return true;
	} else if (state == GAMEST_CONNECT) {

		bool r_server;
		if (net.connect(&r_server)) {
			state = r_server ? GAMEST_SERVER : GAMEST_CLIENT;
			reset();
		} else {
			return true;
		}
	} else {

	}

	NetworkData_t netout, netin;
	int ci = 0;

	for(GameObject *obj : entities) {
		obj->update(evt);
		Coin *coin = dynamic_cast<Coin *>(obj);
		if (coin) netout.coins[ci++] = coin->taken;
	}

	if (state == GAMEST_SINGLE) return true;

	netout.paddle1Pos = mPaddle1->rootNode->getPosition().x;
	netout.paddle2Pos = mPaddle2->rootNode->getPosition().x;
	netout.score = score;
	//netout.score2 = score2;
	netout.ballX = oBall->rootNode->getPosition().x;
	netout.ballY = oBall->rootNode->getPosition().y;
	netout.ballRotation = oBall->rootNode->getOrientation();
	netout.type = NET_UPDATE;
	netout.time = elapsedSec;
	netout.sounds[0] = sounds[0]; netout.sounds[1] = sounds[1]; netout.sounds[2] = sounds[2];
	net.write(&netout);

	while (net.read(&netin)) {
		if (netin.type != NET_UPDATE) continue;
		if (state == GAMEST_SERVER) {
			mPaddle2->setPosition(Ogre::Vector3(netin.paddle2Pos, -490, 0.0));
		} else {
			if (soundOn && netin.sounds[0] != sounds[0]) mSndMgr->getSound("sndHit")->play();
			if (soundOn && netin.sounds[1] != sounds[1]) mSndMgr->getSound("sndPaddle")->play();
			if (soundOn && netin.sounds[2] != sounds[2]) mSndMgr->getSound("sndScore")->play();
			sounds[0] = netin.sounds[0]; sounds[1] = netin.sounds[1]; sounds[2] = netin.sounds[2];
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
	if (state == GAMEST_MENU && (evt.key == OIS::KC_1 || evt.key == OIS::KC_NUMPAD1)) { state = GAMEST_SINGLE; reset(); }
	else if (state == GAMEST_MENU && (evt.key == OIS::KC_2 || evt.key == OIS::KC_NUMPAD2)) state = GAMEST_CONNECT;
	else if (state == GAMEST_SERVER && evt.key == OIS::KC_LEFT) mPaddle1->motion |= 1;
	else if (state == GAMEST_CLIENT && evt.key == OIS::KC_LEFT) mPaddle2->motion |= 1;
	else if (state == GAMEST_SERVER && evt.key == OIS::KC_RIGHT) mPaddle1->motion |= 2;
	else if (state == GAMEST_CLIENT && evt.key == OIS::KC_RIGHT) mPaddle2->motion |= 2;
	else if (state == GAMEST_SINGLE && evt.key == OIS::KC_LEFT) { mPaddle1->motion |= 1; mPaddle2->motion |= 1; }
	else if (state == GAMEST_SINGLE && evt.key == OIS::KC_RIGHT) { mPaddle1->motion |= 2; mPaddle2->motion |= 2; }
	else if (evt.key == OIS::KC_M){
		if (!soundOn) { soundOn = true; mSndMgr->getSound("sndBg")->play(); }
		else if (soundOn) { soundOn = false; mSndMgr->getSound("sndBg")->pause(); }
	}
	else BaseApplication::keyPressed(evt);
    return true;
}

bool Game::keyReleased( const OIS::KeyEvent& evt ){
	if (state == GAMEST_SERVER && evt.key == OIS::KC_LEFT) mPaddle1->motion &= ~1;
	else if (state == GAMEST_CLIENT && evt.key == OIS::KC_LEFT) mPaddle2->motion &= ~1;
	else if (state == GAMEST_SERVER && evt.key == OIS::KC_RIGHT) mPaddle1->motion &= ~2;
	else if (state == GAMEST_CLIENT && evt.key == OIS::KC_RIGHT) mPaddle2->motion &= ~2;
	else if (state == GAMEST_SINGLE && evt.key == OIS::KC_LEFT) { mPaddle1->motion &= ~1; mPaddle2->motion &= ~1; }
	else if (state == GAMEST_SINGLE && evt.key == OIS::KC_RIGHT) { mPaddle1->motion &= ~2; mPaddle2->motion &= ~2; }
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
