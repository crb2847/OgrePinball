#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Wall.h"

Wall::Wall(Ogre::SceneManager* scnMgr, int x) : GameObject(scnMgr, x){
	
	Ogre::Entity* entGround = scnMgr->createEntity("GroundEntity"+x, "ground");
    entGround->setMaterialName("Examples/Rockwall");
    entGround->setCastShadows(false);
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	rootNode->attachObject(entGround);
    //scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
    myName = "GroundEntity"+x;

	bDirection = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	bDirection.normalise();
	bRadius = 00.0f;
	bSpeed = 0.0f;
}

void Wall::update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e){
	 //Ogre::Vector3 bPosition = rootNode->getPosition();
	 
	// for(int x=0; x<e.size(); x++){

	// }

	 //rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
}

void Wall::rotate(Ogre::SceneManager* scnMgr, Ogre::Vector3 dir, Ogre::Radian deg){
    rootNode->rotate(dir, deg);
}

void Wall::attachObject(Ogre::SceneManager* scnMgr){
	//myWall = scnMgr->getRootSceneNode()->createChildSceneNode(myName);
	//myWall->attachObject(myself);
}
