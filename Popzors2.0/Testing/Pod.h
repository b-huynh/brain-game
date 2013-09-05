//
//  Pod.h
//  Testing
//
//  Created by Calvin Phung on 9/3/13.
//
//

#ifndef __Testing__Pod__
#define __Testing__Pod__

#include "OgreFramework.h"
#include "Util.h"

enum PodType { POD_BLUE, POD_GREEN, POD_PINK, POD_YELLOW, POD_BLACK, POD_NONE };
#define NUM_POD_TYPES 5

// These are objects which are attached to the walls and may act as hints, boosters, or penalties.
// They are comprised of a stem (cylinder) and a head (sphere)
class Pod
{
private:
    Vector3 base;
    Vector3 tip;
	PodType type;
    double stemRadius;
    double headRadius;
	SceneNode* stem;
	SceneNode* head;
    SceneNode* shell;
    
public:
	Pod();
	Pod(Vector3 base, Vector3 tip, PodType type, double stemRadius, double headRadius);
	Pod(Vector3 base, Vector3 tip, Quaternion rot, PodType type, double stemRadius, double headRadius);
	
    Vector3 getBase() const;
    Vector3 getTip() const;
	PodType getType() const;
	SceneNode* getStem() const;
	SceneNode* getHead() const;
    
	void move(Vector3 delta);
	
    void setToGrowth(double t);
	void hidePod();
    void revealPod();
    void showShell();
    
	void removeFromScene();
    
    PodType getPodType();
};

#endif /* defined(__Testing__Pod__) */
