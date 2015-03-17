//
//  EngineMainSettings.cpp
//  VinezorsiOS
//
//  Created by Calvin Phung on 6/26/14.
//
//

#include "EngineMainSettings.h"
#include "EngineStateManager.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineMainSettings::EngineMainSettings(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
}

EngineMainSettings::~EngineMainSettings()
{
}

void EngineMainSettings::enter()
{
    alloc();
    player->startMenu();
    
    // Set skybox
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
}

void EngineMainSettings::exit()
{
    dealloc();
}

void EngineMainSettings::update(float elapsed)
{
  
    hud->update(elapsed);
    
    player->musicVolume = hud->musicVolumeSlider->getIndex() / 100.0f;
    player->soundVolume = hud->soundVolumeSlider->getIndex() / 100.0f;
    player->holdout = hud->holdoutSlider->getIndex() / 100.0f;
    player->holdoutLB = hud->holdoutLBSlider->getIndex() / 100.0f;
    player->holdoutUB = hud->holdoutUBSlider->getIndex() / 100.0f;
    player->updateMusicVolume();
 
    if(hud->enterStudySettings)
    {
        //std::cout<<"Called\n";
        engineStateMgr->requestPushEngine(ENGINE_STUDY_SETTINGS, player);
        
    }
}

void EngineMainSettings::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI != "")
        player->reactGUI();
    if (queryGUI == "back")
    {
        
        player->saveProgress(globals.savePath);
        engineStateMgr->requestPopEngine();
    }
    else if (queryGUI == "controlsettings")
    {
        engineStateMgr->requestPushEngine(ENGINE_CONTROL_SETTINGS, player);
    }
    else if (queryGUI == "checktutorials")
    {
        hud->enableNumpad = false;
        TutorialManager* tutorialMgr = player->getTutorialMgr();
        if (tutorialMgr->isEnabled())
            tutorialMgr->disable();
        else
            tutorialMgr->enable();
    }
    else if (queryGUI == "checksyncdata")
    {
        player->syncDataToServer = !player->syncDataToServer;
        hud->enableNumpad = false;
    }
    else if (queryGUI == "studysettings")
    {
       
        hud->enableNumpad = true;
        
        //Add the numpad!
        //Show numpad, If cancel button is pressed, remove numpad.
        
        //engineStateMgr->requestPushEngine(ENGINE_STUDY_SETTINGS, player);
    }
    else if(queryGUI == "numpadbuttoncancel")
    {
        //make numpad go away
        std::cout<<"cancel pressed";
        hud->enableNumpad = false;
        //hud->Passcode_counter = 0;

    }
    else if(queryGUI == "numpadbutton0")
    {
        std::cout<<"Pressed 0\n";
        hud->user_password[hud->Passcode_counter] = 0;
        hud->Passcode_counter++;
    }
    else if(queryGUI == "numpadbutton1")
    {
        std::cout<<"Pressed 1\n";
        hud->user_password[hud->Passcode_counter] = 1;
        hud->Passcode_counter++;

    }
    else if(queryGUI == "numpadbutton2")
    {
        std::cout<<"Pressed 2\n";
        hud->user_password[hud->Passcode_counter] = 2;

        hud->Passcode_counter++;

    }
    else if(queryGUI == "numpadbutton3")
    {
        std::cout<<"Pressed 3\n";
        hud->user_password[hud->Passcode_counter] = 3;

        hud->Passcode_counter++;

    }
    else if(queryGUI == "numpadbutton4")
    {
        std::cout<<"Pressed 4\n";
        hud->user_password[hud->Passcode_counter] = 4;

        hud->Passcode_counter++;

    }
    else if(queryGUI == "numpadbutton5")
    {
        std::cout<<"Pressed 5\n";
        hud->user_password[hud->Passcode_counter] = 5;

        hud->Passcode_counter++;

    }
    else if(queryGUI == "numpadbutton6")
    {
        std::cout<<"Pressed 6\n";
        hud->user_password[hud->Passcode_counter] = 6;

        hud->Passcode_counter++;

    }
    else if(queryGUI == "numpadbutton7")
    {
        std::cout<<"Pressed 7\n";
        hud->user_password[hud->Passcode_counter] = 7;

        hud->Passcode_counter++;

    }
    else if(queryGUI == "numpadbutton8")
    {
        std::cout<<"Pressed 8\n";
        hud->user_password[hud->Passcode_counter] = 8;

        hud->Passcode_counter++;

    }
    else if(queryGUI == "numpadbutton9")
    {
        std::cout<<"Pressed 9\n";
        hud->user_password[hud->Passcode_counter] = 9;

        hud->Passcode_counter++;

    }
}

// The following deal with injecting coordinates to simulate a slider
//
void EngineMainSettings::activateMoved(float x, float y, float dx, float dy)
{
    if (hud)
    {
        if (hud->musicVolumeSlider && hud->musicVolumeSlider->selected)
        {
            hud->musicVolumeSlider->activateMoved(x, y, dx, dy);
        }
        else if (hud->soundVolumeSlider && hud->soundVolumeSlider->selected)
        {
            hud->soundVolumeSlider->activateMoved(x, y, dx, dy);
        }
#ifdef DEBUG_MODE
        else if (hud->holdoutSlider && hud->holdoutSlider->selected)
        {
            hud->holdoutSlider->activateMoved(x, y, dx, dy);
        }
        else if (hud->holdoutLBSlider && hud->holdoutLBSlider->selected)
        {
            hud->holdoutLBSlider->activateMoved(x, y, dx, dy);
        }
        else if (hud->holdoutUBSlider && hud->holdoutUBSlider->selected)
        {
            hud->holdoutUBSlider->activateMoved(x, y, dx, dy);
        }
#endif
    }
}

void EngineMainSettings::activatePressed(float x, float y)
{
    if (hud)
    {
        if (hud->musicVolumeSlider)
        {
            hud->musicVolumeSlider->activatePressed(x, y);
        }
        if (hud->soundVolumeSlider)
        {
            hud->soundVolumeSlider->activatePressed(x, y);
        }
#ifdef DEBUG_MODE
        if (hud->holdoutSlider)
        {
            hud->holdoutSlider->activatePressed(x, y);
        }
        if (hud->holdoutLBSlider)
        {
            hud->holdoutLBSlider->activatePressed(x, y);
        }
        if (hud->holdoutUBSlider)
        {
            hud->holdoutUBSlider->activatePressed(x, y);
        }
#endif
    }
}

void EngineMainSettings::activateReleased(float x, float y, float dx, float dy)
{
    if (hud)
    {
        if (hud->musicVolumeSlider && hud->musicVolumeSlider->selected)
        {
            hud->musicVolumeSlider->activateReleased(x, y, dx, dy);
        }
        else if (hud->soundVolumeSlider && hud->soundVolumeSlider->selected)
        {
            hud->soundVolumeSlider->activateReleased(x, y, dx, dy);
        }
#ifdef DEBUG_MODE
        else if (hud->holdoutSlider && hud->holdoutSlider->selected)
        {
            hud->holdoutSlider->activateReleased(x, y, dx, dy);
        }
        else if (hud->holdoutLBSlider && hud->holdoutLBSlider->selected)
        {
            hud->holdoutLBSlider->activateReleased(x, y, dx, dy);
        }
        else if (hud->holdoutUBSlider && hud->holdoutUBSlider->selected)
        {
            hud->holdoutUBSlider->activateReleased(x, y, dx, dy);
        }
#endif
    }
}

#if !defined(OGRE_IS_IOS)
void EngineMainSettings::mouseMoved(const OIS::MouseEvent &evt)
{
}

void EngineMainSettings::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void EngineMainSettings::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            break;
        default:
            break;
    }
}

void EngineMainSettings::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void EngineMainSettings::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineMainSettings::requestResize()
{
    if (hud) hud->adjust();
}

void EngineMainSettings::alloc()
{
    hud = new HudMainSettings(player);
}

void EngineMainSettings::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}
