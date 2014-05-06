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
    setOverlay();
    showOverlays();
}

void HudLevelSelection::adjust()
{
    setOverlay();
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
    levelSetStars = std::vector<TextAreaOverlayElement*>(LEVEL_ITEM_HEIGHT);
    for (int i = 0; i < levelSetStars.size(); ++i)
    {
        levelSetStars[i] = static_cast<TextAreaOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "LevelSetStars" + Util::toStringInt(i)));
    }
    
    backButtonBackground = static_cast<PanelOverlayElement*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "LevelSelectionBackButtonBackground"));
    
    // Back Button plus the Vector of 2-D Buttons
    // Order matters that the Back Button is first
    buttons = std::vector<HudButton>(1 + LEVEL_ITEM_WIDTH * LEVEL_ITEM_HEIGHT);
    
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
            if (j == levelItemBackgrounds[i].size() - 1)
            {
                levelItemBackgrounds[i][j]->addChild(levelSetStars[i]);
            }
        }
    }
    overlay1->add2D(backButtonBackground);
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
    for (int i = 0; i < levelSetStars.size(); ++i)
        OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(levelSetStars[i]);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroyOverlayElement(backButtonBackground);
    OgreFramework::getSingletonPtr()->m_pOverlayMgr->destroy(overlays[0]);
}

void HudLevelSelection::setOverlay()
{
    // Link and set resources
    levelSelectPrompt->setMetricsMode(GMM_RELATIVE);
    levelSelectPrompt->setPosition(0.10, 0.05);
    levelSelectPrompt->setDimensions(0.80, 0.20);
    levelSelectPrompt->setMaterialName("General/TextArtLevelSelection");
    
    // Assign a 2-D level selection map for each button
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
            setLevelButton(levelItemBackgrounds[i][j], levelItemPlanets[i][j], levelItemNames[i][j], i, j);
    for (int i = 0; i < levelSetStars.size(); ++i)
    {
        int earnedStars = player->getTotalLevelRating(i);
        int totalStars = 3 * NUM_TASKS;
        levelSetStars[i]->setCaption(Util::toStringInt(earnedStars) + "/" + Util::toStringInt(totalStars));
    }
    
    backButtonBackground->setMaterialName("General/BackButton");
    
    float sx = 0.12;
    float sy = 0.30;
    float dx = 0.14;
    float dy = 0.14;
    float curx = sx;
    float cury = sy;
    // Set orientations for background per level
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
    {
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
        {
            std::string buttonName = "level";
            buttonName += (char)('A' + i) + Util::toStringInt(j);
            buttons[1 + i * LEVEL_ITEM_WIDTH + j].setButton(buttonName, overlays[0], GMM_RELATIVE, Vector2(curx, cury), Vector2(0.10, 0.10), levelItemBackgrounds[i][j], NULL);
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
            levelItemPlanets[i][j]->setMetricsMode(GMM_RELATIVE);
            levelItemPlanets[i][j]->setPosition(0.02, 0.02);
            levelItemPlanets[i][j]->setDimensions(0.065, 0.065);
        }
    }
    // Set orientations for the title per level
    for (int i = 0; i < levelItemNames.size(); ++i)
    {
        for (int j = 0; j < levelItemNames[i].size(); ++j)
        {
            levelItemNames[i][j]->setMetricsMode(GMM_RELATIVE);
            levelItemNames[i][j]->setAlignment(TextAreaOverlayElement::Center);
            levelItemNames[i][j]->setPosition(0.05, -0.02);
            levelItemNames[i][j]->setCharHeight(0.025);
            levelItemNames[i][j]->setFontName("Arial");
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
    buttons[BUTTON_BACK].setButton("back", overlays[0], GMM_RELATIVE, Vector2(0.20, 0.78), Vector2(0.30, 0.10), backButtonBackground, NULL);
}

void HudLevelSelection::link(Player* player)
{
    this->player = player;
}

void HudLevelSelection::unlink()
{
    this->player = NULL;
}

void HudLevelSelection::updateDisplay()
{
    // Assign a 2-D level selection map for each button
    for (int i = 0; i < levelItemBackgrounds.size(); ++i)
        for (int j = 0; j < levelItemBackgrounds[i].size(); ++j)
            setLevelButton(levelItemBackgrounds[i][j], levelItemPlanets[i][j], levelItemNames[i][j], i, j);
    // Update the display of stars earned per row
    for (int i = 0; i < levelSetStars.size(); ++i)
    {
        int earnedStars = player->getTotalLevelRating(i);
        int totalStars = 3 * NUM_TASKS;
        levelSetStars[i]->setCaption(Util::toStringInt(earnedStars) + "/" + Util::toStringInt(totalStars));
    }
}

// Assigns the texture description for each level depending on player performance
// in previous and that level
void HudLevelSelection::setLevelButton(PanelOverlayElement* levelBackground, PanelOverlayElement* levelItem, TextAreaOverlayElement* levelName, int row, int col)
{
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
    std::string name = Util::toStringInt(row + 1) + '-' + char('A' + col);
    levelName->setCaption(name);
}