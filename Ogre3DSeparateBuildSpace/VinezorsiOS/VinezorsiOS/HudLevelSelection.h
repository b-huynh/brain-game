//
//  HudLevelSelection.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#ifndef __Vinezors2_0__HudLevelSelection__
#define __Vinezors2_0__HudLevelSelection__

#include "Hud.h"
#include "Util.h"

class HudLevelSelection : public Hud
{
public:
    HudLevelSelection(Player* player);
    virtual ~HudLevelSelection();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
protected:
    Player* player;
    
    enum Buttons {
        BUTTON_LEVELA1, BUTTON_LEVELA2, BUTTON_LEVELA3, BUTTON_LEVELA4, BUTTON_LEVELA5, BUTTON_LEVELA6, BUTTON_LEVELA7, BUTTON_LEVELA8,
        BUTTON_LEVELB1, BUTTON_LEVELB2, BUTTON_LEVELB3, BUTTON_LEVELB4, BUTTON_LEVELB5, BUTTON_LEVELB6, BUTTON_LEVELB7, BUTTON_LEVELB8,
        BUTTON_LEVELC1, BUTTON_LEVELC2, BUTTON_LEVELC3, BUTTON_LEVELC4, BUTTON_LEVELC5, BUTTON_LEVELC6, BUTTON_LEVELC7, BUTTON_LEVELC8,
        BUTTON_BACK
    };
    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* levelSelectPrompt;
    PanelOverlayElement* levelA1Background;
    PanelOverlayElement* levelA2Background;
    PanelOverlayElement* levelA3Background;
    PanelOverlayElement* levelA4Background;
    PanelOverlayElement* levelA5Background;
    PanelOverlayElement* levelA6Background;
    PanelOverlayElement* levelA7Background;
    PanelOverlayElement* levelA8Background;
    PanelOverlayElement* levelB1Background;
    PanelOverlayElement* levelB2Background;
    PanelOverlayElement* levelB3Background;
    PanelOverlayElement* levelB4Background;
    PanelOverlayElement* levelB5Background;
    PanelOverlayElement* levelB6Background;
    PanelOverlayElement* levelB7Background;
    PanelOverlayElement* levelB8Background;
    PanelOverlayElement* levelC1Background;
    PanelOverlayElement* levelC2Background;
    PanelOverlayElement* levelC3Background;
    PanelOverlayElement* levelC4Background;
    PanelOverlayElement* levelC5Background;
    PanelOverlayElement* levelC6Background;
    PanelOverlayElement* levelC7Background;
    PanelOverlayElement* levelC8Background;
    PanelOverlayElement* backButtonBackground;
    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void setOverlay();
    
    void setLevelButton(PanelOverlayElement* levelBackground, char row, int no);
};

#endif /* defined(__Vinezors2_0__HudLevelSelection__) */
