#pragma once

#include "Pch.h"
#include "Pattern.h"

class BaselinePattern : public PopzorsPattern
{
public:
    BaselinePattern(unsigned seed, int width, int height);
    
    virtual void setup();
    virtual void reset();
    virtual void setPattern();
    virtual bool isFinished() const;
    virtual void processSelect(ClickedResult res);
    virtual void update(double elapsed);
    
    // Helper Functions
    void updatePlayerChoice(Poppy* poppy, Pot* pot);
    void updatePoppyBlinks(double elapsed);
    
    //Utilities for Saving
    std::string getFinishedStageData();
    bool save(std::string file);
    
    //Poppy Save Data
    struct poppyData {
        int playerLevel;
        int stageId;
        int poppyID;
        
        int poppyType;
        double poppyFlashTime;
        
        int binPlaceIn;
        double binPlaceTime;
    };
    
    ~BaselinePattern() {}
private:
    int screenWidth;
    int screenHeight;
    
	bool signaled;
	double signalStart;
    double signalLength;
    double timer;
    
    int numImportantPoppies;
    int numDistractingPoppies;
    double poppyRadius;
    
    bool backwardsOrder;
	int blinkPoppyIndex;
    int playerPoppyIndex;
    
    double totalElapsed;
    int score;
    
    Selectable* selected;
    
    //Poppy Data
    std::vector<poppyData> pData;
};
