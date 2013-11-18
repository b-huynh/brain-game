#include "OgreFramework.h"
#include "macUtils.h"
#include "Util.h"

#include "ExampleLoadingBar.h"

namespace Ogre
{
    template<> OgreFramework* Ogre::Singleton<OgreFramework>::msSingleton = 0;
};

using namespace Ogre;

extern Util::ConfigGlobal globals;

OgreFramework::OgreFramework()
{
	m_MoveSpeed			= 10.0f;
	m_RotateSpeed       = 0.3f;
    
	m_bShutDownOgre     = false;
	m_iNumScreenShots   = 0;
    
	m_pRoot				= 0;
	m_pRenderWnd        = 0;
	m_pSceneMgrMain		= 0;
	m_pSceneMgrSide 	= 0;
	m_pCameraMain		= 0;
	m_pViewportMain		= 0;
	m_pCameraSide		= 0;
	m_pViewportSide		= 0;
	m_pLog				= 0;
	m_pTimer			= 0;
    
	m_pInputMgr			= 0;
	m_pKeyboard			= 0;
	m_pMouse			= 0;
    
    m_pResourceGroupMgr = 0;
	m_pOverlayMgr       = 0;
    m_pFontMgr          = 0;
    m_pMaterialMgr      = 0;
    
    m_pSoundMgr         = 0;
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = macBundlePath() + "/Contents/Resources/";
#elif defined(OGRE_IS_IOS)
    m_ResourcePath = macBundlePath() + "/";
#else
    m_ResourcePath = "";
#endif
    m_pTrayMgr          = 0;
    m_FrameEvent        = Ogre::FrameEvent();
}

//|||||||||||||||||||||||||||||||||||||||||||||||
#if defined(OGRE_IS_IOS)
bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MultiTouchListener *pMouseListener, Ogre::RenderTargetListener *pRenderTargetListener)
#else
bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener, Ogre::RenderTargetListener *pRenderTargetListener)
#endif
{
    new Ogre::LogManager();
    
	m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
	m_pLog->setDebugOutputEnabled(true);
    
    String pluginsPath;
    // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
    pluginsPath = m_ResourcePath + "plugins.cfg";
#endif
    m_pRoot = new Ogre::Root(pluginsPath, Ogre::macBundlePath() + "/ogre.cfg");
    
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.load();
#endif
    
    if(m_pRoot->restoreConfig() || m_pRoot->showConfigDialog())
        m_pRenderWnd = m_pRoot->initialise(true, wndTitle);
    else
        return false;
    //    m_pRenderWnd->resize(globals.screenWidth, globals.screenHeight);
    //    m_pRenderWnd->setFullscreen(true, 1024, 800);
    globals.screenWidth = m_pRenderWnd->getWidth();
    globals.screenHeight = m_pRenderWnd->getHeight();
    globals.set();
    
    /*
     Ogre::NameValuePairList paramsWnd;
     paramsWnd["border"] = "fixed";
     m_pRenderWnd = m_pRoot->createRenderWindow(wndTitle, Util::SCREEN_WIDTH, Util::SCREEN_HEIGHT, false, &paramsWnd);
     */
	m_pSceneMgrMain = m_pRoot->createSceneManager(ST_GENERIC, "SceneManagerMain");
	//m_pSceneMgrSide = m_pRoot->createSceneManager(ST_GENERIC, "SceneManagerSide");
	m_pSceneMgrMain->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	//m_pSceneMgrSide->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
	m_pCameraMain = m_pSceneMgrMain->createCamera("CameraMain");
	m_pCameraMain->setPosition(Vector3(0, 0, 50));
	m_pCameraMain->lookAt(Vector3(0, 0, 0));
	m_pCameraMain->setNearClipDistance(0.1);
    //    m_pCameraMain->setFarClipDistance(500.0);
	m_pViewportMain = m_pRenderWnd->addViewport(m_pCameraMain, 1,
                                                0.0,
                                                0.0,
                                                double(globals.viewportMainWidth_modeNone) / globals.screenWidth,
                                                double(globals.viewportMainHeight_modeNone) / globals.screenHeight);
	m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
	m_pCameraMain->setAspectRatio(Real(m_pViewportMain->getActualWidth()) / Real(m_pViewportMain->getActualHeight()));
	m_pViewportMain->setCamera(m_pCameraMain);
    m_pViewportMain->getTarget()->addListener(pRenderTargetListener);
    /*
     m_pCameraSide = m_pSceneMgrSide->createCamera("CameraSide");
     m_pCameraSide->setPosition(Vector3(0, 0, 30));
     m_pCameraSide->lookAt(Vector3(0, 0, 0));
     m_pCameraSide->setNearClipDistance(1);
     m_pCameraSide->setOrthoWindow(10.0, 25.0);
     m_pCameraSide->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
     m_pViewportSide = m_pRenderWnd->addViewport(m_pCameraSide, 0,
     double(globals.viewportMainWidth_modeNone) / globals.screenWidth,
     0.0,
     double(globals.viewportSideWidth_modeNone) / globals.screenWidth,
     double(globals.viewportSideHeight_modeNone) / globals.screenHeight);
     m_pViewportSide->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
     m_pCameraSide->setAspectRatio(Real(m_pViewportSide->getActualWidth()) / Real(m_pViewportSide->getActualHeight()));
     m_pViewportSide->setCamera(m_pCameraSide);
     m_pViewportSide->getTarget()->addListener(pRenderTargetListener);
     */
	unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);
    
	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
    
	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);
    
#if !defined(OGRE_IS_IOS)
    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
    
	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();
    
#else
	m_pMouse = static_cast<OIS::MultiTouch*>(m_pInputMgr->createInputObject(OIS::OISMultiTouch, true));
#endif
    
#if !defined(OGRE_IS_IOS)
	if(pKeyListener == 0)
		m_pKeyboard->setEventCallback(this);
	else
		m_pKeyboard->setEventCallback(pKeyListener);
#endif
    
	if(pMouseListener == 0)
		m_pMouse->setEventCallback(this);
	else
		m_pMouse->setEventCallback(pMouseListener);
    
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
    
    cf.load(m_ResourcePath + "resources.cfg");
    
    m_pResourceGroupMgr = Ogre::ResourceGroupManager::getSingletonPtr();
    
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || defined(OGRE_IS_IOS)
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
                archName = Ogre::String(m_ResourcePath + archName);
#endif
            m_pResourceGroupMgr->addResourceLocation(archName, typeName, secName);
        }
    }
    
    m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();
    m_pFontMgr = Ogre::FontManager::getSingletonPtr();
    
    m_pMaterialMgr = Ogre::MaterialManager::getSingletonPtr();
    
    Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Bootstrap");
    
	m_pTrayMgr = new OgreBites::SdkTrayManager("TrayMgr", m_pRenderWnd, m_pMouse, this);
    //m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    m_pTrayMgr->hideCursor();
    m_pTrayMgr->setListener(this);
    m_pTrayMgr->setTrayPadding(10.0);
    
    Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
    //  m_quitButton = OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "sdkQuitButton", "QUIT", 250);
    
    m_pSoundMgr = OgreOggSound::OgreOggSoundManager::getSingletonPtr();
    m_pSoundMgr->init();
    m_pSoundMgr->createSound("Music1", "Dots5_converted.ogg", false, true, true);
    m_pSoundMgr->createSound("Music2", "Squares5_converted.ogg", false, true, true);
    m_pSoundMgr->createSound("Music3", "Fireworks2_converted.ogg", false, true, true);
    m_pSoundMgr->createSound("Music4", "Flourish2_converted.ogg", false, true, true);
    m_pSoundMgr->createSound("Sound1", "chimeup.wav", false, false, true);
    //m_pSoundMgr->createSound("Sound2", "chimedown.wav", false, false, true);
    m_pSoundMgr->createSound("Sound2", "wrongtriangle.wav", false, false, true);
    m_pSoundMgr->createSound("Sound3", "VinezorsSpaceBlip.wav", false, false, true);
    m_pSoundMgr->createSound("Sound4", "VinezorsSpaceBeep.wav", false, false, true);
    m_pSoundMgr->createSound("Sound5", "VinezorsSpaceChime.wav", false, false, true);
    m_pSoundMgr->createSound("Sound6", "VinezorsSpaceTone.wav", false, false, true);
    
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
	m_pTimer = OGRE_NEW Ogre::Timer();
	m_pTimer->reset();
    
	m_pRenderWnd->setActive(true);
    
	return true;
}

OgreFramework::~OgreFramework()
{
    if(m_pInputMgr) OIS::InputManager::destroyInputSystem(m_pInputMgr);
    if(m_pTrayMgr)  delete m_pTrayMgr;
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.unload();
#endif
    if(m_pRoot)     delete m_pRoot;
}

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	
	if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
        m_bShutDownOgre = true;
        return true;
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		m_pRenderWnd->writeContentsToTimestampedFile("BOF_Screenshot_", ".png");
		return true;
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_M))
	{
		static int mode = 0;
		
		if(mode == 2)
		{
			m_pCameraMain->setPolygonMode(PM_SOLID);
			mode = 0;
		}
		else if(mode == 0)
		{
            m_pCameraMain->setPolygonMode(PM_WIREFRAME);
            mode = 1;
		}
		else if(mode == 1)
		{
			m_pCameraMain->setPolygonMode(PM_POINTS);
			mode = 2;
		}
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_O))
	{
		if(m_pTrayMgr->isLogoVisible())
        {
            m_pTrayMgr->hideLogo();
            m_pTrayMgr->hideFrameStats();
        }
        else
        {
            m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
            m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
        }
	}
    
#endif
	return true;
}

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#if defined(OGRE_IS_IOS)
bool OgreFramework::touchMoved(const OIS::MultiTouchEvent &evt)
{
    OIS::MultiTouchState state = evt.state;
    int origTransX = 0, origTransY = 0;
#if !OGRE_NO_VIEWPORT_ORIENTATIONMODE
    switch(m_pCamera->getViewport()->getOrientationMode())
    {
        case Ogre::OR_LANDSCAPELEFT:
            origTransX = state.X.rel;
            origTransY = state.Y.rel;
            state.X.rel = -origTransY;
            state.Y.rel = origTransX;
            break;
            
        case Ogre::OR_LANDSCAPERIGHT:
            origTransX = state.X.rel;
            origTransY = state.Y.rel;
            state.X.rel = origTransY;
            state.Y.rel = origTransX;
            break;
            
            // Portrait doesn't need any change
        case Ogre::OR_PORTRAIT:
        default:
            break;
    }
#endif
	
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::touchPressed(const OIS:: MultiTouchEvent &evt)
{
    double axisY = evt.state.Y.abs;
    double axisX = evt.state.X.abs;
    
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::touchReleased(const OIS:: MultiTouchEvent &evt)
{
    return true;
}

bool OgreFramework::touchCancelled(const OIS:: MultiTouchEvent &evt)
{
#pragma unused(evt)
	return true;
}
#else
bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
	return true;
}

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}
#endif

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
	m_MoveScale = m_MoveSpeed   * (float)timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * (float)timeSinceLastFrame;
    
#if OGRE_VERSION >= 0x10800
    m_pSceneMgrMain->setSkyBoxEnabled(true);
#endif
    
	m_TranslateVector = Vector3::ZERO;
    
	getInput();
	moveCamera();
    
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
}

void OgreFramework::moveCamera()
{
#if !defined(OGRE_IS_IOS)
	if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_pCameraMain->moveRelative(m_TranslateVector);
	else
#endif
		m_pCameraMain->moveRelative(m_TranslateVector / 10);
}

void OgreFramework::getInput()
{
#if !defined(OGRE_IS_IOS)
    /*
     if(m_pKeyboard->isKeyDown(OIS::KC_A))
     m_TranslateVector.x = -m_MoveScale;
     
     if(m_pKeyboard->isKeyDown(OIS::KC_D))
     m_TranslateVector.x = m_MoveScale;
     
     if(m_pKeyboard->isKeyDown(OIS::KC_W))
     m_TranslateVector.z = -m_MoveScale;
     
     if(m_pKeyboard->isKeyDown(OIS::KC_S))
     m_TranslateVector.z = m_MoveScale;
     */
#endif
}

Ogre::String OgreFramework::getMacBundlePath() const
{
    return macBundlePath();
}

void OgreFramework::buttonHit(OgreBites::Button* button)
{
    if (button->getName() == "quitButton")
    {
        requestOgreShutdown();
    }
}
