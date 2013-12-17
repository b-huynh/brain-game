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

struct PodInfo
{
    PodSignal podSignal;
    PodColor podColor;
    PodShape podShape;
    PodSound podSound;
    Direction podLoc;
    bool goodPod; // is the pod good to take?
    bool podTaken; // is the pod gone?
    
    PodInfo()
    : podSignal(POD_SIGNAL_UNKNOWN), podColor(POD_COLOR_UNKNOWN), podShape(POD_SHAPE_UNKNOWN), podSound(POD_SOUND_UNKNOWN),
    podLoc(NO_DIRECTION), goodPod(false), podTaken(false)
    {}
    
    PodInfo(PodSignal psig, PodColor pcol, PodShape pshp, PodSound psod, Direction pl, bool good, bool taken = false)
    : podSignal(psig), podColor(pcol), podLoc(pl), podSound(psod), goodPod(good), podTaken(taken)
    {}
};

// These are objects which are attached to the walls and may act as hints, boosters, or penalties.
// They are comprised of a stem (cylinder) and a head (sphere)
class Pod
{
private:
    Ogre::SceneNode* parentNode;
    
    MeshType mtype;
    Vector3 base;
    Vector3 tip;
    PodSignal podSignal;
	PodColor podColor;
	PodShape podShape;
	PodSound podSound;
    double stemRadius;
    double stemLength;
    double headRadius;
    SceneNode* entirePod;
	SceneNode* stem;
	SceneNode* head;
	SceneNode* shell;
    double moveSpeed;
    Vector3 rotateSpeed;
    
    Direction loc;
    bool podTaken;
    bool podGood;
    
    Vector3 dest;
public:
	Pod();
    
	Pod(Ogre::SceneNode* parentNode, Vector3 base, Vector3 tip, PodSignal podSignal, PodColor podColor, PodShape podShape, PodSound podSound, Direction loc, double stemRadius, double headRadius);
	
    void loadBasicShape();
    void loadFuelCell();
    
    Vector3 getBase() const;
    Vector3 getTip() const;
	PodSignal getPodSignal() const;
	PodColor getPodColor() const;
	PodShape getPodShape() const;
	PodSound getPodSound() const;
	SceneNode* getStem() const;
	SceneNode* getHead() const;
	Vector3 getDest() const;
	Vector3 getPosition() const;
	Direction getLoc() const;
    PodInfo getPodInfo() const;
	double getStemRadius() const;
	double getStemLength() const;
	double getHeadRadius() const;
    
    bool isPodTaken() const;
    bool isPodGood() const;
    
	void move(Vector3 delta);
	
    void setToGrowth(double t);
	void takePod();
    void hidePod();
    void revealPod();
    void setDest(Vector3 value);
	void setMoveSpeed(double value);
	void setRotateSpeed(Vector3 value);
    void setPodGood(bool value);
    
	void removeFromScene();
    
	void update(double elapsed);
    
    PodColor getPodType() const;
    
    ~Pod();
};

#endif /* defined(__Testing__Pod__) */
