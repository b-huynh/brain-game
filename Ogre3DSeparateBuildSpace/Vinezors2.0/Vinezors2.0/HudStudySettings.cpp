//
//  HudStudySettings.cpp
//  Vinezors2.0
//
//  Created by BrainGameCenter on 3/3/15.
//
//

#include "HudStudySettings.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

HudStudySettings::HudStudySettings(Player* player)
:Hud()
{
    link(player);
    init();
}

HudStudySettings::~HudStudySettings()
{
    dealloc();
    unlink();
}

void HudStudySettings::init()
{
    alloc();
    initOverlay();
    showOverlays();
}

void HudStudySettings::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudStudySettings::update(float elapsed)
{

    initSpeedNumberTextDisplay->setCaption(initSpeedString);
    enableMandatoryRecessNumberTextDisplay->setCaption(manRecessString);
    enableNewNavNumberText->setCaption(newNavigationIncAmountString);
    
    //New Nav Checkbox
    if (player->newNavEnabled)
    {
        enableNewNavButtonBackground->setMaterialName("General/CheckboxGreen");
        enableNewNavTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        enableNewNavText2Display->show();
        enableNewNavNumberButtonBackground->show();
        enableNewNavNumberText->show();
    }
    else
    {
        
        enableNewNavButtonBackground->setMaterialName("General/CheckboxBlank");
        enableNewNavTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        enableNewNavText2Display->hide();
        enableNewNavNumberButtonBackground->hide();
        enableNewNavNumberText->hide();

    }
    
    //Fuel Checkbox
    if (player->fuelEnabled)
    {
        //If fuel is enabled, then unlimited fuel is not enabled
        enableUnlimitedFuelButtonBackground->setMaterialName("General/CheckboxBlank");
        enableUnlimitedFuelTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));

    }
    else
    {
        enableUnlimitedFuelButtonBackground->setMaterialName("General/CheckboxGreen");
        enableUnlimitedFuelTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));

    }
    
    //Holdout Checkbox
    if (player->holdoutEnabled)
    {
   
        enableHoldoutButtonBackground->setMaterialName("General/CheckboxGreen");
        enableHoldoutTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));

        
    }
    else
    {
        enableHoldoutButtonBackground->setMaterialName("General/CheckboxBlank");
        enableHoldoutTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));


    }
    
    //Man Recess Checkbox
    if (player->manRecessEnabled)
    {
        
        enableMandatoryRecessButtonBackground->setMaterialName("General/CheckboxGreen");
        enableMandatoryRecessTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        //enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
        //enableMandatoryRecessNumberTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        enableMandatoryRecessNumberTextDisplay->show();
        enableMandatoryRecessNumberBackground->show();

        
    }
    else
    {
        enableMandatoryRecessButtonBackground->setMaterialName("General/CheckboxBlank");
        enableMandatoryRecessTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        //enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInputGray");
        //enableMandatoryRecessNumberTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        enableMandatoryRecessNumberTextDisplay->hide();
        enableMandatoryRecessNumberBackground->hide();
        
    }
    
    if(enableNumpad)
    {
        if(showDecimal)
        {
            numpadButton_Decimal->show();
            numpadButton_Decimal_TextDisplay->show();

        }
        else
        {
            numpadButton_Decimal->hide();
            numpadButton_Decimal_TextDisplay->hide();
        }
        numpadBackground->show();
        if(nStatus == INIT_VELOCITY)
        {
            initSpeedBackground->setMaterialName("General/SelectedInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");

        }
        else if(nStatus == MAN_RECESS)
        {
            initSpeedBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/SelectedInput");
        }
        else if(nStatus == NEW_NAV_INC)
        {
            initSpeedBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/SelectedInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
        }
    }
    else
    {
        numpadBackground->hide();
        initSpeedBackground->setMaterialName("General/BlankInput");
        enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
        enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");


        
    }

    
}

void HudStudySettings::alloc()
{
    // Allocate Resources
    studySettingsBackdrop1 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsBackdrop1"));
    studySettingsBackdrop2 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsBackdrop2"));
    studySettingsTitleBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsTitleBackground"));
    studySettingsTitleText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsTitleText"));
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsBackButtonBackground"));
    

    //Fuel CheckBox
    enableUnlimitedFuelBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableUnlimitedFuelBackground"));
    enableUnlimitedFuelTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsEnableUnlimitedFuelTextDisplay"));
    enableUnlimitedFuelButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableUnlimitedFuelButtonBackground"));
    
    //Holdout CheckBox
    enableHoldoutBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableHoldoutBackground"));
    enableHoldoutTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsEnableHoldoutTextDisplay"));
    enableHoldoutButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableHoldoutButtonBackground"));
    
    //Init Speed
    initSpeedBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsInitSpeedBackground"));
    initSpeedTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsInitSpeedTextDisplay"));
    initSpeedNumberTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsInitSpeedNumberTextDisplay"));

    
    //Mandatory Recess CheckBox
    enableMandatoryRecessBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableMandatoryRecessBackground"));
    enableMandatoryRecessTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsEnableMandatoryRecessTextDisplay"));
    enableMandatoryRecessButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableMandatoryRecessButtonBackground"));
    enableMandatoryRecessNumberBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableMandatoryRecessNumberBackground"));
    enableMandatoryRecessNumberTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsEnableMandatoryRecessNumberTextDisplay"));
    
    
    //Numpad
    numpadBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadBackground"));
    numpadButton_0 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton0"));
    numpadButton_0_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton0Text"));
    numpadButton_1 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton1"));
    numpadButton_1_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton1Text"));
    numpadButton_2 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton2"));
    numpadButton_2_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton2Text"));
    numpadButton_3 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton3"));
    numpadButton_3_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton3Text"));
    numpadButton_4 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton4"));
    numpadButton_4_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton4Text"));
    numpadButton_5 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton5"));
    numpadButton_5_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton5Text"));
    numpadButton_6 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton6"));
    numpadButton_6_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton6Text"));
    numpadButton_7 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton7"));
    numpadButton_7_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton7Text"));
    numpadButton_8 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton8"));
    numpadButton_8_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton8Text"));
    numpadButton_9 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButton9"));
    numpadButton_9_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButton9Text"));
    numpadButton_Back = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButtonBack"));
    numpadButton_Back_TextDisplay= static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButtonBackText"));
    numpadButton_Save = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButtonSave"));;
    numpadButton_Save_TextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButtonSaveText"));
    numpadButton_Decimal = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsNumpadButtonDecimal"));;
    numpadButton_Decimal_TextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNumpadButtonSDecimalText"));
    
    //New Nav Checkbox
    enableNewNavBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableNewNavBackground"));
    enableNewNavTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNewNavText"));
    enableNewNavButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableNewNavButtonBackground"));
    enableNewNavText2Display = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNewNavText2"));
    enableNewNavNumberButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableNewNavNumberButtonBackground"));
    enableNewNavNumberText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsNewNavNumberText"));
    
    
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("StudySettingsOverlay");
    overlay1->add2D(studySettingsBackdrop1);
    overlay1->add2D(studySettingsBackdrop2);
    overlay1->add2D(studySettingsTitleBackground);
    studySettingsTitleBackground->addChild(studySettingsTitleText);
    
    
#ifdef DEBUG_MODE

#endif
    
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
    numpadButton_Back->addChild(numpadButton_Back_TextDisplay);
    numpadButton_Save->addChild(numpadButton_Save_TextDisplay);
    numpadButton_Decimal->addChild(numpadButton_Decimal_TextDisplay);
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
    numpadBackground->addChild(numpadButton_Back);
    numpadBackground->addChild(numpadButton_Save);
    numpadBackground->addChild(numpadButton_Decimal);

        

    
    overlay1->add2D(enableUnlimitedFuelBackground);
    enableUnlimitedFuelBackground->addChild(enableUnlimitedFuelTextDisplay);
    overlay1->add2D(enableUnlimitedFuelButtonBackground);
    
    //New Nav Checkbox
    overlay1->add2D(enableNewNavBackground);
    enableNewNavBackground->addChild(enableNewNavTextDisplay);
    enableNewNavBackground->addChild(enableNewNavText2Display);
    //enableNewNavBackground->addChild(enableNewNavNumberButtonBackground);
    overlay1->add2D(enableNewNavNumberButtonBackground);
    enableNewNavNumberButtonBackground->addChild(enableNewNavNumberText);
    overlay1->add2D(enableNewNavButtonBackground);
    
    
    
    
    overlay1->add2D(enableMandatoryRecessBackground);
    enableMandatoryRecessBackground->addChild(enableMandatoryRecessTextDisplay);
    overlay1->add2D(enableMandatoryRecessButtonBackground);
    
    overlay1->add2D(enableHoldoutBackground);
    enableHoldoutBackground->addChild(enableHoldoutTextDisplay);
    overlay1->add2D(enableHoldoutButtonBackground);
    
    initSpeedBackground->addChild(initSpeedTextDisplay);
    initSpeedBackground->addChild(initSpeedNumberTextDisplay);
    overlay1->add2D(initSpeedBackground);
    
    overlay1->add2D(enableMandatoryRecessNumberBackground);
    enableMandatoryRecessNumberBackground->addChild(enableMandatoryRecessNumberTextDisplay);
    
    overlay1->add2D(backButtonBackground);
    overlay1->add2D(numpadBackground);

    overlays.push_back(overlay1);
    

}

void HudStudySettings::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(studySettingsBackdrop1);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(studySettingsBackdrop2);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(studySettingsTitleBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(studySettingsTitleText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);

    
    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableUnlimitedFuelBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableUnlimitedFuelTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableUnlimitedFuelButtonBackground);
    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableHoldoutBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableHoldoutTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableHoldoutButtonBackground);
    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(initSpeedBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(initSpeedTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(initSpeedNumberTextDisplay);
    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableMandatoryRecessBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableMandatoryRecessTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableMandatoryRecessButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableMandatoryRecessNumberBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableMandatoryRecessNumberTextDisplay);

    //Numpad:
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
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_Back);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_Back_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_Save);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_Save_TextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_Decimal);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(numpadButton_Decimal_TextDisplay);
    
    //New Nav
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableNewNavBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableNewNavTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableNewNavButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableNewNavText2Display);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableNewNavNumberButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableNewNavNumberText);


    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
    

}

void HudStudySettings::initOverlay()
{

    //New Nav
    enableNewNavBackground->setMetricsMode(GMM_RELATIVE);
    enableNewNavBackground->setPosition(0.20, 0.550);
    enableNewNavBackground->setDimensions(0.60, 0.10);
    
    enableNewNavTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableNewNavTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableNewNavTextDisplay->setPosition(0.0, 0.05);
    enableNewNavTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    enableNewNavTextDisplay->setFontName("MainSmall");
    enableNewNavTextDisplay->setCaption("Enable Fixed Navigation");
    enableNewNavTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
    
    enableNewNavButtonBackground->setMaterialName("General/CheckboxBlank");
    
    enableNewNavText2Display->setMetricsMode(GMM_RELATIVE);
    enableNewNavText2Display->setAlignment(TextAreaOverlayElement::Left);
    enableNewNavText2Display->setPosition(0.0, 0.1);
    enableNewNavText2Display->setCharHeight(0.030 * FONT_SZ_MULT);
    enableNewNavText2Display->setFontName("MainSmall");
    enableNewNavText2Display->setCaption("Increment Value:");
    
    enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
    
    enableNewNavNumberText->setMetricsMode(GMM_RELATIVE);
    enableNewNavNumberText->setAlignment(TextAreaOverlayElement::Left);
    enableNewNavNumberText->setPosition(0.01, 0.0);
    enableNewNavNumberText->setCharHeight(0.030 * FONT_SZ_MULT);
    enableNewNavNumberText->setFontName("MainSmall");
    newNavigationIncAmountString = Util::toStringFloat(player->newNavIncrement,2);
    enableNewNavNumberText->setCaption(Util::toStringFloat(player->newNavIncrement,2));
    
    //End New Nav
    
    
    
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
    numpadButton_Back->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");
    numpadButton_Save->Ogre::OverlayElement::setMaterialName("General/NumpadSaveButton");
    numpadButton_Decimal->Ogre::OverlayElement::setMaterialName("General/CheckboxBlank");

    numpadButton_Save_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_Save_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_Save_TextDisplay->setPosition(0.060, 0.008);
    numpadButton_Save_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_Save_TextDisplay->setFontName("MainSmall");
    numpadButton_Save_TextDisplay->setCaption("SAVE");
    numpadButton_Save_TextDisplay->setColour(Ogre::ColourValue(0,1,0));
    
    numpadButton_Back_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_Back_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_Back_TextDisplay->setPosition(0.030, 0.008);
    numpadButton_Back_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_Back_TextDisplay->setFontName("MainSmall");
    numpadButton_Back_TextDisplay->setCaption("<-");
    
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
    
    numpadButton_Decimal_TextDisplay->setMetricsMode(GMM_RELATIVE);
    numpadButton_Decimal_TextDisplay->setAlignment(TextAreaOverlayElement::Center);
    numpadButton_Decimal_TextDisplay->setPosition(0.035, 0.01);
    numpadButton_Decimal_TextDisplay->setCharHeight(0.03 * FONT_SZ_MULT);
    numpadButton_Decimal_TextDisplay->setFontName("MainSmall");
    numpadButton_Decimal_TextDisplay->setCaption(".");
    
    numpadBackground->setMaterialName("General/TutorialBackdrop");
    //End Numpad
    
    
    enableHoldoutButtonBackground->setMaterialName("General/CheckboxBlank");
    enableUnlimitedFuelButtonBackground->setMaterialName("General/CheckboxBlank");
    enableMandatoryRecessButtonBackground->setMaterialName("General/CheckboxBlank");
    initSpeedBackground->setMaterialName("General/BlankInput");
    enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
    
    

    // Link and set resources
    studySettingsBackdrop1->setMetricsMode(GMM_RELATIVE);
    studySettingsBackdrop1->setPosition(0.00, 0.025);
    studySettingsBackdrop1->setDimensions(1.00, 0.95);
    studySettingsBackdrop1->setMaterialName("General/TutorialBackdrop");
    
    studySettingsTitleBackground->setMetricsMode(GMM_RELATIVE);
    studySettingsTitleBackground->setPosition(0.30, 0.025);
    studySettingsTitleBackground->setDimensions(0.40, 0.20);
    
    studySettingsTitleText->setMetricsMode(GMM_RELATIVE);
    studySettingsTitleText->setAlignment(TextAreaOverlayElement::Center);
    studySettingsTitleText->setPosition(0.20, 0.00);
    studySettingsTitleText->setCharHeight(0.046 * FONT_SZ_MULT);
    studySettingsTitleText->setFontName("MainSmall");
    studySettingsTitleText->setCaption("Study Settings");
    
    enableUnlimitedFuelBackground->setMetricsMode(GMM_RELATIVE);
    enableUnlimitedFuelBackground->setPosition(0.20, 0.150);
    enableUnlimitedFuelBackground->setDimensions(0.60, 0.10);
    
    enableUnlimitedFuelTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableUnlimitedFuelTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableUnlimitedFuelTextDisplay->setPosition(0.0, 0.05);
    enableUnlimitedFuelTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    enableUnlimitedFuelTextDisplay->setFontName("MainSmall");
    enableUnlimitedFuelTextDisplay->setCaption("Enable Unlimited Fuel");
    
    
    enableHoldoutBackground->setMetricsMode(GMM_RELATIVE);
    enableHoldoutBackground->setPosition(0.20, 0.250);
    enableHoldoutBackground->setDimensions(0.60, 0.10);
    
    enableHoldoutTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableHoldoutTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableHoldoutTextDisplay->setPosition(0.0, 0.05);
    enableHoldoutTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    enableHoldoutTextDisplay->setFontName("MainSmall");
    enableHoldoutTextDisplay->setCaption("Enable Holdout");
    
    
    
    enableMandatoryRecessBackground->setMetricsMode(GMM_RELATIVE);
    enableMandatoryRecessBackground->setPosition(0.20, 0.350);
    enableMandatoryRecessBackground->setDimensions(0.60, 0.10);
    
    enableMandatoryRecessTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableMandatoryRecessTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableMandatoryRecessTextDisplay->setPosition(0.0, 0.05);
    enableMandatoryRecessTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    enableMandatoryRecessTextDisplay->setFontName("MainSmall");
    enableMandatoryRecessTextDisplay->setCaption("Mandatory Recess");
    enableMandatoryRecessTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
    

    
    initSpeedTextDisplay->setMetricsMode(GMM_RELATIVE);
    initSpeedTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    initSpeedTextDisplay->setPosition(-0.3, 0.00);
    initSpeedTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    initSpeedTextDisplay->setFontName("MainSmall");
    initSpeedTextDisplay->setCaption("Initial Velocity");
    
    initSpeedNumberTextDisplay->setMetricsMode(GMM_RELATIVE);
    initSpeedNumberTextDisplay->setAlignment(TextAreaOverlayElement::Center);
    initSpeedNumberTextDisplay->setPosition(0.04, 0.005);
    initSpeedNumberTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    initSpeedNumberTextDisplay->setFontName("MainSmall");
    //initSpeedNumberTextDisplay->setCaption("0");
    initSpeedString = Util::toStringInt(player->initialVelocity);
    initSpeedNumberTextDisplay->setCaption(Util::toStringInt(player->initialVelocity));

    
    
    enableMandatoryRecessNumberTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableMandatoryRecessNumberTextDisplay->setAlignment(TextAreaOverlayElement::Center);
    enableMandatoryRecessNumberTextDisplay->setPosition(0.025, 0.005);
    enableMandatoryRecessNumberTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    enableMandatoryRecessNumberTextDisplay->setFontName("MainSmall");
    manRecessString = Util::toStringInt(player->manRecessLevelLimit);
    enableMandatoryRecessNumberTextDisplay->setCaption(Util::toStringInt(player->manRecessLevelLimit));
    //enableMandatoryRecessNumberTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
    
    
    backButtonBackground->setMaterialName("General/BackButton");
    
    //Position Buttons
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.01, 0.90), Vector2(0.30, 0.08), backButtonBackground, NULL);
    
    
    // The Enable Tutorial Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_UNLIMITED_FUEL].setButton("checkfuel", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.20), Vector2(pw, ph), enableUnlimitedFuelButtonBackground, NULL);
    }
    
    // The Enable Holdout Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_HOLDOUT].setButton("checkholdout", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.30), Vector2(pw, ph), enableHoldoutButtonBackground, NULL);
    }
    
    // The Enable InitSpeed
    {
        // calculate dimensions for button size and make sure it's square
        buttons[BUTTON_INPUT_INIT_SPEED].setButton("checkinitspeed", overlays[0], GMM_RELATIVE, Vector2(0.43, 0.50), Vector2(.08, .06), initSpeedBackground, NULL);
    }
    
    // The Enable Tutorial Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_MANDATORY_RECESS].setButton("checkmandatoryrecess", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.40), Vector2(pw, ph), enableMandatoryRecessButtonBackground, NULL);
    }
    
    // Mandatory Recess Input
    {
        
        buttons[BUTTON_MANDATORY_RECESS_NUMBER].setButton("mandatoryrecessnumber", overlays[0], GMM_RELATIVE, Vector2(0.6, 0.40), Vector2(.05, .06), enableMandatoryRecessNumberBackground, NULL);
    }
    
    
    buttons[BUTTON_NUMPAD_0].setButton("numpadbutton0", overlays[0], GMM_RELATIVE, Vector2(0.155, 0.305), Vector2(0.06, 0.06), numpadButton_0, NULL);
    
    buttons[BUTTON_NUMPAD_1].setButton("numpadbutton1", overlays[0], GMM_RELATIVE, Vector2(0.085, 0.095), Vector2(0.06, 0.06), numpadButton_1, NULL);
    buttons[BUTTON_NUMPAD_2].setButton("numpadbutton2", overlays[0], GMM_RELATIVE, Vector2(0.155, 0.095), Vector2(0.06, 0.06), numpadButton_2, NULL);
    buttons[BUTTON_NUMPAD_3].setButton("numpadbutton3", overlays[0], GMM_RELATIVE, Vector2(0.225, 0.095), Vector2(0.06, 0.06), numpadButton_3, NULL);
    
    buttons[BUTTON_NUMPAD_4].setButton("numpadbutton4", overlays[0], GMM_RELATIVE, Vector2(0.085, 0.165), Vector2(0.06, 0.06), numpadButton_4, NULL);
    buttons[BUTTON_NUMPAD_5].setButton("numpadbutton5", overlays[0], GMM_RELATIVE, Vector2(0.155, 0.165), Vector2(0.06, 0.06), numpadButton_5, NULL);
    buttons[BUTTON_NUMPAD_6].setButton("numpadbutton6", overlays[0], GMM_RELATIVE, Vector2(0.225, 0.165), Vector2(0.06, 0.06), numpadButton_6, NULL);
    
    buttons[BUTTON_NUMPAD_7].setButton("numpadbutton7", overlays[0], GMM_RELATIVE, Vector2(0.085, 0.235), Vector2(0.06, 0.06), numpadButton_7, NULL);
    buttons[BUTTON_NUMPAD_8].setButton("numpadbutton8", overlays[0], GMM_RELATIVE, Vector2(0.155, 0.235), Vector2(0.06, 0.06), numpadButton_8, NULL);
    buttons[BUTTON_NUMPAD_9].setButton("numpadbutton9", overlays[0], GMM_RELATIVE, Vector2(0.225, 0.235), Vector2(0.06, 0.06), numpadButton_9, NULL);
    
    buttons[BUTTON_NUMPAD_DECIMAL].setButton("numpadbuttondecimal", overlays[0], GMM_RELATIVE, Vector2(0.085, 0.305), Vector2(0.06, 0.06), numpadButton_Decimal, NULL);
    
    buttons[BUTTON_NUMPAD_BACK].setButton("numpadbuttonback", overlays[0], GMM_RELATIVE, Vector2(0.225, 0.305), Vector2(0.06, 0.06), numpadButton_Back, NULL);
    
    buttons[BUTTON_NUMPAD_SAVE].setButton("numpadbuttonsave", overlays[0], GMM_RELATIVE, Vector2(0.165, 0.375), Vector2(0.12, 0.06), numpadButton_Save, NULL);
    
    buttons[NUMPAD_BG].setButton("numpad", overlays[0], GMM_RELATIVE, Vector2(0.65, 0.40), Vector2(0.5, 0.5), numpadBackground, NULL);
    
    // The Enable NewNav Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_NEW_NAV].setButton("checknewnav", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.6), Vector2(pw, ph), enableNewNavButtonBackground, NULL);
    }
    // The Enable NewNavNumber Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_NEW_NAV_NUMBER].setButton("checknewnavnumber", overlays[0], GMM_RELATIVE, Vector2(0.52, 0.65), Vector2(.10, pw), enableNewNavNumberButtonBackground, NULL);
    }
    
    
    
    
    //This is for no flash(change) in hud
    if (player->newNavEnabled)
    {
        enableNewNavButtonBackground->setMaterialName("General/CheckboxGreen");
        enableNewNavTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        enableNewNavText2Display->show();
        enableNewNavNumberButtonBackground->show();
        enableNewNavNumberText->show();
        
    }
    else
    {
        
        
        enableNewNavButtonBackground->setMaterialName("General/CheckboxBlank");
        enableNewNavTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        enableNewNavText2Display->hide();
        enableNewNavNumberButtonBackground->hide();
        enableNewNavNumberText->hide();

        
    }

    
    if (player->fuelEnabled)
    {
        //If fuel is enabled, then unlimited fuel is not enabled
        enableUnlimitedFuelButtonBackground->setMaterialName("General/CheckboxBlank");
        enableUnlimitedFuelTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        
    }
    else
    {
        enableUnlimitedFuelButtonBackground->setMaterialName("General/CheckboxGreen");
        enableUnlimitedFuelTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        
    }
    
    //Holdout Checkbox
    if (player->holdoutEnabled)
    {
        
        enableHoldoutButtonBackground->setMaterialName("General/CheckboxGreen");
        enableHoldoutTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        
        
    }
    else
    {
        enableHoldoutButtonBackground->setMaterialName("General/CheckboxBlank");
        enableHoldoutTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        
        
    }
    
    //Man Recess Checkbox
    if (player->manRecessEnabled)
    {
        
        enableMandatoryRecessButtonBackground->setMaterialName("General/CheckboxGreen");
        enableMandatoryRecessTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        //enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
        //enableMandatoryRecessNumberTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        enableMandatoryRecessNumberTextDisplay->show();
        enableMandatoryRecessNumberBackground->show();
        
        
    }
    else
    {
        enableMandatoryRecessButtonBackground->setMaterialName("General/CheckboxBlank");
        enableMandatoryRecessTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        //enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInputGray");
        //enableMandatoryRecessNumberTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        enableMandatoryRecessNumberTextDisplay->hide();
        enableMandatoryRecessNumberBackground->hide();
        
    }

    numpadBackground->hide();

    
}

void HudStudySettings::positionSliderBalls()
{

}

void HudStudySettings::link(Player* player)
{
    this->player = player;
}

void HudStudySettings::unlink()
{
    this->player = NULL;
}