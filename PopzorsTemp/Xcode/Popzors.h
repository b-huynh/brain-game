#pragma once

#import "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"
#include "Poppy.h"
#include "Pot.h"

#include <vector>
#include <cstdlib>

using namespace Polycode;

static const Color Cpot1 = Color(71,60,139,255);
static const Color Cpot2 = Color(107,142,35,255);
static const Color Cpot3 = Color(210,105,30,255);
static const Color Cpot4 = Color(205,55,0,255);

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

	Number resultsLength;

	int currentPoppyID;

	//Utility Functions
	ClickedResult getClicked(Vector3 origin, Vector3 dest);				
};
