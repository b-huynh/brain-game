//
//  Powerup.h
//  Vinezors2.0
//
//  Created by Emmanuel Flores on 4/15/14.
//
//

#ifndef Vinezors2_0_Powerup_h
#define Vinezors2_0_Powerup_h

#include "Util.h"

struct Powerup {

    bool available;
    bool active;
    float mainTimer;
    float mainTimeout;
    
    Powerup(float timeout = 0.0f);
    virtual PowerupType getPowerupType() const = 0;
    virtual void reset() = 0;
    virtual ~Powerup() {}
};

struct TractorBeam : public Powerup {
  
    SceneNode* tractorBeamRotatorNode;
    SceneNode* tractorBeamNode;
    ParticleSystem* tractorBeamEffect;
    
    virtual PowerupType getPowerupType() const
    { return POWERUP_TRACTOR_BEAM; }
    
    virtual void reset();
    virtual ~TractorBeam();
    
    TractorBeam();
};

struct TimeWarp : public Powerup {

    Degree origFov;
    float origAngle;
    float currentAngle;
    float maxAngle;
    float angleIncrement;
    
    float slowdown;
    
    int zoomIn;
    
    float origOffset;
    float currentOffset;
    float offsetIncrement;
    float offsetLimit;
    
    float timeBonus;
    float timeBonusTimeout;
    
    int currentTimeVal;
    int timeVal;
    
    
    virtual PowerupType getPowerupType() const
    { return POWERUP_TIME_WARP; }
    
    virtual void reset();
    virtual ~TimeWarp();
    
    TimeWarp(float timeout = 0.0f, float timebonustimeout = 0.0f, float timebonus = 0.0f, float maxangle = 90.0f);
};

struct Shields : public Powerup {
    SceneNode* shieldNode;
    Entity* shieldEntity;
    
    float shieldScaleValue;
    float shieldScaleTarget;
    
    virtual PowerupType getPowerupType() const
    { return POWERUP_SHIELDS; }
    
    virtual void reset();
    virtual ~Shields();
    
    Shields();
};

#endif
