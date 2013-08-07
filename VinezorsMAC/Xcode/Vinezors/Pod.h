#pragma once

#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include "Util.h"

enum PodType { POD_BLUE, POD_GREEN, POD_PINK, POD_BLACK, POD_YELLOW, POD_NONE };
#define NUM_POD_TYPES 5

// These are objects which are attached to the walls and may act as hints, boosters, or penalties.
// They are comprised of a stem (cylinder) and a head (sphere)
class Pod
{
private:
	PodType type;
    Vector3 base;
    Vector3 tip;
    Number stemRadius;
    Number headRadius;
	ScenePrimitive *stem;
	ScenePrimitive *head;
    ScenePrimitive * shell;
    
    bool pastFog;
public:
	Pod();
	Pod(CollisionScene *scene, Vector3 center, Quaternion rot, PodType type, Vector3 base, Vector3 tip, Number stemRadius, Number headRadius);
	
	PodType getType() const;
	ScenePrimitive *getStem() const;
	ScenePrimitive *getHead() const;

	void move(Vector3 delta);
	
    void setToGrowth(double t);
	void hidePod();
    void showPod();
    void showShell();

	void addToCollisionScene(CollisionScene *scene);

	void removeFromCollisionScene(CollisionScene * scene);
    
    PodType getPodType();
    
    bool getPastFog();
    void setPastFog(bool past);
};