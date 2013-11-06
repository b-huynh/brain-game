//
//  Vine.h
//  Testing
//
//  Created by Calvin Phung on 9/13/13.
//
//

#ifndef __Vinezors2_0__Vine__
#define __Vinezors2_0__Vine__

#include <list>
#include "Util.h"

// Represents the character (as a vine), currently incomplete.
struct Vine
{
    Ogre::SceneNode* parentNode;
    
	SceneNode *tip;
	SceneNode *base;
	Vector3 dest;
    Vector3 forward;
    double radius;
    double speed;
    
    Direction loc;
    double previoust;
    double aftert;
    
	Vine();
	Vine(Ogre::SceneNode* parentNode, Vector3 pos, double radius);
	
	SceneNode* getTip() const;
	Vector3 getPos() const;
	void setPos(Vector3 value);
	Vector3 getDest() const;
	Vector3 getForward() const;
	void setDest(Vector3 value);
	void setForward(Vector3 value);
    double getRadius() const;
    void setQuaternion(Quaternion rot);
    
	void move(Vector3 delta);
	
	void update(double elapsed);
    
	void removeFromScene();
};

#endif /* defined(__Testing__Vine__) */
