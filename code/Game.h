#ifndef __Game_h_
#define __Game_h_

#include "BaseApplication.h"

class Game : public BaseApplication
{
	public::vector<Entity> entities;
	Game(void);
	virtual ~Game(void);

	protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Orge::FrameEvent& evt);
};

#endif // #ifndef __Game_h_