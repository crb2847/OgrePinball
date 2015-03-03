#ifndef __Game_h_
#define __Game_h_

#include "BaseApplication.h"
#include "Entity.h"

class Game : public BaseApplication
{
	public:
	std::vector<Entity> entities;
	Game(void);
	virtual ~Game(void);
	::Entity* ball;
	std::vector<Ogre::SceneNode *> coins;

	protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool keyPressed( const OIS::KeyEvent& evt );
	virtual bool keyReleased( const OIS::KeyEvent& evt );
};

#endif // #ifndef __Game_h_