//
//  HudElements.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 5/16/14.
//
//

#ifndef __Vinezors2_0__HudElements__
#define __Vinezors2_0__HudElements__

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
    bool orientation; // false is horizontal, true is vertical
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
    
    HudSlider() : name(""), overlay(NULL), metric(), orientation(false), min(0), max(0), slots(0), p1(), dim1(), dim2(), rangeRef(NULL), ballRef(NULL), p2cache(), selected(false) {}
    
    void setSlider(std::string name, Overlay* olay, Vector2 pos1, Vector2 dimension1, Vector2 dimension2, bool orient, int min, int max, int slots, PanelOverlayElement* range, PanelOverlayElement* ball);
    bool isInsideRange(Vector2 target) const;
    bool isInsideBall(Vector2 target) const;
    void adjust();
    float getRangeWidth() const;
    void setBallPosition(Vector2 value);
    void setBallPosition(int slot);
    Vector2 getBallPosition() const;
    int getIndex() const;
};

#endif /* defined(__Vinezors2_0__HudElements__) */
