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

	protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // #ifndef __Game_h_