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
}

EngineSchedulerMenu::~EngineSchedulerMenu()
{
}

void EngineSchedulerMenu::enter()
{
    alloc();
    
    // Set skybox
    Util::setSkyboxAndFog("");
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
    
    TutorialManager* tutorialMgr = player->getTutorialMgr();
    if (tutorialMgr->isEnabled() && !tutorialMgr->hasVisitedSlide(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_NAVIGATION)) {
        player->levelRequest = &player->scheduler->tutorialLevels[0];
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (tutorialMgr->isEnabled() && !tutorialMgr->hasVisitedSlide(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_1BACK)) {
        player->levelRequest = &player->scheduler->tutorialLevels[1];
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else
    {
        player->startMenu();
        
        if (player->lastPlayed > 0) {
            
            switch (player->lastPlayed)
            {
                case 'A':
                    player->levelRequest = &player->scheduler->scheduleHistoryA.back();
                    hud->setSelectToIcon(hud->historyOverlayPanels[0].entireBackground);
                    //hud->historyOverlayPanels[0].entireBackground->setMaterialName("General/YellowSphereIcon");
                    break;
                case 'B':
                    player->levelRequest = &player->scheduler->scheduleHistoryB.back();
                    hud->setSelectToIcon(hud->historyOverlayPanels[1].entireBackground);
                    //hud->historyOverlayPanels[1].entireBackground->setMaterialName("General/YellowSphereIcon");
                    break;
                case 'C':
                    player->levelRequest = &player->scheduler->scheduleHistoryC.back();
                    hud->setSelectToIcon(hud->historyOverlayPanels[2].entireBackground);
                    //hud->historyOverlayPanels[2].entireBackground->setMaterialName("General/YellowSphereIcon");
                    break;
                case 'D':
                    player->levelRequest = &player->scheduler->scheduleHistoryD.back();
                    hud->setSelectToIcon(hud->historyOverlayPanels[3].entireBackground);
                    //hud->historyOverlayPanels[3].entireBackground->setMaterialName("General/YellowSphereIcon");
                    break;
                case 'E':
                    player->levelRequest = &player->scheduler->scheduleHistoryE.back();
                    hud->setSelectToIcon(hud->historyOverlayPanels[4].entireBackground);
                    //hud->historyOverlayPanels[4].entireBackground->setMaterialName("General/YellowSphereIcon");
                    break;
            }
            hud->setSelection();
        }
    }
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
        // If a level is selected, then we can continue,
        // Also make sure it isn't a level selected from the history panel
        if (player->levelRequest && player->levelRequest->second.rating < 0)
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
        
        switch (iconNo)
        {
            case 0:
                if (player->scheduler->scheduleHistoryA.size() > 0)
                {
                    player->levelRequest = &player->scheduler->scheduleHistoryA.back();
                    hud->setSelection();
                }
                else
                    player->levelRequest = NULL;
                break;
            case 1:
                if (player->scheduler->scheduleHistoryB.size() > 0)
                {
                    player->levelRequest = &player->scheduler->scheduleHistoryB.back();
                    hud->setSelection();
                }
                else
                    player->levelRequest = NULL;
                break;
            case 2:
                if (player->scheduler->scheduleHistoryC.size() > 0)
                {
                    player->levelRequest = &player->scheduler->scheduleHistoryC.back();
                    hud->setSelection();
                }
                else
                    player->levelRequest = NULL;
                break;
            case 3:
                if (player->scheduler->scheduleHistoryD.size() > 0)
                {
                    player->levelRequest = &player->scheduler->scheduleHistoryD.back();
                    hud->setSelection();
                }
                else
                    player->levelRequest = NULL;
                break;
            case 4:
                if (player->scheduler->scheduleHistoryE.size() > 0)
                {
                    player->levelRequest = &player->scheduler->scheduleHistoryE.back();
                    hud->setSelection();
                }
                else
                    player->levelRequest = NULL;
                break;
        }
        return true;
    }
    return false;
}
