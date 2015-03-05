#ifndef __Coin_h_
#define __Coin_h_

#include "GameObject.h"

class Coin : public virtual GameObject
{
	public:
		Coin(Ogre::SceneManager* scnMgr, int);
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
};

#endif // #ifndef __Coin_h_
