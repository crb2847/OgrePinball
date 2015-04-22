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
	void createFrameListener(void);

	// OIS::KeyListener
	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);

	// OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    //methods for button events
    bool startSinglePlayer(const CEGUI::EventArgs &e);
    bool quit(const CEGUI::EventArgs &e);
    bool pauseGame(const CEGUI::EventArgs &e);
    bool resumeGame(const CEGUI::EventArgs &e);
    bool openMainMenu(const CEGUI::EventArgs &e);

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
	CEGUI::OgreRenderer* mRenderer;
    CEGUI::Window *sheet;
    CEGUI::Window *menu;
    CEGUI::Window *scoreBox;
    CEGUI::Window *pause;
    CEGUI::Window *pauseMenu;

	private:
	uint64_t lastHit, gameStart, lastSend;
};

#endif // #ifndef __Game_h_
