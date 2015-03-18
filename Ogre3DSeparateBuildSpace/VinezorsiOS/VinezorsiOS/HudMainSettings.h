//
//  HudMainSettings.h
//  VinezorsiOS
//
//  Created by Calvin Phung on 6/26/14.
//
//

#ifndef __VinezorsiOS__HudMainSettings__
#define __VinezorsiOS__HudMainSettings__

#include "Hud.h"
#include "Util.h"

class HudMainSettings : public Hud
{
public:
    HudMainSettings(Player* player);
    virtual ~HudMainSettings();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
    
    HudSlider* musicVolumeSlider;
    HudSlider* soundVolumeSlider;
    HudSlider* holdoutSlider;
    HudSlider* holdoutLBSlider;
    HudSlider* holdoutUBSlider;
    
    /*
    //Enable NumPad:
    bool enableNumpad = false;
    
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
    float passwordDot3Init_x = .21;*/
    
    
protected:
    Player* player;
    //const int PASSWORD[4] = {3,1,4,1};
    
    static const int NUM_UNIQUE_BUTTONS = 16;
    enum Buttons {
        BUTTON_BACK, BUTTON_CONTROL_SETTINGS, BUTTON_ENABLETUTORIALS, BUTTON_ENABLESYNCDATA, BUTTON_STUDY_SETTINGS,
        BUTTON_NUMPAD_0, BUTTON_NUMPAD_1, BUTTON_NUMPAD_2, BUTTON_NUMPAD_3, BUTTON_NUMPAD_4, BUTTON_NUMPAD_5, BUTTON_NUMPAD_6, BUTTON_NUMPAD_7, BUTTON_NUMPAD_8, BUTTON_NUMPAD_9,
        BUTTON_NUMPAD_CANCEL
    };
    

    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* mainSettingsBackdrop1;
    PanelOverlayElement* mainSettingsBackdrop2;
    PanelOverlayElement* mainSettingsTitleBackground;
    TextAreaOverlayElement* mainSettingsTitleText;
    
    PanelOverlayElement* backButtonBackground;
    PanelOverlayElement* controlSettingsButtonBackground;
    
    PanelOverlayElement* musicVolumeSliderRangeBackground;
    PanelOverlayElement* musicVolumeSliderBallBackground;
    TextAreaOverlayElement* musicVolumeSliderText;
    TextAreaOverlayElement* musicVolumeSliderDisplay;
    
    PanelOverlayElement* studySettingsButtonBackground;
    TextAreaOverlayElement* studySettingTextDisplay;
    
    /*
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
    
    PanelOverlayElement* numpadButton_Cancel;
    TextAreaOverlayElement* numpadButton_Cancel_TextDisplay;

    //Password
    TextAreaOverlayElement* password_Title;
    PanelOverlayElement* password_Choice0;
    PanelOverlayElement* password_Choice1;
    PanelOverlayElement* password_Choice2;
    PanelOverlayElement* password_Choice3;*/
    

    
    
    //Holdout Slider
    PanelOverlayElement* holdoutSliderRangeBackground;
    PanelOverlayElement* holdoutSliderBallBackground;
    TextAreaOverlayElement* holdoutSliderText;
    TextAreaOverlayElement* holdoutSliderDisplay;
    
    PanelOverlayElement* holdoutLBSliderRangeBackground;
    PanelOverlayElement* holdoutLBSliderBallBackground;
    TextAreaOverlayElement* holdoutLBSliderText;
    TextAreaOverlayElement* holdoutLBSliderDisplay;
    
    PanelOverlayElement* holdoutUBSliderRangeBackground;
    PanelOverlayElement* holdoutUBSliderBallBackground;
    TextAreaOverlayElement* holdoutUBSliderText;
    TextAreaOverlayElement* holdoutUBSliderDisplay;
    
    PanelOverlayElement* soundVolumeSliderRangeBackground;
    PanelOverlayElement* soundVolumeSliderBallBackground;
    TextAreaOverlayElement* soundVolumeSliderText;
    TextAreaOverlayElement* soundVolumeSliderDisplay;
    TextAreaOverlayElement* soundVolumeSliderWarning;
    
    PanelOverlayElement* enableTutorialsEntireBackground;
    TextAreaOverlayElement* enableTutorialsTextDisplay;
    PanelOverlayElement* enableTutorialsButtonBackground;
    
    PanelOverlayElement* syncDataEntireBackground;
    TextAreaOverlayElement* syncDataTextDisplay;
    PanelOverlayElement* syncDataButtonBackground;
    TextAreaOverlayElement* syncDataInternetNotification;
    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
    
    void positionSliderBalls();
};

#endif /* defined(__VinezorsiOS__HudMainSettings__) */
