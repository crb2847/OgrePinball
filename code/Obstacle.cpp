#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Obstacle.h"

Obstacle::Obstacle(Game *game, Ogre::Vector3 position) : GameObject(game, K::OBSTACLE){
	name = "Obstacle@" + Ogre::StringConverter::toString(id);
	Ogre::Entity* entity = scnMgr->createEntity("et"+name, "cube.mesh");
	entity->setCastShadows(true);
	entity->setMaterialName("Obstacle");
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("nd"+name);
	rootNode->attachObject(entity);

	rootNode->setScale(scale = Ogre::Vector3(0.65, 0.65, 2));
	rootNode->setOrientation(orientation = Ogre::Quaternion(Ogre::Degree(45), Ogre::Vector3(0,0,1)));
	rootNode->setPosition(Obstacle::position = position);

	Ogre::Vector3 bSize = entity->getBoundingBox().getHalfSize() * 0.95 * scale;
	collShape = new OgreBulletCollisions::BoxCollisionShape(bSize);
	rigidBody = new OgreBulletDynamics::RigidBody("bt"+name, mWorld);
	rigidBody->setStaticShape(rootNode, collShape,
		1, 0.1, // restitution, friction
		position, orientation);

	btCollisionObject *btObj = rigidBody->getBulletObject();
	btObj->setCollisionFlags(btObj->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	btObj->setUserPointer(static_cast<GameObject*>(this));
}

void Obstacle::update(const Ogre::FrameEvent& evt) { }
