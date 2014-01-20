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
    MeshType meshType;
    PodSignal podSignal;
    PodColor podColor;
    PodShape podShape;
    PodSound podSound;
    Direction podLoc;
    bool goodPod; // is the pod good to take?
    bool podTrigger; // trigger on: false = after pod has past, true = on collision
    bool podTaken; // is the pod gone?
    
    PodInfo()
    : meshType(BASIC), podSignal(POD_SIGNAL_UNKNOWN), podColor(POD_COLOR_UNKNOWN), podShape(POD_SHAPE_UNKNOWN), podSound(POD_SOUND_UNKNOWN),
    podLoc(NO_DIRECTION), goodPod(false), podTrigger(false), podTaken(false)
    {}
    
    PodInfo(MeshType mtype, PodSignal psig, PodColor pcol, PodShape pshp, PodSound psod, Direction pl, bool good, bool trigger = false, bool taken = false)
    : meshType(mtype), podSignal(psig), podColor(pcol), podShape(pshp), podSound(psod), podLoc(pl), goodPod(good), podTrigger(trigger), podTaken(taken)
    {}
};

// These are objects which are attached to the walls and may act as hints, boosters, or penalties.
// They are comprised of a stem (cylinder) and a head (sphere)
class Pod
{
private:
    Ogre::SceneNode* parentNode;
    
    MeshType mtype;
    std::string materialName;
    
    Entity* headContentEntity;
    SceneNode* glowNode;
    ParticleSystem* glowEffect;
    
    Vector3 base;
    Vector3 tip;
    PodSignal podSignal;
	PodColor podColor;
	PodShape podShape;
	PodSound podSound;
    float stemRadius;
    float stemLength;
    float headRadius;
    SceneNode* entirePod;
	SceneNode* stem;
	SceneNode* head;
	SceneNode* shell;
    float moveSpeed;
    Vector3 rotateSpeed;
    
    Direction loc;
    bool podTested;
    bool podTaken;
    bool podTrigger;
    bool podGood;
    
    Vector3 dest;
public:
	Pod();
    
	Pod(Ogre::SceneNode* parentNode, Vector3 base, Vector3 tip, MeshType mtype, PodSignal podSignal, PodColor podColor, PodShape podShape, PodSound podSound, Direction loc, float stemRadius, float headRadius);
	
    void loadBasicShape();
    void loadFuelCell();
    
    MeshType getMeshType() const;
    Vector3 getBase() const;
    Vector3 getTip() const;
	PodSignal getPodSignal() const;
	PodColor getPodColor() const;
	PodShape getPodShape() const;
	PodSound getPodSound() const;
    SceneNode* getEntirePod() const;
	SceneNode* getStem() const;
	SceneNode* getHead() const;
	Vector3 getDest() const;
	Vector3 getPosition() const;
	Direction getLoc() const;
    PodInfo getPodInfo() const;
	float getStemRadius() const;
	float getStemLength() const;
	float getHeadRadius() const;
    SceneNode* getGlowNode() const;
    
    bool isPodTested() const;
    bool isPodTaken() const;
    bool getPodTrigger() const;
    bool isPodGood() const;
    
	void move(Vector3 delta);
	
    void setToGrowth(float t);
    void setSkin();
	void takePod();
    void hidePod();
    void revealPod();
    void uncloakPod();
    void generateGlow();
    void setDest(Vector3 value);
	void setMoveSpeed(float value);
	void setRotateSpeed(Vector3 value);
    void setPodTested(bool value);
    void setPodGood(bool value);
    void setPodTrigger(bool value);
    
	void removeFromScene();
    
	void update(float elapsed);
    
    PodColor getPodType() const;
    
    ~Pod();
};

#endif /* defined(__Testing__Pod__) */
