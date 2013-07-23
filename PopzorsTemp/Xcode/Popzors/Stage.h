#pragma once

#include "Pch.h"
#include "Ground.h"
#include "Poppy.h"
#include "Pot.h"

struct ClickedResult {
    Poppy * poppy;
    Pot * pot;
    ClickedResult() : poppy(NULL), pot(NULL) {}
};

// Contains the interface for the environment and all entities.
class Stage {
public:
    Screen* screen;
    CollisionScene* scene;
    
    //Ground
    Ground *ground;
    
	//Objects
	vector<Poppy*> poppies;
	vector<Pot*> pots;
    
    // Utility Functions
    void handlePoppyCollisions(Number elapsed);
    void handlePotCollisions(Number elapsed);
    ClickedResult getClicked(Vector3 origin, Vector3 dest);
    
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
    }
    
    Stage(Screen *screen, CollisionScene *scene) : screen(screen), scene(scene), ground(NULL), poppies(), pots()
    {}
    
    ~Stage() {}
};