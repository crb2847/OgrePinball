#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "PitPlane.h"

PitPlane::PitPlane(Game *game, int x, Ogre::Vector3 normal, Ogre::Real distance) : GameObject(game, x){
	name = "PitPlane@" + Ogre::StringConverter::toString(id);
	collShape = new OgreBulletCollisions::StaticPlaneCollisionShape(normal, distance);

	rigidBody = new OgreBulletDynamics::RigidBody("bt"+name, mWorld);
	rigidBody->setStaticShape(collShape, 1, 0.1); // .., restitution, friction

	btCollisionObject *btObj = rigidBody->getBulletObject();
	btObj->setCollisionFlags(btObj->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK |
			btCollisionObject::CF_NO_CONTACT_RESPONSE);
	btObj->setUserPointer(static_cast<GameObject*>(this));
}

void PitPlane::update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e) {}
