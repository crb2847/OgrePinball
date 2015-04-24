#include "SinglePlayerState.h"

SinglePlayerState::SinglePlayerState(Game* g) : GameState(){

}
SinglePlayerState::~SinglePlayerState(){}

void SinglePlayerState::createState(){}

void SinglePlayerState::frameStarted(const Ogre::FrameEvent& evt){}

void SinglePlayerState::frameRender(const Ogre::FrameEvent& evt){}

void SinglePlayerState::frameEnded(const Ogre::FrameEvent& evt){}

void SinglePlayerState::keyPressed(const OIS::KeyEvent& evt){
	//if (evt.key == OIS::KC_LEFT) { mPaddle->motion |= 1;}
	//else if (evt.key == OIS::KC_RIGHT) { mPaddle->motion |= 2;}
}

void SinglePlayerState::keyReleased(const OIS::KeyEvent& evt){
	//if (evt.key == OIS::KC_LEFT) { mPaddle->motion &= ~1;}
	//else if (evt.key == OIS::KC_RIGHT) { mPaddle->motion &= ~2;}
}

void SinglePlayerState::gyroMoved(int dev, double x, double y, double raw_x, double raw_y) {}
void SinglePlayerState::gyroKeyPressed(int dev, int keycode) {}
void SinglePlayerState::gyroKeyReleased(int dev, int keycode) {}

void SinglePlayerState::reset() {}
