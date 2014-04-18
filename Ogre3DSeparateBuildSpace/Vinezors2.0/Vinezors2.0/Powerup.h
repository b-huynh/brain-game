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
    virtual void dummy() {}
};

struct TractorBeam : public Powerup {
  
    SceneNode* tractorBeamRotatorNode;
    SceneNode* tractorBeamNode;
    ParticleSystem* tractorBeamEffect;
    
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
    
    float xFlySpeed;
    float yFlySpeed;
    float scaleFactor;
    
    SceneNode* timeBonusNode;
    Entity* timeBonusEntity;
    
    TimeWarp(float timeout = 0.0f, float timebonustimeout = 0.0f, float timebonus = 0.0f, float maxangle = 90.0f);
};

#endif
