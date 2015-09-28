//
//  HudCredits.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#include "HudCredits.h"
#include "Player.h"

HudCredits::HudCredits(Player* player)
:Hud()
{
    link(player);
    init();
}

HudCredits::~HudCredits()
{
    dealloc();
    unlink();
}

void HudCredits::init()
{
    alloc();
    initOverlay();
    showOverlays();
}

void HudCredits::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudCredits::update(float elapsed)
{
}

void HudCredits::alloc()
{
    // Allocate Resources
    backButtonBackground = static_cast<PanelOverlayElement*>(
                                                             OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "CreditsBackButtonBackground"));
    
    buttons = std::vector<HudButton>(1);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("CreditsOverlay");
    overlay1->add2D(backButtonBackground);
    overlays.push_back(overlay1);
}

void HudCredits::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudCredits::initOverlay()
{
    backButtonBackground->setMaterialName("General/BackButton");
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.20, 0.78), Vector2(0.30, 0.10), backButtonBackground, NULL);
}

void HudCredits::link(Player* player)
{
    this->player = player;
}

void HudCredits::unlink()
{
    this->player = NULL;
}