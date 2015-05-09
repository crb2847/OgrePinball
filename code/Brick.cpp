#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Brick.h"

const float Brick::colors[][3] =
	{{1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};

Brick::Brick(Game *game, Ogre::Vector3 position, int points, bool hasCoin) :
		GameObject(game, K::BRICK), points(points), hasCoin(hasCoin) {
	name = "Brick@" + Ogre::StringConverter::toString(id);

	entity = scnMgr->createEntity("et"+name, "cube.mesh");
	entity->setCastShadows(true);
	entity->setMaterialName("Brick");

	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("nd"+name);
	rootNode->attachObject(entity);

	rootNode->setScale(scale = Ogre::Vector3(1.0, 0.5, 2));
	rootNode->setPosition(GameObject::position = position);

	Ogre::Vector3 bSize = entity->getBoundingBox().getHalfSize() * 0.95 * scale;
	collShape = new OgreBulletCollisions::BoxCollisionShape(bSize);
	rigidBody = new OgreBulletDynamics::RigidBody("bt"+name, mWorld);
	rigidBody->setStaticShape(rootNode, collShape,
		1, 0.1, // restitution, friction
		position, orientation);

	btCollisionObject *btObj = rigidBody->getBulletObject();
	btObj->setCollisionFlags(btObj->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	btObj->setUserPointer(static_cast<GameObject*>(this));

	update(Ogre::FrameEvent());
}

void Brick::update(const Ogre::FrameEvent& evt) {
	int numColors = sizeof(colors)/sizeof(colors[0]);
	int ci = (points > 0) ? ((points-1) % numColors) : 0;
	const float *c = colors[ci];
	entity->getSubEntity(0)->setCustomParameter(1, Ogre::Vector4(c[0], c[1], c[2], 1.0));
}
