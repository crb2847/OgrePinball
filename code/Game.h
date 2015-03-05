#ifndef __Game_h_
#define __Game_h_

#include "BaseApplication.h"
#include "GameObject.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"
#include "Coin.h"

class Game : public BaseApplication
{
	public:
	std::vector<GameObject*> entities;
	Game(void);
	virtual ~Game(void);

	protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	// virtual bool keyPressed( const OIS::KeyEvent& evt );
 //    virtual bool keyReleased( const OIS::KeyEvent& evt );
};

#endif // #ifndef __Game_h_