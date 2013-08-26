#pragma once

#include "Pch.h"
#include "Pattern.h"

class HerdPattern : public PopzorsPattern
{
public:
    HerdPattern(Screen *screen, CollisionScene *scene, unsigned seed);
    
    //Inherited
    virtual void setup();
    virtual void reset();
    virtual void setPattern();
    virtual bool isFinished() const;
    virtual void processSelect(ClickedResult res);
    virtual void update(Number elapsed);
    //End Inherited
    
    // Helper Functions
    void updatePoppiesBlink(Number elapsed);
    void updatePoppiesHerdState();
    void updatePlayerChoice(Poppy * poppy, Pot * pot);
    
    ~HerdPattern() {}
private:
    
    int numPoppies;
    int numDistractors;
    int numAnswered;
    
    Number blinkLength;
    Number blinkInterval;
    Number timeSinceLastBlink;

    Selectable* selected;
};