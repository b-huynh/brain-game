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

    gameTitleText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "InformationGameTitleText"));
    BGCTopLinkText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "InformationBGCTopLinkText"));
    nbackLinkText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "InformationNBackLinkText"));
    BGCRecallLinkText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "InformationRecallLinkText"));
    
    BGCTopLinkButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "InformationBGCTopLinkButtonBackground"));
    nbackLinkButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "InformationNBackLinkButtonBackground"));
    BGCRecallLinkButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "InformationBGCRecallLinkButtonBackground"));
    

    
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("InformationOverlay");
    overlay1->add2D(informationBackdrop1);
    overlay1->add2D(informationBackdrop2);
    overlay1->add2D(informationTitleBackground);
    overlay1->add2D(paragraphTextBackground);
    overlay1->add2D(BGCTopLinkButtonBackground);
    overlay1->add2D(nbackLinkButtonBackground);
    overlay1->add2D(BGCRecallLinkButtonBackground);


    informationTitleBackground->addChild(informationTitleText);
    paragraphTextBackground->addChild(paragraphText);
    paragraphTextBackground->addChild(gameTitleText);
    paragraphTextBackground->addChild(BGCTopLinkText);
    paragraphTextBackground->addChild(nbackLinkText);
    paragraphTextBackground->addChild(BGCRecallLinkText);
    
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
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(gameTitleText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(BGCTopLinkText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(nbackLinkText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(BGCRecallLinkText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(BGCTopLinkButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(nbackLinkButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(BGCRecallLinkButtonBackground);


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
    paragraphText->setPosition(0.1, 0.08);
    paragraphText->setCharHeight(0.018 * FONT_SZ_MULT);
    paragraphText->setFontName("MainSmall"); // http://en.wikipedia.org/wiki/N-back
    paragraphText->setCaption("\n\n\n\nThis game is based upon the\n\nIn our space-themed game, you have been abducted by a hostile\nalien species. To escape you must navigate though a series of \nwormholes using one of their foreign ships. To advance wormholes \nyou need to zap matching fuel cells (determined by the n-back). \nThe ships sensors will give you cues to help you determine which \nfuel pods to zap (the n-back targets) and which ones to collect \n(the other fuel pods) to fuel your ship. Remember the sequence of \ncolors, shapes and sounds. \n\nSometimes the fuel pods may randomly cloak their color, shape or \nsound. This means one of those signals will be missing, so you \nshould remember all 3 pieces of information (color, shape, sound) \ntogether. \n\nDon\'t fret if you don\'t get it at first, this is brain training and is \nmeant to be challenging, after a few days you\'ll be proud of your \nprogress. \n\nMore information (FAQs and Tutorials) can be found on our \nwebpage:");
    
    gameTitleText->setMetricsMode(GMM_RELATIVE);
    gameTitleText->setAlignment(TextAreaOverlayElement::Left);
    gameTitleText->setPosition(0.1, 0.11);
    gameTitleText->setCharHeight(0.04 * FONT_SZ_MULT);
    gameTitleText->setFontName("MainSmall");
    gameTitleText->setCaption("Recall the Game");
    
    BGCTopLinkText->setMetricsMode(GMM_RELATIVE);
    BGCTopLinkText->setAlignment(TextAreaOverlayElement::Left);
    BGCTopLinkText->setPosition(0.1, 0.16);
    BGCTopLinkText->setCharHeight(0.024 * FONT_SZ_MULT);
    BGCTopLinkText->setFontName("MainSmall");
    BGCTopLinkText->setCaption("UC Riverside Brain Game Center Production");
    BGCTopLinkText->setColour(Ogre::ColourValue(0,.7,1,1));
    
    nbackLinkText->setMetricsMode(GMM_RELATIVE);
    nbackLinkText->setAlignment(TextAreaOverlayElement::Left);
    nbackLinkText->setPosition(0.48, 0.205); //.298
    nbackLinkText->setCharHeight(0.018 * FONT_SZ_MULT);
    nbackLinkText->setFontName("MainSmall");
    nbackLinkText->setCaption("n-back task.");
    nbackLinkText->setColour(Ogre::ColourValue(0,.7,1,1));
    
    BGCRecallLinkText->setMetricsMode(GMM_RELATIVE);
    BGCRecallLinkText->setAlignment(TextAreaOverlayElement::Left);
    BGCRecallLinkText->setPosition(0.24, 0.865);
    BGCRecallLinkText->setCharHeight(0.028 * FONT_SZ_MULT);
    BGCRecallLinkText->setFontName("MainSmall");
    BGCRecallLinkText->setCaption("braingamecenter.ucr.edu/games/recall");
    BGCRecallLinkText->setColour(Ogre::ColourValue(0,.7,1,1));


    
    backButtonBackground->setMaterialName("General/BackButton1");
    //BGCTopLinkButtonBackground->setMaterialName("General/BackButton1");
    //nbackLinkButtonBackground->setMaterialName("General/BackButton1");
    //BGCRecallLinkButtonBackground->setMaterialName("General/BackButton1");


    //Position Buttons
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.01, 0.91), Vector2(0.30, 0.08), backButtonBackground, NULL);
    buttons[BUTTON_BGC_TOP_LINK].setButton("bgctoplink", overlays[0], GMM_RELATIVE, Vector2(0.09, 0.145), Vector2(0.72, 0.035), BGCTopLinkButtonBackground, NULL);
    
    buttons[BUTTON_N_BACK_LINK].setButton("nbacklink", overlays[0], GMM_RELATIVE, Vector2(0.48, 0.211), Vector2(0.15, 0.025), nbackLinkButtonBackground, NULL);
    buttons[BUTTON_BGC_RECALL_LINK].setButton("bgcrecalllink", overlays[0], GMM_RELATIVE, Vector2(0.23, 0.870), Vector2(0.69, 0.035), BGCRecallLinkButtonBackground, NULL);

}


void HudInformation::link(Player* player)
{
    this->player = player;
}

void HudInformation::unlink()
{
    this->player = NULL;
}
