#include "Game.h"

bool HandleContacts(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1) {
   Game *g = (Game *) body0->getUserPointer();
   g->collide(body0, body1);
   return false;
}

Game::Game(){}

Game::~Game(){}

void Game::collide(btCollisionObject *b0, btCollisionObject *b1) {

}

void Game::createScene(void){
	Ogre::Vector3 gravityVector(0,-9.81,0);
	Ogre::AxisAlignedBox bounds (Ogre::Vector3 (-10000, -10000, -10000), Ogre::Vector3 (10000,  10000,  10000));
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector);
	gContactProcessedCallback = (ContactProcessedCallback) HandleContacts;

	// Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    Paddle* paddle = new Paddle(this, 0);
    entities.push_back(paddle);

    Ball* ball = new Ball(this, 0);
    entities.push_back(ball);

    //Create 6 walls
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1000, 1000, 20, 20, true, 1, 5.0, 5.0, Ogre::Vector3::UNIT_X);

    Wall* p = new Wall(this, 0, Ogre::Vector3(0.75f,1.0f,0.2f),
    		Ogre::Quaternion(), Ogre::Vector3(0, -500, 0));
    entities.push_back(p); // Bottom

    p = new Wall(this, 1, Ogre::Vector3(0.75f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Z), Ogre::Vector3(0, 500, 0));
    entities.push_back(p); // Top

    p = new Wall(this, 2, Ogre::Vector3(0.75f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, -100));
    entities.push_back(p); // Back

    p = new Wall(this, 3, Ogre::Vector3(0.75f,1.0f,1.0f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_X), Ogre::Vector3(0, 0, 100));
    entities.push_back(p); // Front
    
    p = new Wall(this, 4, Ogre::Vector3(1.0f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_Z),  Ogre::Vector3(-375, 0, 0));
    entities.push_back(p); // Left

    p = new Wall(this, 5, Ogre::Vector3(1.0f,1.0f,0.2f),
    		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z), Ogre::Vector3(375, 0, 0));
    entities.push_back(p); // Right

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
        for(int x=0; x<entities.size(); x++){
            entities[x]->update(evt, entities);
        }
    }

    return true;
}

/*
bool Game::keyPressed( const OIS::KeyEvent& evt ){
    switch (evt.key)
    {
    case OIS::KC_ESCAPE: 
        mShutDown = true;
        break;
    case OIS::KC_DOWN:
        entities[0]->setBDirection(Ogre::Vector3(-1.0f,0.0f,0.0f));
        break;
    case OIS::KC_UP:
        entities[0]->setBDirection(Ogre::Vector3(1.0f,0.0f,0.0f));
        break;
    default:
        break;
    }
    return true;
}
bool Game::keyReleased( const OIS::KeyEvent& evt ){return true;}
*/

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
