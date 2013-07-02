#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include <list>
#include <vector>
#include "Direction.h"
#include "Pod.h"

using namespace std;

// Contains the components of a segment of a tunnel which include the wall and pod information
class TunnelSlice
{
private:
	Vector3 center;
	double width;
	double depth;

	ScenePrimitive *topLeftWall;
	ScenePrimitive *topWall;
	ScenePrimitive *topRightWall;
	ScenePrimitive *rightWall;
	ScenePrimitive *bottomRightWall;
	ScenePrimitive *bottomWall;
	ScenePrimitive *bottomLeftWall;
	ScenePrimitive *leftWall;

	vector<Pod> pods;
public:
	TunnelSlice();
	TunnelSlice(CollisionScene *scene, Vector3 center, Number width, Number depth);
	
	void move(Vector3 delta);

	void addPod(CollisionScene *scene, Direction loc);

	void addToCollisionScene(CollisionScene *scene);

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
	Number segmentWidth;
	Number segmentDepth;
public:
	Tunnel();
	Tunnel(CollisionScene *scene, Vector3 start, Number segmentWidth, Number segmentDepth);
	
	void addSegment();
	void removeSegment();
	
	void renewIfNecessary(Vector3 checkPos);

	~Tunnel();
};
