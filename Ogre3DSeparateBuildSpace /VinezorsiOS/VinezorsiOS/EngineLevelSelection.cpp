//
//  EngineLevelSelection.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/16/14.
//
//

#include "EngineLevelSelection.h"
#include "EngineStateManager.h"
#include "HudLevelSelection.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineLevelSelection::EngineLevelSelection(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
}

EngineLevelSelection::~EngineLevelSelection()
{
}

void EngineLevelSelection::enter()
{
    alloc();
    player->startMenu();
    
    // Set skybox
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
}

void EngineLevelSelection::exit()
{
    dealloc();
}

void EngineLevelSelection::update(float elapsed)
{
    hud->update(elapsed);
}

void EngineLevelSelection::activatePerformSwipeUp()
{
    int menuRow = player->getMenuRowIndex() - 1;
    if (player->getLevels()->hasLevelRow(menuRow))
        player->setMenuRowIndex(menuRow);
}

void EngineLevelSelection::activatePerformSwipeDown()
{
    int menuRow = player->getMenuRowIndex() + 1;
    if (player->getLevels()->hasLevelRow(menuRow + 2)) // add 2 for the end row
        player->setMenuRowIndex(menuRow);
}

void EngineLevelSelection::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI != "")
        player->reactGUI();
    if (testForLevelButtons(queryGUI))
    {
        
    }
    else if (queryGUI == "back")
    {
        player->saveProgress(globals.savePath);
        engineStateMgr->requestPopEngine();
    }
    else if (queryGUI == "godown")
    {
        activatePerformSwipeDown();
    }
    else if (queryGUI == "goup")
    {
        activatePerformSwipeUp();
    }
    else if (queryGUI == "checktutorials")
    {
        TutorialManager* tutorialMgr = player->getTutorialMgr();
        if (tutorialMgr->isEnabled())
            tutorialMgr->disable();
        else
            tutorialMgr->enable();
    }

}

void EngineLevelSelection::activatePerformPinch()
{
#if defined(DEBUG_MODE)
    if (player->isLevelAvailable(NUM_LEVELS * NUM_TASKS - 1))
    {
        PlayerProgress value;
        value.rating = -1;
        player->setAllProgressTo(value);
    }
    else
    {
        PlayerProgress value;
        value.rating = 5;
        player->setAllProgressTo(value);
    }
#endif
}

#if !defined(OGRE_IS_IOS)
void EngineLevelSelection::mouseMoved(const OIS::MouseEvent &evt)
{
}

void EngineLevelSelection::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            activatePerformSingleTap(evt.state.X.abs, evt.state.Y.abs);
            break;
        default:
            break;
    }
}

void EngineLevelSelection::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            break;
        default:
            break;
    }
}

void EngineLevelSelection::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    switch (keyEventRef.key)
    {
        case OIS::KC_UP:
        {
            activatePerformSwipeUp();
            break;
        }
        case OIS::KC_DOWN:
        {
            activatePerformSwipeDown();
            break;
        }
#ifdef DEBUG_MODE
        case OIS::KC_K:
        {
            PlayerProgress value;
            value.rating = -1;
            player->setAllProgressTo(value);
            break;
        }
        case OIS::KC_L:
        {
            PlayerProgress value;
            value.rating = 5;
            player->setAllProgressTo(value);
            break;
        }
#endif
        default:
            break;
    }
}

void EngineLevelSelection::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineLevelSelection::requestResize()
{
    if (hud) hud->adjust();
}

bool EngineLevelSelection::testForLevelButtons(const std::string & queryGUI)
{
    for (int c = 0; c < 3; ++c)
        for (int i = 0; i < NUM_TASKS; ++i)
        {
            std::string pre = "level";
            std::string queryTest = pre + (char)('A' + c) + Util::toStringInt(i);
            
            if (queryGUI == queryTest)
            {
                LevelSet* levels = player->getLevels();
                int levelSelect = levels->getLevelNo(player->getMenuRowIndex() + c, i);
                if  (player->isLevelAvailable(levelSelect))
                {
                    int row = levels->getLevelRow(levelSelect);
                    int col = levels->getLevelCol(levelSelect);
                    player->setLevelRequest(row, col);
                    engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
                }
                return true;
            }
        }
    return false;
}

void EngineLevelSelection::alloc()
{
    hud = new HudLevelSelection(player);
}

void EngineLevelSelection::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}
