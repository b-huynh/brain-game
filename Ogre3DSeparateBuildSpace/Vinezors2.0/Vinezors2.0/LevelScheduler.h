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
#include <utility>
#include <vector>
#include "LevelSet.h"
#include "PlayerProgress.h"
#include "Util.h"
//________________________________________________________________________________________

struct LevelScheduler
{
    std::vector< std::vector< std::pair<StageRequest, PlayerProgress> > > schedule;
    std::vector< std::vector< std::pair<StageRequest, PlayerProgress> > >::iterator scheduleIt;
    std::vector< std::pair<StageRequest, PlayerProgress> > scheduleHistory;
    double nBackLevelA;
    double nBackLevelB;
    double nBackLevelC;
    double nBackLevelD;
    double nBackLevelE;
    
    // Constructors
    LevelScheduler( double nBackLevelA = 1.0, double nBackLevelB = 1.0, double bBackLevelC = 1.0, double nBackLevelD = 1.0, double nBackLevelE = 1.0 );
    
    // Member functions
    StageRequest getRandomLevel( LevelSet* levels );
    void initializeSchedule( LevelSet* levels );
    std::vector< std::pair<StageRequest, PlayerProgress> > generateChoices();
    int rand_num( int lower, int upper );
    
    friend std::ostream& operator<<(std::ostream& out, const LevelScheduler& sch);
    friend std::istream& operator>>(std::istream& in, LevelScheduler& sch);
};

std::ostream& operator<<(std::ostream& out, const LevelScheduler& sch);
std::istream& operator>>(std::istream& in, LevelScheduler& sch);
//________________________________________________________________________________________

#endif /* defined(__Vinezors2_0__LevelScheduler__) */
