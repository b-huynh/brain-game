//
//  EngineMainSettings.h
//  VinezorsiOS
//
//  Created by Calvin Phung on 6/26/14.
//
//

#ifndef __VinezorsiOS__EngineMainSettings__
#define __VinezorsiOS__EngineMainSettings__

#include "Util.h"

#include "Engine.h"
#include "HudMainSettings.h"

class EngineMainSettings : public Engine
{
public:
    EngineMainSettings(EngineStateManager* engineStateMgr, Player* player);
    virtual ~EngineMainSettings();
    
    virtual void enter();
    virtual void exit();
    virtual void update(float elapsed);
    virtual EngineState getEngineType() const {
        return ENGINE_MAIN_SETTINGS;
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
    HudMainSettings* hud;
    
    void alloc();
    void dealloc();
};

#endif /* defined(__VinezorsiOS__EngineMainSettings__) */
