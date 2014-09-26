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
    
    // Since our scheduler menu can only show so many (currently x=5 previous levels),
    // we have a function that will get the starting index that represents the last x
    // levels in the our current history.
    int getStartingSchedulerHistoryIndex() const;
    // Functions to reset the selected item displayed in the hud
    void clearSelection();
    // Set the hud to reflect the current item selected by the player
    void setSelection();
    
    static const int NUM_SELECTIONS = 3;
    static const int SCHEDULE_LEN = 5;
    
protected:
    Player* player;
    
    static const int NUM_UNIQUE_BUTTONS = 2 + NUM_SELECTIONS + SCHEDULE_LEN;
    enum Buttons {
        BUTTON_BACK, BUTTON_PLAY
    };
    
    PanelOverlayElement* schedulerMenuBackdrop;
    PanelOverlayElement* schedulerMenuTitleBackground;
    TextAreaOverlayElement* schedulerMenuTitleText;
    
    struct LevelOverlayElement
    {
        PanelOverlayElement* entireBackground;
        TextAreaOverlayElement* title;
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
    std::vector<LevelOverlayElement> historyOverlayPanels;
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
