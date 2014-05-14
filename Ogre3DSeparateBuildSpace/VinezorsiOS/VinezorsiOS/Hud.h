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
    bool active;    // Button is active for selection
    
    PanelOverlayElement* backgroundRef;
    TextAreaOverlayElement* textRef;
    
    HudButton() : name(""), overlay(NULL), metric(), p(), dim(), backgroundRef(NULL), textRef(NULL), active(true) {}
    
    void setActive(bool value);
    void hide();
    void show();
    void setButton(std::string name, Overlay* olay, GuiMetricsMode metricMode, Vector2 pos, Vector2 dimension, PanelOverlayElement* bgPtr, TextAreaOverlayElement* txtPtr);
    bool isInside(Vector2 target) const;
};

struct HudSlider
{
    std::string name;
    Overlay* overlay;
    GuiMetricsMode metric;
    int min;
    int max;
    int slots;
    
    // Orientation of slider
    Vector2 p1;
    Vector2 dim1;
    
    // Local orientation of ball
    Vector2 p2;
    Vector2 dim2;
    
    PanelOverlayElement* rangeRef;
    PanelOverlayElement* ballRef;
    
    Vector2 p2cache; // transition
    
    bool selected;
    
    HudSlider() : name(""), overlay(NULL), metric(), min(0), max(0), slots(0), p1(), dim1(), dim2(), rangeRef(NULL), ballRef(NULL), p2cache(), selected(false) {}
    
    void setSlider(std::string name, Overlay* olay, Vector2 pos1, Vector2 dimension1, Vector2 dimension2, int min, int max, int slots, PanelOverlayElement* range, PanelOverlayElement* ball);
    bool isInsideRange(Vector2 target) const;
    bool isInsideBall(Vector2 target) const;
    void adjust();
    float getRangeWidth() const;
    void setBallPosition(Vector2 value);
    void setBallPosition(int slot);
    Vector2 getBallPosition() const;
    int getIndex() const;
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
