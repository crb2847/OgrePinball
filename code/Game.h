#ifndef __Game_h_
#define __Game_h_

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
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
#include "GyroInput.h"
#include "GameState.h"
#include "SinglePlayerState.h"
#include <set>
#include <ctime>
#include <cstdint>
#include <map>

class GameObject;

enum GameState_t {GAMEST_MENU, GAMEST_CONNECT, GAMEST_SINGLE, GAMEST_MULTI};


class Game : public BaseApplication, public GyroListener
{
	friend class GameObject;

	public:
	std::set<GameObject*> entities;
	OgreBulletDynamics::DynamicsWorld *mWorld;
	int maxScore;
	Paddle *mPaddle1;
	Paddle *mPaddle2;
	Ball *oBall;
	uint64_t lastHit, gameStart, lastSend;
	int score, elapsedSec;
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
	void gyroKeyPressed(int dev, int keycode);
	void gyroKeyReleased(int dev, int keycode);

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
    bool openSettingsMenu(const CEGUI::EventArgs &e);
    bool goBack(const CEGUI::EventArgs &e);
    bool openHowToPlayMenu(const CEGUI::EventArgs &e);
    bool goBackHTP(const CEGUI::EventArgs &e);

	int state;
	bool soundOn;
	Paddle *mMyPaddle;
	GyroInput *mGyroInput;
	OgreOggSound::OgreOggSoundManager* mSndMgr;
	OgreOggSound::OgreOggSoundPlugin *mOggSoundPlugin;
	void reset();
	int remPaddlePos;
	unsigned char sounds[3];
	std::map<int, GameState*> gamestates;

	//CEGUI globals
	CEGUI::OgreRenderer* mRenderer;
    CEGUI::Window *sheet;
    CEGUI::Window *menu;
    CEGUI::Window *scoreBox;
    CEGUI::Window *pause;
    CEGUI::Window *pauseMenu;
    CEGUI::Window *settingsMenu;
    CEGUI::Window *sound;
    CEGUI::Window *howToPlayMenu;
    CEGUI::Window *soundPause;
};

#endif // #ifndef __Game_h_
