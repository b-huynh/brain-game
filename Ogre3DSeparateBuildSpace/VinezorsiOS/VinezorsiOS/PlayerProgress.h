//
//  PlayerProgress.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/29/14.
//
//

#ifndef __Vinezors2_0__PlayerProgress__
#define __Vinezors2_0__PlayerProgress__

#include <fstream>
#include "LevelSet.h"

// Tracks player performance on specific levels
struct PlayerProgress
{
    // Best rating for the player on this level
    int rating; // 0 = N, 1 = *, 2 = **, 3 = ***
    int score;  // score rating
    float time; // time left on tunnel
    
    int numCorrect;
    int numWrong;
    int numSafe;
    int numMissed;
    
    int startSpeed;
    int exitSpeed;
    
    int initSpeedSetting;
    
    float accuracy;
    float nbackDelta;
    StageRequest* level;
    
    // returns true if the new rating overwrites the old rating
    bool setRating(int nrating);
    
    PlayerProgress();
};
std::ostream& operator<<(std::ostream& outfile, const PlayerProgress & prg);
std::istream& operator>>(std::istream& infile, PlayerProgress & prg);

#endif /* defined(__Vinezors2_0__PlayerProgress__) */
