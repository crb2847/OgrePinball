#ifndef __Coin_h_
#define __Coin_h_

#include "GameObject.h"

class Coin : public virtual GameObject
{
	public:
		Coin(Game *game, int);
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
};

#endif // #ifndef __Coin_h_
