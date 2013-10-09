#pragma once

#include "Pch.h"
#include "Blinkable.h"

class Selectable : public Blinkable {
protected:
    bool selectable;
public:
    Selectable(Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0), Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0)
    : Blinkable(baseColor, blinkColor, blinktime), selectable(true)
    {}
    
    Selectable(Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime, double ontime, double offtime)
    : Blinkable(baseColor, blinkColor, blinktime, ontime, offtime), selectable(true)
    {}
    
    bool isSelectable() const { return selectable; }
    void setSelectable(bool value) { selectable = value; }
    
	static const int TYPE_POPPY = 0;
	static const int TYPE_POT = 1;
	static const int TYPE_GROUND = 2;
    
    virtual void move(const Vector3 & dValue) = 0;
	virtual int getType() const = 0;
    virtual Entity* getBody() const = 0;
	virtual bool hasEntity(Entity* entity) = 0;
    
    virtual void reset();
    virtual void update(double elapsed);
    
    ~Selectable() {}
};

