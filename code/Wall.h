#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"

class Wall : public virtual GameObject
{
	public:
		Wall(Ogre::SceneManager* scnMgr, int);
		void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
		void rotate(Ogre::SceneManager* scnMgr, Ogre::Vector3, Ogre::Radian);
		void attachObject(Ogre::SceneManager* scnMgr);
	protected:
		std::string myName;
};

#endif // #ifndef __Wall_h_
