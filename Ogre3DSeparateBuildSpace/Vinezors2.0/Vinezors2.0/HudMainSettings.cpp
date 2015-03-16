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
        soundVolumeSliderWarning->setCaption("Certain features will be disabled");
    else
        soundVolumeSliderWarning->setCaption("");
}

void HudMainSettings::alloc()
{
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
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsBackdrop1);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsBackdrop2);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsTitleBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainSettingsTitleText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(controlSettingsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(studySettingsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(studySettingTextDisplay);
    
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
    // Link and set resources
    mainSettingsBackdrop1->setMetricsMode(GMM_RELATIVE);
    mainSettingsBackdrop1->setPosition(0.00, 0.025);
    mainSettingsBackdrop1->setDimensions(1.00, 0.95);
    mainSettingsBackdrop1->setMaterialName("General/TutorialBackdrop");
    
    //mainSettingsBackdrop2->setMetricsMode(GMM_RELATIVE);
    //mainSettingsBackdrop2->setPosition(0.00, 0.625);
    //mainSettingsBackdrop2->setDimensions(1.00, 0.35);
    //mainSettingsBackdrop2->setMaterialName("General/TutorialBackdrop");
    
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
    
    //Numpad Objects
    numpadBackground->setMetricsMode(GMM_RELATIVE);
    numpadBackground->setPosition(0.650, 0.50);
    numpadBackground->setDimensions(0.30, 0.40);
    
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
    password_Choice0->setPosition(0.06, 0.05);
    password_Choice0->setDimensions(0.03, 0.03);
    
    password_Choice1->setMetricsMode(GMM_RELATIVE);
    password_Choice1->setPosition(0.11, 0.05);
    password_Choice1->setDimensions(0.03, 0.03);
    
    password_Choice2->setMetricsMode(GMM_RELATIVE);
    password_Choice2->setPosition(0.16, 0.05);
    password_Choice2->setDimensions(0.03, 0.03);
    
    password_Choice3->setMetricsMode(GMM_RELATIVE);
    password_Choice3->setPosition(0.21, 0.05);
    password_Choice3->setDimensions(0.03, 0.03);
    
    password_Choice0->Ogre::OverlayElement::setMaterialName("General/PasswordDotDisabled");
    password_Choice1->Ogre::OverlayElement::setMaterialName("General/PasswordDotDisabled");
    password_Choice2->Ogre::OverlayElement::setMaterialName("General/PasswordDotDisabled");
    password_Choice3->Ogre::OverlayElement::setMaterialName("General/PasswordDotDisabled");
    
    password_Title->setMetricsMode(GMM_RELATIVE);
    password_Title->setAlignment(TextAreaOverlayElement::Center);
    password_Title->setPosition(0.15, 0.01);
    password_Title->setCharHeight(0.02 * FONT_SZ_MULT);
    password_Title->setFontName("MainSmall");
    password_Title->setCaption("Enter Passcode");

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
    
    backButtonBackground->setMaterialName("General/BackButton");
    controlSettingsButtonBackground->setMaterialName("General/ControlSettingsButton");
    studySettingsButtonBackground->setMaterialName("General/StudySettingsButton");
    numpadBackground->setMaterialName("General/NumpadBackground");
    
    //Position Buttons
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.01, 0.90), Vector2(0.30, 0.08), backButtonBackground, NULL);
    buttons[BUTTON_CONTROL_SETTINGS].setButton("controlsettings", overlays[0], GMM_RELATIVE, Vector2(0.33, 0.90), Vector2(0.30, 0.08), controlSettingsButtonBackground, NULL);
    buttons[BUTTON_STUDY_SETTINGS].setButton("studysettings", overlays[0], GMM_RELATIVE, Vector2(0.65, .90), Vector2(0.30, 0.08), studySettingsButtonBackground, NULL);
    
    buttons[BUTTON_NUMPAD_0].setButton("numpadbutton0", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.305), Vector2(0.06, 0.06), numpadButton_0, NULL);
    
    buttons[BUTTON_NUMPAD_1].setButton("numpadbutton1", overlays[0], GMM_RELATIVE, Vector2(0.05, 0.095), Vector2(0.06, 0.06), numpadButton_1, NULL);
    buttons[BUTTON_NUMPAD_2].setButton("numpadbutton2", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.095), Vector2(0.06, 0.06), numpadButton_2, NULL);
    buttons[BUTTON_NUMPAD_3].setButton("numpadbutton3", overlays[0], GMM_RELATIVE, Vector2(0.19, 0.095), Vector2(0.06, 0.06), numpadButton_3, NULL);
    
    buttons[BUTTON_NUMPAD_4].setButton("numpadbutton4", overlays[0], GMM_RELATIVE, Vector2(0.05, 0.165), Vector2(0.06, 0.06), numpadButton_4, NULL);
    buttons[BUTTON_NUMPAD_5].setButton("numpadbutton5", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.165), Vector2(0.06, 0.06), numpadButton_5, NULL);
    buttons[BUTTON_NUMPAD_6].setButton("numpadbutton6", overlays[0], GMM_RELATIVE, Vector2(0.19, 0.165), Vector2(0.06, 0.06), numpadButton_6, NULL);
    
    buttons[BUTTON_NUMPAD_7].setButton("numpadbutton7", overlays[0], GMM_RELATIVE, Vector2(0.05, 0.235), Vector2(0.06, 0.06), numpadButton_7, NULL);
    buttons[BUTTON_NUMPAD_8].setButton("numpadbutton8", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.235), Vector2(0.06, 0.06), numpadButton_8, NULL);
    buttons[BUTTON_NUMPAD_9].setButton("numpadbutton9", overlays[0], GMM_RELATIVE, Vector2(0.19, 0.235), Vector2(0.06, 0.06), numpadButton_9, NULL);
    
    buttons[BUTTON_NUMPAD_CANCEL].setButton("numpadbuttoncancel", overlays[0], GMM_RELATIVE, Vector2(0.19, 0.305), Vector2(0.06, 0.06), numpadButton_Cancel, NULL);



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