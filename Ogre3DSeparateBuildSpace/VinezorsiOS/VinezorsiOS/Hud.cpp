//
//  Hud.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 11/24/13.
//
//

#include "Hud.h"
#include "Tunnel.h"
#include "Player.h"

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
        textRef->setFontName("Arial");
        textRef->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    }
}

// Determines whether the parameter is inside the button
bool HudButton::isInside(Vector2 target) const
{
    Vector2 check = p;
    if (metric == GMM_PIXELS)
    {
        check.x /= globals.screenWidth;
        check.y /= globals.screenHeight;
    }
    return (target.x >= check.x && target.x <= check.x + dim.x &&
            target.y >= check.y && target.y <= check.y + dim.y);
}

Hud::Hud()
: overlays(), buttons()
{
}

Hud::~Hud()
{
    // Deallocation happens in derived classes
}

void Hud::setOverlay(int index, bool visible)
{
    if (index < 0 || index >= overlays.size()) return;
    if (visible)
        overlays[index]->show();
    else
        overlays[index]->hide();
}

std::string Hud::queryButtons(Vector2 target) const
{
    Vector2 comp = globals.convertToPercentScreen(target);
    for (int i = 0; i < buttons.size(); ++i)
    {
        if ((buttons[i].overlay && buttons[i].overlay->isVisible()) && buttons[i].isInside(comp) && buttons[i].active)
        {
            return buttons[i].name;
        }
    }
    return "";
}

void Hud::hideOverlays()
{
    for (int i = 0; i < overlays.size(); ++i)
        overlays[i]->hide();
}

void Hud::showOverlays()
{
    for (int i = 0; i < overlays.size(); ++i)
        overlays[i]->show();
}

