#pragma once

#include "Pch.h"
#include "Selectable.h"
#include "Poppy.h"

class Pot : public Selectable {
public:
	Pot(Vector3 pos = Vector3(0,0,0), double radius = 0, Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0),
        Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0, const std::string & soundFile = "");
    ~Pot();
    
    void setId(int val);
    void setSound(const std::string & soundFile);
    void handleCollision(double elapsed, Pot* rhs);
    
    int getId() const { return potId; }
    double getRadius() const { return radius; }
    
    void playSound() const;
    
    virtual void setMaterialByColor(Ogre::ColourValue color);
    virtual void addToScene();
    virtual void removeFromScene();
    virtual void reset();
    virtual void update(double elapsed);
    
private:
    int potId;
    double radius;
    //Sound * sound;
};