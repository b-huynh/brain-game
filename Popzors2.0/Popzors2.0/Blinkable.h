#pragma once

#include "Pch.h"

class Blinkable
{
protected:
    Ogre::ColourValue currentColor;
	Ogre::ColourValue baseColor;
	Ogre::ColourValue blinkColor;
	double timeBlinkLength;
	double timeBlinked;
    double ontime;
    double offtime;
    bool toggle;
    double timeToggle;
    
    //Tracking Of Blinkable Objects
    static int BlinkableObjCount;
    int sceneID;
    
public:
    
    Blinkable(Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0), Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0);
    
    Blinkable(Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime, double ontime, double offtime);
    
    void activateBlink();
    virtual void setColor(int r, int g, int b, int a) = 0;
	virtual void setColor(Ogre::ColourValue color) = 0;
	void setTimeBlinkLength(double blinktime);
	void setTimeBlinkLength(double blinktime, double ontime, double offtime);
	void setBaseColor(int r, int g, int b, int a);
	void setBaseColor(Ogre::ColourValue color);
	void setBlinkColor(int r, int g, int b, int a);
	void setBlinkColor(Ogre::ColourValue color);
    void updateBlinkTime(double elapsed);
    
    bool isBlinking() const { return timeBlinked > 0; }
    bool isToggling() const { return timeToggle > 0; }
	Ogre::ColourValue getBaseColor() const { return baseColor; }
	Ogre::ColourValue getBlinkColor() const { return blinkColor; }
    
    virtual void reset();
    virtual void update(double elapsed);
    
    ~Blinkable() {}
};
