#include "OgreApp.h"

#include "Tunnel.h"
#include "Player.h"
#include "Hud.h"
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
#ifdef USE_RTSHADER_SYSTEM
    mShaderGenerator->removeSceneManager(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    
    finalizeRTShaderSystem();
#endif
    delete OgreFramework::getSingletonPtr();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#ifdef USE_RTSHADER_SYSTEM

/*-----------------------------------------------------------------------------
 | Initialize the RT Shader system.
 -----------------------------------------------------------------------------*/
bool OgreApp::initializeRTShaderSystem(SceneManager* sceneMgr)
{
    if (RTShader::ShaderGenerator::initialize())
    {
        mShaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
        
        mShaderGenerator->addSceneManager(sceneMgr);
        
        // Setup core libraries and shader cache path.
        StringVector groupVector = ResourceGroupManager::getSingleton().getResourceGroups();
        StringVector::iterator itGroup = groupVector.begin();
        StringVector::iterator itGroupEnd = groupVector.end();
        String shaderCoreLibsPath;
        String shaderCachePath;
        
        for (; itGroup != itGroupEnd; ++itGroup)
        {
            ResourceGroupManager::LocationList resLocationsList = ResourceGroupManager::getSingleton().getResourceLocationList(*itGroup);
            ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
            ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
            bool coreLibsFound = false;
            
            // Try to find the location of the core shader lib functions and use it
            // as shader cache path as well - this will reduce the number of generated files
            // when running from different directories.
            for (; it != itEnd; ++it)
            {
                if ((*it)->archive->getName().find("RTShaderLib") != String::npos)
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
        MaterialManager::getSingleton().addListener(mMaterialMgrListener);
    }
    
    return true;
}

/*-----------------------------------------------------------------------------
 | Finalize the RT Shader system.
 -----------------------------------------------------------------------------*/
void OgreApp::finalizeRTShaderSystem()
{
    // Restore default scheme.
    MaterialManager::getSingleton().setActiveScheme(MaterialManager::DEFAULT_SCHEME_NAME);
    
    // Unregister the material manager listener.
    if (mMaterialMgrListener != NULL)
    {
        MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
        delete mMaterialMgrListener;
        mMaterialMgrListener = NULL;
    }
    
    // Finalize RTShader system.
    if (mShaderGenerator != NULL)
    {
        RTShader::ShaderGenerator::finalize();
        mShaderGenerator = NULL;
    }
}
#endif // USE_RTSHADER_SYSTEM

void OgreApp::startDemo(void* uiWindow, void* uiView, unsigned int width, unsigned int height, const char* name, MusicMode musica)
{
    totalElapsed = 0.0;
    globals.initPaths(name);
    musicMode = musica;
    
	new OgreFramework();
#if !defined(OGRE_IS_IOS)
    if (!OgreFramework::getSingletonPtr()->initOgre(uiWindow, uiView, width, height, this, this, this))
        return;
#else
	if (!OgreFramework::getSingletonPtr()->initOgre(uiWindow, uiView, width, height, this))
		return;
#endif

    
	m_bShutdown = false;
    
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
	
#ifdef USE_RTSHADER_SYSTEM
    initializeRTShaderSystem(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    MaterialPtr baseWhite = MaterialManager::getSingleton().getByName("BaseWhite", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
    baseWhite->setLightingEnabled(false);
    mShaderGenerator->createShaderBasedTechnique(
                                                 "BaseWhite",
                                                 MaterialManager::DEFAULT_SCHEME_NAME,
                                                 RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
    mShaderGenerator->validateMaterial(RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                       "BaseWhite");
    baseWhite->getTechnique(0)->getPass(0)->setVertexProgram(
                                                             baseWhite->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
    baseWhite->getTechnique(0)->getPass(0)->setFragmentProgram(
                                                               baseWhite->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
    
    // creates shaders for base material BaseWhiteNoLighting using the RTSS
    mShaderGenerator->createShaderBasedTechnique(
                                                 "BaseWhiteNoLighting",
                                                 MaterialManager::DEFAULT_SCHEME_NAME,
                                                 RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
    mShaderGenerator->validateMaterial(RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                       "BaseWhiteNoLighting");
    MaterialPtr baseWhiteNoLighting = MaterialManager::getSingleton().getByName("BaseWhiteNoLighting", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
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
    
    Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "sphereMesh", 1.0, 8, 8);
    Util::createCylinder(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "cylinderMesh", 1.0, 1.0, 8);
    Util::createDiamond(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "diamondMesh", 1.0, 1.0);
    Util::createBox(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "boxMesh", 1.0, 1.0, 1.0);
    Util::createPlane(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "planeMesh", 1.0, 1.0);
    Util::createDefaultSegments(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    //Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "sphereMesh", 1.0, 16, 16);
    //Util::createCylinder(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "cylinderMesh", 1.0, 1.0, 16);
    //Util::createDiamond(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "diamondMesh", 1.0, 1.0);
    //Util::createPlane(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "planeMesh", 1.0, 1.0);
    
	origin = Vector3(0, 0, 0);
    
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(origin.x, origin.y, origin.z + globals.tunnelSegmentDepth / 2));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(origin);
    
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
    if (!player->loadProgress(globals.savePath))
        std::cout << "WARNING: Save File could not be loaded correctly" << std::endl;
    
    tunnel = NULL;
    hud = new Hud();
    
    setLevel(-1, -1);
    
    Light* lightMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->createLight("Light");
    lightMain->setDiffuseColour(1.0, 1.0, 1.0);
    lightMain->setSpecularColour(1.0, 1.0, 1.0);
    //lightMain->setAttenuation(10, 1.0, 0.0001, 0.0);
    lightNodeMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("lightNode");
    lightNodeMain->attachObject(lightMain);
    lightNodeMain->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
    
    /*
     Light* lightSide = OgreFramework::getSingletonPtr()->m_pSceneMgrSide->createLight("Light");
     lightSide->setDiffuseColour(1.0, 1.0, 1.0);
     lightSide->setSpecularColour(1.0, 1.0, 1.0);
     lightNodeSide = OgreFramework::getSingletonPtr()->m_pSceneMgrSide->getRootSceneNode()->createChildSceneNode("lightNode");
     lightNodeSide->attachObject(lightSide);
     lightNodeSide->setPosition(OgreFramework::getSingletonPtr()->m_pCameraSide->getPosition());
    */
    
    sidebarMode = SIDEBAR_NONE;
    //setSidebar();
}

void OgreApp::setSidebar()
{
    Camera* m_pCameraMain = OgreFramework::getSingletonPtr()->m_pCameraMain;
    Camera* m_pCameraSide = OgreFramework::getSingletonPtr()->m_pCameraSide;
    
    Viewport* m_pViewportMain = OgreFramework::getSingletonPtr()->m_pViewportMain;
    Viewport* m_pViewportSide = OgreFramework::getSingletonPtr()->m_pViewportSide;
    
    switch (sidebarMode)
    {
        case SIDEBAR_NONE:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           float(globals.viewportMainWidth_modeNone) / globals.screenWidth,
                                           float(globals.viewportMainHeight_modeNone) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
             m_pCameraSide->setOrthoWindow(0.0, 0.0);
             m_pViewportSide->setDimensions(
             float(globals.viewportMainWidth_modeNone) / globals.screenWidth,
             0.0,
             float(globals.viewportSideWidth_modeNone) / globals.screenWidth,
             float(globals.viewportSideHeight_modeNone) / globals.screenHeight);
             m_pCameraSide->setAspectRatio(Real(0.0));
             
            break;
        case SIDEBAR_RIGHT:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           float(globals.viewportMainWidth_modeRight) / globals.screenWidth,
                                           float(globals.viewportMainHeight_modeRight) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
             m_pCameraSide->setPosition(Vector3(0, 0, 30));
             m_pCameraSide->lookAt(Vector3(0, 0, 0));
             m_pCameraSide->setNearClipDistance(1);
             m_pCameraSide->setOrthoWindow(10.0, 25.0);
             m_pViewportSide->setDimensions(
             float(globals.viewportMainWidth_modeRight) / globals.screenWidth,
             0.0,
             float(globals.viewportSideWidth_modeRight) / globals.screenWidth,
             float(globals.viewportSideHeight_modeRight) / globals.screenHeight);
             m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
             
            break;
        case SIDEBAR_BOTTOM_LTR:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           float(globals.viewportMainWidth_modeBottom) / globals.screenWidth,
                                           float(globals.viewportMainHeight_modeBottom) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
             m_pCameraSide->setPosition(Vector3(0, 0, 30));
             m_pCameraSide->lookAt(Vector3(0, 0, 0));
             m_pCameraSide->setNearClipDistance(1);
             m_pCameraSide->roll(Degree(-90));
             m_pCameraSide->setOrthoWindow(5, 2.5);
             m_pViewportSide->setDimensions(
             0.0,
             float(globals.viewportMainHeight_modeBottom) / globals.screenHeight,
             float(globals.viewportSideWidth_modeBottom) / globals.screenWidth,
             float(globals.viewportSideHeight_modeBottom) / globals.screenHeight);
             m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
             
            break;
        case SIDEBAR_BOTTOM_RTL:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           float(globals.viewportMainWidth_modeBottom) / globals.screenWidth,
                                           float(globals.viewportMainHeight_modeBottom) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
             m_pCameraSide->setPosition(Vector3(0, 0, 30));
             m_pCameraSide->lookAt(Vector3(0, 0, 0));
             m_pCameraSide->setNearClipDistance(1);
             m_pCameraSide->roll(Degree(90));
             m_pCameraSide->setOrthoWindow(5, 2.5);
             m_pViewportSide->setDimensions(
             0.0,
             float(globals.viewportMainHeight_modeBottom) / globals.screenHeight,
             float(globals.viewportSideWidth_modeBottom) / globals.screenWidth,
             float(globals.viewportSideHeight_modeBottom) / globals.screenHeight);
             m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
             
            break;
    }
}

void OgreApp::update(float elapsed)
{
    totalElapsed += elapsed;
    
    OgreFramework::getSingletonPtr()->m_pSoundMgr->update(elapsed);
        
    // Update the game state
    if (!pause) {
        player->update(tunnel, hud, elapsed);
        if (tunnel->needsCleaning())
        {
            // Generate a new tunnel because we are done with current one
            setLevel(-1, -1);
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
    
    // Graphical view changes
    lightNodeMain->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
    
    Quaternion camRot = player->getCombinedRotAndRoll();
    OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
    OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(camRot);
    
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->setOrientation(player->getCombinedRotAndRoll());
    
    hud->update(tunnel, player, elapsed);
}

void OgreApp::setLevel(int n, int c, Evaluation forced)
{
    globals.clearMessage();
    
    Vector3 newOrigin = Vector3(0, 0, 0) + globals.tunnelReferenceForward * (globals.tunnelSegmentWidth / 2);
    Quaternion newRot = Quaternion(1, 0, 0, 0);
    Vector3 newForward = globals.tunnelReferenceForward;
    int oldNBack = tunnel ? tunnel->getNBack() : 0;
    GameMode oldGameMode = tunnel ? tunnel->getMode() : GAME_TIMED;
    Evaluation eval = tunnel ? tunnel->getEval() : EVEN;
    
    if (n >= 0) // For Debugging keys
    {
        if (tunnel)
        {
            player->saveProgress(globals.savePath, globals.currStageID);
            delete tunnel;
        }
        
        tunnel = new Tunnel(
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(),
            newOrigin + newForward * (globals.tunnelSegmentWidth / 2),
            globals.tunnelSegmentWidth,
            globals.tunnelSegmentDepth,
            globals.tunnelMinAngleTurn,
            globals.tunnelMaxAngleTurn,
            (GameMode)globals.gameMode,
            n,
            c,
            SOUTH,
            globals.tunnelSegmentsPerSection,
            globals.tunnelSegmentsPerPod,
            globals.tunnelSegmentsPerDistractors,
            globals.podObjects);
        tunnel->constructTunnel(globals.tunnelSections, newRot, (GameMode)globals.gameMode != GAME_NAVIGATION);
    }
    else // Automatically determine
    {
        GameMode nmode;
        int nlevel;
        int ncontrol;
        
        if (tunnel)
        {
            player->saveStage(globals.logPath);
            Evaluation eval = tunnel->getEval();
            player->saveProgress(globals.savePath, globals.currStageID);
            if (eval == PASS || forced == PASS) globals.currStageID++;
            else if (eval == FAIL || forced == FAIL) globals.currStageID--;
            if (globals.currStageID < 1) globals.currStageID = 1;
            delete tunnel;
        }
        
        // Load configuration
        if (!globals.loadConfig(globals.currStageID))
        {
            std::cout << "WARNING: Config File could not be loaded correctly" << std::endl;
            globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
        }
        
        nlevel = globals.nback;
        ncontrol = globals.control;
        nmode = (GameMode)globals.gameMode;

        tunnel = new Tunnel(
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(),
            newOrigin + newForward * (globals.tunnelSegmentWidth / 2),
            globals.tunnelSegmentWidth,
            globals.tunnelSegmentDepth,
            globals.tunnelMinAngleTurn,
            globals.tunnelMaxAngleTurn,
            nmode,
            nlevel,
            ncontrol,
            SOUTH,
            globals.tunnelSegmentsPerSection,
            globals.tunnelSegmentsPerPod,
            globals.tunnelSegmentsPerDistractors,
            globals.podObjects);
        tunnel->constructTunnel(globals.tunnelSections, newRot, nmode != GAME_NAVIGATION);
    }
    
    // If nback is same then panels are changing, keep speed same
    player->setCamPos(newOrigin);
    player->setCamRot(newRot);
    player->setDesireRot(newRot);
    player->newTunnel(tunnel, musicMode == MUSIC_ENABLED);
    
    hud->init(tunnel, player);
    
    Plane plane;
    switch (globals.setSkyBox)
    {
        case 0:
            plane.d = 3000;
            plane.normal = Ogre::Vector3(0, 0, 1);
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, "General/SpaceSkyPlane", 80, 4, true);
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue::ZERO, 0.0, 300.0, 600.0);
            OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetToInitialState();
            break;
        case 1:
            plane.d = 160;
            plane.normal = Ogre::Vector3(0, 0, 1);
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, "General/TestSkyPlane0", 1, 1, true);
            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(1.0, 1.0, 1.0, 0.0), 0.0, 300.0, 600.0);
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
    
    // Stop the game
    setPause(true);
}

void OgreApp::setPause(bool value)
{
    if (value)
    {
        pause = value;
        OgreFramework::getSingletonPtr()->m_pSoundMgr->stopAllSounds();
        player->pause();
        if (player->getTotalElapsed() > globals.sessionTime)
        {
            globals.setMessage("Times Up for Today!\nPlease check in before you leave.", MESSAGE_FINAL);
            sessionOver = true;
        }
        else
            globals.setMessage("Swipe to Continue - Stage " + Util::toStringInt(globals.currStageID), MESSAGE_NORMAL);
        Ogre::ControllerManager::getSingleton().setTimeFactor(0);
    }
    else if (!sessionOver)
    {
        pause = value;
        player->unpause();
            
        globals.clearMessage();
        Ogre::ControllerManager::getSingleton().setTimeFactor(1);
    }
}

bool OgreApp::isPaused() const
{
    return pause;
}

bool OgreApp::isSessionOver() const
{
    return sessionOver;
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

void OgreApp::activateTouchPress(float x, float y)
{
    //player->setKeySpace(true);
    return true;
    
}

void OgreApp::activateTouchRelease(float x, float y)
{
    //player->setKeySpace(false);
    return true;
}

void OgreApp::activateTouchMove(float dx, float dy)
{
    return true;
}

void OgreApp::activatePerformLeftMove()
{
    if (pause)
    {
        setPause(false);
    }
    else
    {
        if (player->setVineDirRequest(Util::rightOf(player->getVineDest()), tunnel) && !tunnel->isDone())
        {
            float val = player->getDesireRoll();
            player->setDesireRoll(val + 45);
        }
    }
}

void OgreApp::activatePerformRightMove()
{
    if (pause)
    {
        setPause(false);
    }
    else
    {
        if (player->setVineDirRequest(Util::leftOf(player->getVineDest()), tunnel) && !tunnel->isDone())
        {
            float val = player->getDesireRoll();
            player->setDesireRoll(val - 45);
        }
    }
}

void OgreApp::activatePerformDoubleTap(float x, float y)
{
    if (!pause) player->performShockwave(tunnel);
}

void OgreApp::activatePerformSingleTap(float x, float y)
{
    if (y <= 300 && x <= globals.screenWidth / 2) {
        setLevel(-1,-1, FAIL);
    } else if (y <= 300 && x > globals.screenWidth / 2) {
        setLevel(-1,-1, PASS);
    }
}

void OgreApp::activatePerformPinch()
{
    setPause(!pause);
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
            player->setKeyLeft(true);
            if (player->checkPerformLeftMove(true))
                activatePerformLeftMove();
            break;
        }
        case OIS::KC_RIGHT:
        {
            player->setKeyRight(true);
            player->resetCursorMoved();
            if (player->checkPerformRightMove(true))
                activatePerformRightMove();
            break;
        }
        case OIS::KC_UP:
        {
            player->setKeyUp(true);
            if (player->getSpeedControl() == SPEED_CONTROL_FLEXIBLE)
            {
                int s = player->getBaseSpeed();
                if (s + 1 <= globals.maxCamSpeed)
                    s++;
                player->setBaseSpeed(s);
            }
            break;
        }
        case OIS::KC_DOWN:
        {
            player->setKeyDown(true);
            if (player->getSpeedControl() == SPEED_CONTROL_FLEXIBLE)
            {
                int s = player->getBaseSpeed();
                if (s - 1 >= globals.minCamSpeed)
                    s--;
                player->setBaseSpeed(s);
            }
            break;
        }
        case OIS::KC_P:
        {
            setPause(!pause);
            break;
        }
        case OIS::KC_MINUS:
        {
            setLevel(-1, -1, FAIL);
            break;
        }
        case OIS::KC_EQUALS:
        {
            setLevel(-1, -1, PASS);
            break;
        }
        case OIS::KC_X:
        {
            player->changeMovementMode();
            break;
        }
        case OIS::KC_C:
        {
            player->setShowCombo(player->getShowCombo() ? 0 : 1);
            break;
        }
        case OIS::KC_SPACE:
        {
            player->setKeySpace(true);
            activatePerformDoubleTap();
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
