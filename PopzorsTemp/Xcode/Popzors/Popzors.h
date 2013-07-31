#pragma once

#include "Pch.h"
#include "PoppyPattern.h"
#include "PotPattern.h"
#include "HerdPattern.h"

#include <cstdlib>

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
    
    SceneLight * light;
    
    //Label for Debug
    ScreenLabel * label;
    
	//Camera
	Vector3 cameraPos;
	Vector3 origin;

    // Pattern Game Logic
    PopzorsPattern *pattern;
};
