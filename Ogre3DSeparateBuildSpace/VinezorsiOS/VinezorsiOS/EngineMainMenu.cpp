//
//  EngineMainMenu.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#include "EngineMainMenu.h"
#include "EngineStateManager.h"
#include "HudMainMenu.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineMainMenu::EngineMainMenu(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
    enter();
}

EngineMainMenu::~EngineMainMenu()
{
}

void EngineMainMenu::enter()
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

void EngineMainMenu::exit()
{
    dealloc();
}

void EngineMainMenu::update(float elapsed)
{
}

void EngineMainMenu::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI == "play")
    {
        engineStateMgr->requestPushEngine(ENGINE_LEVEL_SELECTION, player);
    }
    else if (queryGUI == "credits")
    {
        engineStateMgr->requestPushEngine(ENGINE_CREDITS, player);
    }
    else if (queryGUI == "exit")
    {
        engineStateMgr->requestPopEngine();
    }
}

#if !defined(OGRE_IS_IOS)
void EngineMainMenu::mouseMoved(const OIS::MouseEvent &evt)
{
    
    return true;
}

void EngineMainMenu::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void EngineMainMenu::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void EngineMainMenu::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void EngineMainMenu::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineMainMenu::requestResize()
{
    if (hud) hud->adjust();
}

void EngineMainMenu::alloc()
{
    hud = new HudMainMenu(player);
}

void EngineMainMenu::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}
