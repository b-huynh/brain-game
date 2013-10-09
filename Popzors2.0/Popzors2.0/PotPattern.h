#pragma once

#include "Pch.h"
#include "Pattern.h"

class PotPattern : public PopzorsPattern
{
public:
    PotPattern(Stage* stage, Player* player);
    
    virtual void setup();
    virtual void reset();
    virtual void setPattern();
    virtual bool isFinished() const;
    virtual void processSelect(ClickedResult res);
    virtual void update(double elapsed);
    
    // Helper Functions
    void updatePlayerChoice(Poppy* poppy, Pot* pot);
    void updatePotBlinks(double elapsed);
    void addPoppy();
    
    //Input event functions
    virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    
    ~PotPattern() {}
private:
	bool signaled;
	double signalStart;
    double signalLength;
    double timer;
    
    int stages;
    int potsPerStage;
    int stageIndex;
    std::vector<Pot*> potsLeft;
    
    int playerNumAnswers;
    
	int blinkPotIndex;
    
    double spawnPoppyTimer;
    int usefulPotIndex;
    
    Selectable* selected;
};
