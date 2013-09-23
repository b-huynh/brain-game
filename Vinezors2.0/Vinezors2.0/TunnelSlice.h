//
//  TunnelSlice.h
//  Testing
//
//  Created by Calvin Phung on 9/9/13.
//
//

#ifndef __Testing__TunnelSlice__
#define __Testing__TunnelSlice__

#include "OgreFramework.h"
#include "Util.h"
#include "Pod.h"

#include <vector>

enum TunnelType { NORMAL_WITH_ONE_POD, NORMAL_WITH_THREE_PODS, NORMAL_WITH_FIVE_PODS, NORMAL_WITH_MANY_PODS, NORMAL_BLANK, CHECKPOINT };

struct SectionInfo
{
    TunnelType tunnelType;
    Direction tunnelDir;
    int tunnelDirAngle;
    
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
    bool good;
    
    PodInfo()
    : podType(POD_NONE), podLoc(NO_DIRECTION), good(false)
    {}
    
    PodInfo(PodType pt, Direction pl, bool g)
    : podType(pt), podLoc(pl), good(g)
    {}
};

// Contains the components of a segment of a tunnel which include the wall and pod information
class TunnelSlice
{
private:
	Vector3 center;
    Quaternion rot;
	double width;
	double depth;
	
	TunnelType type;
    std::string materialName;
    SceneNode* entireWall;
	SceneNode* topLeftWall;
	SceneNode* topWall;
	SceneNode* topRightWall;
	SceneNode* rightWall;
	SceneNode* bottomRightWall;
	SceneNode* bottomWall;
	SceneNode* bottomLeftWall;
	SceneNode* leftWall;
    SceneNode* intermediateSegment;
    
    std::vector<Pod*> pods;
    double growthT;
    double prerangeT;
    
    SectionInfo sectionInfo;
    PodInfo podInfo;
    bool podTaken;
    bool infoStored;
public:
	TunnelSlice();
	TunnelSlice(TunnelType type, Vector3 center, Quaternion rot, double width, double depth);
	
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
    bool isPodTaken() const;
    bool isInfoStored() const;
    
    std::vector<Pod*> findCollisions(SceneNode *ent) const;
    Vector3 requestWallDistance(Direction dir) const;
    Vector3 requestMove(Direction dir) const;
    Vector3 requestPosition(Vector3 cur, Direction dir) const;
    
    void setSectionInfo(const SectionInfo & value);
    void setPodInfo(const PodInfo & value);
    void setPodTaken(bool value);
    void setInfoStored(bool value);
    void setPrerangeT(double value);
	void move(Vector3 delta);
	void addPod(Direction loc, PodType type);
    void connect(TunnelSlice* next);
    void disconnect();
    void clearPods();
    void updateGrowth(double nt);
    
    void rejuvenate(TunnelType type, Vector3 center, Quaternion rot, double width, double depth);
    
	void removeFromScene();
};

#endif /* defined(__Testing__TunnelSlice__) */

