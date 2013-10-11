//
//  Object.h
//  Popzors2.0
//
//  Created by Calvin Phung on 10/10/13.
//
//
#pragma once

#include "Pch.h"

class Object
{
protected:
    int type;
    SceneNode* mainNode;
    Entity* body;
    
	Vector3 pos; // Does not represent actual graphical pos, but initial/reset pos
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
	static const int TYPE_NONE = 0;
	static const int TYPE_POPPY = 1;
	static const int TYPE_POT = 2;
	static const int TYPE_GROUND = 3;
    
    Object();
    Object(int type, Vector3 pos, Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0), Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0);
    Object(int type, Vector3 pos, Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime, double ontime, double offtime);
    
    int getType() const;
    SceneNode* getNode() const;
    Entity* getBody() const;
    Vector3 getPosition() const;
    
    void move(const Vector3 & dValue);
    bool hasEntity(Entity* entity);
    
    void setPosition(double x, double y, double z);
    void setPosition(Vector3 vec);
	void setColor(int r, int g, int b, int a);
    void setColor(Ogre::ColourValue color);
	void setBaseColor(int r, int g, int b, int a);
	void setBaseColor(Ogre::ColourValue color);
	void setBlinkColor(int r, int g, int b, int a);
	void setBlinkColor(Ogre::ColourValue color);
    void activateBlink();
	void setTimeBlinkLength(double blinktime);
	void setTimeBlinkLength(double blinktime, double ontime, double offtime);
    void updateBlinkTime(double elapsed);
    
    bool isBlinking() const { return timeBlinked > 0; }
    bool isToggling() const { return timeToggle > 0; }
	Ogre::ColourValue getBaseColor() const { return baseColor; }
	Ogre::ColourValue getBlinkColor() const { return blinkColor; }
    
    virtual void setMaterialByColor(Ogre::ColourValue color) = 0;
    virtual void addToScene() = 0;
    virtual void removeFromScene() = 0;
    virtual void reset();
    virtual void update(double elapsed);
    
    ~Object();
};

