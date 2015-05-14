#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Ball.h"

Ball::Ball(Game *game) : GameObject(game, K::BALL){
	name = "Ball@" + Ogre::StringConverter::toString(id);
	Ogre::Entity* entity = scnMgr->createEntity("et"+name, "ogrehead.mesh");
	entity->setCastShadows(true);
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("nd"+name);
	rootNode->attachObject(entity);
	rootNode->scale(0.5f,0.5f,0.5f);
	
	bRadius = 40.0f;

	int random = rand()%3;
	switch(random){
		case 0: bDirection = Ogre::Vector3(-1.0f, 1.0f, 0.0f);break;
		case 1: bDirection = Ogre::Vector3(1.0f, 1.0f, 0.0f); break;
		case 2: bDirection = Ogre::Vector3(0.0f, 1.0f, 0.0f); break;
		case 3: bDirection = Ogre::Vector3(1.0f, 0.0f, 0.0f); break;
	}
	bDirection = Ogre::Vector3(1.0f, 2.0f, 0.0f);



	bDirection.normalise();
	bSpeed = 300.0f;

	collShape = new OgreBulletCollisions::SphereCollisionShape(bRadius * 0.45);
	rigidBody = new OgreBulletDynamics::RigidBody("bt"+name, mWorld);
	rigidBody->setShape(rootNode, collShape,
			1.0, 0.5, 1.0, // restitution, friction, mass
			Ogre::Vector3(0, 0, 0), Ogre::Quaternion());
	//rigidBody->setLinearVelocity(bDirection * bSpeed);

	btCollisionObject *btObj = rigidBody->getBulletObject();
	btObj->setCollisionFlags(btObj->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	btObj->setUserPointer(static_cast<GameObject*>(this));
}

void Ball::update(const Ogre::FrameEvent& evt){
	/*
	Ogre::Vector3 bPosition = rootNode->getPosition();
	if (bPosition.y < -1000/2.0f + bRadius && bDirection.y < 0.0f) bDirection.y = -bDirection.y;
	if (bPosition.y > 1000/2.0f - bRadius && bDirection.y > 0.0f) bDirection.y = -bDirection.y;
	if (bPosition.z < -100/2.0f + bRadius && bDirection.z < 0.0f) bDirection.z = -bDirection.z;
	if (bPosition.z > 100/2.0f - bRadius && bDirection.z > 0.0f) bDirection.z = -bDirection.z;
	if (bPosition.x < -775/2.0f + bRadius && bDirection.x < 0.0f) bDirection.x = -bDirection.x;
	if (bPosition.x > 775/2.0f - bRadius && bDirection.x > 0.0f) bDirection.x = -bDirection.x;
	rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection);
	*/
}
