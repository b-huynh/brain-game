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

// Tracks player performance on specific levels
struct PlayerProgress
{
    // Best rating for the player on this level
    int rating; // 0 = N, 1 = *, 2 = **, 3 = ***
    int score;  // score rating
    float time; // time left on tunnel
    
    // Toggle-back collection statistics
    int collection0Correct;
    int collection1Correct;
    int collection2Correct;
    int collection3Correct;
    int collection0Wrong;
    int collection1Wrong;
    int collection2Wrong;
    int collection3Wrong;
    
    // returns true if the new rating overwrites the old rating
    bool setRating(int nrating);
    
    PlayerProgress();
};
std::ostream& operator<<(std::ostream& outfile, const PlayerProgress & prg);
std::istream& operator>>(std::istream& infile, PlayerProgress & prg);

#endif /* defined(__Vinezors2_0__PlayerProgress__) */
