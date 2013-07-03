#pragma once

#include "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"
#include "Poppy.h"
#include "Pot.h"

#include <vector>
#include <cstdlib>

using namespace Polycode;

static const Color Cpot1 = Color(204,0,204,255);
static const Color Cpot2 = Color(105,100,100,255);
static const Color Cpot3 = Color(150,150,0,255);
static const Color Cpot4 = Color(0,255,145,255);

Color getRandomPotColor();

struct ClickedResult {
		Poppy * poppy;
		Pot * pot;
		ClickedResult() : poppy(NULL), pot(NULL) {}
};

class Popzors : public EventHandler	
{
public:
    Popzors(PolycodeView *view);
    ~Popzors();
    
	void handleEvent(Event *e);
    bool Update();
    
private:
    Core *core;

	//Screen
	Screen *screen;
	CollisionScene *scene;

	//Camera
	Vector3 cameraPos;
	Vector3 origin;

	//Selected
	Selectable * selected;

	//Objects
	vector<Poppy> poppies;
	vector<Pot> pots;

	//State variables
	Number timeElapsed;

	bool signaled;
	Number signalStart;
	Number signalLength;

	int currentPoppyID;

	//Utility Functions
	ClickedResult getClicked(Vector3 origin, Vector3 dest);				
};
