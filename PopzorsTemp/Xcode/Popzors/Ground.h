#pragma once

#include "Pch.h"
#include "Blinkable.h"

class Ground : public Blinkable
{
public:
    Ground(Vector3 pos = Vector3(0,0,0), Color baseColor = Color(0,0,0,0),
           Color blinkColor = Color(0,0,0,0), Number blinktime = 0);
  
    virtual void setColor(int r, int g, int b, int a);
    virtual void setColor(Color color);
    
	void addToCollisionScene(CollisionScene * scene);
    
    virtual void reset();
    virtual void update(Number elapsed);
private:
    Vector3 pos;
    
    ScenePrimitive *body;
};