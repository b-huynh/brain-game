#include "OgreDemoApp.h"

#include "Util.h"
#include <fstream>

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
	if(!OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, this, this))
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

void DemoApp::loadConfig(std::string filepath)
{
    std::ifstream in (filepath.c_str());
    std::string check, paramName, colon;
    double paramVal;
    
    in >> check;
    if (check != "{") return;
    
    in >> paramName;
    while (paramName != "}") {
        in >> colon >> paramVal;
        std::cout << "Loading " << paramName << " = " << paramVal << std::endl;
        Util::setConfigValue(paramName, paramVal);
        in >> paramName;
    }
}

void DemoApp::setupDemoScene()
{
    // Grab config file
    
    loadConfig(Util::getSaveDir() + "vinezors.conf");
    
    seed = 0;
    srand(seed);
    
    pause = false;
    
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyBox(true, "Examples/SpaceSkyBox", 5000, true);
    
    Util::generateMaterials();
    
    Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "podMesh", Util::POD_HEAD_RADIUS, 16, 16);
    Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "podMesh", Util::POD_HEAD_RADIUS, 16, 16);
    Util::createSphere(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "vineMesh", Util::VINE_RADIUS, 32, 32);
    Util::createCylinder(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "stemMesh", Util::POD_STEM_RADIUS, Util::POD_STEM_LENGTH, 16);
    Util::createCylinder(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "stemMesh", Util::POD_STEM_RADIUS, Util::POD_STEM_LENGTH, 16);
    Util::createPlane(OgreFramework::getSingletonPtr()->m_pSceneMgrMain, "wallTileMesh", Util::TUNNEL_WALL_LENGTH, Util::TUNNEL_SEGMENT_DEPTH);
    Util::createPlane(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, "seatMesh", 1.0, 1.0);
    
	origin = Vector3(0, 0, 0);
    
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(origin.x, origin.y, origin.z + Util::TUNNEL_SEGMENT_DEPTH / 2));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(origin);
    
	player = new Player(
        "",
        PlayerLevel(),
        OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition(),
        OgreFramework::getSingletonPtr()->m_pCameraMain->getOrientation(),
        Util::INIT_CAM_SPEED,
        Util::VINE_T_OFFSET,
        seed,
        "vinezors" + Util::toStringInt(seed) + ".csv");
	player->addVine(new Vine(OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(), player->getCamPos(), Util::VINE_RADIUS));
    
    tunnel = new Tunnel(
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(),
        Vector3(0, 0, 0) + Util::TUNNEL_REFERENCE_FORWARD * (Util::TUNNEL_SEGMENT_WIDTH / 2),
        Util::TUNNEL_SEGMENT_WIDTH, Util::TUNNEL_SEGMENT_DEPTH,
        Util::TUNNEL_MIN_ANGLE_TURN, Util::TUNNEL_MAX_ANGLE_TURN,
        GAME_TIMED,
        player->getLevel().nback,
        player->getLevel().control,
        player->getVineDir(),
        Util::TUNNEL_SEGMENTS_PER_SECTION,
        Util::TUNNEL_SEGMENTS_PER_POD);
    tunnel->constructTunnel(Util::TUNNEL_SECTIONS);
    
    Light* lightMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->createLight("Light");
    lightMain->setDiffuseColour(1.0, 1.0, 1.0);
    lightMain->setSpecularColour(1.0, 1.0, 1.0);
//    lightMain->setAttenuation(10, 1.0, 0.0001, 0.0);
    lightNodeMain = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("lightNode");
    lightNodeMain->attachObject(lightMain);
    lightNodeMain->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
    
    Light* lightSide = OgreFramework::getSingletonPtr()->m_pSceneMgrSide->createLight("Light");
    lightSide->setDiffuseColour(1.0, 1.0, 1.0);
    lightSide->setSpecularColour(1.0, 1.0, 1.0);
    lightNodeSide = OgreFramework::getSingletonPtr()->m_pSceneMgrSide->getRootSceneNode()->createChildSceneNode("lightNode");
    lightNodeSide->attachObject(lightSide);
    lightNodeSide->setPosition(OgreFramework::getSingletonPtr()->m_pCameraSide->getPosition());
    
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

    // The code snippet below is used to output text
    // create a font resource
    ResourcePtr resourceText = OgreFramework::getSingletonPtr()->m_pFontMgr->create("Arial",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    resourceText->setParameter("type","truetype");
    resourceText->setParameter("source","C64_User_Mono_v1.0-STYLE.ttf");
    resourceText->setParameter("size","16");
    resourceText->setParameter("resolution","96");
    resourceText->load();
    
    // Create panels
    OverlayContainer* panel1 = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "HudInterface"));
    panel1->setMetricsMode(GMM_PIXELS);
    panel1->setPosition(10, 10);
    panel1->setDimensions(10, 10);
    
    OverlayContainer* panel2 = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SideInterface"));
    panel2->setMetricsMode(GMM_PIXELS);
    panel2->setPosition(10, 10);
    panel2->setDimensions(10, 10);
    
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
    label1->setPosition(Util::LABEL1_POSX, Util::LABEL1_POSY);
    label1->setCharHeight(16);
    label1->setFontName("Arial");
    label1->setColour(ColourValue::Black);
    label1->setCaption("Time: " + Util::toStringDouble(player->getTotalElapsed()));
    
    // Create text area
    label2= static_cast<TextAreaOverlayElement*>(
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel2"));
    label2->setMetricsMode(GMM_PIXELS);
    label2->setPosition(Util::LABEL2_POSX, Util::LABEL2_POSY);
    label2->setCharHeight(16);
    label2->setColour(ColourValue::Black);
    label2->setFontName("Arial");
    label2->setCaption("Score: " + Util::toStringInt(player->getScore()));
    
    // Create text area
    label3= static_cast<TextAreaOverlayElement*>(
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel3"));
    label3->setMetricsMode(GMM_PIXELS);
    label3->setPosition(Util::LABEL3_POSX, Util::LABEL3_POSY);
    label3->setCharHeight(16);
    label3->setColour(ColourValue::Black);
    label3->setFontName("Arial");
    label3->setCaption("Nback: " + Util::toStringInt(tunnel->getNBack()));
    
    // Create text area
    label4= static_cast<TextAreaOverlayElement*>(
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel4"));
    label4->setMetricsMode(GMM_PIXELS);
    label4->setPosition(Util::LABEL4_POSX, Util::LABEL4_POSY);
    label4->setCharHeight(16);
    label4->setColour(ColourValue::Black);
    label4->setFontName("Arial");
    label4->setCaption("Speed: " + Util::toStringInt(player->getCamSpeed()));
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayMain");
    Overlay* overlay2 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlaySide");
    overlay1->add2D(panel1);
    overlay2->add2D(panel2);
    panel1->addChild(healthArea);
    panel1->addChild(barHP);
    panel2->addChild(label1);
    panel2->addChild(label2);
    panel2->addChild(label3);
    panel2->addChild(label4);
    
    overlays.push_back(overlay1);
    overlays.push_back(overlay2);
    
    sidebarMode = SIDEBAR_RIGHT;
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
            m_pViewportMain->setDimensions(0.0,
                                           0.0,
                                           double(Util::VIEWPORT_MAIN_WIDTH_MODENONE) / Util::SCREEN_WIDTH,
                                           double(Util::VIEWPORT_MAIN_HEIGHT_MODENONE) / Util::SCREEN_HEIGHT);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
            m_pCameraSide->setOrthoWindow(0.0, 0.0);
            m_pViewportSide->setDimensions(
                                           double(Util::VIEWPORT_MAIN_WIDTH_MODENONE) / Util::SCREEN_WIDTH,
                                           0.0,
                                           double(Util::VIEWPORT_SIDE_WIDTH_MODENONE) / Util::SCREEN_WIDTH,
                                           double(Util::VIEWPORT_SIDE_HEIGHT_MODENONE) / Util::SCREEN_HEIGHT);
            m_pCameraSide->setAspectRatio(Real(0.0));
            break;
        case SIDEBAR_RIGHT:
            m_pViewportMain->setDimensions(0.0,
                                           0.0,
                                           double(Util::VIEWPORT_MAIN_WIDTH_MODERIGHT) / Util::SCREEN_WIDTH,
                                           double(Util::VIEWPORT_MAIN_HEIGHT_MODERIGHT) / Util::SCREEN_HEIGHT);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
            m_pCameraSide->setPosition(Vector3(0, 0, 30));
            m_pCameraSide->lookAt(Vector3(0, 0, 0));
            m_pCameraSide->setNearClipDistance(1);
            m_pCameraSide->setOrthoWindow(10.0, 25.0);
            m_pViewportSide->setDimensions(
                                           double(Util::VIEWPORT_MAIN_WIDTH_MODERIGHT) / Util::SCREEN_WIDTH,
                                           0.0,
                                           double(Util::VIEWPORT_SIDE_WIDTH_MODERIGHT) / Util::SCREEN_WIDTH,
                                           double(Util::VIEWPORT_SIDE_HEIGHT_MODERIGHT) / Util::SCREEN_HEIGHT);
            m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
            break;
        case SIDEBAR_BOTTOM_LTR:
            m_pViewportMain->setDimensions(0.0,
                                           0.0,
                                           double(Util::VIEWPORT_MAIN_WIDTH_MODEBOTTOM) / Util::SCREEN_WIDTH,
                                           double(Util::VIEWPORT_MAIN_HEIGHT_MODEBOTTOM) / Util::SCREEN_HEIGHT);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
            m_pCameraSide->setPosition(Vector3(0, 0, 30));
            m_pCameraSide->lookAt(Vector3(0, 0, 0));
            m_pCameraSide->setNearClipDistance(1);
            m_pCameraSide->roll(Degree(-90));
            m_pCameraSide->setOrthoWindow(5, 2.5);
            m_pViewportSide->setDimensions(
                                           0.0,
                                           double(Util::VIEWPORT_MAIN_HEIGHT_MODEBOTTOM) / Util::SCREEN_HEIGHT,
                                           double(Util::VIEWPORT_SIDE_WIDTH_MODEBOTTOM) / Util::SCREEN_WIDTH,
                                           double(Util::VIEWPORT_SIDE_HEIGHT_MODEBOTTOM) / Util::SCREEN_HEIGHT);
            m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
            break;
        case SIDEBAR_BOTTOM_RTL:
            m_pViewportMain->setDimensions(0.0,
                                           0.0,
                                           double(Util::VIEWPORT_MAIN_WIDTH_MODEBOTTOM) / Util::SCREEN_WIDTH,
                                           double(Util::VIEWPORT_MAIN_HEIGHT_MODEBOTTOM) / Util::SCREEN_HEIGHT);
            m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
            
            m_pCameraSide->setPosition(Vector3(0, 0, 30));
            m_pCameraSide->lookAt(Vector3(0, 0, 0));
            m_pCameraSide->setNearClipDistance(1);
            m_pCameraSide->roll(Degree(-270));
            m_pCameraSide->setOrthoWindow(5, 2.5);
            m_pViewportSide->setDimensions(
                                           0.0,
                                           double(Util::VIEWPORT_MAIN_HEIGHT_MODEBOTTOM) / Util::SCREEN_HEIGHT,
                                           double(Util::VIEWPORT_SIDE_WIDTH_MODEBOTTOM) / Util::SCREEN_WIDTH,
                                           double(Util::VIEWPORT_SIDE_HEIGHT_MODEBOTTOM) / Util::SCREEN_HEIGHT);
            m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
            break;
    }
}

void DemoApp::update(double elapsed)
{
    OgreFramework::getSingletonPtr()->m_pSoundMgr->update();
    
    // Determine whether a stage has completed
    if (!tunnel->isDone())
    {
        if (tunnel->getMode() == GAME_NORMAL)
        {   
        // Notify completion of stage
            if (player->getHP() >= Util::HP_POSITIVE_LIMIT ||
                player->getHP() <= Util::HP_NEGATIVE_LIMIT)
                tunnel->setDone(true);
        }
        else
        {
            if (tunnel->getTotalElapsed() > 120.0)
                tunnel->setDone(true);
        }
        if (tunnel->isDone())
        {
            // Generate a CHECKPOINT tunnel section
            for (int i = 0; i < Util::INITIATION_SECTIONS; ++i) {
                SectionInfo info = SectionInfo(NORMAL_BLANK, NO_DIRECTION, 0);
                if (i == Util::INITIATION_SECTIONS - 1) {
                    info.tunnelType = CHECKPOINT;
                }
                tunnel->addSection(info);
            }
        }
    }
    
    // Update the game state
    if (!pause) {
        player->update(elapsed, tunnel);
        
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
                setLevel(-1);
            }
            
            // Show Pod Color and Play Sound
            TunnelSlice* nextSliceN = tunnel->getNext(Util::POD_APPEARANCE);
            if (nextSliceN && !tunnel->isDone())
            {
                for (int i = 0; i < nextSliceN->getPods().size(); ++i) {
                    nextSliceN->getPods()[i]->revealPod();
//                    playPodSound(nextSliceN->getPods()[i]);
                }
            }
        }
        tunnel->update(elapsed);
        
    } else {
         // Navigation Debug Keys
         if (player->getKeyUp())
             player->move(Vector3(player->getCamForward() * Util::INIT_CAM_SPEED * elapsed));
         if (player->getKeyDown())
             player->move(Vector3(player->getCamForward() * -Util::INIT_CAM_SPEED * elapsed));
         if (player->getKeyLeft())
             player->move(Vector3(player->getCamRight() * -Util::INIT_CAM_SPEED * elapsed));
         if (player->getKeyRight())
             player->move(Vector3(player->getCamRight() * Util::INIT_CAM_SPEED * elapsed));
    }
    
    // Graphical view changes
    lightNodeMain->setPosition(OgreFramework::getSingletonPtr()->m_pCameraMain->getPosition());
    
    Quaternion camRot = player->getCombinedRotAndRoll();
    OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(player->getCamPos());
    OgreFramework::getSingletonPtr()->m_pCameraMain->setOrientation(camRot);
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyBoxNode()->setOrientation(player->getCombinedRotAndRoll());
    
    //if (pause)
    //    label->setText("Enter Name: " + player->getName());
    //else
    //    label->setText("");
    label1->setCaption("Time: " + Util::toStringDouble(player->getTotalElapsed()));
    label2->setCaption("Score: " + Util::toStringInt(player->getScore()));
    label3->setCaption("N-Back: " + Util::toStringInt(tunnel->getNBack()));
    label4->setCaption("Speed: " + Util::toStringInt(player->getCamSpeed()));
    
    double barWidth = Util::HP_BAR_WIDTH;
    if (tunnel->getMode() == GAME_NORMAL)
    {
        int hpRange = Util::HP_POSITIVE_LIMIT - Util::HP_NEGATIVE_LIMIT;
        barWidth *= (player->getHP() - Util::HP_NEGATIVE_LIMIT) / (double)(hpRange);
        barHP->setDimensions(barWidth, Util::HP_BAR_HEIGHT);
        if (player->getHP() >= 0) {
            barHP->setMaterialName("General/BaseGreen");
        } else {
            barHP->setMaterialName("General/BaseRed");
        }
    }
    else
    {
        double ratio = player->getNumCorrect() / static_cast<double>(player->getNumCorrect() + player->getNumWrong() + 10);
        if (ratio < 0.0)
            ratio = 0.0;
        barWidth *= ratio;
        barHP->setDimensions(barWidth, Util::HP_BAR_HEIGHT);
        barHP->setMaterialName("General/BaseGreen");
    }
}

void DemoApp::setLevel(int n)
{
    Vector3 newOrigin = tunnel->getEnd();
    TunnelSlice* current = tunnel->getCurrent();
    Quaternion rot = tunnel->getBack()->getQuaternion();
    Vector3 forward = tunnel->getBack()->getForward();
    int nback = tunnel->getNBack();
    delete tunnel;
    
    if (n >= 0)
    {
        tunnel = new Tunnel(
                            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(),
                            newOrigin + forward * (Util::TUNNEL_SEGMENT_WIDTH / 2),
                            Util::TUNNEL_SEGMENT_WIDTH,
                            Util::TUNNEL_SEGMENT_DEPTH,
                            Util::TUNNEL_MIN_ANGLE_TURN,
                            Util::TUNNEL_MAX_ANGLE_TURN,
                            GAME_NORMAL,
                            n,
                            player->getLevel().control,
                            player->getVineDir(),
                            Util::TUNNEL_SEGMENTS_PER_SECTION,
                            Util::TUNNEL_SEGMENTS_PER_POD);
        tunnel->constructTunnel(Util::TUNNEL_SECTIONS, rot);
    }
    else
    {
        bool pass = false;
        if (player->getHP() > 0)
            pass = true;
        player->evaluatePlayerLevel(pass);
        
        tunnel = new Tunnel(
                            OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(),
                            newOrigin + forward * (Util::TUNNEL_SEGMENT_WIDTH / 2),
                            Util::TUNNEL_SEGMENT_WIDTH,
                            Util::TUNNEL_SEGMENT_DEPTH,
                            Util::TUNNEL_MIN_ANGLE_TURN,
                            Util::TUNNEL_MAX_ANGLE_TURN,
                            GAME_NORMAL,
                            player->getLevel().nback,
                            player->getLevel().control,
                            player->getVineDir(),
                            Util::TUNNEL_SEGMENTS_PER_SECTION,
                            Util::TUNNEL_SEGMENTS_PER_POD);
        tunnel->constructTunnel(Util::TUNNEL_SECTIONS, rot);
    }
    
    player->setCamPos(newOrigin);
    player->setCamRot(rot);
    player->setDesireRot(rot);
    player->setCamRoll(player->getDesireRoll());
    
    player->newTunnel(tunnel);
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
        Vector2 dmove = Vector2(evt.state.X.rel, evt.state.Y.rel);
        
        Vector3 right = player->getCamRight(true);
        Vector3 up = player->getCamUpward(true);
        Quaternion yawRot;
        Quaternion pitchRot;
        yawRot.FromAngleAxis(Degree(-dmove.x) / 2, up);
        pitchRot.FromAngleAxis(Degree(-dmove.y) / 2, right);
        
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
            if (player->setVineDirRequest(Util::rightOf(player->getVineDir()), tunnel))
            {
                double val = player->getDesireRoll();
                player->setDesireRoll(val + 45);
            }
            break;
        }
        case OIS::KC_RIGHT:
        {
            player->setKeyRight(true);
            if (player->setVineDirRequest(Util::leftOf(player->getVineDir()), tunnel))
            {
                double val = player->getDesireRoll();
                player->setDesireRoll(val - 45);
            }
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
        case OIS::KC_1:
        {
            setLevel(1);
            break;
        }
        case OIS::KC_2:
        {
            setLevel(2);
            break;
        }
        case OIS::KC_3:
        {
            setLevel(3);
            break;
        }
        case OIS::KC_4:
        {
            setLevel(4);
            break;
        }
        case OIS::KC_5:
        {
            setLevel(5);
            break;
        }
        case OIS::KC_6:
        {
            setLevel(6);
            break;
        }
        case OIS::KC_7:
        {
            setLevel(7);
            break;
        }
        case OIS::KC_8:
        {
            setLevel(8);
            break;
        }
        case OIS::KC_9:
        {
            setLevel(9);
            break;
        }
        case OIS::KC_0:
        {
            setLevel(10);
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
    if (evt.source == OgreFramework::getSingletonPtr()->m_pViewportMain)
    {
        for(int i = 0; i < overlays.size(); ++i)
            if (overlays[i]->getName() != "OverlayMain")
                overlays[i]->hide();
    }
    else if (evt.source == OgreFramework::getSingletonPtr()->m_pViewportSide)
    {
        for(int i = 0; i < overlays.size(); ++i)
            if (overlays[i]->getName() != "OverlaySide")
                overlays[i]->hide();
    }
}

void DemoApp::postViewportUpdate(const Ogre::RenderTargetViewportEvent & evt)
{
    for(int i = 0; i < overlays.size(); ++i)
        overlays[i]->show();
}
