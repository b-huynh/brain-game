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
: Hud(), goButtonActive(false), bestScoreAnimationFlag(false), bestScoreAnimationTimer(0.0f), pauseNavOffset(0.7f), pauseNavOffsetDest(0.7f), goOffset(0.7f), goOffsetDest(0.7f)
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
    
    setCollectionBar(false, elapsed);
    
    std::string GUIToggleNumber = "General/GUIToggleTextNumber";
    toggle1TextArt->setMaterialName(GUIToggleNumber + Util::toStringInt(tunnel->getNBack() % 10));
    toggle2TextArt->setMaterialName(GUIToggleNumber + Util::toStringInt(Util::clamp((tunnel->getNBack() - 1) % 10, 0, tunnel->getNBack())));
    toggle3TextArt->setMaterialName(GUIToggleNumber + Util::toStringInt(Util::clamp((tunnel->getNBack() - 2) % 10, 0, tunnel->getNBack())));
    // toggle4 is always n-back 0

    // Set up pause navigation texture with appropriate active buttons
    LevelSet* levels = player->getLevels();
    int row = player->getLevelRequestRow();
    int col = player->getLevelRequestCol();
    int level = levels->getLevelNo(row, col);
    int nlevel = ((level + 1) % NUM_TASKS) != 5 ? level + 1 : level + 2;
    bool nextAvail = player->isLevelAvailable(nlevel);
    bool resumeAvail = !tunnel->needsCleaning();
    if (nextAvail && resumeAvail)
        pauseNavigationBackground->setMaterialName("General/PauseNav1");
    else if (!nextAvail && resumeAvail)
        pauseNavigationBackground->setMaterialName("General/PauseNav2");
    else if (nextAvail && !resumeAvail)
        pauseNavigationBackground->setMaterialName("General/PauseNav3");
    else
        pauseNavigationBackground->setMaterialName("General/PauseNav4");
    
    // Pause nav animation
    if (overlays[1]->isVisible())
    {
        float minySpeed = 0.25;
        float yspeed = (pauseNavOffsetDest - pauseNavOffset) * 5.0;
        if (pauseNavOffset > pauseNavOffsetDest)
        {
            if (yspeed > -minySpeed) yspeed = -minySpeed;
            pauseNavOffset += (yspeed * elapsed);
            if (pauseNavOffset < pauseNavOffsetDest)
                pauseNavOffset = pauseNavOffsetDest;
        }
        else if (pauseNavOffset < pauseNavOffsetDest)
        {
            if (yspeed < minySpeed) yspeed = minySpeed;
            pauseNavOffset += (yspeed * elapsed);
            if (pauseNavOffset > pauseNavOffsetDest)
                pauseNavOffset = pauseNavOffsetDest;
        }
        
        float pauseNavHeight = 0.45;
        float pauseNavWidth = pauseNavHeight * globals.screenHeight / globals.screenWidth;
        float pauseNavX = 0.50 - pauseNavWidth / 2;
        float pauseNavY = 0.55 - pauseNavHeight / 2 + pauseNavOffset;
        pauseNavigationBackground->setPosition(pauseNavX, pauseNavY);
        pauseBaseBackground->setPosition(0.325, 0.675 + pauseNavOffset);
    }
    
    // Go button animation
    if (overlays[1]->isVisible())
    {
        float minySpeed = 0.25;
        float yspeed = (goOffsetDest - goOffset) * 5.0;
        if (goOffset > goOffsetDest)
        {
            if (yspeed > -minySpeed) yspeed = -minySpeed;
            goOffset += (yspeed * elapsed);
            if (goOffset < goOffsetDest)
                goOffset = goOffsetDest;
        }
        else if (goOffset < goOffsetDest)
        {
            if (yspeed < minySpeed) yspeed = minySpeed;
            goOffset += (yspeed * elapsed);
            if (goOffset > goOffsetDest)
                goOffset = goOffsetDest;
        }
        
        float gheight = 0.35;
        float gwidth = gheight * globals.screenHeight / globals.screenWidth;
        float gx = 0.50 - gwidth / 2;
        float gy = 0.60 - gheight / 2 + goOffset;
        goBackground->setPosition(gx, gy);
        goBaseBackground->setPosition(0.325, 0.675 + goOffset);
    }
    
    // Update powerup GUI
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
    
    // Grow and shrink score display if it is a high score
    if (bestScoreAnimationFlag)
    {
        bestScoreAnimationTimer += elapsed;
        float sz = 0.024;
        float dsz = 0.002 * Ogre::Math::Sin(3 * Ogre::Math::PI * bestScoreAnimationTimer) + 0.002;
        endTallyScoreValue->setCharHeight((sz + dsz) * FONT_SZ_MULT);
        endTallyScoreValue->setPosition(0.19, 0.11 - dsz);
        
        endTallyScoreLabel->setCharHeight((sz + dsz) * FONT_SZ_MULT);
        endTallyScoreLabel->setPosition(0.355 + dsz, 0.11 - dsz);
    }
    
    // If player is going through score calculation animation go through this
    if(player->endFlag)
    {
        if (tunnel->getEval() == PASS)
        {
            setOverlay(3, true);
        
            tunnel->addToTimePenalty(2.0f);
            float timeLeft = tunnel->getStageTime() - tunnel->getTotalElapsed() - tunnel->getTimePenalty();
        
            if( timeLeft < 0.0f && timeLeft > -1.0f) player->setScore(player->getScore()+100.0f);
            else if( timeLeft > 0.0f ) player->setScore(player->getScore()+200.0f);
        
            label2->setColour(ColourValue(1.0,1.0,0.0));
            label5->setColour(ColourValue(1.0,1.0,0.0));
            
            label2->setCaption(Util::toStringInt(timeLeft));
            endTallyTimeLabel->setCaption("Time    " + Util::toStringInt(timeLeft));
        
            label5->setCaption(Util::toStringInt(player->getScore()));
            
            float highScore = player->getLevelProgress(player->getLevelRequestRow(), player->getLevelRequestCol()).score;
            endTallyScoreLabel->setCaption("Best");
            if (player->getScore() > highScore)
            {
                highScore = player->getScore();
                bestScoreAnimationFlag = true;
            }
            endTallyScoreValue->setCaption(Util::toStringInt(highScore));
        
            if( timeLeft <= 0.0f ) {
                label2->setCaption("0");
                endTallyTimeLabel->setCaption("Time    0");
                tunnel->setCleaning(true);
                player->endFlag = false;
            }
        }
        else
        {
            setOverlay(3, true);
            
            label2->setColour(ColourValue(1.0,1.0,0.0));
            label5->setColour(ColourValue(1.0,1.0,0.0));
            
            label2->setCaption(Util::toStringInt(timeLeft));
            endTallyTimeLabel->setCaption("Time    " + Util::toStringInt(timeLeft));
            
            label5->setCaption(Util::toStringInt(player->getScore()));
            
            float highScore = player->getLevelProgress(player->getLevelRequestRow(), player->getLevelRequestCol()).score;
            endTallyScoreLabel->setCaption("Best");
            if (player->getScore() > highScore)
            {
                highScore = player->getScore();
                bestScoreAnimationFlag = true;
            }
            endTallyScoreValue->setCaption(Util::toStringInt(highScore));
            
            tunnel->setCleaning(true);
            player->endFlag = false;
        }
    }
    // Update these message as long as the tunnel generation is not over
    else if (!tunnel->isDone())
    {
        label1->setCaption(globals.messageBig);
        
        label5->setCaption(Util::toStringInt(player->getScore()));
        label6->setCaption(globals.message);
        
        player->xsTimer -= elapsed;
        std::string xs = "";
        for (int i = 0; i < globals.startingHP - player->getHP(); ++i)
        {
            xs += "x";
        }
        if (tunnel->getMode() != STAGE_MODE_RECESS && player->xsTimer > 0.0f)
        {
            label7->setCaption(xs);
            label7->setColour(ColourValue::ColourValue(1.0, 0.0, 0.0, player->xsTimer));
        }
        else
            label7->setCaption("");
        
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
    }
}

void HudStage::alloc()
{
    // Allocate Resources
    panelText = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageTextInterface"));
    panelText->setMetricsMode(GMM_PIXELS);
    panelText->setPosition(10, 10);
    panelText->setDimensions(10, 10);
    
    pauseBackground = static_cast<PanelOverlayElement*>(
                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePauseBackground"));
    goBaseBackground = static_cast<PanelOverlayElement*>(
                                                         OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageGoBaseBackground"));
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
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem8")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem9")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem10")));
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem11")));

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
    
    endTallyScoreValue = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "EndTallyScoreValue"));
    
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
    
    pauseBaseBackground = static_cast<PanelOverlayElement*>(
                                                            OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePauseBaseBackground"));
    pauseNavigationBackground = static_cast<PanelOverlayElement*>(
                                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePauseNavigationBackground"));
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
    endTallyBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "EndTallyBackground"));
    overlay4->add2D(endTallyBackground);
    overlay4->add2D(endTallyContainer);
    endTallyContainer->addChild(endTallyTimeLabel);
    endTallyContainer->addChild(endTallyScoreLabel);
    endTallyContainer->addChild(endTallyScoreValue);
    
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
    overlay2->add2D(goBaseBackground);
    overlay2->add2D(goBackground);
    overlay2->add2D(pauseBaseBackground);
    overlay2->add2D(pauseNavigationBackground);
    pauseNavigationBackground->addChild(resumeButtonBackground);
    pauseNavigationBackground->addChild(nextButtonBackground);
    pauseNavigationBackground->addChild(restartButtonBackground);
    pauseNavigationBackground->addChild(levelSelectButtonBackground);
    
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
    //std::cout << "Performance: " << levelPerformance.initSpeedSetting << std::endl;
    if (levelPerformance.initSpeedSetting >= 0)
        speedSlider->setBallPosition(levelPerformance.initSpeedSetting);
    else
        speedSlider->setBallPosition(globals.initCamSpeed);
    player->setBaseSpeed(speedSlider->getIndex());
    std::vector<CollectionCriteria> criterias = tunnel->getCollectionCriteria();
    setCollectionBar(true, 0.0f); // Do not do in InitOverlay, it is called constantly on IPads
}

void HudStage::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pauseBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(goBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(goBaseBackground);
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
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(endTallyScoreValue);
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
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pauseBaseBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pauseNavigationBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(resumeButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(nextButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(restartButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSelectButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(endTallyBackground);
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

// Reinitializes overlay resources. Also called when screen is adjusted
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
    endTallyContainer->setPosition(0.35, 0.15);
    endTallyContainer->setDimensions(0.25, 0.25);
    
    GUITopPanel->setMetricsMode(GMM_RELATIVE);
    GUITopPanel->setPosition(0.0, 0.0);
    GUITopPanel->setDimensions(1.0, 1.0);
    GUITopPanel->setMaterialName("General/GUIMainHud");
    
    label1->setMetricsMode(GMM_RELATIVE);
    label1->setAlignment(TextAreaOverlayElement::Center);
    label1->setPosition(0.50, 0.20);
    label1->setCharHeight(0.064 * FONT_SZ_MULT);
    label1->setFontName("MainBig");
    label1->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    
    label2->setMetricsMode(GMM_RELATIVE);
    label2->setAlignment(TextAreaOverlayElement::Left);
    label2->setPosition(0.05, 0.040);
    label2->setCharHeight(0.025 * FONT_SZ_MULT);
    label2->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label2->setFontName("MainSmall");
    
    label2prompt->setMetricsMode(GMM_RELATIVE);
    label2prompt->setAlignment(TextAreaOverlayElement::Left);
    label2prompt->setPosition(0.06, 0.085);
    label2prompt->setCharHeight(0.02 * FONT_SZ_MULT);
    label2prompt->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label2prompt->setFontName("MainSmall");
    label2prompt->setCaption("Time");
    
    label3->setMetricsMode(GMM_RELATIVE);
    label3->setAlignment(TextAreaOverlayElement::Right);
    label3->setPosition(0.20, 0.085);
    label3->setCharHeight(0.02 * FONT_SZ_MULT);
    label3->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label3->setFontName("MainSmall");
    
    label4->setMetricsMode(GMM_RELATIVE);
    label4->setAlignment(TextAreaOverlayElement::Center);
    label4->setPosition(0.042, 0.175);
    label4->setCharHeight(0.02 * FONT_SZ_MULT);
    label4->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label4->setFontName("MainSmall");
    label4->setCaption(Util::toStringInt(speedSlider->getIndex()));
    
    label5->setMetricsMode(GMM_RELATIVE);
    label5->setAlignment(TextAreaOverlayElement::Right);
    label5->setPosition(0.92, 0.040);
    label5->setCharHeight(0.025 * FONT_SZ_MULT);
    label5->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label5->setFontName("MainSmall");
    
    label5prompt->setMetricsMode(GMM_RELATIVE);
    label5prompt->setAlignment(TextAreaOverlayElement::Right);
    label5prompt->setPosition(0.92, 0.085);
    label5prompt->setCharHeight(0.02 * FONT_SZ_MULT);
    label5prompt->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label5prompt->setFontName("MainSmall");
    label5prompt->setCaption("Score");
    
    label6->setMetricsMode(GMM_PIXELS);
    label6->setAlignment(TextAreaOverlayElement::Center);
    label6->setPosition(globals.label6_posX, globals.label6_posY);
    label6->setCharHeight(globals.screenHeight / 50 * FONT_SZ_MULT);
    label6->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    label6->setFontName("MainSmall");
    
    label7->setMetricsMode(GMM_RELATIVE);
    label7->setAlignment(TextAreaOverlayElement::Center);
    label7->setPosition(0.5, 0.2);
    label7->setCharHeight(0.064 * FONT_SZ_MULT);
    label7->setFontName("MainSmall");
    
    timeWarpLabel->setMetricsMode(GMM_RELATIVE);
    timeWarpLabel->setAlignment(TextAreaOverlayElement::Center);
    timeWarpLabel->setPosition(0, 0);
    timeWarpLabel->setCharHeight(0.05 * FONT_SZ_MULT);
    timeWarpLabel->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    timeWarpLabel->setFontName("MainSmall");
    
    endTallyTimeLabel->setMetricsMode(GMM_RELATIVE);
    endTallyTimeLabel->setAlignment(TextAreaOverlayElement::Left);
    endTallyTimeLabel->setPosition(0.0, 0.015);
    endTallyTimeLabel->setCharHeight(0.024 * FONT_SZ_MULT);
    endTallyTimeLabel->setColour(ColourValue::ColourValue(1.0,1.0,0.0));
    endTallyTimeLabel->setFontName("MainSmall");
    
    endTallyScoreLabel->setMetricsMode(GMM_RELATIVE);
    endTallyScoreLabel->setAlignment(TextAreaOverlayElement::Right);
    //endTallyScoreLabel->setPosition(0.365, 0.11);
    endTallyScoreLabel->setPosition(0.355, 0.11);
    endTallyScoreLabel->setCharHeight(0.024 * FONT_SZ_MULT);
    endTallyScoreLabel->setColour(ColourValue::ColourValue(1.0,1.0,0.0));
    endTallyScoreLabel->setFontName("MainSmall");
    
    endTallyScoreValue->setMetricsMode(GMM_RELATIVE);
    endTallyScoreValue->setAlignment(TextAreaOverlayElement::Center);
    endTallyScoreValue->setPosition(0.19, 0.11);
    endTallyScoreValue->setCharHeight(0.024 * FONT_SZ_MULT);
    endTallyScoreValue->setColour(ColourValue::ColourValue(1.0,1.0,0.0));
    endTallyScoreValue->setFontName("MainSmall");
    
    endTallyBackground->setMaterialName("General/EndTallyBackground");
    endTallyBackground->setMetricsMode(GMM_RELATIVE);
    endTallyBackground->setPosition(0.30, 0.125);
    endTallyBackground->setDimensions(0.45,0.20);
    
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
    
    float pauseNavHeight = 0.45;
    float pauseNavWidth = pauseNavHeight * globals.screenHeight / globals.screenWidth;
    float pauseNavX = 0.50 - pauseNavWidth / 2;
    float pauseNavY = 0.54 - pauseNavHeight / 2 + pauseNavOffset;
    pauseNavigationBackground->setMetricsMode(GMM_RELATIVE);
    pauseNavigationBackground->setPosition(pauseNavX, pauseNavY);
    pauseNavigationBackground->setDimensions(pauseNavWidth, pauseNavHeight);
    
    pauseBaseBackground->setMetricsMode(GMM_RELATIVE);
    pauseBaseBackground->setPosition(0.325, 0.675 + pauseNavOffset);
    pauseBaseBackground->setDimensions(0.35, 0.35);
    
    float pauseheight = 0.10;
    float pausewidth = pauseheight * globals.screenHeight / globals.screenWidth;
    buttons[BUTTON_PAUSE].setButton("pause", overlays[0], GMM_RELATIVE, Vector2(0.9025, 0.79), Vector2(pausewidth, pauseheight), pauseBackground, NULL);
    
    float gheight = 0.35;
    float gwidth = gheight * globals.screenHeight / globals.screenWidth;
    float gx = 0.50 - gwidth / 2;
    float gy = 0.50 - gheight / 2 + goOffset;
    buttons[BUTTON_GO].setButton("go", overlays[1], GMM_RELATIVE, Vector2(gx, gy), Vector2(gwidth, gheight), goBackground, NULL);
    
    goBaseBackground->setMetricsMode(GMM_RELATIVE);
    goBaseBackground->setPosition(0.325, 0.675 + goOffset);
    goBaseBackground->setDimensions(0.35, 0.35);
    
    buttons[BUTTON_TOGGLE1].setButton("toggle1", overlays[0], GMM_RELATIVE, Vector2(0.897, 0.31), Vector2(0.08, 0.08), toggle1Background, NULL);
    buttons[BUTTON_TOGGLE2].setButton("toggle2", overlays[0], GMM_RELATIVE, Vector2(0.897, 0.43), Vector2(0.08, 0.08), toggle2Background, NULL);
    buttons[BUTTON_TOGGLE3].setButton("toggle3", overlays[0], GMM_RELATIVE, Vector2(0.897, 0.55), Vector2(0.08, 0.08), toggle3Background, NULL);
    buttons[BUTTON_TOGGLE4].setButton("toggle4", overlays[0], GMM_RELATIVE, Vector2(0.897, 0.67), Vector2(0.08, 0.08), toggle4Background, NULL);
    float pheight = 0.12;
    float pwidth = pheight * globals.screenHeight / globals.screenWidth;
    buttons[BUTTON_POWERUP1].setButton("powerup1", overlays[0], GMM_RELATIVE, Vector2(0.015, 0.45), Vector2(pwidth, pheight), powerup1Background, NULL);
    buttons[BUTTON_POWERUP2].setButton("powerup2", overlays[0], GMM_RELATIVE, Vector2(0.015, 0.60), Vector2(pwidth, pheight), powerup2Background, NULL);
    buttons[BUTTON_POWERUP3].setButton("powerup3", overlays[0], GMM_RELATIVE, Vector2(0.015, 0.75), Vector2(pwidth, pheight), powerup3Background, NULL);
    
    float qheight = 0.085;
    float qwidth = qheight * globals.screenHeight / globals.screenWidth;
    // Sets positions in local 2D space (relative to entire pause nav)
    buttons[BUTTON_RESUME].setButton("resume", overlays[1], GMM_RELATIVE, Vector2(0.4675 - pauseNavX, 0.4075 - pauseNavY + pauseNavOffset), Vector2(qwidth, qheight), resumeButtonBackground, NULL);
    buttons[BUTTON_NEXT].setButton("next", overlays[1], GMM_RELATIVE, Vector2(0.54 - pauseNavX, 0.50 - pauseNavY + pauseNavOffset), Vector2(qwidth, qheight), nextButtonBackground, NULL);
    buttons[BUTTON_RESTART].setButton("restart", overlays[1], GMM_RELATIVE, Vector2(0.395 - pauseNavX, 0.50 - pauseNavY + pauseNavOffset), Vector2(qwidth, qheight), restartButtonBackground, NULL);
    buttons[BUTTON_LEVELSELECT].setButton("levelselect", overlays[1], GMM_RELATIVE, Vector2(0.4675 - pauseNavX, 0.59 - pauseNavY + pauseNavOffset), Vector2(qwidth, qheight), levelSelectButtonBackground, NULL);
    // Sets positions in global 2D space
    //buttons[BUTTON_RESUME].setButton("resume", overlays[1], GMM_RELATIVE, Vector2(0.4675, 0.4075), Vector2(qwidth, qheight), resumeButtonBackground, NULL);
    //buttons[BUTTON_NEXT].setButton("next", overlays[1], GMM_RELATIVE, Vector2(0.54, 0.50), Vector2(qwidth, qheight), nextButtonBackground, NULL);
    //buttons[BUTTON_RESTART].setButton("restart", overlays[1], GMM_RELATIVE, Vector2(0.395, 0.50), Vector2(qwidth, qheight), restartButtonBackground, NULL);
    //buttons[BUTTON_LEVELSELECT].setButton("levelselect", overlays[1], GMM_RELATIVE, Vector2(0.4675, 0.59), Vector2(qwidth, qheight), levelSelectButtonBackground, NULL);
    
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
    sliderRangeBackground->setMaterialName("General/SpeedSliderRangeVertical");
    sliderBallBackground->setMaterialName("General/SpeedSliderBall");
    toggle1Background->setMaterialName("General/GUIToggleButton3");
    toggle2Background->setMaterialName("General/GUIToggleButton2");
    toggle3Background->setMaterialName("General/GUIToggleButton1");
    toggle4Background->setMaterialName("General/GUIToggleButton0");
    powerup1Background->setMaterialName("General/GUIPowerupButtonBlank");
    powerup2Background->setMaterialName("General/GUIPowerupButtonBlank");
    powerup3Background->setMaterialName("General/GUIPowerupButtonBlank");
    pauseBaseBackground->setMaterialName("General/PauseBase");
    goBaseBackground->setMaterialName("General/PauseBase");
    
    //resumeButtonBackground->setMaterialName("General/ResumeButtonRound");
    //nextButtonBackground->setMaterialName("General/NextButtonRound");
    //restartButtonBackground->setMaterialName("General/RestartButtonRound");
    //levelSelectButtonBackground->setMaterialName("General/LevelSelectButtonRound");
    
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
    nbackDisplayLabel->setFontName("MainSmall");
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

void HudStage::setGoButtonState(bool active, bool color)
{
    goButtonActive = active;
    buttons[BUTTON_GO].setActive(goButtonActive);
    if (goButtonActive)
    {
        goOffsetDest = 0.0;
        buttons[BUTTON_GO].show();
    }
    else
    {
        goOffsetDest = 0.7;
        buttons[BUTTON_GO].hide();
    }
    if (color)
        goBackground->setMaterialName("General/ButtonGo");
    else
        goBackground->setMaterialName("General/ButtonGoGray");
}

void HudStage::setPauseNavDest(float navOffset)
{
    pauseNavOffsetDest = navOffset;
}

// Sets the collection bar positions and images.
// If instant parameter is true, the positions will be set instantly,
// otherwise it will animate towards their designated location at a speed.
void HudStage::setCollectionBar(bool instant, float elapsed)
{
    int numSatisfied = tunnel->getNumSatisfiedCriteria();
    
    // How many has the player collected?
    int starPhase = 0;
    if (numSatisfied < 3)
        starPhase = 0;
    else if (numSatisfied < 6)
        starPhase = 1;
    else if (numSatisfied < 9)
        starPhase = 2;
    else if (numSatisfied < 12)
        starPhase = 3;
    else
        starPhase = 4;
    
    // Designate positions for each collection item
    float x = 0.2925;
    float y = 0.035;
    float wpadding = 0.0125;
    float width = 0.0425;
    float height = 0.075;
    // When we win, almost all of them are collapsed,
    // so we want to re-adjust so it looks evenly split.
    if (starPhase == 4)
    {
        x = 0.3125;
        wpadding = 0.0250;
    }
    
    std::vector<CollectionCriteria> criterias = tunnel->getCollectionCriteria();
    for (int i = 0; i < collectionBar.size(); ++i)
    {
        collectionBar[i]->setMetricsMode(GMM_RELATIVE);
        collectionBar[i]->setDimensions(width, height);
        if (instant)
        {
            collectionBar[i]->setPosition(x + wpadding, y);
        }
        else
        {
            // Move from current position towards destination
            Vector2 to = Vector2(x + wpadding, y);
            Vector2 from = Vector2(collectionBar[i]->getLeft(), collectionBar[i]->getTop());
            Vector2 delta = (to - from);
            if (delta.squaredLength() <= 0.000005f)
                collectionBar[i]->setPosition(to.x, to.y);
            else
            {
                Vector2 npos = from + delta * 3 * elapsed;
                collectionBar[i]->setPosition(npos.x, npos.y);
            }
        }
        
        if (i / 3 == starPhase || (i + 1) % 3 == 0)
            x += 2 * wpadding + width;

        // Assign an image based on what type of collection and whether it is collected
        if (i < criterias.size())
        {
            std::string scoreName = "General/GUICollection";
            
            // For togglebacks
            //if (criterias[i].nback <= 0)
            //    scoreName += Util::toStringInt(0);
            //else
            //    scoreName += Util::toStringInt(Util::clamp(3 - (tunnel->getNBack() - criterias[i].nback), 0, 3));
            
            // For just a single n-back and task
            switch (i / 3)
            {
                case 0:
                    scoreName += "0";
                    break;
                case 1:
                    scoreName += "1";
                    break;
                case 2:
                    scoreName += "2";
                    break;
                case 3:
                    scoreName += "3";
                    break;
                default:
                    scoreName += "0";
                    break;
            }
            
            if (criterias[i].collected)
                scoreName += "Filled3";
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
