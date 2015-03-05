#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"

class Ball : public virtual GameObject
{
	public:
		Ball(Game *game, int);
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
};

#endif // #ifndef __Ball_h_
