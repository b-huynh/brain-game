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
    initOverlay();
    showOverlays();
}

void HudMainMenu::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudMainMenu::update(float elapsed)
{
}

void HudMainMenu::alloc()
{
    // Allocate Resources
    mainMenuEntireBackground = static_cast<PanelOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuEntireBackground"));
    mainTitle = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuTitle"));
    playButtonBackground = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuPlayButtonBackground"));
    creditsButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuCreditsButtonBackground"));
    settingsButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "MainMenuSettingsButtonBackground"));
 
    buttons = std::vector<HudButton>(3);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("MainMenuOverlay");
    overlay1->add2D(mainMenuEntireBackground);
    //overlay1->add2D(mainTitle);
    overlay1->add2D(playButtonBackground);
    overlay1->add2D(creditsButtonBackground);
    overlay1->add2D(settingsButtonBackground);
    overlays.push_back(overlay1);
}

void HudMainMenu::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainMenuEntireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(mainTitle);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(playButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(creditsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(settingsButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudMainMenu::initOverlay()
{
    // Link and set resources
    mainMenuEntireBackground->setMetricsMode(GMM_RELATIVE);
    mainMenuEntireBackground->setPosition(0.0, 0.0);
    mainMenuEntireBackground->setDimensions(1.0, 1.0);
    mainMenuEntireBackground->setMaterialName("General/ScreenBackgroundMainMenu");
    
    mainTitle->setMetricsMode(GMM_RELATIVE);
    mainTitle->setPosition(0.15, 0.15);
    mainTitle->setDimensions(0.60, 0.20);
    mainTitle->setMaterialName("General/TextArtMainTitle");
    
    //playButtonBackground->setMaterialName("General/PlayButton");
    //creditsButtonBackground->setMaterialName("General/CreditsButton");
    //settingsButtonBackground->setMaterialName("General/ExitButton");
    buttons[BUTTON_PLAY].setButton("play", overlays[0], GMM_RELATIVE, Vector2(0.16, 0.40), Vector2(0.12, 0.08), playButtonBackground, NULL);
    buttons[BUTTON_CREDITS].setButton("credits", overlays[0], GMM_RELATIVE, Vector2(0.16, 0.49), Vector2(0.15, 0.08), creditsButtonBackground, NULL);
    buttons[BUTTON_SETTINGS].setButton("settings", overlays[0], GMM_RELATIVE, Vector2(0.16, 0.58), Vector2(0.18, 0.08), settingsButtonBackground, NULL);
}

void HudMainMenu::link(Player* player)
{
    this->player = player;
}

void HudMainMenu::unlink()
{
    this->player = NULL;
}