#ifndef __Paddle_h_
#define __Paddle_h_

#include "GameObject.h"

class Paddle : public virtual GameObject
{
	public:
		Paddle(Ogre::SceneManager* scnMgr, int);
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
};

#endif // #ifndef __Paddle_h_
