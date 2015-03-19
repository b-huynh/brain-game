//
//  EngineMainMenu.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#include "EngineMainMenu.h"
#include "EngineStateManager.h"
//#include "HudMainMenu.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineMainMenu::EngineMainMenu(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
}

EngineMainMenu::~EngineMainMenu()
{
}

void EngineMainMenu::enter()
{
    alloc();
    player->startMenu();
    
    // Set skybox
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
}

void EngineMainMenu::exit()
{
    dealloc();
}

void EngineMainMenu::update(float elapsed)
{
    hud->update(elapsed);

    if(hud->enterStudySettings)
     {
         engineStateMgr->requestPushEngine(ENGINE_MAIN_SETTINGS, player);     
     }
}

void EngineMainMenu::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    
    if (queryGUI == "play")
    {
        player->reactGUI();
        engineStateMgr->requestPushEngine(ENGINE_SCHEDULER_MENU, player);
        
        if (!player->sessionStarted) // Should always be false when game first starts up
        {
            player->startSession();
        
            player->sessionStarted = true;
        }
    }
    else if (queryGUI == "credits")
    {
       /* player->reactGUI();
        player->levelRequest = NULL;  // Set to NULL so it won't force jump to scheduler levels
        player->marbleChoice = -1;
        engineStateMgr->requestPushEngine(ENGINE_LEVEL_SELECTION, player);*/
        
        OgreFramework::getSingletonPtr()->requestOpenURL("http://en.wikipedia.org/wiki/N-back");
    }
    else if (queryGUI == "settings")
    {
        player->reactGUI();
        if(player->enableSettingsPasscode)
        {
            hud->enableNumpad = true;
        }
        else{
            engineStateMgr->requestPushEngine(ENGINE_MAIN_SETTINGS, player);

        }
        //engineStateMgr->requestPushEngine(ENGINE_MAIN_SETTINGS, player);
    }
    else if (queryGUI == "information")
    {
        std::cout << "Information Pressed" <<std::endl;
        player->reactGUI();
        engineStateMgr->requestPushEngine(ENGINE_INFORMATION, player);
    }
    else if(queryGUI == "numpadbuttoncancel")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();

        }
        //make numpad go away
        std::cout<<"cancel pressed";
        hud->enableNumpad = false;
        //hud->Passcode_counter = 0;
        
    }
    else if(queryGUI == "numpadbutton0")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 0\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 0;
            hud->Passcode_counter++;
        }
        
    }
    else if(queryGUI == "numpadbutton1")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 1\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 1;
            hud->Passcode_counter++;
        }
        
    }
    else if(queryGUI == "numpadbutton2")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 2\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 2;
            hud->Passcode_counter++;
        }
        
    }
    else if(queryGUI == "numpadbutton3")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 3\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 3;
            hud->Passcode_counter++;
        }
        
    }
    else if(queryGUI == "numpadbutton4")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 4\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 4;
            hud->Passcode_counter++;
        }
        
    }
    else if(queryGUI == "numpadbutton5")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 5\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 5;
            hud->Passcode_counter++;
        }
        
    }
    else if(queryGUI == "numpadbutton6")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 6\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 6;
            hud->Passcode_counter++;
        }
        
    }
    else if(queryGUI == "numpadbutton7")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 7\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 7;
            hud->Passcode_counter++;
        }
        
    }
    else if(queryGUI == "numpadbutton8")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 8\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 8;
            hud->Passcode_counter++;
        }
        
    }
    else if(queryGUI == "numpadbutton9")
    {
        if(hud->enableNumpad)
        {
            player->reactGUI();
            
        }
        std::cout<<"Pressed 9\n";
        if(hud->Passcode_counter < 4)
        {
            hud->user_password[hud->Passcode_counter] = 9;
            hud->Passcode_counter++;
        }
        
    }

}

#if !defined(OGRE_IS_IOS)
void EngineMainMenu::mouseMoved(const OIS::MouseEvent &evt)
{
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
