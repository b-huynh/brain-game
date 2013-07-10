#pragma once

#include "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

#include "Util.h"
#include "Player.h"
#include "Tunnel.h"

using namespace std;

static const double TUNNEL_WIDTH = 5.0;
static const double TUNNEL_DEPTH = 5.0;
static const double LIGHT_INTENSITY = 5.0;
static const double VINE_LENGTH = TUNNEL_WIDTH / 4;
static const double VINE_RADIUS = 0.5;

class Vinezors : public EventHandler {
public:
    Vinezors(PolycodeView *view);
    ~Vinezors();
    
	void handleEvent(Event *e);
    bool Update();
    
private:
    Core *core;

	Screen *screen;

	ScreenLabel *label;

	CollisionScene *scene;
	
	Vector3 origin;

	Player *player;

	Tunnel *tunnel;
	
	ScenePrimitive *fog1;
	ScenePrimitive *fog2;
	ScenePrimitive *fog3;
};
