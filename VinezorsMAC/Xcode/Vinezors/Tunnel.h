#pragma once

#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include <list>
#include <vector>
#include "Util.h"
#include "TunnelSlice.h"

using namespace std;

struct SectionInfo
{
    TunnelType tunnelType;
    Direction tunnelDir;
    Number tunnelDirAngle;
    
    SectionInfo(TunnelType tt, Direction td, Number tda)
        : tunnelType(tt), tunnelDir(td), tunnelDirAngle(tda)
    {}
};

struct PodInfo
{
    PodType podType;
    Direction podLoc;
    
    PodInfo(PodType pt, Direction pl)
        : podType(pt), podLoc(pl)
    {}
};

// Stores the list of tunnel segments
class Tunnel
{
public:
	CollisionScene *scene;
	
	Vector3 start;
	Vector3 end;
	list<TunnelSlice*> segments;
	list<TunnelSlice*>::iterator current;
	Number segmentWidth;
	Number segmentDepth;
    Quaternion endRot;
    
    vector<SectionInfo> sections;
    vector<PodInfo> types;
    int sectionSize;
    int podSegmentSize;
    int sectionIndex;
    int podIndex;
    int renewalSectionCounter;
    int renewalPodCounter;
    
    int nback;
    int sinceLastnback;
    
    bool done;
public:
	Tunnel();
	Tunnel(CollisionScene *scene, Vector3 start, Number segmentWidth, Number segmentDepth, int sectionSize, int podSegmentSize);
	
	CollisionScene * getScene() const;
	Vector3 getStart() const;
	Vector3 getEnd() const;
	Vector3 getCenter() const;
	list<TunnelSlice*>::iterator getCurrentIterator();
	list<TunnelSlice*>::iterator getBeginIterator();
	list<TunnelSlice*>::iterator getEndIterator();
    TunnelSlice* findSliceFromCurrent(Vector3 pos, Number tOffset) const;
	TunnelSlice* getCurrent() const;
	TunnelSlice* getNext(int i) const;
	int getSectionIndex() const;
	int getPodIndex() const;

	Number getSegmentWidth() const;
	Number getSegmentDepth() const;
    Quaternion getNewSegmentQuaternion(Direction dir, int degrees);
    PodType getNBackTest(int section) const;
    int getNBack() const;
    
    bool isDone() const;
    void setDone(bool value);
    
	void removeSegment();
    
    SectionInfo getNextSectionInfo();
    PodInfo getNextPodInfo(SectionInfo & sectionInfo);
    void addSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc);
	void renewSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc);
    void addSection(SectionInfo newSection);
    void renewSection(SectionInfo newSection);
    
	bool renewIfNecessary(Vector3 checkPos);

    vector<Pod *> findPodCollisions(CollisionScene * scene, SceneEntity * entity);
    
    void constructTunnel(int size, int nback, Quaternion q = Quaternion(1, 0, 0, 0));

	~Tunnel();
};
