#include "OgreApp.h"

#include "Player.h"
#include "Networking.h"
#include "EngineStage.h"
#include "EngineLevelSelection.h"
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
    delete engineStateMgr;
    //delete player;
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

#if !defined(OGRE_IS_IOS)
void OgreApp::startDemo(const char* name)
#else
void OgreApp::startDemo(void* uiWindow, void* uiView, unsigned int width, unsigned int height, const char* name)
#endif
{
    globals.playerName = name;
	new OgreFramework();
#if !defined(OGRE_IS_IOS)
    if (!OgreFramework::getSingletonPtr()->initOgre(this, this, this))
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

void OgreApp::update(float elapsed)
{
    OgreFramework::getSingletonPtr()->m_pSoundMgr->update(elapsed);
    player->getTutorialMgr()->update(elapsed, player);
    if (player->getTutorialMgr()->isVisible())
        return;
    engineStateMgr->update(elapsed);
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->update(elapsed);
    else endGame(); // Note need to sync logs at other times as not everyone will exit normally
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreApp::setupDemoScene()
{
    globals.initPaths();
#if defined(OGRE_IS_IOS) && defined(NETWORKING)
    //syncConfig();
#endif
    
    seed = time(0);
    srand(seed);
    std::cout << "SEED: " << seed << std::endl;
    
    // create font resources
    ResourcePtr resourceText1 = OgreFramework::getSingletonPtr()->m_pFontMgr->create("MainSmall", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    resourceText1->setParameter("type", "truetype");
    //resourceText1->setParameter("source", "C64_User_Mono_v1.0-STYLE.ttf");
    resourceText1->setParameter("source", "NEUROPOL.ttf");
    resourceText1->setParameter("size", "16");
    resourceText1->setParameter("resolution", "96");
    resourceText1->load();
    
    ResourcePtr resourceText2 = OgreFramework::getSingletonPtr()->m_pFontMgr->create("MainBig", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    resourceText2->setParameter("type", "truetype");
    resourceText2->setParameter("source", "Philosopher-Bold.ttf");
    resourceText2->setParameter("size", "16");
    resourceText2->setParameter("resolution", "256");
    resourceText2->load();
    
    Util::createDefaultSegments(OgreFramework::getSingletonPtr()->m_pSceneMgrMain);
    
    globals.initPaths();
    //if (!configStageType(globals.configPath, globals.configBackup, "globalConfig"))
    //    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
    
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3::ZERO);
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3::ZERO);
	player = new Player(
                        globals.playerName,
                        OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition(),
                        OgreFramework::getSingletonPtr()->m_pCameraMain->getOrientation(),
                        globals.initCamSpeed,
                        globals.vineTOffset,
                        seed,
                        "vinezors" + Util::toStringInt(seed) + ".csv");
    player->setSounds(true);
    if (!player->loadProgress(globals.savePath))
        std::cout << "WARNING: Save File could not be loaded correctly" << std::endl;
    player->feedLevelRequestFromSchedule();
    
    globals.initLogs(player->getSessionID());
    
    engineStateMgr = new EngineStateManager();
    engineStateMgr->requestPushEngine(ENGINE_MAIN_MENU, player);
    
#if defined(OGRE_IS_IOS) && defined(NETWORKING)
    syncLogs();
#endif
}

void OgreApp::endGame()
{
    // End the game on a good note and only once
    player->saveStage(globals.logPath);
    player->saveProgress(globals.savePath);
#if defined(OGRE_IS_IOS) && defined(NETWORKING)
    syncLogs();
#endif
    OgreFramework::getSingletonPtr()->requestOgreShutdown();
}

void OgreApp::requestResize()
{
    player->getTutorialMgr()->adjust();
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->requestResize();
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
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformLeftMove();
}

void OgreApp::activatePerformRightMove()
{
    player->addAction(ACTION_SWIPE_RIGHT);
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformRightMove();
}

void OgreApp::activatePerformSwipeUp()
{
    // NOTE: NO ACTION RECORDED FOR UP AND DOWN SWIPES
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformSwipeUp();
}

void OgreApp::activatePerformSwipeDown()
{
    // NOTE: NO ACTION RECORDED FOR UP AND DOWN SWIPES
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformSwipeDown();
}

void OgreApp::activatePerformDoubleTap(float x, float y)
{
    player->addAction(ACTION_DOUBLE_TAP);
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformDoubleTap(x, y);
}

void OgreApp::activatePerformSingleTap(float x, float y)
{
    player->addAction(ACTION_SINGLE_TAP);
    if (player->getTutorialMgr()->isVisible())
    {
        if (player->getTutorialMgr()->processInput(Vector2(x, y)))
            player->reactGUI();
        if (player->getTutorialMgr()->isHidden())
        {
            Engine* activeEngine = engineStateMgr->getActiveEngine();
            if (activeEngine)
                activeEngine->activateReturnFromPopup();
        }
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine)
        activeEngine->activatePerformSingleTap(x, y);
}

void OgreApp::activatePerformPinch()
{
    player->addAction(ACTION_PINCH);
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformPinch();
}

void OgreApp::activatePerformBeginLongPress()
{
    //player->addAction(ACTION_TAP_HOLD);
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformBeginLongPress();
}

void OgreApp::activatePerformEndLongPress()
{
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformEndLongPress();
}

void OgreApp::activatePerformBeginShortPress()
{
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformBeginShortPress();
    if (activeEngine) activeEngine->activateVelocity(0.0);
}

void OgreApp::activatePerformEndShortPress()
{
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activatePerformEndShortPress();
}

void OgreApp::activateMoved(float x, float y, float dx, float dy)
{
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activateMoved(x, y, dx, dy);
}

void OgreApp::activatePressed(float x, float y)
{
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine)
        activeEngine->activatePressed(x, y);
}

void OgreApp::activateReleased(float x, float y, float dx, float dy)
{
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activateReleased(x, y, dx, dy);
}

void OgreApp::activateVelocity(float vel)
{
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activateVelocity(vel);
}

void OgreApp::activateAngleTurn(float angle, float vel)
{
    if (player->getTutorialMgr()->isVisible())
    {
        return;
    }
    
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->activateAngleTurn(angle, vel);
}

void OgreApp::saveState()
{
    player->saveProgress(globals.savePath);
}

void OgreApp::loadState()
{
    // Player loads stuff separately
}

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
// Note to mimic IOS mechanisms, Mouse Handler events call two functions
bool OgreApp::mouseMoved(const OIS::MouseEvent &evt)
{
    OgreFramework::getSingletonPtr()->mouseMoved(evt);
    if (player->getTutorialMgr()->isVisible())
    {
        return true;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->mouseMoved(evt);
    if (activeEngine) activeEngine->activateMoved(evt.state.X.abs, evt.state.Y.abs, evt.state.X.rel, evt.state.Y.rel);
    return true;
}

bool OgreApp::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    OgreFramework::getSingletonPtr()->mousePressed(evt, id);
    if (player->getTutorialMgr()->isVisible())
    {
        if (player->getTutorialMgr()->processInput(Vector2(evt.state.X.abs, evt.state.Y.abs)))
            player->reactGUI();
        if (player->getTutorialMgr()->isHidden())
        {
            Engine* activeEngine = engineStateMgr->getActiveEngine();
            if (activeEngine)
                activeEngine->activateReturnFromPopup();
        }
        return true;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->mousePressed(evt, id);
    if (activeEngine) activeEngine->activatePressed(evt.state.X.abs, evt.state.Y.abs);
    return true;
}

bool OgreApp::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    OgreFramework::getSingletonPtr()->mouseReleased(evt, id);
    if (player->getTutorialMgr()->isVisible())
    {
        return true;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->mouseReleased(evt, id);
    if (activeEngine) activeEngine->activateReleased(evt.state.X.abs, evt.state.Y.abs, evt.state.X.rel, evt.state.Y.rel);
    return true;
}

bool OgreApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    if (player->getTutorialMgr()->isVisible())
    {
        return true;
    }
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->keyPressed(keyEventRef);
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    if (player->getTutorialMgr()->isVisible())
    {
        return true;
    }
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    Engine* activeEngine = engineStateMgr->getActiveEngine();
    if (activeEngine) activeEngine->keyReleased(keyEventRef);
	return true;
}
#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
