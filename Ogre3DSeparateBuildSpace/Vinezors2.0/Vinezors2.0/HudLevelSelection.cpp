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
    levelSelectPrompt = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectPrompt"));
    
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
    buttonEnableTutorialsBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionEnableTutorialsBackground"));
    textEnableTutorialsPrompt = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelSelectionEnableTutorialsPrompt"));
    
    // Back Button plus the Vector of 2-D Buttons
    // Order matters that the Back Button is first
    buttons = std::vector<HudButton>(NUM_UNIQUE_BUTTONS + LEVEL_ITEM_WIDTH * LEVEL_ITEM_HEIGHT);
    
    // Create an overlay, and add the panel
    Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("LevelSelectionOverlay");
    overlay1->add2D(levelSelectPrompt);
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
    {
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
        {
            overlay1->add2D(levelItemBackgrounds[i][j]);
            levelItemBackgrounds[i][j]->addChild(levelItemPlanets[i][j]);
            levelItemBackgrounds[i][j]->addChild(levelItemNames[i][j]);
            levelItemBackgrounds[i][j]->addChild(levelItemScores[i][j]);
            if (j == levelItemBackgrounds[i].size() - 1)
            {
                levelItemBackgrounds[i][j]->addChild(levelSetStars[i]);
                levelItemBackgrounds[i][j]->addChild(levelSetScores[i]);
            }
        }
    }
    overlay1->add2D(levelTotalScoreBackground);
    levelTotalScoreBackground->addChild(levelTotalScore);
    overlay1->add2D(buttonGoUpBackground);
    overlay1->add2D(buttonGoDownBackground);
    overlay1->add2D(backButtonBackground);
    overlay1->add2D(buttonEnableTutorialsBackground);
    buttonEnableTutorialsBackground->addChild(textEnableTutorialsPrompt);
    overlays.push_back(overlay1);
}

void HudLevelSelection::dealloc()
{
    // Delete children first, then parents
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSelectPrompt);
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
    for (int i = 0; i < levelSetStars.size(); ++i)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSetStars[i]);
    for (int i = 0; i < levelSetScores.size(); ++i)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSetScores[i]);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelTotalScore);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelTotalScoreBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(buttonGoUpBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(buttonGoDownBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(buttonEnableTutorialsBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(textEnableTutorialsPrompt);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudLevelSelection::initOverlay()
{
    // Link and set resources
    levelSelectPrompt->setMetricsMode(GMM_RELATIVE);
    levelSelectPrompt->setPosition(0.10, 0.05);
    levelSelectPrompt->setDimensions(0.80, 0.20);
    levelSelectPrompt->setMaterialName("General/TextArtLevelSelection");
    
    backButtonBackground->setMaterialName("General/BackButton");
    
    float sx = 0.12;
    float sy = 0.250;
    float dx = 0.135;
    float dy = 0.175;
    float curx = sx;
    float cury = sy;
    // Set orientations for background per level
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
    {
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
        {
            // calculate dimensions for button size and make sure it's square
            float percsize = 0.15;
            float dimen = globals.screenWidth < globals.screenHeight ? percsize * globals.screenWidth / globals.screenHeight : percsize * globals.screenHeight / globals.screenWidth;
            
            std::string buttonName = "level";
            buttonName += (char)('A' + i) + Util::toStringInt(j);
            buttons[NUM_UNIQUE_BUTTONS + i * LEVEL_ITEM_WIDTH + j].setButton(buttonName, overlays[0], GMM_RELATIVE, Vector2(curx, cury), Vector2(dimen, dimen), levelItemBackgrounds[i][j], NULL);
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
            float percsize = 0.10;
            float dimen = globals.screenWidth < globals.screenHeight ? percsize * globals.screenWidth / globals.screenHeight : percsize * globals.screenHeight / globals.screenWidth;
            
            levelItemPlanets[i][j]->setMetricsMode(GMM_RELATIVE);
            levelItemPlanets[i][j]->setPosition(0.02, 0.02);
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
            levelItemNames[i][j]->setCharHeight(0.025);
            levelItemNames[i][j]->setFontName("Arial");
        }
    }
    // Set orientations for the score per level
    for (int i = 0; i < levelItemScores.size(); ++i)
    {
        for (int j = 0; j < levelItemScores[i].size(); ++j)
        {
            levelItemScores[i][j]->setMetricsMode(GMM_RELATIVE);
            levelItemScores[i][j]->setAlignment(TextAreaOverlayElement::Center);
            levelItemScores[i][j]->setPosition(0.050, 0.105);
            levelItemScores[i][j]->setCharHeight(0.016);
            levelItemScores[i][j]->setFontName("Arial");
        }
    }
    // Set orientations for total stars earned per row
    for (int i = 0; i < levelSetStars.size(); ++i)
    {
        levelSetStars[i]->setMetricsMode(GMM_RELATIVE);
        levelSetStars[i]->setAlignment(TextAreaOverlayElement::Center);
        levelSetStars[i]->setPosition(0.15, 0.05);
        levelSetStars[i]->setCharHeight(0.025);
        levelSetStars[i]->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
        levelSetStars[i]->setFontName("Arial");
    }
    // Set orientations for total score earned per row
    for (int i = 0; i < levelSetScores.size(); ++i)
    {
        levelSetScores[i]->setMetricsMode(GMM_RELATIVE);
        levelSetScores[i]->setAlignment(TextAreaOverlayElement::Center);
        levelSetScores[i]->setPosition(0.25, 0.05);
        levelSetScores[i]->setCharHeight(0.025);
        levelSetScores[i]->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
        levelSetScores[i]->setFontName("Arial");
    }
    
    // Display Text
    levelTotalScoreBackground->setMetricsMode(GMM_RELATIVE);
    levelTotalScoreBackground->setPosition(0.675, 0.850);
    levelTotalScoreBackground->setDimensions(0.30, 0.10);
    levelTotalScoreBackground->setMaterialName("General/ScreenBackground2");
    
    levelTotalScore->setMetricsMode(GMM_RELATIVE);
    levelTotalScore->setAlignment(TextAreaOverlayElement::Center);
    levelTotalScore->setPosition(0.150, 0.040);
    levelTotalScore->setCharHeight(0.04);
    levelTotalScore->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    levelTotalScore->setFontName("Arial");
    
    textEnableTutorialsPrompt->setMetricsMode(GMM_RELATIVE);
    textEnableTutorialsPrompt->setAlignment(TextAreaOverlayElement::Center);
    textEnableTutorialsPrompt->setPosition(-0.150, 0.025);
    textEnableTutorialsPrompt->setCharHeight(0.025);
    textEnableTutorialsPrompt->setDimensions(0.80, 0.20);
    textEnableTutorialsPrompt->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    textEnableTutorialsPrompt->setFontName("Arial");
    textEnableTutorialsPrompt->setCaption("Enable Tutorials");
    
    // The Up Button
    {
        // calculate dimensions for button size and make sure it's square
        float percsize = 0.10;
        float dimen = globals.screenWidth < globals.screenHeight ? percsize * globals.screenWidth / globals.screenHeight : percsize * globals.screenHeight / globals.screenWidth;
        buttons[BUTTON_UP].setButton("goup", overlays[0], GMM_RELATIVE, Vector2(0.025, 0.400), Vector2(dimen, dimen), buttonGoUpBackground, NULL);
    }
    // The Down Button
    {
        // calculate dimensions for button size and make sure it's square
        float percsize = 0.10;
        float dimen = globals.screenWidth < globals.screenHeight ? percsize * globals.screenWidth / globals.screenHeight : percsize * globals.screenHeight / globals.screenWidth;
        buttons[BUTTON_DOWN].setButton("godown", overlays[0], GMM_RELATIVE, Vector2(0.025, 0.550), Vector2(dimen, dimen), buttonGoDownBackground, NULL);
    }
    // The Enable Tutorial Checkbox
    {
        // calculate dimensions for button size and make sure it's square
        float percsize = 0.10;
        float dimen = globals.screenWidth < globals.screenHeight ? percsize * globals.screenWidth / globals.screenHeight : percsize * globals.screenHeight / globals.screenWidth;
        buttons[BUTTON_ENABLETUTORIALS].setButton("checktutorials", overlays[0], GMM_RELATIVE, Vector2(0.900, 0.725), Vector2(dimen, dimen), buttonEnableTutorialsBackground, NULL);
    }
    buttonGoUpBackground->setMaterialName("General/ButtonGoUp");
    buttonGoDownBackground->setMaterialName("General/ButtonGoDown");
    
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.20, 0.78), Vector2(0.30, 0.10), backButtonBackground, NULL);
    
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
    
    int menuRow;
    menuRow = player->getMenuRowIndex() + 1;
    if (player->getLevels()->hasLevelRow(menuRow + 2)) // add 2 for the end row
        buttonGoDownBackground->setMaterialName("General/ButtonGoDown");
    else
        buttonGoDownBackground->setMaterialName("General/ButtonGoDownGray");
    
    menuRow = player->getMenuRowIndex() - 1;
    if (player->getLevels()->hasLevelRow(menuRow))
        buttonGoUpBackground->setMaterialName("General/ButtonGoUp");
    else
        buttonGoUpBackground->setMaterialName("General/ButtonGoUpGray");
    
    if (player->getTutorialMgr()->isEnabled())
        buttonEnableTutorialsBackground->setMaterialName("General/CheckboxGreen");
    else
        buttonEnableTutorialsBackground->setMaterialName("General/CheckboxBlank");
    
}

// Assigns the texture description for each level depending on player performance
// in previous and that level
void HudLevelSelection::setLevelButton(PanelOverlayElement* levelBackground, PanelOverlayElement* levelItem, TextAreaOverlayElement* levelName, TextAreaOverlayElement* levelScore, int row, int col)
{
    levelScore->setCaption("-");
    levelScore->setColour(ColourValue::ColourValue(1.0, 1.0, 0.0));
    std::string name = Util::toStringInt(row + 1) + '-' + char('A' + col);
    levelName->setCaption(name);
    
    LevelSet* levels = player->getLevels();
    int levelSelect = levels->getLevelNo(row, col);
    if (player->isLevelAvailable(levelSelect))
    {
        if (player->hasLevelProgress(levelSelect))
        {
            PlayerProgress progress = player->getLevelProgress(levelSelect);
            if (progress.rating >= 3)
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
        levelItem->setMaterialName("General/PlanetAvailable");
        levelName->setColour(ColourValue::ColourValue(1.0, 1.0, 1.0));
    }
    else
    {
        levelBackground->setMaterialName("General/LevelBar0Fill");
        levelItem->setMaterialName("General/PlanetUnavailable");
        levelName->setColour(ColourValue::ColourValue(0.5, 0.5, 0.5));
    }
}