#ifndef __SinglePlayerState_h_
#define __SinglePlayerState_h_

#include "Game.h"

class SinglePlayerState : public GameState   {
	public:
	SinglePlayerState(Game* p);
	~SinglePlayerState();
	void createState();
	void frameStarted(const Ogre::FrameEvent& evt) ;
	void frameRender(const Ogre::FrameEvent& evt) ;
	void frameEnded(const Ogre::FrameEvent& evt) ;
	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);
	void gyroMoved(int dev, double x, double y, double raw_x, double raw_y) ;
	void gyroKeyPressed(int dev, int keycode) ;
	void gyroKeyReleased(int dev, int keycode) ;
	void reset();

	private:
	Game* g;
};

#endif // #ifndef __SinglePlayerState_h_
