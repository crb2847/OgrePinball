#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Paddle.h"

Paddle::Paddle(Ogre::SceneManager* scnMgr, int x) : GameObject(scnMgr, x){
	
	Ogre::Entity* entity = scnMgr->createEntity("Ogre"+x, "ogrehead.mesh");
	entity->setCastShadows(true);
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	rootNode->attachObject(entity);
	rootNode->scale(0.4f,0.4f,0.4f);

	// Ogre::Vector3 bPosition = rootNode->getPosition();
	// bPosition.y = 0;
	// bPosition.x = 0;
	// bPosition.z = -650.0f;

	bRadius = 40.0f;
	bDirection = Ogre::Vector3(-1.0f, 0.0f, 0.0f);
	bDirection.normalise();
	bSpeed = 250.0f;
}

void Paddle::update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e){
	Ogre::Vector3 bPosition = rootNode->getPosition();
	if (bPosition.y < -1400/2.0f + bRadius && bDirection.y < 0.0f) bDirection.y = -bDirection.y;
	if (bPosition.y > 1400/2.0f - bRadius && bDirection.y > 0.0f) bDirection.y = -bDirection.y;
	if (bPosition.z < -1400/2.0f + bRadius && bDirection.z < 0.0f) bDirection.z = -bDirection.z;
	if (bPosition.z > 1400/2.0f - bRadius && bDirection.z > 0.0f) bDirection.z = -bDirection.z;
	if (bPosition.x < -1400/2.0f + bRadius && bDirection.x < 0.0f) bDirection.x = -bDirection.x;
	if (bPosition.x > 1400/2.0f - bRadius && bDirection.x > 0.0f) bDirection.x = -bDirection.x;
	rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
}
