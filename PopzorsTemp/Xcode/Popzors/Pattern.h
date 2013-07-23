#pragma once

#include "pch.h"
#include "Stage.h"

// The base class of a particular Pattern.
class PopzorsPattern
{
public:
    Stage stage;
protected:
    
    int playerNumCorrect;
    int playerTotalProblems;
    int playerLevel;
    
    bool ready;
public:
    PopzorsPattern(Screen *screen, CollisionScene *scene) : stage(screen, scene), playerNumCorrect(0), playerTotalProblems(0), playerLevel(1)
    {}
    
    virtual void setup() = 0; // Deals with allocating the stage screen and scene
    virtual void reset() // Deals with reseting variables associated to the pattern
    {
        playerNumCorrect = 0;
        playerTotalProblems = 0;
        ready = false;
        stage.clear();
    }
    virtual void setPattern() = 0; // Initializes the pattern
    virtual bool isFinished() const = 0; // Determines whether the problem is finished
    virtual void processSelect(ClickedResult res) = 0; // Process input
    virtual void update(Number elapsed) = 0; // Runs periodically
    
    bool isReady() const {
        return ready;
    }
    
    double getPlayerCorrectness() const
    {
        return static_cast<double>(playerNumCorrect) / playerTotalProblems;
    }
    
    
    ~PopzorsPattern() {}
};