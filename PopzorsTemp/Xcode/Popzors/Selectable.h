#pragma once

#include "Pch.h"
#include "Blinkable.h"

class Selectable : public Blinkable {
protected:
    bool selectable;
public:
    Selectable(Color baseColor = Color(0,0,0,0), Color blinkColor = Color(0,0,0,0), Number blinktime = 0)
        : Blinkable(baseColor, blinkColor, blinktime), selectable(true)
    {}
    
    Selectable(Color baseColor, Color blinkColor, Number blinktime, Number ontime, Number offtime)
    : Blinkable(baseColor, blinkColor, blinktime, ontime, offtime), selectable(true)
    {}
    
    bool isSelectable() const { return selectable; }
    void setSelectable(bool value) { selectable = value; }
    
	static const int TYPE_POPPY = 0;
	static const int TYPE_POT = 1;

	virtual int getType() const = 0;
	virtual bool hasEntity(SceneEntity * entity) = 0;
    
    virtual void reset();
    virtual void update(Number elapsed);
    
    ~Selectable() {}
};

