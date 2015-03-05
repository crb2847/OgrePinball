#ifndef __Game_h_
#define __Game_h_

#include "BaseApplication.h"
#include "OgreBulletDynamicsWorld.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"
#include "Coin.h"
#include "PitPlane.h"

class GameObject;

class Game : public BaseApplication
{
	friend class GameObject;

	public:
	std::vector<GameObject*> entities;
	Game(void);
	virtual ~Game(void);
	void collide(btCollisionObject *b0, btCollisionObject *b1);

	protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);
    OgreBulletDynamics::DynamicsWorld *mWorld;
	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);

	Paddle *mPaddle;
};

#endif // #ifndef __Game_h_
