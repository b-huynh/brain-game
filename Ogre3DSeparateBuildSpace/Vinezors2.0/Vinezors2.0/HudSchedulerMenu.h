//
//  HudSchedulerMenu.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 9/15/14.
//
//

#ifndef __Vinezors2_0__HudSchedulerMenu__
#define __Vinezors2_0__HudSchedulerMenu__

#include "Hud.h"
#include "Util.h"

class HudSchedulerMenu : public Hud
{
public:
    HudSchedulerMenu(Player* player);
    virtual ~HudSchedulerMenu();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
    
    static const int NUM_SELECTIONS = 3;
protected:
    Player* player;
    
    static const int NUM_UNIQUE_BUTTONS = 1 + NUM_SELECTIONS;
    enum Buttons {
        BUTTON_BACK
    };
    
    PanelOverlayElement* schedulerMenuBackdrop;
    PanelOverlayElement* schedulerMenuTitleBackground;
    TextAreaOverlayElement* schedulerMenuTitleText;
    
    struct LevelOverlayElement
    {
        PanelOverlayElement* entireBackground;
        TextAreaOverlayElement* description;
        TextAreaOverlayElement* title;
        
    };
    std::vector<LevelOverlayElement> levelOverlayPanels;
    
    PanelOverlayElement* backButtonBackground;
    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
};

#endif /* defined(__Vinezors2_0__HudSchedulerMenu__) */
