//
//  PlayerLevel.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/29/14.
//
//

#ifndef __Vinezors2_0__PlayerLevel__
#define __Vinezors2_0__PlayerLevel__

#include "Score.h"

struct PlayerLevel {
    int sessionID;
    
    // Reps
    int set1Rep;
    int set2Rep;
    int set3Rep;
    
    // Ranks
    int set1; // Color/Sound
    int set2; // Shape/Sound
    int set3; // Sound only
    
    // New
    int set1Notify; // Color/Sound
    int set2Notify; // Shape/Sound
    int set3Notify; // Sound only
    
    int navigation;
    
    int minSpeed;
    int averageSpeed;
    int maxSpeed;
    
    int runSpeed1;
    int runSpeed2;
    int runSpeed3;
    
    ScoreSet navigationScores;
    ScoreSet speedScores;
    
    int getMasteredNBack() const;
    
    int getHighestNBack() const;
    
    int getNavLimit() const;
    
    PlayerLevel();
    
    void calculateNavigation();
    void calculateSpeed();
    std::string getCurrentStats() const;
};

#endif /* defined(__Vinezors2_0__PlayerLevel__) */
