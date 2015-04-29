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
		virtual void frameStarted(const Ogre::FrameEvent& evt){};
		virtual void frameRender(const Ogre::FrameEvent& evt){};
		virtual void frameEnded(const Ogre::FrameEvent& evt){};
		virtual bool keyPressed(const OIS::KeyEvent& evt){};
		virtual bool keyReleased(const OIS::KeyEvent& evt){};
		virtual void gyroMoved(int dev, double x, double y, double raw_x, double raw_y) {}
		virtual void gyroKeyPressed(int dev, int keycode) {}
		virtual void gyroKeyReleased(int dev, int keycode) {}
		virtual void reset(){};
};

#endif // #ifndef __GameState_h_
