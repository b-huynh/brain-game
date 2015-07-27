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
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
    
    TutorialManager* tutorialMgr = player->getTutorialMgr();
    if (tutorialMgr->isEnabled() && !tutorialMgr->hasVisitedSlide(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_NAVIGATION)) {
        player->levelRequest = &player->scheduler->tutorialLevels[0];
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
        player->marbleChoice = 0;
    }
    else if (tutorialMgr->isEnabled() && !tutorialMgr->hasVisitedSlide(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_1BACK)) {
        player->levelRequest = &player->scheduler->tutorialLevels[1];
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
        
        //Reset Tutorial Level Counter!
        player->choice0RestartCounter = 0;
        player->marbleChoice = 0;
    }
    else
    {
        player->startMenu();
        
        switch (player->lastPlayed)
        {
            case PHASE_COLOR_SOUND:
                player->levelRequest = &player->scheduler->scheduleHistoryA.back();
                hud->setSelectToIcon(hud->historyOverlayPanels[0].entireBackground, 0);
                break;
            case PHASE_SHAPE_SOUND:
                player->levelRequest = &player->scheduler->scheduleHistoryB.back();
                hud->setSelectToIcon(hud->historyOverlayPanels[1].entireBackground, 0);
                break;
            case PHASE_SOUND_ONLY:
                player->levelRequest = &player->scheduler->scheduleHistoryC.back();
                hud->setSelectToIcon(hud->historyOverlayPanels[2].entireBackground, 0);
                break;
            case PHASE_ALL_SIGNAL:
                player->levelRequest = &player->scheduler->scheduleHistoryD.back();
                hud->setSelectToIcon(hud->historyOverlayPanels[3].entireBackground, 0);
                break;
            case PHASE_COLLECT:
                player->levelRequest = &player->scheduler->scheduleHistoryE.back();
                hud->setSelectToIcon(hud->historyOverlayPanels[4].entireBackground, 0);
                break;
            default:
                break;
        }
        player->lastPlayed = PHASE_UNKNOWN;
    }
    
}

void EngineSchedulerMenu::exit()
{
    dealloc();
}

void EngineSchedulerMenu::update(float elapsed)
{
    hud->update(elapsed);
    player->totalElapsedGeneral += elapsed;
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
        if(!globals.manRecessEnabled)
        {
            engineStateMgr->requestPopEngine();
        }
        else
        {
            if(globals.manRecessCount != globals.manRecessLevelLimit)
            {
                engineStateMgr->requestPopEngine();
            }
        }

    }
    else if(queryGUI == "manrecessplay")
    {
        /*std::cout<<"Make Man Recess the level request and request the pushback to engine!"<<std::endl;
        //Push the level request for a recess level!
        //globals.levelRequest = &globals.scheduleChoice1;
        bool holdout = false;
        double shift = 0.0;
        int currentUNL = (int)round(player->scheduler->nBackLevelE);
        double playerSkill;
        //If Recess and indRecess Enabled, we use another UNL!
        if(player->indRecessEnabled)
        {
            playerSkill = player->indRecessNBackLevel;
            currentUNL = (int)round(player->indRecessNBackLevel);
            
        }
        double nBackRounded = (int)round(playerSkill + shift);
        
        std::pair<StageRequest, PlayerProgress> node;
        node.first.generateStageRequest(nBackRounded, PHASE_COLLECT, DIFFICULTY_HARD, DURATION_NORMAL, 0.0, 0, currentUNL,player->newNavEnabled);
        player->levelRequest = &node;
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);*/
        player->levelRequest = &player->scheduleManRecessLevel;
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
        /*std::cout << "Choice0Counter: " << player->choice0RestartCounter << std::endl;
        std::cout << "Choice1Counter: " << player->choice1RestartCounter << std::endl;;
        std::cout << "Choice2Counter: " << player->choice2RestartCounter << std::endl;;
        std::cout << "Choice3Counter: " << player->choice3RestartCounter << std::endl;;*/

        //Is it always Choice1 ??
        player->choice0RestartCounter++;
        player->choice1RestartCounter++;
        player->choice2RestartCounter++;
        player->choice3RestartCounter++;


        
    
        
    }
    else if (queryGUI == "play")
    {
        if(!globals.manRecessEnabled)
        {
            // If a level is selected, then we can continue,
            // Also make sure it isn't a level selected from the history panel
            if (player->levelRequest && player->levelRequest->second.rating < 0)
            {
                engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
                
                if ((player->choice1RestartCounter < player->numRetries) && (player->marbleChoice == 1))
                {
                    player->choice1RestartCounter++;
                }
                else if ((player->choice2RestartCounter < player->numRetries) && (player->marbleChoice == 2))
                {
                    player->choice2RestartCounter++;
                }
                else if ((player->choice3RestartCounter < player->numRetries) && (player->marbleChoice == 3))
                {
                    player->choice3RestartCounter++;
                }
            }
        }
        else
        {
            if(globals.manRecessCount != globals.manRecessLevelLimit)
            {
                // If a level is selected, then we can continue,
                // Also make sure it isn't a level selected from the history panel
                if (player->levelRequest && player->levelRequest->second.rating < 0)
                {
                    engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
                    
                    if ((player->choice1RestartCounter < player->numRetries) && (player->marbleChoice == 1))
                    {
                        player->choice1RestartCounter++;
                    }
                    else if ((player->choice2RestartCounter < player->numRetries) && (player->marbleChoice == 2))
                    {
                        player->choice2RestartCounter++;
                    }
                    else if ((player->choice3RestartCounter < player->numRetries) && (player->marbleChoice == 3))
                    {
                        player->choice3RestartCounter++;
                    }
                }
            }
            
        }
        
        
    }
    else if (queryGUI == "reroll")
    {
        if(!globals.manRecessEnabled)
        {
            if (player->rerollCounter > 0)
            {
                player->feedLevelRequestFromSchedule();
                player->rerollCounter--;
                player->saveProgress(globals.savePath);
            }
        }
        else //manRecessEnabled
        {
            if(globals.manRecessCount != globals.manRecessLevelLimit)
            {
                if (player->rerollCounter > 0)
                {
                    player->feedLevelRequestFromSchedule();
                    player->rerollCounter--;
                    player->saveProgress(globals.savePath);
                }
            }
        }
    }
}

void EngineSchedulerMenu::activateReturnFromPopup()
{
    // Fortunately, the only tutorial popup for the scheduler menu is
    // the session over window popup. So we can use this function to handle
    // that.
    engineStateMgr->requestPopEngine();
    player->sessionStarted = false;
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
        
        if(!globals.manRecessEnabled)
        {
            player->marbleChoice = 1;
            player->levelRequest = &player->scheduleChoice1;
            return true;
        }
        else
        {
            if(globals.manRecessCount != globals.manRecessLevelLimit)
            {
                player->marbleChoice = 1;
                player->levelRequest = &player->scheduleChoice1;
                return true;
            }
        }
    }
    else if (queryGUI == "selection1")
    {
        if(!globals.manRecessEnabled)
        {
            player->marbleChoice = 2;
            player->levelRequest = &player->scheduleChoice2;
            return true;
        }
        else
        {
            if(globals.manRecessCount != globals.manRecessLevelLimit)
            {
                player->marbleChoice = 2;
                player->levelRequest = &player->scheduleChoice2;
                return true;
            }
        }
        
    }
    else if (queryGUI == "selection2")
    {
        if(!globals.manRecessEnabled)
        {
            player->marbleChoice = 3;
            player->levelRequest = &player->scheduleChoice3;
            return true;
        }
        else
        {
            if(globals.manRecessCount != globals.manRecessLevelLimit)
            {
                player->marbleChoice = 3;
                player->levelRequest = &player->scheduleChoice3;
                return true;
            }
        }
        
    }
    else if (queryGUI.substr(0, 7) == "history")
    {
        if(!globals.manRecessEnabled)
        {
            int iconNo = std::atoi(queryGUI.substr(7, 1).c_str());
            
            switch (iconNo)
            {
                case 0:
                    if (player->scheduler->scheduleHistoryA.size() > 0)
                    {
                        player->levelRequest = &player->scheduler->scheduleHistoryA.back();
                    }
                    else
                        player->levelRequest = NULL;
                    break;
                case 1:
                    if (player->scheduler->scheduleHistoryB.size() > 0)
                    {
                        player->levelRequest = &player->scheduler->scheduleHistoryB.back();
                    }
                    else
                        player->levelRequest = NULL;
                    break;
                case 2:
                    if (player->scheduler->scheduleHistoryC.size() > 0)
                    {
                        player->levelRequest = &player->scheduler->scheduleHistoryC.back();
                    }
                    else
                        player->levelRequest = NULL;
                    break;
                case 3:
                    if (player->scheduler->scheduleHistoryD.size() > 0)
                    {
                        player->levelRequest = &player->scheduler->scheduleHistoryD.back();
                    }
                    else
                        player->levelRequest = NULL;
                    break;
                case 4:
                    if (player->scheduler->scheduleHistoryE.size() > 0)
                    {
                        player->levelRequest = &player->scheduler->scheduleHistoryE.back();
                    }
                    else
                        player->levelRequest = NULL;
                    break;
            }
            return true;
        }
        else
        {
            if(globals.manRecessCount != globals.manRecessLevelLimit)
            {
                int iconNo = std::atoi(queryGUI.substr(7, 1).c_str());
                
                switch (iconNo)
                {
                    case 0:
                        if (player->scheduler->scheduleHistoryA.size() > 0)
                        {
                            player->levelRequest = &player->scheduler->scheduleHistoryA.back();
                        }
                        else
                            player->levelRequest = NULL;
                        break;
                    case 1:
                        if (player->scheduler->scheduleHistoryB.size() > 0)
                        {
                            player->levelRequest = &player->scheduler->scheduleHistoryB.back();
                        }
                        else
                            player->levelRequest = NULL;
                        break;
                    case 2:
                        if (player->scheduler->scheduleHistoryC.size() > 0)
                        {
                            player->levelRequest = &player->scheduler->scheduleHistoryC.back();
                        }
                        else
                            player->levelRequest = NULL;
                        break;
                    case 3:
                        if (player->scheduler->scheduleHistoryD.size() > 0)
                        {
                            player->levelRequest = &player->scheduler->scheduleHistoryD.back();
                        }
                        else
                            player->levelRequest = NULL;
                        break;
                    case 4:
                        if (player->scheduler->scheduleHistoryE.size() > 0)
                        {
                            player->levelRequest = &player->scheduler->scheduleHistoryE.back();
                        }
                        else
                            player->levelRequest = NULL;
                        break;
                }
                return true;
            }
        }
        
    }
    return false;
}
