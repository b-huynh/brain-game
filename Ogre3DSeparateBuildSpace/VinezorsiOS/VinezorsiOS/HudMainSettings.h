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
protected:
    Player* player;
    
    static const int NUM_UNIQUE_BUTTONS = 3;
    enum Buttons {
        BUTTON_BACK, BUTTON_CONTROL_SETTINGS, BUTTON_ENABLETUTORIALS
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
    
    PanelOverlayElement* soundVolumeSliderRangeBackground;
    PanelOverlayElement* soundVolumeSliderBallBackground;
    TextAreaOverlayElement* soundVolumeSliderText;
    TextAreaOverlayElement* soundVolumeSliderDisplay;
    
    PanelOverlayElement* enableTutorialsEntireBackground;
    TextAreaOverlayElement* enableTutorialsTextDisplay;
    PanelOverlayElement* enableTutorialsButtonBackground;
    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
    
    void positionSliderBalls();
};

#endif /* defined(__VinezorsiOS__HudMainSettings__) */
