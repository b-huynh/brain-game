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
    threshold1 = static_cast<PanelOverlayElement*>(
                                                   OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "threshold1"));
    threshold2 = static_cast<PanelOverlayElement*>(
                                                   OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "threshold2"));
    threshold3 = static_cast<PanelOverlayElement*>(
                                                   OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "threshold3"));
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
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayHealthArea");
    Overlay* overlay2 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayText");
    //overlay1->add2D(healthArea);
    //overlay1->add2D(barHP);
    //overlay1->add2D(indicator);
    //overlay1->add2D(threshold1);
    //overlay1->add2D(threshold2);
    //overlay1->add2D(threshold3);
    overlay1->add2D(pauseButton);
    overlay2->add2D(panelText);
    
    panelText->addChild(label1);
    //panelText->addChild(label2);
    panelText->addChild(label3);
    panelText->addChild(label4);
    panelText->addChild(label5);
    panelText->addChild(label6);
    panelText->addChild(label7);
    overlays.push_back(overlay1);
    overlays.push_back(overlay2);
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
    if (tunnel->getMode() == GAME_TIMED)
    {
        healthArea->hide();
        barHP->hide();
        indicator->hide();
    }
    else if (tunnel->getMode() == GAME_PROFICIENCY)
    {
        healthArea->show();
        barHP->show();
        indicator->show();
    }
    else //if (tunnel->getMode() == GAME_NAVIGATION)
    {
        healthArea->hide();
        barHP->hide();
        indicator->hide();
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
    
    threshold1->setMetricsMode(GMM_RELATIVE);
    threshold1->setDimensions(healthArea->getWidth() / 15, globals.HPBarHeight + 0.01);
    threshold1->setMaterialName("General/StarGray");
    
    threshold2->setMetricsMode(GMM_RELATIVE);
    threshold2->setDimensions(healthArea->getWidth() / 15, globals.HPBarHeight + 0.01);
    threshold2->setMaterialName("General/StarGray");
    
    threshold3->setMetricsMode(GMM_RELATIVE);
    threshold3->setDimensions(healthArea->getWidth() / 15, globals.HPBarHeight + 0.01);
    threshold3->setMaterialName("General/StarGray");
    
    pauseButton->setMetricsMode(GMM_PIXELS);
    pauseButton->setPosition(globals.pauseButton_posX, globals.pauseButton_posY);
    pauseButton->setDimensions(globals.pauseButton_width, globals.pauseButton_height);
    pauseButton->setMaterialName("General/PauseButton");
    
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
    if (tunnel->getMode() == GAME_PROFICIENCY || tunnel->getMode() == GAME_TIMED || tunnel->getMode() == GAME_TEACHING || tunnel->getMode() == GAME_RECESS)
    {
        Ogre::ColourValue fontColor = timeLeft <= 0.0 ? ColourValue(1.0, 0.0, 0.0) : ColourValue(1.0, 1.0, 1.0);
        label4->setColour(fontColor);
        label4->setCaption("Time: " + Util::toStringInt(timeLeft));
    }
    else if (tunnel->getMode() == GAME_NAVIGATION)
    {
        Ogre::ColourValue fontColor = player->getHP() <= globals.HPNegativeLimit ? Ogre::ColourValue(1.0, 0.0, 0.0) : Ogre::ColourValue(1.0, 1.0, 1.0);
        label4->setColour(fontColor);
        label4->setCaption("Chances: " + Util::toStringInt(player->getHP()));
    }
    label3->setCaption(Util::toStringInt(tunnel->getNBack()) + "-Back");
    label2->setCaption("Speed: " + Util::toStringInt(player->getFinalSpeed()));
    if (tunnel->getMode() == GAME_TIMED)
    {
        float percentComplete = static_cast<float>((tunnel->getSpawnLimit() - tunnel->getSignalsLeft())) / tunnel->getSpawnLimit() * 100;
        percentComplete = Util::clamp(percentComplete, 0.0, 100.0);
        label5->setCaption("Completed: " + Util::toStringInt(percentComplete) + "%");
        //label5->setCaption("Signals: " + Util::toStringInt(tunnel->getSignalsLeft()));
    }
    else if (tunnel->getMode() == GAME_TEACHING || tunnel->getMode() == GAME_RECESS)
    {
        float percentComplete = static_cast<float>(player->getNumCorrectTotal()) / tunnel->getNumTargets() * 100;
        percentComplete = Util::clamp(percentComplete, 0.0, 100.0);
        label5->setCaption("Completed: " + Util::toStringInt(percentComplete) + "%");
    }
    else if (tunnel->getMode() == GAME_NAVIGATION)
    {
        float percentComplete = static_cast<float>(player->getTotalElapsed()) / globals.stageTime * 100;
        percentComplete = Util::clamp(percentComplete, 0.0, 100.0);
        label5->setCaption("Completed: " + Util::toStringInt(percentComplete) + "%");
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
    
    float barWidth = barHP->getWidth();
    // Set UI positions depending on game mode
    if (tunnel->getMode() == GAME_PROFICIENCY)
    {
        float HPRange = globals.HPPositiveLimit - globals.HPNegativeLimit + 1;
        //indicator->setPosition(barHP->getLeft() + barWidth * player->getProgress(), indicator->getTop());
        indicator->setPosition(barHP->getLeft() + barWidth * (player->getHP() - globals.HPNegativeLimit) / HPRange, indicator->getTop());
    }
    
    threshold1->setMaterialName("General/StarGold");
    threshold2->setMaterialName("General/StarGold");
    threshold3->setMaterialName("General/StarGold");
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
