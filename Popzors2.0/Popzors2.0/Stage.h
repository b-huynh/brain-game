#pragma once

#include "OgreFramework.h"

#include "Pch.h"
#include "Util.h"
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
    
    PanelOverlayElement* barHP;
    TextAreaOverlayElement* label1;
    TextAreaOverlayElement* label2;
    TextAreaOverlayElement* label3;
    TextAreaOverlayElement* label4;

    //Sound* negativeFeedback;
    //Sound* positiveFeedback;
    
    Selectable* selected;
    
    // Utility Functions
    void handlePoppyCollisions(double elapsed);
    void handlePotCollisions(double elapsed);
    void handleBothCollisions(double elapsed);
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
        
        selected = NULL;
    }
    
//    Stage(Screen *screen, CollisionScene *scene) : screen(screen), scene(scene), ground(NULL), poppies(), pots(), lights(), progressBar(NULL), label1(NULL), label2(NULL), negativeFeedback(NULL), positiveFeedback(NULL)
//    {}

    Stage() : ground(NULL), poppies(), pots(), lights(), selected(NULL), barHP(NULL), label1(NULL), label2(NULL), label3(NULL), label4(NULL)
    {
        setupHUD();
    }
    
    void update(double elapsed);
    
    void setupHUD();
    
    ~Stage() {}
};