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
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

class GameObject;

enum GameState_t {GAMEST_MENU, GAMEST_CONNECT, GAMEST_SINGLE, GAMEST_CLIENT, GAMEST_SERVER};


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
	void createFrameListener(void);

	CEGUI::OgreRenderer* mRenderer;
	int state;
	bool soundOn;
	OgreBulletDynamics::DynamicsWorld *mWorld;
	Paddle *mPaddle1;
	Paddle *mPaddle2;
	Paddle *mMyPaddle;
	Ball *oBall;
	OgreOggSound::OgreOggSoundManager* mSndMgr;
	OgreOggSound::OgreOggSoundPlugin *mOggSoundPlugin;
	int score, elapsedSec;
	void reset();
	Network net;
	int remPaddlePos;
	unsigned char sounds[3];

	//CEGUI globals
    CEGUI::Window *sheet;
    CEGUI::Window *menu;

	private:
	uint64_t lastHit, gameStart, lastSend;
};

#endif // #ifndef __Game_h_
