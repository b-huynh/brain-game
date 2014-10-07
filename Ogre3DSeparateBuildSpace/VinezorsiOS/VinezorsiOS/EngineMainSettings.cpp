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
    enter();
}

EngineMainSettings::~EngineMainSettings()
{
}

void EngineMainSettings::enter()
{
    alloc();
    player->startMenu();
    
    // Set skybox
    Util::setSkyboxAndFog("");
	OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
	OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
}

void EngineMainSettings::exit()
{
#ifdef DEBUG_MODE
    // If in debug mode, modify all levels to have the player holdout frequency.
    player->getLevels()->holdoutFreqvar=player->holdout;
    std::cout<<"Holdout: "<<1/player->holdout<<"   LowerBound: "<<player->holdoutLB <<"   UpperBound: "<<player->holdoutUB<<std::endl;
#endif
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
    player->setVolume();
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
        TutorialManager* tutorialMgr = player->getTutorialMgr();
        if (tutorialMgr->isEnabled())
            tutorialMgr->disable();
        else
            tutorialMgr->enable();
    }
    else if (queryGUI == "checksyncdata")
    {
        player->syncDataToServer = !player->syncDataToServer;
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
