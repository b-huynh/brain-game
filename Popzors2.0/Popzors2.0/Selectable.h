#pragma once

#include "Pch.h"
#include "Object.h"

class Selectable : public Object {
protected:
    bool selectable;
public:
    Selectable(int type, Vector3 pos, Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0), Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0)
    : Object(type, pos, baseColor, blinkColor, blinktime), selectable(true)
    {}
    
    Selectable(int type, Vector3 pos, Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime, double ontime, double offtime)
    : Object(type, pos, baseColor, blinkColor, blinktime, ontime, offtime), selectable(true)
    {}
    
    bool isSelectable() const { return selectable; }
    void setSelectable(bool value) { selectable = value; }
    
    virtual void reset();
    virtual void update(double elapsed);
    
    ~Selectable() {}
};

