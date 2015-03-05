#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Coin.h"

Coin::Coin(Ogre::SceneManager* scnMgr, int x) : GameObject(scnMgr, x){
	
	Ogre::Entity* entity = scnMgr->createEntity("Coin"+x, "sphere.mesh");
	//ball->setMaterialName("BallColor/CubeMap");
	entity->setCastShadows(true);
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	rootNode->attachObject(entity);
	rootNode->scale(0.4f,0.4f,0.4f);
	/*
	bRadius = 40.0f;
	bDirection = Ogre::Vector3(1.0f, 2.0f, 3.0f);
	bDirection.normalise();
	bSpeed = 250.0f;
	*/
}

void Coin::update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e){
	 Ogre::Vector3 bPosition = rootNode->getPosition();
	 
	// for(int x=0; x<e.size(); x++){

	// }

	 rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
}
