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
//________________________________________________________________________________________

/**
 Creates a new scheduler
 */
LevelScheduler::LevelScheduler( double nBackLevelA, double nBackLevelB, double nBackLevelC, double nBackLevelD, double nBackLevelE )
: scheduleHistory(), binA(NULL), binB(NULL), binC(NULL), binD(NULL), binE(NULL)
{
        this->nBackLevelA = 1.0f;
        this->nBackLevelB = 1.0f;
        this->nBackLevelC = 1.0f;
        this->nBackLevelD = 1.0f;
        this->nBackLevelE = 1.0f;
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

void LevelScheduler::populateBins()
{
    const int MAX_BIN_SIZE = 3;
    
    if(!binA) binA = new std::list<Bin>();
    if(!binB) binB = new std::list<Bin>();
    if(!binC) binC = new std::list<Bin>();
    if(!binD) binD = new std::list<Bin>();
    if(!binE) binE = new std::list<Bin>();

    for(int i = 0; i < MAX_BIN_SIZE; ++i)
    {
        switch ( (StageDifficulty)i ) {
            case DIFFICULTY_EASY:
                binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_EASY));
                binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_EASY));
                binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY));
                binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_EASY));
                binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_EASY));
                break;
            case DIFFICULTY_NORMAL:
                binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_NORMAL));
                binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_NORMAL));
                binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_NORMAL));
                binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_NORMAL));
                binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_NORMAL));
                break;
            case DIFFICULTY_HARD:
                binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_HARD));
                binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_HARD));
                binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_HARD));
                binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_HARD));
                binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_HARD));
                break;
            default:
                break;
        }
    }
}
//________________________________________________________________________________________


void LevelScheduler::removeBin(LevelPhase phaseX, StageDifficulty difficultyX)
{
    switch (phaseX) {
        case PHASE_COLLECT:
            binE->remove(Bin(phaseX, difficultyX));
            break;
        
        case PHASE_COLOR_SOUND:
            binA->remove(Bin(phaseX, difficultyX));
            break;
            
        case PHASE_SHAPE_SOUND:
            binB->remove(Bin(phaseX, difficultyX));
            break;
            
        case PHASE_SOUND_ONLY:
            binC->remove(Bin(phaseX, difficultyX));
            break;
            
        case PHASE_HOLDOUT:
            binD->remove(Bin(phaseX, difficultyX));
            break;
            
        default:
            break;
    }
}
//________________________________________________________________________________________



/**
 Picks a random bin and returns it. If the bin is empty, it will recursively pick another. (Poor implementation but works for now)
 
 @return - A bin that has at least 1 element.
 */
std::list<Bin>* LevelScheduler::pickRandomBin()
{
    const int MIN_BIN = 0, MAX_BIN = 4;
    int binNum = rand_num(MIN_BIN, MAX_BIN);
    enum binNums { binNumA, binNumB, binNumC, binNumD, binNumE };
    
    // Keep track of the total number of elements in the bins.
    // If there are no elements left, populate the bin with more.
    int totalBinItems = 0;
    if(binA && binB && binC && binD && binE)
    {
        totalBinItems = binA->size() + binB->size() + binC->size() + binD->size() + binE->size();
    }
    if (totalBinItems == 0) populateBins();
    
    switch (binNum) {
        case binNumA:
            if(binA->empty()) return pickRandomBin();
            return binA;
            break;
        case binNumB:
            if(binB->empty()) return pickRandomBin();
            return binB;
            break;
        case binNumC:
            if(binC->empty()) return pickRandomBin();
            return binC;
            break;
        case binNumD:
            if(binD->empty()) return pickRandomBin();
            return binD;
            break;
        case binNumE:
            if(binE->empty()) return pickRandomBin();
            return binE;
            break;
        default:
            break;
    }
}

std::vector< std::pair<StageRequest, PlayerProgress> > LevelScheduler::generateChoices()
{
    
    // Modifiers for each difficulty nBack
    const double N_BACK_EASY = 0.7;
    const double N_BACK_NORMAL = 0.0;
    const double N_BACK_HARD = -0.7;
    
    
    /* For debugging purposes */
    cout <<  "/--------------------------------\\" << endl
         <<  "|       Current nBack Levels     |" << endl
         << "\\--------------------------------/" << endl
         << "A: " << nBackLevelA << endl
         << "B: " << nBackLevelB << endl
         << "C: " << nBackLevelC << endl
         << "D: " << nBackLevelD << endl
         << "E: " << nBackLevelE << endl
         <<  "__________________________________" << endl;
    // */
    
    
    std::vector< std::pair<StageRequest, PlayerProgress> > result;
    std::pair<StageRequest, PlayerProgress> node;
    LevelPhase phase;
    StageDifficulty difficulty;
    double playerSkill;
    int nBackRounded;
    int binIndex;
    
    
    for(int i = 0; i < 3; ++i)
    {
        std::list<Bin>& binRef = *pickRandomBin();
        std::list<Bin>::iterator binIt = binRef.begin();
        binIndex = rand_num(0, binRef.size() - 1);
        for(int i = 0; i < binIndex; ++i, ++binIt);
        
        phase = binIt->phaseX;
        difficulty = binIt->difficultyX;
        cout << "\n\n================================\n\nPhase: " << phase << endl;
        cout << "Difficulty: " << difficulty << endl;
        
        switch ( phase ) {
            case PHASE_COLLECT:
                playerSkill = nBackLevelE;
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
                playerSkill = nBackLevelA;
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
                playerSkill = nBackLevelB;
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
                playerSkill = nBackLevelC;
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
                playerSkill = nBackLevelD;
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
       
        int temp = rand_num(0,1);
        
        if(nBackRounded < 1) nBackRounded = 1;
        
        /*if (temp==1) {
            switch ( phase ) {
                case PHASE_COLLECT:
                    if (latestNBDelta>0) {
                        holdout+=10;
                    }
                    break;
                case PHASE_COLOR_SOUND:
                    if (latestNBDelta>0) {
                        holdout+=10;
                    }
                    break;
                case PHASE_SHAPE_SOUND:
                    if (latestNBDelta>0) {
                        holdout+=10;
                    }
                    break;
                case PHASE_SOUND_ONLY:
                    if (latestNBDelta>0) {
                        holdout+=10;
                    }
                    break;
                case PHASE_HOLDOUT:
                    if (latestNBDelta>0) {
                        holdout+=10;
                    }
                    break;
                default:
                    break;
            }
        }
        */
        
        node.first.generateStageRequest(nBackRounded, phase, difficulty, currentHoldout);
        
        node.second.nBackSkill = playerSkill;
        // binRef.remove(*binIt); // can't remove here... until they pick
        result.push_back(node);
    }
    
    return result;
}
//________________________________________________________________________________________

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


std::ostream& operator<<(std::ostream& out, const LevelScheduler& sch)
{
    std::cout << "Saving Scheduler\n";
    out << sch.nBackLevelA << " " << sch.nBackLevelB << " " << sch.nBackLevelC << " " << sch.nBackLevelD << " " << sch.nBackLevelE;
    return out;
}

//________________________________________________________________________________________

std::istream& operator>>(std::istream& in, LevelScheduler& sch)
{
    std::cout << "Loading Scheduler\n";
    in >> sch.nBackLevelA >> sch.nBackLevelB >> sch.nBackLevelC >> sch.nBackLevelD >> sch.nBackLevelE;
    return in;
}

//________________________________________________________________________________________
