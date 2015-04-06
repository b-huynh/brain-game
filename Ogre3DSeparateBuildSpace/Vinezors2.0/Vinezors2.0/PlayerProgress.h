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
    
    // REMEMBER time speeds up as speed increases... need a new way to measure time
    // ctime??
    float time; // time left on tunnel
    
    int numCorrect;
    int numWrong;
    int numSafe;
    int numMissed;
    int numPickups;
    int numCorrectH;
    int numWrongH;
    int numSafeH;
    int numMissedH;
    
    int startSpeed;
    int exitSpeed;
    
    int initSpeedSetting;
    
    float accuracy;
    float nBackDelta;   // Delta change for experience
    float nBackReturn;  // Offset delta for holdouts
    float nBackSkill;   // Memory Experience
    float nBackOffset;  // Offset score for holdouts
    float nBackResult;  // Result from memory experience added with delta
    float nBackNoffset;  // Result from memory offset added with delta
    
    // returns true if the new rating overwrites the old rating
    bool setRating(int nrating);
    
    PlayerProgress();
};
std::ostream& operator<<(std::ostream& outfile, const PlayerProgress & prg);
std::istream& operator>>(std::istream& infile, PlayerProgress & prg);

#endif /* defined(__Vinezors2_0__PlayerProgress__) */
