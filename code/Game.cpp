#include "Game.h"

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);

bool HandleContacts(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1) {
   GameObject *o0 = (GameObject *) body0->getUserPointer();
   GameObject *o1 = (GameObject *) body1->getUserPointer();
   o0->getGame()->collission(o0, o1);
   return true;
}

Game::Game(void) : BaseApplication() {
	soundOn = true;
    level=0;
	state = GAMEST_MENU;
	mGyroInput = nullptr;
	mscore = 0;
	mPaddle0 = mPaddle1 = mPaddle2 = nullptr;
}

Game::~Game() {
	delete mGyroInput;
}

void Game::setLevel(){
	if(level==0){
		maxScore = 13;
		std::vector<Ogre::Vector3> coinPos { 
			Ogre::Vector3(100,100,0), Ogre::Vector3(-100,400,0), Ogre::Vector3(-100,150,0), 
			Ogre::Vector3(-50,200,0), Ogre::Vector3(-250,300,0), Ogre::Vector3(280,400,0), 
			Ogre::Vector3(-250,0,0), Ogre::Vector3(-300,100,0), Ogre::Vector3(-300,-400,0), 
			Ogre::Vector3(-100,-300,0), Ogre::Vector3(0,-350,0), Ogre::Vector3(100,-270,0), Ogre::Vector3(200,-340,0)};
	    for (Ogre::Vector3 p : coinPos)
	    	entities.insert(new Coin(this, p));
	    std::vector<Ogre::Vector3> obstaclePos { Ogre::Vector3(-300,-150,0), Ogre::Vector3(300,-150,0),
	    	Ogre::Vector3(-250,150,0), Ogre::Vector3(250,150,0) };
	    for (Ogre::Vector3 p : obstaclePos)
	    	entities.insert(new Obstacle(this, p));
	}
	if(level==1){
		maxScore = 13;
    	std::vector<Ogre::Vector3> coinPos { Ogre::Vector3(100,100,0), Ogre::Vector3(-100,400,0), Ogre::Vector3(-100,150,0), 
    		Ogre::Vector3(-50,200,0), Ogre::Vector3(-250,300,0), Ogre::Vector3(280,400,0), Ogre::Vector3(-250,0,0), 
    		Ogre::Vector3(-300,100,0), Ogre::Vector3(-300,-400,0), Ogre::Vector3(-100,-300,0), Ogre::Vector3(0,-350,0), 
    		Ogre::Vector3(100,-270,0), Ogre::Vector3(200,-340,0)}; //13 coins
	    for (Ogre::Vector3 p : coinPos)
	    	entities.insert(new Coin(this, p));

	    std::vector<Ogre::Vector3> obstaclePos { Ogre::Vector3(0,400,0), Ogre::Vector3(0,325,0),
	    	Ogre::Vector3(-500,50,0), Ogre::Vector3(500,0,0) };
	    for (Ogre::Vector3 p : obstaclePos)
	    	entities.insert(new Obstacle(this, p));

	    std::vector<Ogre::Vector3> brickPos { Ogre::Vector3(-350,-250,0),  Ogre::Vector3(350,-250,0),
	    	Ogre::Vector3(-350,250,0),Ogre::Vector3(350,250,0)};
	    for (Ogre::Vector3 p : brickPos)
    	entities.insert(new Brick(this, p,2,false));

    } else if(level==2){
		maxScore = 10;
    	std::vector<Ogre::Vector3> coinPos { 
    		Ogre::Vector3(100,100,0), Ogre::Vector3(-100,400,0), Ogre::Vector3(-100,150,0), 
    		Ogre::Vector3(-50,200,0), Ogre::Vector3(-250,300,0), Ogre::Vector3(280,400,0)};
	    for (Ogre::Vector3 p : coinPos)
	    	entities.insert(new Coin(this, p));

	    std::vector<Ogre::Vector3> obstaclePos { 
	    	Ogre::Vector3(250,0,0), Ogre::Vector3(-250,0,0)};
	    for (Ogre::Vector3 p : obstaclePos)
	    	entities.insert(new Obstacle(this, p));

	    std::vector<Ogre::Vector3> brickPos { 
	    	Ogre::Vector3(-350,-250,0), Ogre::Vector3(350,-250,0), Ogre::Vector3(-350,250,0),Ogre::Vector3(350,250,0),
		    Ogre::Vector3(0,250,0),  Ogre::Vector3(100,250,0),Ogre::Vector3(-100,250,0),Ogre::Vector3(200,250,0)};
	    for (Ogre::Vector3 p : brickPos)
    	entities.insert(new Brick(this, p,2,false));

    	std::vector<Ogre::Vector3> brickPosC { 
    		Ogre::Vector3(0,-250,0),  Ogre::Vector3(100,-250,0),Ogre::Vector3(-100,-250,0),Ogre::Vector3(200,-250,0)};
	    for (Ogre::Vector3 p : brickPosC)
    		entities.insert(new Brick(this, p,1,true));
    }
    else if(level==3){
		maxScore = 3;
		oBall->setPosition(Ogre::Vector3(400,0,0));
    	std::vector<Ogre::Vector3> obstaclePos { 
    		Ogre::Vector3(0,-200,0), Ogre::Vector3(0,200,0), Ogre::Vector3(275,0,0), Ogre::Vector3(-275,0,0),
    		Ogre::Vector3(315,200,0), Ogre::Vector3(-315,200,0)};
	    for (Ogre::Vector3 p : obstaclePos)
	    	entities.insert(new Obstacle(this, p));

	    std::vector<Ogre::Vector3> brickPos { 
	    	Ogre::Vector3(100,0,0), Ogre::Vector3(-100,0,0),
		    Ogre::Vector3(100,50,0),Ogre::Vector3(0,-50,0),Ogre::Vector3(0,50,0),
			Ogre::Vector3(-100,50,0),Ogre::Vector3(100,-50,0),Ogre::Vector3(-100,-50,0)};
	    for (Ogre::Vector3 p : brickPos)
    	entities.insert(new Brick(this, p,3,false));
    	
	    std::vector<Ogre::Vector3> brickPosC { Ogre::Vector3(0,0,0),Ogre::Vector3(200,0,0),Ogre::Vector3(-200,0,0)};
	    for (Ogre::Vector3 p : brickPosC)
    	entities.insert(new Brick(this, p,1,true));
    }
    else if(level==4){
		maxScore = 5;
		oBall->setPosition(Ogre::Vector3(0,400,0));
	    std::vector<Ogre::Vector3> brickPos { 
	    	Ogre::Vector3(-500,500,0),Ogre::Vector3(-500,450,0),Ogre::Vector3(-500,400,0),Ogre::Vector3(-400,450,0),
	    	Ogre::Vector3(-300,500,0),Ogre::Vector3(-300,450,0),Ogre::Vector3(-300,400,0),Ogre::Vector3(-100,500,0),
	    	Ogre::Vector3(-100,450,0),Ogre::Vector3(-100,400,0),
	    	Ogre::Vector3(-300,200,0),Ogre::Vector3(-300,150,0),Ogre::Vector3(-300,100,0),Ogre::Vector3(-300,50,0),Ogre::Vector3(-300,0,0),
	    	Ogre::Vector3(-200,0,0),Ogre::Vector3(-100,0,0),Ogre::Vector3(-100,100,0),Ogre::Vector3(-100,150,0),Ogre::Vector3(-100,50,0),
	    	Ogre::Vector3(-100,200,0),Ogre::Vector3(200,200,0),Ogre::Vector3(200,150,0),Ogre::Vector3(200,100,0),
	    	Ogre::Vector3(200,50,0),Ogre::Vector3(200,0,0),Ogre::Vector3(300,200,0),Ogre::Vector3(100,200,0) };
	    for (Ogre::Vector3 p : brickPos)
    	entities.insert(new Brick(this, p,4,false));

	    std::vector<Ogre::Vector3> brickPosC { 
	    	Ogre::Vector3(-250,-150,0),Ogre::Vector3(-400,250,0),Ogre::Vector3(350,-150,0),
	    	Ogre::Vector3(350,400,0),Ogre::Vector3(150,350,0) };
	    for (Ogre::Vector3 p : brickPosC)
    	entities.insert(new Brick(this, p,2,true));
    }
    else if(level==5){
		maxScore = 10;

	    std::vector<Ogre::Vector3> brickPos { Ogre::Vector3(-350,-250,0) };
	    for (Ogre::Vector3 p : brickPos)
    	entities.insert(new Brick(this, p,1,false));

	    std::vector<Ogre::Vector3> brickPosC { Ogre::Vector3(-350,-250,0) };
	    for (Ogre::Vector3 p : brickPosC)
    	entities.insert(new Brick(this, p,1,true));
    }
}

void Game::reset(){
	elapsedSec = 0;
	score[0] = score[1] = score[2] = 0; player = 0;
	lastHit = 0;
	gameStart = clock();
	mscore = 0;
	deleteMe.clear();
	if (state == GAMEST_SINGLE) {
		if (!mPaddle0) entities.insert(mPaddle0 = new Paddle(this, 0));
		if (mPaddle1) { entities.erase(mPaddle1); delete mPaddle1; mPaddle1 = nullptr; }
		if (mPaddle2) { entities.erase(mPaddle2); delete mPaddle2; mPaddle2 = nullptr; }
	} else if (state == GAMEST_MULTI) {
		if (mPaddle0) { entities.erase(mPaddle0); delete mPaddle0; mPaddle0 = nullptr; }
		if (!mPaddle1) entities.insert(mPaddle1 = new Paddle(this, 1));
		if (!mPaddle2) entities.insert(mPaddle2 = new Paddle(this, 2));
	}
	oBall->setPosition(Ogre::Vector3(0,0,0));
	oBall->rigidBody->setLinearVelocity(Ogre::Vector3(1.0f, 1.0f, 0.0f) * oBall->bSpeed);
    
    cleanWorld();
    setLevel();
    //entities.insert(new Brick(this, Ogre::Vector3(-250,-250,0), 1, true));
    //entities.insert(new Brick(this, Ogre::Vector3(-250,250,0), 2, true));

	scoreBox->setText("Score: 0");
	scoreBox1->setText("Player One Score: 0");
	scoreBox2->setText("Player Two Score: 0");

	if (soundOn) mSndMgr->getSound("sndBg")->play();
}
//-------------------------------------------------------------------------------------
void Game::createFrameListener() {
  BaseApplication::createFrameListener();
}

void Game::collission(GameObject *o0, GameObject *o1) {
	if (o0->kind > o1->kind) {GameObject *tmp = o0; o0 = o1; o1 = tmp;}
	if (o0->kind != K::BALL) return;

	Brick *brick; Coin *coin;

	switch(o1->kind){
	case K::WALL:
	case K::OBSTACLE:
		if((clock()-lastHit) > (CLOCKS_PER_SEC*0.15))
			if (soundOn) mSndMgr->getSound("sndHit")->play();
		lastHit = clock();
		if (oBall->rigidBody->getLinearVelocity().length() <= 500.0)
			oBall->rigidBody->setLinearVelocity(oBall->rigidBody->getLinearVelocity() * 1.1);
		break;
	case K::PADDLE:
		if (soundOn) mSndMgr->getSound("sndPaddle")->play();
		oBall->rigidBody->setLinearVelocity(
			oBall->rigidBody->getLinearVelocity().normalisedCopy() * 450.0);
		player = (o1 == mPaddle0) ? 0 : ((o1 == mPaddle1) ? 1 : 2);
		break;
	case K::PIT:
		reset();
		break;
	case K::BRICK:
		brick = dynamic_cast<Brick *>(o1);
		brick->points--;
		if (brick->points == 0) {
			if (brick->hasCoin)
				entities.insert(new Coin(this, brick->position));
			deleteMe.push_back(brick);
			//entities.erase(brick);
			//delete brick;
		}
		break;
	case K::COIN:
		coin = dynamic_cast<Coin *>(o1);
		entities.erase(coin);
		delete coin;

		score[player]++; mscore++;
		scoreBox->setText("Score: " + std::to_string(score[0]));
		scoreBox1->setText("Player One Score: " + std::to_string(score[1]));
		scoreBox2->setText("Player Two Score: " + std::to_string(score[2]));

		if (mscore == maxScore)
		{
		  sheet->removeChild(pause);
		  sheet->addChild(complete);
		  if(state == GAMEST_SINGLE)
		  {
		    sheet->removeChild(scoreBox);
		  	state = GAMEST_SINGLE_MENU;
		  }
		  else if(state == GAMEST_MULTI)
		  {
		  	sheet->removeChild(scoreBox1);
		  	sheet->removeChild(scoreBox2);
		  	state = GAMEST_MULTI_MENU;
		  }		
		}
		if (soundOn) mSndMgr->getSound("sndScore")->play();
		break;
	}
}

void Game::cleanWorld(void){
	std::set<GameObject*> newWorld;
	for(GameObject *obj : entities) {
		if (obj->kind == K::COIN || obj->kind == K::OBSTACLE || obj->kind == K::BRICK)
			delete obj;
		else newWorld.insert(obj);
	}
	entities = newWorld;
}

void Game::createScene(void){
	mGyroInput = new GyroInput();
	mGyroInput->setEventCallback(this);
	mGyroInput->connect();
	// Init Bullet
	Ogre::Vector3 gravityVector(0,-70,0); //Ogre::Vector3 gravityVector(0,-70,0);
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

    oBall = new Ball(this);
    entities.insert(oBall);

	//Create 6 walls
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1000, 1000, 20, 20, true, 1, 5.0, 5.0, Ogre::Vector3::UNIT_X);

    PitPlane* pit = new PitPlane(this, Ogre::Vector3::UNIT_Y, Ogre::Real(-510));
    entities.insert(pit); // Bottom

    Wall *p = new Wall(this, Ogre::Vector3(1.4f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Z), Ogre::Vector3(0, 500, 0));
    entities.insert(p); // Top

    p = new Wall(this, Ogre::Vector3(1.4f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, -100));
    entities.insert(p); // Back

    p = new Wall(this, Ogre::Vector3(1.4f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, 100));
    entities.insert(p); // Front

    p = new Wall(this, Ogre::Vector3(1.0f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_Z),  Ogre::Vector3(-700, 0, 0));
    entities.insert(p); // Left

    p = new Wall(this, Ogre::Vector3(1.0f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z), Ogre::Vector3(700, 0, 0));
    entities.insert(p); // Right

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("OutsideLight");
    light->setPosition(90.0f, 90.0f, 800.0f);

    elapsedSec = 0;
    initCEGUI();
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
    	startGame(GAMEST_SINGLE);
    } else if (state == GAMEST_CONN_MULTI && mGyroInput->connect() >= 2) {
    	startGame(GAMEST_MULTI);
    }

    if (state != GAMEST_SINGLE && state != GAMEST_MULTI) return true;

    for(GameObject *obj : deleteMe) {
    	if(entities.erase(obj))
    		delete obj;
    }
    deleteMe.clear();

	for(GameObject *obj : entities)
		obj->update(evt);

    return true;
}

void Game::startGame(int newState) {
	state = newState;
	if (newState == GAMEST_MULTI) {
		sheet->removeChild(multiPlayerConnection);
		sheet->addChild(scoreBox1);
		sheet->addChild(scoreBox2);
		sheet->addChild(pause);
	} else if (newState == GAMEST_SINGLE) {
		sheet->removeChild(singlePlayerConnection);
		sheet->addChild(scoreBox);
		sheet->addChild(pause);
	}
	reset();
}

bool Game::keyPressed( const OIS::KeyEvent& evt ){
	if (evt.key == OIS::KC_LEFT) {
		if (state == GAMEST_SINGLE && mPaddle0) mPaddle0->motion |= 1;
		else if (state == GAMEST_MULTI && mPaddle2) mPaddle2->motion |= 1;
	} 
	else if(evt.key == OIS::KC_SPACE){
		int random = rand()%4;
		oBall->rigidBody->setLinearVelocity(oBall->bDirection * 150);
		switch(random){
			case 0: oBall->bDirection = Ogre::Vector3(-1.0f, 1.0f, 0.0f);break;
			case 1: oBall->bDirection = Ogre::Vector3(-1.0f, -1.0f, 0.0f); break;
			case 2: oBall->bDirection = Ogre::Vector3(1.0f, 1.0f, 0.0f); break;
			case 3: oBall->bDirection = Ogre::Vector3(1.0f, -1.0f, 0.0f); break;
			case 4: oBall->bDirection = Ogre::Vector3(0.0f, 1.0f, 0.0f); break;
			case 5: oBall->bDirection = Ogre::Vector3(1.0f, 0.0f, 0.0f); break;
		}
	}
	else if (evt.key == OIS::KC_RIGHT) {
		if (state == GAMEST_SINGLE && mPaddle0) mPaddle0->motion |= 2;
		else if (state == GAMEST_MULTI && mPaddle2) mPaddle2->motion |= 2;
	} else if (evt.key == OIS::KC_A) {
		if (state == GAMEST_MULTI && mPaddle1) mPaddle1->motion |= 1;
	} else if (evt.key == OIS::KC_D) {
		if (state == GAMEST_MULTI && mPaddle1) mPaddle1->motion |= 2;
	} else if (evt.key == OIS::KC_M) {
		if (!soundOn) { soundOn = true; mSndMgr->getSound("sndBg")->play(); }
		else if (soundOn) { soundOn = false; mSndMgr->getSound("sndBg")->pause(); }
	} else if (evt.key == OIS::KC_8) {
	    mCamera->setPosition(Ogre::Vector3(0,0,1300));
	    mCamera->lookAt(Ogre::Vector3(0,0,0));
	} else if (evt.key == OIS::KC_9) {
	    mCamera->setPosition(Ogre::Vector3(0,900,900));
	    mCamera->lookAt(Ogre::Vector3(0,0,50));
	} else if (evt.key == OIS::KC_0) {
	    mCamera->setPosition(Ogre::Vector3(0,-1100,900));
	    mCamera->lookAt(Ogre::Vector3(0,0,84));
	} else if (evt.key == OIS::KC_1) {
		if(level <5) {level++; reset();}
	} else BaseApplication::keyPressed(evt);
    return true;
}

bool Game::keyReleased( const OIS::KeyEvent& evt ){
	if (evt.key == OIS::KC_LEFT) {
		if (state == GAMEST_SINGLE && mPaddle0) mPaddle0->motion &= ~1;
		else if (state == GAMEST_MULTI && mPaddle2) mPaddle2->motion &= ~1;
	} else if (evt.key == OIS::KC_RIGHT) {
		if (state == GAMEST_SINGLE && mPaddle0) mPaddle0->motion &= ~2;
		else if (state == GAMEST_MULTI && mPaddle2) mPaddle2->motion &= ~2;
	} else if (evt.key == OIS::KC_A) {
		if (state == GAMEST_MULTI && mPaddle1) mPaddle1->motion &= ~1;
	} else if (evt.key == OIS::KC_D) {
		if (state == GAMEST_MULTI && mPaddle1) mPaddle1->motion &= ~2;
	} else BaseApplication::keyReleased(evt);
	return true;
}

bool Game::mouseMoved( const OIS::MouseEvent &arg ) {
  CEGUI::GUIContext &context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
  // Scroll wheel.
  if (arg.state.Z.rel)
    context.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
  return true;
}

bool Game::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
  return true;  
}

bool Game::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
  return true;
}

void Game::gyroKeyPressed(int dev, int keycode) {
	if (keycode == GK_ZOOMIN) {
		if (state == GAMEST_SINGLE && mPaddle0) mPaddle0->tilt(-1);
		else if (state == GAMEST_MULTI && dev == 0 && mPaddle1) mPaddle1->tilt(-1);
		else if (state == GAMEST_MULTI && dev == 1 && mPaddle2) mPaddle2->tilt(-1);
	} else if (keycode == GK_ZOOMOUT) {
		if (state == GAMEST_SINGLE && mPaddle0) mPaddle0->tilt(1);
		else if (state == GAMEST_MULTI && dev == 0 && mPaddle1) mPaddle1->tilt(1);
		else if (state == GAMEST_MULTI && dev == 1 && mPaddle2) mPaddle2->tilt(1);
	} else if (keycode == GK_MUTE) {
		if (!soundOn) { soundOn = true; mSndMgr->getSound("sndBg")->play(); }
		else if (soundOn) { soundOn = false; mSndMgr->getSound("sndBg")->pause(); }
	}
}

void Game::gyroKeyReleased(int dev, int keycode) {
	if (keycode == GK_ZOOMOUT || keycode == GK_ZOOMIN) {
		if (state == GAMEST_SINGLE && mPaddle0) mPaddle0->tilt(0);
		else if (state == GAMEST_MULTI && dev == 0 && mPaddle1) mPaddle1->tilt(0);
		else if (state == GAMEST_MULTI && dev == 1 && mPaddle2) mPaddle2->tilt(0);
	}
}

void Game::gyroMoved(int dev, double x, double y, double raw_x, double raw_y) {
	if (state == GAMEST_SINGLE) {
		mPaddle0->gyroMovement(x);
	} else if (state == GAMEST_MULTI) {
		if (dev == 0) mPaddle1->gyroMovement(x);
		if (dev == 1) mPaddle2->gyroMovement(x);
	}
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
