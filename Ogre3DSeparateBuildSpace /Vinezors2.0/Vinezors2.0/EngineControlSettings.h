//
//  EngineControlSettings.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 5/2/14.
//
//

#ifndef __Vinezors2_0__EngineControlSettings__
#define __Vinezors2_0__EngineControlSettings__

#include "Util.h"

#include "Engine.h"
#include "HudControlSettings.h"

class EngineControlSettings : public Engine
{
public:
    EngineControlSettings(EngineStateManager* engineStateMgr, Player* player);
    virtual ~EngineControlSettings();
    
    virtual void enter();
    virtual void exit();
    virtual void update(float elapsed);
    virtual EngineState getEngineType() const {
        return ENGINE_CONTROL_SETTINGS;
    }
    
    virtual void activatePerformSingleTap(float x, float y);
    
    virtual void activateMoved(float x, float y, float dx, float dy);
	virtual void activatePressed(float x, float y);
	virtual void activateReleased(float x, float y, float dx, float dy);
    
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
    HudControlSettings* hud;
    
    void alloc();
    void dealloc();
};

#endif /* defined(__Vinezors2_0__EngineControlSettings__) */
