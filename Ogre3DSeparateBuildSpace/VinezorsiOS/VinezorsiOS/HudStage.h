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
    void setPauseNavSettings(bool nextAvail, bool resumeAvail);
protected:
    Player* player;
    Tunnel* tunnel;
    
    enum ButtonSet {
        BUTTON_PAUSE, BUTTON_GO,
        BUTTON_RESUME, BUTTON_NEXT, BUTTON_RESTART, BUTTON_LEVELSELECT
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
    
    OverlayContainer* endTallyContainer;
    TextAreaOverlayElement* endTallyTimeLabel;
    TextAreaOverlayElement* endTallyTimeValue;
    TextAreaOverlayElement* endTallyScoreLabel;
    TextAreaOverlayElement* endTallyScoreValue;
    PanelOverlayElement* endTallyBackground;
    
    PanelOverlayElement* speedDisplayBackground;
    PanelOverlayElement* nbackDisplayBackground;
    TextAreaOverlayElement* nbackDisplayLabel;
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
    
    PanelOverlayElement* powerup1Background;
    PanelOverlayElement* powerup2Background;
    PanelOverlayElement* powerup3Background;
    
    float pauseNavOffset;
    float pauseNavOffsetDest;
    
    float goOffset;
    float goOffsetDest;
    PanelOverlayElement* pauseBaseBackground;
    PanelOverlayElement* pauseNavigationBackground;
    PanelOverlayElement* resumeButtonBackground;
    PanelOverlayElement* nextButtonBackground;
    PanelOverlayElement* restartButtonBackground;
    PanelOverlayElement* levelSelectButtonBackground;
    
    PanelOverlayElement* circleBackground;
    
    HudSlider* speedSlider;
    
    bool goButtonActive;
    
    bool bestScoreAnimationFlag;
    float bestScoreAnimationTimer;
    
    bool nextAvail;
    bool resumeAvail;
    
    void link(Player* player, Tunnel* tunnel);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();
    
    void setCollectionBar(bool instant, float elapsed);
};

#endif /* defined(__Vinezors2_0__HudStage__) */
