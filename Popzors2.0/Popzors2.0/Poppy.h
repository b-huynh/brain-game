#pragma once

#include "Pch.h"
#include "Selectable.h"

class Poppy : public Selectable {
public:
	Poppy(Vector3 pos = Vector3(0,0,0), Ogre::ColourValue baseColor = Ogre::ColourValue(0,0,0,0), Ogre::ColourValue blinkColor = Ogre::ColourValue(0,0,0,0), double blinktime = 0, double radius = POPPY_RADIUS);
	~Poppy();
    
    void setId(int val);
    void setPotIdRef(int val);
    void setMoving(bool val);
    void setDest(Vector3 val);
    void setMoveSpeed(double speed);
    
    void handleCollision(double elapsed, Poppy * rhs);
    
	int getId() const { return popId; }
    bool getMoving() const { return moving; }
    Vector3 getDest() const { return dest; }
	int getPotIdRef() const { return potIdRef; }
    bool isActive () { return active; }
    void setActive (bool active) { this->active = active; }

    void activateJump();
    void deactivateJump();
    void deactivateJumpAtDest(bool stop);
    void jump(double elapsed);
    
	virtual void setMaterialByColor(Ogre::ColourValue color);
    virtual void addToScene();
    virtual void removeFromScene();
    virtual void reset();
    virtual void update(double elapsed);
private:
    bool jumping;
    double timeJumped;
    
    double moveSpeed;
    
    int popId;
    
    int potIdRef;
    
    bool active;
    
    bool moving;
    Vector3 dest;
    
    bool stopJumpAtDest;
};
