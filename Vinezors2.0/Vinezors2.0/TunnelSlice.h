//
//  TunnelSlice.h
//  Testing
//
//  Created by Calvin Phung on 9/9/13.
//
//

#ifndef __Vinezors2_0__TunnelSlice__
#define __Vinezors2_0__TunnelSlice__

#include "OgreFramework.h"
#include "Util.h"
#include "Pod.h"

#include <vector>

enum TunnelType { NORMAL_WITH_ONE_POD, NORMAL_WITH_THREE_PODS, NORMAL_WITH_FIVE_PODS, NORMAL_WITH_MANY_PODS, NORMAL_BLANK, CHECKPOINT };

struct SectionInfo
{
    TunnelType tunnelType;
    Direction tunnelDir; // The direction each segment is turning
    int tunnelDirAngle; // The amount of turning degrees for each segment
    
    SectionInfo()
    : tunnelType(NORMAL_BLANK), tunnelDir(NO_DIRECTION), tunnelDirAngle(0)
    {}
    
    SectionInfo(TunnelType tt, Direction td, int tda)
    : tunnelType(tt), tunnelDir(td), tunnelDirAngle(tda)
    {}
};

struct PodInfo
{
    PodType podType;
    Direction podLoc; 
    bool goodPod; // is the pod good to take?
    bool podTaken; // is the pod gone?
    
    PodInfo()
    : podType(POD_NONE), podLoc(NO_DIRECTION), goodPod(false)
    {}
    
    PodInfo(PodType pt, Direction pl, bool g)
    : podType(pt), podLoc(pl), goodPod(g), podTaken(false)
    {}
};

// Contains the components of a segment of a tunnel which include the wall and pod information
class TunnelSlice
{
private:
    SceneNode* parentNode;
    
	Vector3 center;
    Quaternion rot;
	double width;
	double depth;
	
	TunnelType type;
    std::string materialName;
    
    // This segment's mesh
    SceneNode* entireWall;
	SceneNode* topLeftWall;
	SceneNode* topWall;
	SceneNode* topRightWall;
	SceneNode* rightWall;
	SceneNode* bottomRightWall;
	SceneNode* bottomWall;
	SceneNode* bottomLeftWall;
	SceneNode* leftWall;
    
    // Mesh connecting to another segment if any
    SceneNode* entireIntermediate;
	SceneNode* topLeftIntermediate;
	SceneNode* topIntermediate;
	SceneNode* topRightIntermediate;
	SceneNode* rightIntermediate;
	SceneNode* bottomRightIntermediate;
	SceneNode* bottomIntermediate;
	SceneNode* bottomLeftIntermediate;
	SceneNode* leftIntermediate;
    
    std::vector<Pod*> pods;
    double growthT; // Pod Growth Timing animation
    double prerangeT; // Used to reduce bounce of player
    
    bool sidesUsed[NUM_DIRECTIONS];
    
    SectionInfo sectionInfo;
    PodInfo podInfo;
    bool infoStored; // Used to avoid saving data multiple times
public:
	TunnelSlice();
	TunnelSlice(Ogre::SceneNode* parentNode, TunnelType type, Vector3 center, Quaternion rot, double width, double depth, const bool sides[NUM_DIRECTIONS]);
	
    void initWalls();
    
    double getWallLength() const;
    TunnelType getType() const;
    Quaternion getQuaternion() const;
	Vector3 getStart() const;
	Vector3 getEnd() const;
	Vector3 getCenter() const;
	Vector3 getCenter(double t) const;
	double getT(Vector3 pos) const;
	double getPrerangeT() const;
    Vector3 getForward() const;
	Vector3 getUpward() const;
	Vector3 getRight() const;
    std::vector<Pod *> getPods() const;
    SectionInfo getSectionInfo() const;
    PodInfo getPodInfo() const;
    bool isInfoStored() const;
    bool hasAvailableSide(Direction side) const;
    
    std::vector<Pod*> findCollisions(SceneNode *ent) const;
    Vector3 requestWallDistance(Direction dir) const;
    Vector3 requestMove(Direction dir) const;
    Vector3 requestPosition(Vector3 cur, Direction dir) const;
    
    void setSectionInfo(const SectionInfo & value);
    void setPodInfo(const PodInfo & value);
    void setInfoStored(bool value);
    void setPrerangeT(double value);
	void move(Vector3 delta);
	void addPod(Direction loc, PodType type);
    void setIntermediateWall(SceneNode* entire, Direction dir, Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4);
    void connect(TunnelSlice* next);
    void disconnect();
    void clearPods();
    void updateGrowth(double nt);
    
    void rejuvenate(TunnelType type, Vector3 center, Quaternion rot, double width, double depth);
    
	void removeFromScene();
};

#endif /* defined(__Testing__TunnelSlice__) */

