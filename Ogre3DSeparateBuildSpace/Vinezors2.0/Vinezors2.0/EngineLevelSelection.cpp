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
    enter();
}

EngineLevelSelection::~EngineLevelSelection()
{
}

void EngineLevelSelection::enter()
{
    alloc();
    player->startMenu();
    
    // Set skybox
    Plane plane;
    plane.d = 80;
    plane.normal = Ogre::Vector3(0, 0, 1);
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, "General/SpaceSkyPlane", 1, 1, true);
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(0.5, 0.0, 0.5), 0.0, 300.0, 600.0);
    OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
}

void EngineLevelSelection::exit()
{
    dealloc();
}

void EngineLevelSelection::update(float elapsed)
{
}

void EngineLevelSelection::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI == "levelA1")
    {
        StageRequest level;
        level.nback = 0;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 0));
        level.navLevels.push_back(NavigationLevel(0, 3, 0));
        level.phase = 'E';
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA2")
    {
        StageRequest level;
        level.nback = 1;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 0));
        level.navLevels.push_back(NavigationLevel(0, 3, 0));
        level.phase = 'D';
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA3")
    {
        StageRequest level;
        level.nback = 1;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 1));
        level.navLevels.push_back(NavigationLevel(0, 0, 1));
        level.phase = 'A';
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA4")
    {
        StageRequest level;
        level.nback = 1;
        level.navLevels.push_back(NavigationLevel(0, 2, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 1));
        level.navLevels.push_back(NavigationLevel(0, 3, 1));
        level.navLevels.push_back(NavigationLevel(0, 3, 2));
        level.phase = 'A';
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA5")
    {
        StageRequest level;
        level.nback = 2;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 0, 1));
        level.navLevels.push_back(NavigationLevel(0, 0, 2));
        level.phase = 'B';
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA6")
    {
        StageRequest level;
        level.nback = 2;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 1));
        level.navLevels.push_back(NavigationLevel(0, 2, 1));
        level.navLevels.push_back(NavigationLevel(0, 3, 2));
        level.phase = 'C';
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA7")
    {
        StageRequest level;
        level.nback = 3;
        level.navLevels.push_back(NavigationLevel(0, 0, 2));
        level.navLevels.push_back(NavigationLevel(0, 1, 3));
        level.navLevels.push_back(NavigationLevel(0, 2, 4));
        level.navLevels.push_back(NavigationLevel(0, 3, 5));
        level.phase = 'D';
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "back")
    {
        engineStateMgr->requestPopEngine();
    }
}

#if !defined(OGRE_IS_IOS)
void EngineLevelSelection::mouseMoved(const OIS::MouseEvent &evt)
{
    
    return true;
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
    return true;
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
    return true;
}

void EngineLevelSelection::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void EngineLevelSelection::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineLevelSelection::requestResize()
{
    if (hud) hud->adjust();
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
