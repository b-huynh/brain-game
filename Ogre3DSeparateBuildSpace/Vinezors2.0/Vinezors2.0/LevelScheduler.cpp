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
LevelScheduler::LevelScheduler( double nBackLevelA, double nBackLevelB, double nBackLevelC, double nBackLevelD, double nBackLevelE, double currentHoldout )
: scheduleHistory(), binA(NULL), binB(NULL), binC(NULL), binD(NULL), binE(NULL), totalMarbles(0), timePlayed(0), sessionFinished(false), sessionFinishedAcknowledged(false)
{
    this->nBackLevelA = nBackLevelA;
    this->nBackLevelB = nBackLevelB;
    this->nBackLevelC = nBackLevelC;
    this->nBackLevelD = nBackLevelD;
    this->nBackLevelE = nBackLevelE;
    this->currentHoldout = currentHoldout;
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
    const int NUM_DIFFICULTIES = 3;
    
    if(!binA) binA = new std::list<Bin>();
    if(!binB) binB = new std::list<Bin>();
    if(!binC) binC = new std::list<Bin>();
    if(!binD) binD = new std::list<Bin>();
    if(!binE) binE = new std::list<Bin>();

    for(int i = 0; i < NUM_DIFFICULTIES; ++i)
    {
        switch ( (StageDifficulty)i ) {
            case DIFFICULTY_EASY:
                binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_EASY, DURATION_NORMAL, false));
                binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_EASY, DURATION_SHORT, false));
                binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY, DURATION_SHORT, false));
                binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_EASY, DURATION_SHORT, false));
                binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_EASY, DURATION_SHORT, false));
                break;
            case DIFFICULTY_NORMAL:                                                                     // DURRATION      DIFFICULTY     HOLDOUT
                                                                                                        // RECESS:
                binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false));     // normal           normal          N
                binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, true));      // normal           normal          Y
                binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false));     // normal           normal          N
                
                                                                                                        // SHAPE SOUND:
                binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false));     // normal           normal          N
                binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, true));      // normal           normal          Y
                binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_NORMAL, DURATION_LONG, false));       // long             normal          N
                
                                                                                                        // SHAPE SOUND:
                binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_NORMAL, DURATION_SHORT, false));       // short            normal          N
                binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_NORMAL, DURATION_NORMAL, false));      // normal           normal          Y
                binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_NORMAL, DURATION_LONG, false));        // long             normal          N
                
                                                                                                        // SHAPE SOUND:
                binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_NORMAL, DURATION_NORMAL, false));         // normal           normal          N
                binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_NORMAL, DURATION_NORMAL, true));          // normal           normal          Y
                binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_NORMAL, DURATION_LONG, false));           // long             normal          N
                
                                                                                                        // SHAPE SOUND:
                binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_NORMAL, DURATION_NORMAL, false));         // normal           normal          N
                binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_NORMAL, DURATION_NORMAL, true));          // normal           normal          Y
                binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_NORMAL, DURATION_LONG, false));           // long             normal          N
                break;
            case DIFFICULTY_HARD:
                binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_HARD, DURATION_NORMAL, false));
                binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_HARD, DURATION_LONG, false));
                binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_HARD, DURATION_LONG, false));
                binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_HARD, DURATION_LONG, false));
                binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_HARD, DURATION_LONG, false));
                break;
            default:
                break;
        }
    }
    /*
    binA->push_back(Bin(PHASE_COLOR_SOUND, (StageDifficulty)(rand() % NUM_DIFFICULTIES), true));
    binB->push_back(Bin(PHASE_SHAPE_SOUND, (StageDifficulty)(rand() % NUM_DIFFICULTIES), true));
    binC->push_back(Bin(PHASE_SOUND_ONLY, (StageDifficulty)(rand() % NUM_DIFFICULTIES), false));
    binD->push_back(Bin(PHASE_HOLDOUT, (StageDifficulty)(rand() % NUM_DIFFICULTIES), true));
     */
    
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
    if(binA && binB && binC && binD && binE) {
        totalMarbles = binA->size() + binB->size() + binC->size() + binD->size() + binE->size();
    }
    if (totalMarbles <= 3) {
        populateBins();
        totalMarbles = binA->size() + binB->size() + binC->size() + binD->size() + binE->size();
    }
    
    // =========================================================================
    // Debug output - should be commented out on final release
        cout << "\n------------------------------------------------" << endl;
        cout << "Total Bin Marbles: " << totalMarbles << endl;
        cout << "------------------------------------------------" << endl;
    // =========================================================================
    
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

// can keep a linear list of marbles to randomly pick from instead
void LevelScheduler::pickRandomMarble( std::vector<Bin>& choices )
{
    std::list<Bin>& binRef = *pickRandomBin();
    std::list<Bin>::iterator binIt = binRef.begin();
    int binIndex = rand_num(0, binRef.size() - 1);
    for ( int i = 0; i < binIndex; ++i, ++binIt );
    
    for ( int i = 0; i < choices.size(); ++i )
    {
        if ( *binIt == choices[i] )
        {
            
            // =========================================================================
            // Debug output - should be commented out on final release
                cout << "Need to pick extra bin & marble due to duplicate" << endl;
            // =========================================================================
            
            pickRandomMarble(choices);
            return;
        }
    }
    choices.push_back(*binIt);
    
    // =========================================================================
    // Debug output - should be commented out on final release
        for(int i = 0; i < choices.size(); ++i)
        {
            cout << "Choice " << i << ": " << choices[i].phaseX << ", " << choices[i].difficultyX << endl;
        }
    // =========================================================================
    
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
    
    std::vector<Bin> choices;
    std::vector< std::pair<StageRequest, PlayerProgress> > result;
    std::pair<StageRequest, PlayerProgress> node;
    LevelPhase phase;
    StageDifficulty difficulty;
    StageDuration duration;
    bool holdout;
    double playerSkill;
    int nBackRounded;
    
    for(int i = 0; i < 3; ++i)
    {
        pickRandomMarble( choices );
        phase = choices[i].phaseX;
        difficulty = choices[i].difficultyX;
        duration = choices[i].durationX;
        holdout = choices[i].holdout;
        
//        cout << "\n\n================================\n\nPhase: " << phase << endl;
//        cout << "Difficulty: " << difficulty << endl;
        
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
        
        if(nBackRounded < 1) nBackRounded = 1;
        if (holdout)
            node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, currentHoldout);
        else
            node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 0.0);
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
    out << sch.nBackLevelA << " " << sch.nBackLevelB << " " << sch.nBackLevelC << " " << sch.nBackLevelD << " " << sch.nBackLevelE << " " << sch.currentHoldout;
    return out;
}

//________________________________________________________________________________________

std::istream& operator>>(std::istream& in, LevelScheduler& sch)
{
    std::cout << "Loading Scheduler\n";
    in >> sch.nBackLevelA >> sch.nBackLevelB >> sch.nBackLevelC >> sch.nBackLevelD >> sch.nBackLevelE >> sch.currentHoldout;
    return in;
}

//________________________________________________________________________________________
