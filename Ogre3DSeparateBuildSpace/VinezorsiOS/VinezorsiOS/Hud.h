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
    Overlay* overlay;
    GuiMetricsMode metric;
    Vector2 p;
    Vector2 dim;
    
    PanelOverlayElement* backgroundRef;
    TextAreaOverlayElement* textRef;
    
    HudButton() : name(""), overlay(NULL), metric(), p(), dim(), backgroundRef(NULL), textRef(NULL) {}
    
    void setButton(std::string name, Overlay* olay, GuiMetricsMode metricMode, Vector2 pos, Vector2 dimension, PanelOverlayElement* bgPtr, TextAreaOverlayElement* txtPtr);
    bool isInside(Vector2 target) const;
};

class Hud
{
public:
    Hud();
    virtual ~Hud();
    
    virtual void init() = 0;                // Initialize all elements
    virtual void adjust() = 0;              // For resizing windows, we have to readjust some items
    virtual void update(float elapsed) = 0; // Update certain animated items (timers, ect.)
    
    std::string queryButtons(Vector2 target) const; // Returns any buttons containing target
    
    void hideOverlays();
    void showOverlays();
protected:
    std::vector<Ogre::Overlay*> overlays;
    std::vector<HudButton> buttons;
    
    virtual void alloc() = 0;
    virtual void dealloc() = 0;
    virtual void setOverlay() = 0;
};

#endif /* defined(__Vinezors2_0__Hud__) */
