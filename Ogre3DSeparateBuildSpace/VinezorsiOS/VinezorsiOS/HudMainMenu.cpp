//
//  HudMainMenu.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#include "HudMainMenu.h"
#include "Player.h"

HudMainMenu::HudMainMenu(Player* player)
:Hud()
{
    link(player);
    init();
}

HudMainMenu::~HudMainMenu()
{
    dealloc();
    unlink();
}

void HudMainMenu::init()
{
    alloc();
    initOverlay();
    showOverlays();
}

void HudMainMenu::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudMainMenu::update(float elapsed)
{
    if(startDotTimer)
    {
        numToWait -= elapsed;
        if(numToWait <= 0)
        {
            enterStudySettings = true;
        }
    }
    
    if(startDotTimerIncorrect)
    {
        numToWaitIncorrect -=elapsed;
        if(numToWaitIncorrect <=0)
        {
            //Animate the choices!
            
            password_Choice0->setMaterialName("General/PasswordDotDisabled");
            password_Choice1->setMaterialName("General/PasswordDotDisabled");
            password_Choice2->setMaterialName("General/PasswordDotDisabled");
            password_Choice3->setMaterialName("General/PasswordDotDisabled");
            Passcode_counter = 0;
            startDotTimerIncorrect = false;
            numToWaitIncorrect = .3;
        }
    }
    
    if((Passcode_counter == 1) && (password_Choice0->getMaterialName() != "General/PasswordDotEnabled"))
    {
        password_Choice0->setMaterialName("General/PasswordDotEnabled");
        //std::cout<<"Woah1!\n";
    }
    if((Passcode_counter == 2) && (password_Choice1->getMaterialName() != "General/PasswordDotEnabled"))
    {
        password_Choice1->setMaterialName("General/PasswordDotEnabled");
        //std::cout<<"Woah2!\n";
    }
    if((Passcode_counter == 3) && (password_Choice2->getMaterialName() != "General/PasswordDotEnabled"))
    {
        password_Choice2->setMaterialName("General/PasswordDotEnabled");
        //std::cout<<"Woah3!\n";
    }
    if((Passcode_counter == 4) && (password_Choice3->getMaterialName() != "General/PasswordDotEnabled"))
    {
        password_Choice3->setMaterialName("General/PasswordDotEnabled");
        //std::cout<<"Woah4!\n";
        
        //Check if passcode is valid:
        bool valid = true;
        for(int i = 0; i < PASSWORD_LENGTH; i++)
        {
            if(user_password[i] != PASSWORD[i])
            {
                valid = false;
            }
        }
        
        if(valid)
        {
            //std::cout<<"HEHEHEHE\n";
            startDotTimer = true;
        }
        else
        {
            startDotTimerIncorrect = true;
            
            
        }
        
    }
    
    if(enableNumpad)
    {
        //Show Numpad
        //numpadBackground->setEnabled(true);
        numpadBackground->show();
    }
    else
    {
        //NumpadDisappear!
        numpadBackground->hide();
        password_Choice0->setMaterialName("General/PasswordDotDisabled");
        password_Choice1->setMaterialName("General/PasswordDotDisabled");
        password_Choice2->setMaterialName("General/PasswordDotDisabled");
        password_Choice3->setMaterialName("General/PasswordDotDisabled");
        Passcode_counter = 0;
        
        
    }

}

void HudMainMenu::alloc()
{
    // Allocate Resources
    mainMenuEntireBackground = static_cast<PanelOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuEntireBackground"));
    playButtonBackground = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuPlayButtonBackground"));
    creditsButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuCreditsButtonBackground"));
    settingsButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuSettingsButtonBackground"));
    informationButtonBackground= static_cast<PanelOverlayElement*>(
                                                                   OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuInformationButtonBackground"));
    
    playButtonTextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainMenuPlayButtonText"));
    creditsButtonTextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainMenuCreditsButtonText"));
    settingsButtonTextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainMenuSettingsButtonText"));
    informationButtonTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainMenuInformationButtonText"));
    
    //Numpad
    numpadBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadBackground"));
    numpadButton_0 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton0"));
    numpadButton_0_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton0Text"));
    numpadButton_1 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton1"));
    numpadButton_1_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton1Text"));
    numpadButton_2 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton2"));
    numpadButton_2_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton2Text"));
    numpadButton_3 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton3"));
    numpadButton_3_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton3Text"));
    numpadButton_4 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton4"));
    numpadButton_4_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton4Text"));
    numpadButton_5 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton5"));
    numpadButton_5_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton5Text"));
    numpadButton_6 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton6"));
    numpadButton_6_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton6Text"));
    numpadButton_7 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton7"));
    numpadButton_7_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton7Text"));
    numpadButton_8 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton8"));
    numpadButton_8_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton8Text"));
    numpadButton_9 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButton9"));
    numpadButton_9_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButton9Text"));
    numpadButton_Cancel = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadButtonCancel"));
    numpadButton_Cancel_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadButtonCancelText"));
    
    //Password
    password_Choice0= static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadPasswordChoice0"));
    password_Choice1= static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadPasswordChoice1"));
    password_Choice2= static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadPasswordChoice2"));
    password_Choice3= static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainSettingsNumpadPasswordChoice3"));
    password_Title= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "MainSettingsNumpadPasswordText"));
    
 
    buttons = std::vector<HudButton>(15);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("MainMenuOverlay");
    overlay1->add2D(mainMenuEntireBackground);
    overlay1->add2D(playButtonBackground);
    playButtonBackground->addChild(playButtonTextDisplay);
    overlay1->add2D(creditsButtonBackground);
    creditsButtonBackground->addChild(creditsButtonTextDisplay);
    overlay1->add2D(settingsButtonBackground);
    settingsButtonBackground->addChild(settingsButtonTextDisplay);
    overlay1->add2D(informationButtonBackground);
    informationButtonBackground->addChild(informationButtonTextDisplay);

    
    //Numpad
    numpadButton_0->addChild(numpadButton_0_TextDisplay);
    numpadButton_1->addChild(numpadButton_1_TextDisplay);
    numpadButton_2->addChild(numpadButton_2_TextDisplay);
    numpadButton_3->addChild(numpadButton_3_TextDisplay);
    numpadButton_4->addChild(numpadButton_4_TextDisplay);
    numpadButton_5->addChild(numpadButton_5_TextDisplay);
    numpadButton_6->addChild(numpadButton_6_TextDisplay);
    numpadButton_7->addChild(numpadButton_7_TextDisplay);
    numpadButton_8->addChild(numpadButton_8_TextDisplay);
    numpadButton_9->addChild(numpadButton_9_TextDisplay);
    numpadButton_Cancel->addChild(numpadButton_Cancel_TextDisplay);
    numpadBackground->addChild(numpadButton_0);
    numpadBackground->addChild(numpadButton_1);
    numpadBackground->addChild(numpadButton_2);
    numpadBackground->addChild(numpadButton_3);
    numpadBackground->addChild(numpadButton_4);
    numpadBackground->addChild(numpadButton_5);
    numpadBackground->addChild(numpadButton_6);
    numpadBackground->addChild(numpadButton_7);
    numpadBackground->addChild(numpadButton_8);
    numpadBackground->addChild(numpadButton_9);
    numpadBackground->addChild(numpadButton_Cancel);
    numpadBackground->addChild(password_Choice0);
    numpadBackground->addChild(password_Choice1);
    numpadBackground->addChild(password_Choice2);
    numpadBackground->addChild(password_Choice3);
    numpadBackground->addChild(password_Title);
    
    
    overlay1->add2D(numpadBackground);

    
    overlays.push_back(overlay1);
}

void HudMainMenu::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainMenuEntireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(playButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(creditsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(settingsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(informationButtonBackground);

    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(playButtonTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(creditsButtonTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(settingsButtonTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(informationButtonTextDisplay);



    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_0);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_0_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_1);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_1_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_2);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_2_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_3);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_3_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_4);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_4_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_5);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_5_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_6);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_6_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_7);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_7_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_8);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_8_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_9);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_9_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_Cancel);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_Cancel_TextDisplay);
    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(password_Title);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(password_Choice0);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(password_Choice1);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(password_Choice2);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(password_Choice3);
    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudMainMenu::initOverlay()
{
    //Set Buttons Text
    playButtonTextDisplay->setMetricsMode(GMM_RELATIVE);
    playButtonTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    playButtonTextDisplay->setPosition(0.015, 0.03);
    playButtonTextDisplay->setCharHeight(0.02 * FONT_SZ_MULT);
    playButtonTextDisplay->setFontName("MainSmall");
    playButtonTextDisplay->setCaption("Train");
    
    creditsButtonTextDisplay->setMetricsMode(GMM_RELATIVE);
    creditsButtonTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    creditsButtonTextDisplay->setPosition(0.015, 0.03);
    creditsButtonTextDisplay->setCharHeight(0.02 * FONT_SZ_MULT);
    creditsButtonTextDisplay->setFontName("MainSmall");
    creditsButtonTextDisplay->setCaption("Levels");
    
    settingsButtonTextDisplay->setMetricsMode(GMM_RELATIVE);
    settingsButtonTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    settingsButtonTextDisplay->setPosition(0.015, 0.03);
    settingsButtonTextDisplay->setCharHeight(0.02 * FONT_SZ_MULT);
    settingsButtonTextDisplay->setFontName("MainSmall");
    settingsButtonTextDisplay->setCaption("Settings");
    
    informationButtonTextDisplay->setMetricsMode(GMM_RELATIVE);
    informationButtonTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    informationButtonTextDisplay->setPosition(0.015, 0.03);
    informationButtonTextDisplay->setCharHeight(0.02 * FONT_SZ_MULT);
    informationButtonTextDisplay->setFontName("MainSmall");
    informationButtonTextDisplay->setCaption("Information");
    
    
    // Link and set resources
    mainMenuEntireBackground->setMetricsMode(GMM_RELATIVE);
    mainMenuEntireBackground->setPosition(0.0, 0.0);
    mainMenuEntireBackground->setDimensions(1.0, 1.0);
    mainMenuEntireBackground->setMaterialName("General/ScreenBackgroundMainMenu");
    
    //Numpad Objects
    numpadBackground->setMetricsMode(GMM_RELATIVE);
    numpadBackground->setPosition(0.650, 0.50);
    numpadBackground->setDimensions(0.34, 0.46);
    
    numpadButton_0->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_1->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_2->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_3->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_4->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_5->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_6->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_7->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_8->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_9->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_Cancel->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    
    password_Choice0->setMetricsMode(GMM_RELATIVE);
    password_Choice0->setPosition(0.06, 0.09);
    password_Choice0->setDimensions(0.03, 0.03);
    
    password_Choice1->setMetricsMode(GMM_RELATIVE);
    password_Choice1->setPosition(0.11, 0.09);
    password_Choice1->setDimensions(0.03, 0.03);
    
    password_Choice2->setMetricsMode(GMM_RELATIVE);
    password_Choice2->setPosition(0.16, 0.09);
    password_Choice2->setDimensions(0.03, 0.03);
    
    password_Choice3->setMetricsMode(GMM_RELATIVE);
    password_Choice3->setPosition(0.21, 0.09);
    password_Choice3->setDimensions(0.03, 0.03);
    
    // password_Choice0->Ogre::OverlayElement::setMaterialName("General/PasswordDotDisabled");
    //password_Choice1->Ogre::OverlayElement::setMaterialName("General/PasswordDotDisabled");
    // password_Choice2->Ogre::OverlayElement::setMaterialName("General/PasswordDotDisabled");
    // password_Choice3->Ogre::OverlayElement::setMaterialName("General/PasswordDotDisabled");
    
    password_Title->setMetricsMode(GMM_RELATIVE);
    password_Title->setAlignment(TextAreaOverlayElement::Center);
    password_Title->setPosition(0.15, 0.04); //.01
    password_Title->setCharHeight(0.015 * FONT_SZ_MULT);
    password_Title->setFontName("MainSmall");
    
    numpadButton_Cancel_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_Cancel_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_Cancel_TextDisplay->setPosition(0.030, 0.008);
    numpadButton_Cancel_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_Cancel_TextDisplay->setFontName("MainSmall");
    numpadButton_Cancel_TextDisplay->setCaption("X");
    
    numpadButton_0_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_0_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_0_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_0_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_0_TextDisplay->setFontName("MainSmall");
    numpadButton_0_TextDisplay->setCaption("0");
    
    numpadButton_1_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_1_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_1_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_1_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_1_TextDisplay->setFontName("MainSmall");
    numpadButton_1_TextDisplay->setCaption("1");
    
    numpadButton_2_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_2_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_2_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_2_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_2_TextDisplay->setFontName("MainSmall");
    numpadButton_2_TextDisplay->setCaption("2");
    
    numpadButton_3_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_3_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_3_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_3_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_3_TextDisplay->setFontName("MainSmall");
    numpadButton_3_TextDisplay->setCaption("3");
    
    numpadButton_4_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_4_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_4_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_4_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_4_TextDisplay->setFontName("MainSmall");
    numpadButton_4_TextDisplay->setCaption("4");
    
    numpadButton_5_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_5_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_5_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_5_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_5_TextDisplay->setFontName("MainSmall");
    numpadButton_5_TextDisplay->setCaption("5");
    
    numpadButton_6_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_6_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_6_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_6_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_6_TextDisplay->setFontName("MainSmall");
    numpadButton_6_TextDisplay->setCaption("6");
    
    numpadButton_7_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_7_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_7_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_7_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_7_TextDisplay->setFontName("MainSmall");
    numpadButton_7_TextDisplay->setCaption("7");
    
    numpadButton_8_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_8_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_8_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_8_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_8_TextDisplay->setFontName("MainSmall");
    numpadButton_8_TextDisplay->setCaption("8");
    
    numpadButton_9_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_9_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_9_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_9_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_9_TextDisplay->setFontName("MainSmall");
    numpadButton_9_TextDisplay->setCaption("9");
    
    numpadBackground->setMaterialName("General/TutorialBackdrop");

    //Set Material to Buttons:
    playButtonBackground->setMaterialName("General/MainMenuButton1");
    creditsButtonBackground->setMaterialName("General/MainMenuButton2");
    settingsButtonBackground->setMaterialName("General/MainMenuButton3");
    informationButtonBackground->setMaterialName("General/MainMenuButton4");

    //End Set Material to Buttons
    
    buttons[BUTTON_PLAY].setButton("play", overlays[0], GMM_RELATIVE, Vector2(0.16, 0.40), Vector2(0.12, 0.08), playButtonBackground, NULL);
    buttons[BUTTON_CREDITS].setButton("credits", overlays[0], GMM_RELATIVE, Vector2(0.16, 0.49), Vector2(0.155, 0.08), creditsButtonBackground, NULL);
    buttons[BUTTON_SETTINGS].setButton("settings", overlays[0], GMM_RELATIVE, Vector2(0.16, 0.58), Vector2(0.190, 0.08), settingsButtonBackground, NULL);
    buttons[BUTTON_INFORMATION].setButton("information", overlays[0], GMM_RELATIVE, Vector2(0.16, 0.67), Vector2(0.225, 0.08), informationButtonBackground, NULL);
    
    
    buttons[BUTTON_NUMPAD_0].setButton("numpadbutton0", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.345), Vector2(0.06, 0.06), numpadButton_0, NULL);
    
    buttons[BUTTON_NUMPAD_1].setButton("numpadbutton1", overlays[0], GMM_RELATIVE, Vector2(0.05, 0.135), Vector2(0.06, 0.06), numpadButton_1, NULL);
    buttons[BUTTON_NUMPAD_2].setButton("numpadbutton2", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.135), Vector2(0.06, 0.06), numpadButton_2, NULL);
    buttons[BUTTON_NUMPAD_3].setButton("numpadbutton3", overlays[0], GMM_RELATIVE, Vector2(0.19, 0.135), Vector2(0.06, 0.06), numpadButton_3, NULL);
    
    buttons[BUTTON_NUMPAD_4].setButton("numpadbutton4", overlays[0], GMM_RELATIVE, Vector2(0.05, 0.205), Vector2(0.06, 0.06), numpadButton_4, NULL);
    buttons[BUTTON_NUMPAD_5].setButton("numpadbutton5", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.205), Vector2(0.06, 0.06), numpadButton_5, NULL);
    buttons[BUTTON_NUMPAD_6].setButton("numpadbutton6", overlays[0], GMM_RELATIVE, Vector2(0.19, 0.205), Vector2(0.06, 0.06), numpadButton_6, NULL);
    
    buttons[BUTTON_NUMPAD_7].setButton("numpadbutton7", overlays[0], GMM_RELATIVE, Vector2(0.05, 0.275), Vector2(0.06, 0.06), numpadButton_7, NULL);
    buttons[BUTTON_NUMPAD_8].setButton("numpadbutton8", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.275), Vector2(0.06, 0.06), numpadButton_8, NULL);
    buttons[BUTTON_NUMPAD_9].setButton("numpadbutton9", overlays[0], GMM_RELATIVE, Vector2(0.19, 0.275), Vector2(0.06, 0.06), numpadButton_9, NULL);
    
    buttons[BUTTON_NUMPAD_CANCEL].setButton("numpadbuttoncancel", overlays[0], GMM_RELATIVE, Vector2(0.19, 0.345), Vector2(0.06, 0.06), numpadButton_Cancel, NULL);

}

void HudMainMenu::link(Player* player)
{
    this->player = player;
}

void HudMainMenu::unlink()
{
    this->player = NULL;
}