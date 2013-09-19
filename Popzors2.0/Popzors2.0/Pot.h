#pragma once

#include "Pch.h"
#include "Selectable.h"
#include "Poppy.h"

class Pot : public Selectable {
public:
	Pot(Vector3 pos = Vector3(0,0,0), double radius = 0, Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0),
        Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0, const std::string & soundFile = "");
    ~Pot();
    
	virtual int getType() const { return Selectable::TYPE_POT; }
	virtual bool hasEntity(Entity * entity);
    
    void setId(int val);
	virtual void setColor(int r, int g, int b, int a);
	virtual void setColor(Ogre::ColourValue color);
	void setPosition(double x, double y, double z);
	void setPosition(Vector3 vec);
    void setSound(const std::string & soundFile);
    //void handleCollision(double elapsed, CollisionScene *scene, Pot* rhs);
    
    int getId() const { return potId; }
	Vector3 getPosition() const { return potNode->getPosition(); }
    double getRadius() const { return radius; }
    
    void addToScene();
    void removeFromScene();
    
    void playSound();
    
    virtual void reset();
    virtual void update(double elapsed);
    
private:
    int potId;
    double radius;
    
	Vector3 pos;
	//ScenePrimitive * body;
    //Sound * sound;
    
    Entity* body;
    SceneNode* potNode;
};