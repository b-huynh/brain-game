//
//  EngineInformation.cpp
//  VinezorsiOS
//
//  Created by BGC_Bernie on 3/18/15.
//
//

#include "EngineInformation.h"
#include "EngineStateManager.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineInformation::EngineInformation(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
}

EngineInformation::~EngineInformation()
{
}

void EngineInformation::enter()
{
    alloc();
    player->startMenu();
    
    // Set skybox
    OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
    OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
}

void EngineInformation::exit()
{
    dealloc();
}

void EngineInformation::update(float elapsed)
{
    
    hud->update(elapsed);

}


void EngineInformation::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI != "")
        player->reactGUI();
    if (queryGUI == "back")
    {
        
        player->saveProgress(globals.savePath);
        engineStateMgr->requestPopEngine();
    }
    else if (queryGUI == "bgctoplink")
    {
        
        std::cout << "Link Pressed!" << std::endl;
        OgreFramework::getSingletonPtr()->requestOpenURL("http://braingamecenter.ucr.edu");
    }
    else if (queryGUI == "nbacklink")
    {
        
        std::cout << "N Back Pressed!" << std::endl;
        OgreFramework::getSingletonPtr()->requestOpenURL("http://en.wikipedia.org/wiki/N-back");

    }
    else if (queryGUI == "bgcrecalllink")
    {
        
        std::cout << "Recall Link Pressed!" << std::endl;
        OgreFramework::getSingletonPtr()->requestOpenURL("http://braingamecenter.ucr.edu/games/recall");

    }
    
}

// The following deal with injecting coordinates to simulate a slider
//
void EngineInformation::activateMoved(float x, float y, float dx, float dy)
{
    
     //Testing Dragging!
     
     
     std::cout<<"(" << x <<", " << y << ")" << " Dx: " << dx << " Dy: " << dy <<std::endl;
     Ogre::Vector2 temp2 = globals.convertToPercentScreen(Vector2(x,y));
     
     std::cout <<"In percents: " << "(" << temp2.x <<", " << temp2.y << ")" << std::endl;
     
     //Move everything depending on the dy!
     
     //np = p2cache + globals.convertToPercentScreen(Vector2(0.0, dy));
     Ogre::Vector2 temp = globals.convertToPercentScreen(Vector2(0.0,dy));
     //std::cout << "Temp: "<<"(" << temp.x <<", " << temp.y << ")";
     //std::cout << "Top: "<<hud->paragraphTextBackground->getTop() <<std::endl;
     hud->paragraphTextBackground->setPosition(0, temp.y);
    
}

void EngineInformation::activatePressed(float x, float y)
{

}

void EngineInformation::activateReleased(float x, float y, float dx, float dy)
{
}

#if !defined(OGRE_IS_IOS)
void EngineInformation::mouseMoved(const OIS::MouseEvent &evt)
{
}

void EngineInformation::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void EngineInformation::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            break;
        default:
            break;
    }
}

void EngineInformation::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void EngineInformation::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineInformation::requestResize()
{
    if (hud) hud->adjust();
}

void EngineInformation::alloc()
{
    hud = new HudInformation(player);
}

void EngineInformation::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}
