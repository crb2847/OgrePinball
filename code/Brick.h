#ifndef __Brick_h_
#define __Brick_h_

#include "GameObject.h"

class Brick : public virtual GameObject
{
	public:
	    Brick(Game *game, Ogre::Vector3 position, int points, bool hasCoin);
		void update(const Ogre::FrameEvent& evt);
		int points; bool hasCoin;
	private:
		const static float colors[][3];
};

#endif
