#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Entity.h"

Entity::Entity(Ogre::SceneManager* scnMgr, int x){
	Ogre::Entity* entity = scnMgr->createEntity("Sphere"+x, "sphere.mesh");
	 //ball->setMaterialName("BallColor/CubeMap");
	 entity->setCastShadows(true);
	 rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	 rootNode->attachObject(entity);
	 rootNode->scale(0.4f,0.4f,0.4f);
	 bRadius = 40.0f;
	 bDirection = Ogre::Vector3(1.0f, 2.0f, 3.0f);
	 bDirection.normalise();
	 bSpeed = 250.0f;
}

void Entity::move(const Ogre::FrameEvent& evt) {
	 Ogre::Vector3 bPosition = rootNode->getPosition();
	 if (bPosition.y < -1400/2.0f + bRadius && bDirection.y < 0.0f) bDirection.y = -bDirection.y;
	 if (bPosition.y > 1400/2.0f - bRadius && bDirection.y > 0.0f) bDirection.y = -bDirection.y;
	 if (bPosition.z < -1400/2.0f + bRadius && bDirection.z < 0.0f) bDirection.z = -bDirection.z;
	 if (bPosition.z > 1400/2.0f - bRadius && bDirection.z > 0.0f) bDirection.z = -bDirection.z;
	 if (bPosition.x < -1400/2.0f + bRadius && bDirection.x < 0.0f) bDirection.x = -bDirection.x;
	 if (bPosition.x > 1400/2.0f - bRadius && bDirection.x > 0.0f) bDirection.x = -bDirection.x;
	 rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
}

void Entity::setBDirection(Ogre::Vector3 x){
	bDirection = x;
}