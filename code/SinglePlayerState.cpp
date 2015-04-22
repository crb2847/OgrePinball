#include "SinglePlayerState.h"

SinglePlayerState::SinglePlayerState(Paddle* p) : GameState(){
	mPaddle = p;
}
SinglePlayerState::~SinglePlayerState(){}

void SinglePlayerState::createState(){}

void SinglePlayerState::frameStartedState(const Ogre::FrameEvent& evt){}

void SinglePlayerState::frameRenderState(const Ogre::FrameEvent& evt){}

void SinglePlayerState::frameEnded(const Ogre::FrameEvent& evt){}

void SinglePlayerState::keyPressedState(const OIS::KeyEvent& evt){
	if (evt.key == OIS::KC_LEFT) { mPaddle->motion |= 1;}
	else if (evt.key == OIS::KC_RIGHT) { mPaddle->motion |= 2;}
}

void SinglePlayerState::keyReleasedState(const OIS::KeyEvent& evt){
	if (evt.key == OIS::KC_LEFT) { mPaddle->motion &= ~1;}
	else if (evt.key == OIS::KC_RIGHT) { mPaddle->motion &= ~2;}
}