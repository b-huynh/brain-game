//
//  EngineSchedulerMenu.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 9/15/14.
//
//

#include "EngineSchedulerMenu.h"
#include "EngineStateManager.h"
#include "LevelSet.h"
#include "PlayerProgress.h"

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
    
    player->feedLevelRequestFromSchedule();
    
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
    std::string queryGUI = hud->processButtons(Vector2(x, y));
    if (queryGUI != "")
        player->reactGUI();
    if (testForLevelButtons(queryGUI))
    {
        
    }
    else if (queryGUI == "back")
    {
        engineStateMgr->requestPopEngine();
    }
    else if (queryGUI == "play")
    {
        // If a level is selected, then we can continue
        if (player->levelRequest)
            engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
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
    if (queryGUI == "selection0")
    {
        player->levelRequest = &player->scheduleChoice1;
        hud->setSelection();
        return true;
    }
    else if (queryGUI == "selection1")
    {
        player->levelRequest = &player->scheduleChoice2;
        hud->setSelection();
        return true;
    }
    else if (queryGUI == "selection2")
    {
        player->levelRequest = &player->scheduleChoice3;
        hud->setSelection();
        return true;
    }
    else if (queryGUI.substr(0, 7) == "history")
    {
        int iconNo = std::atoi(queryGUI.substr(7, 1).c_str());
        
        // Since we may not be able to fit the entire playthrough on the screen,
        // display the last 4 or 5 plays
        int startingScheduleIndex = hud->getStartingSchedulerHistoryIndex();
        int historyIndexRequest = startingScheduleIndex + iconNo;
        if (historyIndexRequest < player->scheduler->scheduleHistory.size())
        {
            player->levelRequest = &player->scheduler->scheduleHistory[historyIndexRequest];
            hud->setSelection();
        }
        else
            player->levelRequest = NULL;
        return true;
    }
    return false;
}
