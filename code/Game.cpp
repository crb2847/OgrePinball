#include "Game.h"

Game::Game(){}

Game::~Game(){}

void Game::createScene(void){
	// Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    //Create 6 walls
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -500);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1000, 1000, 20, 20, true, 1, 5.0, 5.0, Ogre::Vector3::UNIT_X);

    /*
    * This is how you add a new entity, after you add a new entity they will be facing the same direction so you need to change the vector attribute of e before you push it to the list
    */

    Entity d(mSceneMgr, 0);
    entities.push_back(d);

    Entity e(mSceneMgr, 1);
    entities.push_back(e);



    Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    entGround->setMaterialName("Examples/Rockwall");
    entGround->setCastShadows(false);
    Ogre::SceneNode* wall = mSceneMgr->getRootSceneNode()->createChildSceneNode("GroundEntity");
    wall->attachObject(entGround);

    Ogre::Entity* entGround2 = mSceneMgr->createEntity("GroundEntity2", "ground");
    entGround2->setMaterialName("Examples/Rockwall");
    entGround2->setCastShadows(false);
    Ogre::SceneNode* wall2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("GroundEntity2");
    wall2->attachObject(entGround2);
    wall2->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(180)));
 
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

    wall->scale(0.75f,1.0f,0.2f);
    wall2->scale(0.75f,1.0f,0.2f);
    wall3->setPosition(0.0f,0.0f,400.0f);
    wall4->setPosition(0.0f,0.0f,-400.0f);
    wall3->scale(0.75f,1.0f,1.0f);
    wall4->scale(0.75f,1.0f,1.0f);
    wall6->translate(-125.0f,0.0f,0.0f);
    wall5->translate(125.0f,0.0f,0.0f);
    wall5->scale(1.0f,1.0f,0.2f);
    wall6->scale(1.0f,1.0f,0.2f);

    Ogre::Entity *entity = mSceneMgr->createEntity("Obs1", "cube.mesh");
    entity->setCastShadows(false);
    entity->setMaterialName("Obstacle");
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Obs1");
    node->attachObject(entity);
    node->setPosition(-80, 200, 0);
    node->setScale(0.2, 0.4, 0.8);

    std::vector<Ogre::Vector3> coinPos {Ogre::Vector3(30,150,0), Ogre::Vector3(30,-30,0), Ogre::Vector3(40,-90,0), Ogre::Vector3(-70,90,0) ,Ogre::Vector3(-40,-130,0) };

    int i = 0;

    for(Ogre::Vector3 coin: coinPos){
        entity = mSceneMgr->createEntity("Coin" + std::to_string(i), "disc.mesh");
        entity->setCastShadows(false);
        entity->setMaterialName("Coin");

    	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Coin" + std::to_string(i));
		node->attachObject(entity);
		node->setPosition(coin);
		node->setScale(30, 30, 30);
		node->rotate(Ogre::Vector3(1,1,1), Ogre::Degree(25));
		coins.push_back(node);
		i++;
    }

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("OutsideLight");
    light->setPosition(90.0f, 90.0f, 800.0f);
}

bool Game::keyPressed( const OIS::KeyEvent& evt ){
    switch (evt.key)
    {
    case OIS::KC_ESCAPE: 
        mShutDown = true;
        break;
    case OIS::KC_DOWN:
        entities[0].setBDirection(Ogre::Vector3(-1.0f,0.0f,0.0f));
        break;
    case OIS::KC_UP:
        entities[0].setBDirection(Ogre::Vector3(1.0f,0.0f,0.0f));
        break;
    default:
        break;
    }
    return true;
}
bool Game::keyReleased( const OIS::KeyEvent& evt ){return true;}


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
        for(int x=0; x<entities.size(); x++)
            entities[x].move(evt);
        for(Ogre::SceneNode *c : coins){
        	if (c->getPosition().distance(entities[1].getNode()->getPosition()) < 40.0f) {
        		c->setVisible(false);
        	}
        }
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