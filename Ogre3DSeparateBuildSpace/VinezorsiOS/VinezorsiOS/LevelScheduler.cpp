//
//  LevelScheduler.cpp
//  Vinezors2.0
//
//  Created by Darrin Lin on 8/4/14.
//
//
//________________________________________________________________________________________
#include <iostream>
#include "LevelScheduler.h"

using namespace std;

#define SCHEDULE_SIZE 5     // Number of levels in a schedule
#define fileName "nBackLevels.txt"
//________________________________________________________________________________________

/**
 Creates a new scheduler
 */
LevelScheduler::LevelScheduler( double nBackLevelA, double nBackLevelB, double nBackLevelC, double nBackLevelD, double nBackLevelE )
: schedule(), scheduleIt(NULL), scheduleHistory()
{
    // Reads save file and reassigns nBackLevelX with saved values
    if( !readNBL() )
    {
        this->nBackLevelA = nBackLevelA;
        this->nBackLevelB = nBackLevelB;
        this->nBackLevelC = nBackLevelC;
        this->nBackLevelD = nBackLevelD;
        this->nBackLevelE = nBackLevelE;
    }
}
//________________________________________________________________________________________


void LevelScheduler::saveNBL()
{
    ofstream outfile(fileName);
    
    if(outfile)
    {
        outfile << nBackLevelA << endl
        << nBackLevelB << endl
        << nBackLevelC << endl
        << nBackLevelD << endl
        << nBackLevelE << endl;
    }
}
//________________________________________________________________________________________


bool LevelScheduler::readNBL()
{
    ifstream infile(fileName);
    
    if(infile)
    {
        infile >> nBackLevelA >> nBackLevelB >> nBackLevelC >> nBackLevelD >> nBackLevelE;
        infile.close();
        return true;
    }
    else
    {
        infile.close();
        return false;
    }
}
//________________________________________________________________________________________

/**
 Creates a new scheduler with a predefined schedule.
 Shuffles the schedule to be randomized.
 
 @param newSchedule - the vector containing the predefined schedule
 */
//LevelScheduler::LevelScheduler( std::vector< std::pair<StageRequest, PlayerProgress> > newSchedule )
//: schedule(), scheduleIt(NULL)
//{
//    // Shuffle the newSchedule
//    // std::random_shuffle(newSchedule.begin(), newSchedule.end());
//
//    // Store it in the schedule
//    //std::cout << "new sched size: " << newSchedule.size() << std::endl;
//    for ( std::vector< std::pair<StageRequest, PlayerProgress> >::iterator it = newSchedule.begin(); it != newSchedule.end(); ++it )
//    {
//        schedule.push_back( *it );
//        // std::cout << "Nav Level Size: " << it->first.navLevels.size() << std::endl;
//    }
//
//    // Set the iterator to point to the first level in the scheduler
//    scheduleIt = schedule.begin();
//}
//________________________________________________________________________________________

/**
 Gets a random level from the defined level set
 Does not return levels that is already in the schedule
 
 @param levels - a set of all hard-coded levels
 @return StageRequest - the random level that was obtained
 */
//StageRequest LevelScheduler::getRandomLevel(LevelSet* levels)
//{
//    // Get a random level
//    StageRequest ret = levels->retrieveLevel( rand_num(0, 7), rand_num(0, 4) );
//
//    // If the schedule is !empty, check for duplicate
//    if( !schedule.empty() )
//    {
//        for( std::list< std::pair<StageRequest, PlayerProgress> >::iterator it = schedule.begin(); it != schedule.end(); ++it )
//        {
//            // If there is a duplicate get another rand level
//            if( ret == it->first )
//                ret = levels->retrieveLevel( rand_num(0, 7), rand_num(0, 4) );
//        }
//    }
//    return ret;
//}
//________________________________________________________________________________________

/**
 Obtains a set of random levels for the scheduler
 
 @param levels - a set of all hard-coded levels
 */
//void LevelScheduler::initializeSchedule(LevelSet* levels)
//{
// need to get player progress from previous sched...
/*
 std::pair<StageRequest, PlayerProgress> node;
 node.first = levels->retrieveLevel(highestE, 0);
 node.second = PlayerProgress();
 schedule.push_back(node);
 
 node.first = levels->retrieveLevel(highestA, 1);
 node.second = PlayerProgress();
 schedule.push_back(node);
 
 node.first = levels->retrieveLevel(highestB, 2);
 node.second = PlayerProgress();
 schedule.push_back(node);
 
 node.first = levels->retrieveLevel(highestC, 3);
 node.second = PlayerProgress();
 schedule.push_back(node);
 
 node.first = levels->retrieveLevel(highestD, 4);
 node.second = PlayerProgress();
 schedule.push_back(node);
 
 scheduleIt = schedule.begin();
 */
/*
 for (int i = 0; i < 5; ++i)
 {
 node.first = levels->retrieveLevel(0, i);
 node.second = PlayerProgress();
 schedule.push_back(node);
 }
 
 scheduleIt = schedule.begin();
 */
/*
 // old schedule initializer (random sched)
 schedule.clear();
 schedule.resize(SCHEDULE_SIZE);
 std::pair<StageRequest, PlayerProgress> node;
 
 // Populate schedule with random levels
 for ( int i = 0; i < SCHEDULE_SIZE; ++i )
 {
 node.first = getRandomLevel(levels);
 node.second = PlayerProgress();
 schedule.push_back(node);
 }
 
 // Set the iterator to point to the first level in the scheduler
 scheduleIt = schedule.begin();
 */
//}
//________________________________________________________________________________________

std::vector< std::pair<StageRequest, PlayerProgress> > LevelScheduler::generateChoices()
{
    // Modifiers for each difficulty nBack
    const double N_BACK_EASY = 0.7;
    const double N_BACK_NORMAL = 0.0;
    const double N_BACK_HARD = -0.7;
    
    cout << "current nBack Levels" << endl
    << "A: " << nBackLevelA << endl
    << "B: " << nBackLevelB << endl
    << "C: " << nBackLevelC << endl
    << "D: " << nBackLevelD << endl
    << "E: " << nBackLevelE << endl
    << "-----------------------------------" << endl;
    
    
    std::vector< std::pair<StageRequest, PlayerProgress> > result;
    std::pair<StageRequest, PlayerProgress> node;
    LevelPhase phase;
    StageDifficulty difficulty;
    int nBackRounded;
    
    for(int i = 0; i < 3; ++i)
    {
        phase = (LevelPhase)rand_num(0, 4);
        difficulty = (StageDifficulty)rand_num(0, 2);
        
        switch ( phase ) {
            case PHASE_COLLECT:
                switch (difficulty) {
                    case DIFFICULTY_EASY:
                        nBackRounded = (int)round(nBackLevelE + N_BACK_EASY);
                        break;
                    case DIFFICULTY_NORMAL:
                        nBackRounded = (int)round(nBackLevelE + N_BACK_NORMAL);
                        break;
                    case DIFFICULTY_HARD:
                        nBackRounded = (int)round(nBackLevelE + N_BACK_HARD);
                        break;
                    default:
                        break;
                }
                break;
            case PHASE_COLOR_SOUND:
                switch (difficulty) {
                    case DIFFICULTY_EASY:
                        nBackRounded = (int)round(nBackLevelA + N_BACK_EASY);
                        break;
                    case DIFFICULTY_NORMAL:
                        nBackRounded = (int)round(nBackLevelA + N_BACK_NORMAL);
                        break;
                    case DIFFICULTY_HARD:
                        nBackRounded = (int)round(nBackLevelA + N_BACK_HARD);
                        break;
                    default:
                        break;
                }
                break;
            case PHASE_SHAPE_SOUND:
                switch (difficulty) {
                    case DIFFICULTY_EASY:
                        nBackRounded = (int)round(nBackLevelB + N_BACK_EASY);
                        break;
                    case DIFFICULTY_NORMAL:
                        nBackRounded = (int)round(nBackLevelB + N_BACK_NORMAL);
                        break;
                    case DIFFICULTY_HARD:
                        nBackRounded = (int)round(nBackLevelB + N_BACK_HARD);
                        break;
                    default:
                        break;
                }
                break;
            case PHASE_SOUND_ONLY:
                switch (difficulty) {
                    case DIFFICULTY_EASY:
                        nBackRounded = (int)round(nBackLevelC + N_BACK_EASY);
                        break;
                    case DIFFICULTY_NORMAL:
                        nBackRounded = (int)round(nBackLevelC + N_BACK_NORMAL);
                        break;
                    case DIFFICULTY_HARD:
                        nBackRounded = (int)round(nBackLevelC + N_BACK_HARD);
                        break;
                    default:
                        break;
                }
                break;
            case PHASE_HOLDOUT:
                switch (difficulty) {
                    case DIFFICULTY_EASY:
                        nBackRounded = (int)round(nBackLevelD + N_BACK_EASY);
                        break;
                    case DIFFICULTY_NORMAL:
                        nBackRounded = (int)round(nBackLevelD + N_BACK_NORMAL);
                        break;
                    case DIFFICULTY_HARD:
                        nBackRounded = (int)round(nBackLevelD + N_BACK_HARD);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        
        if(nBackRounded < 1) nBackRounded = 1;
        node.first.generateStageRequest(nBackRounded, phase, difficulty);
        result.push_back(node);
    }
    
    schedule.push_back(result);
    return result;
}

/**
 Simple random number generator that returns a random number between lower and upper
 
 @param lower - lower bound
 @param upper - upper bound
 @return int - the random number generated
 */
int LevelScheduler::rand_num(int lower, int upper)
{
    return rand() % (upper - lower + 1) + lower;
}
//________________________________________________________________________________________
