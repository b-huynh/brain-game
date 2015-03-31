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
: Hud(), goButtonActive(false), pauseNavOffset(0.7f), pauseNavOffsetDest(0.7f), goOffset(0.7f), goOffsetDest(0.7f), leftZapT(0.0f), rightZapT(0.0f), prevFuelTimer(globals.fuelMax), fuelBarAnimationTimer(0.0f)
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
    label4->setCaption(Util::toStringInt(player->getBaseSpeed()));
    if (player->hasTriggeredStartup())
    {
        speedSlider->setBallDestination(player->getBaseSpeed());
        speedSlider->update(elapsed);
    }
    speedSlider->adjust();
    
    setCollectionBar(false, elapsed);
    setFuelBar(elapsed);
    
    if (tunnel->getMode() != STAGE_MODE_RECESS)
    {
        if (leftZapT > 0.0f)
        {
            HudLeftZapper->setMaterialName("General/GUIMainHudLeftShifterPressed");
            leftZapT -= elapsed;
        }
        else
            HudLeftZapper->setMaterialName("General/GUIMainHudLeftShifter");
        if (rightZapT > 0.0f)
        {
            HudRightZapper->setMaterialName("General/GUIMainHudRightShifterPressed");
            rightZapT -= elapsed;
        }
        else
            HudRightZapper->setMaterialName("General/GUIMainHudRightShifter");
    }
    else
    {
        HudLeftZapper->setMaterialName("General/GUIMainHudShifterOff");
        HudRightZapper->setMaterialName("General/GUIMainHudShifterOff");
    }
    
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
        sliderRangeBackground->setPosition(0.20, 0.35 + goOffset);
    }
    
    // Update powerup GUI
    float timeLeft = fmax(tunnel->getTimeLeft(), 0.0f);
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
    
    // Update these message as long as the tunnel generation is not over
    if (!tunnel->isDone())
    {
        label1->setCaption(globals.messageBig);
        
        player->xsTimer -= elapsed;
        std::string xs = "";
        for (int i = 0; i < globals.startingHP - player->getHP(); ++i)
        {
            xs += "x";
        }
        if (tunnel->getMode() != STAGE_MODE_RECESS && player->xsTimer > 0.0f)
        {
            label6->setCaption(xs);
            label6->setColour(ColourValue::ColourValue(1.0, 0.0, 0.0, player->xsTimer));
        }
        else
            label6->setCaption("");
        
        label7->setCaption(globals.message);
        label7->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0, 1.0));
        label7->setCharHeight(0.025 * FONT_SZ_MULT);
        

    }
    else if (!tunnel->needsCleaning())
    {
        if (tunnel->getEval() != PASS)
        {
    
            if (player->getHP() <= 0)
            {
                label7->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0, 1.0));
                label7->setCharHeight(0.025 * FONT_SZ_MULT);
                label7->setCaption("\nBlast\nToo many unstable zaps");
            }
            else if (tunnel->getEval() == EVEN)
            {
                label7->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0, 1.0));
                label7->setCharHeight(0.025 * FONT_SZ_MULT);
                label7->setCaption("\nBlast\nWe've run out of Fuel");
            }
            else if (tunnel->getEval() == FAIL && tunnel->getTimeLeft() <= 0.0f)
            {
                label7->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0, 1.0));
                label7->setCharHeight(0.025 * FONT_SZ_MULT);
                label7->setCaption("\nBlast\nWe've run out of Time");
            }
        }
    }
    else //if (tunnel->needsCleaning())
    {
        label7->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0, 1.0));
        label7->setCharHeight(0.025 * FONT_SZ_MULT);
        
        bool retryEnabled = !pausePanelBack->isVisible(); // !IsVisible means the gray version is not visible
        if (tunnel->getEval() == PASS || player->getAccuracy() >= 0.75)
        {
            if (retryEnabled)
                label7->setCaption("\nYou may play again or continue.");
            else
                label7->setCaption("\nContinue to the next wormhole.");
        }
        else
        {
            if (retryEnabled)
                label7->setCaption("\nTry again or continue?");
            else
                label7->setCaption("\nContinue to the next wormhole.");
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
    collectionBar.push_back(static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageCollectionItem12")));
    
    // Allocate the hud elements
    HudEntire = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHudEntire"));
    HudTopPanel = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHudTopPanel"));
    HudLeftPanel = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHudLeftPanel"));
    HudRightPanel = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHudRightPanel"));
    HudLeftZapper = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHudLeftZapper"));
    HudRightZapper = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHudRightZapper"));
    /*
    for (int i = 0; i < globals.startingHP; ++i)
    {
        PanelOverlayElement* fuelContainer = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHudFuelContainer" + Util::toStringInt(i)));
        PanelOverlayElement* fuelBar = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHudFuelBar" + Util::toStringInt(i)));
        HudFuelContainers.push_back(fuelContainer);
        HudFuelBars.push_back(fuelBar);
    }
    */
    HudFuelBar = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageHudFuelBar"));
    
    // Allocate text area elements
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
    
    nbackDisplayBackground = static_cast<PanelOverlayElement*>(
                                                               OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageNBackDisplayBackground"));
    nbackDisplayLabel = static_cast<TextAreaOverlayElement*>(
                                                             OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "StageNBackDisplayLabel"));
    
    pauseBaseBackground = static_cast<PanelOverlayElement*>(
                                                            OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePauseBaseBackground"));
    pauseNavigationBackground = static_cast<PanelOverlayElement*>(
                                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePauseNavigationBackground"));

    pausePanelPlay = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePausePanelPlay"));
    pausePanelForward = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePausePanelForward"));
    pausePanelBack = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePausePanelBack"));
    pausePanelMenu = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StagePausePanelMenu"));
    
    resumeButtonBackground = static_cast<PanelOverlayElement*>(
                                                               OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageResumeButtonBackground"));
    nextButtonBackground = static_cast<PanelOverlayElement*>(
                                                             OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageNextButtonBackground"));
    restartButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageRestartButtonBackground"));
    levelSelectButtonBackground = static_cast<PanelOverlayElement*>(
                                                                    OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageLevelSelectButtonBackground"));
    leftZapperButtonBackground = static_cast<PanelOverlayElement*>(
                                                                OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageLeftZapperButtonBackground"));
    rightZapperButtonBackground = static_cast<PanelOverlayElement*>(
                                                                    OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "StageRightZapperButtonBackground"));
    
    buttons = std::vector<HudButton>(8);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("StageOverlayHUD");
    Overlay* overlay2 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("StageOverlayPauseMenu");
    
    Overlay* overlay3 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("TimeWarpOverlay");
    timeWarpContainer = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "TimeWarpInterface"));
    
    overlay3->add2D(timeWarpContainer);
    timeWarpContainer->addChild(timeWarpLabel);
    
    overlay1->add2D(HudEntire);
    HudEntire->addChild(HudLeftPanel);
    HudEntire->addChild (HudRightPanel);
    HudEntire->addChild(HudTopPanel);
    HudLeftPanel->addChild(leftZapperButtonBackground);
    HudRightPanel->addChild(rightZapperButtonBackground);
    HudLeftPanel->addChild(HudLeftZapper);
    HudRightPanel->addChild(HudRightZapper);
    HudTopPanel->addChild(HudFuelBar);
    
    //for (int i = 0; i < HudFuelContainers.size(); ++i)
    //    HudTopPanel->addChild(HudFuelContainers[i]);
    //for (int i = 0; i < HudFuelBars.size(); ++i)
    //   HudTopPanel->addChild(HudFuelBars[i]);
    
    overlay1->add2D(pauseBackground);
    //for (int i = 0; i < collectionBar.size(); ++i)
    for (int i = collectionBar.size() - 1; i >= 0; --i)
        overlay1->add2D(collectionBar[i]);
    
    overlay1->add2D(nbackDisplayBackground);
    nbackDisplayBackground->addChild(nbackDisplayLabel);
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
    overlay2->add2D(pauseBaseBackground);
    overlay2->add2D(pauseNavigationBackground);
    overlay2->add2D(goBaseBackground);
    overlay2->add2D(goBackground);
    pauseNavigationBackground->addChild(pausePanelPlay);
    pauseNavigationBackground->addChild(pausePanelForward);
    pauseNavigationBackground->addChild(pausePanelBack);
    pauseNavigationBackground->addChild(pausePanelMenu);
    pauseNavigationBackground->addChild(resumeButtonBackground);
    pauseNavigationBackground->addChild(nextButtonBackground);
    pauseNavigationBackground->addChild(restartButtonBackground);
    pauseNavigationBackground->addChild(levelSelectButtonBackground);
    
    overlay1->add2D(sliderRangeBackground);
    sliderRangeBackground->addChild(sliderBallBackground);
    
    overlays.push_back(overlay1);
    overlays.push_back(overlay2);
    overlays.push_back(overlay3);
    
    speedSlider = new HudSlider();
    
    // Horizontal Slider
    speedSlider->setSlider("speed", overlays[1], Vector2(0.20, 0.35), Vector2(0.60, 0.03), Vector2(0.05, 0.03), false,
                           globals.minCamSpeed, globals.maxCamSpeed, globals.maxCamSpeed - globals.minCamSpeed + 1, sliderRangeBackground, sliderBallBackground);
    
    // Set the ball position to the previous speed setting if the player played this level before
    // Otherwise, use the suggested speed
    PlayerProgress levelPerformance = player->getLevelProgress(player->getLevelRequestRow(), player->getLevelRequestCol());
    speedSlider->adjust();
    //std::cout << "Performance: " << levelPerformance.initSpeedSetting << std::endl;
    
    if(player->levelRequest)
    {
        // Set starting speed based on speeds tracked from the scheduler
        // Each task column tracks a speed
        double startingSpeed = 15.0;
        
        // If assigned a specific level (via scheduler)
        switch (player->levelRequest->first.phaseX)
        {
            case PHASE_COLOR_SOUND:
            {
                if (!player->scheduler->firstTimeA)
                    startingSpeed = player->scheduler->speedA;
                else
                    startingSpeed = player->scheduler->predictAverageStartingSpeed(globals.initialVelocity);
                break;
            }
            case PHASE_SHAPE_SOUND:
            {
                if (!player->scheduler->firstTimeB)
                    startingSpeed = player->scheduler->speedB;
                else
                    startingSpeed = player->scheduler->predictAverageStartingSpeed(globals.initialVelocity);
                break;
            }
            case PHASE_SOUND_ONLY:
            {
                if (!player->scheduler->firstTimeC)
                    startingSpeed = player->scheduler->speedC;
                else
                    startingSpeed = player->scheduler->predictAverageStartingSpeed(globals.initialVelocity);
                break;
            }
            case PHASE_ALL_SIGNAL:
            {
                if (!player->scheduler->firstTimeD)
                    startingSpeed = player->scheduler->speedD;
                else
                    startingSpeed = player->scheduler->predictAverageStartingSpeed(globals.initialVelocity);
                break;
            }
            case PHASE_COLLECT:
            {
                if (!player->scheduler->firstTimeE)
                    startingSpeed = player->scheduler->speedE;
                else
                    startingSpeed = player->scheduler->predictAverageStartingSpeed(globals.initialVelocity);
                break;
            }
            default:
                speedSlider->setBallPosition(globals.initCamSpeed);
                break;
        }
        
        speedSlider->setBallPosition(startingSpeed);
    }
    else
    {
        // If level played by 2-D grid select
        if (levelPerformance.initSpeedSetting >= 0)
            speedSlider->setBallPosition(levelPerformance.initSpeedSetting);
        else
            speedSlider->setBallPosition(globals.initCamSpeed);
    }
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
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(HudEntire);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(HudTopPanel);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(HudLeftPanel);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(HudRightPanel);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(HudLeftZapper);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(HudRightZapper);
    /*
    for (int i = 0; i < HudFuelContainers.size(); ++i)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(HudFuelContainers[i]);
    for (int i = 0; i < HudFuelBars.size(); ++i)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(HudFuelBars[i]);
    HudFuelContainers.clear();
    HudFuelBars.clear();
    */
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(HudFuelBar);
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
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(nbackDisplayBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(nbackDisplayLabel);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pauseBaseBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pauseNavigationBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pausePanelPlay);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pausePanelForward);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pausePanelBack);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(pausePanelMenu);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(resumeButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(nextButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(restartButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSelectButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(leftZapperButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(rightZapperButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(panelText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(timeWarpContainer);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[1]);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[2]);
    if (speedSlider) delete speedSlider;
    speedSlider = NULL;
}

// Reinitializes overlay resources. Also called when screen is adjusted
void HudStage::initOverlay()
{
    float AR = static_cast<float>(globals.screenHeight) / globals.screenWidth;
    
    // Link and Set Resources
    panelText->setMetricsMode(GMM_PIXELS);
    panelText->setPosition(10, 10);
    panelText->setDimensions(10, 10);
    
    timeWarpContainer->setMetricsMode(GMM_RELATIVE);
    timeWarpContainer->setPosition(0.35, 0.15);
    timeWarpContainer->setDimensions(0.25, 0.25);
    
    //HudEntire->setMetricsMode(GMM_RELATIVE);
    //HudEntire->setPosition(0.0, 0.0);
    //HudEntire->setDimensions(1.0, 1.0);
    //HudEntire->setMaterialName("General/GUIMainHudEntire");
    
    HudLeftPanel->setMetricsMode(GMM_RELATIVE);
    HudLeftPanel->setPosition(0.000, 0.000);
    HudLeftPanel->setDimensions(0.260, 1.000);
    HudLeftPanel->setMaterialName("General/GUIMainHudLeft");
    
    HudTopPanel->setMetricsMode(GMM_RELATIVE);
    HudTopPanel->setPosition(0.000, 0.030);
    HudTopPanel->setDimensions(1.000, 0.101);
    HudTopPanel->setMaterialName("General/GUIMainHudBar");
    
    HudRightPanel->setMetricsMode(GMM_RELATIVE);
    HudRightPanel->setPosition(0.740, 0.000);
    HudRightPanel->setDimensions(0.260, 1.000);
    HudRightPanel->setMaterialName("General/GUIMainHudRight");
    
    HudFuelBar->setMetricsMode(GMM_RELATIVE);
    HudFuelBar->setPosition(0.760, 0.013);
    HudFuelBar->setMaterialName("General/GUIFuelBar");
    
    float leftZapperX = 0.016;
    float leftZapperY = 0.000;
    float rightZapperX = 0.178;
    float rightZapperY = 0.000;
    float dimZapperX = 0.066;
    float dimZapperY = 1.000;
    HudLeftZapper->setMetricsMode(GMM_RELATIVE);
    HudLeftZapper->setPosition(leftZapperX, leftZapperY);
    HudLeftZapper->setDimensions(dimZapperX, dimZapperY);
    
    HudRightZapper->setMetricsMode(GMM_RELATIVE);
    HudRightZapper->setPosition(rightZapperX, rightZapperY);
    HudRightZapper->setDimensions(dimZapperX, dimZapperY);
    
    label1->setMetricsMode(GMM_RELATIVE);
    label1->setAlignment(TextAreaOverlayElement::Center);
    label1->setPosition(0.50, 0.20);
    label1->setCharHeight(0.064 * FONT_SZ_MULT);
    label1->setFontName("MainBig");
    label1->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    
    label2->setMetricsMode(GMM_RELATIVE);
    label2->setAlignment(TextAreaOverlayElement::Left);
    label2->setPosition(0.060, 0.040);
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
    label4->setPosition(0.040, 0.215);
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
    label5prompt->setCaption("Fuel");
    
    label6->setMetricsMode(GMM_RELATIVE);
    label6->setAlignment(TextAreaOverlayElement::Center);
    label6->setPosition(0.5, 0.3);
    label6->setCharHeight(0.064 * FONT_SZ_MULT);
    label6->setFontName("MainSmall");
    
    label7->setMetricsMode(GMM_RELATIVE);
    label7->setAlignment(TextAreaOverlayElement::Center);
    label7->setPosition(0.50, 0.12);
    label7->setFontName("MainSmall");
    
    timeWarpLabel->setMetricsMode(GMM_RELATIVE);
    timeWarpLabel->setAlignment(TextAreaOverlayElement::Center);
    timeWarpLabel->setPosition(0, 0);
    timeWarpLabel->setCharHeight(0.05 * FONT_SZ_MULT);
    timeWarpLabel->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    timeWarpLabel->setFontName("MainSmall");
    
    float pauseNavHeight = 0.45;
    float pauseNavWidth = pauseNavHeight * AR;
    float pauseNavX = 0.50 - pauseNavWidth / 2;
    float pauseNavY = 0.54 - pauseNavHeight / 2 + pauseNavOffset;
    pauseNavigationBackground->setMetricsMode(GMM_RELATIVE);
    pauseNavigationBackground->setPosition(pauseNavX, pauseNavY);
    pauseNavigationBackground->setDimensions(pauseNavWidth, pauseNavHeight);
    
    pauseBaseBackground->setMetricsMode(GMM_RELATIVE);
    pauseBaseBackground->setPosition(0.325, 0.675 + pauseNavOffset);
    pauseBaseBackground->setDimensions(0.35, 0.35);
    // Little Panel Specifics
    float len1 = 0.165;
    float len2 = 0.100;
    pausePanelPlay->setMetricsMode(GMM_RELATIVE);
    pausePanelPlay->setPosition(0.142 * AR, 0.082);
    pausePanelPlay->setDimensions(len1 * AR, len2);
    pausePanelForward->setMetricsMode(GMM_RELATIVE);
    pausePanelForward->setPosition(0.265 * AR, 0.142);
    pausePanelForward->setDimensions(len2 * AR, len1);
    pausePanelBack->setMetricsMode(GMM_RELATIVE);
    pausePanelBack->setPosition(0.080 * AR, 0.142);
    pausePanelBack->setDimensions(len2 * AR, len1);
    pausePanelMenu->setMetricsMode(GMM_RELATIVE);
    pausePanelMenu->setPosition(0.142 * AR, 0.268);
    pausePanelMenu->setDimensions(len1 * AR, len2);
    pauseNavigationBackground->setMaterialName("General/PauseNav");
    pausePanelPlay->setMaterialName("General/PauseGrayPlay");
    pausePanelBack->setMaterialName("General/PauseGrayBack");
    pausePanelForward->setMaterialName("General/PauseGrayForward");
    pausePanelMenu->setMaterialName("General/PauseGrayMenu");
    
    float pauseheight = 0.10;
    float pausewidth = pauseheight * AR;
    buttons[BUTTON_PAUSE].setButton("pause", overlays[0], GMM_RELATIVE, Vector2(0.912, 0.818), Vector2(pausewidth, pauseheight), pauseBackground, NULL);
    
    float gheight = 0.35;
    float gwidth = gheight * AR;
    float gx = 0.50 - gwidth / 2;
    float gy = 0.50 - gheight / 2 + goOffset;
    buttons[BUTTON_GO].setButton("go", overlays[1], GMM_RELATIVE, Vector2(gx, gy), Vector2(gwidth, gheight), goBackground, NULL);
    
    sliderRangeBackground->setPosition(0.20, 0.35 + goOffset);
    goBaseBackground->setMetricsMode(GMM_RELATIVE);
    goBaseBackground->setPosition(0.325, 0.675 + goOffset);
    goBaseBackground->setDimensions(0.35, 0.35);
    
    float qheight = 0.085;
    float qwidth = qheight * AR;
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
    
    // padding for the button
    float bufferZapperX = 0.04;
    float bufferZapperY = 0.04;
    buttons[BUTTON_LEFTZAPPER].setButton("leftzap", overlays[0], GMM_RELATIVE, Vector2(leftZapperX - bufferZapperX / 2, leftZapperY - bufferZapperY / 2), Vector2(dimZapperX + bufferZapperX, dimZapperY + bufferZapperY), leftZapperButtonBackground, NULL);
    buttons[BUTTON_RIGHTZAPPER].setButton("rightzap", overlays[0], GMM_RELATIVE, Vector2(rightZapperX - bufferZapperX / 2, rightZapperY - bufferZapperY / 2), Vector2(dimZapperX + bufferZapperX, dimZapperY + bufferZapperY), rightZapperButtonBackground, NULL);
    
    pauseBackground->setMaterialName("General/PauseButton");
    pauseBaseBackground->setMaterialName("General/PauseBase");
    goBaseBackground->setMaterialName("General/PauseBase");
    
    nbackDisplayBackground->setMetricsMode(GMM_RELATIVE);
    nbackDisplayBackground->setPosition(0.910, 0.205);
    nbackDisplayBackground->setDimensions(0.075, 0.08);
    //nbackDisplayBackground->setMaterialName("General/GUIToggleButton");
    
    nbackDisplayLabel->setMetricsMode(GMM_RELATIVE);
    nbackDisplayLabel->setAlignment(TextAreaOverlayElement::Center);
    nbackDisplayLabel->setPosition(0.04, 0.0225);
    nbackDisplayLabel->setCharHeight(0.03 * FONT_SZ_MULT);
    nbackDisplayLabel->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    nbackDisplayLabel->setFontName("MainSmall");
    nbackDisplayLabel->setCaption(Util::toStringInt(tunnel->getHighestCriteria()));
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

void HudStage::setSpeedDialState(bool active)
{
    sliderRangeBackground->setMaterialName("General/SpeedSliderRangeHorizontal");
    if (active)
        sliderBallBackground->setMaterialName("General/SpeedSliderBall");
    else
        sliderBallBackground->setMaterialName("General/SpeedSliderBallOff");
}

void HudStage::setPauseNavSettings(bool resumeAvail, bool forwardAvail, bool menuAvail, bool backAvail)
{
    // Note: These panels contain grayed versions of the buttons. Therefore, showing them means it's inactive.
    if (resumeAvail) pausePanelPlay->hide();
    else pausePanelPlay->show();
    if (forwardAvail) pausePanelForward->hide();
    else pausePanelForward->show();
    if (menuAvail) pausePanelMenu->hide();
    else pausePanelMenu->show();
    if (backAvail) pausePanelBack->hide();
    else pausePanelBack->show();
}

// Sets the collection bar positions and images.
// If instant parameter is true, the positions will be set instantly,
// otherwise it will animate towards their designated location at a speed.
void HudStage::setCollectionBar(bool instant, float elapsed)
{
    int numSatisfied = tunnel->getNumSatisfiedCriteria();
    
    // How many has the player collected?
    int starPhase = 0;
    if (numSatisfied < 4)
        starPhase = 0;
    else if (numSatisfied < 8)
        starPhase = 1;
    else if (numSatisfied < 12)
        starPhase = 2;
    else
        starPhase = 3;
    
    // Designate positions for each collection item
    float x = 0.295;
    float y = 0.035;
    float wpadding = 0.0075;
    float width = 0.0425;
    float height = 0.075;
    // When we win or lose, almost all of them are collapsed,
    // so we want to re-adjust so it looks evenly split.
    if ((tunnel->isDone() && tunnel->getMode() != STAGE_MODE_RECESS) ||
        starPhase == 3)
    {
        x = 0.3125;
        wpadding = 0.0250;
        starPhase = 3;
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
        
        // Check to see if we should print our next tablet to the right
        //
        // is this tablet in the set that should be expanded?
        //if (i / 4 == starPhase ||
        //    // Is the next tablet in a new set and is it not the trophy tablet?
        //    ((i + 1) % 4 == 0 && ((i + 1) != 12 || starPhase >= 3)))
        if (i / 4 == starPhase || (i + 1) % 4 == 0)
            x += 2 * wpadding + width;
        
        // Assign an image based on what type of collection and whether it is collected
        if (i < criterias.size())
        {
            std::string scoreName = "General/GUICollection";
            
            // For just a single n-back and task
            switch (i / 4)
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
            {
                if (i == 12)
                    scoreName = "General/GUICollectionTrophyFull";
                else
                    scoreName += "Filled";
            }
            else
            {
                if (i == 12)
                    scoreName = "General/GUICollectionTrophyEmpty";
                else
                    scoreName += "Blank";
            }
            collectionBar[i]->setMaterialName(scoreName);
        }
        else
        {
            collectionBar[i]->setMaterialName("General/GUICollectionGreyed");
        }
    }
}

void HudStage::setFuelBar(float elapsed)
{
    float totalWidth = 0.180;
    float percentFuel = tunnel->getFuelTimer() / globals.fuelMax;
    HudFuelBar->setDimensions(percentFuel * totalWidth, 0.056);
    
    /*
     // Animates fuel growth and shrinking to simulate fuel filling the container
     if (tunnel->getFuelTimer() > prevFuelTimer)
        fuelBarAnimationTimer = 1.0f;
     prevFuelTimer = tunnel->getFuelTimer();
     
     if (fuelBarAnimationTimer > 0.0f)
        fuelBarAnimationTimer -= 4 * elapsed;
     else
        fuelBarAnimationTimer = 0.0f;
     
     // Deals with fuel split into multiple containers depleting from the furthest to the right first
    float sz = 0.005 * Ogre::Math::Sin(Ogre::Math::PI * fuelBarAnimationTimer);
    float totalWidth = 0.1725;
    float numBars = globals.startingHP;
    float totalPadding = totalWidth * 0.1;
    float fbPadding = (totalPadding) / (numBars - 1);
    float fbSize = (totalWidth - totalPadding) / numBars;
    
    float percentFuel = tunnel->getFuelTimer() / globals.fuelMax;
    float fbFuel = 1.0 / numBars;
    for (int i = 0; i < HudFuelBars.size(); ++i)
    {
        HudFuelContainers[i]->setMetricsMode(GMM_RELATIVE);
        HudFuelContainers[i]->setPosition(0.735 + (fbSize + fbPadding) * i - fbPadding, 0.025 - fbPadding);
        HudFuelContainers[i]->setDimensions(fbSize + 2 * fbPadding, 0.045 + 2 * fbPadding);
        HudFuelContainers[i]->setMaterialName("General/GUIFuelContainer");
        
        HudFuelBars[i]->setMetricsMode(GMM_RELATIVE);
        HudFuelBars[i]->setPosition(0.735 + (fbSize + fbPadding) * i - sz / 2, 0.025 - sz / 2);
        if (i < player->getHP())
        {
            percentFuel -= fbFuel;
            if (percentFuel >= 0.0)
                HudFuelBars[i]->setDimensions(fbSize + sz, 0.045 + sz);
            else
                HudFuelBars[i]->setDimensions(fbSize * ((fbFuel + percentFuel) / fbFuel) + sz, 0.045 + sz);
            HudFuelBars[i]->setMaterialName("General/GUIFuelBar");
        }
        else
        {
            percentFuel -= fbFuel;
            HudFuelBars[i]->setDimensions(fbSize + sz, 0.045 + sz);
            HudFuelBars[i]->setMaterialName("General/GUIBrokenBar");
        }
    }
    //HudFuelBar->setDimensions(percentFuel * totalWidth, 0.045);
     */
}
