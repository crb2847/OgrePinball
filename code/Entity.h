#ifndef __Entity_h_
#define __Entity_h_

class Entity
{
	public: 
	 Entity(Ogre::SceneManager* scnMgr, int);
	 void setBDirection(Ogre::Vector3);
	 void move(const Ogre::FrameEvent& evt);
	 Ogre::SceneNode* getNode() { return rootNode; }
	protected: 
		Ogre::SceneNode* rootNode;
		Ogre::Real bRadius;
		Ogre::Vector3 bDirection;
		Ogre::Real bSpeed;
};

#endif // #ifndef __Ball_h_

