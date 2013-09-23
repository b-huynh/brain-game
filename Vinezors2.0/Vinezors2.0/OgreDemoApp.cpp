#include "OgreDemoApp.h"

#include "Util.h"

DemoApp::DemoApp()
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::~DemoApp()
{
#ifdef USE_RTSHADER_SYSTEM
    mShaderGenerator->removeSceneManager(OgreFramework::getSingletonPtr()->m_pSceneMgr);
    
    finalizeRTShaderSystem();
#endif
    delete barHP;
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
	if(!OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, NULL))
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
    seed = 0;
    srand(seed);
    
    pause = false;
    
	OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 1500, true);
    
    Util::generateMaterials();
    
    Util::createSphere("podMesh", Util::POD_HEAD_RADIUS, 16, 16);
    Util::createSphere("vineMesh", Util::VINE_RADIUS, 32, 32);
    Util::createCylinder("stemMesh", Util::POD_STEM_RADIUS, Util::POD_STEM_LENGTH, 16);
    Util::createPlane("wallTileMesh", Util::TUNNEL_WALL_LENGTH, Util::TUNNEL_DEPTH);
    
	origin = Vector3(0, 0, 0);
    
	OgreFramework::getSingletonPtr()->m_pCamera->setPosition(Vector3(origin.x, origin.y, origin.z + Util::TUNNEL_DEPTH / 2));
	OgreFramework::getSingletonPtr()->m_pCamera->lookAt(origin);
    
	player = new Player(
        "",
        OgreFramework::getSingletonPtr()->m_pCamera->getPosition(),
        OgreFramework::getSingletonPtr()->m_pCamera->getOrientation(),
        Util::CAM_SPEED,
        Util::VINE_T_OFFSET,
        seed,
        "vinezors" + Util::toStringInt(seed) + ".csv");
	player->addVine(new Vine(player->getCamPos(), Util::VINE_RADIUS));
    player->setDesireRoll(90);
    player->setCamRoll(90);
    
    tunnel = new Tunnel(
        Vector3(0, 0, 0) + Util::TUNNEL_REFERENCE_FORWARD * (Util::TUNNEL_WIDTH / 2),
        Util::TUNNEL_WIDTH, Util::TUNNEL_DEPTH,
        Util::TUNNEL_SEGMENTS_PER_SECTION,
        Util::TUNNEL_SEGMENTS_PER_POD);
    tunnel->constructTunnel(Util::TUNNEL_SECTIONS, Util::NBACK);
    
    Light* light = OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light1");
    light->setDiffuseColour(1.0, 1.0, 1.0);
    light->setSpecularColour(1.0, 1.0, 1.0);
    light->setAttenuation(300, 1.0, 0.01, 0.0);
    lightNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("lightNode");
    lightNode->attachObject(light);
    
    /*
    // if barHP is a ManualObject ptr
    barHP = OgreFramework::getSingletonPtr()->m_pSceneMgr->createManualObject("BarHP");
    barHP->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY); // when using this, ensure Depth Check is Off in the material
//    Util::drawRect(barHP, -0.9, -0.9, 1.8, 0.1, ColourValue(1.0, 0.0, 0.0, 0.0), true);
    Util::drawRect(barHP, 10, 10, 300, 200, ColourValue(1.0, 0.0, 0.0, 0.0), true);

    OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->attachObject(barHP);
    // if barHP is a Rectangle2D
    barHP = new Rectangle2D(true);
    barHP->setCorners(Util::HP_BAR_XREF, Util::HP_BAR_YREF + Util::HP_BAR_HEIGHT, Util::HP_BAR_XREF + Util::HP_BAR_WIDTH, Util::HP_BAR_YREF);
    barHP->setMaterial("General/BaseRed");
    barHP->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
    barHP->setBoundingBox(AxisAlignedBox::BOX_INFINITE);
    OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->attachObject(barHP);
    */
    
    soundMusic = OgreFramework::getSingletonPtr()->m_pSoundMgr->createSound("Music1", "FrozenHillside.ogg", false, true, true);
    soundFeedbackGood = OgreFramework::getSingletonPtr()->m_pSoundMgr->createSound("Sound1", "chimeup.wav", false, false, true);

    // The code snippet below is used to output text
    // create a font resource
    ResourcePtr resourceText = OgreFramework::getSingletonPtr()->m_pFontMgr->create("Arial",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    resourceText->setParameter("type","truetype");
    resourceText->setParameter("source","C64_User_Mono_v1.0-STYLE.ttf");
    resourceText->setParameter("size","16");
    resourceText->setParameter("resolution","96");
    resourceText->load();
    
    // Create a panel
    OverlayContainer* panel = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "HudHelloWorld"));
    panel->setMetricsMode(GMM_PIXELS);
    panel->setPosition(10, 10);
    panel->setDimensions(10, 10);
    
    BorderPanelOverlayElement* healthArea = static_cast<BorderPanelOverlayElement*>(
            OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("BorderPanel", "HealthAreaBorder"));
    healthArea->setMetricsMode(GMM_RELATIVE);
    healthArea->setPosition(Util::HP_BAR_XREF - 0.01, Util::HP_BAR_YREF - 0.01);
    healthArea->setDimensions(Util::HP_BAR_WIDTH + 0.02, Util::HP_BAR_HEIGHT + 0.02);
    healthArea->setMaterialName("BaseWhite");
    
    barHP = static_cast<PanelOverlayElement*>(
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "HealthBar"));
    barHP->setMetricsMode(GMM_RELATIVE);
    barHP->setPosition(Util::HP_BAR_XREF, Util::HP_BAR_YREF);
    barHP->setDimensions(0.0, 0.0);
    barHP->setMaterialName("General/BaseRed");
    
    // Create text area
    label1= static_cast<TextAreaOverlayElement*>(
            OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel1"));
    label1->setMetricsMode(GMM_PIXELS);
    label1->setPosition(10, 525);
    label1->setCharHeight(26);
    label1->setFontName("Arial");
    label1->setColour(ColourValue::Black);
    label1->setCaption("Time: " + Util::toStringDouble(player->getTotalElapsed()));
    
    // Create text area
    label2= static_cast<TextAreaOverlayElement*>(
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel2"));
    label2->setMetricsMode(GMM_PIXELS);
    label2->setPosition(10, 475);
    label2->setCharHeight(26);
    label2->setColour(ColourValue::Black);
    label2->setFontName("Arial");
    label2->setCaption("Score: " + Util::toStringInt(player->getScore()));
    
    // Create text area
    label3= static_cast<TextAreaOverlayElement*>(
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel3"));
    label3->setMetricsMode(GMM_PIXELS);
    label3->setPosition(525, 525);
    label3->setCharHeight(26);
    label3->setColour(ColourValue::Black);
    label3->setFontName("Arial");
    label3->setCaption("Nback: " + Util::toStringInt(tunnel->getNBack()));
    
    // Create text area
    label4= static_cast<TextAreaOverlayElement*>(
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel4"));
    label4->setMetricsMode(GMM_PIXELS);
    label4->setPosition(525, 475);
    label4->setCharHeight(26);
    label4->setColour(ColourValue::Black);
    label4->setFontName("Arial");
    label4->setCaption("Speed: " + Util::toStringInt(player->getCamSpeed()));
    
    // Create an overlay, and add the panel
    Overlay* overlay = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayHelloWorld");
    overlay->add2D(panel);
    panel->addChild(label1);
    panel->addChild(label2);
    panel->addChild(label3);
    panel->addChild(label4);
    panel->addChild(healthArea);
    panel->addChild(barHP);
    overlay->show();
}

void DemoApp::update(double elapsed)
{
    OgreFramework::getSingletonPtr()->m_pSoundMgr->update();
    //elapsed /= 100;
    
    if (false && !soundMusic->isPlaying() && player->getTotalElapsed() > 1.5)
        soundMusic->play();
    
    // Determine whether a stage has completed
    if (!tunnel->isDone() &&
        // Notify completion of stage
        (player->getHP() >= Util::HP_POSITIVE_LIMIT ||
         player->getHP() <= Util::HP_NEGATIVE_LIMIT)) {
            tunnel->setDone(true);
            
            // Generate a CHECKPOINT tunnel section
            for (int i = 0; i < Util::INITIATION_SECTIONS; ++i) {
                SectionInfo info = SectionInfo(NORMAL_BLANK, NO_DIRECTION, 0);
                if (i == Util::INITIATION_SECTIONS - 1) {
                    info.tunnelType = CHECKPOINT;
                }
                tunnel->addSection(info);
            }
        }
    
    if (!pause) {
        int currentHp = player->getHP();
        player->update(elapsed, tunnel);
        player->checkCollisions(tunnel);
        
        //Play Feedback Sound
        if (currentHp < player->getHP()) {
            soundFeedbackGood->play();
            printf("good\n");
        }
        else if (currentHp > player->getHP()) {
            //negativeFeedback->getSound()->Play();
            printf("bad\n");
        }
        
        // Animate Pod Growing outwards or Growing inwards
        const double GROWTH_SPEED = 1;
        TunnelSlice* nextSliceM = tunnel->getNext(Util::POD_APPEARANCE);
        if (nextSliceM) {
            if (!tunnel->isDone())
                nextSliceM->updateGrowth(GROWTH_SPEED * elapsed);
            else
                nextSliceM->updateGrowth(-GROWTH_SPEED * elapsed);
        }
        nextSliceM = tunnel->getNext(Util::POD_APPEARANCE + 1);
        if (nextSliceM) {
            if (!tunnel->isDone())
                nextSliceM->updateGrowth(GROWTH_SPEED * elapsed);
            else
                nextSliceM->updateGrowth(-GROWTH_SPEED * elapsed);
        }
        
        // If player's current position is out of its current segment...
        if (tunnel->renewIfNecessary(player->getCamPos())) {
            
            // Set new camera slerp goal
            player->setOldPos(player->getCamPos());
            player->setOldRot(player->getCamRot());
            player->setOldRoll(player->getCamRoll());
            TunnelSlice* nextSlice1 = tunnel->getNext(1);
            if (nextSlice1)
            {
                player->setDesireRot(nextSlice1->getQuaternion());
            }
            else {
                // Generate a new tunnel because we are at the end
                Vector3 newOrigin = tunnel->getEnd();
                TunnelSlice* current = tunnel->getCurrent();
                Quaternion rot = current->getQuaternion();
                Vector3 forward = current->getForward();
                int nback = tunnel->getNBack();
                delete tunnel;
                
                int newNback = nback;
                if (player->getHP() > 0)
                    newNback++;
                else if (player->getHP() < 0)
                    newNback--;
                if (newNback < 0)
                    newNback = 0;
                
                tunnel = new Tunnel(
                    newOrigin + forward * (Util::TUNNEL_WIDTH / 2),
                    Util::TUNNEL_WIDTH,
                    Util::TUNNEL_DEPTH,
                    Util::TUNNEL_SEGMENTS_PER_SECTION,
                    Util::TUNNEL_SEGMENTS_PER_POD);
                tunnel->constructTunnel(Util::TUNNEL_SECTIONS, newNback, rot);
                player->newTunnel(tunnel);
            }
            
            // Show Pod Color and Play Sound
            TunnelSlice* nextSliceN = tunnel->getNext(Util::POD_APPEARANCE);
            if (nextSliceN && !tunnel->isDone())
            {
                for (int i = 0; i < nextSliceN->getPods().size(); ++i) {
                    nextSliceN->getPods()[i]->revealPod();
                    //playPodSound(nextSliceN->getPods()[i]);
                }
            }
        }
        
    } else {
         // Navigation Debug Keys
         if (player->getKeyUp())
             player->move(Vector3(player->getCamForward() * Util::CAM_SPEED * elapsed));
         if (player->getKeyDown())
             player->move(Vector3(player->getCamForward() * -Util::CAM_SPEED * elapsed));
         if (player->getKeyLeft())
             player->move(Vector3(player->getCamRight() * -Util::CAM_SPEED * elapsed));
         if (player->getKeyRight())
             player->move(Vector3(player->getCamRight() * Util::CAM_SPEED * elapsed));
    }
    
    lightNode->setPosition(OgreFramework::getSingletonPtr()->m_pCamera->getPosition());
    
    Quaternion camRot = player->getCombinedRotAndRoll();
    OgreFramework::getSingletonPtr()->m_pCamera->setPosition(player->getCamPos());
    OgreFramework::getSingletonPtr()->m_pCamera->setOrientation(camRot);
    
    //if (pause)
    //    label->setText("Enter Name: " + player->getName());
    //else
    //    label->setText("");
    label1->setCaption("Time: " + Util::toStringDouble(player->getTotalElapsed()));
    label2->setCaption("Score: " + Util::toStringInt(player->getScore()));
    label3->setCaption("N-Back: " + Util::toStringInt(tunnel->getNBack()));
    label4->setCaption("Speed: " + Util::toStringInt(player->getCamSpeed()));
    
    double barWidth = Util::HP_BAR_WIDTH;
    if (player->getHP() >= 0) {
        barWidth *= player->getHP() / (double)(Util::HP_POSITIVE_LIMIT);
        
        barHP->setDimensions(barWidth, Util::HP_BAR_HEIGHT);
  
        if (player->getHP() >= Util::HP_POSITIVE_LIMIT)
            barHP->setMaterialName("General/BaseBlue");
        else
            barHP->setMaterialName("General/BaseGreen");
    } else {
        barWidth *= player->getHP() / (double)(Util::HP_NEGATIVE_LIMIT);
        
        barHP->setDimensions(barWidth, Util::HP_BAR_HEIGHT);
        
        if (player->getHP() <= Util::HP_NEGATIVE_LIMIT)
            barHP->setMaterialName("General/BaseRed");
        else
            barHP->setMaterialName("General/BaseYellow");
    }
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

bool DemoApp::mouseMoved(const OIS::MouseEvent &evt)
{
    if (pause) {
        Vector2 dmove = Vector2(evt.state.X.rel, evt.state.Y.rel) - player->getMousePos();
        
        Vector3 right = player->getCamRight();
        Vector3 up = player->getCamUpward();
        Quaternion yawRot;
        Quaternion pitchRot;
        yawRot.FromAngleAxis(Degree(dmove.x), up);
        pitchRot.FromAngleAxis(Degree(dmove.y), right);
        
        Quaternion curRot = player->getCamRot();
        curRot = pitchRot * yawRot * curRot;
        player->setCamRot(curRot);
        curRot = player->getCombinedRotAndRoll();
        OgreFramework::getSingletonPtr()->m_pCamera->setOrientation(curRot);
        
        player->setMousePos(Vector2(evt.state.X.rel, evt.state.Y.rel));
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
            break;
        }
        case OIS::KC_RIGHT:
        {
            player->setKeyRight(true);
            break;
        }
        case OIS::KC_UP:
        {
            player->setKeyUp(true);
            int s = player->getCamSpeed();
            if (s + 1 <= Util::MAX_CAM_SPEED)
                s++;
            player->setCamSpeed(s);
            break;
        }
        case OIS::KC_DOWN:
        {
            player->setKeyDown(true);
            int s = player->getCamSpeed();
            if (s - 1 >= Util::MIN_CAM_SPEED)
                s--;
            player->setCamSpeed(s);
            break;
        }
        case OIS::KC_P:
        {
            pause = !pause;
            if (!pause) {
                player->setCamPos(player->getOldPos());
                player->setCamRot(player->getOldRot());
                player->setCamRoll(player->getOldRoll());
            } else {
                player->setOldPos(player->getCamPos());
                player->setOldRot(player->getCamRot());
                player->setOldRoll(player->getCamRoll());
            }
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
