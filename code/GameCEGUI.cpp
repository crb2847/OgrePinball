#include "Game.h"

void Game::initCEGUI(void){
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	sheet = wmgr.createWindow("DefaultWindow", "OgrePinball/Sheet");

	//the start menu
	menu = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Menu");
	menu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	menu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));
	sheet->addChild(menu);

	//the one-player game button
	CEGUI::Window *onePlayer = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/OnePlayer");
	onePlayer->setText("Single Player Game");
	onePlayer->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	onePlayer->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.02,0)));
	menu->addChild(onePlayer);

	//the two-player game button
	CEGUI::Window *twoPlayer = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/TwoPlayer");
	twoPlayer->setText("Multiplayer Player Game");
	twoPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	twoPlayer->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.22,0)));
	menu->addChild(twoPlayer);

	//the settings button
	CEGUI::Window *settings = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Settings");
	settings->setText("Settings");
	settings->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	settings->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.42,0)));
	menu->addChild(settings);

	//the how to play button
	CEGUI::Window *howToPlay = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/HowToPlay");
	howToPlay->setText("How To Play");
	howToPlay->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	howToPlay->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.62,0)));
	menu->addChild(howToPlay);

	//the quit button
	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Quit");
	quit->setText("Quit");
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	quit->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.82,0)));
	menu->addChild(quit);

	//the pause button
	pause = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Pause");
	pause->setText("Pause");
	pause->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));

	//the pause menu
	pauseMenu = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/PauseMenu");
	pauseMenu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	pauseMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	//the resume button
	CEGUI::Window *resume = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Pause/PauseMenu/Resume");
	resume->setText("Resume Game");
	resume->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	resume->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.16,0)));
	pauseMenu->addChild(resume);

	//the main menu button (in the pause menu)
	CEGUI::Window *mainMenu = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Pause/PauseMenu/MainMenu");
	mainMenu->setText("Main Menu");
	mainMenu->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	mainMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.4,0)));
	pauseMenu->addChild(mainMenu);

	//the sound button (in the pause menu)
	soundPause = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Pause/PauseMenu/Sound");
	soundPause->setText("Disable Sound");
	soundPause->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	soundPause->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.64,0)));
	pauseMenu->addChild(soundPause);

	//the settings menu
	settingsMenu = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Settings/SettingsMenu");
	settingsMenu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	settingsMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	//the sound button (in the settings menu)
	sound = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/SettingsMenu/Sound");
	sound->setText("Disable Sound");
	sound->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	sound->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.28,0)));
	settingsMenu->addChild(sound);

	//the back button (in the settings menu)
	CEGUI::Window *back = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/SettingsMenu/Back");
	back->setText("Back");
	back->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
	back->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.52,0)));
	settingsMenu->addChild(back);

	//the how to play menu
	howToPlayMenu = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Menu/howToPlayMenu");
	howToPlayMenu->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	howToPlayMenu->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	//the back button (in the how to play menu)
	CEGUI::Window *backHTP = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/howToPlayMenu/Back");
	backHTP->setText("Back");
	backHTP->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	backHTP->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.82,0)));
	howToPlayMenu->addChild(backHTP);

	//the score box
	scoreBox = wmgr.createWindow("TaharezLook/StaticText", "OgrePinball/Pause/PauseMenu/Sound");
	scoreBox->setText("Score: 0");
	scoreBox->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
	scoreBox->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.9,0), CEGUI::UDim(0,0)));

	//the score box (player one in multi)
	scoreBox1 = wmgr.createWindow("TaharezLook/StaticText", "OgrePinball/Pause/PauseMenu/Sound");
	scoreBox1->setText("Player One Score: 0");
	scoreBox1->setSize(CEGUI::USize(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
	scoreBox1->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.8,0), CEGUI::UDim(0,0)));

	//the score box (player two in multi)
	scoreBox2 = wmgr.createWindow("TaharezLook/StaticText", "OgrePinball/Pause/PauseMenu/Sound");
	scoreBox2->setText("Player Two Score: 0");
	scoreBox2->setSize(CEGUI::USize(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
	scoreBox2->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.8,0), CEGUI::UDim(0.05,0)));

	//the single player connection screen
	singlePlayerConnection = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Menu/SinglePlayer/SinglePlayerConnection");
	singlePlayerConnection->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	singlePlayerConnection->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	//the multiplayer connection screen
	multiPlayerConnection = wmgr.createWindow("TaharezLook/FrameWindow", "OgrePinball/Menu/Multiplayer/multiPlayerConnection");
	multiPlayerConnection->setSize(CEGUI::USize(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
	multiPlayerConnection->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.3,0), CEGUI::UDim(0.25,0)));

	//the back button (in the multiplayer connection screen)
	CEGUI::Window *backMCS = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Multiplayer/MCS/Back");
	backMCS->setText("Back");
	backMCS->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	backMCS->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.82,0)));
	multiPlayerConnection->addChild(backMCS);

	//the waiting for connection text box (multi)
	CEGUI::Window *info = wmgr.createWindow("TaharezLook/StaticText", "OgrePinball/Menu/Muliplayer/MCS/Info");
	info->setText("Waiting on connection with the controllers");
	info->setSize(CEGUI::USize(CEGUI::UDim(0.96, 0), CEGUI::UDim(0.3, 0)));
	info->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.02,0), CEGUI::UDim(0.02,0)));
	multiPlayerConnection->addChild(info);

	//the waiting for connection text box (single)
	CEGUI::Window *info2 = wmgr.createWindow("TaharezLook/StaticText", "OgrePinball/Menu/Muliplayer/MCS/Info");
	info2->setText("Waiting on connection with the controller");
	info2->setSize(CEGUI::USize(CEGUI::UDim(0.96, 0), CEGUI::UDim(0.3, 0)));
	info2->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.02,0), CEGUI::UDim(0.02,0)));
	singlePlayerConnection->addChild(info2);

	//the play with keys button (in multi connection screen)
	CEGUI::Window *keysMCS = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Multiplayer/MCS/Back");
	keysMCS->setText("Play With Keys");
	keysMCS->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	keysMCS->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.62,0)));
	multiPlayerConnection->addChild(keysMCS);

	//the back button (in the single player connection screen)
	CEGUI::Window *backSCS = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Multiplayer/MCS/Back");
	backSCS->setText("Back");
	backSCS->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	backSCS->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.82,0)));
	singlePlayerConnection->addChild(backSCS);

	//the play with keys button (in single connection screen)
	CEGUI::Window *keysSCS = wmgr.createWindow("TaharezLook/Button", "OgrePinball/Menu/Multiplayer/MCS/Back");
	keysSCS->setText("Play With Keys");
	keysSCS->setSize(CEGUI::USize(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.16, 0)));
	keysSCS->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.125,0), CEGUI::UDim(0.62,0)));
	singlePlayerConnection->addChild(keysSCS);

	//how to play information
	CEGUI::Window *HTPinfo = wmgr.createWindow("TaharezLook/StaticText", "OgrePinball/Menu/howToPlayMenu/Info");
	HTPinfo->setText("Single Player Instructions:\nScore points by collecting coins without\nmissing the ball.\n\nMulti Player Instructions:\nThe player who last hit the ball gets the\npoints from the coins.\n\nControls:\nIn both modes, you can play with either\nthe arrow keys (and AS keys for\nmultiplayer) or by tilting the controllers\nPressing R turns on wireframes\n8, 9, and 0 change the camera");
	HTPinfo->setSize(CEGUI::USize(CEGUI::UDim(0.96, 0), CEGUI::UDim(0.85, 0)));
	HTPinfo->setPosition(CEGUI::Vector2<CEGUI::UDim>(CEGUI::UDim(0.02,0), CEGUI::UDim(0.02,0)));
	howToPlayMenu->addChild(HTPinfo);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
	onePlayer->   subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::startSinglePlayer, this));
	twoPlayer->   subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::startMultiPlayer,  this));
	quit->        subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::quit,              this));
	pause->       subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::pauseGame,         this));
	resume->      subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::resumeGame,        this));
	mainMenu->    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::openMainMenu,      this));
	settings->    subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::openSettingsMenu,  this));
	back->        subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::goBack,            this));
	howToPlay->   subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::openHowToPlayMenu, this));
	backHTP->     subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::goBackHTP,         this));
	backMCS->     subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::goBackMCS,         this));
	keysMCS->     subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::playKeysMCS,       this));
	backSCS->     subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::goBackSCS,         this));
	keysSCS->     subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::playKeysSCS,       this));
}
//-------------------------------------------------------------------------------------
bool Game::quit(const CEGUI::EventArgs &e)
{
  mShutDown = true;
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::startSinglePlayer(const CEGUI::EventArgs &e)
{
  sheet->removeChild(menu);
  sheet->addChild(singlePlayerConnection);
  state = GAMEST_CONN_SINGLE;
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::pauseGame(const CEGUI::EventArgs &e)
{
  sheet->removeChild(pause);
  sheet->addChild(pauseMenu);
  if(state == GAMEST_SINGLE)
  {
    sheet->removeChild(scoreBox);
  	state = GAMEST_SINGLE_MENU;
  }
  else if(state == GAMEST_MULTI)
  {
  	sheet->removeChild(scoreBox1);
  	sheet->removeChild(scoreBox2);
  	state = GAMEST_MULTI_MENU;
  }
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::resumeGame(const CEGUI::EventArgs &e)
{
  sheet->removeChild(pauseMenu);
  sheet->addChild(pause);
  if(state == GAMEST_SINGLE_MENU)
  {
  	sheet->addChild(scoreBox);
  	state = GAMEST_SINGLE;
  }
  else if(state == GAMEST_MULTI_MENU)
  {
  	sheet->addChild(scoreBox1);
  	sheet->addChild(scoreBox2);
  	state = GAMEST_MULTI;
  }
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::openMainMenu(const CEGUI::EventArgs &e)
{
  sheet->removeChild(pauseMenu);
  sheet->addChild(menu);
  state = GAMEST_MENU;
}
//-------------------------------------------------------------------------------------
bool Game::openSettingsMenu(const CEGUI::EventArgs &e)
{
  sheet->removeChild(menu);
  sheet->addChild(settingsMenu);
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::goBack(const CEGUI::EventArgs &e)
{
  sheet->removeChild(settingsMenu);
  sheet->addChild(menu);
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::openHowToPlayMenu(const CEGUI::EventArgs &e)
{
  sheet->removeChild(menu);
  sheet->addChild(howToPlayMenu);

  return true;
}
//-------------------------------------------------------------------------------------
bool Game::goBackHTP(const CEGUI::EventArgs &e)
{
  sheet->removeChild(howToPlayMenu);
  sheet->addChild(menu);

  return true;
}
//-------------------------------------------------------------------------------------
bool Game::goBackMCS(const CEGUI::EventArgs &e)
{
  sheet->removeChild(multiPlayerConnection);
  sheet->addChild(menu);

  return true;
}
//-------------------------------------------------------------------------------------
bool Game::startMultiPlayer(const CEGUI::EventArgs &e)
{
  sheet->removeChild(menu);
  sheet->addChild(multiPlayerConnection);
  state = GAMEST_CONN_MULTI;
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::playKeysMCS(const CEGUI::EventArgs &e)
{
  startGame(GAMEST_MULTI);
  return true;
}
//-------------------------------------------------------------------------------------
bool Game::goBackSCS(const CEGUI::EventArgs &e)
{
  sheet->removeChild(singlePlayerConnection);
  sheet->addChild(menu);

  return true;
}
//-------------------------------------------------------------------------------------
bool Game::playKeysSCS(const CEGUI::EventArgs &e)
{
  startGame(GAMEST_SINGLE);
  return true;
}
//-------------------------------------------------------------------------------------

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID) {
    case OIS::MB_Left:   return CEGUI::LeftButton;
    case OIS::MB_Right:  return CEGUI::RightButton;
    case OIS::MB_Middle: return CEGUI::MiddleButton;
    default:             return CEGUI::LeftButton;
    }
}
