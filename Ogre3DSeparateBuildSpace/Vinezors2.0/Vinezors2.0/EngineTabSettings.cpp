//
//  EngineTabSettings.cpp
//  VinezorsiOS
//
//  Created by BGC_Bernie on 4/28/15.
//
//

#include "EngineTabSettings.h"
#include "EngineStateManager.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineTabSettings::EngineTabSettings(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
}

EngineTabSettings::~EngineTabSettings()
{
}

void EngineTabSettings::enter()
{
    alloc();
    player->startMenu();
    

    
    // Set skybox
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlaneEnabled(false);
    OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.11f, 0.11f, 0.11f, 1.0f));    
    OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
    OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
}

void EngineTabSettings::exit()
{
    //OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyBoxEnabled(true);
    //OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlaneEnabled(true);

    dealloc();
}

void EngineTabSettings::update(float elapsed)
{
    
    hud->update(elapsed);
    
}


void EngineTabSettings::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI != "")
        player->reactGUI();
    if (queryGUI == "back")
    {
        
        player->saveProgress(globals.savePath);
        engineStateMgr->requestPopEngine();
    }
    else if(queryGUI=="genoption")
    {
        std::cout<< "Pressed Gen" << std::endl;
    }
    else if(queryGUI == "checkfuel")
    {
        if(hud->enableUnlimitedFuelButtonBackground->getMaterialName() == "General/TabSettingsSwitchOn")
        {
            hud->enableUnlimitedFuelButtonBackground->setMaterialName("General/TabSettingsSwitchOff");

        }
        else
        {
            hud->enableUnlimitedFuelButtonBackground->setMaterialName("General/TabSettingsSwitchOn");
        }

    }

}

// The following deal with injecting coordinates to simulate a slider
//
void EngineTabSettings::activateMoved(float x, float y, float dx, float dy)
{
    Ogre::Vector2 temp = globals.convertToPercentScreen(Vector2(0.0,dy));

    std::cout << "Temp: "<<"(" << temp.x <<", " << temp.y << ")" << std::endl;

    //Testing Dragging!
   /*
    if(touchedBox)
    {
        Ogre::Vector2 temp2 = globals.convertToPercentScreen(Vector2(x,y));
        
    
        //Move everything depending on the dy!
        Ogre::Vector2 temp = globals.convertToPercentScreen(Vector2(0.0,dy));
        
        
        //float newy = initTextPosition
        float constant = 1;
        temp.y /= constant;
        hud->enableUnlimitedFuelBackground->setPosition(.32, hud->initTextPosition.y+temp.y);
    }
    */
}

void EngineTabSettings::activatePressed(float x, float y)
{

    
   /* Vector2 check = Vector2(hud->enableUnlimitedFuelBackground->_getDerivedLeft(),
                            hud->enableUnlimitedFuelBackground->_getDerivedTop());
    
    Vector2 target = globals.convertToPercentScreen(Vector2(x,y));
    
    //std::cout <<"(" << target.x <<", " << target.y << ")";

    Vector2 dim = Vector2(.7,1);

    if(target.x >= check.x && target.x <= check.x + dim.x &&
       target.y >= check.y && target.y <= check.y + dim.y)
    {
        touchedBox =true;
    }

*/
    
}

void EngineTabSettings::activateReleased(float x, float y, float dx, float dy)
{

//    touchedBox = false;
}

#if !defined(OGRE_IS_IOS)
void EngineTabSettings::mouseMoved(const OIS::MouseEvent &evt)
{
}

void EngineTabSettings::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void EngineTabSettings::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            break;
        default:
            break;
    }
}

void EngineTabSettings::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void EngineTabSettings::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineTabSettings::requestResize()
{
    if (hud) hud->adjust();
}

void EngineTabSettings::alloc()
{
    hud = new HudTabSettings(player);
}

void EngineTabSettings::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}
