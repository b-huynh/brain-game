#include "OgreApp.h"

#include "Tunnel.h"
#include "Player.h"
#include "Hud.h"
#include "LevelManager.h"
#include <fstream>
#include <ctime>
#include <cstdlib>

Util::ConfigGlobal globals;

OgreApp::OgreApp()
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreApp::~OgreApp()
{
#ifdef INCLUDE_RTSHADER_SYSTEM
    mShaderGenerator->removeSceneManager(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    
    destroyRTShaderSystem();
#endif
    delete OgreFramework::getSingletonPtr();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#ifdef INCLUDE_RTSHADER_SYSTEM

/*-----------------------------------------------------------------------------
 | Initialize the RT Shader system.
 -----------------------------------------------------------------------------*/
bool OgreApp::initialiseRTShaderSystem(Ogre::SceneManager* sceneMgr)
{
    if (Ogre::RTShader::ShaderGenerator::initialize())
    {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
        
        mShaderGenerator->addSceneManager(sceneMgr);
        
        // Setup core libraries and shader cache path.
        Ogre::StringVector groupVector = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
        Ogre::StringVector::iterator itGroup = groupVector.begin();
        Ogre::StringVector::iterator itGroupEnd = groupVector.end();
        Ogre::String shaderCoreLibsPath;
        Ogre::String shaderCachePath;
        
        for (; itGroup != itGroupEnd; ++itGroup)
        {
            Ogre::ResourceGroupManager::LocationList resLocationsList = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(*itGroup);
            Ogre::ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
            Ogre::ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
            bool coreLibsFound = false;
            
            // Try to find the location of the core shader lib functions and use it
            // as shader cache path as well - this will reduce the number of generated files
            // when running from different directories.
            for (; it != itEnd; ++it)
            {
                if ((*it)->archive->getName().find("RTShaderLib") != Ogre::String::npos)
                {
                    shaderCoreLibsPath = (*it)->archive->getName() + "/";
                    shaderCachePath = shaderCoreLibsPath;
                    coreLibsFound = true;
                    break;
                }
            }
            // Core libs path found in the current group.
            if (coreLibsFound)
                break;
        }
        
        // Core shader libs not found -> shader generating will fail.
        if (shaderCoreLibsPath.empty())
            return false;
        
        // Create and register the material manager listener.
        mMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener(mShaderGenerator);
        Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
    }
    
    return true;
}

/*-----------------------------------------------------------------------------
 | Destroy the RT Shader system.
 -----------------------------------------------------------------------------*/
void OgreApp::destroyRTShaderSystem()
{
    // Restore default scheme.
    Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
    
    // Unregister the material manager listener.
    if (mMaterialMgrListener != NULL)
    {
        Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
        delete mMaterialMgrListener;
        mMaterialMgrListener = NULL;
    }
    
    // Destroy RTShader system.
    if (mShaderGenerator != NULL)
    {
        Ogre::RTShader::ShaderGenerator::destroy();
        mShaderGenerator = NULL;
    }
}
#endif // INCLUDE_RTSHADER_SYSTEM

#if !defined(OGRE_IS_IOS)
void OgreApp::startDemo(const char* name, MusicMode musica)
#else
void OgreApp::startDemo(void* uiWindow, void* uiView, unsigned int width, unsigned int height, const char* name)
#endif
{
    globals.playerName = name;
    gameState = STATE_PLAY;
    musicMode = MUSIC_ENABLED;
    
	new OgreFramework();
#if !defined(OGRE_IS_IOS)
    if (!OgreFramework::getSingletonPtr()->initOgre(this, this, this))
        return;
#else
	if (!OgreFramework::getSingletonPtr()->initOgre(uiWindow, uiView, width, height))
		return;
#endif

    
	m_bShutdown = false;
    
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
	
#ifdef INCLUDE_RTSHADER_SYSTEM
    initialiseRTShaderSystem(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    Ogre::MaterialPtr baseWhite = Ogre::MaterialManager::getSingleton().getByName("BaseWhite", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
    baseWhite->setLightingEnabled(false);
    mShaderGenerator->createShaderBasedTechnique(
                                                 "BaseWhite",
                                                 Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
                                                 Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
    mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                       "BaseWhite");
    baseWhite->getTechnique(0)->getPass(0)->setVertexProgram(
                                                             baseWhite->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
    baseWhite->getTechnique(0)->getPass(0)->setFragmentProgram(
                                                               baseWhite->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
    
    // creates shaders for base material BaseWhiteNoLighting using the RTSS
    mShaderGenerator->createShaderBasedTechnique(
                                                 "BaseWhiteNoLighting",
                                                 Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
                                                 Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
    mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                       "BaseWhiteNoLighting");
    Ogre::MaterialPtr baseWhiteNoLighting = Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
    baseWhiteNoLighting->getTechnique(0)->getPass(0)->setVertexProgram(
                                                                       baseWhiteNoLighting->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
    baseWhiteNoLighting->getTechnique(0)->getPass(0)->setFragmentProgram(
                                                                         baseWhiteNoLighting->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
#endif
    
	setupDemoScene();
#if !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
	runDemo();
#endif
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::setupDemoScene()
{
    seed = time(0);
    srand(seed);
    sessionOver = false;
    
    Util::generateMaterials();
    
    Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "sphereMesh", 1.0, 256, 4);
    Util::createCylinder(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "cylinderMesh", 1.0, 1.0, 8);
    Util::createDiamond(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "diamondMesh", 1.0, 1.0);
    Util::createBox(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "boxMesh", 1.0, 1.0, 1.0);
    Util::createPlane(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "planeMesh", 1.0, 1.0);
    Util::createDefaultSegments(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    
	origin = Vector3(0, 0, 0);
    
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(origin.x, origin.y, origin.z + globals.tunnelSegmentDepth / 2));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(origin);
    
    globals.initPaths();
    //if (!configStageType(globals.configPath, globals.configBackup, "globalConfig"))
    //    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
    
	player = new Player(
                        globals.playerName,
                        OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition(),
                        OgreFramework::getSingletonPtr()->m_pCameraMain->getOrientation(),
                        globals.initCamSpeed,
                        globals.vineTOffset,
                        SPEED_CONTROL_AUTO,
                        seed,
                        "vinezors" + Util::toStringInt(seed) + ".csv");
	player->addVine(new Vine(OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(), player->getCamPos(), globals.vineRadius));
    player->setSounds(true);
    player->setRunningSpeed(globals.set1StartingSpeed, globals.set2StartingSpeed, globals.set3StartingSpeed, globals.trialStartingSpeed, globals.startingNav);
    if (player->getName() != "subject999")
    {
        if (!player->loadProgress(globals.savePath))
            std::cout << "WARNING: Save File could not be loaded correctly" << std::endl;
    }
    else
    {
        // Special training session for space game
        PlayerLevel skill = player->getSkillLevel();
        skill.sessionID = 0;
        skill.set1 = 1;
        skill.set2 = 1;
        skill.set3 = 1;
        skill.set1Rep = 0;
        skill.set2Rep = 1;
        skill.set3Rep = 2;
        player->setSkillLevel(skill);
    }
    globals.initLogs(player->getSkillLevel().sessionID);
    
    tunnel = NULL;
    hud = new Hud();
    
    // Determine length of time
    //globals.sessionTime = (globals.sessionTimeMin + ((globals.sessionTimeMax - globals.sessionTimeMin) / globals.expectedNumSessions) * player->getSkillLevel().sessionID);
    
    std::cout << "Session Length: " << globals.sessionTime << std::endl;
    
    levelMgr = new LevelManager(player, globals.scheduleMain, globals.scheduleRepeat, globals.scheduleRepeatRandomPool);
    
    setLevel(EVEN);
    
//    OgreFramework::getSingletonPtr()->m_pCameraMain
    backSphere = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("backSphereNode");
    Entity* backContentEntity = backSphere->getCreator()->createEntity("backSphereEntity", "sphereMesh");
    backContentEntity->setMaterialName("General/PodUnknown");
    backSphere->attachObject(backContentEntity);
    backSphere->scale(globals.podHeadRadius * 4, globals.podHeadRadius * 4, globals.podHeadRadius);
    backSphere->translate(Vector3(0, 0, -globals.tunnelSegmentDepth * 20));
    
}

void OgreApp::update(float elapsed)
{
    OgreFramework::getSingletonPtr()->m_pSoundMgr->update(elapsed);
    
    switch (gameState)
    {
        case STATE_PLAY:
        {
            // Update the game state
            if (!pause) {
                player->update(elapsed);
                if (tunnel->needsCleaning())
                {
                    endLevel();
                    //// Generate a new tunnel because we are done with current one
                    if (gameState == STATE_PLAY)
                        setLevel();
                }
            } else {
#if !defined(OGRE_IS_IOS)
                // Navigation Debug Keys
                if (player->getKeyUp())
                    player->move(Vector3(player->getCamForward() * globals.initCamSpeed * elapsed));
                if (player->getKeyDown())
                    player->move(Vector3(player->getCamForward() * -globals.initCamSpeed * elapsed));
                if (player->getKeyLeft())
                    player->move(Vector3(player->getCamRight() * -globals.initCamSpeed * elapsed));
                if (player->getKeyRight())
                    player->move(Vector3(player->getCamRight() * globals.initCamSpeed * elapsed));
#endif
            }
            hud->update(elapsed);
            
            Quaternion camRot = player->getCombinedRotAndRoll();
            OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
            OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(camRot);
            
            backSphere->setPosition(player->getCamPos() + player->getCamForward() * globals.tunnelSegmentDepth * 20);
            backSphere->setOrientation(camRot);
            break;
        }
        case STATE_PROMPT:
        {
            break;
        }
    }
}

void OgreApp::endLevel(Evaluation forced)
{
    globals.clearMessage();
    
    if (tunnel)
    {
        tunnel->setCleaning(true);
        /*
        if (levelMgr->levelFinished(tunnel, forced))
        {
            gameState = STATE_PROMPT;
            globals.setMessage("Play Again?\n\n\n(Yes / No)\n\n\n<---     --->", MESSAGE_NORMAL);
        }
        else
            gameState = STATE_PLAY;
         */
        gameState = STATE_PLAY;
    }
}

void OgreApp::setLevel(Evaluation forced, bool forward)
{
    globals.clearMessage();
    
    std::vector<NavigationLevel> navLevels;
    if (tunnel)
    {
        levelMgr->updatePlayerSkill(tunnel, forced);
        if (forward)
            levelMgr->incrementSchedIndex();
        else
            levelMgr->decrementSchedIndex();
        navLevels = tunnel->getNavLevels();
        player->unlink();
        tunnel->unlink();
        hud->unlink();
    }
    
    // Don't gen a new tunnel if our schedule is over
    if (!predictSessionOver())
    {
        tunnel = levelMgr->getNextLevel(tunnel);
        if (navLevels.size() <= 0) // First stage
        {
            if (player->getName() == "subject999")
                globals.appendMessage("\n\nSession No: " + Util::toStringInt(player->getSkillLevel().sessionID), MESSAGE_NORMAL);
            else
                globals.appendMessage("\n\nSession No: " + Util::toStringInt(player->getSkillLevel().sessionID + 1), MESSAGE_NORMAL);
        }
    }
    // Link/Relink Pointers
    tunnel->link(player, hud);
    player->link(tunnel, hud);
    hud->init(tunnel, player);
    
    if (!predictSessionOver())
    {
        // If repeated, repeat the same navigation level
        if (levelMgr->isRepeatingPhase())
            tunnel->setNavigationLevels(navLevels);
        else
            tunnel->setNavigationLevels();
        tunnel->constructTunnel(globals.tunnelSections);
        // If nback is same then panels are changing, keep speed same
        player->setCamPos(tunnel->getStart() + tunnel->getCurrent()->getForward() * globals.tunnelSegmentDepth);
        player->setCamRot(tunnel->getCurrent()->getQuaternion());
        player->setDesireRot(player->getCamRot());
        player->newTunnel(musicMode == MUSIC_ENABLED);
    
        Plane plane;
        switch (globals.setSkyBox)
        {
            case 0:
                break;
            case 1:
                plane.d = 190;
                plane.normal = Ogre::Vector3(0, 0, 1);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, "General/TestSkyPlane0", 1, 1, true);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(1.0, 1.0, 1.0, 0.0), 0.0, 300.0,  600.0);
                OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetToInitialState();
                break;
            default:
                plane.d = 3000;
                plane.normal = Ogre::Vector3(0, 0, 1);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, "General/SpaceSkyPlane", 80, 4, true);
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue::ZERO, 0.0, 300.0, 600.0);
                OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
                OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetToInitialState();
                break;
        }
    }
    
    // Stop the game
    setPause(true);
}

float OgreApp::getTotalElapsed() const
{
    return player->getTotalElapsed();
}

void OgreApp::setPause(bool value)
{
    if (value)
    {
        pause = value;
        OgreFramework::getSingletonPtr()->m_pSoundMgr->pauseAllSounds();
        player->pause();
        if (predictSessionOver())
        {
            if (player->getName() == "subject999")
                globals.setMessage("Training is complete!\nPlease check in.\n\nDouble tap to exit.", MESSAGE_FINAL);
            else
            {
                globals.setMessage("That's it for Today!\nPlease check in before you leave.\n\nYou completed: " + Util::toStringInt(player->getNumStagesWon()) + " stages!\n\nDouble tap to exit.", MESSAGE_FINAL);
            }
            endGame();
        }
        else
        {
            globals.appendMessage("\n\nSwipe to Continue", MESSAGE_NORMAL);
#ifdef DEBUG_MODE
            globals.appendMessage("\n\n" + player->getSkillLevel().getCurrentStats(), MESSAGE_NORMAL);
            globals.appendMessage("\n\n" + Util::toStringInt(globals.sessionTime - getTotalElapsed()), MESSAGE_NORMAL);
#endif
        }
        Ogre::ControllerManager::getSingleton().setTimeFactor(0);
    }
    else if (!sessionOver)
    {
        pause = value;
        player->unpause();
        OgreFramework::getSingletonPtr()->m_pSoundMgr->resumeAllPausedSounds();
            
        globals.clearMessage();
        Ogre::ControllerManager::getSingleton().setTimeFactor(1);
    }
}

bool OgreApp::isPaused() const
{
    return pause;
}

bool OgreApp::predictSessionOver() const
{
    return sessionOver ||
    ((!tunnel || (tunnel->getTotalElapsed() <= 0.0 || tunnel->needsCleaning())) && // At the beginning or end of stages only
            ((getTotalElapsed() > globals.sessionTime && // Ran out of time, done with main schedule
            levelMgr->isDoneWithMainSchedule() &&  // , and not retrying a level
            !levelMgr->isRepeatingPhase()) ||
            (player->getName() == "subject999" && // First session exits early
             levelMgr->isDoneWithMainSchedule())));
}

void OgreApp::endGame()
{
    if (!sessionOver)
    {
        player->saveStage(globals.logPath);
        player->saveProgress(globals.savePath, levelMgr->isDoneWithMainSchedule());
        sessionOver = true;
    }
}

void OgreApp::requestResize()
{
    if (hud) hud->setOverlay();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::runDemo()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");
    
	float timeSinceLastFrame = 0;
	float startTime = 0;
    
    OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
    
#if (!defined(OGRE_IS_IOS)) && !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
	while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown())
	{
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;
        
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		WindowEventUtilities::messagePump();
#endif
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
            
#if !OGRE_IS_IOS
			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
#endif
			OgreFramework::getSingletonPtr()->m_pMouse->capture();
            
			OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
            
			timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}
#endif
    
#if !defined(OGRE_IS_IOS)
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
#endif
}

void OgreApp::activatePerformLeftMove()
{
    player->addAction(ACTION_SWIPE_LEFT);
    switch (gameState)
    {
        case STATE_PLAY:
        {
            if (pause)
            {
                setPause(false);
            }
            else
            {
                if (player->setVineDirRequest(Util::rightOf(player->getVineDest())) && !tunnel->isDone())
                {
                    float val = player->getDesireRoll();
                    player->setDesireRoll(val + 45);
                }
            }
            break;
        }
        case STATE_PROMPT:
        {
            gameState = STATE_PLAY;
            levelMgr->repeatPreviousPhase();
            setLevel();
            break;
        }
    }
}

void OgreApp::activatePerformRightMove()
{
    player->addAction(ACTION_SWIPE_RIGHT);
    switch (gameState)
    {
        case STATE_PLAY:
        {
            if (pause)
            {
                setPause(false);
            }
            else
            {
                if (player->setVineDirRequest(Util::leftOf(player->getVineDest())) && !tunnel->isDone())
                {
                    float val = player->getDesireRoll();
                    player->setDesireRoll(val - 45);
                }
            }
            break;
        }
        case STATE_PROMPT:
        {
            gameState = STATE_PLAY;
            setLevel(FAIL);
            break;
        }
    }
}

void OgreApp::activatePerformDoubleTap(float x, float y)
{
    player->addAction(ACTION_DOUBLE_TAP);
    if (sessionOver)
        OgreFramework::getSingletonPtr()->requestOgreShutdown();
}

void OgreApp::activatePerformSingleTap(float x, float y)
{
    player->addAction(ACTION_SINGLE_TAP);
    if (x >= globals.pauseButton_posX && x <= globals.pauseButton_posX + globals.pauseButton_width &&
        y >= globals.pauseButton_posY && y <= globals.pauseButton_posY + globals.pauseButton_posY) {
        if (!pause) setPause(true);
    }
#ifdef DEBUG_MODE
    else if (y <= 100 && x <= globals.screenWidth / 2) {
        setLevel(EVEN, false);
    } else if (y <= 100 && x > globals.screenWidth / 2) {
        setLevel(EVEN);
    }
#endif
    else if (gameState == STATE_PLAY && !pause)
        player->performShockwave();
}

void OgreApp::activatePerformPinch()
{
    //player->addAction(ACTION_PINCH);
    //setPause(!pause);
}

void OgreApp::activatePerformBeginLongPress()
{
    //player->addAction(ACTION_TAP_HOLD);
    //if (gameState == STATE_PLAY && !pause && globals.boostEnabled) player->performBoost();
}

void OgreApp::activatePerformEndLongPress()
{
}

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
bool OgreApp::mouseMoved(const OIS::MouseEvent &evt)
{
    if (pause) {
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
    else
    {
        //if (player->getMouseLeft())
        {
            player->checkCursorMove(evt.state.X.rel, evt.state.Y.rel);
            if (player->checkPerformLeftMove(false))
            {
                activatePerformLeftMove();
                player->setCursorMoved();
            }
            else if (player->checkPerformRightMove(false))
            {
                activatePerformRightMove();
                player->setCursorMoved();
            }
        }
    }
    return true;
}

bool OgreApp::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            player->setMouseLeft(true);
            break;
        default:
            break;
    }
    return true;
}

bool OgreApp::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            player->setMouseLeft(false);
            player->resetCursorMoved();
            break;
        default:
            break;
    }
    return true;
}
#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
    switch (keyEventRef.key)
    {
        case OIS::KC_LEFT:
        {
            break;
        }
        case OIS::KC_RIGHT:
        {
            break;
        }
        case OIS::KC_UP:
        {
            break;
        }
        case OIS::KC_DOWN:
        {
            break;
        }
        case OIS::KC_P:
        {
            setPause(!pause);
            break;
        }
        case OIS::KC_MINUS:
        {
            endLevel(FAIL);
            setLevel(FAIL);
            break;
        }
        case OIS::KC_EQUALS:
        {
            endLevel(PASS);
            setLevel(PASS);
            break;
        }
        case OIS::KC_X:
        {
            break;
        }
        case OIS::KC_C:
        {
            break;
        }
        case OIS::KC_SPACE:
        {
            player->setKeySpace(true);
            activatePerformSingleTap(0, 0);
            break;
        }
        case OIS::KC_Z:
        {
            player->setKeySpace(true);
            activatePerformSingleTap(0, 0);
            break;
        }
        default:
            break;
    }
#endif
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    
    switch (keyEventRef.key)
    {
        case OIS::KC_LEFT:
        {
            player->setKeyLeft(false);
            break;
        }
        case OIS::KC_RIGHT:
        {
            player->setKeyRight(false);
            break;
        }
        case OIS::KC_UP:
        {
            player->setKeyUp(false);
            break;
        }
        case OIS::KC_DOWN:
        {
            player->setKeyDown(false);
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
#endif
    
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||


void OgreApp::preViewportUpdate(const Ogre::RenderTargetViewportEvent & evt)
{
    if (evt.source == OgreFramework::getSingletonPtr()->m_pViewportSide)
    {
        hud->hideOverlays();
    }
}

void OgreApp::postViewportUpdate(const Ogre::RenderTargetViewportEvent & evt)
{
    hud->showOverlays();
}