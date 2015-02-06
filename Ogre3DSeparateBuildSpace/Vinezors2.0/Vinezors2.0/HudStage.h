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
    
    HudSlider* getSpeedSlider() const { return speedSlider; }
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
    
    // Used to change the background of the go button
    bool isGoButtonActive() const;
    void setGoButtonState(bool active, bool color = true);
    void setPauseNavDest(float navOffset);
    void setSpeedDialState(bool active);
    void setPauseNavSettings(bool nextAvail, bool resumeAvail, bool forwardAvail, bool backAvail);
    
    float leftZapT;
    float rightZapT;
protected:
    Player* player;
    Tunnel* tunnel;
    
    enum ButtonSet {
        BUTTON_PAUSE, BUTTON_GO,
        BUTTON_RESUME, BUTTON_NEXT, BUTTON_RESTART, BUTTON_LEVELSELECT,
        BUTTON_LEFTZAPPER, BUTTON_RIGHTZAPPER
    };
    
    // Overlay items for GAME_STATE_PLAY
    BorderPanelOverlayElement* healthArea;
    PanelOverlayElement* barHP;
    PanelOverlayElement* indicator;
    PanelOverlayElement* pauseBackground;
    
    PanelOverlayElement* goBaseBackground;
    PanelOverlayElement* goBackground;
    PanelOverlayElement* sliderRangeBackground;
    PanelOverlayElement* sliderBallBackground;
    
    std::vector<PanelOverlayElement*> collectionBar;
    PanelOverlayElement* HudEntire;
    PanelOverlayElement* HudTopPanel;
    PanelOverlayElement* HudLeftPanel;
    PanelOverlayElement* HudRightPanel;
    PanelOverlayElement* HudLeftZapper;
    PanelOverlayElement* HudRightZapper;
    //std::vector<PanelOverlayElement*> HudFuelContainers;
    //std::vector<PanelOverlayElement*> HudFuelBars;
    PanelOverlayElement* HudFuelBar;
    OverlayContainer* panelText;
    TextAreaOverlayElement* label1;
    TextAreaOverlayElement* label2;
    TextAreaOverlayElement* label2prompt;
    TextAreaOverlayElement* label3;
    TextAreaOverlayElement* label4;
    TextAreaOverlayElement* label5;
    TextAreaOverlayElement* label5prompt;
    TextAreaOverlayElement* label6;
    TextAreaOverlayElement* label7;
    
    OverlayContainer* timeWarpContainer;
    TextAreaOverlayElement* timeWarpLabel;
    
    PanelOverlayElement* speedDisplayBackground;
    PanelOverlayElement* nbackDisplayBackground;
    TextAreaOverlayElement* nbackDisplayLabel;
    
    float pauseNavOffset;
    float pauseNavOffsetDest;
    
    float goOffset;
    float goOffsetDest;
    PanelOverlayElement* pauseBaseBackground;
    PanelOverlayElement* pauseNavigationBackground;
    PanelOverlayElement* pausePanelPlay;
    PanelOverlayElement* pausePanelForward;
    PanelOverlayElement* pausePanelBack;
    PanelOverlayElement* pausePanelMenu;
    PanelOverlayElement* resumeButtonBackground;
    PanelOverlayElement* nextButtonBackground;
    PanelOverlayElement* restartButtonBackground;
    PanelOverlayElement* levelSelectButtonBackground;
    
    PanelOverlayElement* leftZapperButtonBackground;
    PanelOverlayElement* rightZapperButtonBackground;
    
    PanelOverlayElement* circleBackground;
    
    HudSlider* speedSlider;
    
    bool goButtonActive;
    
    float prevFuelTimer;
    float fuelBarAnimationTimer;
    
    void link(Player* player, Tunnel* tunnel);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
    
    void setCollectionBar(bool instant, float elapsed);
    void setFuelBar(float elapsed);
};

#endif /* defined(__Vinezors2_0__HudStage__) */
