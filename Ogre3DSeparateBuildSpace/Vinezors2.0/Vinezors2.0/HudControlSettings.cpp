//
//  HudControlSettings.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 5/2/14.
//
//

#include "HudControlSettings.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

HudControlSettings::HudControlSettings(Player* player)
:Hud()
{
    link(player);
    init();
}

HudControlSettings::~HudControlSettings()
{
    dealloc();
    unlink();
}

void HudControlSettings::init()
{
    alloc();
    initOverlay();
    showOverlays();
}

void HudControlSettings::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudControlSettings::update(float elapsed)
{
    if (player->inverted)
        invertedButtonBackground->setMaterialName("General/CheckboxGreen");
    else
        invertedButtonBackground->setMaterialName("General/CheckboxBlank");
    
    maxVelSliderDisplay->setCaption(Util::toStringInt(player->maxVel));
    minVelStopperSliderDisplay->setCaption(Util::toStringInt(player->minVelStopper));
    dampingDecayFreeSliderDisplay->setCaption(Util::toStringFloat(player->dampingDecayFree));
    dampingDecayStopSliderDisplay->setCaption(Util::toStringFloat(player->dampingDecayStop));
    dampingDropFreeSliderDisplay->setCaption(Util::toStringInt(player->dampingDropFree));
    dampingDropStopSliderDisplay->setCaption(Util::toStringInt(player->dampingDropStop));
}

void HudControlSettings::resetDefaults()
{
    player->initSettings();
    positionSliderBalls();
}

void HudControlSettings::alloc()
{
    // Allocate Resources
    controlSettingsBackdrop = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsBackdrop"));
    controlSettingsTitleBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsTitleBackground"));
    controlSettingsTitleText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsTitleText"));
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsBackButtonBackground"));
    defaultsButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsDefaultsButtonBackground"));
    
    maxVelSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsMaxVelSliderRangeBackground"));
    maxVelSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsMaxVelSliderBallBackground"));
    maxVelSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsMaxVelText"));
    maxVelSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsMaxVelDisplay"));
    
    minVelStopperSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsMinVelStopperSliderRangeBackground"));
    minVelStopperSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsMinVelStopperSliderBallBackground"));
    minVelStopperSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsMinVelStopperText"));
    minVelStopperSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsMinVelStopperDisplay"));
    
    dampingDecayFreeSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsDampingDecayFreeSliderRangeBackground"));
    dampingDecayFreeSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsDampingDecayFreeSliderBallBackground"));
    dampingDecayFreeSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsDampingDecayFreeText"));
    dampingDecayFreeSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsDampingDecayFreeDisplay"));
    
    dampingDecayStopSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsDampingDecayStopSliderRangeBackground"));
    dampingDecayStopSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsDampingDecayStopSliderBallBackground"));
    dampingDecayStopSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsDampingDecayStopText"));
    dampingDecayStopSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsDampingDecayStopDisplay"));
    
    dampingDropFreeSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsDampingDropFreeSliderRangeBackground"));
    dampingDropFreeSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsDampingDropFreeSliderBallBackground"));
    dampingDropFreeSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsDampingDropFreeText"));
    dampingDropFreeSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsDampingDropFreeDisplay"));
    
    dampingDropStopSliderRangeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsDampingDropStopSliderRangeBackground"));
    dampingDropStopSliderBallBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsDampingDropStopSliderBallBackground"));
    dampingDropStopSliderText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsDampingDropStopText"));
    dampingDropStopSliderDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsDampingDropStopDisplay"));
    
    invertedEntireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsInvertedEntireBackground"));
    invertedTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ControlSettingsInvertedTextDisplay"));;
    invertedButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ControlSettingsInvertedButtonBackground"));
    
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("ControlSettingsOverlay");
    overlay1->add2D(controlSettingsBackdrop);
    overlay1->add2D(controlSettingsTitleBackground);
    controlSettingsTitleBackground->addChild(controlSettingsTitleText);
    
    overlay1->add2D(maxVelSliderRangeBackground);
    maxVelSliderRangeBackground->addChild(maxVelSliderBallBackground);
    maxVelSliderRangeBackground->addChild(maxVelSliderText);
    maxVelSliderRangeBackground->addChild(maxVelSliderDisplay);
    
    overlay1->add2D(minVelStopperSliderRangeBackground);
    minVelStopperSliderRangeBackground->addChild(minVelStopperSliderBallBackground);
    minVelStopperSliderRangeBackground->addChild(minVelStopperSliderText);
    minVelStopperSliderRangeBackground->addChild(minVelStopperSliderDisplay);
    
    overlay1->add2D(dampingDecayFreeSliderRangeBackground);
    dampingDecayFreeSliderRangeBackground->addChild(dampingDecayFreeSliderBallBackground);
    dampingDecayFreeSliderRangeBackground->addChild(dampingDecayFreeSliderText);
    dampingDecayFreeSliderRangeBackground->addChild(dampingDecayFreeSliderDisplay);
    
    overlay1->add2D(dampingDecayStopSliderRangeBackground);
    dampingDecayStopSliderRangeBackground->addChild(dampingDecayStopSliderBallBackground);
    dampingDecayStopSliderRangeBackground->addChild(dampingDecayStopSliderText);
    dampingDecayStopSliderRangeBackground->addChild(dampingDecayStopSliderDisplay);
    
    overlay1->add2D(dampingDropFreeSliderRangeBackground);
    dampingDropFreeSliderRangeBackground->addChild(dampingDropFreeSliderBallBackground);
    dampingDropFreeSliderRangeBackground->addChild(dampingDropFreeSliderText);
    dampingDropFreeSliderRangeBackground->addChild(dampingDropFreeSliderDisplay);
    
    overlay1->add2D(dampingDropStopSliderRangeBackground);
    dampingDropStopSliderRangeBackground->addChild(dampingDropStopSliderBallBackground);
    dampingDropStopSliderRangeBackground->addChild(dampingDropStopSliderText);
    dampingDropStopSliderRangeBackground->addChild(dampingDropStopSliderDisplay);
    
    overlay1->add2D(invertedEntireBackground);
    invertedEntireBackground->addChild(invertedTextDisplay);
    overlay1->add2D(invertedButtonBackground);
    
    overlay1->add2D(backButtonBackground);
    overlay1->add2D(defaultsButtonBackground);
    overlays.push_back(overlay1);
    
    maxVelSlider = new HudSlider();
    minVelStopperSlider = new HudSlider();
    dampingDecayFreeSlider = new HudSlider();
    dampingDecayStopSlider = new HudSlider();
    dampingDropFreeSlider = new HudSlider();
    dampingDropStopSlider = new HudSlider();
    
    // Horizontal slider
    maxVelSlider->setSlider("maxVel", overlays[0], Vector2(0.10, 0.20), Vector2(0.60, 0.03), Vector2(0.05, 0.03), false,
                            0, 5000, 5001, maxVelSliderRangeBackground, maxVelSliderBallBackground);
    
    minVelStopperSlider->setSlider("minVelStopper", overlays[0], Vector2(0.10, 0.30), Vector2(0.60, 0.03), Vector2(0.05, 0.03), false,
                                   0, 2500, 2501, minVelStopperSliderRangeBackground, minVelStopperSliderBallBackground);
    
    dampingDecayFreeSlider->setSlider("dampingDecayFree", overlays[0], Vector2(0.10, 0.40), Vector2(0.60, 0.03), Vector2(0.05, 0.03), false,
                                      0, 1000, 1001, dampingDecayFreeSliderRangeBackground, dampingDecayFreeSliderBallBackground);
    
    dampingDecayStopSlider->setSlider("dampingDecayStop", overlays[0], Vector2(0.10, 0.50), Vector2(0.60, 0.03), Vector2(0.05, 0.03), false,
                                    0, 1000, 1001, dampingDecayStopSliderRangeBackground, dampingDecayStopSliderBallBackground);
    
    dampingDropFreeSlider->setSlider("dampingDropFree", overlays[0], Vector2(0.10, 0.60), Vector2(0.60, 0.03), Vector2(0.05, 0.03), false,
                                     0, 1000, 1001, dampingDropFreeSliderRangeBackground, dampingDropFreeSliderBallBackground);
    
    dampingDropStopSlider->setSlider("dampingDropStop", overlays[0], Vector2(0.10, 0.70), Vector2(0.60, 0.03), Vector2(0.05, 0.03), false,
                                     0, 1000, 1001, dampingDropStopSliderRangeBackground, dampingDropStopSliderBallBackground);
    
    positionSliderBalls();
}

void HudControlSettings::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(controlSettingsBackdrop);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(controlSettingsTitleBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(controlSettingsTitleText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(defaultsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(maxVelSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(maxVelSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(maxVelSliderText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(maxVelSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(minVelStopperSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(minVelStopperSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(minVelStopperSliderText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(minVelStopperSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDecayFreeSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDecayFreeSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDecayFreeSliderText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDecayFreeSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDecayStopSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDecayStopSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDecayStopSliderText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDecayStopSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDropFreeSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDropFreeSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDropFreeSliderText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDropFreeSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDropStopSliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDropStopSliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDropStopSliderText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(dampingDropStopSliderDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(invertedEntireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(invertedTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(invertedButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
    
    if (maxVelSlider) delete maxVelSlider; maxVelSlider = NULL;
    if (minVelStopperSlider) delete minVelStopperSlider; minVelStopperSlider = NULL;
    if (dampingDecayFreeSlider) delete dampingDecayFreeSlider; dampingDecayFreeSlider = NULL;
    if (dampingDecayStopSlider) delete dampingDecayStopSlider; dampingDecayStopSlider = NULL;
    if (dampingDropFreeSlider) delete dampingDropFreeSlider; dampingDropFreeSlider = NULL;
    if (dampingDropStopSlider) delete dampingDropStopSlider; dampingDropStopSlider = NULL;
}

void HudControlSettings::initOverlay()
{
    // Link and set resources
    controlSettingsBackdrop->setMetricsMode(GMM_RELATIVE);
    controlSettingsBackdrop->setPosition(0.00, 0.025);
    controlSettingsBackdrop->setDimensions(1.00, 0.95);
    controlSettingsBackdrop->setMaterialName("General/TutorialBackdrop");
    
    controlSettingsTitleBackground->setMetricsMode(GMM_RELATIVE);
    controlSettingsTitleBackground->setPosition(0.30, 0.025);
    controlSettingsTitleBackground->setDimensions(0.40, 0.20);
    
    controlSettingsTitleText->setMetricsMode(GMM_RELATIVE);
    controlSettingsTitleText->setAlignment(TextAreaOverlayElement::Center);
    controlSettingsTitleText->setPosition(0.20, 0.00);
    controlSettingsTitleText->setCharHeight(0.046 * FONT_SZ_MULT);
    controlSettingsTitleText->setFontName("MainSmall");
    controlSettingsTitleText->setCaption("Control Settings");
    
    maxVelSliderText->setMetricsMode(GMM_RELATIVE);
    maxVelSliderText->setAlignment(TextAreaOverlayElement::Left);
    maxVelSliderText->setPosition(0.00, -0.03);
    maxVelSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    maxVelSliderText->setFontName("MainSmall");
    maxVelSliderText->setCaption("Max Velocity");
    
    maxVelSliderDisplay->setMetricsMode(GMM_RELATIVE);
    maxVelSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    maxVelSliderDisplay->setPosition(0.65, 0.00);
    maxVelSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    maxVelSliderDisplay->setFontName("MainSmall");
    
    minVelStopperSliderText->setMetricsMode(GMM_RELATIVE);
    minVelStopperSliderText->setAlignment(TextAreaOverlayElement::Left);
    minVelStopperSliderText->setPosition(0.00, -0.03);
    minVelStopperSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    minVelStopperSliderText->setFontName("MainSmall");
    minVelStopperSliderText->setCaption("Minimum Velocity Crossing Panel");
    
    minVelStopperSliderDisplay->setMetricsMode(GMM_RELATIVE);
    minVelStopperSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    minVelStopperSliderDisplay->setPosition(0.65, 0.00);
    minVelStopperSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    minVelStopperSliderDisplay->setFontName("MainSmall");
    
    dampingDecayFreeSliderText->setMetricsMode(GMM_RELATIVE);
    dampingDecayFreeSliderText->setAlignment(TextAreaOverlayElement::Left);
    dampingDecayFreeSliderText->setPosition(0.00, -0.03);
    dampingDecayFreeSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    dampingDecayFreeSliderText->setFontName("MainSmall");
    dampingDecayFreeSliderText->setCaption("Velocity Decay in Free Motion");
    
    dampingDecayFreeSliderDisplay->setMetricsMode(GMM_RELATIVE);
    dampingDecayFreeSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    dampingDecayFreeSliderDisplay->setPosition(0.65, 0.00);
    dampingDecayFreeSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    dampingDecayFreeSliderDisplay->setFontName("MainSmall");
    
    dampingDecayStopSliderText->setMetricsMode(GMM_RELATIVE);
    dampingDecayStopSliderText->setAlignment(TextAreaOverlayElement::Left);
    dampingDecayStopSliderText->setPosition(0.00, -0.03);
    dampingDecayStopSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    dampingDecayStopSliderText->setFontName("MainSmall");
    dampingDecayStopSliderText->setCaption("Velocity Decay in Stopping");
    
    dampingDecayStopSliderDisplay->setMetricsMode(GMM_RELATIVE);
    dampingDecayStopSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    dampingDecayStopSliderDisplay->setPosition(0.65, 0.00);
    dampingDecayStopSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    dampingDecayStopSliderDisplay->setFontName("MainSmall");
    
    dampingDropFreeSliderText->setMetricsMode(GMM_RELATIVE);
    dampingDropFreeSliderText->setAlignment(TextAreaOverlayElement::Left);
    dampingDropFreeSliderText->setPosition(0.00, -0.03);
    dampingDropFreeSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    dampingDropFreeSliderText->setFontName("MainSmall");
    dampingDropFreeSliderText->setCaption("Velocity Drop in Free Motion");
    
    dampingDropFreeSliderDisplay->setMetricsMode(GMM_RELATIVE);
    dampingDropFreeSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    dampingDropFreeSliderDisplay->setPosition(0.65, 0.00);
    dampingDropFreeSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    dampingDropFreeSliderDisplay->setFontName("MainSmall");
    
    dampingDropStopSliderText->setMetricsMode(GMM_RELATIVE);
    dampingDropStopSliderText->setAlignment(TextAreaOverlayElement::Left);
    dampingDropStopSliderText->setPosition(0.00, -0.03);
    dampingDropStopSliderText->setCharHeight(0.026 * FONT_SZ_MULT);
    dampingDropStopSliderText->setFontName("MainSmall");
    dampingDropStopSliderText->setCaption("Velocity Drop in Stopping");
    
    dampingDropStopSliderDisplay->setMetricsMode(GMM_RELATIVE);
    dampingDropStopSliderDisplay->setAlignment(TextAreaOverlayElement::Center);
    dampingDropStopSliderDisplay->setPosition(0.65, 0.00);
    dampingDropStopSliderDisplay->setCharHeight(0.026 * FONT_SZ_MULT);
    dampingDropStopSliderDisplay->setFontName("MainSmall");
    
    invertedEntireBackground->setMetricsMode(GMM_RELATIVE);
    invertedEntireBackground->setPosition(0.20, 0.75);
    invertedEntireBackground->setDimensions(0.60, 0.10);
    
    invertedTextDisplay->setMetricsMode(GMM_RELATIVE);
    invertedTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    invertedTextDisplay->setPosition(0.0, 0.05);
    invertedTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    invertedTextDisplay->setFontName("MainSmall");
    invertedTextDisplay->setCaption("Inverted Navigation");
    
    maxVelSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    maxVelSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    minVelStopperSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    minVelStopperSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    dampingDecayFreeSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    dampingDecayFreeSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    dampingDecayStopSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    dampingDecayStopSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    dampingDropFreeSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    dampingDropFreeSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    dampingDropStopSliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    dampingDropStopSliderBallBackground->setMaterialName("General/SpeedSliderBallHorizontal");
    
    backButtonBackground->setMaterialName("General/BackButton1");
    defaultsButtonBackground->setMaterialName("General/DefaultsButton");
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.15, 0.90), Vector2(0.30, 0.08), backButtonBackground, NULL);
    buttons[BUTTON_DEFAULTS].setButton("defaults", overlays[0], GMM_RELATIVE, Vector2(0.55, 0.90), Vector2(0.30, 0.08), defaultsButtonBackground, NULL);
    
    // The Enable Tutorial Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_INVERTED].setButton("inverted", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.800), Vector2(pw, ph), invertedButtonBackground, NULL);
    }
    
}

void HudControlSettings::positionSliderBalls()
{
    // Set the ball position to the current settings
    maxVelSlider->adjust();
    maxVelSlider->setBallPosition(player->maxVel);
    minVelStopperSlider->adjust();
    minVelStopperSlider->setBallPosition(player->minVelStopper);
    dampingDecayFreeSlider->adjust();
    dampingDecayFreeSlider->setBallPosition(player->dampingDecayFree * (dampingDecayFreeSlider->getRange() - 1));
    dampingDecayStopSlider->adjust();
    dampingDecayStopSlider->setBallPosition(player->dampingDecayStop * (dampingDecayStopSlider->getRange() - 1));
    dampingDropFreeSlider->adjust();
    dampingDropFreeSlider->setBallPosition(player->dampingDropFree);
    dampingDropStopSlider->adjust();
    dampingDropStopSlider->setBallPosition(player->dampingDropStop);
}

void HudControlSettings::link(Player* player)
{
    this->player = player;
}

void HudControlSettings::unlink()
{
    this->player = NULL;
}