#include "Game.h"

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);

bool HandleContacts(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1) {
   GameObject *o0 = (GameObject *) body0->getUserPointer();
   GameObject *o1 = (GameObject *) body1->getUserPointer();
   o0->getGame()->collission(o0, o1);
   return true;
}

Game::Game(void) : BaseApplication() {
	remPaddlePos = 0;
	soundOn = true;
    level=0;
	state = GAMEST_MENU;
	nextState = -1;
	mGyroInput = nullptr;
}

Game::~Game() {
	delete mGyroInput;
}

void Game::reset(){
	elapsedSec = 0;
	score[0] = score[1] = 0; player = 2;
	lastHit = 0;
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
//-------------------------------------------------------------------------------------
void Game::createFrameListener() {
  BaseApplication::createFrameListener();
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
		player = (o1 == mPaddle1) ? 0 : 1;
		break;
	case K::PIT:
		scoreBox->setText("Score: 0");
		reset();
		break;
	case K::COIN:
		Coin *coin = dynamic_cast<Coin *>(o1);
		if (coin->taken) break;
		score[player]++;
		printf("Score: player0=%d, player1=%d\n", score[0], score[1]);
		scoreBox->setText("Score: " + std::to_string(score[0]));
		if (score[player] == maxScore){
			sounds[3]++;
            level++;
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

void Game::cleanWorld(void){
	std::set<GameObject*> newWorld;
	for(GameObject *obj : entities) {
		if (dynamic_cast<Coin *>(obj) || dynamic_cast<Obstacle *>(obj)) {
			delete obj;
		} else newWorld.insert(obj);
	}
	entities = newWorld;
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

	mPaddle1 = new Paddle(this,1);
	entities.insert(mPaddle1);

    mPaddle2 = new Paddle(this,2);
    entities.insert(mPaddle2);

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

    maxScore = 13;

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

    elapsedSec = 0;

    cleanWorld();

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
	    CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Quit");
	    quit->setText("Quit");
	    quit->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    quit->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.82,0)));
	    menu->addChild(quit);

		//the pause button
	    pause = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Pause");
	    pause->setText("Pause");
	    pause->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));

	    //the pause menu
	    pauseMenu = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/PauseMenu");
	    pauseMenu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	    pauseMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

        //the resume button
	    CEGUI::Window *resume = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Pause/PauseMenu/Resume");
	    resume->setText("Resume Game");
	    resume->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    resume->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.16,0)));
	    pauseMenu->addChild(resume);

	    //the main menu button (in the pause menu)
	    CEGUI::Window *mainMenu = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Pause/PauseMenu/MainMenu");
	    mainMenu->setText("Main Menu");
	    mainMenu->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    mainMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.4,0)));
	    pauseMenu->addChild(mainMenu);

	    //the sound button (in the pause menu)
	    soundPause = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Pause/PauseMenu/Sound");
	    soundPause->setText("Disable Sound");
	    soundPause->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    soundPause->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.64,0)));
	    pauseMenu->addChild(soundPause);

	    //the settings menu
	    settingsMenu = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Settings/SettingsMenu");
	    settingsMenu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	    settingsMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	    //the sound button (in the settings menu)
	    sound = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/SettingsMenu/Sound");
	    sound->setText("Disable Sound");
	    sound->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    sound->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.28,0)));
	    settingsMenu->addChild(sound);

	    //the back button (in the settings menu)
	    CEGUI::Window *back = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/SettingsMenu/Back");
	    back->setText("Back");
	    back->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    back->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.52,0)));
	    settingsMenu->addChild(back);

        //the how to play menu
	    howToPlayMenu = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Menu/howToPlayMenu");
	    howToPlayMenu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	    howToPlayMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	    //the back button (in the how to play menu)
	    CEGUI::Window *backHTP = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/howToPlayMenu/Back");
	    backHTP->setText("Back");
	    backHTP->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    backHTP->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.82,0)));
	    howToPlayMenu->addChild(backHTP);

        //the score box
	    scoreBox = wmgr.createWindow("TaharezLook/StaticText", "OgrePinball/Pause/PauseMenu/Sound");
	    scoreBox->setText("Score: 0");
	    scoreBox->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
	    scoreBox->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.9,0), CEGUI::UDim(0,0)));

        //the single player connection screen
	    singlePlayerConnection = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Menu/SinglePlayer/SinglePlayerConnection");
	    singlePlayerConnection->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	    singlePlayerConnection->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	    //the multiplayer connection screen
	    multiPlayerConnection = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Menu/Multiplayer/multiPlayerConnection");
	    multiPlayerConnection->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	    multiPlayerConnection->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	    //the back button (in the multiplayer connection screen)
	    CEGUI::Window *backMCS = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Multiplayer/MCS/Back");
	    backMCS->setText("Back");
	    backMCS->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    backMCS->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.82,0)));
	    multiPlayerConnection->addChild(backMCS);

        //the waiting for connection text box
	    CEGUI::Window *info = wmgr.createWindow("TaharezLook/StaticText", "OgrePinball/Menu/Muliplayer/MCS/Info");
	    info->setText("Waiting on connection with the controllers");
	    info->setSize(CEGUI::USize(CEGUI::UDim(0.96, 0), CEGUI::UDim(0.3, 0)));
	    info->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.02,0), CEGUI::UDim(0.02,0)));
	    multiPlayerConnection->addChild(info);

		//the play with keys button
	    CEGUI::Window *keysMCS = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Multiplayer/MCS/Back");
	    keysMCS->setText("Play With Keys");
	    keysMCS->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    keysMCS->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.62,0)));
	    multiPlayerConnection->addChild(keysMCS);

	    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
	    onePlayer->   subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::startSinglePlayer, this));
	    twoPlayer->   subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::startMultiPlayer,  this));
    	quit->        subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::quit,              this));		
		pause->       subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::pauseGame,         this));
   		resume->      subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::resumeGame,        this));
   		mainMenu->    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::openMainMenu,      this));
		settings->    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::openSettingsMenu,  this));
		back->        subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::goBack,            this));
  	  	howToPlay->   subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::openHowToPlayMenu, this)); 
   		backHTP->     subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::goBackHTP,         this));	
   		backMCS->     subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::goBackMCS,         this));
      	keysMCS->     subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::playKeysMCS,         this));	
	}
}

bool Game::frameStarted(const Ogre::FrameEvent& evt) {
	if (state != GAMEST_SINGLE && state != GAMEST_MULTI) return true;
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameEnded(const Ogre::FrameEvent& evt) {
	if (state != GAMEST_SINGLE && state != GAMEST_MULTI) return true;
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt){
	if(!BaseApplication::frameRenderingQueued(evt)) return false;
	mGyroInput->capture();

    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

    if (state == GAMEST_CONN_SINGLE && mGyroInput->connect() >= 1) {
    	state = GAMEST_SINGLE; reset();
    } else if (state == GAMEST_CONN_MULTI && mGyroInput->connect() >= 2) {
    	if (multiPlayerConnection) sheet->removeChild(multiPlayerConnection);
    	state = GAMEST_MULTI; reset();
    }

    if (state != GAMEST_SINGLE && state != GAMEST_MULTI) return true;

	for(GameObject *obj : entities)
		obj->update(evt);

    return true;
}

bool Game::keyPressed( const OIS::KeyEvent& evt ){
	if (evt.key == OIS::KC_LEFT) {
		if (state == GAMEST_SINGLE) { mPaddle1->motion |= 1; mPaddle2->motion |= 1; }
		else if (state == GAMEST_MULTI) mPaddle1->motion |= 1;
	} else if (evt.key == OIS::KC_RIGHT) {
		if (state == GAMEST_SINGLE) { mPaddle1->motion |= 2; mPaddle2->motion |= 2; }
		else if (state == GAMEST_MULTI) mPaddle1->motion |= 2;
	} else if (evt.key == OIS::KC_A) {
		if (state == GAMEST_MULTI) mPaddle2->motion |= 1;
	} else if (evt.key == OIS::KC_D) {
		if (state == GAMEST_MULTI) mPaddle2->motion |= 2;
	} else if (evt.key == OIS::KC_M) {
		if (!soundOn) { soundOn = true; mSndMgr->getSound("sndBg")->play(); }
		else if (soundOn) { soundOn = false; mSndMgr->getSound("sndBg")->pause(); }
	} else if (evt.key == OIS::KC_8) {
	    mCamera->setPosition(Ogre::Vector3(0,0,1300));
	    mCamera->lookAt(Ogre::Vector3(0,0,0));
	} else if (evt.key == OIS::KC_9) {
	    mCamera->setPosition(Ogre::Vector3(0,850,800));
	    mCamera->lookAt(Ogre::Vector3(0,100,0));
	} else if (evt.key == OIS::KC_0) {
	    mCamera->setPosition(Ogre::Vector3(0,-920,600));
	    mCamera->lookAt(Ogre::Vector3(0,-100,0));
	} else BaseApplication::keyPressed(evt);
    return true;
}

bool Game::keyReleased( const OIS::KeyEvent& evt ){
	if (evt.key == OIS::KC_LEFT) {
		if (state == GAMEST_SINGLE) { mPaddle1->motion &= ~1; mPaddle2->motion &= ~1; }
		else if (state == GAMEST_MULTI) mPaddle1->motion &= ~1;
	} else if (evt.key == OIS::KC_RIGHT) {
		if (state == GAMEST_SINGLE) { mPaddle1->motion &= ~2; mPaddle2->motion &= ~2; }
		else if (state == GAMEST_MULTI) mPaddle1->motion &= ~2;
	} else if (evt.key == OIS::KC_A) {
		if (state == GAMEST_MULTI) mPaddle2->motion &= ~1;
	} else if (evt.key == OIS::KC_D) {
		if (state == GAMEST_MULTI) mPaddle2->motion &= ~2;
	} else BaseApplication::keyReleased(evt);
	return true;
}
//-------------------------------------------------------------------------------------
bool Game::mouseMoved( const OIS::MouseEvent &arg )
{
  CEGUI::GUIContext &context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
  
  // Scroll wheel.
  if (arg.state.Z.rel)
    context.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
  return true;
}
//------------------------------------------------------------------------------------- 
bool Game::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
  return true;  
}
//-------------------------------------------------------------------------------------
bool Game::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
  return true;
}
//-------------------------------------------------------------------------------------
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}
//-------------------------------------------------------------------------------------
bool Game::quit(const CEGUI::EventArgs &e)
{
  mShutDown = true;
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::startSinglePlayer(const CEGUI::EventArgs &e)
{
  sheet->removeChild(menu);
  sheet->addChild(pause);
  sheet->addChild(scoreBox);
  state = GAMEST_SINGLE; 
  reset();
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::pauseGame(const CEGUI::EventArgs &e)
{
  sheet->removeChild(pause);
  sheet->removeChild(scoreBox);
  sheet->addChild(pauseMenu);
  state = GAMEST_MENU;
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::resumeGame(const CEGUI::EventArgs &e)
{
  sheet->removeChild(pauseMenu);
  sheet->addChild(pause);
  sheet->addChild(scoreBox);
  state = GAMEST_SINGLE;
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::openMainMenu(const CEGUI::EventArgs &e)
{
  sheet->removeChild(pauseMenu);
  sheet->addChild(menu);
  state = GAMEST_MENU;
}
//-------------------------------------------------------------------------------------
bool Game::openSettingsMenu(const CEGUI::EventArgs &e)
{
  sheet->removeChild(menu);
  sheet->addChild(settingsMenu);
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::goBack(const CEGUI::EventArgs &e)
{
  sheet->removeChild(settingsMenu);
  sheet->addChild(menu);
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::openHowToPlayMenu(const CEGUI::EventArgs &e)
{
  sheet->removeChild(menu);
  sheet->addChild(howToPlayMenu);

  return true;
}
//-------------------------------------------------------------------------------------
bool Game::goBackHTP(const CEGUI::EventArgs &e)
{
  sheet->removeChild(howToPlayMenu);
  sheet->addChild(menu);

  return true;
}
//-------------------------------------------------------------------------------------
bool Game::goBackMCS(const CEGUI::EventArgs &e)
{
  sheet->removeChild(multiPlayerConnection);
  sheet->addChild(menu);

  return true;
}
//-------------------------------------------------------------------------------------
bool Game::startMultiPlayer(const CEGUI::EventArgs &e)
{
  sheet->removeChild(menu);
  sheet->addChild(multiPlayerConnection);
  state = GAMEST_CONN_MULTI;
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::playKeysMCS(const CEGUI::EventArgs &e)
{
  sheet->removeChild(multiPlayerConnection);
  state = GAMEST_MULTI;
  reset();
  return true;
}
//-------------------------------------------------------------------------------------

void Game::gyroKeyPressed(int dev, int keycode) {
	if (keycode == GK_ZOOMIN) {
		if (state == GAMEST_SINGLE) { mPaddle1->tilt(-1);  mPaddle2->tilt(-1); }
		else if (state == GAMEST_MULTI && dev == 0) mPaddle1->tilt(-1);
		else if (state == GAMEST_MULTI && dev == 1) mPaddle2->tilt(-1);
	} else if (keycode == GK_ZOOMOUT) {
		if (state == GAMEST_SINGLE) { mPaddle1->tilt(1);  mPaddle2->tilt(1); }
		else if (state == GAMEST_MULTI && dev == 0) mPaddle1->tilt(1);
		else if (state == GAMEST_MULTI && dev == 1) mPaddle2->tilt(1);
	} else if (keycode == GK_MUTE) {
		if (!soundOn) { soundOn = true; mSndMgr->getSound("sndBg")->play(); }
		else if (soundOn) { soundOn = false; mSndMgr->getSound("sndBg")->pause(); }
	}
	printf("Gyro key pressed, device=%d, keycode=%d\n", dev, keycode);
}

void Game::gyroKeyReleased(int dev, int keycode) {
	if (keycode == GK_ZOOMOUT || keycode == GK_ZOOMIN) {
		if (state == GAMEST_SINGLE) { mPaddle1->tilt(0);  mPaddle2->tilt(0); }
		else if (state == GAMEST_MULTI && dev == 0) mPaddle1->tilt(0);
		else if (state == GAMEST_MULTI && dev == 1) mPaddle2->tilt(0);
	}
	printf("Gyro key released, device=%d, keycode=%d\n", dev, keycode);
}

void Game::gyroMoved(int dev, double x, double y, double raw_x, double raw_y) {
	if (state == GAMEST_SINGLE) {
		mPaddle1->gyroMovement(x);
		mPaddle2->gyroMovement(x);
	} else if (state == GAMEST_MULTI) {
		if (dev == 0) mPaddle1->gyroMovement(x);
		if (dev == 1) mPaddle2->gyroMovement(x);
	}
	printf("Gyro moved, device=%d, x=%f, y=%f, raw_x=%f, raw_y=%f\n", dev, x, y, raw_x, raw_y);
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
