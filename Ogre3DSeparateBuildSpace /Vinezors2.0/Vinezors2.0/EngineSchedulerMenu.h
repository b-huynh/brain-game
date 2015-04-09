//
//  EngineSchedulerMenu.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 9/15/14.
//
//

#ifndef __Vinezors2_0__EngineSchedulerMenu__
#define __Vinezors2_0__EngineSchedulerMenu__


#include "Engine.h"
#include "HudSchedulerMenu.h"
#include "Player.h"

class EngineSchedulerMenu : public Engine
{
public:
    EngineSchedulerMenu(EngineStateManager* engineStateMgr, Player* player);
    virtual ~EngineSchedulerMenu();
    
    virtual void enter();
    virtual void exit();
    virtual void update(float elapsed);
    virtual EngineState getEngineType() const {
        return ENGINE_SCHEDULER_MENU;
    }
    
    virtual void activatePerformSingleTap(float x, float y);
    
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
	Player* player;
    HudSchedulerMenu* hud;
    
    void alloc();
    void dealloc();
    
    bool testForLevelButtons(const std::string & queryGUI);
};

#endif /* defined(__Vinezors2_0__EngineSchedulerMenu__) */
