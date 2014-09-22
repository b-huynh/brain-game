//
//  EngineSchedulerMenu.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 9/15/14.
//
//

#include "EngineSchedulerMenu.h"
#include "EngineStateManager.h"
#include "HudSchedulerMenu.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineSchedulerMenu::EngineSchedulerMenu(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
    enter();
}

EngineSchedulerMenu::~EngineSchedulerMenu()
{
}

void EngineSchedulerMenu::enter()
{
    alloc();
    player->startMenu();
    
    // Set skybox
    Util::setSkyboxAndFog("");
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
}

void EngineSchedulerMenu::exit()
{
    dealloc();
}

void EngineSchedulerMenu::update(float elapsed)
{
    hud->update(elapsed);
}

void EngineSchedulerMenu::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI != "")
        player->reactGUI();
    if (testForLevelButtons(queryGUI))
    {
        
    }
    else if (queryGUI == "back")
    {
        engineStateMgr->requestPopEngine();
    }
}

#if !defined(OGRE_IS_IOS)
void EngineSchedulerMenu::mouseMoved(const OIS::MouseEvent &evt)
{
}

void EngineSchedulerMenu::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void EngineSchedulerMenu::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            break;
        default:
            break;
    }
}

void EngineSchedulerMenu::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    switch (keyEventRef.key)
    {
        default:
            break;
    }
}

void EngineSchedulerMenu::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineSchedulerMenu::requestResize()
{
    if (hud) hud->adjust();
}

void EngineSchedulerMenu::alloc()
{
    hud = new HudSchedulerMenu(player);
}

void EngineSchedulerMenu::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}

bool EngineSchedulerMenu::testForLevelButtons(const std::string & queryGUI)
{
//    std::vector< std::pair<StageRequest, PlayerProgress> > choices = player->scheduler->generateChoices();
    player->feedLevelRequestFromSchedule();
    if (queryGUI == "selection0")
    {
        // player->setLevelRequest(0, 1);
        player->levelRequest = &player->scheduleChoice1;
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
        return true;
    }
    else if (queryGUI == "selection1")
    {
//        player->setLevelRequest(0, 2);
        player->levelRequest = &player->scheduleChoice2;
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
        return true;
    }
    else if (queryGUI == "selection2")
    {
//        player->setLevelRequest(0, 3);
        player->levelRequest = &player->scheduleChoice3;
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
        return true;
    }
    return false;
}
