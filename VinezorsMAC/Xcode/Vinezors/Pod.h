#pragma once

#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include "Util.h"

enum PodType { POD_UNKNOWN, POD_BLUE, POD_GREEN, POD_PINK, POD_YELLOW, POD_BLACK };

// These are objects which are attached to the walls and may act as hints, boosters, or penalties.
// They are comprised of a stem (cylinder) and a head (sphere)
class Pod
{
private:
	PodType type;
	ScenePrimitive *stem;
	ScenePrimitive *head;
    ScenePrimitive * shell;
    
    bool pastFog;
public:
	Pod();
	Pod(CollisionScene *scene, PodType type, Vector3 base, Vector3 tip, Number stemRadius, Number headRadius);
	
	PodType getType() const;
	ScenePrimitive *getStem() const;
	ScenePrimitive *getHead() const;

	void move(Vector3 delta);
	
	void hidePod();

	void addToCollisionScene(CollisionScene *scene);

	void removeFromCollisionScene(CollisionScene * scene);
    
    PodType getPodType();
    
    bool getPastFog();
    void setPastFog(bool past);
};