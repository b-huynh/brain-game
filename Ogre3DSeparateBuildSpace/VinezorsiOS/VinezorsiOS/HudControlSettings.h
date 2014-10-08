//
//  HudControlSettings.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 5/2/14.
//
//

#ifndef __Vinezors2_0__HudControlSettings__
#define __Vinezors2_0__HudControlSettings__

#include "Hud.h"
#include "Util.h"

class HudControlSettings : public Hud
{
public:
    HudControlSettings(Player* player);
    virtual ~HudControlSettings();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
    
    void resetDefaults();
    
    HudSlider* maxVelSlider;
    HudSlider* minVelStopperSlider;
    HudSlider* dampingDecayFreeSlider;
    HudSlider* dampingDecayStopSlider;
    HudSlider* dampingDropFreeSlider;
    HudSlider* dampingDropStopSlider;
protected:
    Player* player;
    
    static const int NUM_UNIQUE_BUTTONS = 3;
    enum Buttons {
        BUTTON_BACK, BUTTON_DEFAULTS, BUTTON_INVERTED
    };
    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* controlSettingsBackdrop;
    
    PanelOverlayElement* controlSettingsTitleBackground;
    TextAreaOverlayElement* controlSettingsTitleText;
    
    PanelOverlayElement* backButtonBackground;
    PanelOverlayElement* defaultsButtonBackground;
    
    PanelOverlayElement* maxVelSliderRangeBackground;
    PanelOverlayElement* maxVelSliderBallBackground;
    TextAreaOverlayElement* maxVelSliderText;
    TextAreaOverlayElement* maxVelSliderDisplay;
    
    PanelOverlayElement* minVelStopperSliderRangeBackground;
    PanelOverlayElement* minVelStopperSliderBallBackground;
    TextAreaOverlayElement* minVelStopperSliderText;
    TextAreaOverlayElement* minVelStopperSliderDisplay;
    
    PanelOverlayElement* dampingDecayFreeSliderRangeBackground;
    PanelOverlayElement* dampingDecayFreeSliderBallBackground;
    TextAreaOverlayElement* dampingDecayFreeSliderText;
    TextAreaOverlayElement* dampingDecayFreeSliderDisplay;
    
    PanelOverlayElement* dampingDecayStopSliderRangeBackground;
    PanelOverlayElement* dampingDecayStopSliderBallBackground;
    TextAreaOverlayElement* dampingDecayStopSliderText;
    TextAreaOverlayElement* dampingDecayStopSliderDisplay;
    
    PanelOverlayElement* dampingDropFreeSliderRangeBackground;
    PanelOverlayElement* dampingDropFreeSliderBallBackground;
    TextAreaOverlayElement* dampingDropFreeSliderText;
    TextAreaOverlayElement* dampingDropFreeSliderDisplay;
    
    PanelOverlayElement* dampingDropStopSliderRangeBackground;
    PanelOverlayElement* dampingDropStopSliderBallBackground;
    TextAreaOverlayElement* dampingDropStopSliderText;
    TextAreaOverlayElement* dampingDropStopSliderDisplay;
    
    PanelOverlayElement* invertedEntireBackground;
    TextAreaOverlayElement* invertedTextDisplay;
    PanelOverlayElement* invertedButtonBackground;
    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
    
    void positionSliderBalls();
};

#endif /* defined(__Vinezors2_0__HudControlSettings__) */
