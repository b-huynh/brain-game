#include "OgreDemoApp.h"

#include "Util.h"

DemoApp::DemoApp()
{
	m_pCubeNode		= 0;
	m_pCubeEntity   = 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::~DemoApp()
{
#ifdef USE_RTSHADER_SYSTEM
    mShaderGenerator->removeSceneManager(OgreFramework::getSingletonPtr()->m_pSceneMgr);
    
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

void DemoApp::startDemo()
{
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, 0))
		return;
    
	m_bShutdown = false;
    
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
	
#ifdef USE_RTSHADER_SYSTEM
    initializeRTShaderSystem(OgreFramework::getSingletonPtr()->m_pSceneMgr);
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
	//OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
    
    /*
	m_pCubeEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("Cube", "ogrehead.mesh");
	m_pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");
	m_pCubeNode->attachObject(m_pCubeEntity);
     */
    
    Util::generateMaterials();
    Util::createSphere("sphereMesh", 10, 64, 64);
    
    Entity* sphereEntity1 = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("mySphereEntity1", "sphereMesh");
    SceneNode* sphereNode1 = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("SphereNode1");
    sphereEntity1->setMaterialName("SunMaterial");
    sphereNode1->attachObject(sphereEntity1);
    
    Light* light = OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light");
    //light->setDiffuseColour(1.0, 1.0, 1.0);
    //light->setSpecularColour(1.0, 1.0, 1.0);
    light->setPosition(70, 70, 70);
    
    Entity* sphereEntity2 = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("mySphereEntity2", "sphereMesh");
    SceneNode* sphereNode2 = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("SphereNode2");
    sphereEntity2->setMaterialName("EarthMaterial");
    sphereNode2->attachObject(sphereEntity2);
    sphereNode2->setPosition(25, 25, 25);
    
    /*
    // create ManualObject
    ManualObject* manual = OgreFramework::getSingletonPtr()->m_pSceneMgr->createManualObject("manual");

    // specify the material (by name) and rendering type
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    // define start and end point
    manual->position(-100, -100, 100);
    manual->position(100, -100, 100);
    manual->position(100, -100, -100);
    manual->position(-100, -100, -100);
    manual->triangle(0, 1, 2);
    manual->triangle(0, 2, 3);
    
    // tell Ogre, your definition has finished
    manual->end();
    
    // add ManualObject to the RootSceneNode (so it will be visible)
    OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->attachObject(manual);
    */
    
    // create a font resource
    ResourcePtr resource = OgreFramework::getSingletonPtr()->m_pFontMgr->create("Arial",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    resource->setParameter("type","truetype");
    resource->setParameter("source","C64_User_Mono_v1.0-STYLE.ttf");
    resource->setParameter("size","16");
    resource->setParameter("resolution","96");
    resource->load();
    
    // Create a panel
    OverlayContainer* panel = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "PanelName"));
    panel->setMetricsMode(GMM_PIXELS);
    panel->setPosition(10, 10);
    panel->setDimensions(300, 120);
    
    // Create text area
    TextAreaOverlayElement* textArea = static_cast<TextAreaOverlayElement*>(
            OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaName"));
    textArea->setMetricsMode(GMM_PIXELS);
    textArea->setPosition(0, 0);
    textArea->setDimensions(300, 120);
    textArea->setCharHeight(26);
    // set the font name to the font resource that you just created.
    textArea->setFontName("Arial");
    // say something
    textArea->setCaption("Hello, World!");
    
    // Create an overlay, and add the panel
    Overlay* overlay = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayName");
    overlay->add2D(panel);
    panel->addChild(textArea);
    overlay->show();
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

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F))
	{
        //do something
	}
#endif
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
#endif

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||
