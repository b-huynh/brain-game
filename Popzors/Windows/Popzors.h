#pragma once

#include "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

class Popzors {
public:
    Popzors(PolycodeView *view);
    ~Popzors();
    
	void handleEvent(Event *e);
    bool Update();
    
private:

    Core *core;

	Screen *screen;
	Scene *scene;
};
