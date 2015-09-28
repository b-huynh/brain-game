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
