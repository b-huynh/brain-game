//
//  Tunnel.h
//  Testing
//
//  Created by Calvin Phung on 9/10/13.
//
//

#ifndef __Vinezors2_0__Tunnel__
#define __Vinezors2_0__Tunnel__

#include <list>
#include <vector>
#include "Util.h"
#include "TunnelSlice.h"
#include "History.h"

enum GameMode { GAME_TIMED, GAME_NORMAL };

// Stores the list of tunnel segments
class Tunnel
{
public:
    SceneNode* parentNode;
    
    SceneNode* mainTunnelNode;
	Vector3 start;
	Vector3 end;
    std::list<TunnelSlice*> segments;
    std::list<TunnelSlice*>::iterator current;
    int segmentCounter;
	double segmentWidth;
	double segmentDepth;
	int segmentMinAngleTurn;
	int segmentMaxAngleTurn;
    Quaternion endRot;
    
    std::vector<SectionInfo> sections;
    std::vector<PodInfo> types;
    int sectionSize;
    int podSegmentSize;
    int sectionIndex;
    int podIndex;
    int renewalSectionCounter;
    int renewalPodCounter;
    std::list<Pod*> activePods; // Animating pods
    
    // Stage attributes
    GameMode mode;
    double totalElapsed;
    int nback;
    int control;
    History* history;
    Direction basis;
    bool sidesUsed[NUM_DIRECTIONS];
    
    bool done;      // Says stage is over, but not the ending animation
    bool cleanup;   // Totally done, ending animation is over
public:
	Tunnel();
    
	Tunnel(Ogre::SceneNode* parentNode, Vector3 start, double segmentWidth, double segmentDepth, int segmentMinAngleTurn, int segmentMaxAngleTurn, GameMode mode, int nback, int control, Direction sloc, int sectionSize, int podSegmentSize);
	
    SceneNode* getMainTunnelNode() const;
	Vector3 getStart() const;
	Vector3 getEnd() const;
	Vector3 getCenter() const;
    std::list<TunnelSlice*>::const_iterator getCurrentIterator() const;
    std::list<TunnelSlice*>::const_iterator getBeginIterator() const;
    std::list<TunnelSlice*>::const_iterator getEndIterator() const;
    TunnelSlice* findSliceFromCurrent(Vector3 pos, double tOffset, double & tLeft) const;
	TunnelSlice* getCurrent() const;
	TunnelSlice* getFront() const;
	TunnelSlice* getBack() const;
	TunnelSlice* getNext(int i) const;
	int getSectionIndex() const;
	int getPodIndex() const;
    Quaternion getQuaternion() const;
    Quaternion getCombinedQuaternion(TunnelSlice* slice) const;
    History* getHistory() const;
    
	double getSegmentWidth() const;
	double getSegmentDepth() const;
    std::vector<SectionInfo> getSectionInfo() const;
    std::vector<PodInfo> getPodInfo() const;
    Quaternion getNewSegmentQuaternion(Direction dir, int degrees);
    PodType getNBackTest(int section) const;
    GameMode getMode() const;
    double getTotalElapsed() const;
    int getNBack() const;
    int getControl() const;
    Direction getBasis() const;
    bool hasAvailableSide(Direction side) const;
    std::string determineMaterial() const;
    
    bool isDone() const;
    void setDone(Evaluation eval);
    bool needsCleaning() const;
    
    void setNewControl(int control);
	void removeSegment();
    
    SectionInfo getNextSectionInfo() const;
    PodInfo getNextPodInfo(SectionInfo & sectionInfo) const;
    void addSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc, bool podGood);
	void renewSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc, bool podGood);
    void addSection(SectionInfo newSection);
    void renewSection(SectionInfo newSection);
    
	bool renewIfNecessary(Vector3 checkPos);
    
    std::vector<Pod *> findPodCollisions(SceneNode* ent);
    
    void constructTunnel(int size, Quaternion q = Quaternion(1, 0, 0, 0));
    
    void update(Player* player, Hud* hud, double elapsed);
    
	~Tunnel();
};

#endif /* defined(__Testing__Tunnel__) */
