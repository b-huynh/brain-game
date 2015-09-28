//
//  HudInformation.h
//  VinezorsiOS
//
//  Created by BGC_Bernie on 3/18/15.
//
//

#ifndef VinezorsiOS_HudInformation_h
#define VinezorsiOS_HudInformation_h

#include "Hud.h"
#include "Util.h"

class HudInformation : public Hud
{
public:
    HudInformation(Player* player);
    virtual ~HudInformation();
    
    virtual void init();
    virtual void adjust();
    virtual void update(float elapsed);
    
    PanelOverlayElement* paragraphTextBackground;

    
protected:
    Player* player;
    
    static const int NUM_UNIQUE_BUTTONS = 4;
    enum Buttons {
        BUTTON_BACK, BUTTON_BGC_TOP_LINK, BUTTON_N_BACK_LINK, BUTTON_BGC_RECALL_LINK
    };
    
    
    
    // Overlay items for GAME_STATE_MENU
    PanelOverlayElement* informationBackdrop1;
    PanelOverlayElement* informationBackdrop2;
    PanelOverlayElement* informationTitleBackground;
    TextAreaOverlayElement* informationTitleText;
    
    PanelOverlayElement* backButtonBackground;
    
    
    
    TextAreaOverlayElement* gameTitleText;
    TextAreaOverlayElement* BGCTopLinkText;
    PanelOverlayElement* BGCTopLinkButtonBackground;

    TextAreaOverlayElement* nbackLinkText;
    PanelOverlayElement* nbackLinkButtonBackground;

    TextAreaOverlayElement* BGCRecallLinkText;
    PanelOverlayElement* BGCRecallLinkButtonBackground;

    
    TextAreaOverlayElement* paragraphText;




    

    
    void link(Player* player);
    void unlink();
    virtual void alloc();
    virtual void dealloc();
    virtual void initOverlay();

};

#endif
