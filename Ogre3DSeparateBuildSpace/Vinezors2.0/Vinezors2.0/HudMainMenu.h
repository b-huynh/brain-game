//
//  HudMainMenu.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#ifndef __Vinezors2_0__HudMainMenu__
#define __Vinezors2_0__HudMainMenu__

#include "Hud.h"
#include "Util.h"

class HudMainMenu : public Hud
{
public:
    HudMainMenu(Player* player);
    virtual ~HudMainMenu();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
protected:
    Player* player;
    
    enum Buttons {
        BUTTON_PLAY, BUTTON_CREDITS, BUTTON_EXIT
    };
    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* mainTitle;
    PanelOverlayElement* playButtonBackground;
    PanelOverlayElement* creditsButtonBackground;
    PanelOverlayElement* exitButtonBackground;
    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void setOverlay();
};

#endif /* defined(__Vinezors2_0__HudMainMenu__) */
