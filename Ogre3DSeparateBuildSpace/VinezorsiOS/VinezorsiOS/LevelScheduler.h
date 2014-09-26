//
//  LevelScheduler.h
//  Vinezors2.0
//
//  Created by Darrin Lin on 8/4/14.
//
//
//________________________________________________________________________________________

#ifndef __Vinezors2_0__LevelScheduler__
#define __Vinezors2_0__LevelScheduler__
//________________________________________________________________________________________

#include <iostream>
#include <fstream>
#include <list>
#include <utility>
#include <vector>
#include <time.h>
#include "LevelSet.h"
#include "PlayerProgress.h"
#include "Util.h"
//________________________________________________________________________________________

struct Bin
{
    Bin(LevelPhase phaseX = PHASE_COLLECT, StageDifficulty difficultyX = DIFFICULTY_EASY)
    : phaseX(phaseX), difficultyX(difficultyX)
    {}
    
    LevelPhase phaseX;
    StageDifficulty difficultyX;
    
    bool operator==(const Bin rhs) const
    {
        if( (phaseX == rhs.phaseX) && (difficultyX == rhs.difficultyX) )
            return true;
        return false;
    }
};

struct LevelScheduler
{
    // Constructor
    LevelScheduler( double nBackLevelA = 1.0, double nBackLevelB = 1.0, double bBackLevelC = 1.0, double nBackLevelD = 1.0, double nBackLevelE = 1.0 );
    
//    std::vector< std::vector< std::pair<StageRequest, PlayerProgress> > > schedule;
//    std::vector< std::vector< std::pair<StageRequest, PlayerProgress> > >::iterator scheduleIt;
    std::vector< std::pair<StageRequest, PlayerProgress> > scheduleHistory;
    std::list<Bin>* binA;
    std::list<Bin>* binB;
    std::list<Bin>* binC;
    std::list<Bin>* binD;
    std::list<Bin>* binE;
    double nBackLevelA;
    double nBackLevelB;
    double nBackLevelC;
    double nBackLevelD;
    double nBackLevelE;
    
    // Member functions
    StageRequest getRandomLevel( LevelSet* levels );
    void initializeSchedule( LevelSet* levels );
    
    void populateBins();
    void removeBin(LevelPhase phaseX, StageDifficulty difficultyX);
    std::list<Bin>* pickRandomBin();
    std::vector< std::pair<StageRequest, PlayerProgress> > generateChoices();
    int rand_num( int lower, int upper );
    
    friend std::ostream& operator<<(std::ostream& out, const LevelScheduler& sch);
    friend std::istream& operator>>(std::istream& in, LevelScheduler& sch);
};

std::ostream& operator<<(std::ostream& out, const LevelScheduler& sch);
std::istream& operator>>(std::istream& in, LevelScheduler& sch);
//________________________________________________________________________________________

#endif /* defined(__Vinezors2_0__LevelScheduler__) */
