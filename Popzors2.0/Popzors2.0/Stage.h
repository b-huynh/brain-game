#pragma once

#include "OgreFramework.h"

#include "Pch.h"
#include "Ground.h"
#include "Poppy.h"
#include "Pot.h"


struct ClickedResult {
    Poppy * poppy;
    Pot * pot;
    int eventCode;
    //RayTestResult ray;
    
    ClickedResult()
    : poppy(NULL), pot(NULL), eventCode(-1) //, ray(RayTestResult())
    {}
};


// Contains the interface for the environment and all entities.
class Stage {
public:
    //Ground
    Ground* ground;
    
    //Lighting
    std::vector<Light*> lights;
    
	//Objects
    std::vector<Poppy*> poppies;
    std::vector<Pot*> pots;
    
	//ScreenLabel* label1;
    //ScreenLabel* label2;

    //ScreenShape* progressBar;
    //Sound* negativeFeedback;
    //Sound* positiveFeedback;
    
    Selectable* selected;
    
    // Utility Functions
    void handlePoppyCollisions(double elapsed);
    void handlePotCollisions(double elapsed);
    //ClickedResult rayTest(Vector3 origin, Vector3 dest);
    //ClickedResult getClicked(InputEvent * inputEvent);
    
    // Cleanup entire scene
    void clear()
    {
        if (ground) {
            ground->removeFromScene();
            delete ground;
        }
        ground = NULL;
        
        for (int i = 0; i < poppies.size(); ++i)
        {
            poppies[i]->removeFromScene();
            delete poppies[i];
        }
        poppies.clear();
        
        for (int i = 0; i < pots.size(); ++i)
        {
            pots[i]->removeFromScene();
            delete pots[i];
        }
        pots.clear();
        
        for (int i = 0; i < lights.size(); ++i)
        {
            OgreFramework::getSingletonPtr()->m_pSceneMgr->destroyLight(lights[i]);
            delete lights[i];
        }
        lights.clear();
        
//        if (progressBar) {
//            screen->removeChild(progressBar);
//            delete progressBar;
//        }
//        
//        if (label1) {
//            screen->removeChild(label1);
//            delete label1;
//        }
//        if (label2) {
//            screen->removeChild(label2);
//            delete label2;
//        }
//        if (negativeFeedback) delete negativeFeedback;
//        if (positiveFeedback) delete positiveFeedback;
        
        selected = NULL;
    }
    
//    Stage(Screen *screen, CollisionScene *scene) : screen(screen), scene(scene), ground(NULL), poppies(), pots(), lights(), progressBar(NULL), label1(NULL), label2(NULL), negativeFeedback(NULL), positiveFeedback(NULL)
//    {}

    Stage() : ground(NULL), poppies(), pots(), lights(), selected(NULL)
    {
    }
    
    void update(double elapsed) {
        ground->update(elapsed);
        for (int i = 0; i < poppies.size(); ++i)
            poppies[i]->update(elapsed);
        for (int i = 0; i < pots.size(); ++i)
            pots[i]->update(elapsed);
    }
    
    ~Stage() {}
};