#pragma once

#include "pch.h"
#include "Stage.h"
#include "Player.h"

#include <fstream>

// The base class of a particular Pattern.
class PopzorsPattern
{
public:
    Stage* stage;
    Player* player;
    
    bool ready;
    
    std::vector<std::string> saveData;
public:
    PopzorsPattern(Stage* stage, Player* player) : stage(stage), player(player), ready(false)
    {
    }
    
    virtual void setup() = 0; // Deals with allocating the stage screen and scene
    
    virtual void reset() // Deals with reseting variables associated to the pattern
    {
        player->reset();
        ready = false;
        stage->clear();
    }
    
    virtual void setPattern() = 0; // Initializes the pattern
    virtual bool isFinished() const = 0; // Determines whether the problem is finished
    virtual void processSelect(ClickedResult res) = 0; // Process input
    virtual void update(double elapsed) = 0; // Runs periodically
    
    bool isReady() const {
        return ready;
    }
    
    std::string getName() const
    {
        return player->name;
    }
    
    void setName(const std::string & value)
    {
        player->name = value;
    }
    
    double getPlayerCorrectness() const
    {
        return static_cast<double>(player->numCorrect) / player->totalProblems;
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
    
    ~PopzorsPattern() {
        reset();
    }
};