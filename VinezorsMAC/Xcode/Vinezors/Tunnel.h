#pragma once

#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include <list>
#include <vector>
#include "Util.h"
#include "Pod.h"

using namespace std;

enum TunnelType { NORMAL_WITH_PODS, NORMAL_ELONGATED, NORMAL_BLANK };

// Contains the components of a segment of a tunnel which include the wall and pod information
class TunnelSlice
{
private:
	Vector3 center;
	double width;
	double depth;
	
	TunnelType type;
	ScenePrimitive *topLeftWall;
	ScenePrimitive *topWall;
	ScenePrimitive *topRightWall;
	ScenePrimitive *rightWall;
	ScenePrimitive *bottomRightWall;
	ScenePrimitive *bottomWall;
	ScenePrimitive *bottomLeftWall;
	ScenePrimitive *leftWall;

	vector<Pod *> pods;
public:
	TunnelSlice();
	TunnelSlice(CollisionScene *scene, TunnelType type, Vector3 center, Number width, Number depth);
	
	Vector3 getCenter() const;
    vector<Pod *> getPods();
    
	vector<Pod *> findCollisions(CollisionScene *scene, SceneEntity *ent) const;

	void move(Vector3 delta);
	void changeWallTexture();
	void addPod(CollisionScene *scene, Direction loc,  PodType type);

	void addToCollisionScene(CollisionScene *scene);
	void postAddToCollisionScene(CollisionScene *scene);
	void removeFromCollisionScene(CollisionScene * scene);
};

// Stores the list of tunnel segments
class Tunnel
{
private:
	CollisionScene *scene;
	
	Vector3 start;
	Vector3 end;
	list<TunnelSlice *> segments;
	list<TunnelSlice *>::iterator current;
	Number segmentWidth;
	Number segmentDepth;
public:
	Tunnel();
	Tunnel(CollisionScene *scene, Vector3 start, Number segmentWidth, Number segmentDepth);
	
	CollisionScene * getScene() const;
	Vector3 getStart() const;
	Vector3 getEnd() const;
	Vector3 getCenter() const;
	TunnelSlice *getCurrent() const;
	Number getSegmentWidth() const;
	Number getSegmentDepth() const;

    void addSegment(PodType type, Direction loc);
	void addSegment(PodType type);
	void removeSegment();
	void renewSegment();
	
	void renewIfNecessary(Vector3 checkPos);

    vector<Pod *> findPodCollisions(CollisionScene * scene, SceneEntity * entity);
    
    void constructTunnel(int size, int nback);

	~Tunnel();
};
