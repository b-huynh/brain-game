#pragma once

#include "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

#define TUNNEL_WIDTH 5.0
#define TUNNEL_DEPTH 10.0
#define LIGHT_INTENSITY 5.0

class TunnelSlice
{
public:
	TunnelSlice();
	TunnelSlice(CollisionScene *scene, Vector3 center, Number width, Number depth, Number intensity);
	
	void addToCollisionScene(CollisionScene *scene) const;

private:
	ScenePrimitive *topLeftWall;
	ScenePrimitive *topWall;
	ScenePrimitive *topRightWall;
	ScenePrimitive *rightWall;
	ScenePrimitive *bottomRightWall;
	ScenePrimitive *bottomWall;
	ScenePrimitive *bottomLeftWall;
	ScenePrimitive *leftWall;
	
	ScenePrimitive *topLeftWallBorder;
	ScenePrimitive *topWallBorder;
	ScenePrimitive *topRightWallBorder;
	ScenePrimitive *rightWallBorder;
	ScenePrimitive *bottomRightWallBorder;
	ScenePrimitive *bottomWallBorder;
	ScenePrimitive *bottomLeftWallBorder;
	ScenePrimitive *leftWallBorder;

	SceneLight *light;
};

class Tunnel
{
};

class Vinezors {
public:
    Vinezors(PolycodeView *view);
    ~Vinezors();
    
	void handleEvent(Event *e);
    bool Update();
    
private:

    Core *core;

	Screen *screen;
	CollisionScene *scene;
	
	Vector3 origin;
	Vector3 camPos;
	vector<TunnelSlice> tunnels;
};
