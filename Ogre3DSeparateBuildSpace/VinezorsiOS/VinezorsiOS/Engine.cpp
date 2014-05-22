//
//  Engine.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#include "Engine.h"

Engine::Engine(EngineStateManager* engineStateMgr)
: engineStateMgr(engineStateMgr)
{
    
}

Engine::~Engine()
{
    
}

void Engine::activatePerformLeftMove()
{
    
}

void Engine::activatePerformLeftMove(float angle)
{
    
}

void Engine::activatePerformRightMove()
{
    
}

void Engine::activatePerformRightMove(float angle)
{
    
}

void Engine::activatePerformSwipeUp()
{
    
}

void Engine::activatePerformSwipeDown()
{
    
}

void Engine::activatePerformDoubleTap(float x, float y)
{
    
}

void Engine::activatePerformSingleTap(float x, float y)
{
    
}

void Engine::activatePerformPinch()
{
    
}

void Engine::activatePerformBeginLongPress()
{
    
}

void Engine::activatePerformEndLongPress()
{
    
}

void Engine::activatePerformBeginShortPress()
{
    
}

void Engine::activatePerformEndShortPress()
{
    
}

void Engine::activateMoved(float x, float y, float dx, float dy)
{
    
}

void Engine::activatePressed(float x, float y)
{
    
}

void Engine::activateReleased(float x, float y, float dx,  float dy)
{
    
}

void Engine::activateVelocity(float vel)
{
  
}

void Engine::activateAngleTurn(float angle)
{
    
}

void Engine::activateReturnFromPopup()
{
    
}

#if !defined(OGRE_IS_IOS)
void Engine::mouseMoved(const OIS::MouseEvent &evt)
{
}

void Engine::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
}

void Engine::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
}

void Engine::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void Engine::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void Engine::requestResize()
{
    
}
