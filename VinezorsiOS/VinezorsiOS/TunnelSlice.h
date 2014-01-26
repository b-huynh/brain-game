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
#include "Vine.h"

#include <vector>

enum TunnelType { NORMAL, BLANK, CHECKPOINT_PASS, CHECKPOINT_FAIL, CHECKPOINT_EVEN };

struct SectionInfo
{
    TunnelType tunnelType;
    Direction tunnelDir; // The direction each segment is turning
    int tunnelDirAngle; // The amount of turning degrees for each segment
    Quaternion tunnelRot;
    bool sidesUsed[NUM_DIRECTIONS];
    
    SectionInfo()
    : tunnelType(NORMAL), tunnelDir(NO_DIRECTION), tunnelDirAngle(0), tunnelRot(), sidesUsed()
    {
        for (int i = 0; i < NUM_DIRECTIONS; ++i)
            sidesUsed[i] = false;
    }
    
    SectionInfo(const SectionInfo & info)
    : tunnelType(info.tunnelType), tunnelDir(info.tunnelDir), tunnelDirAngle(info.tunnelDirAngle), tunnelRot(info.tunnelRot), sidesUsed()
    {
        for (int i = 0; i < NUM_DIRECTIONS; ++i)
            sidesUsed[i] = info.sidesUsed[i];
        
    }
    
    SectionInfo(TunnelType tt, Direction td, int tda, Quaternion rot, const bool used[NUM_DIRECTIONS])
    : tunnelType(tt), tunnelDir(td), tunnelDirAngle(tda), tunnelRot(rot), sidesUsed()
    {
        for (int i = 0; i < NUM_DIRECTIONS; ++i)
            sidesUsed[i] = used[i];
    }
};

// Contains the components of a segment of a tunnel which include the wall and pod information
class TunnelSlice
{
private:
    SceneNode* parentNode;
    
    int tunnelSliceID;
    
	Vector3 center;
    Quaternion rot;
    Direction dir; // Direction from previous tunnel slice
    int dirAngle;
	float width;
	float depth;
	
	TunnelType type;
    std::string materialName;
    
    // This segment's mesh
    SceneNode* sliceNode;
    
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
    std::vector<MeshPtr> meshes;
    
    std::vector<Pod*> pods;
    float growthT; // Pod Growth Timing animation
    
    bool sidesUsed[NUM_DIRECTIONS];
    
    bool podHistory; // Used to avoid saving data multiple times for history panel
    bool infoStored; // Used to avoid saving data multiple times for log files
public:
	TunnelSlice();
	TunnelSlice(Ogre::SceneNode* parentNode, int nid, SectionInfo info, Vector3 start, float width, float depth, const std::string & material);
	
    void initWalls();
    
    int getTunnelSliceID() const;
    float getWallLength() const;
    TunnelType getType() const;
    Quaternion getQuaternion() const;
	Vector3 getStart() const;
	Vector3 getEnd() const;
	Vector3 getCenter() const;
	Vector3 getCenter(float t) const;
	float getT(Vector3 pos) const;
    Vector3 getForward() const;
	Vector3 getUpward() const;
	Vector3 getRight() const;
    std::vector<Pod *> getPods() const;
    SectionInfo getSectionInfo() const;
    bool isPodHistory() const;
    bool isInfoStored() const;
    bool hasAvailableSide(Direction side) const;
    
    std::vector<Pod*> findCollisions(SceneNode *ent) const;
    std::vector<Pod*> findCollisions(Vine *vine);
    Vector3 requestWallDirection(Direction dir) const;
    Vector3 requestWallDistance(Direction dir) const;
    Vector3 requestWallMove(Direction dir, float offset) const;
    Vector3 requestWallPosition(Vector3 cur, Direction dir, float offset) const;
    
    void setPodHistory(bool value);
    void setInfoStored(bool value);
	void move(Vector3 delta);
	void addPod(const PodInfo & value);
    void setIntermediateWall(SceneNode* entire, Direction dir, ManualObject * manual, Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, Vector3 & bl, Vector3 & tr);
    void connect(TunnelSlice* next);
    void disconnect();
    void clearPods();
    void updateGrowth(float nt);
    
    void rejuvenate(int nid, SectionInfo info, Vector3 start, float width, float depth, const std::string & material);
    
	void removeFromScene();
};

#endif /* defined(__Testing__TunnelSlice__) */

