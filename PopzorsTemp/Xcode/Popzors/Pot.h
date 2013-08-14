#pragma once

#include "Pch.h"
#include "Selectable.h"
#include "Poppy.h"

class Pot : public Selectable {
public:
	Pot(Vector3 pos = Vector3(0,0,0), Number radius = 0, Color baseColor = Color(0,0,0,0),
        Color blinkColor = Color(0,0,0,0), Number blinktime = 0, const std::string & soundFile = "");
    ~Pot();
    
	virtual int getType() const { return Selectable::TYPE_POT; }
	virtual bool hasEntity(SceneEntity * entity);
    
    void setId(int val);
	virtual void setColor(int r, int g, int b, int a);
	virtual void setColor(Color color);
	void setPosition(Number x, Number y, Number z);
	void setPosition(Vector3 vec);
    void setSound(const std::string & soundFile);
    void handleCollision(Number elapsed, CollisionScene *scene, Pot* rhs);
    
    int getId() const { return potId; }
	Vector3 getPosition() const { return body->getPosition(); }
    Number getRadius() const { return radius; }
    
	void addToCollisionScene(CollisionScene * scene);
    void removeFromCollisionScene(CollisionScene * scene);
    
    void playSound();
    
    virtual void reset();
    virtual void update(Number elapsed);
    
private:
    int potId;
    Number radius;
    
	Vector3 pos;
	ScenePrimitive * body;
    Sound * sound;
};