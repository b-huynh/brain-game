//
//  PlayerProgress.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/29/14.
//
//

#include "PlayerProgress.h"

PlayerProgress::PlayerProgress()
    : rating(-1), score(0.0), time(0.0), numCorrect(0), numWrong(0), numSafe(0), numMissed(0), startSpeed(-1), exitSpeed(-1), initSpeedSetting(-1),
    nbackDelta(0.0), level(NULL)
{}

// returns true if the new rating overwrites the old rating
bool PlayerProgress::setRating(int nrating)
{
    if (nrating >= rating)
    {
        rating = nrating;
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& outfile, const PlayerProgress & prg)
{
    outfile << prg.rating << " "
            << prg.score << " "
            << prg.time << " "
            << prg.numCorrect << " "
            << prg.numWrong << " "
            << prg.numSafe << " "
            << prg.numMissed << " "
            << prg.startSpeed << " "
            << prg.exitSpeed << " "
            << prg.initSpeedSetting;
    return outfile;
}

std::istream& operator>>(std::istream& infile, PlayerProgress & prg)
{
    infile >> prg.rating
        >> prg.score
        >> prg.time
        >> prg.numCorrect
        >> prg.numWrong
        >> prg.numSafe
        >> prg.numMissed
        >> prg.startSpeed
        >> prg.exitSpeed
        >> prg.initSpeedSetting;
    return infile;
}