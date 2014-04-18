//
//  Hud.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 11/24/13.
//
//

#ifndef __Vinezors2_0__Hud__
#define __Vinezors2_0__Hud__

#include <vector>
#include "Util.h"

struct HudButton
{
    std::string name;
    GuiMetricsMode metric;
    Vector2 p;
    Vector2 dim;
    
    PanelOverlayElement* backgroundRef;
    TextAreaOverlayElement* textRef;
    
    HudButton() : name(""), metric(), p(), dim(), backgroundRef(NULL), textRef(NULL) {}
    
    void setButton(std::string name, GuiMetricsMode metricMode, Vector2 pos, Vector2 dimension, PanelOverlayElement* bgPtr, TextAreaOverlayElement* txtPtr);
    bool isInside(Vector2 target) const;
};

class Hud
{
private:
    Player* player;
    Tunnel* tunnel;
    
    std::vector<Ogre::Overlay*> overlays;
    
    OverlayContainer* panelText;
    BorderPanelOverlayElement* healthArea;
    PanelOverlayElement* barHP;
    PanelOverlayElement* indicator;
    PanelOverlayElement* pauseButton;
    TextAreaOverlayElement* label1;
    TextAreaOverlayElement* label2;
    TextAreaOverlayElement* label3;
    TextAreaOverlayElement* label4;
    TextAreaOverlayElement* label5;
    TextAreaOverlayElement* label6;
    TextAreaOverlayElement* label7;
    
    PanelOverlayElement* itemEntireBackground;
    PanelOverlayElement* item1Background;
    PanelOverlayElement* item2Background;
    PanelOverlayElement* item3Background;
    TextAreaOverlayElement* item1Text;
    TextAreaOverlayElement* item2Text;
    TextAreaOverlayElement* item3Text;
    
    std::vector<HudButton> buttons;
public:
    Hud();
    void unlink();
    void link(Tunnel* tunnel, Player* player);
    void init(Tunnel* tunnel, Player* player);
    void setOverlay();
    void update(float elapsed);
    std::string queryButtons(Vector2 target) const;
    
    void hideOverlays();
    void showOverlays();
    
    ~Hud();
};

#endif /* defined(__Vinezors2_0__Hud__) */
