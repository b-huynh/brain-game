//
//  HudTabSettings.h
//  VinezorsiOS
//
//  Created by BGC_Bernie on 4/28/15.
//
//

#ifndef __VinezorsiOS__HudTabSettings__
#define __VinezorsiOS__HudTabSettings__


#include "Hud.h"
#include "Util.h"

class HudTabSettings : public Hud
{
public:
    HudTabSettings(Player* player);
    virtual ~HudTabSettings();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
    
    PanelOverlayElement* paragraphTextBackground;
    TextAreaOverlayElement* paragraphText;

    
    Vector2 initTextPosition = Vector2(.2,.2);
    
    //Unlimited Switch
    PanelOverlayElement* enableUnlimitedFuelBackground;
    TextAreaOverlayElement* enableUnlimitedFuelTextDisplay;
    PanelOverlayElement* enableUnlimitedFuelButtonBackground;
    PanelOverlayElement* enableUnlimitedFuelUnderline;



protected:
    Player* player;
    
    static const int NUM_UNIQUE_BUTTONS = 3;
    enum Buttons {
        BUTTON_BACK, BUTTON_ENABLE_UNLIMITED_FUEL, BUTTON_GENERAL_OPTION
    };
    
    //Entire Background
    PanelOverlayElement* tabSettingsEntireBackground;
    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* tabSettingsTitleBackground;
    TextAreaOverlayElement* tabSettingsTitleText;

    //Line Seperator
    PanelOverlayElement* tabSettingsLineSeperator;
    
    //Back Button Text
    PanelOverlayElement* backButtonBackground;
    TextAreaOverlayElement* backButtonText;

    //DottedLineMainSettings
    PanelOverlayElement* tabSettingsMainSettingDottedSeperator;
    
    //General Text
    PanelOverlayElement* GeneralOptionBackground;
    PanelOverlayElement* GeneralOptionButton;
    TextAreaOverlayElement* GeneralOptionText;

    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
    
    void positionSliderBalls();
};

#endif /* defined(__VinezorsiOS__HudTabSettings__) */