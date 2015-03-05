#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"
#include <string>

class Wall : public virtual GameObject
{
	public:
		Wall(Game *game, Ogre::Vector3 scale, Ogre::Quaternion orientation, Ogre::Vector3 position);
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
		void attachObject(Ogre::SceneManager* scnMgr);
	protected:
		std::string myName;

};
#endif // #ifndef __Wall_h_
