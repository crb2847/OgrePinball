#ifndef CODE_PITPLANE_H_
#define CODE_PITPLANE_H_

#include "GameObject.h"

class PitPlane : public virtual GameObject {
	public:
	PitPlane(Game *game, Ogre::Vector3 normal, Ogre::Real distance);
	void update(const Ogre::FrameEvent& evt);
};

#endif /* CODE_PITPLANE_H_ */
