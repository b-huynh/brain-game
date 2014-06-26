//
//  HudElements.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 5/16/14.
//
//

#include "HudElements.h"

extern Util::ConfigGlobal globals;

void HudButton::setActive(bool value)
{
    active = value;
}

void HudButton::hide()
{
    setActive(false);
    if (backgroundRef)
        backgroundRef->hide();
    if (textRef)
        textRef->hide();
}

void HudButton::show()
{
    setActive(true);
    if (backgroundRef)
        backgroundRef->show();
    if (textRef)
        textRef->show();
}

// Assigns pointer references to the button and initializes dimensions
void HudButton::setButton(std::string name, Overlay* olay, GuiMetricsMode metricMode, Vector2 pos, Vector2 dimension, PanelOverlayElement* bgPtr, TextAreaOverlayElement* txtPtr)
{
    this->name = name;
    overlay = olay;
    p = pos;
    dim = dimension;
    metric = metricMode;
    backgroundRef = bgPtr;
    if (backgroundRef)
    {
        backgroundRef->setMetricsMode(metric);
        backgroundRef->setPosition(pos.x, pos.y);
        backgroundRef->setDimensions(dim.x, dim.y);
    }
    textRef = txtPtr;
    if (textRef)
    {
        textRef->setMetricsMode(metric);
        textRef->setAlignment(TextAreaOverlayElement::Center);
        textRef->setCharHeight(1.0f / 40.0f);
        textRef->setPosition(pos.x + dim.x / 2, pos.y + dim.y / 2);
        textRef->setFontName("MainSmall");
        textRef->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    }
}

// Determines whether the parameter is inside the button
bool HudButton::isInside(Vector2 target) const
{
    Vector2 check = p;
    
    // For inherited positions
    if (backgroundRef && backgroundRef->getParent())
        check = p + Vector2(backgroundRef->getParent()->_getDerivedLeft(),
                            backgroundRef->getParent()->_getDerivedTop());
    
    if (metric == GMM_PIXELS)
    {
        check.x /= globals.screenWidth;
        check.y /= globals.screenHeight;
    }
    
    return (target.x >= check.x && target.x <= check.x + dim.x &&
            target.y >= check.y && target.y <= check.y + dim.y);
}

void HudSlider::setSlider(std::string name, Overlay* olay, Vector2 pos1, Vector2 dimension1, Vector2 dimension2, bool orient, int min, int max, int slots, PanelOverlayElement* range, PanelOverlayElement* ball)
{
    this->name = name;
    overlay = olay;
    p1 = pos1;
    p2 = Vector2::ZERO;
    p2cache = p2;
    p2dest = p2;
    orientation = orient;
    dim1 = dimension1;
    dim2 = dimension2;
    this->min = min;
    this->max = max;
    this->slots = slots;
    metric = GMM_RELATIVE;
    rangeRef = range;
    if (rangeRef)
    {
        rangeRef->setMetricsMode(metric);
        rangeRef->setPosition(pos1.x, pos1.y);
        rangeRef->setDimensions(dim1.x, dim1.y);
    }
    ballRef = ball;
    if (ballRef)
    {
        ballRef->setMetricsMode(metric);
        ballRef->setPosition(0, 0);
        ballRef->setDimensions(dim2.x, dim2.y);
    }
}

// Determines whether the parameter is inside the slider range
bool HudSlider::isInsideRange(Vector2 target) const
{
    Vector2 check = p1;
    if (metric == GMM_PIXELS)
    {
        check.x /= globals.screenWidth;
        check.y /= globals.screenHeight;
    }
    if (orientation)
        return (target.x >= check.x && target.x <= check.x + dim1.x &&
                target.y >= check.y && target.y <= check.y + dim1.y + dim2.y / 2);
    else
        return (target.x >= check.x && target.x <= check.x + dim1.x + dim2.x / 2 &&
                target.y >= check.y && target.y <= check.y + dim1.y);
}

// Determines whether the parameter is inside the button
bool HudSlider::isInsideBall(Vector2 target) const
{
    Vector2 check = p1 + p2;
    if (metric == GMM_PIXELS)
    {
        check.x /= globals.screenWidth;
        check.y /= globals.screenHeight;
    }
    return (target.x >= check.x && target.x <= check.x + dim2.x &&
            target.y >= check.y && target.y <= check.y + dim2.y);
}

// Adjusts the slider mainly for switching screens
void HudSlider::adjust()
{
    float rheight;
    float rwidth;
    float sheight;
    float swidth;
    if (globals.screenWidth > globals.screenHeight)
    {
        rheight = dim2.y;
        rwidth = dim2.x;
        // to square out
        //rwidth = rheight * globals.screenHeight / globals.screenWidth;
        if (orientation)
        {
            swidth = rwidth;
            sheight = dim1.y;
        }
        else
        {
            swidth = dim1.x;
            sheight = rheight;
        }
    }
    else
    {
        rwidth = dim2.x;
        rheight = dim2.y;
        //rheight = rwidth * globals.screenWidth / globals.screenHeight;
        if (orientation)
        {
            swidth = rwidth;
            sheight = dim1.y;
        }
        else
        {
            swidth = dim1.x;
            sheight = rheight;
        }
    }
    //dim1 = Vector2(swidth, sheight);
    //dim2 = Vector2(rwidth, rheight);
    rangeRef->setDimensions(swidth, sheight);
    ballRef->setDimensions(rwidth, rheight);
    
}

// Returns the range of the slider in HUD coordinates
float HudSlider::getRangeWidth() const
{
    if (orientation)
        return rangeRef->getHeight() - ballRef->getHeight();
    else
        return rangeRef->getWidth() - ballRef->getWidth();
}

// Sets the ball's position bounded to the range of the slider
void HudSlider::setBallPosition(Vector2 value)
{
    if (orientation)
        p2 = Vector2(value.x, Util::clamp(value.y, 0.0, getRangeWidth()));
    else
        p2 = Vector2(Util::clamp(value.x, 0.0, getRangeWidth()), value.y);
    p2dest = p2;
    ballRef->setPosition(p2.x, p2.y);
}

// Set the ball position at the specified slot
void HudSlider::setBallPosition(int slot)
{
    Vector2 pos;
    if (orientation)
        pos = Vector2(p2.x, (max - slot) * (getRangeWidth()) / (slots - 1));
    else
        pos = Vector2((slot - min) * (getRangeWidth()) / (slots - 1), p2.y);
    setBallPosition(pos);
}

// Returns the ball's position
Vector2 HudSlider::getBallPosition() const
{
    return p2;
}

// set the destination so that p2pos animates its way to p2dest
void HudSlider::setBallDestination(Vector2 value)
{
    if (orientation)
        p2dest = Vector2(value.x, Util::clamp(value.y, 0.0, getRangeWidth()));
    else
        p2dest = Vector2(Util::clamp(value.x, 0.0, getRangeWidth()), value.y);
}

void HudSlider::setBallDestination(int slot)
{
    Vector2 pos;
    if (orientation)
        pos = Vector2(p2.x, (max - slot) * (getRangeWidth()) / (slots - 1));
    else
        pos = Vector2((slot - min) * (getRangeWidth()) / (slots - 1), p2.y);
    setBallDestination(pos);
}

// Returns a discrete index based on the ball's position
int HudSlider::getIndex() const
{
    if (orientation)
        return max - p2.y / getRangeWidth() * (slots - 1) + Util::EPSILON;
    else
        return p2.x / getRangeWidth() * (slots - 1) + min + Util::EPSILON;
}

int HudSlider::getRange() const
{
    return slots;
}

int HudSlider::getMin() const
{
    return min;
}

int HudSlider::getMax() const
{
    return max;
}

// The following deal with injecting coordinates to simulate a slider
//
void HudSlider::activateMoved(float x, float y, float dx, float dy)
{
    Vector2 np;
#if !defined(OGRE_IS_IOS)
    if (orientation)
        np = p2 + globals.convertToPercentScreen(Vector2(0.0, dy));
    else
        np = p2 + globals.convertToPercentScreen(Vector2(dx, 0.0));
#else
    if (orientation)
        np = p2cache + globals.convertToPercentScreen(Vector2(0.0, dy));
    else
        np = p2cache + globals.convertToPercentScreen(Vector2(dx, 0.0));
#endif
    setBallPosition(np);
            
    //std::cout << "move " << p2.x << " " << globals.convertToPercentScreen(Vector2(dx, 0.0)).x << std::endl;;
}

void HudSlider::activatePressed(float x, float y)
{
    Vector2 pos = globals.convertToPercentScreen(Vector2(x, y));
            
    // Case for tapping into the slider which makes ball jump to position
    if (isInsideRange(pos))
    {
        // Project position to range
        if (orientation)
        {
            float ny = pos.y - p1.y - dim2.y / 2;
            setBallPosition(Vector2(p2.x, ny));
        }
        else
        {
            float nx = pos.x - p1.x - dim2.x / 2;
            setBallPosition(Vector2(nx, p2.y));
        }
    }
            
    // Initialize p2cache first time we touch ball
    // p2cache is better since iOS tracking uses total dx which is more accurate
    // then passing in a dx everytime
    if (isInsideBall(pos))
    {
        selected = true;
        p2cache = p2;
    }
    //std::cout << "press\n";
}

void HudSlider::activateReleased(float x, float y, float dx, float dy)
{
    selected = false;
    Vector2 np;
#if !defined(OGRE_IS_IOS)
    if (orientation)
        np = p2 + globals.convertToPercentScreen(Vector2(0.0, dy));
    else
        np = p2 + globals.convertToPercentScreen(Vector2(dx, 0.0));
#else
    if (orientation)
        np = p2cache + globals.convertToPercentScreen(Vector2(0.0, dy));
    else
        np = p2cache + globals.convertToPercentScreen(Vector2(dx, 0.0));
#endif
    setBallPosition(np);
    //std::cout << "release\n";
}

void HudSlider::update(float elapsed)
{
    Vector2 dir = (p2dest - p2) * 10;
    if (dir.squaredLength() >= 0.001)
    {
        p2 += (dir * elapsed);
        ballRef->setPosition(p2.x, p2.y);
    }
}

