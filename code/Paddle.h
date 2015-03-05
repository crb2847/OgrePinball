#ifndef __Paddle_h_
#define __Paddle_h_

#include "GameObject.h"

class Paddle : public virtual GameObject
{
	public:
		Paddle(Game *game, int);
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
};

#endif // #ifndef __Paddle_h_
