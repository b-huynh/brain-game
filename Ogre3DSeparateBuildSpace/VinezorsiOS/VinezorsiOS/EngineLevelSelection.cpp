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
    levelSelect = -1;
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
    
    if (levelSelect >= 0 && levelSelect < player->levelCompletion.size())
    {
        if (player->win)
        {
            player->levelCompletion[levelSelect] = 1;
        }
    }
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
        levelSelect = 0;
        StageRequest level;
        level.nback = 3;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 0));
        level.navLevels.push_back(NavigationLevel(0, 3, 0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.phase = 'E';
        level.hasHoldout = true;
        globals.initCamSpeed = 15;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        player->setToggleBack(3);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA2")
    {
        levelSelect = 1;
        StageRequest level;
        level.nback = 3;
        level.navLevels.push_back(NavigationLevel(0, 2, 0));
        level.navLevels.push_back(NavigationLevel(0, 3, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.phase = 'D';
        level.hasHoldout = true;
        globals.initCamSpeed = 16;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        player->setToggleBack(3);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA3")
    {
        levelSelect = 2;
        StageRequest level;
        level.nback = 3;
        level.navLevels.push_back(NavigationLevel(0, 0, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 0, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 1));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.phase = 'D';
        level.hasHoldout = true;
        globals.initCamSpeed = 17;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_SHIELDS);
        player->setToggleBack(2);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA4")
    {
        levelSelect = 3;
        StageRequest level;
        level.nback = 3;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 0, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.phase = 'D';
        level.hasHoldout = true;
        globals.initCamSpeed = 16;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_SHIELDS);
        player->setToggleBack(1);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA5")
    {
        levelSelect = 4;
        StageRequest level;
        level.nback = 3;
        level.navLevels.push_back(NavigationLevel(0, 2, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 1));
        level.navLevels.push_back(NavigationLevel(0, 3, 0));
        level.navLevels.push_back(NavigationLevel(0, 3, 1));
        level.collectionCriteria.push_back(CollectionCriteria(0));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.phase = 'D';
        level.hasHoldout = true;
        globals.initCamSpeed = 17;
        globals.minCamSpeed = 16;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_TRACTOR_BEAM);
        player->setToggleBack(3);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA6")
    {
        levelSelect = 5;
        StageRequest level;
        level.nback = 3;
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.phase = 'D';
        level.hasHoldout = true;
        globals.initCamSpeed = 16;
        globals.minCamSpeed = 16;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_TRACTOR_BEAM);
        level.powerups.push_back(POWERUP_SHIELDS);
        player->setToggleBack(1);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelA7")
    {
        levelSelect = 6;
        StageRequest level;
        level.nback = 3;
        level.navLevels.push_back(NavigationLevel(0, 0, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 1));
        level.navLevels.push_back(NavigationLevel(0, 2, 2));
        level.navLevels.push_back(NavigationLevel(0, 3, 2));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(1));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.phase = 'D';
        level.hasHoldout = true;
        
        globals.initCamSpeed = 16;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_TIME_WARP);
        player->setToggleBack(2);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelB1")
    {
        levelSelect = 7;
        StageRequest level;
        level.nback = 3;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 0));
        level.navLevels.push_back(NavigationLevel(0, 3, 0));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.phase = 'E';
        globals.initCamSpeed = 15;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_TRACTOR_BEAM);
        level.powerups.push_back(POWERUP_TIME_WARP);
        level.powerups.push_back(POWERUP_SHIELDS);
        player->setToggleBack(3);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelB2")
    {
        levelSelect = 8;
        StageRequest level;
        level.nback = 4;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 0));
        level.navLevels.push_back(NavigationLevel(0, 3, 0));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.phase = 'D';
        globals.initCamSpeed = 16;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        player->setToggleBack(3);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelB3")
    {
        levelSelect = 9;
        StageRequest level;
        level.nback = 4;
        level.navLevels.push_back(NavigationLevel(0, 0, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 1));
        level.navLevels.push_back(NavigationLevel(0, 0, 2));
        level.navLevels.push_back(NavigationLevel(0, 1, 2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(2));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(3));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.phase = 'A';
        globals.initCamSpeed = 17;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_TRACTOR_BEAM);
        level.powerups.push_back(POWERUP_TIME_WARP);
        level.powerups.push_back(POWERUP_SHIELDS);
        player->setToggleBack(2);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelB4")
    {
        levelSelect = 10;
        StageRequest level;
        level.nback = 5;
        level.navLevels.push_back(NavigationLevel(0, 0, 0));
        level.navLevels.push_back(NavigationLevel(0, 0, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.collectionCriteria.push_back(CollectionCriteria(4));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.phase = 'A';
        globals.initCamSpeed = 16;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_TRACTOR_BEAM);
        level.powerups.push_back(POWERUP_TIME_WARP);
        level.powerups.push_back(POWERUP_SHIELDS);
        player->setToggleBack(1);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelB5")
    {
        levelSelect = 11;
        StageRequest level;
        level.nback = 5;
        level.navLevels.push_back(NavigationLevel(0, 2, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 1));
        level.navLevels.push_back(NavigationLevel(0, 3, 0));
        level.navLevels.push_back(NavigationLevel(0, 3, 1));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.phase = 'B';
        globals.initCamSpeed = 17;
        globals.minCamSpeed = 16;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_TRACTOR_BEAM);
        level.powerups.push_back(POWERUP_TIME_WARP);
        level.powerups.push_back(POWERUP_SHIELDS);
        player->setToggleBack(3);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelB6")
    {
        levelSelect = 12;
        StageRequest level;
        level.nback = 6;
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 0));
        level.navLevels.push_back(NavigationLevel(0, 2, 2));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.collectionCriteria.push_back(CollectionCriteria(5));
        level.collectionCriteria.push_back(CollectionCriteria(6));
        level.phase = 'C';
        globals.initCamSpeed = 16;
        globals.minCamSpeed = 16;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_TRACTOR_BEAM);
        level.powerups.push_back(POWERUP_TIME_WARP);
        level.powerups.push_back(POWERUP_SHIELDS);
        player->setToggleBack(1);
        player->setStageRequest(level);
        engineStateMgr->requestPushEngine(ENGINE_STAGE, player);
    }
    else if (queryGUI == "levelB7")
    {
        levelSelect = 13;
        StageRequest level;
        level.nback = 6;
        level.navLevels.push_back(NavigationLevel(0, 0, 1));
        level.navLevels.push_back(NavigationLevel(0, 1, 1));
        level.navLevels.push_back(NavigationLevel(0, 2, 2));
        level.navLevels.push_back(NavigationLevel(0, 3, 2));
        level.collectionCriteria.push_back(CollectionCriteria(6));
        level.collectionCriteria.push_back(CollectionCriteria(6));
        level.collectionCriteria.push_back(CollectionCriteria(6));
        level.collectionCriteria.push_back(CollectionCriteria(6));
        level.collectionCriteria.push_back(CollectionCriteria(6));
        level.collectionCriteria.push_back(CollectionCriteria(6));
        level.collectionCriteria.push_back(CollectionCriteria(6));
        level.collectionCriteria.push_back(CollectionCriteria(6));
        level.phase = 'D';
        level.hasHoldout = true;
        
        globals.initCamSpeed = 16;
        globals.minCamSpeed = 15;
        globals.maxCamSpeed = 40;
        level.powerups.push_back(POWERUP_TRACTOR_BEAM);
        level.powerups.push_back(POWERUP_TIME_WARP);
        level.powerups.push_back(POWERUP_SHIELDS);
        player->setToggleBack(2);
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
