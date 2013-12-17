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

Hud::Hud()
{
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
    thresholdBottom = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "thresholdBottom"));
    thresholdTop = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "thresholdTop"));
    
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
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayHealthArea");
    Overlay* overlay2 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayText");
    overlay1->add2D(healthArea);
    //overlay1->add2D(barHP);
    overlay1->add2D(indicator);
    //overlay1->add2D(thresholdBottom);
    //overlay1->add2D(thresholdTop);
    overlay2->add2D(panelText);
    
    panelText->addChild(label1);
    panelText->addChild(label2);
    panelText->addChild(label3);
    panelText->addChild(label4);
    panelText->addChild(label5);
    panelText->addChild(label6);
    overlays.push_back(overlay1);
    overlays.push_back(overlay2);
}

void Hud::init(Tunnel* tunnel, Player* player)
{
    panelText->setMetricsMode(GMM_PIXELS);
    panelText->setPosition(10, 10);
    panelText->setDimensions(10, 10);
    
    healthArea->setMetricsMode(GMM_RELATIVE);
    healthArea->setPosition(globals.HPBarXRef - 0.01, globals.HPBarYRef - 0.01);
    healthArea->setDimensions(globals.HPBarWidth + 0.02, globals.HPBarHeight + 0.02);
    healthArea->setMaterialName("General/BarColors");
    //healthArea->setMaterialName("General/BaseBlack");
    
    barHP->setMetricsMode(GMM_RELATIVE);
    barHP->setPosition(globals.HPBarXRef, globals.HPBarYRef);
    barHP->setDimensions(0.0, globals.HPBarHeight);
    barHP->setMaterialName("General/BarColors");
    
    indicator->setMetricsMode(GMM_RELATIVE);
    indicator->setPosition(barHP->getLeft(), barHP->getTop() - 0.005);
    indicator->setDimensions(healthArea->getWidth() / 50, globals.HPBarHeight + 0.01);
    indicator->setMaterialName("General/Indicator");
    
    thresholdBottom->setMetricsMode(GMM_RELATIVE);
    thresholdBottom->setPosition(barHP->getLeft() + globals.stageDevanceThreshold, barHP->getTop() - 0.005);
    thresholdBottom->setDimensions(healthArea->getWidth() / 50, globals.HPBarHeight + 0.01);
    thresholdBottom->setMaterialName("General/Indicator");
    
    thresholdTop->setMetricsMode(GMM_RELATIVE);
    thresholdTop->setPosition(barHP->getLeft() + globals.stageAdvanceThreshold, barHP->getTop() - 0.005);
    thresholdTop->setDimensions(healthArea->getWidth() / 50, globals.HPBarHeight + 0.01);
    thresholdTop->setMaterialName("General/Indicator");
    
    label1->setMetricsMode(GMM_PIXELS);
    label1->setAlignment(TextAreaOverlayElement::Center);
    label1->setPosition(globals.label1_posX, globals.label1_posY);
    label1->setCharHeight(globals.screenHeight / 30);
    label1->setFontName("Arial");
    label1->setColour(ColourValue::Green);
    
    label2->setMetricsMode(GMM_PIXELS);
    label2->setPosition(globals.label2_posX, globals.label2_posY);
    label2->setCharHeight(globals.screenHeight / 50);
    label2->setColour(ColourValue::White);
    label2->setFontName("Arial");
    
    label3->setMetricsMode(GMM_PIXELS);
    label3->setAlignment(TextAreaOverlayElement::Right);
    label3->setPosition(globals.label3_posX, globals.label3_posY);
    label3->setCharHeight(globals.screenHeight / 50);
    label3->setColour(ColourValue::White);
    label3->setFontName("Arial");
    
    label4->setMetricsMode(GMM_PIXELS);
    label4->setPosition(globals.label4_posX, globals.label4_posY);
    label4->setCharHeight(globals.screenHeight / 50);
    label4->setColour(ColourValue::White);
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
    
    if (tunnel->getMode() == GAME_TIMED)
    {
        healthArea->hide();
        barHP->hide();
        indicator->hide();
        thresholdBottom->hide();
        thresholdTop->hide();
    }
    else
    {
        healthArea->show();
        barHP->show();
        indicator->show();
        thresholdBottom->show();
        thresholdTop->show();
    }
    
    for(int i = 0; i < overlays.size(); ++i)
        overlays[i]->show();
}

void Hud::update(Tunnel* tunnel, Player* player, double elapsed)
{
    if (tunnel->getMode() == GAME_TIMED)
        label1->setCaption(Util::toStringInt(std::max(globals.timedRunTimer - tunnel->getTotalElapsed(), 0.0)));
    else
        label1->setCaption("");
    if (tunnel->getMode() == GAME_TIMED)
        //label2->setCaption("Distance: " + Util::toStringInt(player->getScore()));
        label2->setCaption("");
    else
    {
        label2->setCaption("");
        // Not for the 20th deadline
        //if (player->getNumCorrectBonus() <= 0)
        //    label2->setCaption("Points: " + Util::toStringInt(player->getPoints()));
        //else
        //    label2->setCaption("Points: " + Util::toStringInt(player->getPoints()) + " + " + Util::toStringInt(player->getNumCorrectBonus()));
    }
    label3->setCaption(Util::toStringInt(tunnel->getNBack()) + "-Back");
    label4->setCaption("Speed: " + Util::toStringInt(player->getCamSpeed()));
    label5->setCaption("Signals: " + Util::toStringInt(tunnel->getSignalsLeft()));
    label6->setCaption(globals.message);
    
    double indicatorRange = barHP->getWidth();
    double barWidth = globals.HPBarWidth;
    // As accuracy bar
    if (tunnel->getMode() != GAME_TIMED)
    {
        barHP->setDimensions(barWidth, globals.HPBarHeight);
        indicator->setPosition(barHP->getLeft() + player->getProgress(tunnel), indicator->getTop());
    }
    /*
    // As HP counter
    if (tunnel->getMode() != GAME_TIMED)
    {
        barHP->setDimensions(barWidth, globals.HPBarHeight);
        int hpRange = globals.HPPositiveLimit - globals.HPNegativeLimit;
        indicatorRange *= (player->getHP() - globals.HPNegativeLimit) / (double)(hpRange);
        indicator->setPosition(barHP->getLeft() + indicatorRange, indicator->getTop());
    }
     */
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
