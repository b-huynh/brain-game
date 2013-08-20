#pragma once

#include "Pch.h"
#include "Pattern.h"

class PoppyPattern : public PopzorsPattern
{
public:
    PoppyPattern(Screen *screen, CollisionScene *scene);
    
    virtual void setup();
    virtual void reset();
    virtual void setPattern();
    virtual bool isFinished() const;
    virtual void processSelect(ClickedResult res);
    virtual void update(Number elapsed);
    
    // Helper Functions
    void updatePlayerChoice(Poppy* poppy, Pot* pot);
    void updatePoppyBlinks(Number elapsed);
    
    ~PoppyPattern() {}
private:
	bool signaled;
	Number signalStart;
    Number signalLength;
    Number timer;
    
    int numImportantPoppies;
    int numDistractingPoppies;

    bool backwardsOrder;
	int blinkPoppyIndex;
    int playerPoppyIndex;
    
    Selectable* selected;
};
