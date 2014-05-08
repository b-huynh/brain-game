//
//  PlayerProgress.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/29/14.
//
//

#include "PlayerProgress.h"

PlayerProgress::PlayerProgress() : rating(-1), score(0.0), time(0.0),
collection0Correct(0), collection1Correct(0), collection2Correct(0), collection3Correct(0),
collection0Wrong(0), collection1Wrong(0), collection2Wrong(0), collection3Wrong(0)
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
            << prg.collection0Correct << " "
            << prg.collection1Correct << " "
            << prg.collection2Correct << " "
            << prg.collection3Correct << " "
            << prg.collection0Wrong << " "
            << prg.collection1Wrong << " "
            << prg.collection2Wrong << " "
            << prg.collection3Wrong;
    return outfile;
}

std::istream& operator>>(std::istream& infile, PlayerProgress & prg)
{
    infile >> prg.score
        >> prg.time
        >> prg.collection0Correct
        >> prg.collection1Correct
        >> prg.collection2Correct
        >> prg.collection3Correct
        >> prg.collection0Wrong
        >> prg.collection1Wrong
        >> prg.collection2Wrong
        >> prg.collection3Wrong;
    return infile;
}