#ifndef __Paddle_h_
#define __Paddle_h_

#include "GameObject.h"

class Paddle : public virtual GameObject
{
	public:
		Paddle(Game *game);
		void setPosition(Ogre::Vector3 pos);
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
		int motion = 0;
};

#endif // #ifndef __Paddle_h_
