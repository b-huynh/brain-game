//
//  HudTabSettings.cpp
//  VinezorsiOS
//
//  Created by BGC_Bernie on 4/28/15.
//
//

#include "HudTabSettings.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

HudTabSettings::HudTabSettings(Player* player)
:Hud()
{
    link(player);
    init();
}

HudTabSettings::~HudTabSettings()
{
    dealloc();
    unlink();
}

void HudTabSettings::init()
{
    alloc();
    initOverlay();
    showOverlays();
}

void HudTabSettings::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudTabSettings::update(float elapsed)
{
    
}

void HudTabSettings::alloc()
{
    //std::cout << "alloc" << std::endl;
    // Allocate Resources
    tabSettingsEntireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TabSettingsEntireBackground"));
    
    tabSettingsTitleBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TabSettingsTitleBackground"));
    tabSettingsTitleText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TabSettingsTitleText"));
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TabSettingsBackButtonBackground"));

    paragraphTextBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TabSettingsParagraphTextBackground"));

    tabSettingsLineSeperator = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TabSettingsLineSeperator"));
    
    paragraphText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TabSettingsparagraphText"));
    
    enableUnlimitedFuelTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "enableUnlimitedFuelTextDisplay"));
    
    enableUnlimitedFuelButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "enableUnlimitedFuelButtonBackground"));
    
    enableUnlimitedFuelBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "enableUnlimitedFuelBackground"));
    
    tabSettingsMainSettingDottedSeperator = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "tabSettingsMainSettingDottedSeperator"));
    
    backButtonText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "backButtonText"));
    
    enableUnlimitedFuelUnderline = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "enableUnlimitedFuelUnderline"));
    
    GeneralOptionBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "GeneralOptionBackground"));
    GeneralOptionButton = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "GeneralOptionButton"));
    GeneralOptionText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "GeneralOptionText"));
    

    
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS);
    
    // Create an overlay, and add the panel
    // Two Overlays: Left/Right Overlay.
    // Remove overlay objects for non-selected settings!
    
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("TabSettingsOverlay");
    tabSettingsTitleBackground->addChild(tabSettingsTitleText);
    
    overlay1->add2D(tabSettingsEntireBackground);
    backButtonBackground->addChild(backButtonText);
    overlay1->add2D(backButtonBackground);
    paragraphTextBackground->addChild(paragraphText);
    overlay1->add2D(paragraphTextBackground);
    
    // Unlimited Fuel
    overlay1->add2D(enableUnlimitedFuelBackground);
    enableUnlimitedFuelBackground->addChild(enableUnlimitedFuelTextDisplay);
    enableUnlimitedFuelBackground->addChild(enableUnlimitedFuelButtonBackground);
    overlay1->add2D(enableUnlimitedFuelUnderline);
    
    //General Option
    overlay1->add2D(GeneralOptionBackground);
    GeneralOptionBackground->addChild(GeneralOptionButton);
    GeneralOptionBackground->addChild(GeneralOptionText);


    overlay1->add2D(tabSettingsTitleBackground);
    overlay1->add2D(tabSettingsLineSeperator);
    overlay1->add2D(tabSettingsMainSettingDottedSeperator);


    
    overlays.push_back(overlay1);
    
    paragraphTextBackground->setMetricsMode(GMM_RELATIVE);
    paragraphTextBackground->setPosition(0.3, 0.1);
    paragraphTextBackground->setDimensions(0.70, 0.70);
    //paragraphTextBackground->setMaterialName("General/TutorialBackdrop");
    

    positionSliderBalls();
    
}

void HudTabSettings::dealloc()
{
    
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(tabSettingsEntireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(tabSettingsTitleBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(tabSettingsTitleText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(paragraphTextBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(tabSettingsLineSeperator);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(paragraphText);
    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableUnlimitedFuelBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableUnlimitedFuelTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableUnlimitedFuelButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableUnlimitedFuelUnderline);
    
    //General Option
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(GeneralOptionText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(GeneralOptionButton);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(GeneralOptionBackground);

    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(tabSettingsMainSettingDottedSeperator);

    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonText);

    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
    

}

void HudTabSettings::initOverlay()
{
    Ogre::ColourValue fontColor = Ogre::ColourValue(0.62f, 0.85f, 0.85f);

    //General Option
    
    GeneralOptionBackground->setMetricsMode(GMM_RELATIVE);
    GeneralOptionBackground->setPosition(0.155, 0.115);
    GeneralOptionBackground->setDimensions(0, 0);
    
    //GeneralOptionButton->setMaterialName("General/BackButton1");
    
    GeneralOptionText->setMetricsMode(GMM_RELATIVE);
    GeneralOptionText->setAlignment(TextAreaOverlayElement::Center);
    GeneralOptionText->setPosition(0.00, 0.000);
    GeneralOptionText->setCharHeight(0.025 * FONT_SZ_MULT);
    GeneralOptionText->setFontName("MainSmall");
    GeneralOptionText->setCaption("General");
    GeneralOptionText->setColour(fontColor);
    
    /*GeneralOptionBackground
    GeneralOptionButton
    GeneralOptionText*/
    
    backButtonText->setMetricsMode(GMM_RELATIVE);
    backButtonText->setAlignment(TextAreaOverlayElement::Center);
    backButtonText->setPosition(0.07, 0.0);
    backButtonText->setCharHeight(0.030 * FONT_SZ_MULT);
    backButtonText->setFontName("MainSmall");
    backButtonText->setCaption("Back");
    backButtonText->setColour(fontColor);
    
    tabSettingsMainSettingDottedSeperator->setMetricsMode(GMM_RELATIVE); //GMM_PIXELS
    tabSettingsMainSettingDottedSeperator->setPosition(0.001, 0.165);
    tabSettingsMainSettingDottedSeperator->setDimensions(.35, .0025);//0.10);
    tabSettingsMainSettingDottedSeperator->setMaterialName("General/TabSettingsLDottedSeperator");
    
    // Link and set resources
    //tabSettingsEntireBackground->setMetricsMode(GMM_RELATIVE);
    //tabSettingsEntireBackground->setPosition(0.00, 0.00);
    //tabSettingsEntireBackground->setDimensions(1.00, 1.00);
    //tabSettingsEntireBackground->setMaterialName("General/ScreenBackgroundSchedulerMenu");

    
    tabSettingsTitleBackground->setMetricsMode(GMM_RELATIVE); //GMM_PIXELS
    tabSettingsTitleBackground->setPosition(0.00, 0.0);
    tabSettingsTitleBackground->setDimensions(1, .1);//0.10);
    tabSettingsTitleBackground->setMaterialName("General/TabSettingsTitleBar");
    
    //TabSettingsLineSeperator
    tabSettingsLineSeperator->setMetricsMode(GMM_RELATIVE);
    tabSettingsLineSeperator->setPosition(0.35, 0.0);
    tabSettingsLineSeperator->setDimensions(0.001, 1);
    tabSettingsLineSeperator->setMaterialName("General/TabSettingsLineSeperator");

    //Color: Look in scheduler
    tabSettingsTitleText->setMetricsMode(GMM_RELATIVE);
    tabSettingsTitleText->setAlignment(TextAreaOverlayElement::Center);
    tabSettingsTitleText->setPosition(0.70, 0.01);
    tabSettingsTitleText->setCharHeight(0.030 * FONT_SZ_MULT);
    tabSettingsTitleText->setFontName("MainSmall");
    tabSettingsTitleText->setCaption("General");
    tabSettingsTitleText->setColour(fontColor);
    
    paragraphText->setMetricsMode(GMM_RELATIVE);
    paragraphText->setAlignment(TextAreaOverlayElement::Center);
    paragraphText->setPosition(0.10, 0.5);
    paragraphText->setCharHeight(0.030 * FONT_SZ_MULT);
    paragraphText->setFontName("MainSmall");
    //paragraphText->setCaption("TESTING");
    paragraphText->setColour(fontColor);
    
    enableUnlimitedFuelBackground->setMetricsMode(GMM_RELATIVE);
    enableUnlimitedFuelBackground->setPosition(0.40, 0.06); //.2,.15
    enableUnlimitedFuelBackground->setDimensions(0.60, 0.10);
    
    enableUnlimitedFuelUnderline->setMetricsMode(GMM_RELATIVE); //GMM_PIXELS
    enableUnlimitedFuelUnderline->setPosition(0.35, 0.165);
    enableUnlimitedFuelUnderline->setDimensions(.65, .0025);//0.10);
    enableUnlimitedFuelUnderline->setMaterialName("General/TabSettingsRDottedSeperator");
    
    enableUnlimitedFuelTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableUnlimitedFuelTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableUnlimitedFuelTextDisplay->setPosition(0.0, 0.05);
    enableUnlimitedFuelTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT); //.03
    enableUnlimitedFuelTextDisplay->setFontName("MainSmall");
    enableUnlimitedFuelTextDisplay->setCaption("Tutorials");
    enableUnlimitedFuelTextDisplay->setColour(fontColor);

    
    //backButtonBackground->setMaterialName("General/BackButton1");
    enableUnlimitedFuelButtonBackground->setMaterialName("General/TabSettingsSwitchOff");

    
    //Position Buttons
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.04, 0.90), Vector2(0.15, 0.07), backButtonBackground, NULL);
    

    // calculate dimensions for button size and make sure it's square
    //float ph = 0.05;
    //float pw = ph * (globals.screenWidth / globals.screenHeight);
    buttons[BUTTON_ENABLE_UNLIMITED_FUEL].setButton("checkfuel", overlays[0], GMM_RELATIVE, Vector2(0.45, 0.045),
                                                    Vector2(.095, .040), enableUnlimitedFuelButtonBackground, NULL);
    
    buttons[BUTTON_GENERAL_OPTION].setButton("genoption", overlays[0], GMM_RELATIVE, Vector2(0.5, 0.5),
                                                    Vector2(.2, .2), GeneralOptionButton, NULL);
    
}

void HudTabSettings::positionSliderBalls()
{
   
}

void HudTabSettings::link(Player* player)
{
    this->player = player;
}

void HudTabSettings::unlink()
{
    this->player = NULL;
}