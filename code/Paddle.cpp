#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Paddle.h"

Paddle::Paddle(Game *game, int pID) : GameObject(game, K::PADDLE){
	name = "Paddle@" + Ogre::StringConverter::toString(id);
	Ogre::Entity* entity = scnMgr->createEntity("et"+name, "cube.mesh");
	entity->setCastShadows(true);
	entity->setMaterialName("Paddle");
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("nd"+name);
	rootNode->attachObject(entity);

	bRadius = 65.0f;
	rootNode->setScale(scale = Ogre::Vector3(bRadius/50, 0.1, 2));
	rootNode->setPosition(position = Ogre::Vector3(((pID==1)?-700:0) + bRadius, -490, 0));
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

	bDirection = Ogre::Vector3(-1.0f, 0.0f, 0.0f);
	bDirection.normalise();
	bSpeed = (pID == 0) ? 1000.0 : 500.0f;
	playerID = pID;
}

void Paddle::setPosition(Ogre::Vector3 pos) {
	float lim = 1400/2.0f - bRadius;
	if (playerID == 0) {
		if (pos.x < -lim) pos.x = -lim;
		if (pos.x > lim) pos.x = lim;
	} if (playerID == 1) {
		if (pos.x < -lim) pos.x = -lim;
		if (pos.x > -bRadius) pos.x = -bRadius;
	} else if (playerID == 2){
		if (pos.x > lim) pos.x = lim;
		if (pos.x < bRadius) pos.x = bRadius;
	}
	rootNode->setPosition(pos);
	btTransform trans = rigidBody->getBulletObject()->getWorldTransform();
	trans.setOrigin(OgreBulletCollisions::OgreBtConverter::to(pos));
	rigidBody->getBulletObject()->setWorldTransform(trans);
}

void Paddle::update(const Ogre::FrameEvent& evt){
	if (!motion || motion == 3) return;
	Ogre::Vector3 pos = rootNode->getPosition();
	pos += bSpeed * evt.timeSinceLastFrame * bDirection * ((motion & 1) ? 1 : -1);
	setPosition(pos);
}

void Paddle::gyroMovement(double d){
	Ogre::Vector3 pos = rootNode->getPosition();
	pos.x += d*0.5;
	setPosition(pos);
}

void Paddle::tilt(double a) {
	setRotation(Ogre::Quaternion(Ogre::Degree(16*a), Ogre::Vector3(0,0,1)));
}
