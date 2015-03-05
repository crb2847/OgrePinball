#ifndef __Entity_h_
#define __Entity_h_

#include "OgreBulletDynamicsRigidBody.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"

class Game;

namespace K {
	enum Kind { WALL, PIT, BALL, COIN};
}

class GameObject
{
	public: 
		GameObject(Game *game, int kind);
		void setBDirection(Ogre::Vector3);
		void move(const Ogre::FrameEvent& evt);
		virtual void update(const Ogre::FrameEvent& evt, std::vector<GameObject*> &e);
		Ogre::SceneNode* getNode() { return rootNode; }

	protected: 

		Game *game;
		Ogre::SceneNode* rootNode;
		Ogre::Real bRadius;
		Ogre::Vector3 bDirection;
		Ogre::Real bSpeed;

		static int cur_id;
		Ogre::String name;
		int id, kind;

		Ogre::Vector3 scale, position;
		Ogre::Quaternion orientation;

		Ogre::SceneManager *scnMgr;
		OgreBulletDynamics::DynamicsWorld *mWorld;

		Ogre::Entity* entity;
		OgreBulletCollisions::CollisionShape *collShape;
		OgreBulletDynamics::RigidBody *rigidBody;
};

#endif // #ifndef __Entity_h_

