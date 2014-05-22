//
//  HudStage.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#include "HudStage.h"

#include "Tunnel.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

HudStage::HudStage(Player* player, Tunnel* tunnel)
: Hud(), goButtonActive(false)
{
    link(player, tunnel);
    init();
}

HudStage::~HudStage()
{
    dealloc();
    unlink();
}

void HudStage::init()
{
    alloc();
    initOverlay();
}

void HudStage::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudStage::update(float elapsed)
{
    if (tunnel->getMode() == STAGE_MODE_RECESS)
        label3->setCaption(Util::toStringInt(tunnel->getPercentComplete() * 100) + "%");
    else
        label3->setCaption("");
    
    label4->setCaption(Util::toStringInt(player->getBaseSpeed()));
    if (player->hasTriggeredStartup())
    {
        speedSlider->setBallDestination(player->getBaseSpeed());
        speedSlider->update(elapsed);
    }
    speedSlider->adjust();
    
    std::string GUIToggleNumber = "General/GUIToggleTextNumber";
    toggle1TextArt->setMaterialName(GUIToggleNumber + Util::toStringInt(tunnel->getNBack() % 10));
    toggle2TextArt->setMaterialName(GUIToggleNumber + Util::toStringInt(Util::clamp((tunnel->getNBack() - 1) % 10, 0, tunnel->getNBack())));
    toggle3TextArt->setMaterialName(GUIToggleNumber + Util::toStringInt(Util::clamp((tunnel->getNBack() - 2) % 10, 0, tunnel->getNBack())));
    // toggle4 is always n-back 0
    
    if (!tunnel->needsCleaning())
        resumeButtonBackground->setMaterialName("General/ResumeButtonRound");
    else
        resumeButtonBackground->setMaterialName("General/ResumeButtonRoundGRAY");
    
    LevelSet* levels = player->getLevels();
    int levelRow = player->getLevelRequestRow();
    int levelCol = player->getLevelRequestCol();
    int level = levels->getLevelNo(levelRow, levelCol);
    if (player->isLevelAvailable(level + 1))
        nextButtonBackground->setMaterialName("General/NextButtonRound");
    else
        nextButtonBackground->setMaterialName("General/NextButtonRoundGRAY");
    
    // Order matters, will be overwritten by time warp or end score
    float timeLeft = fmax(tunnel->getStageTime() - tunnel->getTotalElapsed() - tunnel->getTimePenalty(), 0.0f);
    Ogre::ColourValue fontColor = timeLeft <= 0.0 ? ColourValue(1.0, 0.0, 0.0) : ColourValue(1.0, 1.0, 1.0);
    label2->setColour(fontColor);
    label2->setCaption(Util::toStringInt(timeLeft));
    if( player->isPowerUpActive("TimeWarp") ) {
        TimeWarp* t = (TimeWarp*)player->getPowerUpPtr("TimeWarp");
        
        if( t->zoomIn == 0 ) {
            setOverlay(2, false);
            timeWarpLabel->setCharHeight(0.05 * FONT_SZ_MULT);
            timeWarpContainer->setPosition(0.50, 0.15);
        }
        else if( t->zoomIn == 1 ) {
            setOverlay(2, true);
            timeWarpLabel->setCharHeight(timeWarpLabel->getCharHeight()-0.0005 * FONT_SZ_MULT);
            //timeWarpContainer->setPosition(timeWarpContainer->getLeft()+0.001 * FONT_SZ_MULT, timeWarpContainer->getTop()+0.0005 * FONT_SZ_MULT);
        }
        else {
            setOverlay(2, true);
            label2->setColour(ColourValue(1.0,1.0,0.0));
            label2->setCharHeight(0.030 * FONT_SZ_MULT);
        }
        
        std::string val;
        if( t->currentTimeVal < 10 ) val = " "+Util::toStringInt(t->currentTimeVal);
        else val = Util::toStringInt(t->currentTimeVal);
        
        timeWarpLabel->setCaption("+" + val + " Seconds");
    }
    else {
        setOverlay(2, false);
        label2->setColour(ColourValue(1.0,1.0,1.0));
        label2->setCharHeight(0.025 * FONT_SZ_MULT);
    }
    
    if( player->winFlag ) {
        setOverlay(3, true);
        
        tunnel->addToTimePenalty(2.0f);
        float timeLeft = tunnel->getStageTime() - tunnel->getTotalElapsed() - tunnel->getTimePenalty();
        
        if( timeLeft < 0.0f && timeLeft > -1.0f) player->setScore(player->getScore()+100.0f);
        else if( timeLeft > 0.0f ) player->setScore(player->getScore()+200.0f);
        
        label2->setColour(ColourValue(1.0,1.0,0.0));
        label5->setColour(ColourValue(1.0,1.0,0.0));
        
        label2->setCaption(Util::toStringInt(timeLeft));
        endTallyTimeLabel->setCaption("Time  " + Util::toStringInt(timeLeft));
        
        label5->setCaption(Util::toStringInt(player->getScore()));
        
        endTallyScoreLabel->setCaption(Util::toStringInt(player->getScore()) + "  Score");
        
        if( timeLeft <= 0.0f ) {
            label2->setCaption("0");
            endTallyTimeLabel->setCaption("Time  0");
            tunnel->setCleaning(true);
            player->winFlag = false;
        }
        
        return;
    }
    if( tunnel->isDone() ) return;

    label1->setCaption(globals.messageBig);
    
    label5->setCaption(Util::toStringInt(player->getScore()));
    label6->setCaption(globals.message);
    label7->setCaption("");
    
    // Set Progress Bar indicator position for the appropriate mode
    float barWidth = barHP->getWidth();
    if (tunnel->getMode() == STAGE_MODE_PROFICIENCY)
    {
        float HPRange = globals.HPPositiveLimit - globals.HPNegativeLimit + 1;
        //indicator->setPosition(barHP->getLeft() + barWidth * player->getProgress(), indicator->getTop());
        indicator->setPosition(barHP->getLeft() + barWidth * (player->getHP() - globals.HPNegativeLimit) / HPRange, indicator->getTop());
    }
    
    switch (player->getToggleBack())
    {
        case 0:
            toggleIndicator->setPosition(0.897, 0.31);
            break;
        case 1:
            toggleIndicator->setPosition(0.897, 0.43);
            break;
        case 2:
            toggleIndicator->setPosition(0.897, 0.55);
            break;
        case 3:
            toggleIndicator->setPosition(0.897, 0.67);
            break;
    }
    
    if (player->isPowerUpAvailable("TractorBeam"))
        buttons[BUTTON_POWERUP1].backgroundRef->setMaterialName("General/GUIPowerupButton2");
    else
        buttons[BUTTON_POWERUP1].backgroundRef->setMaterialName("General/GUIPowerupButtonBlank");
    if (player->isPowerUpAvailable("Shields"))
        buttons[BUTTON_POWERUP2].backgroundRef->setMaterialName("General/GUIPowerupButton0");
    else
        buttons[BUTTON_POWERUP2].backgroundRef->setMaterialName("General/GUIPowerupButtonBlank");
    if (player->isPowerUpAvailable("TimeWarp"))
        buttons[BUTTON_POWERUP3].backgroundRef->setMaterialName("General/GUIPowerupButton1");
    else
        buttons[BUTTON_POWERUP3].backgroundRef->setMaterialName("General/GUIPowerupButtonBlank");
    
    std::vector<CollectionCriteria> criterias = tunnel->getCollectionCriteria();
    for (int i = 0; i < collectionBar.size(); ++i)
    {
        if (i < criterias.size())
        {
            std::string scoreName = "General/GUICollection";
            
            // For togglebacks
            //if (criterias[i].nback <= 0)
            //    scoreName += Util::toStringInt(0);
            //else
            //    scoreName += Util::toStringInt(Util::clamp(3 - (tunnel->getNBack() - criterias[i].nback), 0, 3));
            
            // For just a single n-back and task
            switch (tunnel->getPhase())
            {
                case 'A':
                    scoreName += "0";
                    break;
                case 'B':
                    scoreName += "1";
                    break;
                case 'C':
                    scoreName += "2";
                    break;
                case 'D':
                    scoreName += "3";
                    break;
                default:
                    scoreName += "0";
                    break;
            }
            
            if (criterias[i].collected)
                scoreName += "Filled";
            else
                scoreName += "Blank";
            collectionBar[i]->setMaterialName(scoreName);
        }
        else
        {
            collectionBar[i]->setMaterialName("General/GUICollectionGreyed");
        }
    }
}

void HudStage::alloc()
{
    // Allocate Resources
    panelText = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageTextInterface"));
    panelText->setMetricsMode(GMM_PIXELS);
    panelText->setPosition(10, 10);
    panelText->setDimensions(10, 10);
    
    healthArea = static_cast<BorderPanelOverlayElement*>(
                                                         OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("BorderPanel", "StageHealthAreaBorder"));
    
    barHP = static_cast<PanelOverlayElement*>(
                                              OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHealthBar"));
    indicator = static_cast<PanelOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageIndicator"));
    pauseBackground = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePauseBackground"));
    goBackground = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageGoBackground"));
    
    sliderRangeBackground = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageSliderRangeBackground"));
    sliderBallBackground = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageSliderBallBackground"));
    
    collectionBar = std::vector<PanelOverlayElement*>();
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem0")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem1")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem2")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem3")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem4")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem5")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem6")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem7")));
    
    GUITopPanel = static_cast<PanelOverlayElement*>(
                                                    OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "GUITopPanel"));
    
    // Create text area
    label1 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextAreaLabel1"));
    label2 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextAreaLabel2"));
    label2prompt = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextAreaLabel2Prompt"));
    label3 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextAreaLabel3"));
    label4 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextAreaLabel4"));
    label5 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextAreaLabel5"));
    label5prompt = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextAreaLabel5Prompt"));
    label6 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextAreaLabel6"));
    label7 = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextAreaLabel7"));
    
    timeWarpLabel = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageTextTimeWarpLabel"));
    
    endTallyTimeLabel = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "EndTallyTimeLabel"));
    endTallyScoreLabel = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "EndTallyScoreLabel"));
    
    nbackDisplayBackground = static_cast<PanelOverlayElement*>(
                                                               OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageNBackDisplayBackground"));
    nbackDisplayLabel = static_cast<TextAreaOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageNBackDisplayLabel"));
    
    // Note: These overlay elements are linked to a button, if we end up deleting these and NULLing it, it should be NULLed in the associated button as well
    toggle1Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageToggle1Background"));
    toggle2Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageToggle2Background"));
    toggle3Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageToggle3Background"));
    toggle4Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageToggle4Background"));
    toggle1TextArt = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageToggle1TextArt"));
    toggle2TextArt = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageToggle2TextArt"));
    toggle3TextArt = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageToggle3TextArt"));
    toggle4TextArt = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageToggle4TextArt"));
    toggleIndicator = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageToggleIndicator"));
    powerup1Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "Powerup1Background"));
    powerup2Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "Powerup2Background"));
    powerup3Background = static_cast<PanelOverlayElement*>(
                                                          OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "Powerup3Background"));
    
    
    resumeButtonBackground = static_cast<PanelOverlayElement*>(
                                                           OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageResumeButtonBackground"));
    nextButtonBackground = static_cast<PanelOverlayElement*>(
                                                           OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageNextButtonBackground"));
    restartButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageRestartButtonBackground"));
    levelSelectButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageLevelSelectButtonBackground"));
    
    buttons = std::vector<HudButton>(13);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("StageOverlayHUD");
    Overlay* overlay2 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("StageOverlayPauseMenu");
    
    
    
    Overlay* overlay3 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("TimeWarpOverlay");
    timeWarpContainer = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TimeWarpInterface"));
    overlay3->add2D(timeWarpContainer);
    timeWarpContainer->addChild(timeWarpLabel);
    
    
    Overlay* overlay4 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("EndTallyOverlay");
    endTallyContainer = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "EndTallyInterface"));
    overlay4->add2D(endTallyContainer);
    endTallyContainer->addChild(endTallyTimeLabel);
    endTallyContainer->addChild(endTallyScoreLabel);
    
    //overlay1->add2D(healthArea);
    //overlay1->add2D(barHP);
    //overlay1->add2D(indicator);
    
    overlay1->add2D(GUITopPanel);
    
    overlay1->add2D(pauseBackground);
    for (int i = 0; i < collectionBar.size(); ++i)
        overlay1->add2D(collectionBar[i]);
    
    overlay1->add2D(powerup1Background);
    overlay1->add2D(powerup2Background);
    overlay1->add2D(powerup3Background);
    overlay1->add2D(nbackDisplayBackground);
    nbackDisplayBackground->addChild(nbackDisplayLabel);
    //overlay1->add2D(toggle1Background);
    //overlay1->add2D(toggle2Background);
    //overlay1->add2D(toggle3Background);
    //overlay1->add2D(toggle4Background);
    //toggle1Background->addChild(toggle1TextArt);
    //toggle2Background->addChild(toggle2TextArt);
    //toggle3Background->addChild(toggle3TextArt);
    //toggle4Background->addChild(toggle4TextArt);
    overlay1->add2D(toggleIndicator);
    overlay1->add2D(panelText);
    panelText->addChild(label1);
    panelText->addChild(label2);
    panelText->addChild(label2prompt);
    panelText->addChild(label3);
    panelText->addChild(label4);
    panelText->addChild(label5);
    panelText->addChild(label5prompt);
    panelText->addChild(label6);
    panelText->addChild(label7);
    overlay2->add2D(resumeButtonBackground);
    overlay2->add2D(nextButtonBackground);
    overlay2->add2D(restartButtonBackground);
    overlay2->add2D(levelSelectButtonBackground);
    
    overlay1->add2D(goBackground);
    overlay1->add2D(sliderRangeBackground);
    sliderRangeBackground->addChild(sliderBallBackground);
    
    overlays.push_back(overlay1);
    overlays.push_back(overlay2);
    overlays.push_back(overlay3);
    overlays.push_back(overlay4);
    
    speedSlider = new HudSlider();
    
    // Horizontal slider
    //speedSlider->setSlider("speed", overlays[0], Vector2(0.25, 0.45), Vector2(0.50, 0.10), Vector2(0.10, 0.10), false,
    //                       globals.minCamSpeed, globals.maxCamSpeed, globals.maxCamSpeed - globals.minCamSpeed + 1, sliderRangeBackground, sliderBallBackground);
    
    // Vertical Slider
    speedSlider->setSlider("speed", overlays[0], Vector2(0.04, 0.25), Vector2(0.03, 0.55), Vector2(0.03, 0.09), true,
                           globals.minCamSpeed, globals.maxCamSpeed, globals.maxCamSpeed - globals.minCamSpeed + 1, sliderRangeBackground, sliderBallBackground);
    
    // Set the ball position to the previous speed setting if the player played this level before
    // Otherwise, use the suggested speed
    PlayerProgress levelPerformance = player->getLevelProgress(player->getLevelRequestRow(), player->getLevelRequestCol());
    speedSlider->adjust();
    if (levelPerformance.initSpeedSetting >= 0)
        speedSlider->setBallPosition(levelPerformance.initSpeedSetting);
    else
        speedSlider->setBallPosition(globals.initCamSpeed);
}

void HudStage::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(healthArea);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(barHP);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(indicator);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pauseBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(goBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sliderRangeBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(sliderBallBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(GUITopPanel);
    for (int i = 0; i < collectionBar.size(); ++i)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(collectionBar[i]);
    collectionBar.clear();
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(label1);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(label2);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(label2prompt);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(label3);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(label4);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(label5);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(label5prompt);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(label6);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(label7);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(timeWarpLabel);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(endTallyTimeLabel);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(endTallyScoreLabel);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(nbackDisplayBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(nbackDisplayLabel);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(toggle1TextArt);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(toggle2TextArt);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(toggle3TextArt);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(toggle4TextArt);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(toggle1Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(toggle2Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(toggle3Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(toggle4Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(toggleIndicator);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(powerup1Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(powerup2Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(powerup3Background);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(resumeButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(nextButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(restartButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSelectButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(panelText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(timeWarpContainer);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(endTallyContainer);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[1]);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[2]);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[3]);
    if (speedSlider) delete speedSlider;
    speedSlider = NULL;
}

void HudStage::initOverlay()
{
    // Link and Set Resources
    panelText->setMetricsMode(GMM_PIXELS);
    panelText->setPosition(10, 10);
    panelText->setDimensions(10, 10);
    
    timeWarpContainer->setMetricsMode(GMM_RELATIVE);
    timeWarpContainer->setPosition(0.35, 0.15);
    timeWarpContainer->setDimensions(0.25, 0.25);
    
    endTallyContainer->setMetricsMode(GMM_RELATIVE);
    endTallyContainer->setPosition(0.375, 0.15);
    endTallyContainer->setDimensions(0.25, 0.25);
    
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
    
    GUITopPanel->setMetricsMode(GMM_RELATIVE);
    GUITopPanel->setPosition(0.0, 0.0);
    GUITopPanel->setDimensions(1.0, 1.0);
    GUITopPanel->setMaterialName("General/GUIMainHud");
    
    float x = 0.285;
    float y = 0.035;
    float wpadding = 0.0050;
    float width = 0.0425;
    float height = 0.075;
    std::vector<CollectionCriteria> criterias = tunnel->getCollectionCriteria();
    for (int i = 0; i < collectionBar.size(); ++i)
    {
        collectionBar[i]->setMetricsMode(GMM_RELATIVE);
        collectionBar[i]->setPosition(x + wpadding, y);
        collectionBar[i]->setDimensions(width, height);
        x += 2 * wpadding + width;
    }
    
    label1->setMetricsMode(GMM_PIXELS);
    label1->setAlignment(TextAreaOverlayElement::Center);
    label1->setPosition(globals.label1_posX, globals.label1_posY);
    label1->setCharHeight(globals.screenHeight / 40 * FONT_SZ_MULT);
    label1->setFontName("Arial");
    label1->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    
    label2->setMetricsMode(GMM_RELATIVE);
    label2->setAlignment(TextAreaOverlayElement::Left);
    label2->setPosition(0.05, 0.040);
    //label2->setAlignment(TextAreaOverlayElement::Right);
    //label2->setPosition(0.21, 0.04);
    label2->setCharHeight(0.025 * FONT_SZ_MULT);
    label2->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label2->setFontName("Arial");
    
    label2prompt->setMetricsMode(GMM_RELATIVE);
    label2prompt->setAlignment(TextAreaOverlayElement::Left);
    label2prompt->setPosition(0.06, 0.085);
    //label2prompt->setAlignment(TextAreaOverlayElement::Center);
    //label2prompt->setPosition(0.12, 0.08);
    label2prompt->setCharHeight(0.02 * FONT_SZ_MULT);
    label2prompt->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label2prompt->setFontName("Arial");
    label2prompt->setCaption("Time");
    
    label3->setMetricsMode(GMM_RELATIVE);
    label3->setAlignment(TextAreaOverlayElement::Right);
    label3->setPosition(0.20, 0.085);
    label3->setCharHeight(0.02 * FONT_SZ_MULT);
    label3->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label3->setFontName("Arial");
    
    label4->setMetricsMode(GMM_RELATIVE);
    label4->setAlignment(TextAreaOverlayElement::Center);
    label4->setPosition(0.042, 0.175);
    label4->setCharHeight(0.02 * FONT_SZ_MULT);
    label4->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label4->setFontName("Arial");
    
    label5->setMetricsMode(GMM_RELATIVE);
    label5->setAlignment(TextAreaOverlayElement::Right);
    label5->setPosition(0.92, 0.040);
    label5->setCharHeight(0.025 * FONT_SZ_MULT);
    label5->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label5->setFontName("Arial");
    
    label5prompt->setMetricsMode(GMM_RELATIVE);
    label5prompt->setAlignment(TextAreaOverlayElement::Right);
    label5prompt->setPosition(0.92, 0.085);
    //label5prompt->setAlignment(TextAreaOverlayElement::Center);
    //label5prompt->setPosition(0.85, 0.08);
    label5prompt->setCharHeight(0.02 * FONT_SZ_MULT);
    label5prompt->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label5prompt->setFontName("Arial");
    label5prompt->setCaption("Score");
    
    label6->setMetricsMode(GMM_PIXELS);
    label6->setAlignment(TextAreaOverlayElement::Center);
    label6->setPosition(globals.label6_posX, globals.label6_posY);
    label6->setCharHeight(globals.screenHeight / 50 * FONT_SZ_MULT);
    label6->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    label6->setFontName("Arial");
    
    label7->setMetricsMode(GMM_PIXELS);
    label7->setAlignment(TextAreaOverlayElement::Center);
    label7->setPosition(globals.label7_posX, globals.label7_posY);
    label7->setCharHeight(globals.screenHeight / 50 * FONT_SZ_MULT);
    label7->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    label7->setFontName("Arial");
    
    timeWarpLabel->setMetricsMode(GMM_RELATIVE);
    timeWarpLabel->setAlignment(TextAreaOverlayElement::Center);
    timeWarpLabel->setPosition(0, 0);
    timeWarpLabel->setCharHeight(0.05 * FONT_SZ_MULT);
    timeWarpLabel->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    timeWarpLabel->setFontName("Arial");
    
    endTallyTimeLabel->setMetricsMode(GMM_PIXELS);
    endTallyTimeLabel->setAlignment(TextAreaOverlayElement::Left);
    endTallyTimeLabel->setPosition(0, 0);
    endTallyTimeLabel->setCharHeight(globals.screenHeight/40 * FONT_SZ_MULT);
    endTallyTimeLabel->setColour(ColourValue::ColourValue(1.0,1.0,0.0));
    endTallyTimeLabel->setFontName("Arial");
    
    endTallyScoreLabel->setMetricsMode(GMM_PIXELS);
    endTallyScoreLabel->setAlignment(TextAreaOverlayElement::Right);
    endTallyScoreLabel->setPosition(200, 20);
    endTallyScoreLabel->setCharHeight(globals.screenHeight/40 * FONT_SZ_MULT);
    endTallyScoreLabel->setColour(ColourValue::ColourValue(1.0,1.0,0.0));
    endTallyScoreLabel->setFontName("Arial");
    
    toggle1TextArt->setMetricsMode(GMM_RELATIVE);
    toggle1TextArt->setPosition(0.0250, 0.0025);
    toggle1TextArt->setDimensions(0.03, 0.08);
    
    toggle2TextArt->setMetricsMode(GMM_RELATIVE);
    toggle2TextArt->setPosition(0.0250, 0.0025);
    toggle2TextArt->setDimensions(0.03, 0.08);
    
    toggle3TextArt->setMetricsMode(GMM_RELATIVE);
    toggle3TextArt->setPosition(0.0250, 0.0025);
    toggle3TextArt->setDimensions(0.03, 0.08);
    
    toggle4TextArt->setMetricsMode(GMM_RELATIVE);
    toggle4TextArt->setPosition(0.0250, 0.0025);
    toggle4TextArt->setDimensions(0.03, 0.08);
    toggle4TextArt->setMaterialName("General/GUIToggleTextNumber0");
    
    float pauseheight = 0.075;
    float pausewidth = pauseheight * globals.screenHeight / globals.screenWidth;
    buttons[BUTTON_PAUSE].setButton("pause", overlays[0], GMM_RELATIVE, Vector2(0.91, 0.79), Vector2(pausewidth, pauseheight), pauseBackground, NULL);
    
    float gheight = 0.40;
    float gwidth = gheight * globals.screenHeight / globals.screenWidth;
    buttons[BUTTON_GO].setButton("go", overlays[0], GMM_RELATIVE, Vector2(0.50 - gwidth / 2, 0.50 - gheight / 2), Vector2(gwidth, gheight), goBackground, NULL);
    
    buttons[BUTTON_TOGGLE1].setButton("toggle1", overlays[0], GMM_RELATIVE, Vector2(0.897, 0.31), Vector2(0.08, 0.08), toggle1Background, NULL);
    buttons[BUTTON_TOGGLE2].setButton("toggle2", overlays[0], GMM_RELATIVE, Vector2(0.897, 0.43), Vector2(0.08, 0.08), toggle2Background, NULL);
    buttons[BUTTON_TOGGLE3].setButton("toggle3", overlays[0], GMM_RELATIVE, Vector2(0.897, 0.55), Vector2(0.08, 0.08), toggle3Background, NULL);
    buttons[BUTTON_TOGGLE4].setButton("toggle4", overlays[0], GMM_RELATIVE, Vector2(0.897, 0.67), Vector2(0.08, 0.08), toggle4Background, NULL);
    float pheight = 0.12;
    float pwidth = pheight * globals.screenHeight / globals.screenWidth;
    buttons[BUTTON_POWERUP1].setButton("powerup1", overlays[0], GMM_RELATIVE, Vector2(0.015, 0.45), Vector2(pwidth, pheight), powerup1Background, NULL);
    buttons[BUTTON_POWERUP2].setButton("powerup2", overlays[0], GMM_RELATIVE, Vector2(0.015, 0.60), Vector2(pwidth, pheight), powerup2Background, NULL);
    buttons[BUTTON_POWERUP3].setButton("powerup3", overlays[0], GMM_RELATIVE, Vector2(0.015, 0.75), Vector2(pwidth, pheight), powerup3Background, NULL);
    
    float qheight = 0.10;
    float qwidth = 0.30;
    buttons[BUTTON_RESUME].setButton("resume", overlays[1], GMM_RELATIVE, Vector2(0.35, 0.30), Vector2(qwidth, qheight), resumeButtonBackground, NULL);
    buttons[BUTTON_NEXT].setButton("next", overlays[1], GMM_RELATIVE, Vector2(0.35, 0.42), Vector2(qwidth, qheight), nextButtonBackground, NULL);
    buttons[BUTTON_RESTART].setButton("restart", overlays[1], GMM_RELATIVE, Vector2(0.35, 0.54), Vector2(qwidth, qheight), restartButtonBackground, NULL);
    buttons[BUTTON_LEVELSELECT].setButton("levelselect", overlays[1], GMM_RELATIVE, Vector2(0.35, 0.66), Vector2(qwidth, qheight), levelSelectButtonBackground, NULL);
    
    toggleIndicator->setMetricsMode(GMM_RELATIVE);
    toggleIndicator->setDimensions(0.08, 0.08);
    toggleIndicator->setMaterialName("General/GUIToggleIndicator");
    switch (player->getToggleBack())
    {
        case 0:
            toggleIndicator->setPosition(0.895, 0.45);
            break;
        case 1:
            toggleIndicator->setPosition(0.895, 0.55);
            break;
        case 2:
            toggleIndicator->setPosition(0.895, 0.65);
            break;
        case 3:
            toggleIndicator->setPosition(0.895, 0.75);
            break;
    }
    
    pauseBackground->setMaterialName("General/PauseButton");
    //sliderRangeBackground->setMaterialName("General/SliderRangeHorizontal");
    sliderRangeBackground->setMaterialName("General/SliderRangeVertical");
    sliderBallBackground->setMaterialName("General/SliderBall");
    toggle1Background->setMaterialName("General/GUIToggleButton3");
    toggle2Background->setMaterialName("General/GUIToggleButton2");
    toggle3Background->setMaterialName("General/GUIToggleButton1");
    toggle4Background->setMaterialName("General/GUIToggleButton0");
    powerup1Background->setMaterialName("General/GUIPowerupButtonBlank");
    powerup2Background->setMaterialName("General/GUIPowerupButtonBlank");
    powerup3Background->setMaterialName("General/GUIPowerupButtonBlank");
    
    resumeButtonBackground->setMaterialName("General/ResumeButtonRound");
    nextButtonBackground->setMaterialName("General/NextButtonRound");
    restartButtonBackground->setMaterialName("General/RestartButtonRound");
    levelSelectButtonBackground->setMaterialName("General/LevelSelectButtonRound");
    
    nbackDisplayBackground->setMetricsMode(GMM_RELATIVE);
    nbackDisplayBackground->setPosition(0.897, 0.175);
    nbackDisplayBackground->setDimensions(0.08, 0.08);
    switch (tunnel->getPhase())
    {
        case 'A':
            nbackDisplayBackground->setMaterialName("General/GUIToggleButton0");
            break;
        case 'B':
            nbackDisplayBackground->setMaterialName("General/GUIToggleButton1");
            break;
        case 'C':
            nbackDisplayBackground->setMaterialName("General/GUIToggleButton2");
            break;
        case 'D':
            nbackDisplayBackground->setMaterialName("General/GUIToggleButton3");
            break;
        default:
            nbackDisplayBackground->setMaterialName("General/GUIToggleButton0");
            break;
    }
    
    nbackDisplayLabel->setMetricsMode(GMM_RELATIVE);
    nbackDisplayLabel->setAlignment(TextAreaOverlayElement::Center);
    nbackDisplayLabel->setPosition(0.04, 0.0225);
    nbackDisplayLabel->setCharHeight(0.03 * FONT_SZ_MULT);
    nbackDisplayLabel->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    nbackDisplayLabel->setFontName("Arial");
    nbackDisplayLabel->setCaption(Util::toStringInt(tunnel->getHighestCriteria()));
    
    // NOTE: should have a button member function called inactivate so it deactivates
    // in the queryGUI function! Hiding currently works since the effect of toggle
    // is negligible in other logic
    toggle1TextArt->hide();
    toggle2TextArt->hide();
    toggle3TextArt->hide();
    toggle4TextArt->hide();
    toggleIndicator->hide();
    powerup1Background->hide();
    powerup2Background->hide();
    powerup3Background->hide();
    if (tunnel->getMode() != STAGE_MODE_RECESS)
    {
        if (tunnel->isMultiCollectionTask())
        {
            // Bad hack but
            // Don't show 3-Back for multi-collection tasks of 1 or less.
            if (player->getLevelRequestRow() > 0)
                toggle1TextArt->show();
            toggle2TextArt->show();
            toggle3TextArt->show();
            toggle4TextArt->show();
            toggleIndicator->show();
        }
        else
        {
            switch (player->getToggleBack())
            {
                case 0:
                    toggle1TextArt->show();
                    break;
                case 1:
                    toggle2TextArt->show();
                    break;
                case 2:
                    toggle3TextArt->show();
                    break;
                case 3:
                    toggle4TextArt->show();
                    break;
            }
            toggleIndicator->hide();
        }
    }
}

void HudStage::link(Player* player, Tunnel* tunnel)
{
    this->player = player;
    this->tunnel = tunnel;
}

void HudStage::unlink()
{
    this->player = NULL;
    this->tunnel = NULL;
}

bool HudStage::isGoButtonActive() const
{
    return goButtonActive;
}

void HudStage::notifyGoButton(bool active)
{
    goButtonActive = active;
    buttons[BUTTON_GO].setActive(goButtonActive);
    if (goButtonActive)
    {
        buttons[BUTTON_GO].show();
        goBackground->setMaterialName("General/ButtonGo");
    }
    else
    {
        buttons[BUTTON_GO].hide();
        goBackground->setMaterialName("General/ButtonGoGray");
    }
}

