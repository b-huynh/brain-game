//
//  HudStudySettings.h
//  Vinezors2.0
//
//  Created by BrainGameCenter on 3/3/15.
//
//

#ifndef __Vinezors2_0__HudStudySettings__
#define __Vinezors2_0__HudStudySettings__

#include "Hud.h"
#include "Util.h"


class HudStudySettings : public Hud
{
public:
    HudStudySettings(Player* player);
    virtual ~HudStudySettings();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
    
    void initStrings();
    
    //Enable NumPad:
    bool enableNumpad = false;
    
    
    //InitSpeed
    std::string initSpeedString = "";
    
    //ManRecess
    std::string manRecessString = "";
    
    //Senior Navigation
    std::string newNavigationIncAmountString = "";
    
    //IndRecess
    std::string indRecessString = "";
    
    //Holdout Delay
    std::string holdoutDelayString = "";
    
    //SessionStartTime
    std::string sessionStartTimeString = "";
    
    //SessionEndTime
    std::string sessionEndtimeString = "";
    
    //NumberofSessions
    std::string numOfSessionsString = "";
    
    //Passcode
    int Passcode_counter = 0;
    int PASSWORD_LENGTH = 4;
    int user_password[4] = {-1,-1,-1,-1};
    bool enterStudySettings = false;
    bool startDotTimer = false;
    bool startDotTimerIncorrect= false;
    
    float numToWait = .3;
    float numToWaitIncorrect = .3;
    
    float passwordDot0Init_x = .06;
    float passwordDot1Init_x = .11;
    float passwordDot2Init_x = .16;
    float passwordDot3Init_x = .21;
    
    const int PASSWORD[4] = {3,1,4,1};
    
    bool showDecimal = false;
    bool popUpisOut = false;
    
    
    void showPopUp();
    void hidePopUp();
    
    enum NumpadStatus {
        
        NONE, INIT_VELOCITY, MAN_RECESS, HOLDOUT_DELAY, NEW_NAV_INC, IND_RECESS_INC, SESSION_START_TIME, SESSION_END_TIME,
        NUM_OF_SESSIONS, PASSCODE
        
    };
    
    int nStatus = NONE;
    
protected:
    Player* player;
    
    static const int NUM_UNIQUE_BUTTONS = 36;
    enum Buttons {
        BUTTON_BACK,BUTTON_ENABLE_UNLIMITED_FUEL, BUTTON_ENABLE_HOLDOUT, BUTTON_INPUT_INIT_SPEED, BUTTON_ENABLE_MANDATORY_RECESS,
        BUTTON_MANDATORY_RECESS_NUMBER, BUTTON_NUMPAD_0, BUTTON_NUMPAD_1, BUTTON_NUMPAD_2, BUTTON_NUMPAD_3,
        BUTTON_NUMPAD_4, BUTTON_NUMPAD_5, BUTTON_NUMPAD_6, BUTTON_NUMPAD_7, BUTTON_NUMPAD_8,
        BUTTON_NUMPAD_9, BUTTON_NUMPAD_BACK, BUTTON_NUMPAD_SAVE, BUTTON_NUMPAD_DECIMAL, NUMPAD_BG,
        BUTTON_ENABLE_HOLDOUT_DELAY, BUTTON_HOLDOUT_DELAY_NUMBER, BUTTON_ENABLE_NEW_NAV, BUTTON_ENABLE_NEW_NAV_NUMBER, BUTTON_ENABLE_NEW_SOUNDS,
        BUTTON_ENABLE_IND_RECESS, BUTTON_ENABLE_IND_RECESS_NUMBER, BUTTON_ENABLE_SESSION_SETTINGS, BUTTON_SESSION_START_NUMBER, BUTTON_SESSION_END_NUMBER,
        BUTTON_NUM_OF_SESSIONS_NUMBER, BUTTON_ENABLE_PASSCODE, BUTTON_ENABLE_SESSION_SCREEN, BUTTON_ENABLE_IND_RECESS_FIXED,
        BUTTON_POPUP_OK, BUTTON_POPUP_REVERT
        
    };
    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* studySettingsBackdrop1;
    PanelOverlayElement* studySettingsBackdrop2;
    PanelOverlayElement* studySettingsTitleBackground;
    TextAreaOverlayElement* studySettingsTitleText;
    
    PanelOverlayElement* backButtonBackground;
    
    //Unlimited Fuel Checkbox
    PanelOverlayElement* enableUnlimitedFuelBackground;
    TextAreaOverlayElement* enableUnlimitedFuelTextDisplay;
    PanelOverlayElement* enableUnlimitedFuelButtonBackground;
    
    //Holdout Checkbox
    PanelOverlayElement* enableHoldoutBackground;
    TextAreaOverlayElement* enableHoldoutTextDisplay;
    PanelOverlayElement* enableHoldoutButtonBackground;
    
    //Init Speed
    PanelOverlayElement* initSpeedBackground;
    TextAreaOverlayElement* initSpeedTextDisplay;
    TextAreaOverlayElement* initSpeedNumberTextDisplay;
    
    //Mandatory Recess
    PanelOverlayElement* enableMandatoryRecessBackground;
    TextAreaOverlayElement* enableMandatoryRecessTextDisplay;
    PanelOverlayElement* enableMandatoryRecessButtonBackground;
    PanelOverlayElement* enableMandatoryRecessNumberBackground;
    TextAreaOverlayElement* enableMandatoryRecessNumberTextDisplay;
    
    //Numpad
    PanelOverlayElement* numpadBackground;
    
    PanelOverlayElement* numpadButton_0;
    TextAreaOverlayElement* numpadButton_0_TextDisplay;
    
    PanelOverlayElement* numpadButton_1;
    TextAreaOverlayElement* numpadButton_1_TextDisplay;
    
    PanelOverlayElement* numpadButton_2;
    TextAreaOverlayElement* numpadButton_2_TextDisplay;
    
    PanelOverlayElement* numpadButton_3;
    TextAreaOverlayElement* numpadButton_3_TextDisplay;
    
    PanelOverlayElement* numpadButton_4;
    TextAreaOverlayElement* numpadButton_4_TextDisplay;
    
    PanelOverlayElement* numpadButton_5;
    TextAreaOverlayElement* numpadButton_5_TextDisplay;
    
    PanelOverlayElement* numpadButton_6;
    TextAreaOverlayElement* numpadButton_6_TextDisplay;
    
    PanelOverlayElement* numpadButton_7;
    TextAreaOverlayElement* numpadButton_7_TextDisplay;
    
    PanelOverlayElement* numpadButton_8;
    TextAreaOverlayElement* numpadButton_8_TextDisplay;
    
    PanelOverlayElement* numpadButton_9;
    TextAreaOverlayElement* numpadButton_9_TextDisplay;
    
    PanelOverlayElement* numpadButton_Back;
    TextAreaOverlayElement* numpadButton_Back_TextDisplay;
    
    PanelOverlayElement* numpadButton_Save;
    TextAreaOverlayElement* numpadButton_Save_TextDisplay;
    
    PanelOverlayElement* numpadButton_Decimal;
    TextAreaOverlayElement* numpadButton_Decimal_TextDisplay;
    
    //Holdout Delay Checkbox
    PanelOverlayElement* enableHoldoutDelayBackground;
    TextAreaOverlayElement* enableHoldoutDelayTextDisplay;
    PanelOverlayElement* enableHoldoutDelayButtonBackground;
    
    TextAreaOverlayElement* enableHoldoutDelayNumberTextDisplay;
    PanelOverlayElement* enableHoldoutDelayNumberButtonBackground;
    
    //New Nav Checkbox
    PanelOverlayElement* enableNewNavBackground;
    TextAreaOverlayElement* enableNewNavTextDisplay;
    PanelOverlayElement* enableNewNavButtonBackground;
    TextAreaOverlayElement* enableNewNavText2Display;
    PanelOverlayElement* enableNewNavNumberButtonBackground;
    TextAreaOverlayElement* enableNewNavNumberText;

    //New Sounds Checkbox
    PanelOverlayElement* enableNewSoundsBackground;
    TextAreaOverlayElement* enableNewSoundsTextDisplay;
    PanelOverlayElement* enableNewSoundsButtonBackground;
    
    //Independent Recess
    PanelOverlayElement* enableIndRecessBackground;
    TextAreaOverlayElement* enableIndRecessTextDisplay;
    PanelOverlayElement* enableIndRecessButtonBackground;
    TextAreaOverlayElement* enableIndRecessText2Display;
    PanelOverlayElement* enableIndRecessNumberButtonBackground;
    TextAreaOverlayElement* enableIndRecessNumberText;
    
    //Passcode Checkbox
    PanelOverlayElement* enableSettingsPasscodeBackground;
    TextAreaOverlayElement* enableSettingsPasscodeTextDisplay;
    PanelOverlayElement* enableSettingsPasscodeButtonBackground;

    
    //Session Start Time
    PanelOverlayElement* sessionTimeSettingsBackground;
    TextAreaOverlayElement* sessionStartTimeTextDisplay;
    PanelOverlayElement* sessionStartTimeNumberBackground;
    TextAreaOverlayElement* sessionStartTimeNumberTextDisplay;
    
    //Session End Time
    TextAreaOverlayElement* sessionEndTimeTextDisplay;
    PanelOverlayElement* sessionEndTimeNumberBackground;
    TextAreaOverlayElement* sessionEndTimeNumberTextDisplay;
    
    //Session Num Time
    TextAreaOverlayElement* sessionNumTextDisplay;
    PanelOverlayElement* sessionNumNumberBackground;
    TextAreaOverlayElement* sessionNumNumberTextDisplay;
    
    //SessionID Screen Toggle
    PanelOverlayElement* enableSessionScreenBackground;
    TextAreaOverlayElement* enableSessionScreenTextDisplay;
    PanelOverlayElement* enableSessionScreenButtonBackground;
    
    //Ind Recess Fixed Checkbox
    TextAreaOverlayElement* enableIndRecessFixedTextDisplay;
    PanelOverlayElement* enableIndRecessFixedButtonBackground;
    //PanelOverlayElement* enableIndRecessFixedBackground;
    
    //Vendor ID
    TextAreaOverlayElement* vendorIDTextDisplay;
    PanelOverlayElement* vendorIDBackground;
    
    //PopUp Change
    PanelOverlayElement* somethingChangedMessageBackground;
    PanelOverlayElement* somethingChangedDisableBackground;
    TextAreaOverlayElement* somethingChangedMessageText;
    
    //Button "Restart Session"
    PanelOverlayElement* somethingChangedOkButton;
    TextAreaOverlayElement* somethingChangedOkButtonText;
    
    //Button "Revert"
    PanelOverlayElement* somethingChangedRevertButton;
    TextAreaOverlayElement* somethingChangedRevertButtonText;

    //Passcode Bubbles
    TextAreaOverlayElement* password_Title;
    PanelOverlayElement* password_Choice0;
    PanelOverlayElement* password_Choice1;
    PanelOverlayElement* password_Choice2;
    PanelOverlayElement* password_Choice3;

    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
    
    void positionSliderBalls();
};

#endif /* defined(__Vinezors2_0__HudStudySettings__) */