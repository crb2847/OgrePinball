#include "Game.h"

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);

bool HandleContacts(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1) {
   GameObject *o0 = (GameObject *) body0->getUserPointer();
   GameObject *o1 = (GameObject *) body1->getUserPointer();
   o0->getGame()->collission(o0, o1);
   return true;
}

Game::Game(void) : BaseApplication(), net() {
	remPaddlePos = 0;
	soundOn = true;
	state = GAMEST_SINGLE;
	mGyroInput = nullptr;

	gamestates[GAMEST_MENU] = new GameState();
	gamestates[GAMEST_CONNECT] = new GameState();
	gamestates[GAMEST_SINGLE] = new SinglePlayerState(this);
	gamestates[GAMEST_MULTI] = new GameState();
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

    for (auto &st : gamestates)
    	st.second->createState();

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

		//the pause button
	    pause = wmgr.createWindow("TaharezLook/Button", "Raquetball/Pause");
	    pause->setText("Pause");
	    pause->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));

	    //the pause menu
	    pauseMenu = wmgr.createWindow("TaharezLook/FrameWindow", "Raquetball/PauseMenu");
	    pauseMenu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	    pauseMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

        //the resume button
	    CEGUI::Window *resume = wmgr.createWindow("TaharezLook/Button", "Raquetball/Pause/PauseMenu/Resume");
	    resume->setText("Resume Game");
	    resume->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    resume->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.16,0)));
	    pauseMenu->addChild(resume);

	    //the main menu button (in the pause menu)
	    CEGUI::Window *mainMenu = wmgr.createWindow("TaharezLook/Button", "Raquetball/Pause/PauseMenu/MainMenu");
	    mainMenu->setText("Main Menu");
	    mainMenu->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    mainMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.4,0)));
	    pauseMenu->addChild(mainMenu);

	    //the sound button (in the pause menu)
	    soundPause = wmgr.createWindow("TaharezLook/Button", "Raquetball/Pause/PauseMenu/Sound");
	    soundPause->setText("Disable Sound");
	    soundPause->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    soundPause->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.64,0)));
	    pauseMenu->addChild(soundPause);

	    //the settings menu
	    settingsMenu = wmgr.createWindow("TaharezLook/FrameWindow", "Raquetball/Settings/SettingsMenu");
	    settingsMenu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	    settingsMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	    //the sound button (in the settings menu)
	    sound = wmgr.createWindow("TaharezLook/Button", "Raquetball/Menu/SettingsMenu/Sound");
	    sound->setText("Disable Sound");
	    sound->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    sound->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.28,0)));
	    settingsMenu->addChild(sound);

	    //the back button (in the settings menu)
	    CEGUI::Window *back = wmgr.createWindow("TaharezLook/Button", "Raquetball/Menu/SettingsMenu/Back");
	    back->setText("Back");
	    back->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	    back->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.52,0)));
	    settingsMenu->addChild(back);

        //the how to play menu
	    howToPlayMenu = wmgr.createWindow("TaharezLook/FrameWindow", "Raquetball/Menu/howToPlayMenu");
	    howToPlayMenu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	    howToPlayMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	    //the back button (in the how to play menu)
	    CEGUI::Window *backHTP = wmgr.createWindow("TaharezLook/Button", "Raquetball/Menu/howToPlayMenu/Back");
	    backHTP->setText("Back");
	    backHTP->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	    backHTP->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.82,0)));
	    howToPlayMenu->addChild(backHTP);

        //the score box
	    scoreBox = wmgr.createWindow("TaharezLook/StaticText", "Raquetball/Pause/PauseMenu/Sound");
	    scoreBox->setText("Score: 0");
	    scoreBox->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
	    scoreBox->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.9,0), CEGUI::UDim(0,0)));

	    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
	    onePlayer->   subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::startSinglePlayer, this));
    	quit->        subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::quit,              this));		
		pause->       subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::pauseGame,         this));
   		resume->      subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::resumeGame,        this));
   		mainMenu->    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::openMainMenu,      this));
		settings->    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::openSettingsMenu,  this));
		back->        subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::goBack,            this));
  	  	howToPlay->   subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::openHowToPlayMenu, this)); 
   		backHTP->     subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::goBackHTP,         this));		
	}
}

bool Game::frameStarted(const Ogre::FrameEvent& evt) {
	gamestates[state]->frameStarted(evt);
	return true;
}

bool Game::frameEnded(const Ogre::FrameEvent& evt) {
	gamestates[state]->frameEnded(evt);
	return true;
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt){
	//gamestates[state]->frameRender();
	if(!BaseApplication::frameRenderingQueued(evt)) return false;
	mGyroInput->capture();

    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	if (state == GAMEST_MENU) {
		return true;
	}

	for(GameObject *obj : entities)
		obj->update(evt);

    return true;
}

bool Game::keyPressed( const OIS::KeyEvent& evt ){
	if (gamestates[state]->keyPressed(evt)) {}
	else if (state == GAMEST_MENU && (evt.key == OIS::KC_1 || evt.key == OIS::KC_NUMPAD1)) { state = GAMEST_SINGLE; reset(); }
	else if (state == GAMEST_MENU && (evt.key == OIS::KC_2 || evt.key == OIS::KC_NUMPAD2)) state = GAMEST_CONNECT;
	// else if (state == GAMEST_SINGLE && evt.key == OIS::KC_LEFT) { mPaddle1->motion |= 1; mPaddle2->motion |= 1; }
	// else if (state == GAMEST_SINGLE && evt.key == OIS::KC_RIGHT) { mPaddle1->motion |= 2; mPaddle2->motion |= 2; }
	else if (evt.key == OIS::KC_M){
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
	}

	else BaseApplication::keyPressed(evt);
    return true;
}

bool Game::keyReleased( const OIS::KeyEvent& evt ){
	if (gamestates[state]->keyReleased(evt)) {}
	else BaseApplication::keyReleased(evt);
	// if (state == GAMEST_SINGLE && evt.key == OIS::KC_LEFT) { mPaddle1->motion &= ~1; mPaddle2->motion &= ~1; }
	// else if (state == GAMEST_SINGLE && evt.key == OIS::KC_RIGHT) { mPaddle1->motion &= ~2; mPaddle2->motion &= ~2; }
	// else 
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

void Game::gyroKeyPressed(int dev, int keycode) {
	printf("Gyro key pressed, device=%d, keycode=%d\n", dev, keycode);
}

void Game::gyroKeyReleased(int dev, int keycode) {
	printf("Gyro key released, device=%d, keycode=%d\n", dev, keycode);
}

void Game::gyroMoved(int dev, double x, double y, double raw_x, double raw_y) {
	printf("Gyro moved, device=%d, x=%f, y=%f, raw_x=%f, raw_y=%f\n", dev, x, y, raw_x, raw_y);
	if (mPaddle1) mPaddle1->gyroMovement(x);
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
