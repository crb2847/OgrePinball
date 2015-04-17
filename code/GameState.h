#ifndef __GameState_h_
#define __GameState_h_

#include "BaseApplication.h"
#include "OgreBulletDynamicsWorld.h"
#include "OgreOggSound.h"
#include "OgreOggSoundPlugin.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"
#include "Coin.h"
#include "PitPlane.h"
#include "Obstacle.h"
#include "Network.h"

class GameState{
	public:
		GameState(){};
		~GameState(){};
		virtual void createState(){};
		virtual void frameStartedState(const Ogre::FrameEvent& evt){};
		virtual void frameRenderState(const Ogre::FrameEvent& evt){};
		virtual void frameEnded(const Ogre::FrameEvent& evt){};
		virtual void keyPressedState(const OIS::KeyEvent& evt){};
		virtual void keyReleasedState(const OIS::KeyEvent& evt){};
		virtual void reset(){};
};

#endif // #ifndef __GameState_h_