//
//  EngineCredits.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#include "EngineCredits.h"
#include "EngineStateManager.h"
#include "HudCredits.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineCredits::EngineCredits(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
    enter();
}

EngineCredits::~EngineCredits()
{
}

void EngineCredits::enter()
{
    alloc();
    player->startMenu();
    
    // Set skybox
    Util::setSkyboxAndFog("General/PurpleSpaceSkyPlane");
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
}

void EngineCredits::exit()
{
    dealloc();
}

void EngineCredits::update(float elapsed)
{
}

void EngineCredits::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI == "back")
    {
        engineStateMgr->requestPopEngine();
    }
}

#if !defined(OGRE_IS_IOS)
void EngineCredits::mouseMoved(const OIS::MouseEvent &evt)
{
}

void EngineCredits::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void EngineCredits::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            break;
        default:
            break;
    }
}

void EngineCredits::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void EngineCredits::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineCredits::requestResize()
{
    if (hud) hud->adjust();
}

void EngineCredits::alloc()
{
    hud = new HudCredits(player);
}

void EngineCredits::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}
