#pragma once

#include "OgreFramework.h"

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
    PopzorsPattern(Stage* stage, Player* player);
    ~PopzorsPattern();
    
    virtual void setup() = 0; // Deals with allocating the stage screen and scene
    virtual void reset(); // Deals with reseting variables associated to the pattern
    virtual void setPattern() = 0; // Initializes the pattern
    
    virtual bool isFinished() const = 0; // Determines whether the problem is finished
    virtual void processSelect(ClickedResult res) = 0; // Process input
    virtual void update(double elapsed) = 0; // Runs periodically
    
    bool isReady() const;
    
    std::string getName() const;
    void setName(const std::string & value);
    
    double getPlayerCorrectness() const;
    
    virtual bool save(std::string file);
    virtual bool load(std::string file);
    
    //OIS Input Event functions
    virtual bool keyPressed(const OIS::KeyEvent &keyEventRef);
	virtual bool keyReleased(const OIS::KeyEvent &keyEventRef);
    
    virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
};