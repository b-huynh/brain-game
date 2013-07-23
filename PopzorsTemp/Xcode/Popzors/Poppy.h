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
    void setMoving(bool val);
    void setDest(Vector3 val);
    void handleCollision(Number elapsed, CollisionScene *scene, Poppy* rhs);
    
	virtual int getType() const { return Selectable::TYPE_POPPY; }
	virtual bool hasEntity(SceneEntity * entity);
    
	int getId() const { return popId; }
    bool getMoving() const { return moving; }
    Vector3 getDest() const { return dest; }
    Vector3 getPosition() const { return body->getPosition(); }
	int getPotIdRef() const { return potIdRef; }

    virtual void reset();
    
    void activateJump();
    void deactivateJump();
    void jump(Number elapsed);
    
    void update(Number elapsed);
private:
    bool jumping;
    Number timeJumped;
    
    int popId;
	Vector3 pos; // Does not represent actual graphical pos, but initial/reset pos
    
    int potIdRef;
    
    bool moving;
    Vector3 dest;
    
	ScenePrimitive * body;
};
