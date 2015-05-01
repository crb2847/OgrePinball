#ifndef __Entity_h_
#define __Entity_h_

#include "OgreBulletDynamicsRigidBody.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"

class Game;

namespace K {
	enum Kind {BALL, WALL, PIT, COIN, PADDLE, OBSTACLE};
}

class GameObject
{
	friend class Game;
	public: 
	    virtual ~GameObject();
		GameObject(Game *game, int kind);
		void setBDirection(Ogre::Vector3);
		void move(const Ogre::FrameEvent& evt);
		virtual void update(const Ogre::FrameEvent& evt);
		Ogre::SceneNode* getNode() { return rootNode; }
		Game *getGame() {return game;}
		virtual void setPosition(Ogre::Vector3 pos);
		virtual void setRotation(Ogre::Quaternion orientation);

		Ogre::String name;
		int id, kind, subkind;

	protected: 
		static int cur_id;

		Game *game;
		Ogre::SceneNode* rootNode;
		Ogre::Real bRadius;
		Ogre::Vector3 bDirection;
		Ogre::Real bSpeed;

		Ogre::Vector3 scale, position;
		Ogre::Quaternion orientation;

		Ogre::SceneManager *scnMgr;
		OgreBulletDynamics::DynamicsWorld *mWorld;

		Ogre::Entity* entity;
		OgreBulletCollisions::CollisionShape *collShape;
		OgreBulletDynamics::RigidBody *rigidBody;
};

#endif // #ifndef __Entity_h_

