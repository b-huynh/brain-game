#pragma once

#include "Pch.h"
#include "Selectable.h"

class Ground : public Selectable
{
public:
    Ground(Vector3 pos = Vector3(0,0,0), Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0),
           Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0);
    ~Ground();
    
    virtual void setMaterialByColor(Ogre::ColourValue color);
    virtual void addToScene();
    virtual void removeFromScene();
};