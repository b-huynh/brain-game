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
    
    virtual std::string processButtons(Vector2 target); // Note: calls base class queryButtons
    
    // Functions to reset the selected item displayed in the hud
    void clearSelection();
    // Set the hud to reflect the current item selected by the player
    void setSelection();
    
    void setSelectToIcon(PanelOverlayElement* icon, int mode);
    
    static const int NUM_SELECTIONS = 3;
    static const int SCHEDULE_LEN = 5;
    
protected:
    Player* player;
    
    static const int NUM_UNIQUE_BUTTONS = 3 + NUM_SELECTIONS + SCHEDULE_LEN;
    enum Buttons {
        BUTTON_BACK, BUTTON_PLAY, BUTTON_START_MAN_RECESS
    };
    
    PanelOverlayElement* schedulerMenuEntireBackground;
    
    PanelOverlayElement* schedulerMenuScoreCurrBackground;
    TextAreaOverlayElement* schedulerMenuScoreCurrText;
    
    PanelOverlayElement* schedulerMenuAverageMemoryBackground;
    TextAreaOverlayElement* schedulerMenuAverageMemoryText;
    
    //For Mandatory Recess!
    //MEssage
    PanelOverlayElement* schedulerManRecessMessageBackground;
    PanelOverlayElement* schedulerManRecessDisableBackground;
    TextAreaOverlayElement* schedulerManRecessMessageText;
    
    //Button
    PanelOverlayElement* schedulerManRecessPlayButtonBackground;
    
    struct LevelOverlayElement
    {
        PanelOverlayElement* entireBackground;
        TextAreaOverlayElement* title;
        TextAreaOverlayElement* value;
    };
    
    struct DescriptionOverlayElement
    {
        PanelOverlayElement* entireBackground;
        TextAreaOverlayElement* title;
        TextAreaOverlayElement* names;
        TextAreaOverlayElement* values;
        PanelOverlayElement* meritIcon;
    };

    std::vector<LevelOverlayElement> levelOverlayPanels;
public:
    std::vector<LevelOverlayElement> historyOverlayPanels;
    PanelOverlayElement* selectIconHistory;
    PanelOverlayElement* selectIconChoice;
protected:
    DescriptionOverlayElement levelDetails;
    
    
    PanelOverlayElement* backButtonBackground;
    PanelOverlayElement* playButtonBackground;
    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
    
    void setScheduleHistory();
};

#endif /* defined(__Vinezors2_0__HudSchedulerMenu__) */
