#ifndef __Obstacle_h_
#define __Obstacle_h_

#include "GameObject.h"

class Obstacle : public virtual GameObject
{
	public:
		Obstacle(Game *game, Ogre::Vector3 position);
		void update(const Ogre::FrameEvent& evt);
};

#endif // #ifndef __Obstacle_h_
