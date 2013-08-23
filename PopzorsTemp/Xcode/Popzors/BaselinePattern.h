#pragma once

#include "Pch.h"
#include "Pattern.h"

class BaselinePattern : public PopzorsPattern
{
public:
    BaselinePattern(Screen *screen, CollisionScene *scene, unsigned seed, int width, int height);
    
    virtual void setup();
    virtual void reset();
    virtual void setPattern();
    virtual bool isFinished() const;
    virtual void processSelect(ClickedResult res);
    virtual void update(Number elapsed);
    
    // Helper Functions
    void updatePlayerChoice(Poppy* poppy, Pot* pot);
    void updatePoppyBlinks(Number elapsed);
    
    //Utilities for Saving
    std::string getFinishedStageData();
    bool save(std::string file);
    
    //Poppy Save Data
    struct poppyData {
        int playerLevel;
        int stageId;
        int poppyID;
        
        int poppyType;
        Number poppyFlashTime;
        
        int binPlaceIn;
        Number binPlaceTime;
    };
    
    ~BaselinePattern() {}
private:
    int screenWidth;
    int screenHeight;
    
	bool signaled;
	Number signalStart;
    Number signalLength;
    Number timer;
    
    int numImportantPoppies;
    int numDistractingPoppies;
    double poppyRadius;
    
    bool backwardsOrder;
	int blinkPoppyIndex;
    int playerPoppyIndex;
    
    Number totalElapsed;
    int score;
    
    Selectable* selected;
    
    //Poppy Data
    vector<poppyData> pData;
};
