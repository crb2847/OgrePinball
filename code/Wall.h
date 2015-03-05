#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"
#include <string>

class Wall : public virtual GameObject
{
	public:
		Wall(Game *game, int, Ogre::Vector3 scale = Ogre::Vector3(1,1,1),
				Ogre::Quaternion orientation = Ogre::Quaternion(1,0,0,0), Ogre::Vector3 position = Ogre::Vector3(0,0,0));
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
		void attachObject(Ogre::SceneManager* scnMgr);
	protected:
		std::string myName;

};

#endif // #ifndef __Wall_h_
