#ifndef __Ball_h_
#define __Ball_h_


#include <Ogre.h>

class Ball {
	protected:
	 Ogre::SceneNode* rootNode;
	 Ogre::Real bRadius;
	 Ogre::Vector3 bDirection;
	 Ogre::Real bSpeed;
	public:
	 Ball(Ogre::SceneManager* scnMgr);
	 Ball(Ogre::SceneManager* scnMgr, int);
	 void setBDirection(Ogre::Vector3);
	 //~Ball();
	 void move(const Ogre::FrameEvent& evt);
	 Ogre::SceneNode* getNode() { return rootNode; }
};

#endif // #ifndef __Ball_h_