#include "SinglePlayerState.h"

SinglePlayerState::SinglePlayerState(Game* game) : GameState(){
	g = game;
}
SinglePlayerState::~SinglePlayerState(){}

void SinglePlayerState::createState(){
	g->mPaddle1 = new Paddle(g,1);
	g->entities.insert(g->mPaddle1);

    g->oBall = new Ball(g);
    g->entities.insert(g->oBall);

	//Create 6 walls
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1000, 1000, 20, 20, true, 1, 5.0, 5.0, Ogre::Vector3::UNIT_X);

    PitPlane* pit = new PitPlane(g, Ogre::Vector3::UNIT_Y, Ogre::Real(-510));
    g->entities.insert(pit); // Bottom

    Wall *p = new Wall(g, Ogre::Vector3(0.75f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Z), Ogre::Vector3(0, 500, 0));
    g->entities.insert(p); // Top

    p = new Wall(g, Ogre::Vector3(0.75f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, -100));
    g->entities.insert(p); // Back

    p = new Wall(g, Ogre::Vector3(0.75f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, 100));
    g->entities.insert(p); // Front
    
    p = new Wall(g, Ogre::Vector3(1.0f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_Z),  Ogre::Vector3(-375, 0, 0));
    g->entities.insert(p); // Left

    p = new Wall(g, Ogre::Vector3(1.0f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z), Ogre::Vector3(375, 0, 0));
    g->entities.insert(p); // Right

    g->maxScore = 13;

    std::vector<Ogre::Vector3> coinPos { Ogre::Vector3(100,100,0), Ogre::Vector3(-100,400,0), Ogre::Vector3(-100,150,0), Ogre::Vector3(-50,200,0), Ogre::Vector3(-250,300,0), Ogre::Vector3(280,400,0), Ogre::Vector3(-250,0,0), Ogre::Vector3(-300,100,0), Ogre::Vector3(-300,-400,0), Ogre::Vector3(-100,-300,0), Ogre::Vector3(0,-350,0), Ogre::Vector3(100,-270,0), Ogre::Vector3(200,-340,0)};
    for (Ogre::Vector3 p : coinPos)
    	g->entities.insert(new Coin(g, p));

    std::vector<Ogre::Vector3> obstaclePos { Ogre::Vector3(-350,-250,0), Ogre::Vector3(355,480,0),
    	Ogre::Vector3(-100,50,0), Ogre::Vector3(100,-150,0) };
    for (Ogre::Vector3 p : obstaclePos)
    	g->entities.insert(new Obstacle(g, p));
}

void SinglePlayerState::frameStarted(const Ogre::FrameEvent& evt){
	g->mWorld->stepSimulation(evt.timeSinceLastFrame);
}

void SinglePlayerState::frameRender(const Ogre::FrameEvent& evt){
	g->elapsedSec = (clock()-g->gameStart)/CLOCKS_PER_SEC;
	for(GameObject *obj : g->entities) {
		obj->update(evt);
	}
}

void SinglePlayerState::frameEnded(const Ogre::FrameEvent& evt){
	g->mWorld->stepSimulation(evt.timeSinceLastFrame);
}

void SinglePlayerState::keyPressed(const OIS::KeyEvent& evt){
	if (evt.key == OIS::KC_LEFT) { g->mPaddle1->motion |= 1;}
	else if (evt.key == OIS::KC_RIGHT) { g->mPaddle1->motion |= 2;}
}

void SinglePlayerState::keyReleased(const OIS::KeyEvent& evt){
	if (evt.key == OIS::KC_LEFT) { g->mPaddle1->motion &= ~1;}
	else if (evt.key == OIS::KC_RIGHT) { g->mPaddle1->motion &= ~2;}
}

void SinglePlayerState::gyroMoved(int dev, double x, double y, double raw_x, double raw_y) {}
void SinglePlayerState::gyroKeyPressed(int dev, int keycode) {}
void SinglePlayerState::gyroKeyReleased(int dev, int keycode) {}

void SinglePlayerState::reset() {}
