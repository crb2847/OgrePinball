#ifndef __Paddle_h_
#define __Paddle_h_

#include "GameObject.h"

class Paddle : public virtual GameObject
{
	public:
		Paddle(Game *game, int pID);
		void gyroMovement(double d);
		void setPosition(Ogre::Vector3 pos);
		void update(const Ogre::FrameEvent& evt);
		void tilt(double a);
		int motion = 0;
		int playerID;
};

#endif // #ifndef __Paddle_h_
