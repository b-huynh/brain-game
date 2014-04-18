//
//  HudCredits.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#ifndef __Vinezors2_0__HudCredits__
#define __Vinezors2_0__HudCredits__

#include "Hud.h"
#include "Util.h"

class HudCredits : public Hud
{
public:
    HudCredits(Player* player);
    virtual ~HudCredits();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
protected:
    Player* player;
    
    enum Buttons {
        BUTTON_BACK
    };
    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* creditsTitle;
    PanelOverlayElement* backButtonBackground;
    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void setOverlay();
};

#endif /* defined(__Vinezors2_0__HudCredits__) */
