//
//  EngineStage.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/16/14.
//
//

#include "EngineStage.h"
#include "EngineStateManager.h"
#include "HudStage.h"
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
            globals.appendMessage("\n\nSwipe to Continue", MESSAGE_NORMAL);
            stageState = STAGE_STATE_PAUSE;
            break;
        }
        case STAGE_STATE_RUNNING:
        {
            OgreFramework::getSingletonPtr()->m_pSoundMgr->update(elapsed);

            // Update the game state
            tunnel->update(elapsed);
            if (tunnel->needsCleaning())
            {
                completeStage(EVEN);
                stageState = STAGE_STATE_PROMPT;
                setPause(true);
                globals.appendMessage("\nPlay Again?", MESSAGE_NORMAL);
            }

            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, false);
            
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
            
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, false);
            break;
        }
        case STAGE_STATE_PROMPT:
        {
            hud->update(elapsed);
            hud->setOverlay(0, true);
            hud->setOverlay(1, true);
            
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
            stageState = STAGE_STATE_RUNNING;
            setPause(false);
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
            stageState = STAGE_STATE_RUNNING;
            setPause(false);
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
                player->setToggleBack(0);
                if (tunnel) tunnel->respondToToggleCheat();
            }
            else if (queryGUI == "toggle2")
            {
                player->setToggleBack(1);
                if (tunnel) tunnel->respondToToggleCheat();
            }
            else if (queryGUI == "toggle3")
            {
                player->setToggleBack(2);
                if (tunnel) tunnel->respondToToggleCheat();
            }
            else if (queryGUI == "toggle4")
            {
                player->setToggleBack(3);
                if (tunnel) tunnel->respondToToggleCheat();
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
                player->setToggleBack(0);
                if (tunnel) tunnel->respondToToggleCheat();
            }
            else if (queryGUI == "toggle2")
            {
                player->setToggleBack(1);
                if (tunnel) tunnel->respondToToggleCheat();
            }
            else if (queryGUI == "toggle3")
            {
                player->setToggleBack(2);
                if (tunnel) tunnel->respondToToggleCheat();
            }
            else if (queryGUI == "toggle4")
            {
                player->setToggleBack(3);
                if (tunnel) tunnel->respondToToggleCheat();
            }
            else if (queryGUI == "pause")
            {
                setPause(true);
                stageState = STAGE_STATE_PROMPT;
            }
            break;
        }
        case STAGE_STATE_PROMPT:
        {
            std::string queryGUI = hud->queryButtons(Vector2(x, y));
            
            if (queryGUI == "resume")
            {
                if (!tunnel->needsCleaning())
                {
                    setPause(false);
                    stageState = STAGE_STATE_RUNNING;
                }
            }
            else if (queryGUI == "next")
            {
                LevelSet* levels = player->getLevels();
                int row = levels->getLevelRow(player->getLevelRequestRow());
                int col = levels->getLevelCol(player->getLevelRequestCol());
                int level = levels->getLevelNo(row, col);
                if (player->isLevelAvailable(level + 1))
                {
                    row = levels->getLevelRow(level + 1);
                    col = levels->getLevelCol(level + 1);
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
            player->setBaseSpeed(player->getBaseSpeed() + 1);
            break;
        }
        case OIS::KC_DOWN:
        {
            player->setBaseSpeed(player->getBaseSpeed() - 1);
            break;
        }
        case OIS::KC_1:
        {
            player->setToggleBack(3);
            if (tunnel) tunnel->respondToToggleCheat();
            break;
        }
        case OIS::KC_2:
        {
            player->setToggleBack(2);
            if (tunnel) tunnel->respondToToggleCheat();
            break;
        }
        case OIS::KC_3:
        {
            player->setToggleBack(1);
            if (tunnel) tunnel->respondToToggleCheat();
            break;
        }
        case OIS::KC_4:
        {
            player->setToggleBack(0);
            if (tunnel) tunnel->respondToToggleCheat();
            break;
        }
        case OIS::KC_P:
        {
            if (stageState == STAGE_STATE_RUNNING)
            {
                setPause(true);
                globals.appendMessage("\n\nSwipe to Continue", MESSAGE_NORMAL);
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
            if (stageState == STAGE_STATE_RUNNING) tunnel->setDone(EVEN);
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
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_4));

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
    
    tunnel->setHoldOut(level.hasHoldout);
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