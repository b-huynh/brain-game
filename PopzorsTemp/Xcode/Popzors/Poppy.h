#pragma once

#include "Pch.h"
#include "Selectable.h"

class Poppy : public Selectable {
public:
	Poppy(Vector3 pos = Vector3(0,0,0), Color baseColor = Color(0,0,0,0), Color blinkColor = Color(0,0,0,0), Number blinktime = 0);
	~Poppy();
    
	void addToCollisionScene(CollisionScene * scene);
    void removeFromCollisionScene(CollisionScene * scene);
    void setId(int val);
	virtual void setColor(int r, int g, int b, int a);
	virtual void setColor(Color color);
    void setPosition(Number x, Number y, Number z);
	void setPosition(Vector3 vec);
    void setPotIdRef(int val);
    void handleCollision(Number elapsed, CollisionScene *scene, Poppy & rhs);
    
	int getType() { return Selectable::TYPE_POPPY; }
	int getId() { return popId; }
	int getPotIdRef() { return potIdRef; }
	bool hasEntity(SceneEntity * entity);

    virtual void reset();
    
    void update(Number elapsed);
private:
    int popId;
	Vector3 pos; // Does not represent actual graphical pos, but initial/reset pos
    
    int potIdRef;
    
	ScenePrimitive * body;
};
