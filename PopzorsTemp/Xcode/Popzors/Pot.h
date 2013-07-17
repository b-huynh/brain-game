#pragma once

#include "Pch.h"
#include "Selectable.h"

class Pot : public Selectable {
public: 
	Pot(Vector3 pos = Vector3(0,0,0), Color baseColor = Color(0,0,0,0),
        Color blinkColor = Color(0,0,0,0), Number blinktime = 0, const String & soundFile = "");

	int getType() { return Selectable::TYPE_POT; }
	bool hasEntity(SceneEntity * entity);
    
    void setId(int val);
	virtual void setColor(int r, int g, int b, int a);
	virtual void setColor(Color color);
	void setPosition(Number x, Number y, Number z);
	void setPosition(Vector3 vec);
    
    int getId() { return potId; }
	Vector3 getPosition() { return pos; }
    
	void addToCollisionScene(CollisionScene * scene);
    
    void playSound();
    
    virtual void reset();
    virtual void update(Number elapsed);
private:
    int potId;
    
	Vector3 pos;
	ScenePrimitive * body;
    Sound * sound;
};