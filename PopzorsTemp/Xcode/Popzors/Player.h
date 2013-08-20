#pragma once

#include "Pch.h"
#include <vector>

static const Number PLAYER_SUCCESS = 1.0;
static const Number PLAYER_FAILURE = 0.0;

class Player {
public:
    
    struct PlayerResult {
        int playerLevel;
        int numCorrect;
        int totalProblems;
        Number accuracy;
        
        PlayerResult() {}
    };
    
    static const int levelUpCeiling = 3;
    
    //Player stats
    int numCorrect;
    int totalProblems;
    
    int level;
    int numConsecutiveSuccess;
    
    vector<PlayerResult> progression;
    
    Player();
    
    void reset();
    void resetLevel();
    
    //Update players level based on accu racy
    void updateLevel(Number accuracy);
    
    //Save/load progress from file
    bool saveProgress(std::string file);
    bool loadProgress(std::string file);
    
};
