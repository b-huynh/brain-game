#pragma once

#include "pch.h"
#include "Stage.h"
#include "Player.h"

// The base class of a particular Pattern.
class PopzorsPattern
{
public:
    Stage stage;
protected:
    Player player;
    
    bool ready;
    
    vector<std::string> saveData;
public:
    PopzorsPattern(Screen *screen, CollisionScene *scene) : stage(screen, scene)
    {}
    
    virtual void setup() = 0; // Deals with allocating the stage screen and scene
    
    virtual void reset() // Deals with reseting variables associated to the pattern
    {
        player.reset();
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
        return static_cast<double>(player.numCorrect) / player.totalProblems;
    }
    
    virtual bool save(std::string file)
    {
        return false;
    }
    
    bool load(std::string file)
    {
        return false;
        
        /*
        reset();
        setup();
        bool result = player.loadProgress(file);
        setPattern();
        return result;
         */
    }
    
    ~PopzorsPattern() {}
};