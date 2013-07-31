#pragma once

#include "Pch.h"
#include "Ground.h"
#include "Poppy.h"
#include "Pot.h"

struct ClickedResult {
    Poppy * poppy;
    Pot * pot;
    int eventCode;
    RayTestResult ray;
    
    ClickedResult()
    : poppy(NULL), pot(NULL), eventCode(-1), ray(RayTestResult())
    {}
};

// Contains the interface for the environment and all entities.
class Stage {
public:
    Screen* screen;
    CollisionScene* scene;
    
    //Ground
    Ground *ground;
    
    //Lighting
    vector<SceneLight *> lights;
    
	//Objects
	vector<Poppy*> poppies;
	vector<Pot*> pots;

    
    // Utility Functions
    void handlePoppyCollisions(Number elapsed);
    void handlePotCollisions(Number elapsed);
    ClickedResult rayTest(Vector3 origin, Vector3 dest);
    ClickedResult getClicked(InputEvent * inputEvent);
    
    // Cleanup entire scene
    void clear()
    {
        if (ground) {
            ground->removeFromCollisionScene(scene);
            delete ground;
        }
        ground = NULL;
        
        for (int i = 0; i < poppies.size(); ++i)
        {
            poppies[i]->removeFromCollisionScene(scene);
            delete poppies[i];
        }
        poppies.clear();
        
        for (int i = 0; i < pots.size(); ++i)
        {
            pots[i]->removeFromCollisionScene(scene);
            delete pots[i];
        }
        pots.clear();
        
        for (int i = 0; i < lights.size(); ++i)
        {
            scene->removeEntity(lights[i]);
            delete lights[i];
        }
        lights.clear();
    }
    
    Stage(Screen *screen, CollisionScene *scene) : screen(screen), scene(scene), ground(NULL), poppies(), pots(), lights()
    {}
    
    ~Stage() {}
};