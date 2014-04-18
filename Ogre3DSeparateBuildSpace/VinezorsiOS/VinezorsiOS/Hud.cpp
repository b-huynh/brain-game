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

// Assigns pointer references to the button and initializes dimensions
void HudButton::setButton(std::string name, GuiMetricsMode metricMode, Vector2 pos, Vector2 dimension, PanelOverlayElement* bgPtr, TextAreaOverlayElement* txtPtr)
{
    this->name = name;
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
        textRef->setColour(ColourValue::ColourValue(0.0, 0.0, 0.0));
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
{
    player = NULL;
    tunnel = NULL;
    
    // The code snippet below is used to output text
    // create a font resource
    ResourcePtr resourceText = OgreFramework::getSingletonPtr()->m_pFontMgr->create("Arial",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    resourceText->setParameter("type", "truetype");
    resourceText->setParameter("source", "C64_User_Mono_v1.0-STYLE.ttf");
    resourceText->setParameter("size", "16");
    resourceText->setParameter("resolution", "96");
    resourceText->load();
    
    panelText = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TextInterface"));
    panelText->setMetricsMode(GMM_PIXELS);
    panelText->setPosition(10, 10);
    panelText->setDimensions(10, 10);
    
    healthArea = static_cast<BorderPanelOverlayElement*>(
                                                         OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("BorderPanel", "HealthAreaBorder"));
    
    barHP = static_cast<PanelOverlayElement*>(
                                              OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "HealthBar"));
    indicator = static_cast<PanelOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "Indicator"));
    pauseButton = static_cast<PanelOverlayElement*>(
                                                    OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "pauseButton"));
    
    // Create text area
    label1 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel1"));
    label2 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel2"));
    label3 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel3"));
    label4 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel4"));
    label5 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel5"));
    label6 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel6"));
    label7 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel7"));
    
    itemEntireBackground = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ItemEntireBackground"));
    // Note: These overlay elements are linked to a button, if we end up deleting these and NULLing it, it should be NULLed in the associated button as well
    item1Background = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "Item1Background"));
    item2Background = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "Item2Background"));
    item3Background = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "Item3Background"));
    item1Text = static_cast<TextAreaOverlayElement*>(
                                                     OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "Item1Text"));
    item2Text = static_cast<TextAreaOverlayElement*>(
                                                     OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "Item2Text"));
    item3Text = static_cast<TextAreaOverlayElement*>(
                                                     OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "Item3Text"));
    
    buttons = std::vector<HudButton>(4);
    
    // Create an overlay, and add the panel
    Overlay* overlay = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("EntireOverlay");
    overlay->add2D(healthArea);
    //overlay->add2D(barHP);
    overlay->add2D(indicator);
    overlay->add2D(pauseButton);
    
    overlay->add2D(itemEntireBackground);
    overlay->add2D(item1Background);
    overlay->add2D(item2Background);
    overlay->add2D(item3Background);
    overlay->add2D(panelText);
    panelText->addChild(label1);
    panelText->addChild(label2);
    panelText->addChild(label3);
    panelText->addChild(label4);
    panelText->addChild(label5);
    panelText->addChild(label6);
    panelText->addChild(label7);
    panelText->addChild(item1Text);
    panelText->addChild(item2Text);
    panelText->addChild(item3Text);
    overlays.push_back(overlay);
}

void Hud::unlink()
{
    this->tunnel = NULL;
    this->player = NULL;
}

void Hud::link(Tunnel* tunnel, Player* player)
{
    this->tunnel = tunnel;
    this->player = player;
}

void Hud::init(Tunnel* tunnel, Player* player)
{
    link(tunnel, player);
    setOverlay();
    if (tunnel->getMode() == GAME_PROFICIENCY)
    {
        healthArea->show();
        barHP->show();
        indicator->show();
        itemEntireBackground->show();
        item1Background->show();
        item2Background->show();
        item3Background->show();
        item1Text->show();
        item2Text->show();
        item3Text->show();
    }
    else
    {
        healthArea->hide();
        barHP->hide();
        indicator->hide();
        itemEntireBackground->hide();
        item1Background->hide();
        item2Background->hide();
        item3Background->hide();
        item1Text->hide();
        item2Text->hide();
        item3Text->hide();
    }
    
    for(int i = 0; i < overlays.size(); ++i)
        overlays[i]->show();
}

void Hud::setOverlay()
{
    panelText->setMetricsMode(GMM_PIXELS);
    panelText->setPosition(10, 10);
    panelText->setDimensions(10, 10);
    
    healthArea->setMetricsMode(GMM_RELATIVE);
    healthArea->setPosition(globals.HPBarXRef - 0.01, globals.HPBarYRef - 0.01);
    healthArea->setDimensions(globals.HPBarWidth + 0.02, globals.HPBarHeight + 0.02);
    barHP->setMetricsMode(GMM_RELATIVE);
    barHP->setPosition(globals.HPBarXRef, globals.HPBarYRef);
    barHP->setDimensions(0.0, globals.HPBarHeight);
    barHP->setMaterialName("General/BarColors");
    if (globals.HPPositiveLimit >= 8)
    {
        healthArea->setMaterialName("General/BarColors3");
        barHP->setDimensions(healthArea->getWidth() * (14.0 / 14.0), healthArea->getHeight());
    }
    else if (globals.HPPositiveLimit >= 5)
    {
        healthArea->setMaterialName("General/BarColors2");
        barHP->setDimensions(healthArea->getWidth() * (11.0 / 14.0), healthArea->getHeight());
    }
    else //if (globals.HPPositiveLimit >= 3)
    {
        healthArea->setMaterialName("General/BarColors1");
        barHP->setDimensions(healthArea->getWidth() * (9.0 / 14.0), healthArea->getHeight());
    }
    
    indicator->setMetricsMode(GMM_RELATIVE);
    indicator->setPosition(barHP->getLeft(), barHP->getTop() - 0.005);
    indicator->setDimensions(healthArea->getWidth() / 20, globals.HPBarHeight + 0.01);
    indicator->setMaterialName("General/Indicator");
    
    label1->setMetricsMode(GMM_PIXELS);
    label1->setAlignment(TextAreaOverlayElement::Center);
    label1->setPosition(globals.label1_posX, globals.label1_posY);
    label1->setCharHeight(globals.screenHeight / 40);
    label1->setFontName("Arial");
    label1->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    
    label2->setMetricsMode(GMM_PIXELS);
    label2->setPosition(globals.label2_posX, globals.label2_posY);
    label2->setCharHeight(globals.screenHeight / 50);
    label2->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label2->setFontName("Arial");
    
    label3->setMetricsMode(GMM_PIXELS);
    label3->setAlignment(TextAreaOverlayElement::Right);
    label3->setPosition(globals.label3_posX, globals.label3_posY);
    label3->setCharHeight(globals.screenHeight / 50);
    label3->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label3->setFontName("Arial");
    
    label4->setMetricsMode(GMM_PIXELS);
    label4->setPosition(globals.label4_posX, globals.label4_posY);
    label4->setCharHeight(globals.screenHeight / 50);
    label4->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label4->setFontName("Arial");
    
    label5->setMetricsMode(GMM_PIXELS);
    label5->setAlignment(TextAreaOverlayElement::Right);
    label5->setPosition(globals.label5_posX, globals.label5_posY);
    label5->setCharHeight(globals.screenHeight / 50);
    label5->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    label5->setFontName("Arial");
    
    label6->setMetricsMode(GMM_PIXELS);
    label6->setAlignment(TextAreaOverlayElement::Center);
    label6->setPosition(globals.label6_posX, globals.label6_posY);
    label6->setCharHeight(globals.screenHeight / 50);
    label6->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    label6->setFontName("Arial");
    
    label7->setMetricsMode(GMM_PIXELS);
    label7->setAlignment(TextAreaOverlayElement::Center);
    label7->setPosition(globals.label7_posX, globals.label7_posY);
    label7->setCharHeight(globals.screenHeight / 50);
    label7->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    label7->setFontName("Arial");
    
    buttons[0].setButton("pause", GMM_PIXELS, Vector2(globals.pauseButton_posX, globals.pauseButton_posY), Vector2(globals.pauseButton_width, globals.pauseButton_height), pauseButton, NULL);
    buttons[1].setButton("item1", GMM_RELATIVE, Vector2(0.80, 0.30), Vector2(0.15, 0.10), item1Background, item1Text);
    buttons[2].setButton("item2", GMM_RELATIVE, Vector2(0.80, 0.45), Vector2(0.15, 0.10), item2Background, item2Text);
    buttons[3].setButton("item3", GMM_RELATIVE, Vector2(0.80, 0.60), Vector2(0.15, 0.10), item3Background, item3Text);
    pauseButton->setMaterialName("General/PauseButton");
    
    itemEntireBackground->setMetricsMode(GMM_RELATIVE);
    itemEntireBackground->setPosition(0.775, 0.25);
    itemEntireBackground->setDimensions(0.20, 0.50);
    itemEntireBackground->setMaterialName("General/BaseGray");
}

void Hud::update(float elapsed)
{
    float timeLeft = fmax(globals.stageTime - tunnel->getTotalElapsed() - tunnel->getTimePenalty(), 0.0f);
    /*
     // Big Timer
    if (tunnel->getMode() == GAME_TIMED)
    {
        Ogre::ColourValue timeLeftCol = timeLeft <= 0.0 ? ColourValue(1.0, 1.0, 0.0) : ColourValue(0.0, 1.0, 0.0);
        label1->setColour(timeLeftCol);
        label1->setCaption(Util::toStringInt(timeLeft));
    }
    else
     */
    label1->setCaption(globals.messageBig);
    if (tunnel->getMode() == GAME_PROFICIENCY || tunnel->getMode() == GAME_TEACHING || tunnel->getMode() == GAME_RECESS)
    {
        Ogre::ColourValue fontColor = timeLeft <= 0.0 ? ColourValue(1.0, 0.0, 0.0) : ColourValue(1.0, 1.0, 1.0);
        label2->setColour(fontColor);
        label2->setCaption("Time: " + Util::toStringInt(timeLeft));
    }
    switch (tunnel->getPhase())
    {
        case 'A':
            label3->setCaption("Color/Sound " + Util::toStringInt(tunnel->getNBack()) + "-Back");
            break;
        case 'B':
            label3->setCaption("Shape/Sound " + Util::toStringInt(tunnel->getNBack()) + "-Back");
            break;
        case 'C':
            label3->setCaption("Sound " + Util::toStringInt(tunnel->getNBack()) + "-Back");
            break;
        case 'D':
            label3->setCaption("Navigation");
            break;
        case 'E':
            label3->setCaption("Color/Shape/Sound " + Util::toStringInt(tunnel->getNBack()) + "-Back");
            break;
        case 'F':
            label3->setCaption("Tutorial");
            break;
        case 'G':
            label3->setCaption("Recess");
            break;
        case 'H':
            label3->setCaption("Color/Shape/Sound " + Util::toStringInt(tunnel->getNBack()) + "-Back");
            break;
        default:
            label3->setCaption("");
            break;
    }
    label4->setCaption("Speed: " + Util::toStringInt(player->getFinalSpeed()));
    if (tunnel->getMode() == GAME_TEACHING || tunnel->getMode() == GAME_RECESS)
    {
        float percentComplete = static_cast<float>(player->getNumCorrectTotal()) / tunnel->getNumTargets() * 100;
        percentComplete = Util::clamp(percentComplete, 0.0, 100.0);
        label5->setCaption("Completed: " + Util::toStringInt(percentComplete) + "%");
    }
    else if (tunnel->getMode() == GAME_PROFICIENCY)
    {
        label5->setCaption("Score: " + Util::toStringInt(player->getScore()));
    }
    else
        label5->setCaption("");
    label6->setCaption(globals.message);
    label7->setCaption("");
    if (tunnel->getMode() == GAME_PROFICIENCY && tunnel->getNBack() > 0 && player->getShowCombo())
    {
        if (tunnel->getSpawnCombo() > 1)
            label7->setCaption("Combo" + Util::toStringInt(tunnel->getSpawnCombo() - 1));
    }
    
    // Set Progress Bar indicator position for the appropriate mode
    float barWidth = barHP->getWidth();
    if (tunnel->getMode() == GAME_PROFICIENCY)
    {
        float HPRange = globals.HPPositiveLimit - globals.HPNegativeLimit + 1;
        //indicator->setPosition(barHP->getLeft() + barWidth * player->getProgress(), indicator->getTop());
        indicator->setPosition(barHP->getLeft() + barWidth * (player->getHP() - globals.HPNegativeLimit) / HPRange, indicator->getTop());
    }
    
    // Update toggle buttons to reflect which N-Back setting is active
    switch (player->getToggleBack())
    {
        case 0:
            item1Background->setMaterialName("General/BaseYellow");
            item2Background->setMaterialName("General/BaseRed");
            item3Background->setMaterialName("General/BaseRed");
            break;
        case 1:
            item2Background->setMaterialName("General/BaseYellow");
            item1Background->setMaterialName("General/BaseRed");
            item3Background->setMaterialName("General/BaseRed");
            break;
        case 2:
            item3Background->setMaterialName("General/BaseYellow");
            item1Background->setMaterialName("General/BaseRed");
            item2Background->setMaterialName("General/BaseRed");
            break;
    }
    item1Text->setCaption(Util::toStringInt(tunnel->getNBack()) + "-Back");
    item2Text->setCaption(Util::toStringInt(Util::clamp(tunnel->getNBack() - 1, 0, tunnel->getNBack())) + "-Back");
    item3Text->setCaption(Util::toStringInt(Util::clamp(tunnel->getNBack() - 2, 0, tunnel->getNBack())) + "-Back");
}

std::string Hud::queryButtons(Vector2 target) const
{
    Vector2 comp = globals.convertToPercentScreen(target);
    for (int i = 0; i < buttons.size(); ++i)
    {
        if (buttons[i].isInside(comp))
            return buttons[i].name;
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

Hud::~Hud()
{
    //***** Need to properly deallocate
}
