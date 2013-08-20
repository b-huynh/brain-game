#pragma once

#include "Pch.h"
#include "Pattern.h"

class PotPattern : public PopzorsPattern
{
public:
    PotPattern(Screen *screen, CollisionScene *scene);
    
    virtual void setup();
    virtual void reset();
    virtual void setPattern();
    virtual bool isFinished() const;
    virtual void processSelect(ClickedResult res);
    virtual void update(Number elapsed);
    
    // Helper Functions
    void updatePlayerChoice(Poppy* poppy, Pot* pot);
    void updatePotBlinks(Number elapsed);
    void addPoppy();
    
    ~PotPattern() {}
private:
	bool signaled;
	Number signalStart;
    Number signalLength;
    Number timer;
    
    int stages;
    int potsPerStage;
    int stageIndex;
    vector<Pot*> potsLeft;
    
    int playerNumAnswers;
    
	int blinkPotIndex;
    
    Number spawnPoppyTimer;
    int usefulPotIndex;
    
    Selectable* selected;
};
