//
//  EngineCredits.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/18/14.
//
//

#ifndef __Vinezors2_0__EngineCredits__
#define __Vinezors2_0__EngineCredits__

#include "Util.h"

#include "Engine.h"

class EngineCredits : public Engine
{
public:
    EngineCredits(EngineStateManager* engineStateMgr, Player* player);
    virtual ~EngineCredits();
    
    virtual void enter();
    virtual void exit();
    virtual void update(float elapsed);
    virtual void getEngineType() const {
        return ENGINE_CREDITS;
    }
    
    virtual void activatePerformSingleTap(float x, float y);
    
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
    Hud* hud;
    
    void alloc();
    void dealloc();
};

#endif /* defined(__Vinezors2_0__EngineCredits__) */
