#pragma once

#include "Pch.h"
#include <vector>

static const double PLAYER_SUCCESS = 1.0;
static const double PLAYER_FAILURE = 0.0;

class Player {
public:
    
    struct PlayerResult {
        int playerLevel;
        int numCorrect;
        int totalProblems;
        double accuracy;
        
        PlayerResult() {}
    };
    
    static const int levelUpCeiling = 3;
    
    unsigned seed;
    std::string name;
    double totalElapsed;
    
    //Player stats
    int numCorrect;
    int totalProblems;
    
    int score; 
    int level;
    int numConsecutiveSuccess;
    
    std::vector<PlayerResult> progression;
    
    Player();
    
    void reset();
    void resetLevel();
    
    //Update players level based on accuracy
    void updateSuccess(double accuracy);
    void updateLevel(double accuracy);
    
    //Save/load progress from file
    bool saveProgress(std::string file);
    bool loadProgress(std::string file);
    
};
