#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Wall.h"
#include <iostream>

Wall::Wall(Game *game, Ogre::Vector3 scale,
		Ogre::Quaternion orientation, Ogre::Vector3 position) : GameObject(game, K::WALL){

	name = "Wall@" + Ogre::StringConverter::toString(id);
	Ogre::Entity* entGround = scnMgr->createEntity("et"+name, "ground");
    entGround->setMaterialName("Wall");
    entGround->setCastShadows(false);
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("nd"+name);
	rootNode->attachObject(entGround);

	rootNode->scale(scale);
	rootNode->rotate(orientation);
	rootNode->setPosition(position);

	Ogre::Vector3 normal = orientation * Ogre::Vector3(0.0f, 1.0f, 0.0f);
	Ogre::Real distance = normal.dotProduct(position);

	collShape = new OgreBulletCollisions::StaticPlaneCollisionShape(normal, distance);
	rigidBody = new OgreBulletDynamics::RigidBody("bt"+name, mWorld);
	rigidBody->setStaticShape(collShape, 0.8, 0.2); // .., restitution, friction

	btCollisionObject *btObj = rigidBody->getBulletObject();
	btObj->setCollisionFlags(btObj->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	btObj->setUserPointer(static_cast<GameObject*>(this));

	bDirection = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	bDirection.normalise();
	bRadius = 00.0f;
	bSpeed = 0.0f;
}

void Wall::update(const Ogre::FrameEvent& evt){
	 //Ogre::Vector3 bPosition = rootNode->getPosition();
	 
	// for(int x=0; x<e.size(); x++){

	// }

	 //rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
}

void Wall::attachObject(Ogre::SceneManager* scnMgr){
	//myWall = scnMgr->getRootSceneNode()->createChildSceneNode(myName);
	//myWall->attachObject(myself);
}

