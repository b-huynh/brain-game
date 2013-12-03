//
//  Pod.h
//  Testing
//
//  Created by Calvin Phung on 9/3/13.
//
//

#ifndef __Vinezors2_0__Pod__
#define __Vinezors2_0__Pod__

#include "OgreFramework.h"
#include "Util.h"

// This is here tempoarily until we figure out how to change colors of specific parts of mesh
enum MeshType { BASIC, FUEL };

// These are objects which are attached to the walls and may act as hints, boosters, or penalties.
// They are comprised of a stem (cylinder) and a head (sphere)
class Pod
{
private:
    Ogre::SceneNode* parentNode;
    
    MeshType mtype;
    Vector3 base;
    Vector3 tip;
	PodType type;
    double stemRadius;
    double stemLength;
    double headRadius;
    SceneNode* entirePod;
	SceneNode* stem;
	SceneNode* head;
    double moveSpeed;
    double rotateSpeed;
    
    Direction loc;
    bool podTaken;
    
    Vector3 dest;
public:
	Pod();
    
	Pod(Ogre::SceneNode* parentNode, Vector3 base, Vector3 tip, PodType type, double stemRadius, double headRadius, Direction loc);
	
    void loadBasicShape();
    void loadFuelCell();
    
    Vector3 getBase() const;
    Vector3 getTip() const;
	PodType getType() const;
	SceneNode* getStem() const;
	SceneNode* getHead() const;
	Vector3 getDest() const;
	Vector3 getPosition() const;
	Direction getLoc() const;
	double getStemRadius() const;
	double getStemLength() const;
	double getHeadRadius() const;
    
    bool isPodTaken() const;
    
	void move(Vector3 delta);
	
    void setToGrowth(double t);
	void takePod();
    void hidePod();
    void revealPod();
    void setDest(Vector3 value);
	void setMoveSpeed(double value);
	void setRotateSpeed(double value);
    
	void removeFromScene();
    
	void update(double elapsed);
    
    PodType getPodType() const;
    
    ~Pod();
};

#endif /* defined(__Testing__Pod__) */
