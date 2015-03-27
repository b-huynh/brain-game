//
//  HudMainSettings.cpp
//  VinezorsiOS
//
//  Created by Calvin Phung on 6/26/14.
//
//

#include "HudMainSettings.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

HudMainSettings::HudMainSettings(Player* player)
    :Hud()
{
    link(player);
    init();
}

HudMainSettings::~HudMainSettings()
{
    dealloc();
    unlink();
}

void HudMainSettings::init()
{
    alloc();
    initOverlay();
    showOverlays();
}

void HudMainSettings::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudMainSettings::update(float elapsed)
{
    musicVolumeSliderDisplay->setCaption(Util::toStringInt(player->musicVolume * 100));
    soundVolumeSliderDisplay->setCaption(Util::toStringInt(player->soundVolume * 100));
    holdoutSliderDisplay->setCaption(Util::toStringInt(player->holdout * 100));
    holdoutLBSliderDisplay->setCaption(Util::toStringInt(player->holdoutLB * 100));
    holdoutUBSliderDisplay->setCaption(Util::toStringInt(player->holdoutUB * 100));
    
    if (player->getTutorialMgr()->isEnabled())
    {
        enableTutorialsButtonBackground->setMaterialName("General/CheckboxGreen");
    }
    else
    {
        enableTutorialsButtonBackground->setMaterialName("General/CheckboxBlank");
    }
    
    if (globals.syncDataToServer)
    {
        syncDataButtonBackground->setMaterialName("General/CheckboxGreen");
        syncDataInternetNotification->setCaption("A Wi-Fi connection must be enabled for this option");
    }
    else
    {
        syncDataButtonBackground->setMaterialName("General/CheckboxBlank");
        syncDataInternetNotification->setCaption("");
    }
    
    if (player->soundVolume <= 0.0)
        soundVolumeSliderWarning->setCaption("Certain features require sound");
    else
        soundVolumeSliderWarning->setCaption("");
}

void HudMainSettings::alloc()
{
    //std::cout << "alloc" << std::endl;
    // Allocate Resources
    mainSettingsBackdrop1 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsBackdrop1"));
    mainSettingsBackdrop2 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsBackdrop2"));
    mainSettingsTitleBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsTitleBackground"));
    mainSettingsTitleText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsTitleText"));
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsBackButtonBackground"));
    controlSettingsButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsButtonBackground"));
    studySettingsButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel",
        "StudySettingsButtonBackground"));
    studySettingTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsTextDisplay"));
    
    
    
    musicVolumeSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsMusicVolumeSliderRangeBackground"));
    musicVolumeSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsMusicVolumeSliderBallBackground"));
    musicVolumeSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsMusicVolumeText"));
    musicVolumeSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsMusicVolumeDisplay"));
    
    //Holdout
    holdoutSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsholdoutSliderRangeBackground"));
    holdoutSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsholdoutSliderBallBackground"));
    holdoutSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsholdoutText"));
    holdoutSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsholdoutDisplay"));

    holdoutLBSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsholdoutLBSliderRangeBackground"));
    holdoutLBSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsholdoutLBSliderBallBackground"));
    holdoutLBSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsholdoutLBText"));
    holdoutLBSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsholdoutLBDisplay"));
    
    holdoutUBSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsholdoutUBSliderRangeBackground"));
    holdoutUBSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsholdoutUBSliderBallBackground"));
    holdoutUBSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsholdoutUBText"));
    holdoutUBSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsholdoutUBDisplay"));
    
    soundVolumeSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsSoundVolumeSliderRangeBackground"));
    soundVolumeSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsSoundVolumeSliderBallBackground"));
    soundVolumeSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsSoundVolumeText"));
    soundVolumeSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsSoundVolumeDisplay"));
    soundVolumeSliderWarning = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsSoundVolumeWarning"));
    
    enableTutorialsEntireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsEnableTutorialsEntireBackground"));
    enableTutorialsTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsEnableTutorialsTextDisplay"));;
    enableTutorialsButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsEnableTutorialsButtonBackground"));
    
    syncDataEntireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsSyncDataEntireBackground"));
    syncDataTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsSyncDataTextDisplay"));;
    syncDataButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsSyncDataButtonBackground"));
    syncDataInternetNotification = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsSyncDataInternetNotification"));;
    
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("MainSettingsOverlay");
    overlay1->add2D(mainSettingsBackdrop1);
    overlay1->add2D(mainSettingsBackdrop2);
    overlay1->add2D(mainSettingsTitleBackground);
    mainSettingsTitleBackground->addChild(mainSettingsTitleText);
    
    overlay1->add2D(musicVolumeSliderRangeBackground);
    musicVolumeSliderRangeBackground->addChild(musicVolumeSliderBallBackground);
    musicVolumeSliderRangeBackground->addChild(musicVolumeSliderText);
    musicVolumeSliderRangeBackground->addChild(musicVolumeSliderDisplay);
 
    overlay1->add2D(soundVolumeSliderRangeBackground);
    soundVolumeSliderRangeBackground->addChild(soundVolumeSliderBallBackground);
    soundVolumeSliderRangeBackground->addChild(soundVolumeSliderText);
    soundVolumeSliderRangeBackground->addChild(soundVolumeSliderDisplay);
    soundVolumeSliderRangeBackground->addChild(soundVolumeSliderWarning);
    
#ifdef DEBUG_MODE
    overlay1->add2D(holdoutSliderRangeBackground);
    holdoutSliderRangeBackground->addChild(holdoutSliderBallBackground);
    holdoutSliderRangeBackground->addChild(holdoutSliderText);
    holdoutSliderRangeBackground->addChild(holdoutSliderDisplay);
    
    overlay1->add2D(holdoutLBSliderRangeBackground);
    holdoutLBSliderRangeBackground->addChild(holdoutLBSliderBallBackground);
    holdoutLBSliderRangeBackground->addChild(holdoutLBSliderText);
    holdoutLBSliderRangeBackground->addChild(holdoutLBSliderDisplay);
    
    overlay1->add2D(holdoutUBSliderRangeBackground);
    holdoutUBSliderRangeBackground->addChild(holdoutUBSliderBallBackground);
    holdoutUBSliderRangeBackground->addChild(holdoutUBSliderText);
    holdoutUBSliderRangeBackground->addChild(holdoutUBSliderDisplay);
#endif
    
    overlay1->add2D(enableTutorialsEntireBackground);
    enableTutorialsEntireBackground->addChild(enableTutorialsTextDisplay);
    overlay1->add2D(enableTutorialsButtonBackground);
    
    overlay1->add2D(syncDataEntireBackground);
    syncDataEntireBackground->addChild(syncDataTextDisplay);
    overlay1->add2D(syncDataButtonBackground);
    syncDataEntireBackground->addChild(syncDataInternetNotification);
    
    overlay1->add2D(backButtonBackground);
    overlay1->add2D(controlSettingsButtonBackground);
    overlay1->add2D(studySettingsButtonBackground);
    studySettingsButtonBackground->addChild(studySettingTextDisplay);
  
    
    overlays.push_back(overlay1);
    
    musicVolumeSlider = new HudSlider();
    soundVolumeSlider = new HudSlider();
    holdoutSlider = new HudSlider();
    holdoutLBSlider = new HudSlider();
    holdoutUBSlider = new HudSlider();
    
    // Horizontal slider
#ifdef DEBUG_MODE
    // Reposition in debug mode because there are more sliders
    musicVolumeSlider->setSlider("musicvolume", overlays[0], Vector2(0.10, 0.25), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false,
                                 0, 100, 101, musicVolumeSliderRangeBackground, musicVolumeSliderBallBackground);
    soundVolumeSlider->setSlider("soundvolume", overlays[0], Vector2(0.10, 0.15), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false,
                                 0, 100, 101, soundVolumeSliderRangeBackground, soundVolumeSliderBallBackground);
#else
    musicVolumeSlider->setSlider("musicvolume", overlays[0], Vector2(0.10, 0.45), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false,
                                 0, 100, 101, musicVolumeSliderRangeBackground, musicVolumeSliderBallBackground);
    soundVolumeSlider->setSlider("soundvolume", overlays[0], Vector2(0.10, 0.25), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false,
                                 0, 100, 101, soundVolumeSliderRangeBackground, soundVolumeSliderBallBackground);
#endif
    holdoutSlider->setSlider("holdout", overlays[0], Vector2(0.10, 0.35), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false, 0, 100, 101, holdoutSliderRangeBackground, holdoutSliderBallBackground);
    holdoutLBSlider->setSlider("holdoutLB", overlays[0], Vector2(0.10, 0.45), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false, 0, 100, 101, holdoutLBSliderRangeBackground, holdoutLBSliderBallBackground);
    holdoutUBSlider->setSlider("holdoutUB", overlays[0], Vector2(0.10, 0.55), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false, 0, 100, 101, holdoutUBSliderRangeBackground, holdoutUBSliderBallBackground);

    positionSliderBalls();
    
   }

void HudMainSettings::dealloc()
{
    //std::cout << "dealloc" << std::endl;

    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsBackdrop1);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsBackdrop2);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsTitleBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsTitleText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(controlSettingsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(studySettingsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(studySettingTextDisplay);
   

    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(musicVolumeSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(musicVolumeSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(musicVolumeSliderText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(musicVolumeSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderText);
    //OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutLBSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutLBSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutLBSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutLBSliderText);
    //OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutLBSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutUBSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutUBSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutUBSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutUBSliderText);
    //OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutUBSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(soundVolumeSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(soundVolumeSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(soundVolumeSliderText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(soundVolumeSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(soundVolumeSliderWarning);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableTutorialsEntireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableTutorialsTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableTutorialsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(syncDataEntireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(syncDataTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(syncDataButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(syncDataInternetNotification);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
    
    if (musicVolumeSlider) delete musicVolumeSlider; musicVolumeSlider = NULL;
    if (soundVolumeSlider) delete soundVolumeSlider; soundVolumeSlider = NULL;
    if (holdoutSlider) delete holdoutSlider; holdoutSlider = NULL;
    if (holdoutLBSlider) delete holdoutLBSlider; holdoutLBSlider = NULL;
    if (holdoutUBSlider) delete holdoutUBSlider; holdoutUBSlider = NULL;
}

void HudMainSettings::initOverlay()
{
    //std::cout << "initOverlay" << std::endl;

    // Link and set resources
    mainSettingsBackdrop1->setMetricsMode(GMM_RELATIVE);
    mainSettingsBackdrop1->setPosition(0.00, 0.025);
    mainSettingsBackdrop1->setDimensions(1.00, 0.95);
    mainSettingsBackdrop1->setMaterialName("General/TutorialBackdrop");
    
    
    mainSettingsTitleBackground->setMetricsMode(GMM_RELATIVE);
    mainSettingsTitleBackground->setPosition(0.30, 0.025);
    mainSettingsTitleBackground->setDimensions(0.40, 0.20);
    
    mainSettingsTitleText->setMetricsMode(GMM_RELATIVE);
    mainSettingsTitleText->setAlignment(TextAreaOverlayElement::Center);
    mainSettingsTitleText->setPosition(0.20, 0.00);
    mainSettingsTitleText->setCharHeight(0.046 * FONT_SZ_MULT);
    mainSettingsTitleText->setFontName("MainSmall");
    mainSettingsTitleText->setCaption("Main Settings");
    
    musicVolumeSliderDisplay->setMetricsMode(GMM_RELATIVE);
    musicVolumeSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    musicVolumeSliderDisplay->setPosition(0.65, 0.00);
    musicVolumeSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    musicVolumeSliderDisplay->setFontName("MainSmall");
    
    musicVolumeSliderText->setMetricsMode(GMM_RELATIVE);
    musicVolumeSliderText->setAlignment(TextAreaOverlayElement::Left);
    musicVolumeSliderText->setPosition(0.00, -0.03);
    musicVolumeSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    musicVolumeSliderText->setFontName("MainSmall");
    musicVolumeSliderText->setCaption("Music Volume");
    
    holdoutSliderDisplay->setMetricsMode(GMM_RELATIVE);
    holdoutSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    holdoutSliderDisplay->setPosition(0.65, 0.00);
    holdoutSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    holdoutSliderDisplay->setFontName("MainSmall");
    
    holdoutSliderText->setMetricsMode(GMM_RELATIVE);
    holdoutSliderText->setAlignment(TextAreaOverlayElement::Left);
    holdoutSliderText->setPosition(0.00, -0.03);
    holdoutSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    holdoutSliderText->setFontName("MainSmall");
    holdoutSliderText->setCaption("Holdout Frequency");
    
    holdoutLBSliderDisplay->setMetricsMode(GMM_RELATIVE);
    holdoutLBSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    holdoutLBSliderDisplay->setPosition(0.70, 0.01);
    holdoutLBSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    holdoutLBSliderDisplay->setFontName("MainSmall");
    
    holdoutLBSliderText->setMetricsMode(GMM_RELATIVE);
    holdoutLBSliderText->setAlignment(TextAreaOverlayElement::Left);
    holdoutLBSliderText->setPosition(0.00, -0.03);
    holdoutLBSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    holdoutLBSliderText->setFontName("MainSmall");
    holdoutLBSliderText->setCaption("Holdout Lower Bound");
    
    holdoutUBSliderDisplay->setMetricsMode(GMM_RELATIVE);
    holdoutUBSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    holdoutUBSliderDisplay->setPosition(0.70, 0.01);
    holdoutUBSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    holdoutUBSliderDisplay->setFontName("MainSmall");
    
    holdoutUBSliderText->setMetricsMode(GMM_RELATIVE);
    holdoutUBSliderText->setAlignment(TextAreaOverlayElement::Left);
    holdoutUBSliderText->setPosition(0.00, -0.03);
    holdoutUBSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    holdoutUBSliderText->setFontName("MainSmall");
    holdoutUBSliderText->setCaption("Holdout Upper Bound");
    
    soundVolumeSliderDisplay->setMetricsMode(GMM_RELATIVE);
    soundVolumeSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    soundVolumeSliderDisplay->setPosition(0.65, 0.00);
    soundVolumeSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    soundVolumeSliderDisplay->setFontName("MainSmall");
    
    soundVolumeSliderText->setMetricsMode(GMM_RELATIVE);
    soundVolumeSliderText->setAlignment(TextAreaOverlayElement::Left);
    soundVolumeSliderText->setPosition(0.00, -0.03);
    soundVolumeSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    soundVolumeSliderText->setFontName("MainSmall");
    soundVolumeSliderText->setCaption("Sound Volume");
    
    soundVolumeSliderWarning->setMetricsMode(GMM_RELATIVE);
    soundVolumeSliderWarning->setAlignment(TextAreaOverlayElement::Left);
    soundVolumeSliderWarning->setPosition(0.26, -0.03);
    soundVolumeSliderWarning->setCharHeight(0.018 * FONT_SZ_MULT);
    soundVolumeSliderWarning->setFontName("MainSmall");
    soundVolumeSliderWarning->setColour(Ogre::ColourValue(1.0, 1.0, 0.0));
    
    enableTutorialsEntireBackground->setMetricsMode(GMM_RELATIVE);
    enableTutorialsEntireBackground->setPosition(0.20, 0.60);
    enableTutorialsEntireBackground->setDimensions(0.60, 0.10);
    
    enableTutorialsTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableTutorialsTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableTutorialsTextDisplay->setPosition(0.0, 0.05);
    enableTutorialsTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    enableTutorialsTextDisplay->setFontName("MainSmall");
    enableTutorialsTextDisplay->setCaption("Enable Tutorials");
    
    syncDataEntireBackground->setMetricsMode(GMM_RELATIVE);
    syncDataEntireBackground->setPosition(0.20, 0.70);
    syncDataEntireBackground->setDimensions(0.60, 0.10);
    
    syncDataTextDisplay->setMetricsMode(GMM_RELATIVE);
    syncDataTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    syncDataTextDisplay->setPosition(0.0, 0.05);
    syncDataTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    syncDataTextDisplay->setFontName("MainSmall");
    syncDataTextDisplay->setCaption("Share Gameplay Data to Server");
    
    syncDataInternetNotification->setMetricsMode(GMM_RELATIVE);
    syncDataInternetNotification->setAlignment(TextAreaOverlayElement::Left);
    syncDataInternetNotification->setPosition(0.0, 0.09);
    syncDataInternetNotification->setCharHeight(0.018 * FONT_SZ_MULT);
    syncDataInternetNotification->setFontName("MainSmall");
    syncDataInternetNotification->setColour(Ogre::ColourValue(1.0, 1.0, 0.0));
    
    studySettingTextDisplay->setMetricsMode(GMM_RELATIVE);
    studySettingTextDisplay->setAlignment(TextAreaOverlayElement::Center);
    studySettingTextDisplay->setPosition(0.15, 0.015);
    studySettingTextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    studySettingTextDisplay->setFontName("MainSmall");
    studySettingTextDisplay->setCaption("Study Settings");
    
    
    musicVolumeSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    musicVolumeSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    soundVolumeSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    soundVolumeSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    holdoutSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    holdoutSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    holdoutLBSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    holdoutLBSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    holdoutUBSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    holdoutUBSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    
    backButtonBackground->setMaterialName("General/BackButton1");
    controlSettingsButtonBackground->setMaterialName("General/ControlSettingsButton");
    studySettingsButtonBackground->setMaterialName("General/StudySettingsButton");
    //numpadBackground->setMaterialName("General/NumpadBackground");
    
    //Position Buttons
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.01, 0.90), Vector2(0.30, 0.08), backButtonBackground, NULL);
    buttons[BUTTON_CONTROL_SETTINGS].setButton("controlsettings", overlays[0], GMM_RELATIVE, Vector2(0.33, 0.90), Vector2(0.30, 0.08), controlSettingsButtonBackground, NULL);
    buttons[BUTTON_STUDY_SETTINGS].setButton("studysettings", overlays[0], GMM_RELATIVE, Vector2(0.65, .90), Vector2(0.30, 0.08), studySettingsButtonBackground, NULL);
    // The Enable Tutorial Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLETUTORIALS].setButton("checktutorials", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.650), Vector2(pw, ph), enableTutorialsButtonBackground, NULL);
    }
    
    // The Sync Data Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLESYNCDATA].setButton("checksyncdata", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.750), Vector2(pw, ph), syncDataButtonBackground, NULL);
    }


}

void HudMainSettings::positionSliderBalls()
{
    // Set the ball position to the current settings
    musicVolumeSlider->adjust();
    musicVolumeSlider->setBallPosition(player->musicVolume * 100);
    soundVolumeSlider->adjust();
    soundVolumeSlider->setBallPosition(player->soundVolume * 100);
    holdoutSlider->adjust();
    holdoutSlider->setBallPosition(player->holdout * 100);
    holdoutLBSlider->adjust();
    holdoutLBSlider->setBallPosition(player->holdoutLB * 100);
    holdoutUBSlider->adjust();
    holdoutUBSlider->setBallPosition(player->holdoutUB * 100);
}

void HudMainSettings::link(Player* player)
{
    this->player = player;
}

void HudMainSettings::unlink()
{
    this->player = NULL;
}