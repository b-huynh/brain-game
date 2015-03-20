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
    initStrings();
    showOverlays();
}

void HudStudySettings::initStrings()
{
    holdoutDelayString = Util::toStringFloat(player->holdoutdelayNumber,1);
    manRecessString = Util::toStringInt(player->manRecessLevelLimit);
    newNavigationIncAmountString = Util::toStringFloat(player->newNavIncrement,2);
    indRecessString = Util::toStringFloat(player->indRecessIncrement,2);
    holdoutDelayString = Util::toStringFloat(player->holdoutdelayNumber,1);
    initSpeedString = Util::toStringInt(player->initialVelocity);
    sessionStartTimeString = Util::toStringInt(player->sessionStartTime);
    sessionEndtimeString= Util::toStringInt(player->sessionEndTime);
    numOfSessionsString = Util::toStringInt(player->numOfSessions);

    
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
    enableIndRecessNumberText->setCaption(indRecessString);
    enableHoldoutDelayNumberTextDisplay->setCaption(holdoutDelayString);
    sessionStartTimeNumberTextDisplay->setCaption(sessionStartTimeString);
    sessionEndTimeNumberTextDisplay->setCaption(sessionEndtimeString);
    sessionNumNumberTextDisplay->setCaption(numOfSessionsString);
    

    
    if(player->sessionScreenEnabled)
    {
        enableSessionScreenButtonBackground->setMaterialName("General/CheckboxGreen");
        enableSessionScreenTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
    }
    else
    {
        enableSessionScreenButtonBackground->setMaterialName("General/CheckboxBlank");
        enableSessionScreenTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));

    }
    
    if(player->enableSettingsPasscode)
    {
        enableSettingsPasscodeTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        enableSettingsPasscodeButtonBackground->setMaterialName("General/CheckboxGreen");
    }
    else
    {
        enableSettingsPasscodeTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        enableSettingsPasscodeButtonBackground->setMaterialName("General/CheckboxBlank");
    }
    //Ind Recess Checkbox
    if (player->indRecessEnabled)
    {
        enableIndRecessButtonBackground->setMaterialName("General/CheckboxGreen");
        enableIndRecessTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        enableIndRecessText2Display->show();
        enableIndRecessNumberButtonBackground->show();
        enableIndRecessNumberText->show();
        enableIndRecessFixedButtonBackground->show();
        enableIndRecessFixedTextDisplay->show();
        if(player->enableIndRecessFixed)
        {
            enableIndRecessFixedButtonBackground->setMaterialName("General/CheckboxGreen");
            enableIndRecessFixedTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        }
        else
        {
            enableIndRecessFixedButtonBackground->setMaterialName("General/CheckboxBlank");
            enableIndRecessFixedTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
            
        }
    }
    else
    {
        
        enableIndRecessButtonBackground->setMaterialName("General/CheckboxBlank");
        enableIndRecessTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        enableIndRecessText2Display->hide();
        enableIndRecessNumberButtonBackground->hide();
        enableIndRecessNumberText->hide();
        enableIndRecessFixedButtonBackground->hide();
        enableIndRecessFixedTextDisplay->hide();

        
    }
    
    //NewSounds Checkbox
    if (globals.newSounds)
    {
        
        enableNewSoundsButtonBackground->setMaterialName("General/CheckboxGreen");
        enableNewSoundsTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
    }
    else
    {
        
        
        enableNewSoundsButtonBackground->setMaterialName("General/CheckboxBlank");
        enableNewSoundsTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        
    }
    
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
        enableHoldoutDelayButtonBackground->show();
        enableHoldoutDelayTextDisplay->show();
        //HoldoutDelayCheckBox
        if (player->holdoutdelayEnabled)
        {
            enableHoldoutDelayButtonBackground->setMaterialName("General/CheckboxGreen");
            enableHoldoutDelayTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
            enableHoldoutDelayNumberButtonBackground->show();
            enableHoldoutDelayNumberTextDisplay->show();
        }
        else
        {
            
            enableHoldoutDelayButtonBackground->setMaterialName("General/CheckboxBlank");
            enableHoldoutDelayTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
            enableHoldoutDelayNumberButtonBackground->hide();
            enableHoldoutDelayNumberTextDisplay->hide();
            
        }

        
    }
    else
    {
        enableHoldoutButtonBackground->setMaterialName("General/CheckboxBlank");
        enableHoldoutTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        
        //Delay:
        enableHoldoutDelayButtonBackground->hide();
        enableHoldoutDelayTextDisplay->hide();
        enableHoldoutDelayNumberButtonBackground->hide();
        enableHoldoutDelayNumberTextDisplay->hide();


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
            enableIndRecessNumberButtonBackground->setMaterialName("General/BlankInput");
            enableHoldoutDelayNumberButtonBackground->setMaterialName("General/BlankInput");
            sessionStartTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionEndTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionNumNumberBackground->setMaterialName("General/BlankInput");


        }
        else if(nStatus == MAN_RECESS)
        {
            initSpeedBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/SelectedInput");
            enableIndRecessNumberButtonBackground->setMaterialName("General/BlankInput");
            enableHoldoutDelayNumberButtonBackground->setMaterialName("General/BlankInput");
            sessionStartTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionEndTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionNumNumberBackground->setMaterialName("General/BlankInput");




        }
        else if(nStatus == NEW_NAV_INC)
        {
            initSpeedBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/SelectedInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
            enableIndRecessNumberButtonBackground->setMaterialName("General/BlankInput");
            enableHoldoutDelayNumberButtonBackground->setMaterialName("General/BlankInput");
            sessionStartTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionEndTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionNumNumberBackground->setMaterialName("General/BlankInput");




        }
        else if(nStatus == IND_RECESS_INC)
        {
            initSpeedBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
            enableIndRecessNumberButtonBackground->setMaterialName("General/SelectedInput");
            enableHoldoutDelayNumberButtonBackground->setMaterialName("General/BlankInput");
            sessionStartTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionEndTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionNumNumberBackground->setMaterialName("General/BlankInput");


            
        }
        else if(nStatus == HOLDOUT_DELAY)
        {
            initSpeedBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
            enableIndRecessNumberButtonBackground->setMaterialName("General/BlankInput");
            enableHoldoutDelayNumberButtonBackground->setMaterialName("General/SelectedInput");
            sessionStartTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionEndTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionNumNumberBackground->setMaterialName("General/BlankInput");

            
            
        }
        else if(nStatus == SESSION_START_TIME)
        {
            initSpeedBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
            enableIndRecessNumberButtonBackground->setMaterialName("General/BlankInput");
            enableHoldoutDelayNumberButtonBackground->setMaterialName("General/BlankInput");
            sessionStartTimeNumberBackground->setMaterialName("General/SelectedInput");
            sessionEndTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionNumNumberBackground->setMaterialName("General/BlankInput");

            
            
        }
        else if(nStatus == SESSION_END_TIME)
        {
            initSpeedBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
            enableIndRecessNumberButtonBackground->setMaterialName("General/BlankInput");
            enableHoldoutDelayNumberButtonBackground->setMaterialName("General/BlankInput");
            sessionStartTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionEndTimeNumberBackground->setMaterialName("General/SelectedInput");
            sessionNumNumberBackground->setMaterialName("General/BlankInput");

            
            
            
        }
        else if(nStatus == NUM_OF_SESSIONS)
        {
            initSpeedBackground->setMaterialName("General/BlankInput");
            enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
            enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
            enableIndRecessNumberButtonBackground->setMaterialName("General/BlankInput");
            enableHoldoutDelayNumberButtonBackground->setMaterialName("General/BlankInput");
            sessionStartTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionEndTimeNumberBackground->setMaterialName("General/BlankInput");
            sessionNumNumberBackground->setMaterialName("General/SelectedInput");
            
            
            
            
        }
    }
    else
    {
        numpadBackground->hide();
        initSpeedBackground->setMaterialName("General/BlankInput");
        enableMandatoryRecessNumberBackground->setMaterialName("General/BlankInput");
        enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
        enableIndRecessNumberButtonBackground->setMaterialName("General/BlankInput");
        enableHoldoutDelayNumberButtonBackground->setMaterialName("General/BlankInput");
        sessionStartTimeNumberBackground->setMaterialName("General/BlankInput");
        sessionEndTimeNumberBackground->setMaterialName("General/BlankInput");
        sessionNumNumberBackground->setMaterialName("General/BlankInput");


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
    
    //New Sounds CheckBox
    enableNewSoundsBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableNewSoundsBackground"));
    enableNewSoundsTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsEnableNewSoundsTextDisplay"));
    enableNewSoundsButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableNewSoundsButtonBackground"));
    
    //IndRecess Checkbox
    enableIndRecessBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableIndRecessBackground"));
    enableIndRecessTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsIndRecessText"));
    enableIndRecessButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableIndRecessButtonBackground"));
    enableIndRecessText2Display = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsIndRecessText2"));
    enableIndRecessNumberButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableIndRecessNumberButtonBackground"));
    enableIndRecessNumberText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsIndRecessNumberText"));
    
    //Holdout Delay Stuff
    enableHoldoutDelayBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableHoldoutDelayBackground"));
    enableHoldoutDelayTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsHoldoutDelayText"));
    enableHoldoutDelayButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsHoldoutDelayButtonBackground"));
    enableHoldoutDelayNumberTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsHoldoutDelayNumberText"));
    enableHoldoutDelayNumberButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableHoldoutDelayNumberBackground"));

    //Passcode Checkbox
    enableSettingsPasscodeBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableSettingsPasscodeBackground"));
    enableSettingsPasscodeTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsEnableSettingsPasscodeText"));
    enableSettingsPasscodeButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsEnableSettingsPasscodeButtonBackground"));
    
    //Session StartTime
    sessionTimeSettingsBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsSesstionTimeSettingsBackground"));
    sessionStartTimeTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingssessionStartTimeText"));
    sessionStartTimeNumberBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsessionStartTimeNumberBackground"));
    sessionStartTimeNumberTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingssessionStartTimeNumberTextDisplay"));
    
    //Session End Time
    sessionEndTimeTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingssessionStartEndTimeTextDisplay"));
    sessionEndTimeNumberBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingssessionEndTimeNumberBackground"));
    sessionEndTimeNumberTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingssessionEndTimeNumberTextDisplay"));
    
    //Session Num Time
    sessionNumTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingssessionNumTextDisplay"));
    sessionNumNumberBackground  = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingssessionNumNumberBackground"));
    sessionNumNumberTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingssessionNumNumberTextDisplay"));
    
    //SessionID Screen Toggle
    enableSessionScreenBackground  = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsenableSessionScreenBackground"));
    enableSessionScreenTextDisplay   = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsenableSessionScreenTextDisplay"));
    enableSessionScreenButtonBackground  = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsenableSessionScreenButtonBackground"));
    
    //Ind Recess Fixed
    enableIndRecessFixedTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsenableIndRecessFixedTextDisplay"));
    enableIndRecessFixedButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsenableIndRecessFixedButtonBackground"));

    //Vendor ID
    vendorIDTextDisplay = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StudySettingsvendorIDTextDisplay"));
    vendorIDBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StudySettingsvendorIDBackground"));
    
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("StudySettingsOverlay");
    overlay1->add2D(studySettingsBackdrop1);
    overlay1->add2D(studySettingsBackdrop2);
    overlay1->add2D(studySettingsTitleBackground);
    studySettingsTitleBackground->addChild(studySettingsTitleText);


    
#ifdef DEBUG_MODE

#endif
    //overlay1->add2D();

    overlay1->add2D(enableIndRecessFixedButtonBackground);
    enableIndRecessFixedButtonBackground->addChild(enableIndRecessFixedTextDisplay);
    
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

        
    overlay1->add2D(vendorIDBackground);
    vendorIDBackground->addChild(vendorIDTextDisplay);
    
    overlay1->add2D(enableUnlimitedFuelBackground);
    enableUnlimitedFuelBackground->addChild(enableUnlimitedFuelTextDisplay);
    overlay1->add2D(enableUnlimitedFuelButtonBackground);
    
    //Enable Session Screen
    
    overlay1->add2D(enableSessionScreenBackground);
    enableSessionScreenBackground->addChild(enableSessionScreenTextDisplay);
    overlay1->add2D(enableSessionScreenButtonBackground);
    
    //Session Start Time and End Time and Num Sessions
    //Session Num Time
    
    
    overlay1->add2D(sessionTimeSettingsBackground);
    sessionTimeSettingsBackground->addChild(sessionStartTimeTextDisplay);
    sessionTimeSettingsBackground->addChild(sessionEndTimeTextDisplay);
    sessionTimeSettingsBackground->addChild(sessionNumTextDisplay);

    
    overlay1->add2D(sessionStartTimeNumberBackground);
    overlay1->add2D(sessionEndTimeNumberBackground);
    overlay1->add2D(sessionNumNumberBackground);


    sessionStartTimeNumberBackground->addChild(sessionStartTimeNumberTextDisplay);
    sessionEndTimeNumberBackground->addChild(sessionEndTimeNumberTextDisplay);
    sessionNumNumberBackground->addChild(sessionNumNumberTextDisplay);


    
    //Passcode Checkbox
    overlay1->add2D(enableSettingsPasscodeBackground);
    enableSettingsPasscodeBackground->addChild(enableSettingsPasscodeTextDisplay);
    overlay1->add2D(enableSettingsPasscodeButtonBackground);
    
    //IndRecess Checkbox
    overlay1->add2D(enableIndRecessBackground);
    
    //overlay1->add2D(enableIndRecessFixedBackground);
 

    
    enableIndRecessBackground->addChild(enableIndRecessTextDisplay);
    enableIndRecessBackground->addChild(enableIndRecessText2Display);
    overlay1->add2D(enableIndRecessNumberButtonBackground);
    enableIndRecessNumberButtonBackground->addChild(enableIndRecessNumberText);
    overlay1->add2D(enableIndRecessButtonBackground);
    
    //Holdout Delay Stuff
    overlay1->add2D(enableHoldoutDelayBackground);
    enableHoldoutDelayBackground->addChild(enableHoldoutDelayTextDisplay);
    overlay1->add2D(enableHoldoutDelayButtonBackground);
    enableHoldoutDelayButtonBackground->addChild(enableHoldoutDelayNumberTextDisplay);
    overlay1->add2D(enableHoldoutDelayNumberButtonBackground);
    
    //New Nav Checkbox
    overlay1->add2D(enableNewNavBackground);
    enableNewNavBackground->addChild(enableNewNavTextDisplay);
    enableNewNavBackground->addChild(enableNewNavText2Display);
    overlay1->add2D(enableNewNavNumberButtonBackground);
    enableNewNavNumberButtonBackground->addChild(enableNewNavNumberText);
    overlay1->add2D(enableNewNavButtonBackground);
    
    //New Sounds Checkbox
    overlay1->add2D(enableNewSoundsBackground);
    enableNewSoundsBackground->addChild(enableNewSoundsTextDisplay);
    overlay1->add2D(enableNewSoundsButtonBackground);
    
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


    //New Sounds
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableNewSoundsBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableNewSoundsTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableNewSoundsButtonBackground);
    
    //New Nav
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableIndRecessBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableIndRecessTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableIndRecessButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableIndRecessText2Display);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableIndRecessNumberButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableIndRecessNumberText);
    
    
    //Holdout Delay Stuff
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableHoldoutDelayBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableHoldoutDelayTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableHoldoutDelayButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableHoldoutDelayNumberTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableHoldoutDelayNumberButtonBackground);



    //Passcode Checkbox
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableSettingsPasscodeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableSettingsPasscodeTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableSettingsPasscodeButtonBackground);
    
    //Session StartTime
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionTimeSettingsBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionStartTimeTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionStartTimeNumberBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionStartTimeNumberTextDisplay);

    //Session End Time
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionEndTimeTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionEndTimeNumberBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionEndTimeNumberTextDisplay);

    //Session Num Time
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionNumTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionNumNumberBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sessionNumNumberTextDisplay);


    //SessionID Screen Toggle
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableSessionScreenBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableSessionScreenTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableSessionScreenButtonBackground);
    
    //Ind REcess Fixed
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableIndRecessFixedTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableIndRecessFixedButtonBackground);
    //OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableIndRecessFixedBackground);


    //Vendor ID
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(vendorIDTextDisplay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(vendorIDBackground);

    

    
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
    

}

void HudStudySettings::initOverlay()
{
    //Ind Recess Fixed
    vendorIDBackground->setMetricsMode(GMM_RELATIVE);
    vendorIDBackground->setPosition(0.0, 0.0);
    vendorIDBackground->setDimensions(0.60, 0.10);
    
    vendorIDTextDisplay->setMetricsMode(GMM_RELATIVE);
    vendorIDTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    vendorIDTextDisplay->setPosition(0.35, 0.96);
    vendorIDTextDisplay->setCharHeight(0.015 * FONT_SZ_MULT);
    vendorIDTextDisplay->setFontName("MainSmall");
    vendorIDTextDisplay->setCaption("Vendor ID: " + globals.VendorID);
    
    enableIndRecessFixedTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableIndRecessFixedTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableIndRecessFixedTextDisplay->setPosition(0.07, 0.0);
    enableIndRecessFixedTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    enableIndRecessFixedTextDisplay->setFontName("MainSmall");
    enableIndRecessFixedTextDisplay->setCaption("Fixed");
    
    enableIndRecessFixedButtonBackground->setMaterialName("General/CheckboxBlank");
    
    //SessionID Screen Toggle
    //Session StartTime
    enableSessionScreenBackground->setMetricsMode(GMM_RELATIVE);
    enableSessionScreenBackground->setPosition(0.62, 0.37);
    enableSessionScreenBackground->setDimensions(0.60, 0.10);
    
    enableSessionScreenTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableSessionScreenTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableSessionScreenTextDisplay->setPosition(0.02, 0.00);
    enableSessionScreenTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    enableSessionScreenTextDisplay->setFontName("MainSmall");
    enableSessionScreenTextDisplay->setCaption("Enable SessionID");
    
    //enableUnlimitedFuelButtonBackground->setMaterialName("General/CheckboxBlank");

    //OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(enableSessionScreenButtonBackground);

    //Session Num Sessions
    sessionNumTextDisplay->setMetricsMode(GMM_RELATIVE);
    sessionNumTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    sessionNumTextDisplay->setPosition(0.46, 0.1);
    sessionNumTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    sessionNumTextDisplay->setFontName("MainSmall");
    sessionNumTextDisplay->setCaption("#: ");
    
    sessionNumNumberBackground->setMaterialName("General/BlankInput");
    
    sessionNumNumberTextDisplay->setMetricsMode(GMM_RELATIVE);
    sessionNumNumberTextDisplay->setAlignment(TextAreaOverlayElement::Center);
    sessionNumNumberTextDisplay->setPosition(0.04, 0.0);
    sessionNumNumberTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    sessionNumNumberTextDisplay->setFontName("MainSmall");

    //Session EndTime
    sessionEndTimeTextDisplay->setMetricsMode(GMM_RELATIVE);
    sessionEndTimeTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    sessionEndTimeTextDisplay->setPosition(0.27, 0.1);
    sessionEndTimeTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    sessionEndTimeTextDisplay->setFontName("MainSmall");
    sessionEndTimeTextDisplay->setCaption("Max: ");
    
    sessionEndTimeNumberBackground->setMaterialName("General/BlankInput");
    
    sessionEndTimeNumberTextDisplay->setMetricsMode(GMM_RELATIVE);
    sessionEndTimeNumberTextDisplay->setAlignment(TextAreaOverlayElement::Center);
    sessionEndTimeNumberTextDisplay->setPosition(0.04, 0.0);
    sessionEndTimeNumberTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    sessionEndTimeNumberTextDisplay->setFontName("MainSmall");

    
    //Session StartTime
    sessionTimeSettingsBackground->setMetricsMode(GMM_RELATIVE);
    sessionTimeSettingsBackground->setPosition(0.055, 0.72);
    sessionTimeSettingsBackground->setDimensions(0.60, 0.10);
    
    sessionStartTimeTextDisplay->setMetricsMode(GMM_RELATIVE);
    sessionStartTimeTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    sessionStartTimeTextDisplay->setPosition(0.0, 0.1); //.07
    sessionStartTimeTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    sessionStartTimeTextDisplay->setFontName("MainSmall");
    sessionStartTimeTextDisplay->setCaption("Time Min: ");
    
    sessionStartTimeNumberBackground->setMaterialName("General/BlankInput");
    
    sessionStartTimeNumberTextDisplay->setMetricsMode(GMM_RELATIVE);
    sessionStartTimeNumberTextDisplay->setAlignment(TextAreaOverlayElement::Center);
    sessionStartTimeNumberTextDisplay->setPosition(0.04, 0.0);
    sessionStartTimeNumberTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    sessionStartTimeNumberTextDisplay->setFontName("MainSmall");

    
    
    //Passcode Checkbox
    enableSettingsPasscodeBackground->setMetricsMode(GMM_RELATIVE);
    enableSettingsPasscodeBackground->setPosition(0.2, 0.66);
    enableSettingsPasscodeBackground->setDimensions(0.60, 0.10);
    
    enableSettingsPasscodeTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableSettingsPasscodeTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableSettingsPasscodeTextDisplay->setPosition(0.0, 0.07);
    enableSettingsPasscodeTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    enableSettingsPasscodeTextDisplay->setFontName("MainSmall");
    enableSettingsPasscodeTextDisplay->setCaption("Settings Passcode");
    enableSettingsPasscodeTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
    
    enableSettingsPasscodeButtonBackground->setMaterialName("General/CheckboxBlank");
    
    //HoldoutDelay
    enableHoldoutDelayBackground->setMetricsMode(GMM_RELATIVE);
    enableHoldoutDelayBackground->setPosition(0.45, 0.16);
    enableHoldoutDelayBackground->setDimensions(0.60, 0.10);
    
    enableHoldoutDelayTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableHoldoutDelayTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableHoldoutDelayTextDisplay->setPosition(0.0, 0.05);
    enableHoldoutDelayTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    enableHoldoutDelayTextDisplay->setFontName("MainSmall");
    enableHoldoutDelayTextDisplay->setCaption("Holdout Delay");
    enableHoldoutDelayTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
    
    enableHoldoutDelayButtonBackground->setMaterialName("General/CheckboxBlank");
    
    enableHoldoutDelayNumberButtonBackground->setMaterialName("General/BlankInput");
    
    enableHoldoutDelayNumberTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableHoldoutDelayNumberTextDisplay->setAlignment(TextAreaOverlayElement::Center);
    enableHoldoutDelayNumberTextDisplay->setPosition(0.38, 0.00);
    enableHoldoutDelayNumberTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    enableHoldoutDelayNumberTextDisplay->setFontName("MainSmall");
    //WOW: In iOS version initOverlay always runs. Inificient?
    //holdoutDelayString = Util::toStringFloat(player->holdoutdelayNumber,1);
    //enableHoldoutDelayNumberTextDisplay->setCaption(Util::toStringFloat(player->holdoutdelayNumber,1));

    //Vector2(0.60, 0.26)
    
    
    //IndRecess
    enableIndRecessBackground->setMetricsMode(GMM_RELATIVE);
    enableIndRecessBackground->setPosition(0.20, 0.58);
    enableIndRecessBackground->setDimensions(0.60, 0.10);
    
    enableIndRecessTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableIndRecessTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableIndRecessTextDisplay->setPosition(0.0, 0.05);
    enableIndRecessTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    enableIndRecessTextDisplay->setFontName("MainSmall");
    enableIndRecessTextDisplay->setCaption("Independent Recess");
    enableIndRecessTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
    
    enableIndRecessButtonBackground->setMaterialName("General/CheckboxBlank");
    
    enableIndRecessText2Display->setMetricsMode(GMM_RELATIVE);
    enableIndRecessText2Display->setAlignment(TextAreaOverlayElement::Left);
    enableIndRecessText2Display->setPosition(0.0, 0.1);
    enableIndRecessText2Display->setCharHeight(0.025 * FONT_SZ_MULT);
    enableIndRecessText2Display->setFontName("MainSmall");
    enableIndRecessText2Display->setCaption("Increment Value:");
    
    enableIndRecessNumberButtonBackground->setMaterialName("General/BlankInput");
    
    enableIndRecessNumberText->setMetricsMode(GMM_RELATIVE);
    enableIndRecessNumberText->setAlignment(TextAreaOverlayElement::Left);
    enableIndRecessNumberText->setPosition(0.01, 0.0);
    enableIndRecessNumberText->setCharHeight(0.030 * FONT_SZ_MULT);
    enableIndRecessNumberText->setFontName("MainSmall");
    //indRecessString = Util::toStringFloat(player->indRecessIncrement,2);
    //enableIndRecessNumberText->setCaption(Util::toStringFloat(player->indRecessIncrement,2));
    
    //End IndRecess
    
    //New Sounds
    enableNewSoundsBackground->setMetricsMode(GMM_RELATIVE);
    enableNewSoundsBackground->setPosition(0.20, 0.50);
    enableNewSoundsBackground->setDimensions(0.60, 0.10);
    
    enableNewSoundsTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableNewSoundsTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableNewSoundsTextDisplay->setPosition(0.0, 0.06);
    enableNewSoundsTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    enableNewSoundsTextDisplay->setFontName("MainSmall");
    enableNewSoundsTextDisplay->setCaption("New Sounds");
    
    enableNewSoundsButtonBackground->setMaterialName("General/CheckboxBlank");
    
    //New Nav
    enableNewNavBackground->setMetricsMode(GMM_RELATIVE);
    enableNewNavBackground->setPosition(0.20, 0.40);
    enableNewNavBackground->setDimensions(0.60, 0.10);
    
    enableNewNavTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableNewNavTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableNewNavTextDisplay->setPosition(0.0, 0.05);
    enableNewNavTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    enableNewNavTextDisplay->setFontName("MainSmall");
    enableNewNavTextDisplay->setCaption("Fixed Navigation");
    enableNewNavTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
    
    enableNewNavButtonBackground->setMaterialName("General/CheckboxBlank");
    
    enableNewNavText2Display->setMetricsMode(GMM_RELATIVE);
    enableNewNavText2Display->setAlignment(TextAreaOverlayElement::Left);
    enableNewNavText2Display->setPosition(0.0, 0.1);
    enableNewNavText2Display->setCharHeight(0.025 * FONT_SZ_MULT);
    enableNewNavText2Display->setFontName("MainSmall");
    enableNewNavText2Display->setCaption("Increment Value:");
    
    enableNewNavNumberButtonBackground->setMaterialName("General/BlankInput");
    
    enableNewNavNumberText->setMetricsMode(GMM_RELATIVE);
    enableNewNavNumberText->setAlignment(TextAreaOverlayElement::Left);
    enableNewNavNumberText->setPosition(0.01, 0.0);
    enableNewNavNumberText->setCharHeight(0.030 * FONT_SZ_MULT);
    enableNewNavNumberText->setFontName("MainSmall");
    //newNavigationIncAmountString = Util::toStringFloat(player->newNavIncrement,2);
    //enableNewNavNumberText->setCaption(Util::toStringFloat(player->newNavIncrement,2));
    
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
    enableUnlimitedFuelBackground->setPosition(0.20, 0.08); //.2,.15
    enableUnlimitedFuelBackground->setDimensions(0.60, 0.10);
    
    enableUnlimitedFuelTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableUnlimitedFuelTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableUnlimitedFuelTextDisplay->setPosition(0.0, 0.05);
    enableUnlimitedFuelTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT); //.03
    enableUnlimitedFuelTextDisplay->setFontName("MainSmall");
    enableUnlimitedFuelTextDisplay->setCaption("Unlimited Fuel");
    
    
    enableHoldoutBackground->setMetricsMode(GMM_RELATIVE);
    enableHoldoutBackground->setPosition(0.20, 0.16); //.2,.25
    enableHoldoutBackground->setDimensions(0.60, 0.10);
    
    enableHoldoutTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableHoldoutTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableHoldoutTextDisplay->setPosition(0.0, 0.05);
    enableHoldoutTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);//.03
    enableHoldoutTextDisplay->setFontName("MainSmall");
    enableHoldoutTextDisplay->setCaption("Holdout");
    
    
    
    enableMandatoryRecessBackground->setMetricsMode(GMM_RELATIVE);
    enableMandatoryRecessBackground->setPosition(0.20, 0.24); //.2,.35
    enableMandatoryRecessBackground->setDimensions(0.60, 0.10);
    
    enableMandatoryRecessTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableMandatoryRecessTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    enableMandatoryRecessTextDisplay->setPosition(0.0, 0.05);
    enableMandatoryRecessTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);//.03
    enableMandatoryRecessTextDisplay->setFontName("MainSmall");
    enableMandatoryRecessTextDisplay->setCaption("Mandatory Recess");
    enableMandatoryRecessTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
    

    
    initSpeedTextDisplay->setMetricsMode(GMM_RELATIVE);
    initSpeedTextDisplay->setAlignment(TextAreaOverlayElement::Left);
    initSpeedTextDisplay->setPosition(-0.26, 0.00);
    initSpeedTextDisplay->setCharHeight(0.025 * FONT_SZ_MULT);
    initSpeedTextDisplay->setFontName("MainSmall");
    initSpeedTextDisplay->setCaption("Initial Velocity");
    
    initSpeedNumberTextDisplay->setMetricsMode(GMM_RELATIVE);
    initSpeedNumberTextDisplay->setAlignment(TextAreaOverlayElement::Center);
    initSpeedNumberTextDisplay->setPosition(0.04, 0.005);
    initSpeedNumberTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    initSpeedNumberTextDisplay->setFontName("MainSmall");
    //initSpeedNumberTextDisplay->setCaption("0");
    //initSpeedString = Util::toStringInt(player->initialVelocity);
    //initSpeedNumberTextDisplay->setCaption(Util::toStringInt(player->initialVelocity));

    
    
    enableMandatoryRecessNumberTextDisplay->setMetricsMode(GMM_RELATIVE);
    enableMandatoryRecessNumberTextDisplay->setAlignment(TextAreaOverlayElement::Center);
    enableMandatoryRecessNumberTextDisplay->setPosition(0.025, 0.005);
    enableMandatoryRecessNumberTextDisplay->setCharHeight(0.030 * FONT_SZ_MULT);
    enableMandatoryRecessNumberTextDisplay->setFontName("MainSmall");
    //manRecessString = Util::toStringInt(player->manRecessLevelLimit);
    //enableMandatoryRecessNumberTextDisplay->setCaption(Util::toStringInt(player->manRecessLevelLimit));
    //enableMandatoryRecessNumberTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
    
    
    backButtonBackground->setMaterialName("General/BackButton1");
    
    //Position Buttons
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.01, 0.90), Vector2(0.30, 0.08), backButtonBackground, NULL);
    
    // The Enable Tutorial Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_UNLIMITED_FUEL].setButton("checkfuel", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.13), Vector2(pw, ph), enableUnlimitedFuelButtonBackground, NULL); //Position: .125,.20
    }
    
    // The Enable Holdout Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_HOLDOUT].setButton("checkholdout", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.21), Vector2(pw, ph), enableHoldoutButtonBackground, NULL);//Pos: .125,.3
    }
    // The Enable Holdout Delay Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_HOLDOUT_DELAY].setButton("checkholdoutdelay", overlays[0], GMM_RELATIVE, Vector2(0.38, 0.21), Vector2(pw, ph), enableHoldoutDelayButtonBackground, NULL);//Pos: .125,.3
    }
    // The Enable Holdout Delay Number Checkbox
    {
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        // calculate dimensions for button size and make sure it's square
         buttons[BUTTON_HOLDOUT_DELAY_NUMBER].setButton("checkholdoutdelaynumber", overlays[0], GMM_RELATIVE, Vector2(0.71, 0.21), Vector2(.1, pw), enableHoldoutDelayNumberButtonBackground, NULL);
    }
    
    // The Enable InitSpeed
    {
        // calculate dimensions for button size and make sure it's square
        buttons[BUTTON_INPUT_INIT_SPEED].setButton("checkinitspeed", overlays[0], GMM_RELATIVE, Vector2(0.39, 0.37), Vector2(.08, .06), initSpeedBackground, NULL);
    }
    
    // The Enable Tutorial Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_MANDATORY_RECESS].setButton("checkmandatoryrecess", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.29), Vector2(pw, ph), enableMandatoryRecessButtonBackground, NULL); //.125, .4
    }
    
    // Mandatory Recess Input
    {
        
        buttons[BUTTON_MANDATORY_RECESS_NUMBER].setButton("mandatoryrecessnumber", overlays[0], GMM_RELATIVE, Vector2(0.53, 0.29), Vector2(.05, .06), enableMandatoryRecessNumberBackground, NULL);
    }
    
    // The Enable Passcode Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_PASSCODE].setButton("checksettingspasscode", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.73), Vector2(pw, ph), enableSettingsPasscodeButtonBackground, NULL); //Position: .125,.20
    }
    
    // The Session Start Time
    {
        // calculate dimensions for button size and make sure it's square
        buttons[BUTTON_SESSION_START_NUMBER].setButton("sessionstarttime", overlays[0], GMM_RELATIVE, Vector2(0.23, 0.81), Vector2(.08, .06), sessionStartTimeNumberBackground, NULL);
    }
    // The Session End Time
    {
        // calculate dimensions for button size and make sure it's square
        buttons[BUTTON_SESSION_END_NUMBER].setButton("sessionendtime", overlays[0], GMM_RELATIVE, Vector2(0.42, 0.81), Vector2(.08, .06), sessionEndTimeNumberBackground, NULL);
    }
    // The Session Num
    {
        // calculate dimensions for button size and make sure it's square
        buttons[BUTTON_NUM_OF_SESSIONS_NUMBER].setButton("sessionnum", overlays[0], GMM_RELATIVE, Vector2(0.55, 0.81), Vector2(.08, .06), sessionNumNumberBackground, NULL);
    }
    // The Enable session id screen
    {
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        // calculate dimensions for button size and make sure it's square
        buttons[BUTTON_ENABLE_SESSION_SCREEN].setButton("checksessionid", overlays[0], GMM_RELATIVE, Vector2(0.58, 0.37), Vector2(pw, ph), enableSessionScreenButtonBackground, NULL);
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
        buttons[BUTTON_ENABLE_NEW_NAV].setButton("checknewnav", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.45), Vector2(pw, ph), enableNewNavButtonBackground, NULL);
    }
    // The Enable NewNavNumber Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_NEW_NAV_NUMBER].setButton("checknewnavnumber", overlays[0], GMM_RELATIVE, Vector2(0.50, 0.50), Vector2(.10, pw), enableNewNavNumberButtonBackground, NULL);
    }
    
    // The Enable New Sounds Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_NEW_SOUNDS].setButton("checknewsounds", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.55), Vector2(pw, ph), enableNewSoundsButtonBackground, NULL);
    }
    
    // The Enable IndRecess Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_IND_RECESS].setButton("checkindrecess", overlays[0], GMM_RELATIVE, Vector2(0.125, 0.63), Vector2(pw, ph), enableIndRecessButtonBackground, NULL);
    }
    // The Enable IndRecess Fixed Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_IND_RECESS_FIXED].setButton("checkindrecessfixed", overlays[0], GMM_RELATIVE, Vector2(0.57, 0.62), Vector2(pw, ph), enableIndRecessFixedButtonBackground, NULL);
    }
    // The Enable IndRecessNumber Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float ph = 0.05;
        float pw = ph * (globals.screenWidth / globals.screenHeight);
        buttons[BUTTON_ENABLE_IND_RECESS_NUMBER].setButton("checkindrecessnumber", overlays[0], GMM_RELATIVE, Vector2(0.50, 0.68), Vector2(.10, pw), enableIndRecessNumberButtonBackground, NULL);
    }
    
    //should add the check for passcode toggle only in OSX Version
    if(player->enableSettingsPasscode)
    {
        enableSettingsPasscodeTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        enableSettingsPasscodeButtonBackground->setMaterialName("General/CheckboxGreen");
    }
    else
    {
        enableSettingsPasscodeTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        enableSettingsPasscodeButtonBackground->setMaterialName("General/CheckboxBlank");
    }
    //NewSounds Checkbox
    if (globals.newSounds)
    {
        
        enableNewSoundsButtonBackground->setMaterialName("General/CheckboxGreen");
        enableNewSoundsTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
    }
    else
    {
       
        
        enableNewSoundsButtonBackground->setMaterialName("General/CheckboxBlank");
        enableNewSoundsTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        
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
    
    //Ind Recess Checkbox
    if (player->indRecessEnabled)
    {
        enableIndRecessButtonBackground->setMaterialName("General/CheckboxGreen");
        enableIndRecessTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        enableIndRecessText2Display->show();
        enableIndRecessNumberButtonBackground->show();
        enableIndRecessNumberText->show();
        enableIndRecessFixedButtonBackground->show();
        enableIndRecessFixedTextDisplay->show();
        if(player->enableIndRecessFixed)
        {
            enableIndRecessFixedButtonBackground->setMaterialName("General/CheckboxGreen");
            enableIndRecessFixedTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
        }
        else
        {
            enableIndRecessFixedButtonBackground->setMaterialName("General/CheckboxBlank");
            enableIndRecessFixedTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
            
        }
    }
    else
    {
        
        enableIndRecessButtonBackground->setMaterialName("General/CheckboxBlank");
        enableIndRecessTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        enableIndRecessText2Display->hide();
        enableIndRecessNumberButtonBackground->hide();
        enableIndRecessNumberText->hide();
        enableIndRecessFixedButtonBackground->hide();
        enableIndRecessFixedTextDisplay->hide();

        
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
        enableHoldoutDelayButtonBackground->show();
        enableHoldoutDelayTextDisplay->show();
        //HoldoutDelayCheckBox
        if (player->holdoutdelayEnabled)
        {
            enableHoldoutDelayButtonBackground->setMaterialName("General/CheckboxGreen");
            enableHoldoutDelayTextDisplay->setColour(Ogre::ColourValue(1,1,1,1));
            enableHoldoutDelayNumberButtonBackground->show();
            enableHoldoutDelayNumberTextDisplay->show();
        }
        else
        {
            
            enableHoldoutDelayButtonBackground->setMaterialName("General/CheckboxBlank");
            enableHoldoutDelayTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
            enableHoldoutDelayNumberButtonBackground->hide();
            enableHoldoutDelayNumberTextDisplay->hide();
            
        }
        
        
    }
    else
    {
        enableHoldoutButtonBackground->setMaterialName("General/CheckboxBlank");
        enableHoldoutTextDisplay->setColour(Ogre::ColourValue(.5,.5,.5,1));
        
        //Delay:
        enableHoldoutDelayButtonBackground->hide();
        enableHoldoutDelayTextDisplay->hide();
        enableHoldoutDelayNumberButtonBackground->hide();
        enableHoldoutDelayNumberTextDisplay->hide();
        
        
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