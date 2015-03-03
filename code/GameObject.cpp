#include "GameObject.h"

#include <OgreEntity.h>
#include <OgreSceneManager.h>

GameObject::GameObject(Ogre::SceneManager* scnMgr, int x){
	Ogre::Entity* entity = scnMgr->createEntity("Sphere"+x, "ogrehead.mesh");
	 //ball->setMaterialName("BallColor/CubeMap");
	 entity->setCastShadows(true);
	 rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	 rootNode->attachObject(entity);
	 rootNode->scale(1.0f,1.0f,1.0f);
	 bRadius = 40.0f;
	 bDirection = Ogre::Vector3(1.0f, x, 0.0f);
	 bDirection.normalise();
	 bSpeed = 500-150.0f*x;
}

void GameObject::move(const Ogre::FrameEvent& evt) {
	 Ogre::Vector3 bPosition = rootNode->getPosition();
	 if (bPosition.y < -1000/2.0f + bRadius && bDirection.y < 0.0f) bDirection.y = -bDirection.y;
	 if (bPosition.y > 1000/2.0f - bRadius && bDirection.y > 0.0f) bDirection.y = -bDirection.y;
	 if (bPosition.z < -100/2.0f + bRadius && bDirection.z < 0.0f) bDirection.z = -bDirection.z;
	 if (bPosition.z > 100/2.0f - bRadius && bDirection.z > 0.0f) bDirection.z = -bDirection.z;
	 if (bPosition.x < -775/2.0f + bRadius && bDirection.x < 0.0f) bDirection.x = -bDirection.x;
	 if (bPosition.x > 775/2.0f - bRadius && bDirection.x > 0.0f) bDirection.x = -bDirection.x;
	 rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
}

void GameObject::setBDirection(Ogre::Vector3 x){
	bDirection = x;
}
