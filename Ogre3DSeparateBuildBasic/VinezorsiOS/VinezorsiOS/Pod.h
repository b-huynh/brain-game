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

// These are objects which are attached to the walls and may act as hints, boosters, or penalties.
// They are comprised of a stem (cylinder) and a head (sphere)
class Pod
{
private:
    Ogre::SceneNode* parentNode;
    
    PodMeshType mtype;
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
    
	Pod(Ogre::SceneNode* parentNode, Vector3 base, Vector3 tip, PodMeshType mtype, PodSignal podSignal, PodColor podColor, PodShape podShape, PodSound podSound, Direction loc, float stemRadius, float headRadius);
	
    void loadPod();
    void loadBasicShape();
    void loadFlower();
    void loadFuelCell();
    void loadHazard();
    
    PodMeshType getMeshType() const;
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
    void generateGlow(PodColor color, PodShape shape);
    void setDest(Vector3 value);
	void setMoveSpeed(float value);
	void setRotateSpeed(Vector3 value);
    void setPodTested(bool value);
    void setPodGood(bool value);
    void setPodTrigger(bool value);
    
    void removeGlow();
	void removeFromScene();
    
	void update(float elapsed);
    
    PodColor getPodType() const;
    
    ~Pod();
};

#endif /* defined(__Testing__Pod__) */
