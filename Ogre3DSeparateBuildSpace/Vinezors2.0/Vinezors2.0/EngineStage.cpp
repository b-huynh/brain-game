//
//  EngineStage.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/16/14.
//
//

#include "EngineStage.h"
#include "EngineStateManager.h"
#include "Networking.h"
#include "Tunnel.h"
#include "Player.h"
#include "LevelSet.h"

extern Util::ConfigGlobal globals;

EngineStage::EngineStage(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->lightNode = NULL;
    this->tunnel = NULL;
    this->player = player;
    this->hud = NULL;
}

EngineStage::~EngineStage()
{
}

void EngineStage::enter()
{
    // We may assume we are always at the top of the stack
    stageState = STAGE_STATE_INIT;
}

void EngineStage::exit()
{
    OgreFramework::getSingletonPtr()->m_pSoundMgr->resumeAllPausedSounds();
    OgreFramework::getSingletonPtr()->m_pSoundMgr->stopAllSounds();
    dealloc();
}

void EngineStage::update(float elapsed)
{
    OgreFramework::getSingletonPtr()->m_pSoundMgr->update(elapsed);
    bool replayableTutorial = (player->choice0RestartCounter < player->numRetries && player->marbleChoice == 0);
    bool replayableMarble = (player->choice1RestartCounter < player->numRetries && player->marbleChoice == 1) ||
                            (player->choice2RestartCounter < player->numRetries && player->marbleChoice == 2) ||
                            (player->choice3RestartCounter < player->numRetries && player->marbleChoice == 3);
    
    switch (stageState)
    {
        case STAGE_STATE_INIT:
        {
            //std::cout<<"HERE"<<std::endl;
            setup();
            //std::cout<<"HERE2"<<std::endl;
            setPause(true);
            stageState = STAGE_STATE_PAUSE;
            
            globals.setBigMessage("");
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, true);
            hud->setOverlay(2, false);
            hud->setGoButtonState(false);
            hud->setSpeedDialState(true);
            hud->setPauseNavSettings(!tunnel->needsCleaning(),
                                     (player->levelRequest && player->levelRequest->second.rating >= 0) || (!player->levelRequest && player->isNextLevelAvailable()),
                                     (player->levelRequest && player->levelRequest->second.rating < 0 && (player->marbleChoice <= 0 || replayableMarble)) || (!player->levelRequest),
                                     (player->levelRequest && (replayableTutorial || replayableMarble)) || (!player->levelRequest));
            hud->setPauseNavDest(0.7);
            
            //OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
            break;
        }
        case STAGE_STATE_RUNNING:
        {
#if defined(OGRE_IS_IOS)
            // Update the game state
            updateSpin(elapsed);
#endif
            tunnel->update(elapsed);
            
            if (tunnel->needsCleaning())
            {
                completeStage(EVEN);
                stageState = STAGE_STATE_PROMPT;
                setPause(true);
                
                std::string finalScoreMessage = Util::toStringInt(player->getScore());
                globals.appendMessage("\nScore " + finalScoreMessage + "\n\nPlay Again?", MESSAGE_NORMAL);
            }
            
            globals.setBigMessage("");
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setGoButtonState(false);
            hud->setPauseNavSettings(!tunnel->needsCleaning(),
                                     (player->levelRequest && player->levelRequest->second.rating >= 0) || (!player->levelRequest && player->isNextLevelAvailable()),
                                     (player->levelRequest && player->levelRequest->second.rating < 0 && (player->marbleChoice <= 0 || replayableMarble)) || (!player->levelRequest),
                                     (player->levelRequest && (replayableTutorial || replayableMarble)) || (!player->levelRequest));
            hud->setPauseNavDest(0.7);
            
            // Graphical view changes from camera, light, and skybox
            Quaternion camRot = player->getCombinedRotAndRoll();
            OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
            OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(camRot);
            if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->setOrientation(player->getCombinedRotAndRoll());
            if (lightNode)
            {
                lightNode->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
                lightMain->setDirection(Vector3::UNIT_Y * -1);
            }
            
            //OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
            
            // See by the end of this frame if we have a tutorial popup, if so pause the game, keep the music running
            if (player->getTutorialMgr()->isVisible() && !player->getTutorialMgr()->isSpecial())
            {
                setPause(true, false);
                globals.appendMessage("\n\nPaused", MESSAGE_NORMAL);
                stageState = STAGE_STATE_PAUSE;
            }
            
            break;
        }
        case STAGE_STATE_PAUSE:
        {
            // Navigation Debug Keys
            if (player->getKeyUp())
                player->move(Vector3(player->getCamForward() * globals.initCamSpeed * elapsed));
            if (player->getKeyDown())
                player->move(Vector3(player->getCamForward() * -globals.initCamSpeed * elapsed));
            if (player->getKeyLeft())
                player->move(Vector3(player->getCamRight() * -globals.initCamSpeed * elapsed));
            if (player->getKeyRight())
                player->move(Vector3(player->getCamRight() * globals.initCamSpeed * elapsed));
            
            OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
            //OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
            
            if (!player->hasTriggeredStartup())
            {
                int sliderSpeed = hud->getSpeedSlider()->getIndex();
                globals.initCamSpeed = Util::clamp(sliderSpeed, globals.minCamSpeed, globals.maxCamSpeed);
                player->setSpeedParameters(globals.initCamSpeed, globals.minCamSpeed, globals.maxCamSpeed);
                
                // Save them results for people who replay
                player->saveSpeedSettings();
            }
            
            globals.setBigMessage("");
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setGoButtonState(true, true);
            hud->setPauseNavSettings(!tunnel->needsCleaning(),
                                     (player->levelRequest && player->levelRequest->second.rating >= 0) || (!player->levelRequest && player->isNextLevelAvailable()),
                                     (player->levelRequest && player->levelRequest->second.rating < 0 && (player->marbleChoice <= 0 || replayableMarble)) || (!player->levelRequest),
                                     (player->levelRequest && (replayableTutorial || replayableMarble)) || (!player->levelRequest));
            hud->setPauseNavDest(0.7);
            break;
        }
        case STAGE_STATE_PROMPT:
        {
            //OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
            
            globals.setBigMessage("");
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, true);
            hud->setGoButtonState(false);
            hud->setPauseNavSettings(!tunnel->needsCleaning(),
                                     (player->levelRequest && player->levelRequest->second.rating >= 0) || (!player->levelRequest && player->isNextLevelAvailable()),
                                     (player->levelRequest && player->levelRequest->second.rating < 0 && (player->marbleChoice <= 0 || replayableMarble)) || (!player->levelRequest),
                                     (player->levelRequest && (replayableTutorial || replayableMarble)) || (!player->levelRequest));
            hud->setPauseNavDest(0.0);
            break;
        }
        case STAGE_STATE_READY:
        {
            //OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
            
            globals.clearMessage();
            readyTimer -= elapsed;
            readyTimer = Util::clamp(readyTimer, 0.0, 3.0);
            if (readyTimer <= 0.0)
            {
                stageState = STAGE_STATE_RUNNING;
                setPause(false);
            }
            else if (readyTimer < 0.75)
                globals.setBigMessage("GO");
            else if (readyTimer < 1.50)
                globals.setBigMessage("1");
            else if (readyTimer < 2.25)
                globals.setBigMessage("2");
            else
                globals.setBigMessage("3");
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setGoButtonState(false);
            hud->setPauseNavSettings(!tunnel->needsCleaning(),
                                     (player->levelRequest && player->levelRequest->second.rating >= 0) || (!player->levelRequest && player->isNextLevelAvailable()),
                                     (player->levelRequest && player->levelRequest->second.rating < 0 && (player->marbleChoice <= 0 || replayableMarble)) || (!player->levelRequest),
                                     (player->levelRequest && (replayableTutorial || replayableMarble)) || (!player->levelRequest));
            hud->setPauseNavDest(0.7);
            hud->setSpeedDialState(false);
            break;
        }
        case STAGE_STATE_DONE:
        {
            // have a done screen after a certain time limit is reached
            if ( player->getTotalElapsed() >= globals.sessionTime )
                player->scheduler->sessionFinished = true;
            
            std::cout << "\n\n-----------------------------------------\n";
            std::cout << "Game Time Played: " << player->getTotalElapsed() << std::endl;
            std::cout << "-----------------------------------------\n\n";
            
            if(player->scheduler->sessionFinished && !player->getTutorialMgr()->hasVisitedSlide(TutorialManager::TUTORIAL_END_OF_SESSION))
            {
                // If player has not visited end of session slide yet, set it up
                std::cout << "\n\n\n===========================================\n"
                << "Session Finished!\n"
                << "===========================================\n";
                player->getTutorialMgr()->prepareSlides(TutorialManager::TUTORIAL_END_OF_SESSION, 0.0);
                //player->getTutorialMgr()->setAdditionalText(player->getSessionStats());   // For the study
                
                // Update session ID before save
                player->setSessionID(player->getSessionID() + 1);
            }
            
            // scheduler grading done in here
            // also need to save nback levels after finishing a level
            if (player->levelRequest && player->levelRequest->second.rating >= 0)
            {
                
                player->assessLevelPerformance(player->levelRequest);
                if (player->scheduler->sessionFinished) {
                    std::cout << "finished!\n";
                }
                else
                {
                    std::cout << "not finished!\n";
                }
                player->saveProgress(globals.savePath);
                player->lastPlayed = player->levelRequest->first.phaseX;
                player->levelRequest = NULL;    // Reset selection and avoid saving twice on next update frame
                
                // Grab new choices for player to choose from
                player->feedLevelRequestFromSchedule();
                
                
                
                //Reset Restart Counters:
                
                player->choice0RestartCounter = 0;
                player->choice1RestartCounter = 0;
                player->choice2RestartCounter = 0;
                player->choice3RestartCounter = 0;
            }
            
            // Unpause Settings but without the sound deactivating
            engineStateMgr->requestPopEngine();
            break;
        }
    }
}

float normalizedAngle(float theta)
{
    while (theta > 360)
        theta -= 360;
    while (theta < 0)
        theta += 360;
    return theta;
}

Direction getDirByRoll(float roll)
{
    while (roll > 360)
        roll -= 360;
    
    while (roll < 0)
        roll += 360;
    
    if (roll <= 22.5) return SOUTH;
    if (roll <= 67.5) return SOUTHWEST;
    if (roll <= 112.5) return WEST;
    if (roll <= 157.5) return NORTHWEST;
    if (roll <= 202.5) return NORTH;
    if (roll <= 247.5) return NORTHEAST;
    if (roll <= 292.5) return EAST;
    if (roll <= 337.5) return SOUTHEAST;
    if (roll <= 360.0) return SOUTH;
    return NO_DIRECTION;
}

bool isPathable(SectionInfo info, float roll)
{
    while (roll > 360)
        roll -= 360;
    
    while (roll < 0)
        roll += 360;
    
    if (roll < 45)
        return info.sidesUsed[SOUTH] && info.sidesUsed[SOUTHEAST];
    else if (roll < 90)
        return info.sidesUsed[SOUTHEAST] && info.sidesUsed[EAST];
    else if (roll < 135)
        return info.sidesUsed[EAST] && info.sidesUsed[NORTHEAST];
    else if (roll < 180)
        return info.sidesUsed[NORTHEAST] && info.sidesUsed[NORTH];
    else if (roll < 225)
        return info.sidesUsed[NORTH] && info.sidesUsed[NORTHWEST];
    else if (roll < 270)
        return info.sidesUsed[NORTHWEST] && info.sidesUsed[WEST];
    else if (roll < 315)
        return info.sidesUsed[WEST] && info.sidesUsed[SOUTHWEST];
    else if (roll < 360)
        return info.sidesUsed[SOUTHWEST] && info.sidesUsed[SOUTH];
    
    return false;
}

TunnelSlice* closestUnpathable(Tunnel* tunnel, int numSegments, float roll, int & depthDist)
{
    std::vector<TunnelSlice*> nextset = tunnel->getNSlices(3);
    depthDist = 0;
    for (; depthDist < nextset.size(); ++depthDist)
    {
        SectionInfo info = nextset[depthDist]->getSectionInfo();
        if (!isPathable(info, roll))
        {
            return nextset[depthDist];
        }
    }
    return NULL;
}

void EngineStage::activatePerformLeftMove()
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
        {
            if (!tunnel->isDone() && player->setVineDirRequest(Util::rightOf(player->getVineDest())))
            {
                float val = player->getDesireRoll();
                player->setDesireRoll(val + 45);
            }
            break;
        }
        case STAGE_STATE_PAUSE:
            break;
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformLeftMove(float angle)
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
        {
            if (!tunnel->isDone())// && player->setVineDirRequest(Util::rightOf(player->getVineDest())))
            {
                SectionInfo info = tunnel->getCurrent()->getSectionInfo();
                TunnelSlice* next = tunnel->getNext(1);
                
                // Perform Player Movement
                float val = player->getCamRoll();
                float nval = val + angle;
                if (isPathable(info, nval))
                    player->setCamRoll(normalizedAngle(nval));
                else
                {
                    activateVelocity(0.0);
                    // resolve overshooting by setting the player directly on the panel
                    float discreteDegrees = Util::getDegrees(getDirByRoll(val));
                    if (isPathable(info, discreteDegrees - 1)) // border case call, so shift it over a little bit
                    {
                        player->setCamRoll(discreteDegrees); // resolve overshooting
                    }
                }
            }
            break;
        }
        case STAGE_STATE_PAUSE:
            break;
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformRightMove()
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
        {
            if (!tunnel->isDone() && player->setVineDirRequest(Util::leftOf(player->getVineDest())))
            {
                float val = player->getDesireRoll();
                player->setDesireRoll(val - 45);
            }
            break;
        }
        case STAGE_STATE_PAUSE:
        {
            //stageState = STAGE_STATE_RUNNING;
            //setPause(false);
            break;
        }
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformRightMove(float angle)
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
        {
            if (!tunnel->isDone())
            {
                SectionInfo info = tunnel->getCurrent()->getSectionInfo();
                TunnelSlice* next = tunnel->getNext(1);
                
                // Perform Player Movement
                float val = player->getCamRoll();
                float nval = val - angle;
                if (isPathable(info, nval))
                    player->setCamRoll(normalizedAngle(nval));
                else
                {
                    activateVelocity(0.0);
                    // resolve overshooting by setting the player directly on the panel
                    float discreteDegrees = Util::getDegrees(getDirByRoll(val));
                    if (isPathable(info, discreteDegrees + 1)) //border case call, so shift it over a little bit
                    {
                        // Subtract 1 due to discrete angle values
                        player->setCamRoll(discreteDegrees); // resolve overshooting
                    }
                }
            }
            break;
        }
        case STAGE_STATE_PAUSE:
        {
            //stageState = STAGE_STATE_RUNNING;
            //setPause(false);
            break;
        }
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformSwipeUp()
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
            break;
        case STAGE_STATE_PAUSE:
        {
            globals.initCamSpeed = Util::clamp(globals.initCamSpeed + 1, globals.minCamSpeed, globals.maxCamSpeed);
            player->setSpeedParameters(globals.initCamSpeed, globals.minCamSpeed, globals.maxCamSpeed);
            break;
        }
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformSwipeDown()
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
            break;
        case STAGE_STATE_PAUSE:
        {
            globals.initCamSpeed = Util::clamp(globals.initCamSpeed - 1, globals.minCamSpeed, globals.maxCamSpeed);
            player->setSpeedParameters(globals.initCamSpeed, globals.minCamSpeed, globals.maxCamSpeed);
            break;
        }
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformDoubleTap(float x, float y)
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
            //player->performBoost();
            break;
        case STAGE_STATE_PAUSE:
            break;
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformSingleTap(float x, float y)
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
        {
            std::string queryGUI = hud->queryButtons(Vector2(x, y));
            if (queryGUI == "pause")
            {
                
                if (!player->endFlag)
                {
                    hud->setSpeedDialState(false);
                    
                    setPause(true);
                    player->reactGUI();
                    stageState = STAGE_STATE_PROMPT;
                }
            }
            else if (queryGUI == "leftzap" && tunnel->getMode() != STAGE_MODE_RECESS)
            {
                hud->leftZapT = 0.1;
                // Perform Zap
                player->setPowerUp("TractorBeam", true);
                player->performPowerUp("TractorBeam");
            }
            else if (queryGUI == "rightzap" && tunnel->getMode() != STAGE_MODE_RECESS)
            {
                hud->rightZapT = 0.1;
                // Perform Zap
                player->setPowerUp("TractorBeam", true);
                player->performPowerUp("TractorBeam");
            }
            else
            {
                std::cout << x << "," << y << std::endl;
                if (tunnel && tunnel->getMode() != STAGE_MODE_RECESS)
                {
                    // Tap we are checking if it is inside circle
                    Vector2 relTap = globals.convertToPercentScreen(Vector2(x, y));
                    // Circle Center
                    float relRadius = 0.15;
                    Vector2 relCenter = Vector2(0.50, 0.75);
                    // Distance between tap and circle center
                    Vector2 relDist = relTap - relCenter;
                    if (relDist.squaredLength() <= relRadius * relRadius)
                    {
                        // Perform Zap
                        player->setPowerUp("TractorBeam", true);
                        player->performPowerUp("TractorBeam");
                    }
                }
            }
            break;
        }
        case STAGE_STATE_PAUSE:
        {
            std::string queryGUI = hud->queryButtons(Vector2(x, y));
            
            if (queryGUI == "go")
            {
                stageState = STAGE_STATE_READY;
                readyTimer = 3.00f;
                player->reactGUI();
            }
            else if (queryGUI == "pause")
            {
                hud->setSpeedDialState(false);
                
                // Display current level index
                LevelSet* levels = player->getLevels();
                std::string msg = "Level: ";
                msg += Util::toStringInt(player->getLevelRequestRow() + 1);
                msg += "-";
                msg += (char)('A' + player->getLevelRequestCol());
                globals.setMessage(msg, MESSAGE_NORMAL);
                
                setPause(true);
                player->reactGUI();
                stageState = STAGE_STATE_PROMPT;
            }
            else
            {
                // Allows single tap on dial without dragging slider to be a way to verify
                HudSlider* speedSlider = NULL;
                if (hud) speedSlider = hud->getSpeedSlider();
            }
            break;
        }
        case STAGE_STATE_PROMPT:
        {
            std::string queryGUI = hud->queryButtons(Vector2(x, y));
            
            if (queryGUI == "resume")
            {
                // If game hasn't started yet, go back to init prompt
                // Otherwise, go to gameplay
                if (!player->hasTriggeredStartup())
                {
                    hud->setSpeedDialState(true);
                    stageState = STAGE_STATE_PAUSE;
                    if (player->getStartMusicTimer() <= 0.0)
                        player->playMusic();
                }
                else if (!tunnel->needsCleaning())
                {
                    stageState = STAGE_STATE_READY;
                    if (!tunnel->isDone())
                        readyTimer = 3.00f;
                    else
                        readyTimer = 0.0f;
                }
                player->reactGUI();
            }
            else if (queryGUI == "next")
            {
                // If we came from the level select menu, extract the next level from that 2D grid.
                // Otherwise, we came from a different set of levels
                if (engineStateMgr->peek(1)->getEngineType() == ENGINE_LEVEL_SELECTION)
                {
                    if (player->isNextLevelAvailable())
                    {
                        LevelSet* levels = player->getLevels();
                        int nlevel = player->getNextLevel();
                        int row = levels->getLevelRow(nlevel);
                        int col = levels->getLevelCol(nlevel);
                        player->setLevelRequest(row, col);
                        stageState = STAGE_STATE_INIT;
                        
                        setPause(false, false);
                    }
                    player->reactGUI();
                }
                else
                {
                    if (player->levelRequest && player->levelRequest->second.rating >= 0)
                    {
                        stageState = STAGE_STATE_DONE;
                        
                        setPause(false, false);
                    }
                    player->reactGUI();
                }
            }
            else if (queryGUI == "restart")
            {
                player->reactGUI();
               
                if (!player->levelRequest) // If not a scheduler level
                {
                    stageState = STAGE_STATE_INIT;
                    
                    player->logData();
                    
                    setPause(false, false);
                }
                else if((player->choice0RestartCounter < player->numRetries) && (player->marbleChoice == 0))
                {
                    
                    player->choice0RestartCounter++;
                    stageState = STAGE_STATE_INIT;
                    
                    player->logData();
                    
                    setPause(false, false);
                    
                }
                else if((player->choice1RestartCounter < player->numRetries) && (player->marbleChoice == 1))
                {
                    
                    player->choice1RestartCounter++;
                    stageState = STAGE_STATE_INIT;
                    
                    player->logData();
                    
                    setPause(false, false);
                }
                else if((player->choice2RestartCounter < player->numRetries) && (player->marbleChoice == 2))
                {
                    
                    player->choice2RestartCounter++;
                    stageState = STAGE_STATE_INIT;
                    
                    player->logData();
                    
                    setPause(false, false);
                }
                else if((player->choice3RestartCounter < player->numRetries) && (player->marbleChoice == 3))
                {
                    
                    player->choice3RestartCounter++;
                    stageState = STAGE_STATE_INIT;
                    
                    player->logData();
                    
                    setPause(false, false);
                }
                
            }
            else if (queryGUI == "levelselect")
            {
                
                player->reactGUI();
                
                if (!player->levelRequest) // If not a scheduler level
                {
                    stageState = STAGE_STATE_DONE;
                    
                    player->logData();
                    
                    setPause(false, false);
                }
                else if((player->choice1RestartCounter < player->numRetries) && (player->marbleChoice == 1))
                {
                    if ((player->levelRequest && player->levelRequest->second.rating < 0) || // For scheduler
                        (!player->levelRequest))    // For level from level select
                    {
                        stageState = STAGE_STATE_DONE;
                        
                        player->logData();
                        
                        setPause(false, false);
                    }
                    
                }
                else if((player->choice2RestartCounter < player->numRetries) && (player->marbleChoice == 2))
                {
                    if ((player->levelRequest && player->levelRequest->second.rating < 0) || // For scheduler
                        (!player->levelRequest))    // For level from level select
                    {
                        stageState = STAGE_STATE_DONE;
                        
                        player->logData();
                        
                        setPause(false, false);
                    }
                    
                }
                
                else if((player->choice3RestartCounter < player->numRetries) && (player->marbleChoice == 3))
                {
                    if ((player->levelRequest && player->levelRequest->second.rating < 0) || // For scheduler
                        (!player->levelRequest))    // For level from level select
                    {
                        stageState = STAGE_STATE_DONE;
                        
                        player->logData();
                        
                        setPause(false, false);
                    }
                    
                }
                else if(player->marbleChoice == 0)
                {
                    if ((player->levelRequest && player->levelRequest->second.rating < 0) || // For scheduler
                        (!player->levelRequest))    // For level from level select
                    {
                        stageState = STAGE_STATE_DONE;
                        
                        player->logData();
                        
                        setPause(false, false);
                    }
                }
                
            }
            break;
        }
        case STAGE_STATE_READY:
        {
            std::string queryGUI = hud->queryButtons(Vector2(x, y));
            
            if (queryGUI == "pause")
            {
                hud->setSpeedDialState(false);
                
                // Display current level index
                LevelSet* levels = player->getLevels();
                std::string msg = "Level: ";
                msg += Util::toStringInt(player->getLevelRequestRow() + 1);
                msg += "-";
                msg += (char)('A' + player->getLevelRequestCol());
                globals.setMessage(msg, MESSAGE_NORMAL);
                
                setPause(true);
                player->reactGUI();
                stageState = STAGE_STATE_PROMPT;
            }
            else
            {
                if (readyTimer < 0.75)
                    readyTimer = 0.0;
                else if (readyTimer < 1.50)
                    readyTimer = 0.75;
                else if (readyTimer < 2.25)
                    readyTimer = 1.50;
                else
                    readyTimer = 2.25;
            }
            break;
        }
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformPinch()
{
#ifdef DEBUG_MODE
#if defined(OGRE_IS_IOS)
    if (stageState == STAGE_STATE_RUNNING)
    {
        player->setGodMode(!player->getGodMode());
        std::cout << "God Mode: " << player->getGodMode() << std::endl;
        
        //tunnel->setDone(PASS);
    }
    else
#endif
    {
        std::cout << "God Mode: " << player->getGodMode() << std::endl;
        player->setGodMode(!player->getGodMode());
        if (tunnel) tunnel->respondToToggleCheat();
    }
#endif
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
            break;
        case STAGE_STATE_PAUSE:
            break;
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformBeginLongPress()
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
        {
            // In single tap now
            //player->performBoost();
            break;
        }
        case STAGE_STATE_PAUSE:
            break;
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformEndLongPress()
{
    switch (stageState)
    {
        case STAGE_STATE_INIT:
            break;
        case STAGE_STATE_RUNNING:
            break;
        case STAGE_STATE_PAUSE:
            break;
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_READY:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

// The following deal with injecting coordinates to simulate a slider
//
void EngineStage::activateMoved(float x, float y, float dx, float dy)
{
    if (stageState == STAGE_STATE_PAUSE && !player->hasTriggeredStartup())
    {
        HudSlider* speedSlider = NULL;
        if (hud) speedSlider = hud->getSpeedSlider();
        if (speedSlider && speedSlider->selected)
        {
            speedSlider->activateMoved(x, y, dx, dy);
        }
    }
}

void EngineStage::activatePressed(float x, float y)
{
    if (stageState == STAGE_STATE_PAUSE && !player->hasTriggeredStartup())
    {
        HudSlider* speedSlider = NULL;
        if (hud) speedSlider = hud->getSpeedSlider();
        if (speedSlider)
        {
            speedSlider->activatePressed(x, y);
        }
    }
}

void EngineStage::activateReleased(float x, float y, float dx, float dy)
{
    if (stageState == STAGE_STATE_PAUSE && !player->hasTriggeredStartup())
    {
        HudSlider* speedSlider = NULL;
        if (hud) speedSlider = hud->getSpeedSlider();
        if (speedSlider && speedSlider->selected)
        {
            speedSlider->activateReleased(x, y, dx, dy);
        }
    }
}

#define NUM_ANGLES_SAVED 3
#define ANGLE_LOOKBACK 0

void EngineStage::activateVelocity(float vel)
{
    //std::cout << "SPIN VELOCITY: " << vel << std::endl;
    //if (vel != 0.0) this->spinVelocity = abs(vel);

    dampingDecay = dampingDecayFree;
    dampingDrop = dampingDropFree;
    
    if (abs(vel) <= minVelFree)
        vel = 0.0;
    spinVelocityTarget = abs(vel);
    
    freeMotion = true;
    if (vel < 0.0)
        spinClockwise = false;
    else if (vel > 0.0)
        spinClockwise = true;
    else
    {
        // 0.0 vel means force stop in this algorithm
        // So turn off everything! Make damping so high you scream
        
        // This'll eventually turn off by damping in update...
        //freeMotion = false;
        //spinVelocity = 0.0;
        
        spinVelocityTarget = 0.0;
        dampingDecay = dampingDecayStop;
        dampingDrop = dampingDropStop;
        
        /*
        if (lastAngles.size() >= NUM_ANGLES_SAVED) {
            player->setCamRoll(lastAngles[ANGLE_LOOKBACK]);
            std::cerr << "Using last angle" << std::endl;
        }
         */
    }
    
    if (abs(spinVelocityTarget) >= maxVel)
        spinVelocityTarget = maxVel;
}

void EngineStage::activateAngleTurn(float angle, float vel)
{
    //std::cout << "ANGLE: " << angle << std::endl;
    
    if (tunnel && !tunnel->isDone())
    {
        //Convert to degrees;
        double dT = (angle * 180.0) / Ogre::Math::PI;
        float roll = player->getCamRoll();
        
        // If unpathable upahead, don't allow player to traverse through
        int depthDist = 0;
        if (!player->inverted)
            dT = -dT;
        TunnelSlice* unpathable = closestUnpathable(tunnel, 3, roll + dT, depthDist);
        if (!unpathable)
        {
            player->setCamRoll(roll + dT);
            player->offsetRollDest = dT;
        }
    }
}

void EngineStage::activateReturnFromPopup()
{
    if (player->endFlag)
    {
        tunnel->setCleaning(true);
    }
    // To prevent the game from immediately resume without prepping the user
    // We throw them into a countdown before resuming
    else if (stageState == STAGE_STATE_RUNNING)
    {
        // Save camera state to avoid odd camera jump when unpausing
        player->pause();
        stageState = STAGE_STATE_READY;
        readyTimer = 3.00f;
    }
    else if (stageState == STAGE_STATE_PAUSE)
    {   
        // Turn on speed dial after they viewed tutorials
        setTaskPrompt();
        hud->setSpeedDialState(true);
    }
}

#if !defined(OGRE_IS_IOS)
void EngineStage::mouseMoved(const OIS::MouseEvent &evt)
{
    if (stageState == STAGE_STATE_PAUSE)
    {
        // Move the camera when paused
        Vector2 dmove = Vector2(evt.state.X.rel, evt.state.Y.rel);
        
        Vector3 right = player->getCamRight(true);
        Vector3 up = player->getCamUpward(true);
        Quaternion yawRot;
        Quaternion pitchRot;
        yawRot.FromAngleAxis(Degree(-dmove.x) / 4, up);
        pitchRot.FromAngleAxis(Degree(-dmove.y) / 4, right);
        
        Quaternion curRot = player->getCamRot();
        curRot = pitchRot * yawRot * curRot;
        player->setCamRot(curRot);
        curRot = player->getCombinedRotAndRoll();
        OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(curRot);
        
        player->setMousePos(Vector2(evt.state.X.abs, evt.state.Y.abs));
    }
}

void EngineStage::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            player->setMouseLeft(true);
            activatePerformSingleTap(evt.state.X.abs, evt.state.Y.abs);
            break;
        default:
            break;
    }
}

void EngineStage::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            player->setMouseLeft(false);
            break;
        default:
            break;
    }
}

void EngineStage::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    switch (keyEventRef.key)
    {
        case OIS::KC_W:
        {
            player->setKeyUp(true);
            break;
        }
        case OIS::KC_A:
        {
            player->setKeyLeft(true);
            break;
        }
        case OIS::KC_S:
        {
            player->setKeyDown(true);
            break;
        }
        case OIS::KC_D:
        {
            player->setKeyRight(true);
            break;
        }
        case OIS::KC_SPACE:
        {
            player->setKeySpace(true);
            break;
        }
        case OIS::KC_LEFT:
        {
            activatePerformLeftMove();
            break;
        }
        case OIS::KC_RIGHT:
        {
            activatePerformRightMove();
            break;
        }
        case OIS::KC_UP:
        {
            activatePerformSwipeUp();
            break;
        }
        case OIS::KC_DOWN:
        {
            activatePerformSwipeDown();
            break;
        }
        case OIS::KC_P:
        {
            if (stageState == STAGE_STATE_RUNNING || stageState == STAGE_STATE_READY)
            {
                setPause(true);
                globals.appendMessage("\n\nPaused", MESSAGE_NORMAL);
                stageState = STAGE_STATE_PAUSE;
            }
            else
            {
                stageState = STAGE_STATE_READY;
                readyTimer = 3.00f;
            }
            break;
        }
        case OIS::KC_Z:
        {
            if (stageState == STAGE_STATE_RUNNING)
                player->performBoost();
            break;
        }
        case OIS::KC_X:
        {
            if (stageState == STAGE_STATE_RUNNING &&
                tunnel->getMode() != STAGE_MODE_RECESS)
            {
                player->setPowerUp("TractorBeam", true);
                player->performPowerUp("TractorBeam");
            }
            break;
        }
        case OIS::KC_C:
        {
            player->setPowerUp("TimeWarp", true);
            if (stageState == STAGE_STATE_RUNNING) player->performPowerUp("TimeWarp");
            break;
        }
        case OIS::KC_V:
        {
            if (stageState == STAGE_STATE_RUNNING) player->performPowerUp("Shields");
            break;
        }
#ifdef DEBUG_MODE
        case OIS::KC_K:
        {
            if (stageState == STAGE_STATE_RUNNING) tunnel->setDone(FAIL);
            break;
        }
        case OIS::KC_L:
        {
            if (stageState == STAGE_STATE_RUNNING) tunnel->setDone(PASS);
            break;
        }
        case OIS::KC_G:
        {
            player->setGodMode(!player->getGodMode());
            std::cout << "God Mode: " << player->getGodMode() << std::endl;
            break;
        }
#endif
        default:
            break;
    }
}

void EngineStage::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    switch (keyEventRef.key)
    {
        case OIS::KC_W:
        {
            player->setKeyUp(false);
            break;
        }
        case OIS::KC_A:
        {
            player->setKeyLeft(false);
            break;
        }
        case OIS::KC_S:
        {
            player->setKeyDown(false);
            break;
        }
        case OIS::KC_D:
        {
            player->setKeyRight(false);
            break;
        }
        case OIS::KC_SPACE:
        {
            player->setKeySpace(false);
            break;
        }
        default:
            break;
    }
}
#endif

void EngineStage::requestResize()
{
    if (hud) hud->adjust();
}

void EngineStage::setup()
{
    globals.clearMessage();
    
    // Dealloc data
    dealloc();
    
    // Retrieve player skill and gen new tunnel
    PlayerLevel skillLevel = player->getSkillLevel();
    
    // Setup player and tunnel params
    Vector3 origin = Vector3::ZERO;
    Vector3 forward = globals.tunnelReferenceForward;
    Quaternion rot = Quaternion(1, 0, 0, 0);
    
    StageMode nmode = STAGE_MODE_PROFICIENCY;
    
    StageRequest level;
    if( player->levelRequest )
        level = player->levelRequest->first;
    else
        level = player->getLevels()->retrieveLevel(player->getLevelRequestRow(), player->getLevelRequestCol());
    
    int numColors=NUM_POD_SIGNALS - 1;
    int numShapes=NUM_POD_SIGNALS - 1;
    int numSounds=NUM_POD_SIGNALS - 1;
    
    std::cout << "Available Features: " << numColors << " "<< numShapes << " "<< numSounds << std::endl;
    
    int nlevel = level.nback;
    switch (level.phaseX)
    {
        case PHASE_COLOR_SOUND:
        {
            nmode = STAGE_MODE_COLLECTION;
            if(level.pods!=0){
                int numofPods = std::min(numColors, numSounds);
                numofPods = std::min(numofPods, level.pods);
                
                std::cout<<"            Creating an Array of Pods Size: "<< numofPods << std::endl;
                
                globals.signalTypes = std::vector<std::vector<PodInfo> >(numofPods);
                
                for(int i=0; i<numofPods; i++){
                    globals.signalTypes[(PodSignal)i].push_back(PodInfo((PodSignal)i, POD_FUEL, (PodColor)(i), POD_SHAPE_UNKNOWN, (PodSound)(i)));
                }
                break;
            }
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_UNKNOWN, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_UNKNOWN, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_UNKNOWN, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_UNKNOWN, POD_SOUND_4));
            break;
        }
        case PHASE_SHAPE_SOUND:
        {
            nmode = STAGE_MODE_COLLECTION;
            if(level.pods!=0){
                int numofPods = std::min(numShapes, numSounds);
                numofPods = std::min(numofPods, level.pods);
                
                std::cout<<"            Creating an Array of Pods Size: "<< numofPods << std::endl;
                
                globals.signalTypes = std::vector<std::vector<PodInfo> >(numofPods);
                
                for(int i=0; i<numofPods; i++){
                    globals.signalTypes[(PodSignal)i].push_back(PodInfo((PodSignal)i, POD_FUEL, POD_COLOR_UNKNOWN, (PodShape)(i), (PodSound)(i)));
                }
                break;
            }
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_ORANGE, POD_SHAPE_DIAMOND, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_ORANGE, POD_SHAPE_SPHERE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_ORANGE, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_ORANGE, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            break;
        }
        case PHASE_SOUND_ONLY:
        {
            nmode = STAGE_MODE_COLLECTION;
            if(level.pods!=0){
                int numofPods = std::min(numSounds, level.pods);
                
                std::cout<<"            Creating an Array of Pods Size: "<< numofPods << std::endl;
                
                globals.signalTypes = std::vector<std::vector<PodInfo> >(numofPods);
                
                for(int i=0; i<numofPods; i++){
                    globals.signalTypes[(PodSignal)i].push_back(PodInfo((PodSignal)i, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, (PodSound)(i)));
                }
                break;
            }
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_4));
            break;
        }
        case PHASE_ALL_SIGNAL:
            nmode = STAGE_MODE_COLLECTION;
            if(level.pods!=0){
                int numofPods = std::min(numShapes, numSounds);
                numofPods = std::min(numofPods, numColors);
                numofPods = std::min(numofPods, level.pods);
                
                std::cout<<"            Creating an Array of Pods Size: "<< numofPods << std::endl;
                
                globals.signalTypes = std::vector<std::vector<PodInfo> >(numofPods);
                
                for(int i=0; i<numofPods; i++){
                    globals.signalTypes[(PodSignal)i].push_back(PodInfo((PodSignal)i, POD_FUEL, (PodColor)(i), (PodShape)(i), (PodSound)(i)));
                }
                break;
            }
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_DIAMOND, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_SPHERE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            break;
        case PHASE_COLLECT:
            nmode = STAGE_MODE_RECESS;
            globals.signalTypes.clear();
            break;
        default:
            break;
    }
    globals.initCamSpeed = level.initCamSpeed;
    globals.minCamSpeed = level.minCamSpeed;
    globals.maxCamSpeed = level.maxCamSpeed;
    
    // This should be a level parameter later
    
    int rank = level.nback;
    /*
    if (rank <= 1)
        globals.fuelReturn = 90.0f;
    else if (rank == 2)
        globals.fuelReturn = 75.0f;
    else //if (rank == 3)
        globals.fuelReturn = 60.0f;
    globals.fuelMax = 450.0;
     */
    if (level.phaseX == PHASE_COLLECT)
        globals.fuelReturn = 2.667f;
    else if (rank <= 1)
        globals.fuelReturn = 6.0f;
    else if (rank == 2)
        globals.fuelReturn = 5.0f;
    else //if (rank == 3)
        globals.fuelReturn = 4.0f;
    globals.fuelMax = 30.0;
    
    if (level.phaseX != PHASE_COLLECT)
    {
        if (level.durationX == DURATION_SHORT)
        {
            globals.startingHP = 5;
            globals.wrongAnswerTimePenalty = 3.0;
            globals.podBinSize = 10;
            globals.stageTotalSignals = 120;
            globals.stageTotalTargets = 40;
        }
        else if (level.durationX == DURATION_NORMAL)
        {
            globals.startingHP = 4;
            globals.wrongAnswerTimePenalty = 5.0;
            globals.podBinSize = 10;
            globals.stageTotalSignals = 120;
            globals.stageTotalTargets = 40;
        }
        else
        {
            globals.startingHP = 3;
            globals.wrongAnswerTimePenalty = 10.0;
            globals.podBinSize = 10;
            globals.stageTotalSignals = 120;
            globals.stageTotalTargets = 40;
        }
    }
    else
    {
        globals.stageTotalSignals = 120;
        globals.stageTotalTargets = 40;
    }
    
    
    std::cout << "Level Params:" << std::endl;
    std::cout << level.navLevels[0].level;
    std::cout << level.navLevels[1].level;
    std::cout << level.navLevels[2].level;
    std::cout << level.navLevels[3].level;
    std::cout << level.navLevels[0].obstacles;
    std::cout << level.navLevels[1].obstacles;
    std::cout << level.navLevels[2].obstacles;
    std::cout << level.navLevels[3].obstacles;
    tunnel = new Tunnel(
                        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(),
                        origin + forward * (globals.tunnelSegmentWidth / 2),
                        rot,
                        globals.tunnelSegmentWidth,
                        globals.tunnelSegmentDepth,
                        globals.tunnelMinAngleTurn,
                        globals.tunnelMaxAngleTurn,
                        globals.stageID,    // The n-th tunnel the player is playing
                        player->getLevels()->getLevelNo(player->getLevelRequestRow(), player->getLevelRequestCol()), // The level number in the level set
                        nmode,
                        level.phaseX,
                        nlevel,
                        level.stageTime,
                        SOUTH,
                        globals.tunnelSegmentsPerSection,
                        globals.tunnelSegmentsPerPod,
                        globals.tunnelSegmentsPerDistractors,
                        globals.tunnelSegmentsPerPowerup,
                        globals.signalTypes,
                        level.powerups);
    tunnel->link(player);
    player->link(tunnel);
    
    tunnel->setHoldoutSettings(level.holdoutPerc, level.holdoutStart, level.holdoutEnd, level.holdoutLevel, level.holdoutSound, level.holdoutColor, level.holdoutShape);
    tunnel->setNavigationLevels(level.navLevels, level.tunnelSectionsPerNavLevel);
    tunnel->setFuelLevel(globals.fuelMax, globals.fuelReturn, globals.startingHP);

    tunnel->setCollectionCriteria(level.collectionCriteria);
    tunnel->constructTunnel(level.nameTunnelTile, globals.tunnelSections);
    player->newTunnel(level.nameMusic);
    player->setSpeedParameters(globals.initCamSpeed, globals.minCamSpeed, globals.maxCamSpeed);
    
    Util::setSkyboxAndFog(level.nameSkybox);
    
    hud = new HudStage(player, tunnel);
    
    Quaternion camRot = player->getCombinedRotAndRoll();
    
    // Set lighting
    lightMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->createLight("StageLight");
    //lightMain->setType(Ogre::Light::LT_DIRECTIONAL);
    lightMain->setDiffuseColour(1.0, 1.0, 1.0);
    lightMain->setSpecularColour(1.0, 1.0, 1.0);
    //lightMain->setAttenuation(10, 1.0, 0.0001, 0.0);
    lightNode = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("lightNode");
    lightNode->attachObject(lightMain);
    
    // Set initial transformations for camera, light, skynode
    OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
    OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(camRot);
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->setOrientation(player->getCombinedRotAndRoll());
    if (lightNode) {
        lightNode->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
        //lightMain->setDirection(Vector3::UNIT_Y * -1);
    }
    
    // Set tutorial slides for certain thingies
    player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_NAVIGATION);
    if (tunnel->getNBack() == 1)
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_1BACK);
    if (tunnel->getHighestCriteria() == 2)
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_2BACK);
    if (tunnel->getPhase() == PHASE_SOUND_ONLY)
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_SOUND_ONLY);
    if (level.hasHoldout())
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_HOLDOUT);
    bool hasObstacles = false;
    for (int i = 0; i < level.navLevels.size(); ++i) {
        if (level.navLevels[i].obstacles > 0) {
            hasObstacles = true;
            break;
        }
    }
    if (hasObstacles)
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_OBSTACLE);
    
    if (!player->getTutorialMgr()->isVisible())
    {
        setTaskPrompt();
        hud->setSpeedDialState(true);
    }
    
    // Spin Parameters
    maxVel = player->maxVel;
    minVelFree = player->minVelFree;
    minVelStopper = player->minVelStopper;
    dampingDecayFree = player->dampingDecayFree;
    dampingDecayStop = player->dampingDecayStop;
    dampingDropFree = player->dampingDropFree;
    dampingDropStop = player->dampingDropStop;
    
    // Spin State Variables
    spinVelocity = 0.0f;
    spinVelocityTarget = 0.0f;
    dampingDecay = 0.0f;
    dampingDrop = 0.0f;
    freeMotion = false;
    freeAngleTraveled = 0.0f;
}

void EngineStage::updateSpin(float elapsed)
{
    //std::cout << "SPIN: " << spinVelocity << " " << spinVelocityTarget << std::endl;
    
    float prevRoll = player->getCamRoll();
    Direction prevDir = getDirByRoll(prevRoll);
    float afterRoll;
    Direction afterDir;
    
    if (freeMotion)
    {
        if (spinVelocityTarget > 0.0)
        {
            const float MAX_ACC = 30000;
        
            // Update towards velocity target
            if (spinVelocity < spinVelocityTarget)
                spinVelocity += (MAX_ACC * elapsed);
        
            // Don't exceed the velocity target
            if (spinVelocity >= spinVelocityTarget)
            {
                spinVelocity = spinVelocityTarget;
                spinVelocityTarget = 0.0;
            }
        
            // Bound velocity to a max
            if (spinVelocity >= maxVel)
                spinVelocity = maxVel;
        }
        else
        {
            // Damp the current vel
            spinVelocity *= dampingDecay;
            spinVelocity -= dampingDrop;
            
            // Zero out vel that is close to stop
            if (spinVelocity <= minVelFree)
                spinVelocity = 0.0;
            
            // Notify free motion is off
            if (spinVelocity <= 0.0)
            {
                freeMotion = false;
                freeAngleTraveled = 0.0;
            }
        }
    }
    
    // Convert radians to degree and obtain change in angle
    player->rollSpeed = (spinVelocity / (globals.screenWidth / 2.0)) * (180.0 / Ogre::Math::PI);
    float dTheta = player->rollSpeed * elapsed;
    
    // Perform Player Movement
    if (freeMotion)
    {
        if ((spinClockwise && player->inverted) ||
            (!spinClockwise && !player->inverted)) {
            this->activatePerformRightMove(dTheta);
            player->offsetRollDest = -dTheta;
            freeAngleTraveled -= dTheta;
        } else {
            this->activatePerformLeftMove(dTheta);
            player->offsetRollDest = dTheta;
            freeAngleTraveled += dTheta;
        }
    }
    
    lastAngles.insert(lastAngles.begin(), player->getCamRoll());
    while (lastAngles.size() > NUM_ANGLES_SAVED)
        lastAngles.pop_back();
    
    const float DELTA_DEGREE = 15.0;
    float curRoll = player->getCamRoll();
    int depthDist = 0;
    TunnelSlice* unpathable = closestUnpathable(tunnel, 3, curRoll, depthDist);
    if (unpathable)
    {
        SectionInfo info = unpathable->getSectionInfo();
        
        // Look for the closest panel using 15 degree delta steps
        float thetaDistEstimate = DELTA_DEGREE;
        while (thetaDistEstimate <= 180.0)
        {
            if (isPathable(info, curRoll - thetaDistEstimate))
            {
                thetaDistEstimate = -thetaDistEstimate;
                break;
            }
            if (isPathable(info, curRoll + thetaDistEstimate))
            {
                break;
            }
            thetaDistEstimate += DELTA_DEGREE;
        }
        
        thetaDistEstimate = thetaDistEstimate * (3 - depthDist);
        
        // Limit the force to 45 degrees
        if (thetaDistEstimate > 45.0)
            thetaDistEstimate = 45.0;
        if (thetaDistEstimate < -45.0)
            thetaDistEstimate = -45.0;
        
        // Assign new roll
        float recoverRollSpeed = thetaDistEstimate * player->getFinalSpeed() * globals.globalModifierCamSpeed / tunnel->getSegmentDepth();
        curRoll = curRoll + recoverRollSpeed * elapsed;
        
        // Resolve overshooting (if we are back on pathable ground)
        if (isPathable(info, curRoll))
        {
            curRoll = Util::getDegrees(getDirByRoll(curRoll));
        }
    }
    
    player->setCamRoll(curRoll);
    
    // Animating the offset banking
    double bankingAnimationSpeed = 30.0;
    if (player->offsetRoll < player->offsetRollDest)
    {
        player->offsetRoll += bankingAnimationSpeed * elapsed;
        if (player->offsetRoll > player->offsetRollDest)
            player->offsetRoll = player->offsetRollDest;
    }
    else if (player->offsetRoll > player->offsetRollDest)
    {
        player->offsetRoll -= bankingAnimationSpeed * elapsed;
        if (player->offsetRoll < player->offsetRollDest)
            player->offsetRoll = player->offsetRollDest;
    }
    
    afterRoll = player->getCamRoll();
    afterDir = getDirByRoll(afterRoll);
    float discreteDegrees = Util::getDegrees(afterDir);
    
    //std::cout << prevDir << " " << afterDir << " " << (discreteDegrees - prevRoll) * (discreteDegrees - afterRoll) << " " << spinVelocity;
    //std::cout << freeAngleTraveled << std::endl;
    
    // Check dot product  for direction is same. If it switches it means we cross a boom
    if (prevDir == afterDir &&  // Make sure it is the same panel in the first place
        (discreteDegrees - prevRoll) * (discreteDegrees - afterRoll) < 0.0 &&   // Check for the transition to the other side of panel
        spinVelocity <= minVelStopper &&    // Is the spin velocity slow enough?
        abs(freeAngleTraveled) >= 5.0) // At least travel some degrees before being able to be stopped
    {
        activateVelocity(0.0);
        player->setCamRoll(discreteDegrees);
    }

    player->setVineDirRequest(getDirByRoll(player->getCamRoll() + player->offsetRoll));
}

void EngineStage::setTaskPrompt()
{
    if (player->levelRequest)
    {
        StageRequest level = player->levelRequest->first;
        PlayerProgress progress = player->levelRequest->second;
        
        if (level.phaseX != PHASE_COLLECT)
        {
            globals.setMessage("Zap matching " + Util::toStringInt(level.nback) + "-Back fuel", MESSAGE_NORMAL);
            if (level.phaseX == PHASE_COLOR_SOUND)
                globals.appendMessage("\nusing Color and Sound", MESSAGE_NORMAL);
            else if (level.phaseX == PHASE_SHAPE_SOUND)
                globals.appendMessage("\nusing Shape and Sound", MESSAGE_NORMAL);
            else if (level.phaseX == PHASE_SOUND_ONLY)
                globals.appendMessage("\nusing Only Sound", MESSAGE_NORMAL);
            else if (level.phaseX == PHASE_ALL_SIGNAL)
                globals.appendMessage("\nUsing Color, Shape, and Sound", MESSAGE_NORMAL);
            globals.appendMessage("\n", MESSAGE_NORMAL);
        }
        
        
        float potential = player->modifyNBackDelta(level, progress, 1.0, true);
        if (level.phaseX == PHASE_COLLECT)
            globals.appendMessage("\nEnjoy the cruise cadet.", MESSAGE_NORMAL);
        if (potential >= 0.35)
            globals.appendMessage("\nStay focused, it's going to get rough.", MESSAGE_NORMAL);
        else if (potential >= 0.25)
            globals.appendMessage("\nGood luck cadet.", MESSAGE_NORMAL);
        else
            globals.appendMessage("\nNo worries cadet. You'll do fine.", MESSAGE_NORMAL);
    }
    else
        globals.setMessage("\n\nReady to launch?", MESSAGE_NORMAL);
    globals.appendMessage("\nSet and verify your speed.", MESSAGE_NORMAL);
}

void EngineStage::dealloc()
{
    player->resetPowerUps();
    player->removeVines();
    if (tunnel)
    {
        delete tunnel;
        tunnel = NULL;
    }
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
    if (lightNode)
    {
        lightNode->detachObject(lightMain);
        lightNode->getCreator()->destroyLight(lightMain);
        lightNode->removeAndDestroyAllChildren();
        lightNode->getCreator()->destroySceneNode(lightNode);
        lightNode = NULL;
    }
}

void EngineStage::setPause(bool value, bool targetAllSounds)
{
    if (value)
    {
        if (targetAllSounds)
            OgreFramework::getSingletonPtr()->m_pSoundMgr->pauseAllSounds();
        player->pause();
        
        Ogre::ControllerManager::getSingleton().setTimeFactor(0);
    }
    else
    {
        player->unpause();
        if (targetAllSounds)
            OgreFramework::getSingletonPtr()->m_pSoundMgr->resumeAllPausedSounds();
        globals.clearMessage();
        Ogre::ControllerManager::getSingleton().setTimeFactor(1);
    }
}

void EngineStage::completeStage(Evaluation forced)
{
    Evaluation eval = tunnel->getEval();
    player->rerollCounter = 2;
    player->saveAllResults(eval);
}