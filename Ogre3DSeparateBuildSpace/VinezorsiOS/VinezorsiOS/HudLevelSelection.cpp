//
//  HudLevelSelection.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#include "HudLevelSelection.h"
#include "Player.h"
#include "LevelSet.h"

HudLevelSelection::HudLevelSelection(Player* player)
: Hud()
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
    levelA8Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionA8Background"));
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
    levelB8Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionB8Background"));
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
    levelC8Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionC8Background"));
    backButtonBackground = static_cast<PanelOverlayElement*>(
                                                             OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionBackButtonBackground"));
    buttons = std::vector<HudButton>(25);
    
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
    overlay1->add2D(levelA8Background);
    overlay1->add2D(levelB1Background);
    overlay1->add2D(levelB2Background);
    overlay1->add2D(levelB3Background);
    overlay1->add2D(levelB4Background);
    overlay1->add2D(levelB5Background);
    overlay1->add2D(levelB6Background);
    overlay1->add2D(levelB7Background);
    overlay1->add2D(levelB8Background);
    overlay1->add2D(levelC1Background);
    overlay1->add2D(levelC2Background);
    overlay1->add2D(levelC3Background);
    overlay1->add2D(levelC4Background);
    overlay1->add2D(levelC5Background);
    overlay1->add2D(levelC6Background);
    overlay1->add2D(levelC7Background);
    overlay1->add2D(levelC8Background);
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
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelA8Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB1Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB2Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB3Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB4Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB5Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB6Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB7Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelB8Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC1Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC2Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC3Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC4Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC5Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC6Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC7Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelC8Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudLevelSelection::setOverlay()
{
    // Link and set resources
    levelSelectPrompt->setMetricsMode(GMM_RELATIVE);
    levelSelectPrompt->setPosition(0.10, 0.05);
    levelSelectPrompt->setDimensions(0.80, 0.20);
    levelSelectPrompt->setMaterialName("General/TextArtLevelSelection");
    
    // Assign a 2-D level selection map for each button
    setLevelButton(levelA1Background, 'A', 1);
    setLevelButton(levelA2Background, 'A', 2);
    setLevelButton(levelA3Background, 'A', 3);
    setLevelButton(levelA4Background, 'A', 4);
    setLevelButton(levelA5Background, 'A', 5);
    setLevelButton(levelA6Background, 'A', 6);
    setLevelButton(levelA7Background, 'A', 7);
    setLevelButton(levelA8Background, 'A', 8);
    setLevelButton(levelB1Background, 'B', 1);
    setLevelButton(levelB2Background, 'B', 2);
    setLevelButton(levelB3Background, 'B', 3);
    setLevelButton(levelB4Background, 'B', 4);
    setLevelButton(levelB5Background, 'B', 5);
    setLevelButton(levelB6Background, 'B', 6);
    setLevelButton(levelB7Background, 'B', 7);
    setLevelButton(levelB8Background, 'B', 8);
    setLevelButton(levelC1Background, 'C', 1);
    setLevelButton(levelC2Background, 'C', 2);
    setLevelButton(levelC3Background, 'C', 3);
    setLevelButton(levelC4Background, 'C', 4);
    setLevelButton(levelC5Background, 'C', 5);
    setLevelButton(levelC6Background, 'C', 6);
    setLevelButton(levelC7Background, 'C', 7);
    setLevelButton(levelC8Background, 'C', 8);
    
    backButtonBackground->setMaterialName("General/BackButton");
    
    buttons[BUTTON_LEVELA1].setButton("levelA1", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.30), Vector2(0.08, 0.08), levelA1Background, NULL);
    buttons[BUTTON_LEVELA2].setButton("levelA2", overlays[0], GMM_RELATIVE, Vector2(0.22, 0.30), Vector2(0.08, 0.08), levelA2Background, NULL);
    buttons[BUTTON_LEVELA3].setButton("levelA3", overlays[0], GMM_RELATIVE, Vector2(0.32, 0.30), Vector2(0.08, 0.08), levelA3Background, NULL);
    buttons[BUTTON_LEVELA4].setButton("levelA4", overlays[0], GMM_RELATIVE, Vector2(0.42, 0.30), Vector2(0.08, 0.08), levelA4Background, NULL);
    buttons[BUTTON_LEVELA5].setButton("levelA5", overlays[0], GMM_RELATIVE, Vector2(0.52, 0.30), Vector2(0.08, 0.08), levelA5Background, NULL);
    buttons[BUTTON_LEVELA6].setButton("levelA6", overlays[0], GMM_RELATIVE, Vector2(0.62, 0.30), Vector2(0.08, 0.08), levelA6Background, NULL);
    buttons[BUTTON_LEVELA7].setButton("levelA7", overlays[0], GMM_RELATIVE, Vector2(0.72, 0.30), Vector2(0.08, 0.08), levelA7Background, NULL);
    buttons[BUTTON_LEVELA8].setButton("levelA8", overlays[0], GMM_RELATIVE, Vector2(0.82, 0.30), Vector2(0.08, 0.08), levelA8Background, NULL);
    buttons[BUTTON_LEVELB1].setButton("levelB1", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.45), Vector2(0.08, 0.08), levelB1Background, NULL);
    buttons[BUTTON_LEVELB2].setButton("levelB2", overlays[0], GMM_RELATIVE, Vector2(0.22, 0.45), Vector2(0.08, 0.08), levelB2Background, NULL);
    buttons[BUTTON_LEVELB3].setButton("levelB3", overlays[0], GMM_RELATIVE, Vector2(0.32, 0.45), Vector2(0.08, 0.08), levelB3Background, NULL);
    buttons[BUTTON_LEVELB4].setButton("levelB4", overlays[0], GMM_RELATIVE, Vector2(0.42, 0.45), Vector2(0.08, 0.08), levelB4Background, NULL);
    buttons[BUTTON_LEVELB5].setButton("levelB5", overlays[0], GMM_RELATIVE, Vector2(0.52, 0.45), Vector2(0.08, 0.08), levelB5Background, NULL);
    buttons[BUTTON_LEVELB6].setButton("levelB6", overlays[0], GMM_RELATIVE, Vector2(0.62, 0.45), Vector2(0.08, 0.08), levelB6Background, NULL);
    buttons[BUTTON_LEVELB7].setButton("levelB7", overlays[0], GMM_RELATIVE, Vector2(0.72, 0.45), Vector2(0.08, 0.08), levelB7Background, NULL);
    buttons[BUTTON_LEVELB8].setButton("levelB8", overlays[0], GMM_RELATIVE, Vector2(0.82, 0.45), Vector2(0.08, 0.08), levelB8Background, NULL);
    buttons[BUTTON_LEVELC1].setButton("levelC1", overlays[0], GMM_RELATIVE, Vector2(0.12, 0.60), Vector2(0.08, 0.08), levelC1Background, NULL);
    buttons[BUTTON_LEVELC2].setButton("levelC2", overlays[0], GMM_RELATIVE, Vector2(0.22, 0.60), Vector2(0.08, 0.08), levelC2Background, NULL);
    buttons[BUTTON_LEVELC3].setButton("levelC3", overlays[0], GMM_RELATIVE, Vector2(0.32, 0.60), Vector2(0.08, 0.08), levelC3Background, NULL);
    buttons[BUTTON_LEVELC4].setButton("levelC4", overlays[0], GMM_RELATIVE, Vector2(0.42, 0.60), Vector2(0.08, 0.08), levelC4Background, NULL);
    buttons[BUTTON_LEVELC5].setButton("levelC5", overlays[0], GMM_RELATIVE, Vector2(0.52, 0.60), Vector2(0.08, 0.08), levelC5Background, NULL);
    buttons[BUTTON_LEVELC6].setButton("levelC6", overlays[0], GMM_RELATIVE, Vector2(0.62, 0.60), Vector2(0.08, 0.08), levelC6Background, NULL);
    buttons[BUTTON_LEVELC7].setButton("levelC7", overlays[0], GMM_RELATIVE, Vector2(0.72, 0.60), Vector2(0.08, 0.08), levelC7Background, NULL);
    buttons[BUTTON_LEVELC8].setButton("levelC8", overlays[0], GMM_RELATIVE, Vector2(0.82, 0.60), Vector2(0.08, 0.08), levelC8Background, NULL);
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

void HudLevelSelection::setLevelButton(PanelOverlayElement* levelBackground, char row, int no)
{
    LevelSet* levels = player->getLevels();
    int levelSelect = levels->getLevelNo(row, no);
    if (levels->hasLevel(levelSelect))
    {
        if (levelSelect < player->levelCompletion.size() && player->levelCompletion[levelSelect] > 0)
            levelBackground->setMaterialName("General/PauseButtonYellow");
        else
            levelBackground->setMaterialName("General/PauseButton");
    }
    else
        levelBackground->setMaterialName("General/PauseButtonGray");
    
}