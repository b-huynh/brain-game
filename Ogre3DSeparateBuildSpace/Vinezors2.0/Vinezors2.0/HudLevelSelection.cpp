//
//  HudLevelSelection.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#include "HudLevelSelection.h"

HudLevelSelection::HudLevelSelection(Player* player)
:Hud()
{
    link(player);
    init();
}

HudLevelSelection::~HudLevelSelection()
{
    dealloc();
    unlink();
}

void HudLevelSelection::init()
{
    alloc();
    setOverlay();
    showOverlays();
}

void HudLevelSelection::adjust()
{
    setOverlay();
}

void HudLevelSelection::update(float elapsed)
{
}

void HudLevelSelection::alloc()
{
    // Allocate Resources
    levelSelectPrompt = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectPrompt"));
    levelA1Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionA1Background"));
    levelA2Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionA2Background"));
    levelA3Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionA3Background"));
    levelA4Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionA4Background"));
    levelA5Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionA5Background"));
    levelA6Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionA6Background"));
    levelA7Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionA7Background"));
    levelB1Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionB1Background"));
    levelB2Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionB2Background"));
    levelB3Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionB3Background"));
    levelB4Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionB4Background"));
    levelB5Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionB5Background"));
    levelB6Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionB6Background"));
    levelB7Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionB7Background"));
    levelC1Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionC1Background"));
    levelC2Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionC2Background"));
    levelC3Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionC3Background"));
    levelC4Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionC4Background"));
    levelC5Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionC5Background"));
    levelC6Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionC6Background"));
    levelC7Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionC7Background"));
    backButtonBackground = static_cast<PanelOverlayElement*>(
                                                             OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionBackButtonBackground"));
    buttons = std::vector<HudButton>(22);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("LevelSelectionOverlay");
    overlay1->add2D(levelSelectPrompt);
    overlay1->add2D(levelA1Background);
    overlay1->add2D(levelA2Background);
    overlay1->add2D(levelA3Background);
    overlay1->add2D(levelA4Background);
    overlay1->add2D(levelA5Background);
    overlay1->add2D(levelA6Background);
    overlay1->add2D(levelA7Background);
    overlay1->add2D(levelB1Background);
    overlay1->add2D(levelB2Background);
    overlay1->add2D(levelB3Background);
    overlay1->add2D(levelB4Background);
    overlay1->add2D(levelB5Background);
    overlay1->add2D(levelB6Background);
    overlay1->add2D(levelB7Background);
    overlay1->add2D(levelC1Background);
    overlay1->add2D(levelC2Background);
    overlay1->add2D(levelC3Background);
    overlay1->add2D(levelC4Background);
    overlay1->add2D(levelC5Background);
    overlay1->add2D(levelC6Background);
    overlay1->add2D(levelC7Background);
    overlay1->add2D(backButtonBackground);
    overlays.push_back(overlay1);
}

void HudLevelSelection::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSelectPrompt);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelA1Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelA2Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelA3Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelA4Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelA5Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelA6Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelA7Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB1Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB2Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB3Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB4Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB5Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB6Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB7Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC1Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC2Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC3Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC4Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC5Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC6Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC7Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudLevelSelection::setOverlay()
{
    // Link and set resources
    levelSelectPrompt->setMetricsMode(GMM_RELATIVE);
    levelSelectPrompt->setPosition(0.10, 0.05);
    levelSelectPrompt->setDimensions(0.80, 0.20);
    levelSelectPrompt->setMaterialName("General/SelectYourLevel");
    
    levelA1Background->setMaterialName("General/PauseButton");
    levelA2Background->setMaterialName("General/PauseButton");
    levelA3Background->setMaterialName("General/PauseButton");
    levelA4Background->setMaterialName("General/PauseButton");
    levelA5Background->setMaterialName("General/PauseButton");
    levelA6Background->setMaterialName("General/PauseButton");
    levelA7Background->setMaterialName("General/PauseButton");
    levelB1Background->setMaterialName("General/PauseButtonGray");
    levelB2Background->setMaterialName("General/PauseButtonGray");
    levelB3Background->setMaterialName("General/PauseButtonGray");
    levelB4Background->setMaterialName("General/PauseButtonGray");
    levelB5Background->setMaterialName("General/PauseButtonGray");
    levelB6Background->setMaterialName("General/PauseButtonGray");
    levelB7Background->setMaterialName("General/PauseButtonGray");
    levelC1Background->setMaterialName("General/PauseButtonGray");
    levelC2Background->setMaterialName("General/PauseButtonGray");
    levelC3Background->setMaterialName("General/PauseButtonGray");
    levelC4Background->setMaterialName("General/PauseButtonGray");
    levelC5Background->setMaterialName("General/PauseButtonGray");
    levelC6Background->setMaterialName("General/PauseButtonGray");
    levelC7Background->setMaterialName("General/PauseButtonGray");
    backButtonBackground->setMaterialName("General/BackButton");
    
    buttons[BUTTON_LEVELA1].setButton("levelA1", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.30), Vector2(0.08, 0.08), levelA1Background, NULL);
    buttons[BUTTON_LEVELA2].setButton("levelA2", overlays[0], GMM_RELATIVE, Vector2(0.22, 0.30), Vector2(0.08, 0.08), levelA2Background, NULL);
    buttons[BUTTON_LEVELA3].setButton("levelA3", overlays[0], GMM_RELATIVE, Vector2(0.32, 0.30), Vector2(0.08, 0.08), levelA3Background, NULL);
    buttons[BUTTON_LEVELA4].setButton("levelA4", overlays[0], GMM_RELATIVE, Vector2(0.42, 0.30), Vector2(0.08, 0.08), levelA4Background, NULL);
    buttons[BUTTON_LEVELA5].setButton("levelA5", overlays[0], GMM_RELATIVE, Vector2(0.52, 0.30), Vector2(0.08, 0.08), levelA5Background, NULL);
    buttons[BUTTON_LEVELA6].setButton("levelA6", overlays[0], GMM_RELATIVE, Vector2(0.62, 0.30), Vector2(0.08, 0.08), levelA6Background, NULL);
    buttons[BUTTON_LEVELA7].setButton("levelA7", overlays[0], GMM_RELATIVE, Vector2(0.72, 0.30), Vector2(0.08, 0.08), levelA7Background, NULL);
    buttons[BUTTON_LEVELB1].setButton("levelB1", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.45), Vector2(0.08, 0.08), levelB1Background, NULL);
    buttons[BUTTON_LEVELB2].setButton("levelB2", overlays[0], GMM_RELATIVE, Vector2(0.22, 0.45), Vector2(0.08, 0.08), levelB2Background, NULL);
    buttons[BUTTON_LEVELB3].setButton("levelB3", overlays[0], GMM_RELATIVE, Vector2(0.32, 0.45), Vector2(0.08, 0.08), levelB3Background, NULL);
    buttons[BUTTON_LEVELB4].setButton("levelB4", overlays[0], GMM_RELATIVE, Vector2(0.42, 0.45), Vector2(0.08, 0.08), levelB4Background, NULL);
    buttons[BUTTON_LEVELB5].setButton("levelB5", overlays[0], GMM_RELATIVE, Vector2(0.52, 0.45), Vector2(0.08, 0.08), levelB5Background, NULL);
    buttons[BUTTON_LEVELB6].setButton("levelB6", overlays[0], GMM_RELATIVE, Vector2(0.62, 0.45), Vector2(0.08, 0.08), levelB6Background, NULL);
    buttons[BUTTON_LEVELB7].setButton("levelB7", overlays[0], GMM_RELATIVE, Vector2(0.72, 0.45), Vector2(0.08, 0.08), levelB7Background, NULL);
    buttons[BUTTON_LEVELC1].setButton("levelC1", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.60), Vector2(0.08, 0.08), levelC1Background, NULL);
    buttons[BUTTON_LEVELC2].setButton("levelC2", overlays[0], GMM_RELATIVE, Vector2(0.22, 0.60), Vector2(0.08, 0.08), levelC2Background, NULL);
    buttons[BUTTON_LEVELC3].setButton("levelC3", overlays[0], GMM_RELATIVE, Vector2(0.32, 0.60), Vector2(0.08, 0.08), levelC3Background, NULL);
    buttons[BUTTON_LEVELC4].setButton("levelC4", overlays[0], GMM_RELATIVE, Vector2(0.42, 0.60), Vector2(0.08, 0.08), levelC4Background, NULL);
    buttons[BUTTON_LEVELC5].setButton("levelC5", overlays[0], GMM_RELATIVE, Vector2(0.52, 0.60), Vector2(0.08, 0.08), levelC5Background, NULL);
    buttons[BUTTON_LEVELC6].setButton("levelC6", overlays[0], GMM_RELATIVE, Vector2(0.62, 0.60), Vector2(0.08, 0.08), levelC6Background, NULL);
    buttons[BUTTON_LEVELC7].setButton("levelC7", overlays[0], GMM_RELATIVE, Vector2(0.72, 0.60), Vector2(0.08, 0.08), levelC7Background, NULL);
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.20, 0.78), Vector2(0.30, 0.10), backButtonBackground, NULL);
}

void HudLevelSelection::link(Player* player)
{
    this->player = player;
}

void HudLevelSelection::unlink()
{
    this->player = NULL;
}