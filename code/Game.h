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
#include "GyroInput.h"
#include "GameState.h"
#include "SinglePlayerState.h"
#include <set>
#include <ctime>
#include <cstdint>
#include "SdkTrays.h"

class GameObject;

enum GameState_t {GAMEST_MENU, GAMEST_CONNECT, GAMEST_SINGLE, GAMEST_MULTI};


class Game : public BaseApplication, public GyroListener
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
	void createFrameListener(void);
	void gyroMoved(int dev, double x, double y, double raw_x, double raw_y);

	int state;
	bool soundOn;
	OgreBulletDynamics::DynamicsWorld *mWorld;
	Paddle *mPaddle1;
	Paddle *mPaddle2;
	Paddle *mMyPaddle;
	Ball *oBall;
	GyroInput *mGyroInput;
	OgreOggSound::OgreOggSoundManager* mSndMgr;
	OgreOggSound::OgreOggSoundPlugin *mOggSoundPlugin;
	OgreBites::ParamsPanel* mScorePanel;
	int score, elapsedSec;
	OgreBites::TextBox* mControlPanel;
	void reset();
	Network net;
	int remPaddlePos;
	unsigned char sounds[3];
	int maxScore;
	std::vector<GameState*> gamestates;
	private:
	uint64_t lastHit, gameStart, lastSend;
};

#endif // #ifndef __Game_h_
