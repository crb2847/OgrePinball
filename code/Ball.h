#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"

class Ball : public virtual GameObject
{
	public:
		Ball(Game *game);
		void update(const Ogre::FrameEvent& evt);
};

#endif // #ifndef __Ball_h_
