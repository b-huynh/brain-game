#include "OgreDemoApp.h"

#include "Tunnel.h"
#include "Player.h"
#include "Hud.h"
#include <fstream>
#include <ctime>
#include <cstdlib>

Util::ConfigGlobal globals;

DemoApp::DemoApp()
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::~DemoApp()
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
bool DemoApp::initializeRTShaderSystem(SceneManager* sceneMgr)
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
void DemoApp::finalizeRTShaderSystem()
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

void DemoApp::startDemo(const char* name, MusicMode musica)
{
    totalElapsed = 0.0;
    globals.initPaths(name);
    musicMode = musica;
    
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, this, this))
		return;
    
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

void DemoApp::setupDemoScene()
{
    progressionMode = (ProgressionMode)globals.progressionMode;
    
    seed = time(0);
    srand(seed);
    
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyBox(true, "Examples/SpaceSkyBox", 5000, true);
    
    Util::generateMaterials();
    
    Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "sphereMesh", 1.0, 16, 16);
    Util::createCylinder(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "cylinderMesh", 1.0, 1.0, 16);
    Util::createDiamond(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "diamondMesh", 1.0, 1.0);
    Util::createBox(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "boxMesh", 1.0, 1.0, 1.0);
    Util::createPlane(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "planeMesh", 1.0, 1.0);
    //Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "sphereMesh", 1.0, 16, 16);
    //Util::createCylinder(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "cylinderMesh", 1.0, 1.0, 16);
    //Util::createDiamond(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "diamondMesh", 1.0, 1.0);
    //Util::createPlane(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "planeMesh", 1.0, 1.0);
    
	origin = Vector3(0, 0, 0);
    
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(origin.x, origin.y, origin.z + globals.tunnelSegmentDepth / 2));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(origin);
    
	player = new Player(
                        globals.playerName,
                        PlayerLevel(),
                        OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition(),
                        OgreFramework::getSingletonPtr()->m_pCameraMain->getOrientation(),
                        globals.initCamSpeed,
                        globals.vineTOffset,
                        SPEED_CONTROL_AUTO,
                        seed,
                        "vinezors" + Util::toStringInt(seed) + ".csv");
	player->addVine(new Vine(OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(), player->getCamPos(), globals.vineRadius));
    player->setSounds(progressionMode != SIMPLE_PROGRESSIVE);
    player->setConfigValues();
    
    hud = new Hud();
    
    setLevel(-1, -1, true);
    
    Light* lightMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->createLight("Light");
    lightMain->setDiffuseColour(1.0, 1.0, 1.0);
    lightMain->setSpecularColour(1.0, 1.0, 1.0);
    //lightMain->setAttenuation(10, 1.0, 0.0001, 0.0);
    lightNodeMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("lightNode");
    lightNodeMain->attachObject(lightMain);
    lightNodeMain->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
    
     Light* lightSide = OgreFramework::getSingletonPtr()->m_pSceneMgrSide->createLight("Light");
     lightSide->setDiffuseColour(1.0, 1.0, 1.0);
     lightSide->setSpecularColour(1.0, 1.0, 1.0);
     lightNodeSide = OgreFramework::getSingletonPtr()->m_pSceneMgrSide->getRootSceneNode()->createChildSceneNode("lightNode");
     lightNodeSide->attachObject(lightSide);
     lightNodeSide->setPosition(OgreFramework::getSingletonPtr()->m_pCameraSide->getPosition());
    
    Ogre::TexturePtr rtt_texture = OgreFramework::getSingletonPtr()->m_pTextureMgr->createManual("RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, OgreFramework::getSingletonPtr()->m_pRenderWnd->getWidth(), OgreFramework::getSingletonPtr()->m_pRenderWnd->getHeight(), 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
    
    sidebarMode = SIDEBAR_NONE;
    setSidebar();
}

void DemoApp::setSidebar()
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
                                           double(globals.viewportMainWidth_modeNone) / globals.screenWidth,
                                           double(globals.viewportMainHeight_modeNone) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
             m_pCameraSide->setOrthoWindow(0.0, 0.0);
             m_pViewportSide->setDimensions(
             double(globals.viewportMainWidth_modeNone) / globals.screenWidth,
             0.0,
             double(globals.viewportSideWidth_modeNone) / globals.screenWidth,
             double(globals.viewportSideHeight_modeNone) / globals.screenHeight);
             m_pCameraSide->setAspectRatio(Real(0.0));
             
            break;
        case SIDEBAR_RIGHT:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           double(globals.viewportMainWidth_modeRight) / globals.screenWidth,
                                           double(globals.viewportMainHeight_modeRight) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
             m_pCameraSide->setPosition(Vector3(0, 0, 30));
             m_pCameraSide->lookAt(Vector3(0, 0, 0));
             m_pCameraSide->setNearClipDistance(1);
             m_pCameraSide->setOrthoWindow(10.0, 25.0);
             m_pViewportSide->setDimensions(
             double(globals.viewportMainWidth_modeRight) / globals.screenWidth,
             0.0,
             double(globals.viewportSideWidth_modeRight) / globals.screenWidth,
             double(globals.viewportSideHeight_modeRight) / globals.screenHeight);
             m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
             
            break;
        case SIDEBAR_BOTTOM_LTR:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           double(globals.viewportMainWidth_modeBottom) / globals.screenWidth,
                                           double(globals.viewportMainHeight_modeBottom) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
             m_pCameraSide->setPosition(Vector3(0, 0, 30));
             m_pCameraSide->lookAt(Vector3(0, 0, 0));
             m_pCameraSide->setNearClipDistance(1);
             m_pCameraSide->roll(Degree(-90));
             m_pCameraSide->setOrthoWindow(5, 2.5);
             m_pViewportSide->setDimensions(
             0.0,
             double(globals.viewportMainHeight_modeBottom) / globals.screenHeight,
             double(globals.viewportSideWidth_modeBottom) / globals.screenWidth,
             double(globals.viewportSideHeight_modeBottom) / globals.screenHeight);
             m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
             
            break;
        case SIDEBAR_BOTTOM_RTL:
            m_pViewportMain->setDimensions(
                                           0.0,
                                           0.0,
                                           double(globals.viewportMainWidth_modeBottom) / globals.screenWidth,
                                           double(globals.viewportMainHeight_modeBottom) / globals.screenHeight);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
             m_pCameraSide->setPosition(Vector3(0, 0, 30));
             m_pCameraSide->lookAt(Vector3(0, 0, 0));
             m_pCameraSide->setNearClipDistance(1);
             m_pCameraSide->roll(Degree(90));
             m_pCameraSide->setOrthoWindow(5, 2.5);
             m_pViewportSide->setDimensions(
             0.0,
             double(globals.viewportMainHeight_modeBottom) / globals.screenHeight,
             double(globals.viewportSideWidth_modeBottom) / globals.screenWidth,
             double(globals.viewportSideHeight_modeBottom) / globals.screenHeight);
             m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
             
            break;
    }
}

void DemoApp::update(double elapsed)
{
    totalElapsed += elapsed;
    
    OgreFramework::getSingletonPtr()->m_pSoundMgr->update(elapsed);

    // Update the game state
    if (!pause) {
        // Stop the game
        if (!pause && player->getTotalElapsed() > globals.sessionTime)
        {
            player->saveProgress(globals.logPath);
            globals.setMessage("Times Up for Today!\nPlease check in before you leave.", MESSAGE_FINAL);
            pause = true;
        }
        
        player->update(tunnel, hud, elapsed);
        if (tunnel->needsCleaning())
        {
            // Generate a new tunnel because we are done with current one
            setLevel(-1, -1);
            globals.setMessage("Swipe to Continue", MESSAGE_NORMAL);
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
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyBoxNode()->setOrientation(player->getCombinedRotAndRoll());
    
    hud->update(tunnel, player, elapsed);
}

void DemoApp::setLevel(int n, int c, bool init)
{
    pause = true;
    
    Vector3 newOrigin = tunnel ? tunnel->getEnd() : Vector3(0, 0, 0) + globals.tunnelReferenceForward * (globals.tunnelSegmentWidth / 2);
    Quaternion newRot = tunnel ? tunnel->getBack()->getQuaternion() : Quaternion(1, 0, 0, 0);
    Vector3 newForward = tunnel ? tunnel->getBack()->getForward() : globals.tunnelReferenceForward;
    int oldNBack = tunnel ? tunnel->getNBack() : 0;
    GameMode oldGameMode = tunnel ? tunnel->getMode() : GAME_TIMED;
    if (tunnel) delete tunnel;
    
    if (n >= 0) // For Debugging keys
    {
        PodType testType = rand() % 2 || progressionMode == SIMPLE_PROGRESSIVE ? POD_TYPE_COLOR : POD_TYPE_SOUND;
        bool haveColor = globals.gameMode == GAME_TIMED || (rand() % 2);
        bool haveSound = globals.gameMode == GAME_TIMED || (rand() % 2);
        
        std::cout << (testType == POD_TYPE_COLOR) << (testType == POD_TYPE_SOUND) << std::endl;
        std::cout << haveColor << haveSound << std::endl;
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
            player->getVineDir(),
            globals.tunnelSegmentsPerSection,
            globals.tunnelSegmentsPerPod,
            testType,
            haveColor,
            haveSound && progressionMode != SIMPLE_PROGRESSIVE,
            false);
        tunnel->constructTunnel(globals.tunnelSections, newRot, true);
        PlayerLevel skill = player->getLevel();
        skill.nback = n;
        skill.control = c;
        player->setLevel(skill);
    }
    else // Automatically determine
    {
        GameMode nmode;
        int nlevel;
        int ncontrol;
        bool loadStage = true;
        bool checkGrade = !init;
        
        if (oldGameMode == GAME_TIMED && progressionMode == DISTRIBUTIVE_ADAPTIVE)
        {
            checkGrade = false;
            if (oldNBack + 1 < player->getLevel().nback && oldNBack + 1 <= globals.timedRunNMax)
            {
                loadStage = false;
                nlevel = oldNBack + 1;
                ncontrol = 1;
                nmode = GAME_TIMED;
            }
            else
                loadStage = true;
        }
        
        bool pass = false;
        player->saveProgress(globals.logPath);
        if (checkGrade)
        {
            Evaluation eval = player->getEvaluation(tunnel);
            if (eval == PASS)
                globals.currStageID++;
            else if (eval == FAIL && globals.currStageID > 1)
                globals.currStageID--;
            
            player->saveStage(globals.savePath, globals.currStageID);
        }
        
        if (loadStage)
        {
            if (globals.loadConfig(globals.currStageID))
            {
                player->setConfigValues();
                nlevel = globals.nback;
                ncontrol = globals.control;
                nmode = (GameMode)globals.gameMode;
            }
            else
            {
                if (checkGrade) player->evaluatePlayerLevel(pass);
                nlevel = player->getLevel().nback;
                ncontrol = player->getLevel().control;
                nmode = tunnel->getMode();
            }
        }
        if (init)
        {
            nmode = (GameMode)globals.gameMode;
            nlevel = player->getLevel().nback;
            ncontrol = player->getLevel().nback;
            if (progressionMode == DISTRIBUTIVE_ADAPTIVE)
            {
                if (player->getLevel().nback > 2)
                {
                    nmode = GAME_TIMED;
                    nlevel = 2;
                    ncontrol = 1;
                }
            }
        }
        
        PodType testType = rand() % 2 || progressionMode == SIMPLE_PROGRESSIVE ? POD_TYPE_COLOR : POD_TYPE_SOUND;
        bool haveColor = nmode == GAME_TIMED || (rand() % 2);
        bool haveSound = nmode == GAME_TIMED || (rand() % 2);
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
            player->getVineDir(),
            globals.tunnelSegmentsPerSection,
            globals.tunnelSegmentsPerPod,
            testType,
            haveColor,
            haveSound && progressionMode != SIMPLE_PROGRESSIVE,
            false);
        tunnel->constructTunnel(globals.tunnelSections, newRot, true);
    }
    
    player->setCamPos(newOrigin);
    player->setCamRot(newRot);
    player->setDesireRot(newRot);
    player->saveCam();
    
    // If nback is same then panels are changing, keep speed same
    player->newTunnel(tunnel, musicMode == MUSIC_ENABLED, tunnel->getNBack() <= oldNBack,
                      tunnel->getMode() == GAME_TIMED || oldGameMode == GAME_TIMED);
    
    hud->init(tunnel, player);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::runDemo()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");
    
	double timeSinceLastFrame = 0;
	double startTime = 0;
    
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

void DemoApp::activatePerformLeftMove()
{
    if (pause)
    {
#if defined(OGRE_IS_IOS)
        if (player->getTotalElapsed() <= globals.sessionTime)
        {
            pause = !pause;
            player->revertCam();
            
            globals.clearMessage();
            player->setCursorMoved();
        }
#endif
    }
    else
    {
        if (player->setVineDirRequest(Util::rightOf(player->getVineDir()), tunnel))
        {
            double val = player->getDesireRoll();
            player->setDesireRoll(val + 45);
            player->setCursorMoved();
        }
    }
}

void DemoApp::activatePerformRightMove()
{
    if (pause)
    {
#if defined(OGRE_IS_IOS)
        if (player->getTotalElapsed() <= globals.sessionTime)
        {
            pause = !pause;
            player->revertCam();
            
            globals.clearMessage();
            player->setCursorMoved();
        }
#endif
    }
    else
    {
        if (player->setVineDirRequest(Util::leftOf(player->getVineDir()), tunnel))
        {
            double val = player->getDesireRoll();
            player->setDesireRoll(val - 45);
            player->setCursorMoved();
        }
    }
}

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
bool DemoApp::touchMoved(const OIS::MultiTouchEvent &evt)
{
    if (player->getKeyLeft())
    {
        player->checkCursorMove(evt.state.X.rel, evt.state.Y.rel);
        if (player->checkPerformLeftMove())
            activatePerformLeftMove();
        else if (player->checkPerformRightMove())
            activatePerformRightMove();
    }
    return true;
}
bool DemoApp::touchPressed(const OIS::MultiTouchEvent &evt)
{
    player->setKeyLeft(true);
    
    double axisY = evt.state.Y.abs;
    double axisX = evt.state.X.abs;
    
    /*
     if (pause && totalElapsed <= globals.sessionTime)
     {
     pause = !pause;
     player->saveCam();
     
     message = "";
     messageType = MESSAGE_NONE;
     label5->setCaption("");
     }
     */
    /*
     if (axisX <= 300 && axisY >= OgreFramework::getSingletonPtr()->m_pRenderWnd->getHeight() - 100)
     {
     OgreFramework::getSingletonPtr()->requestOgreShutdown();
     }
     else if (axisY <= 300 && axisX <= 300) {
     player->setHP(globals.HPPositiveLimit);
     player->evaluatePlayerLevel(true);
     setLevel(-1,-1);
     }
     /*
     else if (axisY <= 300 && axisX > 300) {
     pause = !pause;
     if (!pause) {
     player->revertCam();
     } else {
     player->saveCam();
     }
     }
     else
     player->setKeyLeft(true);
     */
    return true;
}
bool DemoApp::touchReleased(const OIS::MultiTouchEvent &evt)
{
    //if (OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt)) return true;
    player->setKeyLeft(false);
    player->resetCursorMove();
    return true;
}
bool DemoApp::touchCancelled(const OIS::MultiTouchEvent &evt)
{
    return true;
}
#else
bool DemoApp::mouseMoved(const OIS::MouseEvent &evt)
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
    return true;
}

bool DemoApp::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

bool DemoApp::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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
#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
    switch (keyEventRef.key)
    {
        case OIS::KC_LEFT:
        {
            player->setKeyLeft(true);
            if (player->checkPerformLeftMove())
                activatePerformLeftMove();
            break;
        }
        case OIS::KC_RIGHT:
        {
            player->setKeyRight(true);
            if (player->checkPerformRightMove())
                activatePerformRightMove();
            break;
        }
        case OIS::KC_UP:
        {
            player->setKeyUp(true);
            if (player->getSpeedControl() == SPEED_CONTROL_FLEXIBLE)
            {
                int s = player->getCamSpeed();
                if (s + 1 <= globals.maxCamSpeed)
                    s++;
                player->setCamSpeed(s);
            }
            break;
        }
        case OIS::KC_DOWN:
        {
            player->setKeyDown(true);
            if (player->getSpeedControl() == SPEED_CONTROL_FLEXIBLE)
            {
                int s = player->getCamSpeed();
                if (s - 1 >= globals.minCamSpeed)
                    s--;
                player->setCamSpeed(s);
            }
            break;
        }
        case OIS::KC_P:
        {
            pause = !pause;
            if (!pause) {
                globals.clearMessage();
                player->revertCam();
            } else {
                player->saveCam();
            }
            break;
        }
        case OIS::KC_1:
        {
            setLevel(1, player->getLevel().control);
            break;
        }
        case OIS::KC_2:
        {
            setLevel(2, player->getLevel().control);
            break;
        }
        case OIS::KC_3:
        {
            setLevel(3, player->getLevel().control);
            break;
        }
        case OIS::KC_4:
        {
            setLevel(4, player->getLevel().control);
            break;
        }
        case OIS::KC_5:
        {
            setLevel(5, player->getLevel().control);
            break;
        }
        case OIS::KC_6:
        {
            setLevel(6, player->getLevel().control);
            break;
        }
        case OIS::KC_7:
        {
            setLevel(7, player->getLevel().control);
            break;
        }
        case OIS::KC_8:
        {
            setLevel(8, player->getLevel().control);
            break;
        }
        case OIS::KC_9:
        {
            setLevel(9, player->getLevel().control);
            break;
        }
        case OIS::KC_0:
        {
            setLevel(10, player->getLevel().control);
            break;
        }
        case OIS::KC_Q:
        {
            setLevel(player->getLevel().nback, 1);
            break;
        }
        case OIS::KC_W:
        {
            setLevel(player->getLevel().nback, 2);
            break;
        }
        case OIS::KC_E:
        {
            setLevel(player->getLevel().nback, 3);
            break;
        }
        case OIS::KC_R:
        {
            setLevel(player->getLevel().nback, 4);
            break;
        }
        case OIS::KC_T:
        {
            player->setHP(globals.HPPositiveLimit);
            break;
        }
        case OIS::KC_Z:
        {
            sidebarMode++;
            if (sidebarMode > 3)
                sidebarMode = (SidebarLocation)0;
            setSidebar();
            break;
        }
        case OIS::KC_X:
        {
            player->changeMovementMode();
            break;
        }
        default:
            break;
    }
#endif
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::keyReleased(const OIS::KeyEvent &keyEventRef)
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
        default:
            break;
    }
#endif
    
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||


void DemoApp::preViewportUpdate(const Ogre::RenderTargetViewportEvent & evt)
{
    if (evt.source == OgreFramework::getSingletonPtr()->m_pViewportSide)
    {
        hud->hideOverlays();
    }
}

void DemoApp::postViewportUpdate(const Ogre::RenderTargetViewportEvent & evt)
{
    hud->showOverlays();
}
