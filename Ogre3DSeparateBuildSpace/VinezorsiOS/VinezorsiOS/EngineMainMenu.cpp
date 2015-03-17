//
//  EngineMainMenu.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#include "EngineMainMenu.h"
#include "EngineStateManager.h"
#include "HudMainMenu.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineMainMenu::EngineMainMenu(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
}

EngineMainMenu::~EngineMainMenu()
{
}

void EngineMainMenu::enter()
{
    alloc();
    player->startMenu();
    
    // Set skybox
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
}

void EngineMainMenu::exit()
{
    dealloc();
}

void EngineMainMenu::update(float elapsed)
{
}

void EngineMainMenu::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI != "")
        player->reactGUI();
    if (queryGUI == "play")
    {
        engineStateMgr->requestPushEngine(ENGINE_SCHEDULER_MENU, player);
    }
    else if (queryGUI == "credits")
    {
<<<<<<< HEAD
        //player->levelRequest = NULL;  // Set to NULL so it won't force jump to scheduler levels
       // player->marbleChoice = -1;
        //engineStateMgr->requestPushEngine(ENGINE_LEVEL_SELECTION, player);
=======
        player->levelRequest = NULL;  // Set to NULL so it won't force jump to scheduler levels
        player->marbleChoice = -1;
        engineStateMgr->requestPushEngine(ENGINE_LEVEL_SELECTION, player);
>>>>>>> 2d6a258041a97a8de0cb96b4aceb6c9560c0c3cc
    }
    else if (queryGUI == "settings")
    {
        engineStateMgr->requestPushEngine(ENGINE_MAIN_SETTINGS, player);
    }
}

#if !defined(OGRE_IS_IOS)
void EngineMainMenu::mouseMoved(const OIS::MouseEvent &evt)
{
}

void EngineMainMenu::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            activatePerformSingleTap(evt.state.X.abs, evt.state.Y.abs);
            break;
        default:
            break;
    }
}

void EngineMainMenu::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            break;
        default:
            break;
    }
}

void EngineMainMenu::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void EngineMainMenu::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineMainMenu::requestResize()
{
    if (hud) hud->adjust();
}

void EngineMainMenu::alloc()
{
    hud = new HudMainMenu(player);
}

void EngineMainMenu::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}
