//
//  EngineStage.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/16/14.
//
//

#ifndef __Vinezors2_0__EngineStage__
#define __Vinezors2_0__EngineStage__

#include "Util.h"

#include "Engine.h"
#include "HudStage.h"

// Note: This Engine is not yet suited to be any GameState object underneath another in the stack
class EngineStage : public Engine
{
public:
    EngineStage(EngineStateManager* engineStateMgr, Player* player);
    virtual ~EngineStage();
    
    virtual void enter();
    virtual void exit();
    virtual void update(float elapsed);
    virtual EngineState getEngineType() const {
        return ENGINE_STAGE;
    }
    
    virtual void activatePerformLeftMove();
    virtual void activatePerformLeftMove(float angle);
    virtual void activatePerformRightMove();
    virtual void activatePerformRightMove(float angle);
    virtual void activatePerformSwipeUp();
    virtual void activatePerformSwipeDown();
    virtual void activatePerformDoubleTap(float x, float y);
    virtual void activatePerformSingleTap(float x, float y);
    virtual void activatePerformPinch();
    virtual void activatePerformBeginLongPress();
    virtual void activatePerformEndLongPress();
    
    virtual void activateMoved(float x, float y, float dx, float dy);
	virtual void activatePressed(float x, float y);
	virtual void activateReleased(float x, float y, float dx, float dy);
    virtual void activateVelocity(float vel);
    virtual void activateAngleTurn(float angle);
    
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
    // State machine of stage
    //
    // INIT: sets up initial tunnel and goes to begin prompt
    // PAUSE: begin prompt (adjust speed, tutorial, ect.)
    // RUNNING: actual gameplay
    // PROMPT: end prompt and manual pause prompt
    // DONE: cleanup of game state
    enum StageState { STAGE_STATE_INIT, STAGE_STATE_PAUSE, STAGE_STATE_RUNNING, STAGE_STATE_PROMPT, STAGE_STATE_DONE };
    
    StageState stageState;
    
    Light* lightMain;
    SceneNode* lightNode;
    Tunnel* tunnel;
	Player* player;
    HudStage* hud;
    
    //State of tunnel spin
    float spinVelocity;
    float damping;
    bool spinClockwise;
    std::vector<float> lastAngles;
    
    void setup();
    void dealloc();
    void setPause(bool value, bool targetAllSounds = true);
    void completeStage(Evaluation forced);
    void updateSpin(float elapsed);
};

#endif /* defined(__Vinezors2_0__EngineStage__) */
