#pragma once

#include "Pch.h"
#include "Selectable.h"

class Poppy : public Selectable {
public:
	Poppy(Vector3 pos = Vector3(0,0,0), Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0), Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0, double radius = POPPY_RADIUS);
	~Poppy();
    
    void addToScene();
    void removeFromScene();
    
    void setId(int val);
	virtual void setColor(int r, int g, int b, int a);
	virtual void setColor(Ogre::ColourValue color);
    void setPosition(double x, double y, double z);
	void setPosition(Vector3 vec);
    void setPotIdRef(int val);
    void setMoving(bool val);
    void setDest(Vector3 val);
    void setMoveSpeed(double speed);
    
    void handleCollision(double elapsed, Poppy * rhs);

    virtual void move(const Vector3 & dValue);
	virtual int getType() const { return Selectable::TYPE_POPPY; }
    virtual Entity* getBody() const { return body; }
    virtual bool hasEntity(Entity* entity);
    
	int getId() const { return popId; }
    bool getMoving() const { return moving; }
    Vector3 getDest() const { return dest; }
    Vector3 getPosition() const { return poppyNode->getPosition(); }
	int getPotIdRef() const { return potIdRef; }
    bool isActive () { return active; }
    void setActive (bool active) { this->active = active; }

    virtual void reset();
    
    void activateJump();
    void deactivateJump();
    void deactivateJumpAtDest(bool stop);
    void jump(double elapsed);
    
    void update(double elapsed);
private:
    bool jumping;
    double timeJumped;
    
    double moveSpeed;
    
    int popId;
	Vector3 pos; // Does not represent actual graphical pos, but initial/reset pos
    
    int potIdRef;
    
    bool active;
    
    bool moving;
    Vector3 dest;
    
    bool stopJumpAtDest;

    SceneNode* poppyNode;
    Entity* body;
};
