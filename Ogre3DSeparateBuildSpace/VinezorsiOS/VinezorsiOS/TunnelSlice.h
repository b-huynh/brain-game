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
    std::vector<std::string> materialNames;
    
    // Node that is parent of all others
    SceneNode* sliceNode;
    
    // This segment's mesh
    SceneNode* entireWall;
    
    // Mesh connecting to another segment if any
    SceneNode* entireIntermediate;
    
    // Railing
    SceneNode* mainLeftSideRailing;
    SceneNode* mainRightSideRailing;
    
    SceneNode* connectorLeftSideRailing;
    SceneNode* connectorLeftSidePoint1;
    SceneNode* connectorLeftSidePoint2;
    SceneNode* connectorRightSideRailing;
    SceneNode* connectorRightSidePoint1;
    SceneNode* connectorRightSidePoint2;
    
    // Transition
    SceneNode* mainLeftTransition;
    SceneNode* mainRightTransition;
    SceneNode* connectorLeftTransition;
    SceneNode* connectorRightTransition;
    
    std::vector<SceneNode*> transitions;
    
    std::vector<MeshPtr> meshes;
    
    std::vector<Pod*> pods;
    float growthT; // Pod Growth Timing animation
    
    bool sidesUsed[NUM_DIRECTIONS];
    
    bool podHistory; // Used to avoid saving data multiple times for history panel
    bool infoStored; // Used to avoid saving data multiple times for log files
public:
    TunnelSlice();
    TunnelSlice(Ogre::SceneNode* parentNode, int nid, SectionInfo info, Vector3 start, float width, float depth, const std::string & material);
    TunnelSlice(Ogre::SceneNode* parentNode, int nid, SectionInfo info, Vector3 start, float width, float depth, const std::vector<std::string> & materials);
    
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
    std::string getMaterialName() const;
    std::string getMaterialName(bool isConnection) const;
    std::string getRailingMaterial() const;
    std::string getConnectorMaterial() const;
    
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
    void addTransitionSet(std::string name, Vector3 p1, Vector3 p2, bool include1, bool include2, float railingRoll, float scaleValue, float scaleAmount);
    void connect(TunnelSlice* next);
    void disconnect();
    void clearPods();
    void updateGrowth(float nt);
    
    void rejuvenate(int nid, SectionInfo info, Vector3 start, float width, float depth, const std::string & material);
    void rejuvenate(int nid, SectionInfo info, Vector3 start, float width, float depth, const std::vector<std::string> & materials);
    
    void removeFromScene();
    
    SceneNode* gateNode;
    Entity* gateEntity;
    
    SceneNode* gateDoorNodes[8];
    Entity* gateDoorEntities[8];
    
    bool makeDecreasingTransition;
};

#endif /* defined(__Testing__TunnelSlice__) */

