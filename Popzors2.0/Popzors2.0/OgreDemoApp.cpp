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
	if(!OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, this))
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
	//OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 2000, true);
    
    /*
	m_pCubeEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("Cube", "ogrehead.mesh");
	m_pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");
	m_pCubeNode->attachObject(m_pCubeEntity);
     */
    
    Util::generateMaterials();
    
    Util::createSphere("poppyMesh",1,100,100);
    Util::createUncappedCylinder("potMesh", 1, 1, 100);
    
    /*
    Util::createSphere("sphereMesh", Util::POD_HEAD_RADIUS, 16, 16);
    Util::createSphere("sphereMesh2", 10, 32, 32);
    */
     
    /*
    Entity* earthEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("earthEntity", "sphereMesh2");
    earthNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("SphereNode1");
    earthEntity->setMaterialName("General/PodBlue");
    earthNode->attachObject(earthEntity);
    earthNode->setPosition(50, 0, 10);
    earthNode->scale(4, 4, 4);
    
    Light* light1 = OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light1");
    light1->setDiffuseColour(1.0, 1.0, 1.0);
    light1->setSpecularColour(1.0, 1.0, 1.0);
//    light1->setAttenuation(3250, 0.51, 0.0000001, 0.0);
    ParticleSystem* sunParticle =
        OgreFramework::getSingletonPtr()->m_pSceneMgr->createParticleSystem("Sun", "Space/Sun");
    sunNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("ParticleNode1");
    sunNode->attachObject(sunParticle);
    sunNode->attachObject(light1);
    sunNode->scale(3.0, 3.0, 3.0);
    sunNode->setPosition(-125, 100, -250);
    
    Entity* moonEntity1 = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("moonEntity1", "sphereMesh2");
    moonNode = earthNode->createChildSceneNode("moonNode1");
    moonEntity1->setMaterialName("General/PodUnknown");
   // moonEntity1->getSubEntity(0)->getMaterial()->setAmbient(0.5, 0.5, 0.5);
    moonNode->attachObject(moonEntity1);
    
    Entity* moonEntityA = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("moonEntityA", "sphereMesh2");
    moonNodeA = moonNode->createChildSceneNode("moonNodeA");
    moonEntityA->setMaterialName("General/PodRed");
   // moonEntityA->getSubEntity(0)->getMaterial()->setAmbient(0.5, 0.0, 0.0);
    moonNodeA->attachObject(moonEntityA);
    moonNodeA->scale(0.5, 0.5, 0.5);
    
    Entity* moonEntityB = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("moonEntityB", "sphereMesh2");
    moonNodeB = moonNode->createChildSceneNode("moonNodeB");
    moonEntityB->setMaterialName("General/PodGreen");
   // moonEntityB->getSubEntity(0)->getMaterial()->setAmbient(0.0, 0.5, 0.0);
    moonNodeB->attachObject(moonEntityB);
    moonNodeB->scale(0.5, 0.5, 0.5);
    
    theta = Math::PI;
    thetaA = 0.0;
    thetaB = Math::PI;

    
    Quaternion rot;
    rot.FromAngleAxis(Radian(0), Vector3(0, 1, 0));
    pod = new Pod(Vector3(0, 0, 0), Vector3(0, Util::POD_STEM_LENGTH, 0), POD_YELLOW, Util::POD_STEM_RADIUS, Util::POD_HEAD_RADIUS);
    pod->revealPod();
     */
    
    Light* light1 = OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light1");
    light1->setDiffuseColour(1.0, 1.0, 1.0);
    light1->setSpecularColour(1.0, 1.0, 1.0);
    light1->setAttenuation(600, 1.0, 0.007, 0.0002);
    ParticleSystem* sunParticle = OgreFramework::getSingletonPtr()->m_pSceneMgr->createParticleSystem("Sun", "Space/Sun");
    sunNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("ParticleNode1");
    sunNode->attachObject(sunParticle);
    sunNode->attachObject(light1);
    sunNode->scale(3.0, 3.0, 3.0);
    sunNode->setPosition(0, 100, 0);
    
    selected = NULL;
    
    ground = new Ground();
    OgreFramework::getSingletonPtr()->m_pCamera->setPosition(0, 5, 5);
    OgreFramework::getSingletonPtr()->m_pCamera->lookAt(0,0,0);
    
    for (int i = 0; i < 5; ++i) {
        Poppy * poppy = new Poppy(Vector3(0,POPPY_RADIUS,0), Cpot1, Cpot2);
        poppy->setPosition(Vector3(randRangeDouble(-1,1),POPPY_RADIUS,randRangeDouble(-1,1)));
        poppies.push_back(poppy);
    }
    
    Pot* pot = new Pot(Vector3(-1,POT_HEIGHT,-1.5), 0.5, Cpot2, Cpot1);
    pots.push_back(pot);

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
    textArea->setDimensions(300, 120);
    textArea->setPosition(OgreFramework::getSingletonPtr()->m_pRenderWnd->getWidth() / 2 - 150,
                          OgreFramework::getSingletonPtr()->m_pRenderWnd->getHeight() / 2 - 60);
    textArea->setCharHeight(26);
    // set the font name to the font resource that you just created.
    textArea->setFontName("Arial");
    // say something
    textArea->setCaption("");
    
    // Create an overlay, and add the panel
    Overlay* overlay = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayName");
    overlay->add2D(panel);
    panel->addChild(textArea);
    overlay->show();
}

void DemoApp::update(double elapsed)
{
    //std::cout << "TIME ELAPSED ********" << std::endl;
    //std::cout << double(elapsed) << std::endl;
    theta += Math::PI / 256 * (elapsed);
    thetaA -= Math::PI / 64 * (elapsed);
    thetaB -= Math::PI / 64 * (elapsed);
    
    ground->update(elapsed);
    for (int i = 0; i < poppies.size(); ++i) {
        poppies[i]->update(elapsed);
    }
    
    /*
    moonNode->setPosition(cos(theta) * 100, 0, sin(theta) * 100);
    moonNodeA->setPosition(cos(thetaA) * 30, 0, sin(thetaA) * 30);
    moonNodeB->setPosition(cos(thetaB) * 30, 0, sin(thetaB) * 30);
     */
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

bool DemoApp::mouseMoved(const OIS::MouseEvent &evt)
{
#if !defined(OGRE_IS_IOS)
    OgreFramework::getSingletonPtr()->mouseMoved(evt);
    
    Ogre::Ray ray = OgreFramework::getSingletonPtr()->getCursorRay();
    Ogre::RaySceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createRayQuery(ray);
    
    query->setSortByDistance(true);
    Ogre::RaySceneQueryResult result = query->execute();
    
    if (selected && selected->getType() == Selectable::TYPE_POPPY)
        for (int i = 0; i < result.size(); ++i)
            if (ground->hasEntity( (Entity*)result[i].movable )) {
                Poppy* toMove = (Poppy*)selected;
                Vector3 hoverPos = ray * result[i].distance;
                hoverPos.y = POPPY_RADIUS;
                toMove->setPosition(hoverPos);
            }
#endif
    return true;
}

bool DemoApp::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
#if !defined(OGRE_IS_IOS)
    OgreFramework::getSingletonPtr()->mousePressed(evt, id);
        
    Ogre::Ray ray = OgreFramework::getSingletonPtr()->getCursorRay();
    Ogre::RaySceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createRayQuery(ray);
    
    query->setSortByDistance(true);
    Ogre::RaySceneQueryResult result = query->execute();
    
    //Handle Selection
    if (selected == NULL) {
        if (result.size() > 0 && result[0].movable != NULL) {
            for (int i = 0; i < poppies.size(); ++i)
                    if ( poppies[i]->hasEntity( (Ogre::Entity*)result[0].movable ) ) {
                        poppies[i]->setColor(getRandomPotColor());
                        selected = poppies[i];
                    }
            
            for (int i = 0; i < pots.size(); ++i)
                    if (pots[i]->hasEntity( (Ogre::Entity*)result[0].movable) )
                        pots[i]->setColor(getRandomPotColor());
        }
    }
    else {
        if (selected->getType() == Selectable::TYPE_POPPY && result.size() > 0) {
            Poppy* old = (Poppy*)selected;
            selected = NULL;
            Vector3 placeDest = ray * result[0].distance;
            placeDest.y = POPPY_RADIUS;
            old->setPosition(placeDest);
        }
    
    }

#endif
    return true;
}

bool DemoApp::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
#if !defined(OGRE_IS_IOS)
    OgreFramework::getSingletonPtr()->mouseReleased(evt, id);

#endif
    return true;
}
