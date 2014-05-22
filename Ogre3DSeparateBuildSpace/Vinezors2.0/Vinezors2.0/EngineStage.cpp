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
            if (tunnel->getMode() != STAGE_MODE_RECESS || tunnel->getMode() != STAGE_MODE_TEACHING)
                globals.appendMessage("\n\nSet Your Speed", MESSAGE_NORMAL);
            stageState = STAGE_STATE_PAUSE;
            
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, false);
            hud->setOverlay(2, false);
            hud->setOverlay(3, false);
            hud->notifyGoButton(false);
            
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
            break;
        }
        case STAGE_STATE_RUNNING:
        {
            OgreFramework::getSingletonPtr()->m_pSoundMgr->update(elapsed);
            
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

            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, false);
            hud->notifyGoButton(false);
            
            // Graphical view changes from camera, light, and skybox
            Quaternion camRot = player->getCombinedRotAndRoll();
            OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
            OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(camRot);
            if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->setOrientation(player->getCombinedRotAndRoll());
            if (lightNode)
                lightNode->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
            
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
            
            // See by the end of this frame if we have a tutorial popup, if so pause the game, keep the music running
            if (player->getTutorialMgr()->isVisible())
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
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
            
            if (!player->hasTriggeredStartup())
            {
                int sliderSpeed = hud->getSpeedSlider()->getIndex();
                globals.initCamSpeed = Util::clamp(sliderSpeed, globals.minCamSpeed, globals.maxCamSpeed);
                player->setSpeedParameters(globals.initCamSpeed, globals.minCamSpeed, globals.maxCamSpeed);
            }
            
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, false);
            hud->notifyGoButton(true);
            break;
        }
        case STAGE_STATE_PROMPT:
        {
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, true);
            hud->notifyGoButton(false);
            
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
            break;
        }
        case STAGE_STATE_DONE:
        {
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
                    // resolve overshooting by setting the player directly on the panel
                    float discreteDegrees = Util::getDegrees(getDirByRoll(val)) - 1;
                    if (isPathable(info, discreteDegrees))
                    {
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
                    // resolve overshooting by setting the player directly on the panel
                    float discreteDegrees = Util::getDegrees(getDirByRoll(val)) + 1;
                    if (isPathable(info, discreteDegrees))
                    {
                        // Subtract 1 due to discrete angle values
                        player->setCamRoll(discreteDegrees - 1); // resolve overshooting
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
                if (hud->isGoButtonActive())
                {
                    stageState = STAGE_STATE_PAUSE;
                    if (player->hasTriggeredStartup() && player->getStartMusicTimer() <= 0.0)
                        player->playMusic();
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
                    
                    setPause(false);
                    OgreFramework::getSingletonPtr()->m_pSoundMgr->stopAllSounds();
                }
            }
            else if (queryGUI == "restart")
            {
                stageState = STAGE_STATE_INIT;
                
                setPause(false);
                OgreFramework::getSingletonPtr()->m_pSoundMgr->stopAllSounds();
            }
            else if (queryGUI == "levelselect")
            {
                stageState = STAGE_STATE_DONE;
                
                setPause(false);
                OgreFramework::getSingletonPtr()->m_pSoundMgr->stopAllSounds();
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
    if (stageState == STAGE_STATE_RUNNING) tunnel->setDone(PASS);
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
    if (stageState == STAGE_STATE_PAUSE && !player->hasTriggeredStartup())
    {
        HudSlider* speedSlider = NULL;
        if (hud) speedSlider = hud->getSpeedSlider();
        if (speedSlider && speedSlider->selected)
        {
            Vector2 np;
#if !defined(OGRE_IS_IOS)
            if (speedSlider->orientation)
                np = speedSlider->p2 + globals.convertToPercentScreen(Vector2(0.0, dy));
            else
                np = speedSlider->p2 + globals.convertToPercentScreen(Vector2(dx, 0.0));
#else
            if (speedSlider->orientation)
                np = speedSlider->p2cache + globals.convertToPercentScreen(Vector2(0.0, dy));
            else
                np = speedSlider->p2cache + globals.convertToPercentScreen(Vector2(dx, 0.0));
#endif
            speedSlider->setBallPosition(np);
            
            //std::cout << "move " << speedSlider->p2.x << " " << globals.convertToPercentScreen(Vector2(dx, 0.0)).x << std::endl;;
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
            Vector2 pos = globals.convertToPercentScreen(Vector2(x, y));
        
            // Case for tapping into the slider which makes ball jump to position
            if (speedSlider->isInsideRange(pos))
            {
                // Project position to range
                if (speedSlider->orientation)
                {
                    float ny = pos.y - speedSlider->p1.y - speedSlider->dim2.y / 2;
                    speedSlider->setBallPosition(Vector2(speedSlider->p2.x, ny));
                }
                else
                {
                    float nx = pos.x - speedSlider->p1.x - speedSlider->dim2.x / 2;
                    speedSlider->setBallPosition(Vector2(nx, speedSlider->p2.y));
                }
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
    if (stageState == STAGE_STATE_PAUSE && !player->hasTriggeredStartup())
    {
        HudSlider* speedSlider = NULL;
        if (hud) speedSlider = hud->getSpeedSlider();
        if (speedSlider && speedSlider->selected)
        {
            speedSlider->selected = false;
            Vector2 np;
#if !defined(OGRE_IS_IOS)
            if (speedSlider->orientation)
                np = speedSlider->p2 + globals.convertToPercentScreen(Vector2(0.0, dy));
            else
                np = speedSlider->p2 + globals.convertToPercentScreen(Vector2(dx, 0.0));
#else
            if (speedSlider->orientation)
                np = speedSlider->p2cache + globals.convertToPercentScreen(Vector2(0.0, dy));
            else
                np = speedSlider->p2cache + globals.convertToPercentScreen(Vector2(dx, 0.0));
#endif
            speedSlider->setBallPosition(np);
            //std::cout << "release\n";
        }
    }
}

#define NUM_ANGLES_SAVED 3
#define ANGLE_LOOKBACK 0

void EngineStage::activateVelocity(float vel)
{
    //std::cout << "SPIN VELOCITY: " << vel << std::endl;
    //if (vel != 0.0) this->spinVelocity = abs(vel);

    //damping = 1.035;
    //damping = 1.135;
    //damping = 1.05;
    damping = 1.1;
    
    // spin velocity target gets set in activateAngleTurn(...)
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
        //damping = 10.0;
        //damping = 5.0;
        damping = 2.0;
        //damping = 1.5;
        //damping = 1.2;
        
        if (lastAngles.size() >= NUM_ANGLES_SAVED) {
            player->setCamRoll(lastAngles[ANGLE_LOOKBACK]);
            std::cerr << "Using last angle" << std::endl;
        }
    }
    
    if (abs(spinVelocityTarget) >= maxVel)
        spinVelocityTarget = maxVel;
}

void EngineStage::activateAngleTurn(float angle)
{
    if (tunnel && !tunnel->isDone())
    {
        SectionInfo info = tunnel->getCurrent()->getSectionInfo();
        TunnelSlice* next = tunnel->getNext(1);
        
        //Convert to degrees;
        double dT = (angle * 180.0) / Ogre::Math::PI;
        float roll = player->getCamRoll();
        
        // If unpathable upahead, don't allow player to traverse through
        int depthDist = 0;
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
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_UNKNOWN, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_UNKNOWN, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_UNKNOWN, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_UNKNOWN, POD_SOUND_4));
            
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
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_HOLDOUT, POD_SHAPE_UNKNOWN, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_HOLDOUT, POD_SHAPE_UNKNOWN, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_HOLDOUT, POD_SHAPE_UNKNOWN, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_HOLDOUT, POD_SHAPE_UNKNOWN, POD_SOUND_4));

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
                globals.stageTotalCollections = (level.minCamSpeed + level.maxCamSpeed) / 3.0 * level.stageTime / Util::getModdedLengthByNumSegments(globals, globals.tunnelSegmentsPerPod);
            else
                globals.stageTotalCollections = (level.minCamSpeed + level.maxCamSpeed) / 2.5 * level.stageTime / Util::getModdedLengthByNumSegments(globals, globals.tunnelSegmentsPerPod);
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
    
    // Set tutorial slides for certain thingies
    if (tunnel->getMode() == STAGE_MODE_RECESS || tunnel->getMode() == STAGE_MODE_TEACHING || tunnel->getHighestCriteria() <= 0)
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_ZERO_BACK);
    if (tunnel->getHighestCriteria() == 1)
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_ONE_BACK);
    if (tunnel->getHighestCriteria() == 2)
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_TWO_BACK);
    if (tunnel->getPhase() == 'A')
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_COLOR_SOUND);
    if (tunnel->getPhase() == 'B')
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_SHAPE_SOUND);
    if (tunnel->getPhase() == 'C')
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_SOUND_ONLY);
    if (tunnel->getPhase() == 'D')
        player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_HOLDOUT);

    spinVelocity = 0.0f;
    spinVelocityTarget = 0.0f;
    damping = 0.0f;
    freeMotion = false;
}

void EngineStage::updateSpin(float elapsed)
{
    maxVel = 4500.0;
    
    std::cout << "SPIN: " << spinVelocity << " " << spinVelocityTarget << std::endl;
    
    const float MAX_ACC = 50000;
    
    // Update towards velocity target
    if (spinVelocity < spinVelocityTarget)
        spinVelocity += (MAX_ACC * elapsed);
    
    // Don't exceed the velocity target
    if (spinVelocity >= spinVelocityTarget && spinVelocityTarget > 0.0)
    {
        spinVelocity = spinVelocityTarget;
        spinVelocityTarget = 0.0;
    }
    
    // Bound velocity to a max
    if (spinVelocity >= maxVel)
        spinVelocity = maxVel;
    
    float dTheta = (spinVelocity / (globals.screenWidth / 2.0)) * elapsed;
    const float PI = 3.14;
    dTheta = (dTheta * 180.0) / PI;
    
    // Damp the current vel
    spinVelocity /= damping;
    
    // Zero out vel that is close to stop
    if (spinVelocity <= 100.0 && spinVelocityTarget <= 0.0)
        spinVelocity = 0.0;
    
    // Notify free motion is off
    if (spinVelocity <= 0.0)
        freeMotion = false;
    
    // Perform Player Movement
    if (freeMotion)
    {
        if (spinClockwise) {
            this->activatePerformRightMove(dTheta);
            player->offsetRollDest = -dTheta;
        } else {
            this->activatePerformLeftMove(dTheta);
            player->offsetRollDest = dTheta;
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
        player->setCamRoll(curRoll);
    }
    
    /*
    // Avoid moving into empty panels ahead of us
    TunnelSlice* next = tunnel->getNext(1);
    if (next)
    {
        SectionInfo nextInfo = next->getSectionInfo();
        float curRoll = player->getCamRoll();
        if (!isPathable(nextInfo, curRoll))
        {
            // Look for the closest panel using 15 degree delta steps
            float thetaDistEstimate = DELTA_DEGREE;
            while (thetaDistEstimate <= 180.0)
            {
                if (isPathable(nextInfo, curRoll - thetaDistEstimate))
                {
                    thetaDistEstimate = -thetaDistEstimate;
                    break;
                }
                if (isPathable(nextInfo, curRoll + thetaDistEstimate))
                {
                    break;
                }
                thetaDistEstimate += DELTA_DEGREE;
            }
            
            // Limit the force to 45 degrees
            if (thetaDistEstimate > 45.0)
                thetaDistEstimate = 45.0;
            if (thetaDistEstimate < -45.0)
                thetaDistEstimate = -45.0;
            
            // Assign new roll
            float recoverRollSpeed = thetaDistEstimate * player->getFinalSpeed() * globals.globalModifierCamSpeed / tunnel->getSegmentDepth();
            curRoll = curRoll + recoverRollSpeed * elapsed;
            
            // Resolve overshooting (if we are back on pathable ground)
            if (isPathable(nextInfo, curRoll))
            {
                curRoll = Util::getDegrees(getDirByRoll(curRoll));
            }
            player->setCamRoll(curRoll);
        }
    }
     */
    
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
    
    player->setVineDirRequest(getDirByRoll(player->getCamRoll() + player->offsetRoll));
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
        if (targetAllSounds)
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