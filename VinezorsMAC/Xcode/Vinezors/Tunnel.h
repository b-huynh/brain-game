#pragma once

#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include <list>
#include <vector>
#include "Util.h"
#include "TunnelSlice.h"

using namespace std;

// Stores the list of tunnel segments
class Tunnel
{
private:
	CollisionScene *scene;
	
	Vector3 start;
	Vector3 end;
	list<TunnelSlice*> segments;
	list<TunnelSlice*>::iterator current;
	Number segmentWidth;
	Number segmentDepth;
public:
	Tunnel();
	Tunnel(CollisionScene *scene, Vector3 start, Number segmentWidth, Number segmentDepth);
	
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
	Number getSegmentWidth() const;
	Number getSegmentDepth() const;

    void addSegment(PodType type, Direction loc);
	void addSegment(PodType type);
	void removeSegment();
	void renewSegment();
	
	bool renewIfNecessary(Vector3 checkPos);

    vector<Pod *> findPodCollisions(CollisionScene * scene, SceneEntity * entity);
    
    void constructTunnel(int size, int nback);

	~Tunnel();
};
