#include "Game.h"

void Game::createScene(void){
	// Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    //Create 6 walls
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -700);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1500, 1500, 20, 20, true, 1, 5.0, 5.0, Ogre::Vector3::UNIT_X);

    Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    entGround->setMaterialName("Examples/Rockwall");
    entGround->setCastShadows(false);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);

    Ogre::Entity* entGround2 = mSceneMgr->createEntity("GroundEntity2", "ground");
    entGround2->setMaterialName("Examples/Rockwall");
    entGround2->setCastShadows(false);
    Ogre::SceneNode* wall = mSceneMgr->getRootSceneNode()->createChildSceneNode("GroundEntity2");
    wall->attachObject(entGround2);
    wall->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(180)));
 
    Ogre::Entity* entGround3 = mSceneMgr->createEntity("GroundEntity3", "ground");
    entGround3->setMaterialName("Examples/Rockwall");
    entGround3->setCastShadows(false);
    Ogre::SceneNode* wall3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("GroundEntity3");
    wall3->attachObject(entGround3);
    wall3->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(Ogre::Degree(90)));
    

    Ogre::Entity* entGround4 = mSceneMgr->createEntity("GroundEntity4", "ground");
    entGround4->setMaterialName("Examples/Rockwall");
    entGround4->setCastShadows(false);
    Ogre::SceneNode* wall4 = mSceneMgr->getRootSceneNode()->createChildSceneNode("GroundEntity4");
    wall4->attachObject(entGround4);
    wall4->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(Ogre::Degree(270)));
    

    Ogre::Entity* entGround5 = mSceneMgr->createEntity("GroundEntity5", "ground");
    entGround5->setMaterialName("Examples/Rockwall");
    entGround5->setCastShadows(false);
    Ogre::SceneNode* wall5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("GroundEntity5");
    wall5->attachObject(entGround5);
    wall5->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(270)));

    Ogre::Entity* entGround6 = mSceneMgr->createEntity("GroundEntity6", "ground");
    entGround6->setMaterialName("Examples/Rockwall");
    entGround6->setCastShadows(false);
    Ogre::SceneNode* wall6 = mSceneMgr->getRootSceneNode()->createChildSceneNode("GroundEntity6");
    wall6->attachObject(entGround6);
    wall6->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(90)));

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("OutsideLight");
    light->setPosition(90.0f, 90.0f, 70.0f);
}

bool Game::frameRenderingQueued(const Ogre::FrrameEvent& evt){
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
        for(int x=0; x<entities.size(); x++)
            entities[x].move(evt);
    }

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