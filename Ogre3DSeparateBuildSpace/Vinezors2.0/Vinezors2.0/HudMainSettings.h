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
    
    static const int NUM_UNIQUE_BUTTONS = 4;
    enum Buttons {
        BUTTON_BACK, BUTTON_CONTROL_SETTINGS, BUTTON_ENABLETUTORIALS, BUTTON_ENABLESYNCDATA
    };
    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* mainSettingsTitleBackground;
    TextAreaOverlayElement* mainSettingsTitleText;
    
    PanelOverlayElement* backButtonBackground;
    PanelOverlayElement* controlSettingsButtonBackground;
    
    PanelOverlayElement* musicVolumeSliderRangeBackground;
    PanelOverlayElement* musicVolumeSliderBallBackground;
    TextAreaOverlayElement* musicVolumeSliderText;
    TextAreaOverlayElement* musicVolumeSliderDisplay;
    
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
