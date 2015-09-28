//
//  EngineStudySettings.h
//  Vinezors2.0
//
//  Created by BrainGameCenter on 3/3/15.
//
//

#ifndef __Vinezors2_0__EngineStudySettings__
#define __Vinezors2_0__EngineStudySettings__

#include "Util.h"

#include "Engine.h"
#include "HudStudySettings.h"


class EngineStudySettings : public Engine
{
public:
    EngineStudySettings(EngineStateManager* engineStateMgr, Player* player);
    
    virtual ~EngineStudySettings();
    
    
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
    HudStudySettings* hud;
    
    //TempStudySettings
    bool tempfuelEnabled ;
    bool tempholdoutEnabled ;
    int tempinitialVelocity ;
    bool tempmanRecessEnabled;
    int tempmanRecessLevelLimit;
    bool tempnewNavEnabled;
    float tempnewNavIncrement ;
    bool tempindRecessEnabled ;
    float tempindRecessIncrement ;
    bool tempholdoutdelayEnabled ;
    float tempholdoutdelayNumber ;
    bool tempenableSettingsPasscode;
    int tempsessionStartTime ;
    int tempsessionEndTime ;
    int tempnumOfSessions ;
    bool tempnewSounds;
    bool tempsessionScreenEnabled;
    bool tempenableIndRecessFixed;
    double tempholdoutoffsetA;
    double tempholdoutoffsetB;
    double tempholdoutoffsetD;
    
    float tempholdoutMinUpperBound;
    float tempholdoutMaxUpperBound;
    float tempholdoutLowerBoundTime;
    float tempholdoutUpperBoundMinTime;
    float tempholdoutUpperBoundMaxTime;
    int tempholdoutSteps;
    
    bool tempOverallTimerEnabled;
    bool tempaccelEnabled;

    
    void alloc();
    void dealloc();
};


#endif /* defined(__Vinezors2_0__EngineStudySettings__) */