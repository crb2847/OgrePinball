#ifndef __Coin_h_
#define __Coin_h_

#include "GameObject.h"

class Coin : public virtual GameObject
{
	public:
		Coin(Game *game, Ogre::Vector3 position);
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
	protected:
		Ogre::Degree angle;
};

#endif // #ifndef __Coin_h_
