#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "GameObject.h"
#include "Game.h"

int GameObject::cur_id = 0;

GameObject::GameObject(Game *game, int kind) {
	GameObject::game = game;
	mWorld = game->mWorld;
	scnMgr = game->mSceneMgr;
	id = cur_id++;
	GameObject::kind = kind;

	bRadius = 40.0f;
	bDirection = Ogre::Vector3(1.0f, 0.0f, 0.0f);
	bDirection.normalise();
	bSpeed = 250.0f;

	rootNode = nullptr;
	rigidBody = nullptr;
	collShape = nullptr;
}

GameObject::~GameObject() {
	if (rigidBody) delete rigidBody;
	if (collShape) delete collShape;
	if (rootNode) {
		rootNode->detachAllObjects();
		delete rootNode;
	}
}

void GameObject::move(const Ogre::FrameEvent& evt) { }
void GameObject::update(const Ogre::FrameEvent& evt) { }

void GameObject::setBDirection(Ogre::Vector3 x){
	bDirection = x;
}

void GameObject::setPosition(Ogre::Vector3 pos) {
	rootNode->setPosition(pos);
	btTransform trans = rigidBody->getBulletObject()->getWorldTransform();
	trans.setOrigin(OgreBulletCollisions::OgreBtConverter::to(pos));
	rigidBody->getBulletObject()->setWorldTransform(trans);
}

void GameObject::setRotation(Ogre::Quaternion orientation) {
	rootNode->setOrientation(orientation);
	btTransform trans = rigidBody->getBulletObject()->getWorldTransform();
	trans.setRotation(OgreBulletCollisions::OgreBtConverter::to(orientation));
	rigidBody->getBulletObject()->setWorldTransform(trans);
}
