//
//  HudMainMenu.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#include "HudMainMenu.h"
#include "Player.h"

HudMainMenu::HudMainMenu(Player* player)
:Hud()
{
    link(player);
    init();
}

HudMainMenu::~HudMainMenu()
{
    dealloc();
    unlink();
}

void HudMainMenu::init()
{
    alloc();
    setOverlay();
    showOverlays();
}

void HudMainMenu::adjust()
{
    setOverlay();
}

void HudMainMenu::update(float elapsed)
{
}

void HudMainMenu::alloc()
{
    // Allocate Resources
    mainTitle = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuTitle"));
    playButtonBackground = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuPlayButtonBackground"));
    creditsButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuCreditsButtonBackground"));
    exitButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuExitButtonBackground"));
 
    buttons = std::vector<HudButton>(3);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("MainMenuOverlay");
    overlay1->add2D(mainTitle);
    overlay1->add2D(playButtonBackground);
    overlay1->add2D(creditsButtonBackground);
    overlay1->add2D(exitButtonBackground);
    overlays.push_back(overlay1);
}

void HudMainMenu::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainTitle);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(playButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(creditsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(exitButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudMainMenu::setOverlay()
{
    // Link and set resources
    mainTitle->setMetricsMode(GMM_RELATIVE);
    mainTitle->setPosition(0.15, 0.15);
    mainTitle->setDimensions(0.60, 0.20);
    mainTitle->setMaterialName("General/TextArtMainTitle");
    
    playButtonBackground->setMaterialName("General/PlayButton");
    creditsButtonBackground->setMaterialName("General/CreditsButton");
    exitButtonBackground->setMaterialName("General/ExitButton");
    buttons[BUTTON_PLAY].setButton("play", overlays[0], GMM_RELATIVE, Vector2(0.20, 0.42), Vector2(0.30, 0.10), playButtonBackground, NULL);
    buttons[BUTTON_CREDITS].setButton("credits", overlays[0], GMM_RELATIVE, Vector2(0.20, 0.60), Vector2(0.30, 0.10), creditsButtonBackground, NULL);
    buttons[BUTTON_EXIT].setButton("exit", overlays[0], GMM_RELATIVE, Vector2(0.20, 0.78), Vector2(0.30, 0.10), exitButtonBackground, NULL);
}

void HudMainMenu::link(Player* player)
{
    this->player = player;
}

void HudMainMenu::unlink()
{
    this->player = NULL;
}