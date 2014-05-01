//
//  HudStage.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#ifndef __Vinezors2_0__HudStage__
#define __Vinezors2_0__HudStage__

#include "Hud.h"
#include "Util.h"

class HudStage : public Hud
{
public:
    HudStage(Player* player, Tunnel* tunnel);
    virtual ~HudStage();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
protected:
    Player* player;
    Tunnel* tunnel;
    
    enum ButtonSet {
        BUTTON_PAUSE,
        BUTTON_TOGGLE1, BUTTON_TOGGLE2, BUTTON_TOGGLE3, BUTTON_TOGGLE4,
        BUTTON_POWERUP1, BUTTON_POWERUP2, BUTTON_POWERUP3,
        BUTTON_RESUME, BUTTON_NEXT, BUTTON_RESTART, BUTTON_LEVELSELECT
    };
    
    // Overlay items for GAME_STATE_PLAY
    BorderPanelOverlayElement* healthArea;
    PanelOverlayElement* barHP;
    PanelOverlayElement* indicator;
    PanelOverlayElement* pauseBackground;
    std::vector<PanelOverlayElement*> collectionBar;
    PanelOverlayElement* GUITopPanel;
    PanelOverlayElement* GUILeftPanel;
    PanelOverlayElement* GUIRightPanel;
    OverlayContainer* panelText;
    TextAreaOverlayElement* label1;
    TextAreaOverlayElement* label2;
    TextAreaOverlayElement* label3;
    TextAreaOverlayElement* label4;
    TextAreaOverlayElement* label5;
    TextAreaOverlayElement* label6;
    TextAreaOverlayElement* label7;
    
    PanelOverlayElement* toggleEntireBackground;
    PanelOverlayElement* toggle1Background;
    PanelOverlayElement* toggle2Background;
    PanelOverlayElement* toggle3Background;
    PanelOverlayElement* toggle4Background;
    PanelOverlayElement* toggle1TextArt;
    PanelOverlayElement* toggle2TextArt;
    PanelOverlayElement* toggle3TextArt;
    PanelOverlayElement* toggle4TextArt;
    PanelOverlayElement* toggleIndicator;
    TextAreaOverlayElement* toggle1Text;
    TextAreaOverlayElement* toggle2Text;
    TextAreaOverlayElement* toggle3Text;
    TextAreaOverlayElement* toggle4Text;
    
    PanelOverlayElement* powerup1Background;
    PanelOverlayElement* powerup2Background;
    PanelOverlayElement* powerup3Background;
    
    PanelOverlayElement* resumeButtonBackground;
    PanelOverlayElement* nextButtonBackground;
    PanelOverlayElement* restartButtonBackground;
    PanelOverlayElement* levelSelectButtonBackground;
    
    void link(Player* player, Tunnel* tunnel);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void setOverlay();
};

#endif /* defined(__Vinezors2_0__HudStage__) */
