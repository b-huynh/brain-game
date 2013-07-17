#pragma once

#include "Pch.h"
#include "Ground.h"
#include "Poppy.h"
#include "Pot.h"
#include "PoppyPattern.h"

#include <vector>
#include <cstdlib>

using namespace Polycode;

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
    
    void handlePoppyCollisions(Number elapsed);
    
	void handleEvent(Event *e);
    bool Update();
private:
    Core *core;

	//Screen
	Screen *screen;
	CollisionScene *scene;
    
    //Ground
    Ground ground;
    
	//Camera
	Vector3 cameraPos;
	Vector3 origin;

	//Selected
	Selectable * selected;

	//Objects
	vector<Poppy> poppies;
	vector<Pot> pots;

	//State variables
	Number signalLength;
	Number timeElapsed;

	Number resultsLength;

    PoppyPattern pattern;
    
	//Utility Functions
	ClickedResult getClicked(Vector3 origin, Vector3 dest);				
};
