#ifndef __Game_h_
#define __Game_h_

#include "BaseApplication.h"
#include "OgreBulletDynamicsWorld.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"
#include "Coin.h"
#include "PitPlane.h"
#include <set>

class GameObject;

class Game : public BaseApplication
{
	friend class GameObject;

	public:
	std::set<GameObject*> entities;
	Game(void);
	virtual ~Game(void);
	void collission(GameObject *o0, GameObject *o1);

	protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);
	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);

	OgreBulletDynamics::DynamicsWorld *mWorld;
	Paddle *mPaddle;
};

#endif // #ifndef __Game_h_
