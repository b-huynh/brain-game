#pragma once

#include "Pch.h"
#include "Pattern.h"

class BaselinePattern : public PopzorsPattern
{
public:
    BaselinePattern(Stage* stage, Player* player);
    
    virtual void setup();
    virtual void reset();
    virtual void setPattern();
    virtual bool isFinished() const;
    virtual void processSelect(ClickedResult res);
    virtual void update(double elapsed);
    
    // Helper Functions
    void updatePlayerChoice(Poppy* poppy, Pot* pot);
    void updatePoppyBlinks(double elapsed);
    void updateScoreAndFeedback();
    
    //Utilities for Saving
    std::string getFinishedStageData();
    bool save(std::string file);
    
    //Input event functions
    virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    
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
    
    //Poppy Data
    std::vector<poppyData> pData;
};
