//
//  EngineLevelSelection.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/16/14.
//
//

#ifndef __Vinezors2_0__EngineLevelSelection__
#define __Vinezors2_0__EngineLevelSelection__

#include "Util.h"

#include "Engine.h"
#include "LevelSet.h"

class EngineLevelSelection : public Engine
{
public:
    EngineLevelSelection(EngineStateManager* engineStateMgr, Player* player);
    virtual ~EngineLevelSelection();
    
    virtual void enter();
    virtual void exit();
    virtual void update(float elapsed);
    virtual void getEngineType() const {
        return ENGINE_LEVEL_SELECTION;
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
    
    bool testForLevelButtons(const std::string & queryGUI);
    void alloc();
    void dealloc();
};

#endif /* defined(__Vinezors2_0__EngineLevelSelection__) */
