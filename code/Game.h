#ifndef __Game_h_
#define __Game_h_

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
#include <set>
#include <ctime>
#include <cstdint>

class GameObject;

class Game : public BaseApplication
{
	friend class GameObject;

	public:
	std::set<GameObject*> entities;
	Game(bool server);
	virtual ~Game(void);
	void collission(GameObject *o0, GameObject *o1);

	protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);
	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);
	void createFrameListener(void);

	bool soundOn, server;
	OgreBulletDynamics::DynamicsWorld *mWorld;
	Paddle *mPaddle;
	Ball *oBall;
	OgreOggSound::OgreOggSoundManager* mSndMgr;
	OgreOggSound::OgreOggSoundPlugin *mOggSoundPlugin;
	OgreBites::ParamsPanel* mScorePanel;
	int score;
	void reset();
	Network net;
	int remPaddlePos;

	private:
	uint64_t lastHit, gameStart, lastSend;
};

#endif // #ifndef __Game_h_
