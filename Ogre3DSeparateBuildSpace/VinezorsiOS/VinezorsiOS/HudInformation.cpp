//
//  HudInformation.cpp
//  VinezorsiOS
//
//  Created by BGC_Bernie on 3/18/15.
//
//

#include "HudInformation.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

HudInformation::HudInformation(Player* player)
:Hud()
{
    link(player);
    init();
}

HudInformation::~HudInformation()
{
    dealloc();
    unlink();
}

void HudInformation::init()
{
    alloc();
    initOverlay();
    showOverlays();
}

void HudInformation::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudInformation::update(float elapsed)
{
    
}

void HudInformation::alloc()
{
    // Allocate Resources
    informationBackdrop1 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "InformationBackdrop1"));
    informationBackdrop2 = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "InformationBackdrop2"));
    informationTitleBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "InformationTitleBackground"));
    informationTitleText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "InformationTitleText"));
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "InformationBackButtonBackground"));
    
    paragraphText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "InformationParagraphText"));
    paragraphTextBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "InformationParagraphTextBackground"));


    
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("InformationOverlay");
    overlay1->add2D(informationBackdrop1);
    overlay1->add2D(informationBackdrop2);
    overlay1->add2D(informationTitleBackground);
    overlay1->add2D(paragraphTextBackground);
    informationTitleBackground->addChild(informationTitleText);
    paragraphTextBackground->addChild(paragraphText);

    
    
    overlay1->add2D(backButtonBackground);
    
    overlays.push_back(overlay1);

}

void HudInformation::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(informationBackdrop1);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(informationBackdrop2);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(informationTitleBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(informationTitleText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(paragraphText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(paragraphTextBackground);


    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
    
}

void HudInformation::initOverlay()
{
    // Link and set resources
    informationBackdrop1->setMetricsMode(GMM_RELATIVE);
    informationBackdrop1->setPosition(0.00, 0.025);
    informationBackdrop1->setDimensions(1.00, 0.95);
    informationBackdrop1->setMaterialName("General/TutorialBackdrop");
    
    
    informationTitleBackground->setMetricsMode(GMM_RELATIVE);
    informationTitleBackground->setPosition(0.30, 0.025);
    informationTitleBackground->setDimensions(0.40, 0.20);
    
    informationTitleText->setMetricsMode(GMM_RELATIVE);
    informationTitleText->setAlignment(TextAreaOverlayElement::Center);
    informationTitleText->setPosition(0.20, 0.00);
    informationTitleText->setCharHeight(0.046 * FONT_SZ_MULT);
    informationTitleText->setFontName("MainSmall");
    informationTitleText->setCaption("Information");

    
    paragraphTextBackground->setMetricsMode(GMM_RELATIVE);
    paragraphTextBackground->setPosition(0.00, 0.0);
    paragraphTextBackground->setDimensions(0.40, 0.20);
    
    paragraphText->setMetricsMode(GMM_RELATIVE);
    paragraphText->setAlignment(TextAreaOverlayElement::Left);
    paragraphText->setPosition(0.1, 0.20);
    paragraphText->setCharHeight(0.015 * FONT_SZ_MULT);
    paragraphText->setFontName("MainSmall"); //http:// bgc.ucr.edu/recall http://en.wikipedia.org/wiki/N-back
    paragraphText->setCaption("Recall the Game  - \nUC Riverside Brain Game Center Production \n\n\n\nThis game is based upon the n-back task \n\n\nIn our space-themed game, you have been abducted by a hostile alien species. \nTo escape you must navigate though a series of wormholes using one of their \nforeign ships. To advance wormholes you need to zap matching fuel cells \n(determined by the n-back). The ships sensors will give you cues to help you \ndetermine which fuel pods to zap (the n-back targets) and which ones to \ncollect (the other fuel pods) to fuel your ship. Remember the sequence of \ncolors, shapes and sounds. \n\nSometimes the fuel pods may randomly cloak their color, shape or sound. This \nmeans one of those signals will be missing, so you should remember all 3 \npieces of information (color, shape, sound) together. \n\nDon’t fret if you don’t get it at first, this is brain training and is meant to be \nchallenging, after a few days you’ll be proud of your progress. \n\nMore information (FAQs and Tutorials) can be found on our webpage: \nhttp:// bgc.ucr.edu/recall");
    
    backButtonBackground->setMaterialName("General/BackButton1");
    
    //Position Buttons
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.01, 0.90), Vector2(0.30, 0.08), backButtonBackground, NULL);

}


void HudInformation::link(Player* player)
{
    this->player = player;
}

void HudInformation::unlink()
{
    this->player = NULL;
}
