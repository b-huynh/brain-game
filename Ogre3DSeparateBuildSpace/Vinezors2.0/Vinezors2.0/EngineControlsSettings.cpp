//
//  EngineControlSettings.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 5/2/14.
//
//

#include "EngineControlSettings.h"
#include "EngineStateManager.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineControlSettings::EngineControlSettings(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
}

EngineControlSettings::~EngineControlSettings()
{
}

void EngineControlSettings::enter()
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

void EngineControlSettings::exit()
{
    dealloc();
}

void EngineControlSettings::update(float elapsed)
{
    hud->update(elapsed);
    
    player->maxVel = hud->maxVelSlider->getIndex();
    player->minVelStopper = hud->minVelStopperSlider->getIndex();
    player->dampingDecayFree = (float)hud->dampingDecayFreeSlider->getIndex() / (hud->dampingDecayFreeSlider->getRange() - 1);
    player->dampingDecayStop = (float)hud->dampingDecayStopSlider->getIndex() / (hud->dampingDecayStopSlider->getRange() - 1);
    player->dampingDropFree = (float)hud->dampingDropFreeSlider->getIndex();
    player->dampingDropStop = (float)hud->dampingDropStopSlider->getIndex();
}

void EngineControlSettings::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI != "")
        player->reactGUI();
    if (queryGUI == "back")
    {
        player->saveProgress(globals.savePath);
        engineStateMgr->requestPopEngine();
    }
    else if (queryGUI == "defaults")
    {
        hud->resetDefaults();
    }
    else if (queryGUI == "inverted")
    {
        player->inverted = !player->inverted;
    }
}

// The following deal with injecting coordinates to simulate a slider
//
void EngineControlSettings::activateMoved(float x, float y, float dx, float dy)
{
    if (hud)
    {
        if (hud->maxVelSlider && hud->maxVelSlider->selected)
        {
            hud->maxVelSlider->activateMoved(x, y, dx, dy);
        }
        else if (hud->minVelStopperSlider && hud->minVelStopperSlider->selected)
        {
            hud->minVelStopperSlider->activateMoved(x, y, dx, dy);
        }
        else if (hud->dampingDecayFreeSlider && hud->dampingDecayFreeSlider->selected)
        {
            hud->dampingDecayFreeSlider->activateMoved(x, y, dx, dy);
        }
        else if (hud->dampingDecayStopSlider && hud->dampingDecayStopSlider->selected)
        {
            hud->dampingDecayStopSlider->activateMoved(x, y, dx, dy);
        }
        else if (hud->dampingDropFreeSlider && hud->dampingDropFreeSlider->selected)
        {
            hud->dampingDropFreeSlider->activateMoved(x, y, dx, dy);
        }
        else if (hud->dampingDropStopSlider && hud->dampingDropStopSlider->selected)
        {
            hud->dampingDropStopSlider->activateMoved(x, y, dx, dy);
        }
    }
}

void EngineControlSettings::activatePressed(float x, float y)
{
    if (hud)
    {
        if (hud->maxVelSlider)
        {
            hud->maxVelSlider->activatePressed(x, y);
        }
        if (hud->minVelStopperSlider)
        {
            hud->minVelStopperSlider->activatePressed(x, y);
        }
        if (hud->dampingDecayFreeSlider)
        {
            hud->dampingDecayFreeSlider->activatePressed(x, y);
        }
        if (hud->dampingDecayStopSlider)
        {
            hud->dampingDecayStopSlider->activatePressed(x, y);
        }
        if (hud->dampingDropFreeSlider)
        {
            hud->dampingDropFreeSlider->activatePressed(x, y);
        }
        if (hud->dampingDropStopSlider)
        {
            hud->dampingDropStopSlider->activatePressed(x, y);
        }
    }
}

void EngineControlSettings::activateReleased(float x, float y, float dx, float dy)
{
    if (hud)
    {
        if (hud->maxVelSlider && hud->maxVelSlider->selected)
        {
            hud->maxVelSlider->activateReleased(x, y, dx, dy);
        }
        else if (hud->minVelStopperSlider && hud->minVelStopperSlider->selected)
        {
            hud->minVelStopperSlider->activateReleased(x, y, dx, dy);
        }
        else if (hud->dampingDecayFreeSlider && hud->dampingDecayFreeSlider->selected)
        {
            hud->dampingDecayFreeSlider->activateReleased(x, y, dx, dy);
        }
        else if (hud->dampingDecayStopSlider && hud->dampingDecayStopSlider->selected)
        {
            hud->dampingDecayStopSlider->activateReleased(x, y, dx, dy);
        }
        else if (hud->dampingDropFreeSlider && hud->dampingDropFreeSlider->selected)
        {
            hud->dampingDropFreeSlider->activateReleased(x, y, dx, dy);
        }
        else if (hud->dampingDropStopSlider && hud->dampingDropStopSlider->selected)
        {
            hud->dampingDropStopSlider->activateReleased(x, y, dx, dy);
        }
    }
}

#if !defined(OGRE_IS_IOS)
void EngineControlSettings::mouseMoved(const OIS::MouseEvent &evt)
{
}

void EngineControlSettings::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void EngineControlSettings::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            break;
        default:
            break;
    }
}

void EngineControlSettings::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void EngineControlSettings::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineControlSettings::requestResize()
{
    if (hud) hud->adjust();
}

void EngineControlSettings::alloc()
{
    hud = new HudControlSettings(player);
}

void EngineControlSettings::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}
