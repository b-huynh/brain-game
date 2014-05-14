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
    enter();
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
    switch (stageState)
    {
        case STAGE_STATE_INIT:
        {
            setup();
            setPause(true);
            globals.appendMessage("\n\nSet Your Speed", MESSAGE_NORMAL);
            stageState = STAGE_STATE_PAUSE;
            break;
        }
        case STAGE_STATE_RUNNING:
        {
            OgreFramework::getSingletonPtr()->m_pSoundMgr->update(elapsed);

            // Update the game state
            tunnel->update(elapsed);
            
            updateSpin(elapsed);
            
            if (tunnel->needsCleaning())
            {
                completeStage(EVEN);
                stageState = STAGE_STATE_PROMPT;
                setPause(true);
                
                std::string finalScoreMessage = Util::toStringInt(player->getScore());
                globals.appendMessage("\nScore " + finalScoreMessage + "\n\nPlay Again?", MESSAGE_NORMAL);
            }

            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, false);
            hud->setOverlay(2, false);
            
            // Graphical view changes from camera, light, and skybox
            Quaternion camRot = player->getCombinedRotAndRoll();
            OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
            OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(camRot);
            if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->setOrientation(player->getCombinedRotAndRoll());
            if (lightNode)
                lightNode->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
            
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
            
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
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
            
            int sliderSpeed = hud->getSpeedSlider()->getIndex();
            globals.initCamSpeed = Util::clamp(sliderSpeed, globals.minCamSpeed, globals.maxCamSpeed);
            player->setSpeedParameters(globals.initCamSpeed, globals.minCamSpeed, globals.maxCamSpeed);
            
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, false);
            hud->setOverlay(2, true);
            break;
        }
        case STAGE_STATE_PROMPT:
        {
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, true);
            hud->setOverlay(2, false);
            
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
            break;
        }
        case STAGE_STATE_DONE:
        {
            // Unpause Settings but without the sound deactivating
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
            globals.clearMessage();
            Ogre::ControllerManager::getSingleton().setTimeFactor(1);
            engineStateMgr->requestPopEngine();
            break;
        }
    }
}

Direction getDirByRoll (int roll)
{
    while (roll > 360)
        roll -= 360;
    
    while (roll < 0)
        roll += 360;
    
    if (roll < 22.5) return SOUTH;
    if (roll < 67.5) return SOUTHWEST;
    if (roll < 112.5) return WEST;
    if (roll < 157.5) return NORTHWEST;
    if (roll < 202.5) return NORTH;
    if (roll < 247.5) return NORTHEAST;
    if (roll < 292.5) return EAST;
    if (roll < 337.5) return SOUTHEAST;
    if (roll < 360) return SOUTH;
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
        {
            //stageState = STAGE_STATE_RUNNING;
            //setPause(false);
            break;
        }
        case STAGE_STATE_PROMPT:
            break;
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformLeftMove(int angle)
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
                int numSides = 0;
                for (int i = 0; i < NUM_DIRECTIONS; ++i)
                    if (info.sidesUsed[i])
                        ++numSides;
                
                int control;
                if (numSides == 8) control = 4;
                else if (numSides == 7) control = 3;
                else if (numSides == 5) control = 2;
                else if (numSides == 3) control = 1;
                else control = 0;
                float lock = control * (45);
                
                float val = player->getCamRoll();
                if (val < lock || numSides == 8)
                    player->setCamRoll(val + angle);
                
                player->setVineDirRequest(getDirByRoll(player->getCamRoll()));
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
        case STAGE_STATE_DONE:
            break;
    }
}

void EngineStage::activatePerformRightMove(int angle)
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
                int numSides = 0;
                for (int i = 0; i < NUM_DIRECTIONS; ++i)
                    if (info.sidesUsed[i])
                        ++numSides;
                
                int control;
                if (numSides == 8) control = 4;
                else if (numSides == 7) control = 3;
                else if (numSides == 5) control = 2;
                else if (numSides == 3) control = 1;
                else control = 0;
                float lock = control * (-45);
                
                float val = player->getCamRoll();
                if (val > lock || numSides == 8)
                    player->setCamRoll(val - angle);
                
                player->setVineDirRequest(getDirByRoll(player->getCamRoll()));
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
            break;
        case STAGE_STATE_PAUSE:
            break;
        case STAGE_STATE_PROMPT:
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
            
            if (queryGUI == "powerup1")
                player->performPowerUp("TractorBeam");
            else if (queryGUI == "powerup2")
                player->performPowerUp("Shields");
            else if (queryGUI == "powerup3")
                player->performPowerUp("TimeWarp");
            else if (queryGUI == "toggle1")
            {
                if (tunnel && tunnel->isMultiCollectionTask())
                {
                    // Bad hack but
                    // Don't show 3-Back for multi-collection tasks of 1 or less.
                    if (player->getLevelRequestRow() > 0)
                        player->setToggleBack(0);
                    tunnel->respondToToggleCheat();
                }
            }
            else if (queryGUI == "toggle2")
            {
                if (tunnel && tunnel->isMultiCollectionTask())
                {
                    player->setToggleBack(1);
                    tunnel->respondToToggleCheat();
                }
            }
            else if (queryGUI == "toggle3")
            {
                if (tunnel && tunnel->isMultiCollectionTask())
                {
                    player->setToggleBack(2);
                    tunnel->respondToToggleCheat();
                }
            }
            else if (queryGUI == "toggle4")
            {
                if (tunnel && tunnel->isMultiCollectionTask())
                {
                    player->setToggleBack(3);
                    tunnel->respondToToggleCheat();
                }
            }
            else if (queryGUI == "pause")
            {
                setPause(true);
                stageState = STAGE_STATE_PROMPT;
            }
            break;
        }
        case STAGE_STATE_PAUSE:
        {
            std::string queryGUI = hud->queryButtons(Vector2(x, y));
            
            if (queryGUI == "toggle1")
            {
                if (tunnel && tunnel->isMultiCollectionTask())
                {
                    // Bad hack but
                    // Don't show 3-Back for multi-collection tasks of 1 or less.
                    if (player->getLevelRequestRow() > 0)
                        player->setToggleBack(0);
                    tunnel->respondToToggleCheat();
                }
            }
            else if (queryGUI == "toggle2")
            {
                if (tunnel && tunnel->isMultiCollectionTask())
                {
                    player->setToggleBack(1);
                    tunnel->respondToToggleCheat();
                }
            }
            else if (queryGUI == "toggle3")
            {
                if (tunnel && tunnel->isMultiCollectionTask())
                {
                    player->setToggleBack(2);
                    tunnel->respondToToggleCheat();
                }
            }
            else if (queryGUI == "toggle4")
            {
                if (tunnel && tunnel->isMultiCollectionTask())
                {
                    player->setToggleBack(3);
                    tunnel->respondToToggleCheat();
                }
            }
            else if (queryGUI == "pause")
            {
                setPause(true);
                stageState = STAGE_STATE_PROMPT;
            }
            else if (queryGUI == "go")
            {
                stageState = STAGE_STATE_RUNNING;
                setPause(false);
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
                    stageState = STAGE_STATE_PAUSE;
                }
                else if (!tunnel->needsCleaning())
                {
                    setPause(false);
                    stageState = STAGE_STATE_RUNNING;
                }
            }
            else if (queryGUI == "next")
            {
                LevelSet* levels = player->getLevels();
                int row = player->getLevelRequestRow();
                int col = player->getLevelRequestCol();
                int level = levels->getLevelNo(row, col);
                int nlevel = ((level + 1) % NUM_TASKS) != 5 ? level + 1 : level + 2;
                if (player->isLevelAvailable(nlevel))
                {
                    row = levels->getLevelRow(nlevel);
                    col = levels->getLevelCol(nlevel);
                    player->setLevelRequest(row, col);
                    stageState = STAGE_STATE_INIT;
                }
            }
            else if (queryGUI == "restart")
            {
                stageState = STAGE_STATE_INIT;
            }
            else if (queryGUI == "levelselect")
            {
                stageState = STAGE_STATE_DONE;
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
    std::cout << "God Mode: " << player->getGodMode() << std::endl;
    player->setGodMode(!player->getGodMode());
    if (tunnel) tunnel->respondToToggleCheat();
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
            player->performBoost();
            break;
        }
        case STAGE_STATE_PAUSE:
            break;
        case STAGE_STATE_PROMPT:
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
        case STAGE_STATE_DONE:
            break;
    }
}

// The following deal with injecting coordinates to simulate a slider
//
void EngineStage::activateMoved(float x, float y, float dx, float dy)
{
    if (stageState == STAGE_STATE_PAUSE)
    {
        HudSlider* speedSlider = NULL;
        if (hud) speedSlider = hud->getSpeedSlider();
        if (speedSlider && speedSlider->selected)
        {
#if !defined(OGRE_IS_IOS)
            Vector2 np = speedSlider->p2 + globals.convertToPercentScreen(Vector2(dx, 0.0));
#else
            Vector2 np = speedSlider->p2cache + globals.convertToPercentScreen(Vector2(dx, 0.0));
#endif
            speedSlider->setBallPosition(np);
            
            //std::cout << "move " << speedSlider->p2.x << " " << globals.convertToPercentScreen(Vector2(dx, 0.0)).x << std::endl;;
        }
    }
}

void EngineStage::activatePressed(float x, float y)
{
    if (stageState == STAGE_STATE_PAUSE)
    {
        HudSlider* speedSlider = NULL;
        if (hud) speedSlider = hud->getSpeedSlider();
        if (speedSlider)
        {
            Vector2 pos = globals.convertToPercentScreen(Vector2(x, y));
        
            // Case for tapping into the slider which makes ball jump to position
            if (speedSlider->isInsideRange(pos))
            {
                // Project position to range
                float nx = pos.x - speedSlider->p1.x - speedSlider->dim2.x / 2;
                speedSlider->setBallPosition(Vector2(nx, speedSlider->p2.y));
            }
        
            // Initialize p2cache first time we touch ball
            // p2cache is better since iOS tracking uses total dx which is more accurate
            // then passing in a dx everytime
            if (speedSlider->isInsideBall(pos))
            {
                speedSlider->selected = true;
                speedSlider->p2cache = speedSlider->p2;
            }
            //std::cout << "press\n";
        }
    }
}

void EngineStage::activateReleased(float x, float y, float dx, float dy)
{
    if (stageState == STAGE_STATE_PAUSE)
    {
        HudSlider* speedSlider = NULL;
        if (hud) speedSlider = hud->getSpeedSlider();
        if (speedSlider && speedSlider->selected)
        {
            speedSlider->selected = false;
#if !defined(OGRE_IS_IOS)
            Vector2 np = speedSlider->p2 + globals.convertToPercentScreen(Vector2(dx, 0.0));
#else
            Vector2 np = speedSlider->p2cache + globals.convertToPercentScreen(Vector2(dx, 0.0));
#endif
            speedSlider->setBallPosition(np);
            //std::cout << "release\n";
        }
    }
}

void EngineStage::activateVelocity(float vel)
{
    float maxVel = 4500.0;
    
    std::cout << "SPIN VELOCITY: " << vel << std::endl;
    if (vel != 0.0) this->spinVelocity = abs(vel);
    damping = 1.035;
    if (vel < 0.0) spinClockwise = false;
    else if (vel > 0.0) spinClockwise = true;
    else damping = 5.0;
    
    if (abs(spinVelocity) >= maxVel)
        spinVelocity = maxVel;
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
        case OIS::KC_1:
        {
            if (tunnel && tunnel->isMultiCollectionTask())
            {
                player->setToggleBack(3);
                tunnel->respondToToggleCheat();
            }
            break;
        }
        case OIS::KC_2:
        {
            if (tunnel && tunnel->isMultiCollectionTask())
            {
                player->setToggleBack(2);
                tunnel->respondToToggleCheat();
            }
            break;
        }
        case OIS::KC_3:
        {
            if (tunnel && tunnel->isMultiCollectionTask())
            {
                player->setToggleBack(1);
                tunnel->respondToToggleCheat();
            }
            break;
        }
        case OIS::KC_4:
        {
            if (tunnel && tunnel->isMultiCollectionTask())
            {
                // Bad hack but
                // Don't show 3-Back for multi-collection tasks of 1 or less.
                if (player->getLevelRequestRow() > 0)
                    player->setToggleBack(0);
                tunnel->respondToToggleCheat();
            }
            break;
        }
        case OIS::KC_P:
        {
            if (stageState == STAGE_STATE_RUNNING)
            {
                setPause(true);
                globals.appendMessage("\n\nPaused", MESSAGE_NORMAL);
                stageState = STAGE_STATE_PAUSE;
            }
            else
            {
                setPause(false);
                stageState = STAGE_STATE_RUNNING;
            }
            break;
        }
        case OIS::KC_Z:
        {
            activatePerformBeginLongPress();
            break;
        }
        case OIS::KC_X:
        {
            if (stageState == STAGE_STATE_RUNNING) player->performPowerUp("TractorBeam");
            break;
        }
        case OIS::KC_C:
        {
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
    
    //if (!configStageType(globals.configPath, globals.configBackup, "globalConfig"))
    //    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
    
    globals.maxCamSpeed = skillLevel.maxSpeed;
    
    globals.stageTotalTargets1 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
    globals.stageTotalTargets2 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
    globals.stageTotalTargets3 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
    
    StageMode nmode = STAGE_MODE_PROFICIENCY;
    StageRequest level = player->getLevels()->retrieveLevel(player->getLevelRequestRow(), player->getLevelRequestCol());
    int nlevel = level.nback;
    switch (level.phase)
    {
        case 'A':
        {
            nmode = STAGE_MODE_COLLECTION;
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_CONE, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_CONE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_CONE, POD_SOUND_4));
            
            //globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            globals.appendMessage("\nObtain matches by Color!", MESSAGE_NORMAL);
            break;
        }
        case 'B':
        {
            nmode = STAGE_MODE_COLLECTION;
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_DIAMOND, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_SPHERE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            
            //globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            globals.appendMessage("Obtain matches by Shape!", MESSAGE_NORMAL);
            break;
        }
        case 'C':
        {
            nmode = STAGE_MODE_COLLECTION;
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_HOLDOUT, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_HOLDOUT, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_HOLDOUT, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_HOLDOUT, POD_SOUND_4));

            //globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            globals.setMessage("Obtain matches by only sound!", MESSAGE_NORMAL);
            break;
        }
        case 'D':
            nmode = STAGE_MODE_COLLECTION;
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_DIAMOND, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_SPHERE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            
            //globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            globals.setMessage("Obtain matching signals!", MESSAGE_NORMAL);
            break;
        case 'E':
            nmode = STAGE_MODE_RECESS;
            globals.signalTypes.clear();
            
            if (level.initCamSpeed <= 15) // For starting slower stages, be nicer
                globals.stageTotalCollections = (globals.speedMap[level.minCamSpeed] + globals.speedMap[level.maxCamSpeed]) / 3.0 * level.stageTime / Util::getModdedLengthByNumSegments(globals, globals.tunnelSegmentsPerPod);
            else
                globals.stageTotalCollections = (globals.speedMap[level.minCamSpeed] + globals.speedMap[level.maxCamSpeed]) / 2.5 * level.stageTime / Util::getModdedLengthByNumSegments(globals, globals.tunnelSegmentsPerPod);
            //globals.setBigMessage("Recess!");
            globals.setMessage("Reach the end! Grab Fuel Cells!", MESSAGE_NORMAL);
            break;
        case 'F':
            nmode = STAGE_MODE_TEACHING;
            globals.signalTypes.clear();
            //globals.setBigMessage("Training!");
            globals.setMessage("Grab Fuel Cells!", MESSAGE_NORMAL);
            break;
    }
    globals.initCamSpeed = level.initCamSpeed;
    globals.minCamSpeed = level.minCamSpeed;
    globals.maxCamSpeed = level.maxCamSpeed;
    
    tunnel = new Tunnel(
                        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(),
                        origin + forward * (globals.tunnelSegmentWidth / 2),
                        rot,
                        globals.tunnelSegmentWidth,
                        globals.tunnelSegmentDepth,
                        globals.tunnelMinAngleTurn,
                        globals.tunnelMaxAngleTurn,
                        1, // replace or remove...
                        nmode,
                        level.phase, // replace or remove...
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
    
    tunnel->setHoldout(level.hasHoldout,level.holdoutFrequency);
    if (tunnel->getMode() == STAGE_MODE_RECESS)
    {
        // Assign nav levels in an incremental order specified by Liam's formula if the mode is recess
        // Therefore, the level parameter provided looks at the nav level provided as the starting index
        tunnel->setNavigationLevels(level.navLevels[0].level, globals.navMap.size() - 1, level.tunnelSectionsPerNavLevel);
    }
    else
    {
        // Otherwise, nav levels provided by the parameters
        tunnel->setNavigationLevels(level.navLevels, level.tunnelSectionsPerNavLevel);
    }
    tunnel->setCollectionCriteria(level.collectionCriteria);
    tunnel->constructTunnel(level.nameTunnelTile, globals.tunnelSections);
    player->newTunnel(level.nameMusic);
    player->setSpeedParameters(globals.initCamSpeed, globals.minCamSpeed, globals.maxCamSpeed);
    
    Util::setSkyboxAndFog(level.nameSkybox);
    
    /*
     if (skillLevel.set1Notify)
     globals.setBigMessage("Congratulations! You earned " + Util::toStringInt(nlevel) + "-Back!");
     else
     {
     if (skillLevel.set1Rep >= 2)
     globals.setBigMessage(Util::toStringInt(nlevel) + "-Back. Challenge Round!");
     else
     globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
     }
     globals.setMessage("Match by Color!", MESSAGE_NORMAL);
     */
    
    hud = new HudStage(player, tunnel);
    
    Quaternion camRot = player->getCombinedRotAndRoll();
    
    // Set lighting
    lightMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->createLight("StageLight");
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
    if (lightNode)
        lightNode->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
    
    spinVelocity = 0.0f;
    damping = 0.0f;
}

void EngineStage::updateSpin(float elapsed)
{
    float radius = globals.screenWidth / 2.0;
    
    radius = tunnel->getCurrent()->getWallLength() / 1.5;
    
    Quaternion vRot;
    
    spinVelocity /= damping;
    
    float dTheta = (spinVelocity / (globals.screenWidth / 2.0)) * elapsed;
    const float PI = 3.14;
    dTheta = (dTheta * 180.0) / PI;
    
    Radian theta = Ogre::Radian((int)dTheta);
    
    if (spinClockwise) {
        this->activatePerformRightMove((int)dTheta);
        theta = -theta;
    } else {
        this->activatePerformLeftMove((int)dTheta);
    }
    
    Vector3 camUp = player->getCamUpward(true);
    Vector3 camForward = player->getCamForward(true);
    
    vRot.ToAngleAxis(theta, camForward);
    
    Vector3 camDown = vRot * (-camUp);
    
    player->setPos(player->getCamPos() + player->getCamForward() * 25 + (camDown * radius));
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

void EngineStage::setPause(bool value)
{
    if (value)
    {
        OgreFramework::getSingletonPtr()->m_pSoundMgr->pauseAllSounds();
        player->pause();
        
        // Display current level index
        LevelSet* levels = player->getLevels();
        std::string msg = "Level: ";
        msg += Util::toStringInt(player->getLevelRequestRow() + 1);
        msg += "-";
        msg += (char)('A' + player->getLevelRequestCol());
        globals.setMessage(msg, MESSAGE_NORMAL);
        
        Ogre::ControllerManager::getSingleton().setTimeFactor(0);
    }
    else
    {
        player->unpause();
        OgreFramework::getSingletonPtr()->m_pSoundMgr->resumeAllPausedSounds();
        globals.clearMessage();
        Ogre::ControllerManager::getSingleton().setTimeFactor(1);
    }
}

void EngineStage::completeStage(Evaluation forced)
{
    Evaluation eval = tunnel->getEval();
    player->saveAllResults(eval);
}