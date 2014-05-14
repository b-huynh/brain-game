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
    PanelOverlayElement* threshold1;
    PanelOverlayElement* threshold2;
    PanelOverlayElement* threshold3;
    PanelOverlayElement* pauseButton;
    TextAreaOverlayElement* label1;
    TextAreaOverlayElement* label2;
    TextAreaOverlayElement* label3;
    TextAreaOverlayElement* label4;
    TextAreaOverlayElement* label5;
    TextAreaOverlayElement* label6;
    TextAreaOverlayElement* label7;
public:
    Hud();
    void unlink();
    void link(Tunnel* tunnel, Player* player);
    void init(Tunnel* tunnel, Player* player);
    void setOverlay();
    void update(float elapsed);
    
    void hideOverlays();
    void showOverlays();
    
    ~Hud();
};

#endif /* defined(__Vinezors2_0__Hud__) */
