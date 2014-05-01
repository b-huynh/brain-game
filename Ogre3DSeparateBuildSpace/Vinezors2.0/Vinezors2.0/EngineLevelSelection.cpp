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
    Util::setSkyboxAndFog("General/PurpleSpaceSkyPlane");
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
}

void EngineLevelSelection::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    
    if (testForLevelButtons(queryGUI))
    {
        
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

bool EngineLevelSelection::testForLevelButtons(const std::string & queryGUI)
{
    for (char c = 'A'; c <= 'C'; ++c)
        for (int i = 1; i <= 8; ++i)
        {
            std::string pre = "level";
            std::string queryTest = pre + c + Util::toStringInt(i);
            if (queryGUI == queryTest)
            {
                LevelSet* levels = player->getLevels();
                int levelSelect = levels->getLevelNo(c, i);
                if  (levels->hasLevel(levelSelect))
                {
                    player->setLevelRequest(levelSelect);
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
