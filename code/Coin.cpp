#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Coin.h"

Coin::Coin(Game *game, Ogre::Vector3 position) : GameObject(game, K::COIN){
	name = "Coin@" + Ogre::StringConverter::toString(id);
	Ogre::Entity* entity = scnMgr->createEntity("et"+name, "disc.mesh");
	entity->setCastShadows(true);
	entity->setMaterialName("Coin");
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("nd"+name);
	rootNode->attachObject(entity);

	rootNode->setScale(scale = Ogre::Vector3(80, 80, 80));
	//rootNode->setOrientation(orientation = Ogre::Quaternion(angle, Ogre::Vector3(1,1,1)));
	rootNode->setPosition(Coin::position = position);

	Ogre::Vector3 bSize = entity->getBoundingBox().getHalfSize() * 0.95 * scale;
	collShape = new OgreBulletCollisions::BoxCollisionShape(bSize);
	rigidBody = new OgreBulletDynamics::RigidBody("bt"+name, mWorld);
	rigidBody->setStaticShape(rootNode, collShape,
		1, 0.1, // restitution, friction
		position, Ogre::Quaternion());

	btCollisionObject *btObj = rigidBody->getBulletObject();
	btObj->setCollisionFlags(btObj->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK |
			btCollisionObject::CF_NO_CONTACT_RESPONSE);
	btObj->setUserPointer(static_cast<GameObject*>(this));

	bDirection = Ogre::Vector3(1,1,1);
	bDirection.normalise();
	angle = Ogre::Degree(25);
	bSpeed = 250.0f;
	taken = false;
}

void Coin::update(const Ogre::FrameEvent& evt){
	angle += Ogre::Degree(bSpeed * evt.timeSinceLastFrame);
	orientation = Ogre::Quaternion(angle, Ogre::Vector3(1,1,1));

	rootNode->setOrientation(orientation);
	btTransform trans = rigidBody->getBulletObject()->getWorldTransform();
	trans.setRotation(OgreBulletCollisions::OgreBtConverter::to(orientation));
	rigidBody->getBulletObject()->setWorldTransform(trans);
}
