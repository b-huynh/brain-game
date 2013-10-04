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
    
    void update(double elapsed) {
        ground->update(elapsed);
        for (int i = 0; i < poppies.size(); ++i)
            poppies[i]->update(elapsed);
        for (int i = 0; i < pots.size(); ++i)
            pots[i]->update(elapsed);
    }
    
    void setupHUD()
    {
        // The code snippet below is used to output text
        // create a font resource
        ResourcePtr resourceText = OgreFramework::getSingletonPtr()->m_pFontMgr->create("Arial",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        resourceText->setParameter("type","truetype");
        resourceText->setParameter("source","C64_User_Mono_v1.0-STYLE.ttf");
        resourceText->setParameter("size","16");
        resourceText->setParameter("resolution","96");
        resourceText->load();
        
        // Create a panel
        OverlayContainer* panel = static_cast<OverlayContainer*>(OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "HudInterface"));
        panel->setMetricsMode(GMM_PIXELS);
        panel->setPosition(10, 10);
        panel->setDimensions(10, 10);
        
        BorderPanelOverlayElement* healthArea = static_cast<BorderPanelOverlayElement*>(
                                                                                        OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("BorderPanel", "HealthAreaBorder"));
        healthArea->setMetricsMode(GMM_RELATIVE);
        healthArea->setPosition(Util::HP_BAR_XREF - 0.01, Util::HP_BAR_YREF - 0.01);
        healthArea->setDimensions(Util::HP_BAR_WIDTH + 0.02, Util::HP_BAR_HEIGHT + 0.02);
        healthArea->setMaterialName("BaseWhite");
        
        barHP = static_cast<PanelOverlayElement*>(
                                                  OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("Panel", "HealthBar"));
        barHP->setMetricsMode(GMM_RELATIVE);
        barHP->setPosition(Util::HP_BAR_XREF, Util::HP_BAR_YREF);
        barHP->setDimensions(0.0, 0.0);
        barHP->setMaterialName("General/BaseRed");
        
        // Create text area
        label1= static_cast<TextAreaOverlayElement*>(
                                                     OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel1"));
        label1->setMetricsMode(GMM_PIXELS);
        label1->setPosition(Util::LABEL1_POSX, Util::LABEL1_POSY);
        label1->setCharHeight(26);
        label1->setFontName("Arial");
        label1->setColour(ColourValue::White);
        
        // Create text area
        label2= static_cast<TextAreaOverlayElement*>(
                                                     OgreFramework::getSingletonPtr()->m_pOverlayMgr->createOverlayElement("TextArea", "TextAreaLabel2"));
        label2->setMetricsMode(GMM_PIXELS);
        label2->setPosition(Util::LABEL2_POSX, Util::LABEL2_POSY);
        label2->setCharHeight(26);
        label2->setColour(ColourValue::White);
        label2->setFontName("Arial");
        
        // Create an overlay, and add the panel
        Overlay* overlay = OgreFramework::getSingletonPtr()->m_pOverlayMgr->create("OverlayHelloWorld");
        overlay->add2D(panel);
        panel->addChild(label1);
        panel->addChild(label2);
        panel->addChild(healthArea);
        panel->addChild(barHP);
        overlay->show();
    }
    
    ~Stage() {}
};