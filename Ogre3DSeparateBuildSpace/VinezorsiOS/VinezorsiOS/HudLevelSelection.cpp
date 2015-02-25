//
//  HudLevelSelection.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#include "HudLevelSelection.h"
#include "Player.h"
#include "LevelSet.h"

extern Util::ConfigGlobal globals;

HudLevelSelection::HudLevelSelection(Player* player)
: Hud()
{
    link(player);
    init();
}

HudLevelSelection::~HudLevelSelection()
{
    dealloc();
    unlink();
}

void HudLevelSelection::init()
{
    alloc();
    initOverlay();
    showOverlays();
}

void HudLevelSelection::adjust()
{
    initOverlay();
    player->getTutorialMgr()->adjust();
}

void HudLevelSelection::update(float elapsed)
{
    updateDisplay();
}

void HudLevelSelection::alloc()
{
    // Allocate Resources
    levelSelectEntireBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectEntireBackground"));
    levelSelectPromptText = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelSelectPromptText"));
    
    levelItemBackgrounds = std::vector< std::vector<PanelOverlayElement*> >(LEVEL_ITEM_HEIGHT);
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
    {
        levelItemBackgrounds[i] = std::vector<PanelOverlayElement*>(LEVEL_ITEM_WIDTH);
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
        {
            levelItemBackgrounds[i][j] = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelection" + Util::toStringInt(i) + (char)('A' + j)));
        }
    }
    levelItemPlanets = std::vector< std::vector<PanelOverlayElement*> >(LEVEL_ITEM_HEIGHT);
    for (int i = 0; i < levelItemPlanets.size(); ++i)
    {
        levelItemPlanets[i] = std::vector<PanelOverlayElement*>(LEVEL_ITEM_WIDTH);
        for (int j = 0; j < levelItemPlanets[i].size(); ++j)
        {
            levelItemPlanets[i][j] = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelPlanet" + Util::toStringInt(i) + (char)('A' + j)));
        }
    }
    levelItemNames = std::vector< std::vector<TextAreaOverlayElement*> >(LEVEL_ITEM_HEIGHT);
    for (int i = 0; i < levelItemNames.size(); ++i)
    {
        levelItemNames[i] = std::vector<TextAreaOverlayElement*>(LEVEL_ITEM_WIDTH);
        for (int j = 0; j < levelItemNames[i].size(); ++j)
        {
            levelItemNames[i][j] = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelName" + Util::toStringInt(i) + (char)('A' + j)));
        }
    }
    levelItemScores = std::vector< std::vector<TextAreaOverlayElement*> >(LEVEL_ITEM_HEIGHT);
    for (int i = 0; i < levelItemScores.size(); ++i)
    {
        levelItemScores[i] = std::vector<TextAreaOverlayElement*>(LEVEL_ITEM_WIDTH);
        for (int j = 0; j < levelItemScores[i].size(); ++j)
        {
            levelItemScores[i][j] = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelScore" + Util::toStringInt(i) + (char)('A' + j)));
        }
    }
    levelSetNames = std::vector<TextAreaOverlayElement*>(LEVEL_ITEM_HEIGHT);
    for (int i = 0; i < levelSetNames.size(); ++i)
    {
        levelSetNames[i] = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelSetNames" + Util::toStringInt(i)));
    }
    levelSetStars = std::vector<TextAreaOverlayElement*>(LEVEL_ITEM_HEIGHT);
    for (int i = 0; i < levelSetStars.size(); ++i)
    {
        levelSetStars[i] = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelSetStars" + Util::toStringInt(i)));
    }
    levelSetScores = std::vector<TextAreaOverlayElement*>(LEVEL_ITEM_HEIGHT);
    for (int i = 0; i < levelSetScores.size(); ++i)
    {
        levelSetScores[i] = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelSetScores" + Util::toStringInt(i)));
    }
    
    levelTotalScoreBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSetTotalScoreBackground"));
    levelTotalScore = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelSetTotalScore"));
    
    buttonGoUpBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSetGoUpButtonBackground"));
    buttonGoDownBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSetGoDownButtonBackground"));
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionBackButtonBackground"));
    checkDisplayEnableTutorialsBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionCheckDisplayEnableTutorialsBackground"));
    buttonEnableTutorialsBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionEnableTutorialsBackground"));
    textEnableTutorialsPrompt = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelSelectionEnableTutorialsPrompt"));
    
    // Back Button plus the Vector of 2-D Buttons
    // Order matters that the Back Button is first
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS + LEVEL_ITEM_WIDTH * LEVEL_ITEM_HEIGHT);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("LevelSelectionOverlay");
    overlay1->add2D(levelSelectEntireBackground);
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
    {
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
        {
            levelSelectEntireBackground->addChild(levelItemBackgrounds[i][j]);
            //levelItemBackgrounds[i][j]->addChild(levelItemPlanets[i][j]);
            //levelItemBackgrounds[i][j]->addChild(levelItemNames[i][j]);
            levelItemBackgrounds[i][j]->addChild(levelItemScores[i][j]);
            if (j == 0)
                levelItemBackgrounds[i][j]->addChild(levelSetNames[i]);
            if (j == levelItemBackgrounds[i].size() - 1)
            {
                levelItemBackgrounds[i][j]->addChild(levelSetStars[i]);
                levelItemBackgrounds[i][j]->addChild(levelSetScores[i]);
            }
        }
    }
    levelSelectEntireBackground->addChild(levelTotalScoreBackground);
    levelTotalScoreBackground->addChild(levelTotalScore);
    levelSelectEntireBackground->addChild(buttonGoUpBackground);
    levelSelectEntireBackground->addChild(buttonGoDownBackground);
    levelSelectEntireBackground->addChild(backButtonBackground);
    levelSelectEntireBackground->addChild(buttonEnableTutorialsBackground);
    buttonEnableTutorialsBackground->addChild(checkDisplayEnableTutorialsBackground);
    //buttonEnableTutorialsBackground->addChild(textEnableTutorialsPrompt);
    overlays.push_back(overlay1);
    
}

void HudLevelSelection::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSelectEntireBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSelectPromptText);
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
            OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelItemBackgrounds[i][j]);
    for (int i = 0; i < levelItemPlanets.size(); ++i)
        for (int j = 0; j < levelItemPlanets[i].size(); ++j)
            OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelItemPlanets[i][j]);
    for (int i = 0; i < levelItemNames.size(); ++i)
        for (int j = 0; j < levelItemNames[i].size(); ++j)
            OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelItemNames[i][j]);
    for (int i = 0; i < levelItemScores.size(); ++i)
        for (int j = 0; j < levelItemScores[i].size(); ++j)
            OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelItemScores[i][j]);
    for (int i = 0; i < levelSetNames.size(); ++i)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSetNames[i]);
    for (int i = 0; i < levelSetStars.size(); ++i)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSetStars[i]);
    for (int i = 0; i < levelSetScores.size(); ++i)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSetScores[i]);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelTotalScore);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelTotalScoreBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(buttonGoUpBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(buttonGoDownBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(checkDisplayEnableTutorialsBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(buttonEnableTutorialsBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(textEnableTutorialsPrompt);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudLevelSelection::initOverlay()
{
    // Link and set resources
    levelSelectEntireBackground->setMetricsMode(GMM_RELATIVE);
    levelSelectEntireBackground->setPosition(0.0, 0.0);
    levelSelectEntireBackground->setDimensions(1.0, 1.0);
    levelSelectEntireBackground->setMaterialName("General/ScreenBackgroundLevelSelect");
    
    levelSelectPromptText->setMetricsMode(GMM_RELATIVE);
    levelSelectPromptText->setAlignment(TextAreaOverlayElement::Center);
    levelSelectPromptText->setPosition(0.270, 0.085);
    levelSelectPromptText->setCharHeight(0.046 * FONT_SZ_MULT);
    levelSelectPromptText->setFontName("MainSmall");
    levelSelectPromptText->setCaption("Level Selection");
    
    float sx = 0.200;
    float sy = 0.285;
    float dx = 0.120;
    float dy = 0.150;
    float curx = sx;
    float cury = sy;
    // Set orientations for background per level
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
    {
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
        {
            // calculate dimensions for button size and make sure it's square
            double bw = 0.09;
            double bh = bw * globals.screenWidth / globals.screenHeight;
            
            std::string buttonName = "level";
            buttonName += (char)('A' + i) + Util::toStringInt(j);
            buttons[NUM_UNIQUE_BUTTONS + i * LEVEL_ITEM_WIDTH + j].setButton(buttonName, overlays[0], GMM_RELATIVE, Vector2(curx, cury), Vector2(bw, bh), levelItemBackgrounds[i][j], NULL);
            curx += dx;
        }
        curx = sx;
        cury += dy;
    }
    // Set orientations for the planet per level
    for (int i = 0; i < levelItemPlanets.size(); ++i)
    {
        for (int j = 0; j < levelItemPlanets[i].size(); ++j)
        {
            // calculate dimensions for button size and make sure it's square
            float percsize = 0.05;
            float dimen = globals.screenWidth < globals.screenHeight ? percsize * globals.screenWidth / globals.screenHeight : percsize * globals.screenHeight / globals.screenWidth;
            
            levelItemPlanets[i][j]->setMetricsMode(GMM_RELATIVE);
            levelItemPlanets[i][j]->setPosition(0.015, 0.015);
            levelItemPlanets[i][j]->setDimensions(dimen, dimen);
        }
    }
    // Set orientations for the title per level
    for (int i = 0; i < levelItemNames.size(); ++i)
    {
        for (int j = 0; j < levelItemNames[i].size(); ++j)
        {
            levelItemNames[i][j]->setMetricsMode(GMM_RELATIVE);
            levelItemNames[i][j]->setAlignment(TextAreaOverlayElement::Center);
            levelItemNames[i][j]->setPosition(0.06, -0.02);
            levelItemNames[i][j]->setCharHeight(0.025 * FONT_SZ_MULT);
            levelItemNames[i][j]->setFontName("MainSmall");
        }
    }
    // Set orientations for the score per level
    for (int i = 0; i < levelItemScores.size(); ++i)
    {
        for (int j = 0; j < levelItemScores[i].size(); ++j)
        {
            levelItemScores[i][j]->setMetricsMode(GMM_RELATIVE);
            levelItemScores[i][j]->setAlignment(TextAreaOverlayElement::Center);
            levelItemScores[i][j]->setPosition(0.050, 0.12);
            levelItemScores[i][j]->setCharHeight(0.014 * FONT_SZ_MULT);
            levelItemScores[i][j]->setFontName("MainSmall");
        }
    }
    // Set orientations for n-level
    for (int i = 0; i < levelSetNames.size(); ++i)
    {
        levelSetNames[i]->setMetricsMode(GMM_RELATIVE);
        levelSetNames[i]->setAlignment(TextAreaOverlayElement::Center);
        levelSetNames[i]->setPosition(-0.04, 0.03);
        levelSetNames[i]->setCharHeight(0.030 * FONT_SZ_MULT);
        levelSetNames[i]->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
        levelSetNames[i]->setFontName("MainSmall");
        levelSetNames[i]->setCaption("1");
        
    }
    // Set orientations for total stars earned per row
    for (int i = 0; i < levelSetStars.size(); ++i)
    {
        levelSetStars[i]->setMetricsMode(GMM_RELATIVE);
        levelSetStars[i]->setAlignment(TextAreaOverlayElement::Center);
        levelSetStars[i]->setPosition(0.1375, 0.0550);
        levelSetStars[i]->setCharHeight(0.018 * FONT_SZ_MULT);
        levelSetStars[i]->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
        levelSetStars[i]->setFontName("MainSmall");
    }
    // Set orientations for total score earned per row
    for (int i = 0; i < levelSetScores.size(); ++i)
    {
        levelSetScores[i]->setMetricsMode(GMM_RELATIVE);
        levelSetScores[i]->setAlignment(TextAreaOverlayElement::Center);
        levelSetScores[i]->setPosition(0.1910, 0.1025);
        levelSetScores[i]->setCharHeight(0.020 * FONT_SZ_MULT);
        levelSetScores[i]->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
        levelSetScores[i]->setFontName("MainSmall");
    }
    
    // Display Text
    levelTotalScoreBackground->setMetricsMode(GMM_RELATIVE);
    levelTotalScoreBackground->setPosition(0.550, 0.750);
    levelTotalScoreBackground->setDimensions(0.30, 0.10);
    
    levelTotalScore->setMetricsMode(GMM_RELATIVE);
    levelTotalScore->setAlignment(TextAreaOverlayElement::Right);
    levelTotalScore->setPosition(0.26, 0.05);
    levelTotalScore->setCharHeight(0.030 * FONT_SZ_MULT);
    levelTotalScore->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    levelTotalScore->setFontName("MainSmall");
    
    textEnableTutorialsPrompt->setMetricsMode(GMM_RELATIVE);
    textEnableTutorialsPrompt->setAlignment(TextAreaOverlayElement::Center);
    textEnableTutorialsPrompt->setPosition(-0.0625, 0.0);
    textEnableTutorialsPrompt->setCharHeight(0.020 * FONT_SZ_MULT);
    textEnableTutorialsPrompt->setDimensions(0.80, 0.20);
    textEnableTutorialsPrompt->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    textEnableTutorialsPrompt->setFontName("MainSmall");
    textEnableTutorialsPrompt->setCaption("Tutorials");
    
    // Display Text
    checkDisplayEnableTutorialsBackground->setMetricsMode(GMM_RELATIVE);
    checkDisplayEnableTutorialsBackground->setPosition(0.145, 0.0);
    checkDisplayEnableTutorialsBackground->setDimensions(0.0350 * globals.screenHeight / globals.screenWidth, 0.0350);
    
    // The Up Button
    {
        // calculate dimensions for button size and make sure it's square
        float percsize = 0.090;
        float dimen = globals.screenWidth < globals.screenHeight ? percsize * globals.screenWidth / globals.screenHeight : percsize * globals.screenHeight / globals.screenWidth;
        buttons[BUTTON_UP].setButton("goup", overlays[0], GMM_RELATIVE, Vector2(0.0650, 0.4225), Vector2(dimen, dimen), buttonGoUpBackground, NULL);
    }
    // The Down Button
    {
        // calculate dimensions for button size and make sure it's square
        float percsize = 0.085;
        float dimen = globals.screenWidth < globals.screenHeight ? percsize * globals.screenWidth / globals.screenHeight : percsize * globals.screenHeight / globals.screenWidth;
        buttons[BUTTON_DOWN].setButton("godown", overlays[0], GMM_RELATIVE, Vector2(0.0650, 0.5275), Vector2(dimen, dimen), buttonGoDownBackground, NULL);
    }
    // The Enable Tutorial Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float pw = 0.2050;
        float ph = 0.0350;
        buttons[BUTTON_ENABLETUTORIALS].setButton("checktutorials", overlays[0], GMM_RELATIVE, Vector2(0.205, 0.790), Vector2(pw, ph), buttonEnableTutorialsBackground, NULL);
    }
    
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.175, 0.85), Vector2(0.15, 0.05), backButtonBackground, NULL);
    //backButtonBackground->setMaterialName("General/CheckboxBlank");
    
    updateDisplay();
}

void HudLevelSelection::link(Player* player)
{
    this->player = player;
}

void HudLevelSelection::unlink()
{
    this->player = NULL;
}

// Given the player's selection index, convert the index to a mapping the 2-D button set
int HudLevelSelection::convertLevelRowToButtonRow() const
{
    int menuRowIndex = player->getMenuRowIndex();
    return menuRowIndex;
}

void HudLevelSelection::updateDisplay()
{
    // Assign a 2-D level selection map for each button
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
            setLevelButton(levelItemBackgrounds[i][j], levelItemPlanets[i][j], levelItemNames[i][j], levelItemScores[i][j], player->getMenuRowIndex() + i, j);
    
    for (int i = 0; i < levelSetNames.size(); ++i)
    {
        levelSetNames[i]->setCaption(Util::toStringInt(player->getMenuRowIndex() + i + 1));
    }
    
    // Update the display of stars earned per row
    for (int i = 0; i < levelSetStars.size(); ++i)
    {
        int requiredStars = player->getLevels()->getTotalRowRequirement(player->getMenuRowIndex() + i);
        int earnedStars = Util::clamp(player->getTotalLevelRating(player->getMenuRowIndex() + i), 0, requiredStars);
        float totalScore = player->getTotalLevelScore(player->getMenuRowIndex() + i);
        levelSetStars[i]->setCaption(Util::toStringInt(earnedStars) + "/" + Util::toStringInt(requiredStars));
        if (totalScore > 0.0)
            levelSetScores[i]->setCaption(Util::toStringInt(totalScore));
        else
            levelSetScores[i]->setCaption("-");
    }
    float entireScore = player->getTotalLevelScore();
    levelTotalScore->setCaption(Util::toStringInt(entireScore));
    
    if (player->getTutorialMgr()->isEnabled())
        checkDisplayEnableTutorialsBackground->setMaterialName("General/CheckboxGreen");
    else
        checkDisplayEnableTutorialsBackground->setMaterialName("General/CheckboxBlank");
    
}

// Assigns the texture description for each level depending on player performance
// in previous and that level
void HudLevelSelection::setLevelButton(PanelOverlayElement* levelBackground, PanelOverlayElement* levelItem, TextAreaOverlayElement* levelName, TextAreaOverlayElement* levelScore, int row, int col)
{
    levelScore->setCaption("-");
    levelScore->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    std::string name = Util::toStringInt(row + 1) + '-' + char('A' + col);
    levelName->setCaption(name);
    
    LevelSet* levels = player->getLevels();
    int levelSelect = levels->getLevelNo(row, col);
    if (player->isLevelAvailable(levelSelect))
    {
        if (player->hasLevelProgress(levelSelect))
        {
            PlayerProgress progress = player->getLevelProgress(levelSelect);
            if (progress.rating >= 5)
                levelBackground->setMaterialName("General/LevelBar5Fill");
            else if (progress.rating >= 4)
                levelBackground->setMaterialName("General/LevelBar4Fill");
            else if (progress.rating >= 3)
                levelBackground->setMaterialName("General/LevelBar3Fill");
            else if (progress.rating == 2)
                levelBackground->setMaterialName("General/LevelBar2Fill");
            else if (progress.rating == 1)
                levelBackground->setMaterialName("General/LevelBar1Fill");
            else
                levelBackground->setMaterialName("General/LevelBar0Fill");
            
            levelScore->setCaption(Util::toStringInt(progress.score));
        }
        else
            levelBackground->setMaterialName("General/LevelBar0Fill");
        
        
        
        //levelItem->setMaterialName("General/PlanetAvailable");
        levelName->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    }
    else
    {
        if (player->hasLevelProgress(levelSelect))
        {
            PlayerProgress progress = player->getLevelProgress(levelSelect);
            if (progress.rating >= 0) levelScore->setCaption(Util::toStringInt(progress.score));
        }
        levelBackground->setMaterialName("General/LevelBarUnavailable");
        //levelItem->setMaterialName("General/PlanetUnavailable");
        levelName->setColour(ColourValue::ColourValue(0.5, 0.5, 0.5));
    }
}