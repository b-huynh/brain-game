#pragma once

#include "Pch.h"
#include "Selectable.h"

class Ground : public Selectable
{
public:
    Ground(Vector3 pos = Vector3(0,0,0), Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0),
           Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0);
    ~Ground();
  
    virtual void setColor(int r, int g, int b, int a);
    virtual void setColor(Ogre::ColourValue color);
    
    void addToScene();
    void removeFromScene();
    
	virtual int getType() const { return Selectable::TYPE_GROUND; }
    virtual bool hasEntity(Entity* entity);
    
    virtual void reset();
    virtual void update(double elapsed);
private:
    Vector3 pos;
    
    SceneNode* groundNode;
    Entity* body;
};