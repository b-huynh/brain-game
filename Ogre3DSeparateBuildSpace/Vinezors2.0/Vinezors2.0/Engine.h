//
//  Engine.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#ifndef __Vinezors2_0__Engine__
#define __Vinezors2_0__Engine__

#include <OgreFramework.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

enum EngineState { ENGINE_NONE, ENGINE_STAGE, ENGINE_LEVEL_SELECTION, ENGINE_MAIN_MENU, ENGINE_CREDITS };

// Abstract Game Engine class (also known as the GameState in the stack)
class EngineStateManager; // forward decl
class Engine
{
public:
    Engine(EngineStateManager* engineStateMgr);
    virtual ~Engine();
    
    // Required functions to extend from
    //
    // enters through constructor or reappearing at top of stack
    // exits through being covered in stack or being removed from stack
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void update(float elapsed) = 0;
    virtual EngineState getEngineType() const = 0;
    
    // Optional functions to extend from
    // The functions below are interrupt handlers
    virtual void activatePerformLeftMove();
    virtual void activatePerformLeftMove(int angle);
    virtual void activatePerformRightMove();
    virtual void activatePerformRightMove(int angle);
    virtual void activatePerformSwipeUp();
    virtual void activatePerformSwipeDown();
    virtual void activatePerformDoubleTap(float x, float y);
    virtual void activatePerformSingleTap(float x, float y);
    virtual void activatePerformPinch();
    virtual void activatePerformBeginLongPress();
    virtual void activatePerformEndLongPress();
    virtual void activatePerformBeginShortPress();
    virtual void activatePerformEndShortPress();
    
    virtual void activateMoved(float x, float y, float dx, float dy);
	virtual void activatePressed(float x, float y);
	virtual void activateReleased(float x, float y, float dx, float dy);
    virtual void activateVelocity(float vel);
    
    virtual void activateReturnFromPopup();
    
    
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
    virtual void mouseMoved(const OIS::MouseEvent &evt);
	virtual void mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    
    virtual void keyPressed(const OIS::KeyEvent &keyEventRef);
	virtual void keyReleased(const OIS::KeyEvent &keyEventRef);
#endif
    
    virtual void requestResize();
protected:
    EngineStateManager* engineStateMgr;
};

#endif /* defined(__Vinezors2_0__Engine__) */
