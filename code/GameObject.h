#ifndef __Entity_h_
#define __Entity_h_

class GameObject
{
	public: 
	 GameObject(Ogre::SceneManager* scnMgr, int);
	 void setBDirection(Ogre::Vector3);
	 void move(const Ogre::FrameEvent& evt);
	 virtual void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
	 Ogre::SceneNode* getNode() { return rootNode; }
	 
	protected: 
		Ogre::SceneNode* rootNode;
		Ogre::Real bRadius;
		Ogre::Vector3 bDirection;
		Ogre::Real bSpeed;
};

#endif // #ifndef __Entity_h_

