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
#include "HudElements.h"

class Hud
{
public:
    Hud();
    virtual ~Hud();
    
    virtual void init() = 0;                // Initialize all elements
    virtual void adjust() = 0;              // For resizing windows, we have to readjust some items
    virtual void update(float elapsed) = 0; // Update certain animated items (timers, ect.)
    
    virtual std::string queryButtons(Vector2 target) const; // Returns any buttons containing target
    
    void setOverlay(int index, bool visible);
    void hideOverlays();
    void showOverlays();
protected:
    std::vector<Ogre::Overlay*> overlays;
    std::vector<HudButton> buttons;
    
    virtual void alloc() = 0;
    virtual void dealloc() = 0;
    virtual void initOverlay() = 0;
};

#endif /* defined(__Vinezors2_0__Hud__) */
