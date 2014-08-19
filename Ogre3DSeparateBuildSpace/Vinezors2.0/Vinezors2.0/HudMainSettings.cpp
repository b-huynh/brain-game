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
    
    
    if (player->getTutorialMgr()->isEnabled())
        enableTutorialsButtonBackground->setMaterialName("General/CheckboxGreen");
    else
        enableTutorialsButtonBackground->setMaterialName("General/CheckboxBlank");
    
    if (player->syncDataToServer)
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
        soundVolumeSliderWarning->setCaption("Certain features are disabled with sound off");
    else
        soundVolumeSliderWarning->setCaption("");
}

void HudMainSettings::alloc()
{
    // Allocate Resources
    mainSettingsTitleBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsTitleBackground"));
    mainSettingsTitleText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsTitleText"));
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsBackButtonBackground"));
    controlSettingsButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsButtonBackground"));
    
    musicVolumeSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsMusicVolumeSliderRangeBackground"));
    musicVolumeSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsMusicVolumeSliderBallBackground"));
    musicVolumeSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsMusicVolumeText"));
    musicVolumeSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsMusicVolumeDisplay"));
    
    //Holdout
    holdoutSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsholdoutSliderRangeBackground"));
    holdoutSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsholdoutSliderBallBackground"));
    holdoutSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsholdoutText"));
    holdoutSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsholdoutDisplay"));

    
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
    overlay1->add2D(mainSettingsTitleBackground);
    mainSettingsTitleBackground->addChild(mainSettingsTitleText);
    
    overlay1->add2D(musicVolumeSliderRangeBackground);
    musicVolumeSliderRangeBackground->addChild(musicVolumeSliderBallBackground);
    musicVolumeSliderRangeBackground->addChild(musicVolumeSliderText);
    musicVolumeSliderRangeBackground->addChild(musicVolumeSliderDisplay);
    
    overlay1->add2D(holdoutSliderRangeBackground);
    holdoutSliderRangeBackground->addChild(holdoutSliderBallBackground);
    holdoutSliderRangeBackground->addChild(holdoutSliderText);
    holdoutSliderRangeBackground->addChild(holdoutSliderDisplay);
    
    overlay1->add2D(soundVolumeSliderRangeBackground);
    soundVolumeSliderRangeBackground->addChild(soundVolumeSliderBallBackground);
    soundVolumeSliderRangeBackground->addChild(soundVolumeSliderText);
    soundVolumeSliderRangeBackground->addChild(soundVolumeSliderDisplay);
    soundVolumeSliderRangeBackground->addChild(soundVolumeSliderWarning);
    
    overlay1->add2D(enableTutorialsEntireBackground);
    enableTutorialsEntireBackground->addChild(enableTutorialsTextDisplay);
    overlay1->add2D(enableTutorialsButtonBackground);
    
    overlay1->add2D(syncDataEntireBackground);
    syncDataEntireBackground->addChild(syncDataTextDisplay);
    overlay1->add2D(syncDataButtonBackground);
    syncDataEntireBackground->addChild(syncDataInternetNotification);
    
    overlay1->add2D(backButtonBackground);
    overlay1->add2D(controlSettingsButtonBackground);
    overlays.push_back(overlay1);
    
    musicVolumeSlider = new HudSlider();
    soundVolumeSlider = new HudSlider();
    holdoutSlider = new HudSlider();
    
    // Horizontal slider
    
    musicVolumeSlider->setSlider("musicvolume", overlays[0], Vector2(0.10, 0.40), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false,
                                0, 100, 101, musicVolumeSliderRangeBackground, musicVolumeSliderBallBackground);
    soundVolumeSlider->setSlider("soundvolume", overlays[0], Vector2(0.10, 0.25), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false,
                                 0, 100, 101, soundVolumeSliderRangeBackground, soundVolumeSliderBallBackground);
    holdoutSlider->setSlider("holdout", overlays[0], Vector2(0.10, 0.55), Vector2(0.60, 0.05), Vector2(0.05, 0.05), false, 0, 100, 101, holdoutSliderRangeBackground, holdoutSliderBallBackground);

    positionSliderBalls();
}

void HudMainSettings::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsTitleBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsTitleText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(controlSettingsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(musicVolumeSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(musicVolumeSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(musicVolumeSliderText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(musicVolumeSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderText);
    // OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(holdoutSliderDisplay);
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
}

void HudMainSettings::initOverlay()
{
    // Link and set resources
    mainSettingsTitleBackground->setMetricsMode(GMM_RELATIVE);
    mainSettingsTitleBackground->setPosition(0.30, 0.05);
    mainSettingsTitleBackground->setDimensions(0.40, 0.20);
    
    mainSettingsTitleText->setMetricsMode(GMM_RELATIVE);
    mainSettingsTitleText->setAlignment(TextAreaOverlayElement::Center);
    mainSettingsTitleText->setPosition(0.20, 0.00);
    mainSettingsTitleText->setCharHeight(0.046 * FONT_SZ_MULT);
    mainSettingsTitleText->setFontName("MainSmall");
    mainSettingsTitleText->setCaption("Main Settings");
    
    musicVolumeSliderDisplay->setMetricsMode(GMM_RELATIVE);
    musicVolumeSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    musicVolumeSliderDisplay->setPosition(0.70, 0.01);
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
    holdoutSliderDisplay->setPosition(0.70, 0.01);
    holdoutSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    holdoutSliderDisplay->setFontName("MainSmall");
    
    holdoutSliderText->setMetricsMode(GMM_RELATIVE);
    holdoutSliderText->setAlignment(TextAreaOverlayElement::Left);
    holdoutSliderText->setPosition(0.00, -0.03);
    holdoutSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    holdoutSliderText->setFontName("MainSmall");
    holdoutSliderText->setCaption("Holdout Frequency");
    
    soundVolumeSliderDisplay->setMetricsMode(GMM_RELATIVE);
    soundVolumeSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    soundVolumeSliderDisplay->setPosition(0.70, 0.01);
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
    
    musicVolumeSliderRangeBackground->setMaterialName("General/BasicSliderRangeHorizontal");
    musicVolumeSliderBallBackground->setMaterialName("General/BasicSliderBall");
    soundVolumeSliderRangeBackground->setMaterialName("General/BasicSliderRangeHorizontal");
    soundVolumeSliderBallBackground->setMaterialName("General/BasicSliderBall");
    holdoutSliderRangeBackground->setMaterialName("General/BasicSliderRangeHorizontal");
    holdoutSliderBallBackground->setMaterialName("General/BasicSliderBall");
    
    backButtonBackground->setMaterialName("General/BackButton");
    controlSettingsButtonBackground->setMaterialName("General/ControlSettingsButton");
    
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.15, 0.90), Vector2(0.30, 0.08), backButtonBackground, NULL);
    buttons[BUTTON_CONTROL_SETTINGS].setButton("controlsettings", overlays[0], GMM_RELATIVE, Vector2(0.55, 0.90), Vector2(0.30, 0.08), controlSettingsButtonBackground, NULL);
    
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
}

void HudMainSettings::link(Player* player)
{
    this->player = player;
}

void HudMainSettings::unlink()
{
    this->player = NULL;
}