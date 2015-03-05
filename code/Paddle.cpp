#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Paddle.h"

Paddle::Paddle(Game *game) : GameObject(game, K::PADDLE){
	name = "Paddle@" + Ogre::StringConverter::toString(id);
	Ogre::Entity* entity = scnMgr->createEntity("et"+name, "cube.mesh");
	entity->setCastShadows(true);
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("nd"+name);
	rootNode->attachObject(entity);

	rootNode->setScale(scale = Ogre::Vector3(2, 0.1, 2));
	rootNode->setPosition(position = Ogre::Vector3(0, -490, 0));
	rootNode->setOrientation(orientation = Ogre::Quaternion());

	Ogre::Vector3 bSize = entity->getBoundingBox().getHalfSize() * 0.95 * scale;
	collShape = new OgreBulletCollisions::BoxCollisionShape(bSize);
	rigidBody = new OgreBulletDynamics::RigidBody("bt"+name, mWorld);
	rigidBody->setStaticShape(rootNode, collShape,
		1, 0.1, // restitution, friction
		position, orientation);

	btCollisionObject *btObj = rigidBody->getBulletObject();
	btObj->setCollisionFlags(btObj->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	btObj->setUserPointer(static_cast<GameObject*>(this));

	bRadius = 100.0f;
	bDirection = Ogre::Vector3(-1.0f, 0.0f, 0.0f);
	bDirection.normalise();
	bSpeed = 250.0f;
}

void Paddle::setPosition(Ogre::Vector3 pos) {
	rootNode->setPosition(pos);
	btTransform trans = rigidBody->getBulletObject()->getWorldTransform();
	trans.setOrigin(OgreBulletCollisions::OgreBtConverter::to(pos));
	rigidBody->getBulletObject()->setWorldTransform(trans);
}

void Paddle::update(const Ogre::FrameEvent& evt){
	if (!motion || motion == 3) return;
	Ogre::Vector3 pos = rootNode->getPosition();
	pos += bSpeed * evt.timeSinceLastFrame * bDirection * ((motion & 1) ? 1 : -1);
	float lim = 750/2.0f - bRadius;
	if (pos.x < -lim) pos.x = -lim;
	if (pos.x > lim) pos.x = lim;
	setPosition(pos);
}