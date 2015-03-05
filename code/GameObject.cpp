#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "GameObject.h"
#include "Game.h"

GameObject::GameObject(Game *game, int x) {
	this->game = game;
	mWorld = game->mWorld;
	scnMgr = game->mSceneMgr;

	bRadius = 40.0f;
	bDirection = Ogre::Vector3(1.0f, 0.0f, 0.0f);
	bDirection.normalise();
	bSpeed = 250.0f;
}

/*
Only currently handles collision for walls and the ball at the moment.
*/
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

/*
Will handle collision detection for all objects in the future
*/
void GameObject::update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e){
	
}

void GameObject::setBDirection(Ogre::Vector3 x){
	bDirection = x;
}
