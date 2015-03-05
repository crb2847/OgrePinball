#include "Game.h"

bool HandleContacts(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1) {
   GameObject *o0 = (GameObject *) body0->getUserPointer();
   GameObject *o1 = (GameObject *) body1->getUserPointer();
   o0->getGame()->collission(o0, o1);
   return false;
}

Game::Game(){

}

Game::~Game(){}

void Game::collission(GameObject *o0, GameObject *o1) {
	if (o0->kind > o1->kind) {GameObject *tmp = o0; o0 = o1; o1 = tmp;}
	if (o0->kind != K::BALL) return;

	switch(o1->kind){
	case K::WALL:
		break;
	case K::PADDLE:
		break;
	case K::PIT:
		o0->setPosition(Ogre::Vector3(0,0,0));
		o0->rigidBody->setLinearVelocity(o0->bDirection * o0->bSpeed);

		//o0->rigidBody->detachFromParent();
		//delete o0->collShape;
		//delete o0->rigidBody;
		break;
	case K::COIN:
		entities.erase(o1);
		o1->rigidBody->detachFromParent();
		delete o1->collShape;
		delete o1->rigidBody;
		o1->rootNode->detachAllObjects();
		break;
	}


	//if (b0 && mWorld) o0 = mWorld->findObject(b0);
	//if (b1) o1 = mWorld->findObject(b1);

	std::cout << "o0=" << o0->name << ", o1=" << o1->name << "\n";
}

void Game::createScene(void){
	Ogre::Vector3 gravityVector(0,-9.81,0);
	Ogre::AxisAlignedBox bounds (Ogre::Vector3 (-10000, -10000, -10000), Ogre::Vector3 (10000,  10000,  10000));
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector);
	gContactProcessedCallback = (ContactProcessedCallback) HandleContacts;

	// Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    mPaddle = new Paddle(this);
    entities.insert(mPaddle);

    Ball* ball = new Ball(this);
    entities.insert(ball);

    //Create 6 walls
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1000, 1000, 20, 20, true, 1, 5.0, 5.0, Ogre::Vector3::UNIT_X);

    PitPlane* pit = new PitPlane(this, Ogre::Vector3::UNIT_Y, Ogre::Real(-510));
    entities.insert(pit); // Bottom

    Wall *p = new Wall(this, Ogre::Vector3(0.75f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Z), Ogre::Vector3(0, 500, 0));
    entities.insert(p); // Top

    p = new Wall(this, Ogre::Vector3(0.75f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, -100));
    entities.insert(p); // Back

    p = new Wall(this, Ogre::Vector3(0.75f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, 100));
    entities.insert(p); // Front
    
    p = new Wall(this, Ogre::Vector3(1.0f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_Z),  Ogre::Vector3(-375, 0, 0));
    entities.insert(p); // Left

    p = new Wall(this, Ogre::Vector3(1.0f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z), Ogre::Vector3(375, 0, 0));
    entities.insert(p); // Right

    Coin *coin = new Coin(this, Ogre::Vector3(100,100,0));
    entities.insert(coin);

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("OutsideLight");
    light->setPosition(90.0f, 90.0f, 800.0f);
}

bool Game::frameStarted(const Ogre::FrameEvent& evt) {
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameEnded(const Ogre::FrameEvent& evt) {
	mWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
	return true;
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt){
	if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // If dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())          // If details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
        for(GameObject *obj : entities) {
        	obj->update(evt);
        }
    }

    return true;
}


bool Game::keyPressed( const OIS::KeyEvent& evt ){
	BaseApplication::keyPressed(evt);
	if (evt.key == OIS::KC_COMMA) mPaddle->motion |= 1;
	if (evt.key == OIS::KC_PERIOD) mPaddle->motion |= 2;
    return true;
}

bool Game::keyReleased( const OIS::KeyEvent& evt ){
	BaseApplication::keyReleased(evt);
	if (evt.key == OIS::KC_COMMA) mPaddle->motion &= ~1;
	if (evt.key == OIS::KC_PERIOD) mPaddle->motion &= ~2;
	return true;
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Game app;
 
        try 
        {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR| MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif
