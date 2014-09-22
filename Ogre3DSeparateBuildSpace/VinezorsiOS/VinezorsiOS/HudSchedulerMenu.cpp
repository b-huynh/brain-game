//
//  HudSchedulerMenu.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 9/15/14.
//
//

#include "HudSchedulerMenu.h"
#include "Player.h"
#include "LevelSet.h"

extern Util::ConfigGlobal globals;

HudSchedulerMenu::HudSchedulerMenu(Player* player)
: Hud()
{
    link(player);
    init();
}

HudSchedulerMenu::~HudSchedulerMenu()
{
    dealloc();
    unlink();
}

void HudSchedulerMenu::init()
{
    alloc();
    initOverlay();
    showOverlays();
}

void HudSchedulerMenu::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudSchedulerMenu::update(float elapsed)
{
}

void HudSchedulerMenu::alloc()
{
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS + NUM_SELECTIONS);
    
    schedulerMenuBackdrop = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuBackdrop"));
    schedulerMenuTitleBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuTitleBackground"));
    schedulerMenuTitleText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "SchedulerMenuTitleText"));
    
    levelOverlayPanels = std::vector<LevelOverlayElement>(NUM_SELECTIONS);
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        levelOverlayPanels[i].entireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ScheduleMenuLevelEntireBackground" + Util::toStringInt(i)));
        levelOverlayPanels[i].title = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelTitle" + Util::toStringInt(i)));
        levelOverlayPanels[i].description = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelDescription" + Util::toStringInt(i)));
    }
    
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuBackButtonBackground"));
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("SchedulerMenuOverlay");
    overlays.push_back(overlay1);
    overlay1->add2D(schedulerMenuTitleBackground);
    schedulerMenuTitleBackground->addChild(schedulerMenuTitleText);
    overlay1->add2D(schedulerMenuBackdrop);
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        schedulerMenuBackdrop->addChild(levelOverlayPanels[i].entireBackground);
        levelOverlayPanels[i].entireBackground->addChild(levelOverlayPanels[i].title);
        levelOverlayPanels[i].entireBackground->addChild(levelOverlayPanels[i].description);
    }
    overlay1->add2D(backButtonBackground);
}

void HudSchedulerMenu::dealloc()
{
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(schedulerMenuTitleBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(schedulerMenuTitleText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(schedulerMenuBackdrop);
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelOverlayPanels[i].entireBackground);
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelOverlayPanels[i].title);
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelOverlayPanels[i].description);
    }
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudSchedulerMenu::initOverlay()
{
    schedulerMenuTitleBackground->setMetricsMode(GMM_RELATIVE);
    schedulerMenuTitleBackground->setPosition(0.30, 0.025);
    schedulerMenuTitleBackground->setDimensions(0.40, 0.20);
    
    schedulerMenuTitleText->setMetricsMode(GMM_RELATIVE);
    schedulerMenuTitleText->setAlignment(TextAreaOverlayElement::Center);
    schedulerMenuTitleText->setPosition(0.20, 0.00);
    schedulerMenuTitleText->setCharHeight(0.046 * FONT_SZ_MULT);
    schedulerMenuTitleText->setFontName("MainSmall");
    schedulerMenuTitleText->setCaption("Scheduler");
    
    schedulerMenuBackdrop->setMetricsMode(GMM_RELATIVE);
    schedulerMenuBackdrop->setPosition(0.00, 0.025);
    schedulerMenuBackdrop->setDimensions(1.00, 0.95);
    schedulerMenuBackdrop->setMaterialName("General/TutorialBackdrop");
    
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        std::string buttonName = "selection";
        buttonName += Util::toStringInt(i);
        buttons[NUM_UNIQUE_BUTTONS + i].setButton(buttonName, overlays[0], GMM_RELATIVE, Vector2(0.15 + 0.250 * i, 0.35), Vector2(0.20, 0.40), levelOverlayPanels[i].entireBackground, NULL);
        //levelOverlayPanels[i].entireBackground->setMetricsMode(GMM_RELATIVE);
        //levelOverlayPanels[i].entireBackground->setPosition(0.15 + 0.250 * i, 0.35);
        //levelOverlayPanels[i].entireBackground->setDimensions(0.20, 0.40);
        levelOverlayPanels[i].entireBackground->setMaterialName("General/ScreenBackground2");
        
        levelOverlayPanels[i].title->setMetricsMode(GMM_RELATIVE);
        levelOverlayPanels[i].title->setAlignment(TextAreaOverlayElement::Center);
        levelOverlayPanels[i].title->setPosition(0.10, -0.025);
        levelOverlayPanels[i].title->setCharHeight(0.025 * FONT_SZ_MULT);
        levelOverlayPanels[i].title->setFontName("MainSmall");
        levelOverlayPanels[i].title->setCaption("Item " + Util::toStringInt(i));
        
        levelOverlayPanels[i].description->setMetricsMode(GMM_RELATIVE);
        levelOverlayPanels[i].description->setAlignment(TextAreaOverlayElement::Left);
        levelOverlayPanels[i].description->setPosition(0.010, 0.050);
        levelOverlayPanels[i].description->setCharHeight(0.025 * FONT_SZ_MULT);
        levelOverlayPanels[i].description->setFontName("MainSmall");
        levelOverlayPanels[i].description->setCaption("Description\nDescription\nDescription");
    }
    
    // Set up buttons
    backButtonBackground->setMaterialName("General/BackButton");
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.15, 0.90), Vector2(0.30, 0.08), backButtonBackground, NULL);
}

void HudSchedulerMenu::link(Player* player)
{
    this->player = player;
}

void HudSchedulerMenu::unlink()
{
    this->player = NULL;
}
