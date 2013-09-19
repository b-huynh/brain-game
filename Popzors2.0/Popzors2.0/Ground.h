#pragma once

#include "Pch.h"
#include "Blinkable.h"

class Ground : public Blinkable
{
public:
    Ground(Vector3 pos = Vector3(0,0,0), Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0),
           Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0);
    ~Ground();
  
    virtual void setColor(int r, int g, int b, int a);
    virtual void setColor(Ogre::ColourValue color);
    
    void addToScene();
    void removeFromScene();
    
    bool hasEntity(Entity* entity);
    
    virtual void reset();
    virtual void update(double elapsed);
private:
    Vector3 pos;
    
    SceneNode* groundNode;
    Entity* body;
};