//
//  Powerup.cpp
//  Vinezors2.0
//
//  Created by Emmanuel Flores on 4/15/14.
//
//

#include "Powerup.h"

Powerup::Powerup(float timeout)
    : available(false), active(false), mainTimer(0.0f), mainTimeout(timeout) {}

TractorBeam::TractorBeam()
    : Powerup(0.0f), tractorBeamEffect(NULL), tractorBeamNode(NULL), tractorBeamRotatorNode(NULL) {}

TimeWarp::TimeWarp(float timeout, float timebonustimeout, float timebonus, float maxangle)
    : Powerup(timeout), origFov(Degree()), origAngle(0.0f), currentAngle(0.0f), maxAngle(maxangle), angleIncrement(1.0f), slowdown(0.2f), zoomIn(0), origOffset(0.0f), currentOffset(0.0f), offsetIncrement(0.02f), offsetLimit(0.3f), timeBonus(timebonus), timeBonusTimeout(timebonustimeout), xFlySpeed(-0.8f), yFlySpeed(0.5f), scaleFactor(0.9f), timeBonusNode(NULL), timeBonusEntity(NULL) {}
