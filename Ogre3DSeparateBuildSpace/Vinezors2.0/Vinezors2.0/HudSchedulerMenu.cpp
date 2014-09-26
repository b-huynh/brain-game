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
    clearSelection();
}

void HudSchedulerMenu::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudSchedulerMenu::update(float elapsed)
{
}

std::string HudSchedulerMenu::processButtons(Vector2 target)
{
    std::string ret = Hud::queryButtons(target);
    if (ret == "selection0")
    {
        clearSelection();
        levelOverlayPanels[0].entireBackground->setMaterialName("General/YellowSphereIcon");
    }
    else if (ret == "selection1")
    {
        clearSelection();
        levelOverlayPanels[1].entireBackground->setMaterialName("General/YellowSphereIcon");
    }
    else if (ret == "selection2")
    {
        clearSelection();
        levelOverlayPanels[2].entireBackground->setMaterialName("General/YellowSphereIcon");
    }
    else if (ret == "history0")
    {
        clearSelection();
        historyOverlayPanels[0].entireBackground->setMaterialName("General/YellowSphereIcon");
    }
    else if (ret == "history1")
    {
        clearSelection();
        historyOverlayPanels[1].entireBackground->setMaterialName("General/YellowSphereIcon");
    }
    else if (ret == "history2")
    {
        clearSelection();
        historyOverlayPanels[2].entireBackground->setMaterialName("General/YellowSphereIcon");
    }
    else if (ret == "history3")
    {
        clearSelection();
        historyOverlayPanels[3].entireBackground->setMaterialName("General/YellowSphereIcon");
    }
    else if (ret == "history4")
    {
        clearSelection();
        historyOverlayPanels[4].entireBackground->setMaterialName("General/YellowSphereIcon");
    }
    return ret;
}

void HudSchedulerMenu::alloc()
{
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS + NUM_SELECTIONS + SCHEDULE_LEN);
    
    schedulerMenuBackdrop = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuBackdrop"));
    schedulerMenuTitleBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuTitleBackground"));
    schedulerMenuTitleText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "SchedulerMenuTitleText"));
    
    levelOverlayPanels = std::vector<LevelOverlayElement>(NUM_SELECTIONS);
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        levelOverlayPanels[i].entireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ScheduleMenuLevelEntireBackground" + Util::toStringInt(i)));        levelOverlayPanels[i].title = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelTitle" + Util::toStringInt(i)));
    }
    historyOverlayPanels = std::vector<LevelOverlayElement>(SCHEDULE_LEN);
    for (int i = 0; i < historyOverlayPanels.size(); ++i)
    {
        historyOverlayPanels[i].entireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ScheduleMenuHistoryEntireBackground" + Util::toStringInt(i)));
        historyOverlayPanels[i].title = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuHistoryTitle" + Util::toStringInt(i)));
    }
    
    levelDetails.entireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ScheduleMenuLevelDetailsEntireBackground"));
    levelDetails.title = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelDetailsTitle"));
    levelDetails.names = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelDetailsNames"));
    levelDetails.values = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "ScheduleMenuLevelDetailsValues"));
    levelDetails.meritIcon = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "ScheduleMenuLevelDetailsMeritIcon"));
    
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuBackButtonBackground"));
    playButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "SchedulerMenuPlayButtonBackground"));
    
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
    }
    for (int i = 0; i < historyOverlayPanels.size(); ++i)
    {
        schedulerMenuBackdrop->addChild(historyOverlayPanels[i].entireBackground);
        historyOverlayPanels[i].entireBackground->addChild(historyOverlayPanels[i].title);
    }
    overlay1->add2D(levelDetails.entireBackground);
    levelDetails.entireBackground->addChild(levelDetails.title);
    levelDetails.entireBackground->addChild(levelDetails.names);
    levelDetails.entireBackground->addChild(levelDetails.values);
    levelDetails.entireBackground->addChild(levelDetails.meritIcon);
    
    overlay1->add2D(backButtonBackground);
    overlay1->add2D(playButtonBackground);
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
    }
    for (int i = 0; i < historyOverlayPanels.size(); ++i)
    {
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(historyOverlayPanels[i].entireBackground);
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(historyOverlayPanels[i].title);
    }
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.entireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.title);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.names);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.values);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelDetails.meritIcon);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(playButtonBackground);
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
        
        float iconSize = 0.20f;
        float iconHeight = iconSize;
        float iconWidth = iconSize * ((float)globals.screenHeight / globals.screenWidth);
        Vector2 buttonPos;
        if (i == 0)
            buttonPos = Vector2(0.125, 0.40);
        else if (i == 1)
            buttonPos = Vector2(0.375, 0.40);
        else //if (i == 2)
            buttonPos = Vector2(0.25, 0.65);
        buttons[NUM_UNIQUE_BUTTONS + i].setButton(buttonName, overlays[0], GMM_RELATIVE, buttonPos, Vector2(iconWidth, iconHeight), levelOverlayPanels[i].entireBackground, NULL);
        
        levelOverlayPanels[i].title->setMetricsMode(GMM_RELATIVE);
        levelOverlayPanels[i].title->setAlignment(TextAreaOverlayElement::Center);
        levelOverlayPanels[i].title->setPosition(iconWidth / 2.0f, -0.025f);
        levelOverlayPanels[i].title->setCharHeight(0.025 * FONT_SZ_MULT);
        levelOverlayPanels[i].title->setFontName("MainSmall");
        levelOverlayPanels[i].title->setCaption("Choice " + Util::toStringInt(i + 1));
    }
    for (int i = 0; i < historyOverlayPanels.size(); ++i)
    {
        std::string buttonName = "history";
        buttonName += Util::toStringInt(i);
        
        float iconSize = 0.125f;
        float iconHeight = iconSize;
        float iconWidth = iconSize * ((float)globals.screenHeight / globals.screenWidth);
        
        buttons[NUM_UNIQUE_BUTTONS + NUM_SELECTIONS + i].setButton(buttonName, overlays[0], GMM_RELATIVE, Vector2(0.10 + 0.10 * i, 0.15), Vector2(iconWidth, iconHeight), historyOverlayPanels[i].entireBackground, NULL);
        
        historyOverlayPanels[i].title->setMetricsMode(GMM_RELATIVE);
        historyOverlayPanels[i].title->setAlignment(TextAreaOverlayElement::Center);
        historyOverlayPanels[i].title->setPosition(iconWidth / 2.0f, -0.02f);
        historyOverlayPanels[i].title->setCharHeight(0.020 * FONT_SZ_MULT);
        historyOverlayPanels[i].title->setFontName("MainSmall");
    }
    
    levelDetails.entireBackground->setMetricsMode(GMM_RELATIVE);
    levelDetails.entireBackground->setPosition(0.575, 0.375);
    levelDetails.entireBackground->setDimensions(0.35, 0.50);
    levelDetails.entireBackground->setMaterialName("General/ScreenBackground2");
    
    levelDetails.title->setMetricsMode(GMM_RELATIVE);
    levelDetails.title->setAlignment(TextAreaOverlayElement::Center);
    levelDetails.title->setPosition(0.10, -0.025);
    levelDetails.title->setCharHeight(0.032 * FONT_SZ_MULT);
    levelDetails.title->setFontName("MainSmall");
    levelDetails.title->setCaption("Details");
    
    levelDetails.names->setMetricsMode(GMM_RELATIVE);
    levelDetails.names->setAlignment(TextAreaOverlayElement::Left);
    levelDetails.names->setPosition(0.025, 0.025);
    levelDetails.names->setCharHeight(0.02 * FONT_SZ_MULT);
    levelDetails.names->setFontName("MainSmall");
    levelDetails.names->setCaption("Type:\nN-Back:\nCollection:\nNavigation:\nHoldout:\nLength:\n\nAccuracy:\nCollected:\nMistakes:\nAvoided:\nMissed:");
    
    levelDetails.values->setMetricsMode(GMM_RELATIVE);
    levelDetails.values->setAlignment(TextAreaOverlayElement::Right);
    levelDetails.values->setPosition(levelDetails.entireBackground->getWidth() - 0.025, 0.025);
    levelDetails.values->setCharHeight(0.02 * FONT_SZ_MULT);
    levelDetails.values->setFontName("MainSmall");
    
    float meritSize = 0.10f;
    float meritHeight = meritSize;
    float meritWidth = meritSize * ((float)globals.screenHeight / globals.screenWidth);
    levelDetails.meritIcon->setMetricsMode(GMM_RELATIVE);
    levelDetails.meritIcon->setPosition(0.155, -0.075);
    levelDetails.meritIcon->setDimensions(meritWidth, meritHeight);
    //levelDetails.meritIcon->setMaterialName("General/LevelBarUnavailable");
    
    // Set up buttons
    backButtonBackground->setMaterialName("General/BackButton");
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.15, 0.90), Vector2(0.30, 0.08), backButtonBackground, NULL);
    buttons[BUTTON_PLAY].setButton("play", overlays[0], GMM_RELATIVE, Vector2(0.55, 0.90), Vector2(0.30, 0.08), playButtonBackground, NULL);
    
}

void HudSchedulerMenu::link(Player* player)
{
    this->player = player;
}

void HudSchedulerMenu::unlink()
{
    this->player = NULL;
}

// Since our scheduler menu can only show so many (currently x=5 previous levels),
// we have a function that will get the starting index that represents the last x
// levels in the our current history.
int HudSchedulerMenu::getStartingSchedulerHistoryIndex() const
{
    int startingScheduleIndex = player->scheduler->scheduleHistory.size() - SCHEDULE_LEN;
    if (startingScheduleIndex < 0) startingScheduleIndex = 0;
    return startingScheduleIndex;
}

// Functions to reset the selected displayed in the hud
void HudSchedulerMenu::clearSelection()
{
    // Reset all icons (so that we reset the selected item displayed)
    for (int i = 0; i < levelOverlayPanels.size(); ++i)
    {
        levelOverlayPanels[i].entireBackground->setMaterialName("General/BlueSphereIcon");
    }
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
    std::string collection;     // Amount required to collect to complete level (Recess depends on speed)
    std::string navigation;     // Navigation difficulty of level
    std::string holdout;        // Holdout description
    std::string length;         // Length of level
    
    std::string accuracy;
    std::string collected;      // Targets player collected
    std::string mistakes;       // Non-targets player collected
    std::string avoided;        // Non-targets player avoided
    std::string missed;         // Targets player avoided
    
    // Assign description value to each variable
    switch (level.phase)
    {
        case 'A':
            type = "Color&Sound";
            break;
        case 'B':
            type = "Shape&Sound";
            break;
        case 'C':
            type = "Sound Only";
            break;
        case 'D':
            type = "All Features";
            break;
        case 'E':
            type = "Recess";
            break;
    }
    nback = Util::toStringInt(level.nback);
    if (level.phase != 'E')
        collection = Util::toStringInt(level.collectionCriteria.size());
    else
        collection = "-";
    
    switch (level.difficultyX)
    {
        case DIFFICULTY_EASY:
            navigation = "easy";
            break;
        case DIFFICULTY_NORMAL:
            navigation = "normal";
            break;
        case DIFFICULTY_HARD:
            navigation = "hard";
            break;
    }
    holdout = level.hasHoldout ? "yes" : "no";
    length = Util::toStringInt(level.stageTime);
    
    // Check if player ever played the level
    if (progress.rating < 0)
    {
        accuracy = "-";
        collected = "-";
        mistakes = "-";
        avoided = "-";
        missed = "-";
    }
    else
    {
        accuracy = Util::toStringInt(progress.accuracy * 100.0);
        collected = Util::toStringInt(progress.numCorrect);
        mistakes = Util::toStringInt(progress.numWrong);
        avoided = Util::toStringInt(progress.numSafe);
        missed = Util::toStringInt(progress.numMissed);
    }
    
    // Set the text displaying the level details
    levelDetails.values->setCaption(type + "\n" +
                                    nback + "\n" +
                                    collection + "\n" +
                                    navigation + "\n" +
                                    holdout + "\n" +
                                    length + "\n" +
                                    "\n" +
                                    accuracy + "\n" +
                                    collected + "\n" +
                                    mistakes + "\n" +
                                    avoided + "\n" +
                                    missed + "\n");
    
    // Turn on the play button since we have a valid level selected
    if (player->levelRequest && player->levelRequest->second.rating < 0)
        playButtonBackground->setMaterialName("General/PlayButton");
}

// Set how the levels in the history panel are displayed without them being selected
void HudSchedulerMenu::setScheduleHistory()
{
    //std::cout << "Schedule History: " << player->scheduler->scheduleHistory.size() << std::endl;
    
    int startingScheduleIndex = getStartingSchedulerHistoryIndex();
    int i = 0;
    for (; i < historyOverlayPanels.size(); ++i) {
        
        if (startingScheduleIndex + i < player->scheduler->scheduleHistory.size())
        {
            // Display the previously played
            StageRequest level = player->scheduler->scheduleHistory[startingScheduleIndex + i].first;
            PlayerProgress progress = player->scheduler->scheduleHistory[startingScheduleIndex + i].second;
        
            // Display our merit earned for the level (could also display an icon for the type of level)
            if (progress.rating >= 5)
                historyOverlayPanels[i].entireBackground->setMaterialName("General/LevelBar5Fill");
            else if (progress.rating >= 4)
                historyOverlayPanels[i].entireBackground->setMaterialName("General/LevelBar4Fill");
            else if (progress.rating >= 3)
                historyOverlayPanels[i].entireBackground->setMaterialName("General/LevelBar3Fill");
            else if (progress.rating == 2)
                historyOverlayPanels[i].entireBackground->setMaterialName("General/LevelBar2Fill");
            else if (progress.rating == 1)
                historyOverlayPanels[i].entireBackground->setMaterialName("General/LevelBar1Fill");
            else
                historyOverlayPanels[i].entireBackground->setMaterialName("General/LevelBar0Fill");
            historyOverlayPanels[i].title->setCaption(Util::toStringFloat(progress.nbackDelta));
        }
        else if (i == player->scheduler->scheduleHistory.size())
        {
            // The next we will play should show that it is available or the next one we will be completing
            historyOverlayPanels[i].entireBackground->setMaterialName("General/LevelBar0Fill");
            historyOverlayPanels[i].title->setCaption("-");
        }
        else
        {
            // Not there yet, so show that it is unavailable
            historyOverlayPanels[i].entireBackground->setMaterialName("General/LevelBarUnavailable");
            historyOverlayPanels[i].title->setCaption("-");
        }
    }
}
