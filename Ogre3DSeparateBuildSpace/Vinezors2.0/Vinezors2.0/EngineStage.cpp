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
                globals.setMessage("Play Again?\n\n\n(Yes / No)\n\n\n<---     --->", MESSAGE_NORMAL);
            }

            hud->update(elapsed);
            
            // Graphical view changes from camera, light, and skybox
            Quaternion camRot = player->getCombinedRotAndRoll();
            OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
            OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(camRot);
            if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->setOrientation(player->getCombinedRotAndRoll());
            if (lightNode)
                lightNode->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
            
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
            
            hud->update(elapsed);
            break;
        }
        case STAGE_STATE_PROMPT:
            tunnel->update(elapsed);
            hud->update(elapsed);
            break;
        case STAGE_STATE_DONE:
        {
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
            if (player->setVineDirRequest(Util::rightOf(player->getVineDest())) && !tunnel->isDone())
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
        {
            // Request a retry of this level
            stageState = STAGE_STATE_INIT;
            break;
        }
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
            if (player->setVineDirRequest(Util::leftOf(player->getVineDest())) && !tunnel->isDone())
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
        {
            // Request a return to menu which requires main app to pop this off stack
            stageState = STAGE_STATE_DONE;
            break;
        }
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
            
            if (queryGUI == "toggle1")
                player->setToggleBack(0);
            else if (queryGUI == "toggle2")
                player->setToggleBack(1);
            else if (queryGUI == "toggle3")
                player->setToggleBack(2);
            else if (queryGUI == "pause")
            {
                setPause(true);
                stageState = STAGE_STATE_PAUSE;
            }
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

void EngineStage::activatePerformPinch()
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
    return true;
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
    return true;
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
    return true;
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
        case OIS::KC_P:
        {
            break;
        }
        case OIS::KC_MINUS:
        {
            break;
        }
        case OIS::KC_EQUALS:
        {
            break;
        }
        case OIS::KC_SPACE:
        {
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
        case OIS::KC_1:
        {
            player->setToggleBack(2);
            break;
        }
        case OIS::KC_2:
        {
            player->setToggleBack(1);
            break;
        }
        case OIS::KC_3:
        {
            player->setToggleBack(0);
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
        case OIS::KC_SPACE:
        {
            player->setKeySpace(false);
            break;
        }
        case OIS::KC_P:
        {
            if (stageState == STAGE_STATE_RUNNING)
            {
                setPause(true);
                stageState = STAGE_STATE_PAUSE;
            }
            else
            {
                setPause(false);
                stageState = STAGE_STATE_RUNNING;
            }
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
    
    if (!configStageType(globals.configPath, globals.configBackup, "globalConfig"))
        globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
    
    globals.maxCamSpeed = skillLevel.maxSpeed;
    
    globals.stageTotalTargets1 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
    globals.stageTotalTargets2 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
    globals.stageTotalTargets3 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
    
    StageMode nmode = STAGE_MODE_PROFICIENCY;
    StageRequest level = player->getStageRequest();
    int nlevel = level.nback;
    
    switch (level.phase)
    {
        case 'A':
        {
            nmode = STAGE_MODE_PROFICIENCY;
            globals.initCamSpeed = skillLevel.runSpeed1;
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_CONE, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_CONE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_CONE, POD_SOUND_4));
            
            globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            globals.setMessage("Obtain matches by Color!", MESSAGE_NORMAL);
            break;
        }
        case 'B':
        {
            nmode = STAGE_MODE_PROFICIENCY;
            globals.initCamSpeed = skillLevel.runSpeed2;
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_DIAMOND, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_SPHERE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            
            globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            globals.setMessage("Obtain matches by Shape!", MESSAGE_NORMAL);
            break;
        }
        case 'C':
        {
            nmode = STAGE_MODE_PROFICIENCY;
            globals.initCamSpeed = skillLevel.runSpeed3;
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_4));

            globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            globals.setMessage("Obtain matches by only sound!", MESSAGE_NORMAL);
            break;
        }
        case 'D':
            nmode = STAGE_MODE_PROFICIENCY;
            globals.initCamSpeed = (skillLevel.runSpeed1 + skillLevel.runSpeed2 + skillLevel.runSpeed3) / 3;
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_DIAMOND, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_SPHERE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            
            globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            globals.setMessage("Obtain matches by signals!", MESSAGE_NORMAL);
            break;
        case 'E':
            nmode = STAGE_MODE_RECESS;
            globals.initCamSpeed = (skillLevel.runSpeed1 + skillLevel.runSpeed2 + skillLevel.runSpeed3) / 3;
            globals.signalTypes.clear();
            globals.stageTotalCollections = skillLevel.averageSpeed * globals.stageTime / Util::getModdedLengthByNumSegments(globals, globals.tunnelSegmentsPerPod);
            globals.setBigMessage("Recess!");
            globals.setMessage("Grab Fuel Cells!", MESSAGE_NORMAL);
            break;
    }
    
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
                        SOUTH,
                        globals.tunnelSegmentsPerSection,
                        globals.tunnelSegmentsPerPod,
                        globals.tunnelSegmentsPerDistractors,
                        globals.signalTypes);
    tunnel->link(player);
    player->link(tunnel);
    
    tunnel->setNavigationLevels(level.navLevels);
    tunnel->constructTunnel(globals.tunnelSections);
    player->newTunnel();
    
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
    
    // Set skybox
    Plane plane;
    plane.d = 80;
    plane.normal = Ogre::Vector3(0, 0, 1);
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, "General/SpaceSkyPlane", 1, 1, true);
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(0.2, 0.0, 0.2), 0.0, 300.0, 600.0);
    OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
    
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
        globals.appendMessage("\n\nSwipe to Continue", MESSAGE_NORMAL);
#ifdef DEBUG_MODE
        globals.appendMessage("\n\n" + player->getSkillLevel().getCurrentStats(), MESSAGE_NORMAL);
        globals.appendMessage("\n\n" + Util::toStringInt(globals.sessionTime - getTotalElapsed()), MESSAGE_NORMAL);
#endif
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
    PlayerLevel skillLevel = player->getSkillLevel();
    switch (tunnel->getPhase())
    {
        case 'A':
        {
            skillLevel.runSpeed1 = player->getBaseSpeed();
            skillLevel.set1Notify = 0;
            if (eval == PASS || forced == PASS)
            {
                skillLevel.set1Rep++;
                if (skillLevel.set1Rep >= globals.set1Repetitions)
                {
                    skillLevel.runSpeed1 *= globals.nlevelSpeedModifier;
                    skillLevel.runSpeed1 = Util::clamp(skillLevel.runSpeed1, globals.minCamSpeed, globals.maxCamSpeed);
                    skillLevel.set1Rep = 0;
                    skillLevel.set1Notify = 1;
                    skillLevel.set1++;
                }
            }
            else if (eval == FAIL || forced == FAIL)
            {
                skillLevel.set1Rep--;
                if (skillLevel.set1Rep < 0)
                {
                    if (skillLevel.set1 > 1)
                    {
                        skillLevel.set1Rep = globals.set1Repetitions - 1;
                        skillLevel.set1--;
                    }
                    else
                        skillLevel.set1Rep = 0;
                }
            }
            break;
        }
        case 'B':
        {
            skillLevel.runSpeed2 = player->getBaseSpeed();
            skillLevel.set2Notify = 0;
            if (eval == PASS || forced == PASS)
            {
                skillLevel.set2Rep++;
                if (skillLevel.set2Rep >= globals.set2Repetitions)
                {
                    skillLevel.runSpeed2 *= globals.nlevelSpeedModifier;
                    skillLevel.runSpeed2 = Util::clamp(skillLevel.runSpeed2, globals.minCamSpeed, globals.maxCamSpeed);
                    skillLevel.set2Rep = 0;
                    skillLevel.set2Notify = 1;
                    skillLevel.set2++;
                }
            }
            else if (eval == FAIL || forced == FAIL)
            {
                skillLevel.set2Rep--;
                if (skillLevel.set2Rep < 0)
                {
                    if (skillLevel.set2 > 1)
                    {
                        skillLevel.set2Rep = globals.set2Repetitions - 1;
                        skillLevel.set2--;
                    }
                    else
                        skillLevel.set2Rep = 0;
                }
            }
            break;
        }
        case 'C':
        {
            skillLevel.runSpeed3 = player->getBaseSpeed();
            skillLevel.set3Notify = 0;
            if (eval == PASS || forced == PASS)
            {
                skillLevel.set3Rep++;
                if (skillLevel.set3Rep >= globals.set3Repetitions)
                {
                    skillLevel.runSpeed3 *= globals.nlevelSpeedModifier;
                    skillLevel.runSpeed3 = Util::clamp(skillLevel.runSpeed3, globals.minCamSpeed, globals.maxCamSpeed);
                    skillLevel.set3Rep = 0;
                    skillLevel.set3Notify = 1;
                    skillLevel.set3++;
                }
            }
            else if (eval == FAIL || forced == FAIL)
            {
                skillLevel.set3Rep--;
                if (skillLevel.set3Rep < 0)
                {
                    if (skillLevel.set3 > 1)
                    {
                        skillLevel.set3Rep = globals.set3Repetitions - 1;
                        skillLevel.set3--;
                    }
                    else
                        skillLevel.set3Rep = 0;
                }
            }
            break;
        }
        default:
            break;
    }
    player->setSkillLevel(skillLevel);
    player->saveStage(globals.logPath);
    player->saveActions(globals.actionPath);
    player->saveSession(globals.sessionPath);
    player->saveProgress(globals.savePath, globals.currStageID);
}