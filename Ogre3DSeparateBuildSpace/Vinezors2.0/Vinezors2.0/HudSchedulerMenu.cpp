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
    clearSelection();
    float averageMemoryScore = (player->scheduler->nBackLevelA + player->scheduler->nBackLevelB + player->scheduler->nBackLevelC + player->scheduler->nBackLevelD) / 4;
    float gameScore = player->scheduler->scoreCurr;
    if (player->levelRequest)
        setSelection();
    schedulerMenuAverageMemoryText->setCaption(Util::toStringFloat(averageMemoryScore));
    schedulerMenuScoreCurrText->setCaption(Util::toStringInt(gameScore));
}

std::string HudSchedulerMenu::processButtons(Vector2 target)
{
    std::string ret = Hud::queryButtons(target);
    if (ret == "selection0")
    {
        setSelectToIcon(levelOverlayPanels[0].entireBackground, 1);
    }
    else if (ret == "selection1")
    {
        setSelectToIcon(levelOverlayPanels[1].entireBackground, 1);
    }
    else if (ret == "selection2")
    {
        setSelectToIcon(levelOverlayPanels[2].entireBackground, 1);
    }
    else if (ret == "history0")
    {
        setSelectToIcon(historyOverlayPanels[0].entireBackground, 0);
    }
    else if (ret == "history1")
    {
        setSelectToIcon(historyOverlayPanels[1].entireBackground, 0);
    }
    else if (ret == "history2")
    {
        setSelectToIcon(historyOverlayPanels[2].entireBackground, 0);
    }
    else if (ret == "history3")
    {
        setSelectToIcon(historyOverlayPanels[3].entireBackground, 0);
    }
    else if (ret == "history4")
    {
        setSelectToIcon(historyOverlayPanels[4].entireBackground, 0);
    }
    return ret;
}

void HudSchedulerMenu::alloc()
{
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS + NUM_SELECTIONS + SCHEDULE_LEN);
    
    
    schedulerMenuEntireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuEntireBackground"));
    
    schedulerMenuScoreCurrBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuScoreCurrBackground"));
    schedulerMenuScoreCurrText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "SchedulerMenuScoreCurrText"));
    
    schedulerMenuAverageMemoryBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuAverageMemoryBackground"));
    schedulerMenuAverageMemoryText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "SchedulerMenuAverageMemoryText"));
    
    levelOverlayPanels = std::vector<LevelOverlayElement>(NUM_SELECTIONS);
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        levelOverlayPanels[i].entireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ScheduleMenuLevelEntireBackground" + Util::toStringInt(i)));
        levelOverlayPanels[i].title = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelTitle" + Util::toStringInt(i)));
        levelOverlayPanels[i].value = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelValue" + Util::toStringInt(i)));
    }
    historyOverlayPanels = std::vector<LevelOverlayElement>(SCHEDULE_LEN);
    for (int i = 0; i < historyOverlayPanels.size(); ++i)
    {
        historyOverlayPanels[i].entireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ScheduleMenuHistoryEntireBackground" + Util::toStringInt(i)));
        historyOverlayPanels[i].title = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuHistoryTitle" + Util::toStringInt(i)));
        historyOverlayPanels[i].value = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuHistoryValue" + Util::toStringInt(i)));
    }
    
    levelDetails.entireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ScheduleMenuLevelDetailsEntireBackground"));
    levelDetails.title = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelDetailsTitle"));
    levelDetails.names = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelDetailsNames"));
    levelDetails.values = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelDetailsValues"));
    levelDetails.meritIcon = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ScheduleMenuLevelDetailsMeritIcon"));
    
    selectIconHistory = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuSelectIconHistory"));
    selectIconHistory->setPosition(0, 0);
    selectIconHistory->setDimensions(0, 0);
    selectIconChoice = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuSelectIconChoice"));
    selectIconChoice->setPosition(0, 0);
    selectIconChoice->setDimensions(0, 0);
    
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuBackButtonBackground"));
    playButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuPlayButtonBackground"));
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("SchedulerMenuOverlay");
    Overlay* overlay2 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("SchedulerMenuSelector");
    overlays.push_back(overlay1);
    overlays.push_back(overlay2);
    
    overlay1->add2D(schedulerMenuEntireBackground);
    
    
    overlay1->add2D(schedulerMenuScoreCurrBackground);
    schedulerMenuScoreCurrBackground->addChild(schedulerMenuScoreCurrText);
    
    overlay1->add2D(schedulerMenuAverageMemoryBackground);
    schedulerMenuAverageMemoryBackground->addChild(schedulerMenuAverageMemoryText);
    
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        schedulerMenuEntireBackground->addChild(levelOverlayPanels[i].entireBackground);
        levelOverlayPanels[i].entireBackground->addChild(levelOverlayPanels[i].title);
        levelOverlayPanels[i].entireBackground->addChild(levelOverlayPanels[i].value);
    }
    for (int i = 0; i < historyOverlayPanels.size(); ++i)
    {
        schedulerMenuEntireBackground->addChild(historyOverlayPanels[i].entireBackground);
        historyOverlayPanels[i].entireBackground->addChild(historyOverlayPanels[i].title);
        historyOverlayPanels[i].entireBackground->addChild(historyOverlayPanels[i].value);
    }
    overlay1->add2D(levelDetails.entireBackground);
    levelDetails.entireBackground->addChild(levelDetails.title);
    levelDetails.entireBackground->addChild(levelDetails.names);
    levelDetails.entireBackground->addChild(levelDetails.values);
    levelDetails.entireBackground->addChild(levelDetails.meritIcon);
    
    overlay1->add2D(backButtonBackground);
    overlay1->add2D(playButtonBackground);
    
    overlay2->add2D(selectIconHistory);
    overlay2->add2D(selectIconChoice);
}

void HudSchedulerMenu::dealloc()
{
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(schedulerMenuEntireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(schedulerMenuScoreCurrBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(schedulerMenuScoreCurrText);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(schedulerMenuAverageMemoryBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(schedulerMenuAverageMemoryText);
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelOverlayPanels[i].entireBackground);
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelOverlayPanels[i].title);
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelOverlayPanels[i].value);
    }
    for (int i = 0; i < historyOverlayPanels.size(); ++i)
    {
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(historyOverlayPanels[i].entireBackground);
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(historyOverlayPanels[i].title);
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(historyOverlayPanels[i].value);
    }
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.entireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.title);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.names);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.values);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.meritIcon);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(playButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(selectIconHistory);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(selectIconChoice);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[1]);
}

void HudSchedulerMenu::initOverlay()
{
    Ogre::ColourValue fontColor = Ogre::ColourValue(0.62f, 0.85f, 0.85f);
    
    schedulerMenuEntireBackground->setMetricsMode(GMM_RELATIVE);
    schedulerMenuEntireBackground->setPosition(0.00, 0.00);
    schedulerMenuEntireBackground->setDimensions(1.00, 1.00);
    schedulerMenuEntireBackground->setMaterialName("General/ScreenBackgroundSchedulerMenu");
    
    schedulerMenuScoreCurrBackground->setMetricsMode(GMM_RELATIVE);
    schedulerMenuScoreCurrBackground->setPosition(0.00, 0.00);
    schedulerMenuScoreCurrBackground->setDimensions(0.00, 0.00);
    schedulerMenuScoreCurrText->setMetricsMode(GMM_RELATIVE);
    schedulerMenuScoreCurrText->setAlignment(TextAreaOverlayElement::Center);
    schedulerMenuScoreCurrText->setPosition(0.885, 0.134);
    schedulerMenuScoreCurrText->setCharHeight(0.018 * FONT_SZ_MULT);
    schedulerMenuScoreCurrText->setFontName("MainSmall");
    schedulerMenuScoreCurrText->setColour(fontColor);
    
    schedulerMenuAverageMemoryBackground->setMetricsMode(GMM_RELATIVE);
    schedulerMenuAverageMemoryBackground->setPosition(0.00, 0.00);
    schedulerMenuAverageMemoryBackground->setDimensions(0.00, 0.00);
    schedulerMenuAverageMemoryText->setMetricsMode(GMM_RELATIVE);
    schedulerMenuAverageMemoryText->setAlignment(TextAreaOverlayElement::Center);
    schedulerMenuAverageMemoryText->setPosition(0.885, 0.063);
    schedulerMenuAverageMemoryText->setCharHeight(0.018 * FONT_SZ_MULT);
    schedulerMenuAverageMemoryText->setFontName("MainSmall");
    schedulerMenuAverageMemoryText->setColour(fontColor);
    
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        std::string buttonName = "selection";
        buttonName += Util::toStringInt(i);
        
        float iconHeight = 0.224;
        float iconWidth = 0.292 * ((float)globals.screenHeight / globals.screenWidth);
        Vector2 buttonPos = Vector2(0.078, 0.278 + (iconHeight + 0.015) * i);
        buttons[NUM_UNIQUE_BUTTONS + i].setButton(buttonName, overlays[0], GMM_RELATIVE, buttonPos, Vector2(iconWidth, iconHeight), levelOverlayPanels[i].entireBackground, NULL);
        
        levelOverlayPanels[i].title->setMetricsMode(GMM_RELATIVE);
        levelOverlayPanels[i].title->setAlignment(TextAreaOverlayElement::Center);
        levelOverlayPanels[i].title->setPosition(-0.025, iconHeight / 2.0f - 0.0125);
        levelOverlayPanels[i].title->setCharHeight(0.020 * FONT_SZ_MULT);
        levelOverlayPanels[i].title->setFontName("MainSmall");
        levelOverlayPanels[i].title->setCaption(Util::toStringInt(i + 1));
        levelOverlayPanels[i].title->setColour(fontColor);
        
        levelOverlayPanels[i].value->setMetricsMode(GMM_RELATIVE);
        levelOverlayPanels[i].value->setAlignment(TextAreaOverlayElement::Center);
        levelOverlayPanels[i].value->setPosition(0.0f, 0.0f);
        levelOverlayPanels[i].value->setCharHeight(0.020 * FONT_SZ_MULT);
        levelOverlayPanels[i].value->setFontName("MainSmall");
    }
    for (int i = 0; i < historyOverlayPanels.size(); ++i)
    {
        std::string buttonName = "history";
        buttonName += Util::toStringInt(i);
        
        float iconSize = 0.134;
        float iconHeight = iconSize;
        float iconWidth = iconSize * ((float)globals.screenHeight / globals.screenWidth);
        
        buttons[NUM_UNIQUE_BUTTONS + NUM_SELECTIONS + i].setButton(buttonName, overlays[0], GMM_RELATIVE, Vector2(0.041 + 0.130 * i, 0.042), Vector2(iconWidth, iconHeight), historyOverlayPanels[i].entireBackground, NULL);
        
        historyOverlayPanels[i].title->setMetricsMode(GMM_RELATIVE);
        historyOverlayPanels[i].title->setAlignment(TextAreaOverlayElement::Center);
        historyOverlayPanels[i].title->setPosition(0, 0);
        historyOverlayPanels[i].title->setCharHeight(0.020 * FONT_SZ_MULT);
        historyOverlayPanels[i].title->setFontName("MainSmall");
        
        historyOverlayPanels[i].value->setMetricsMode(GMM_RELATIVE);
        historyOverlayPanels[i].value->setAlignment(TextAreaOverlayElement::Center);
        historyOverlayPanels[i].value->setPosition(iconWidth / 2.0f, iconHeight + 0.010);
        historyOverlayPanels[i].value->setCharHeight(0.018 * FONT_SZ_MULT);
        historyOverlayPanels[i].value->setFontName("MainSmall");
        historyOverlayPanels[i].value->setColour(fontColor);
    }
    
    levelDetails.entireBackground->setMetricsMode(GMM_RELATIVE);
    levelDetails.entireBackground->setPosition(0.300, 0.300);
    levelDetails.entireBackground->setDimensions(0.375, 0.575);
    
    levelDetails.title->setMetricsMode(GMM_RELATIVE);
    levelDetails.title->setAlignment(TextAreaOverlayElement::Center);
    levelDetails.title->setPosition(0.10, -0.025);
    levelDetails.title->setCharHeight(0.032 * FONT_SZ_MULT);
    levelDetails.title->setFontName("MainSmall");
    
    levelDetails.names->setMetricsMode(GMM_RELATIVE);
    levelDetails.names->setAlignment(TextAreaOverlayElement::Left);
    levelDetails.names->setPosition(0.025, 0.025);
    levelDetails.names->setCharHeight(0.02 * FONT_SZ_MULT);
    levelDetails.names->setFontName("MainSmall");
    levelDetails.names->setCaption("Type:\nN-Back:\nLength:\nNavigation:\nHoldout:\nPotential:\n\nCompleted:\nMistakes:\nMissed:\nPickups:\nZapped:\nAccuracy:\nEarned:\nMastery:");
    levelDetails.names->setColour(fontColor);
    
    levelDetails.values->setMetricsMode(GMM_RELATIVE);
    levelDetails.values->setAlignment(TextAreaOverlayElement::Right);
    levelDetails.values->setPosition(levelDetails.entireBackground->getWidth() - 0.025, 0.025);
    levelDetails.values->setCharHeight(0.02 * FONT_SZ_MULT);
    levelDetails.values->setFontName("MainSmall");
    levelDetails.values->setColour(fontColor);
    
    float meritSize = 0.10f;
    float meritHeight = meritSize;
    float meritWidth = meritSize * ((float)globals.screenHeight / globals.screenWidth);
    levelDetails.meritIcon->setMetricsMode(GMM_RELATIVE);
    levelDetails.meritIcon->setPosition(0.155, -0.075);
    levelDetails.meritIcon->setDimensions(meritWidth, meritHeight);
    //levelDetails.meritIcon->setMaterialName("General/LevelBarUnavailable");
    
    selectIconHistory->setMetricsMode(GMM_RELATIVE);
    selectIconHistory->setMaterialName("General/IconSelection");
    selectIconChoice->setMetricsMode(GMM_RELATIVE);
    selectIconChoice->setMaterialName("General/BigIconSelection");
    
    // Set up buttons
    backButtonBackground->setMaterialName("General/BackButton2");
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.700, 0.875), Vector2(0.263, 0.100), backButtonBackground, NULL);
    buttons[BUTTON_PLAY].setButton("play", overlays[0], GMM_RELATIVE, Vector2(0.700, 0.281), Vector2(0.263, 0.215), playButtonBackground, NULL);
    
    // Needed for some reason to allow materials to be set in update
    clearSelection();
}

void HudSchedulerMenu::link(Player* player)
{
    this->player = player;
}

void HudSchedulerMenu::unlink()
{
    this->player = NULL;
}

void setBigIconBasedOnLevel(const std::pair<StageRequest, PlayerProgress> & levelRequest, PanelOverlayElement* background)
{
    // Obtain level progress
    StageRequest level = levelRequest.first;
    PlayerProgress progress = levelRequest.second;
    
    switch (level.phaseX)
    {
        case PHASE_COLOR_SOUND:
            background->setMaterialName("General/BigIconColorSound");
            break;
        case PHASE_SHAPE_SOUND:
            background->setMaterialName("General/BigIconShapeSound");
            break;
        case PHASE_SOUND_ONLY:
            background->setMaterialName("General/BigIconSoundOnly");
            break;
        case PHASE_ALL_SIGNAL:
            background->setMaterialName("General/BigIconAllSignal");
            break;
        case PHASE_COLLECT:
            background->setMaterialName("General/BigIconRecess");
            break;
        default:
            background->setMaterialName("General/IconNone");
            break;
    }
}

void setIconBasedOnLevel(const std::pair<StageRequest, PlayerProgress> & levelRequest, PanelOverlayElement* background)
{
    // Obtain level progress
    StageRequest level = levelRequest.first;
    PlayerProgress progress = levelRequest.second;
    
    switch (level.phaseX)
    {
        case PHASE_COLOR_SOUND:
            background->setMaterialName("General/IconColorSound");
            break;
        case PHASE_SHAPE_SOUND:
            background->setMaterialName("General/IconShapeSound");
            break;
        case PHASE_SOUND_ONLY:
            background->setMaterialName("General/IconSoundOnly");
            break;
        case PHASE_ALL_SIGNAL:
            background->setMaterialName("General/IconAllSignal");
            break;
        case PHASE_COLLECT:
            background->setMaterialName("General/IconRecess");
            break;
        default:
            background->setMaterialName("General/IconNone");
            break;
    }
}

// Functions to reset the selected displayed in the hud
void HudSchedulerMenu::clearSelection()
{
    // Reset all icons (so that we reset the selected item displayed)
    setBigIconBasedOnLevel(player->scheduleChoice1, levelOverlayPanels[0].entireBackground);
    setBigIconBasedOnLevel(player->scheduleChoice2, levelOverlayPanels[1].entireBackground);
    setBigIconBasedOnLevel(player->scheduleChoice3, levelOverlayPanels[2].entireBackground);
    setScheduleHistory();
    
    // Clear the level details display since we're resetting the level selected
    std::string nothing;
    levelDetails.values->setCaption(nothing +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n" +
                                    "-" + "\n");
    
    // Gray out the button since we have no level selected
    playButtonBackground->setMaterialName("General/PlayButtonGray");
}

// Set the hud to reflect the current item selected by the player
void HudSchedulerMenu::setSelection()
{
    // Obtain level progress
    StageRequest level = player->levelRequest->first;
    PlayerProgress progress = player->levelRequest->second;
    
    // Grab relevant information about the selected level
    std::string type;           // Type of stage (Color/Sound, Recess, Sound Only, ect.)
    std::string nback;          // N-Back of level (Recess has 0-back)
    std::string navigation;     // Navigation difficulty of level
    std::string length;         // Length of level
    std::string holdout;        // Holdout description
    std::string potential;      // Amount of experience person can earn (without perfect bonus)
    
    std::string completed;
    std::string mistakes;       // Non-targets player collected
    std::string missed;         // Targets player avoided
    std::string pickups;        // Non-targets player picked up
    std::string collected;      // Targets player collected
    std::string accuracy;
    std::string delta;
    std::string mastery;
    
    // Assign description value to each variable
    switch (level.phaseX)
    {
        case PHASE_COLOR_SOUND:
            type = "Color&Sound";
            break;
        case PHASE_SHAPE_SOUND:
            type = "Shape&Sound";
            break;
        case PHASE_SOUND_ONLY:
            type = "Sound Only";
            break;
        case PHASE_ALL_SIGNAL:
            type = "All Features";
            break;
        case PHASE_COLLECT:
            type = "Recess";
            break;
        default:
            break;
    }
    nback = Util::toStringInt(level.nback);
    
    switch (level.difficultyX)
    {
        case DIFFICULTY_EASY:
            navigation = "easier";
            break;
        case DIFFICULTY_NORMAL:
            navigation = "normal";
            break;
        case DIFFICULTY_HARD:
            navigation = "harder";
            break;
    }
    holdout = level.hasHoldout() ? "yes" : "no";
    potential = Util::toStringFloat(player->modifyNBackDelta(level, progress, 1.0, true));
    bool tooEasy = player->getMemoryChallenge(level, progress) < -0.5;
    if (tooEasy || level.phaseX == PHASE_COLLECT)
        potential += "R";
    else 
        potential += "M";
    
    if (level.phaseX == PHASE_COLLECT)
        length = "medium";
    else if (level.collectionCriteria.size() <= 4)
        length = "short";
    else if (level.collectionCriteria.size() <= 8)
        length = "medium";
    else //if (level.collectionCriteria.size() <= 13)
        length = "long";
    
    // Check if player ever played the level
    if (progress.rating < 0)
    {
        completed = "-";
        accuracy = "-";
        collected = "-";
        mistakes = "-";
        pickups = "-";
        missed = "-";
        delta ="-";
        mastery = Util::toStringFloat(progress.nBackSkill);
        if (level.hasHoldout() && progress.nBackOffset < 0.0)
            mastery = "(" + Util::toStringFloat(progress.nBackOffset) + ") " + mastery;
    }
    else
    {
        completed = progress.rating >= 5 ? "yes" : "no";
        accuracy = Util::toStringInt(progress.accuracy * 100.0);
        if (level.phaseX != PHASE_COLLECT)
        {
            int criteria = 0;
            switch (level.durationX)
            {
                case DURATION_SHORT:
                    criteria = 4;
                    break;
                case DURATION_NORMAL:
                    criteria = 8;
                    break;
                case DURATION_LONG:
                    criteria = 13;
                    break;
                default:
                    criteria = 0;
                    break;
            }
            mistakes = Util::toStringInt(progress.numWrong);
            missed = Util::toStringInt(progress.numMissed);
            pickups = Util::toStringInt(progress.numPickups) + " / " + Util::toStringInt(progress.numSafe + progress.numWrong);
            collected = Util::toStringInt(progress.numCorrect) + " / " + Util::toStringInt(criteria);
        }
        else
        {
            mistakes = "-";
            missed = "-";
            pickups = Util::toStringInt(progress.numCorrect) + " / " + Util::toStringInt(progress.numCorrect + progress.numWrong + progress.numMissed);
            collected = "-";
        }
        if (progress.nBackDelta >= 0.0f)
            delta = "+";
        
        delta += Util::toStringFloat(progress.nBackDelta);
        if (level.hasHoldout() && progress.nBackReturn != 0.0)
        {
            if (progress.nBackReturn > 0.0)
                delta = "(+" + Util::toStringFloat(progress.nBackReturn) + ") " + delta;
            else
                delta = "(" + Util::toStringFloat(progress.nBackReturn) + ") " + delta;
        }
        if (tooEasy && level.phaseX != PHASE_COLLECT)
            delta += "R";
        if (!tooEasy || progress.nBackDelta < 0.0f || progress.nBackReturn < 0.0f)
            delta += "M";
        
        //if (progress.accuracy >= 1.0 - Util::EPSILON)
        //    delta = "x1.3 " + delta;
        mastery = Util::toStringFloat(progress.nBackResult);
        if (level.hasHoldout() && progress.nBackNoffset < 0.0)
            mastery = "(" + Util::toStringFloat(progress.nBackNoffset) + ") " + mastery;
    }
    
    // Set the text displaying the level details
    levelDetails.values->setCaption(type + "\n" +
                                    nback + "\n" +
                                    length + "\n" +
                                    navigation + "\n" +
                                    holdout + "\n" +
                                    potential + "\n" +
                                    "\n" +
                                    completed + "\n" +
                                    mistakes + "\n" +
                                    missed + "\n" +
                                    pickups + "\n" +
                                    collected + "\n" +
                                    accuracy + "\n" +
                                    delta + "\n" +
                                    mastery);
    
    // Turn on the play button since we have a valid level selected
    if (player->levelRequest && player->levelRequest->second.rating < 0)
        playButtonBackground->setMaterialName("General/PlayButton");
}

// Assigns a transparent yellow glow to the icon we are selecting (the parameter)
// Since all overlay elements are by relative width and height, we can set the size
// of the glowy circle to the size of the icon element
void HudSchedulerMenu::setSelectToIcon(PanelOverlayElement* icon, int mode)
{
    if (mode == 0)
    {
        float selectIconWidth = 0.1225;
        float selectIconHeight = 0.1424;
        float mainIconWidth = icon->getWidth();
        float mainIconHeight = icon->getHeight();
        selectIconHistory->setPosition(icon->_getDerivedLeft() - (selectIconWidth - mainIconWidth) / 2, icon->_getDerivedTop() - (selectIconHeight - mainIconHeight) / 2);
        selectIconHistory->setWidth(selectIconWidth);
        selectIconHistory->setHeight(selectIconHeight);
        selectIconHistory->show();
        selectIconChoice->hide();
    }
    else
    {
        float selectIconWidth = 0.2617;
        float selectIconHeight = 0.2224;
        float mainIconWidth = icon->getWidth();
        float mainIconHeight = icon->getHeight();
        selectIconChoice->setPosition(icon->_getDerivedLeft() - 0.04f, icon->_getDerivedTop() - (selectIconHeight - mainIconHeight) / 2);
        selectIconChoice->setWidth(selectIconWidth);
        selectIconChoice->setHeight(selectIconHeight);
        selectIconChoice->show();
        selectIconHistory->hide();
    }
}


// Set how the levels in the history panel are displayed without them being selected
void HudSchedulerMenu::setScheduleHistory()
{
    //std::cout << "Schedule History: " << player->scheduler->scheduleHistory.size() << std::endl;
    
    int sizeA = player->scheduler->scheduleHistoryA.size();
    if (sizeA > 0)
    {
        setIconBasedOnLevel(player->scheduler->scheduleHistoryA[sizeA - 1], historyOverlayPanels[0].entireBackground);
        //historyOverlayPanels[0].title->setCaption(Util::toStringInt(player->scheduler->scheduleHistoryA.size()));
        historyOverlayPanels[0].value->setCaption(Util::toStringFloat(player->scheduler->nBackLevelA));
    }
    else
        historyOverlayPanels[0].entireBackground->setMaterialName("General/IconNone");
    int sizeB = player->scheduler->scheduleHistoryB.size();
    if (sizeB > 0)
    {
        setIconBasedOnLevel(player->scheduler->scheduleHistoryB[sizeB - 1], historyOverlayPanels[1].entireBackground);
        //historyOverlayPanels[1].title->setCaption(Util::toStringInt(player->scheduler->scheduleHistoryB.size()));
        historyOverlayPanels[1].value->setCaption(Util::toStringFloat(player->scheduler->nBackLevelB));
    }
    else
        historyOverlayPanels[1].entireBackground->setMaterialName("General/IconNone");
    int sizeC = player->scheduler->scheduleHistoryC.size();
    if (sizeC > 0)
    {
        setIconBasedOnLevel(player->scheduler->scheduleHistoryC[sizeC - 1], historyOverlayPanels[2].entireBackground);
        //historyOverlayPanels[2].title->setCaption(Util::toStringInt(player->scheduler->scheduleHistoryC.size()));
        historyOverlayPanels[2].value->setCaption(Util::toStringFloat(player->scheduler->nBackLevelC));
    }
    else
        historyOverlayPanels[2].entireBackground->setMaterialName("General/IconNone");
    int sizeD = player->scheduler->scheduleHistoryD.size();
    if (sizeD > 0)
    {
        setIconBasedOnLevel(player->scheduler->scheduleHistoryD[sizeD - 1], historyOverlayPanels[3].entireBackground);
        //historyOverlayPanels[3].title->setCaption(Util::toStringInt(player->scheduler->scheduleHistoryD.size()));
        historyOverlayPanels[3].value->setCaption(Util::toStringFloat(player->scheduler->nBackLevelD));
    }
    else
        historyOverlayPanels[3].entireBackground->setMaterialName("General/IconNone");
    int sizeE = player->scheduler->scheduleHistoryE.size();
    if (sizeE > 0)
    {
        setIconBasedOnLevel(player->scheduler->scheduleHistoryE[sizeE - 1], historyOverlayPanels[4].entireBackground);
        //historyOverlayPanels[4].title->setCaption(Util::toStringInt(player->scheduler->scheduleHistoryE.size()));
        historyOverlayPanels[4].value->setCaption(Util::toStringFloat(player->scheduler->nBackLevelE));
    }
    else
        historyOverlayPanels[4].entireBackground->setMaterialName("General/IconNone");
    
}
