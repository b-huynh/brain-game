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
: tutorialLevels(), scheduleHistoryA(), scheduleHistoryB(), scheduleHistoryC(), scheduleHistoryD(), scheduleHistoryE(), binA(NULL), binB(NULL), binC(NULL), binD(NULL), binE(NULL), totalMarbles(0), sessionFinished(false), sessionFinishedAcknowledged(false)
{
    this->nBackLevelA = nBackLevelA;
    this->nBackLevelB = nBackLevelB;
    this->nBackLevelC = nBackLevelC;
    this->nBackLevelD = nBackLevelD;
    this->nBackLevelE = nBackLevelE;
    this->scoreCurr = 0.0;
    this->currentHoldout = currentHoldout;
    this->speedA = 15.0f;
    this->speedB = 15.0f;
    this->speedC = 15.0f;
    this->speedD = 15.0f;
    this->speedE = 15.0f;
    this->firstTimeA = true;
    this->firstTimeB = true;
    this->firstTimeC = true;
    this->firstTimeD = true;
    this->firstTimeE = true;
    this->playCount = 0;
    this->holdoutOffsetA = 0.0f;
    this->holdoutOffsetB = 0.0f;
    this->holdoutOffsetD = 0.0f;
    initTutorialLevels(); // Also called after loading the scheduler
}
//________________________________________________________________________________________


// Initializes tutorials which will be played when the player has just started the scheduler
// and hasn't seen instruction on how to navigate and how to play
void LevelScheduler::initTutorialLevels()
{
    StageRequest level;
    std::pair<StageRequest, PlayerProgress> ret;
    
    // Tutorial levels have a stageNo of -1 to indicate they do not count towards level score
    
    // A recess for all navigation
    level = StageRequest();
    level.stageNo = -1;
    level.nback = 0;
    level.stageTime = 100.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    level.phaseX = PHASE_COLLECT;
    level.difficultyX = DIFFICULTY_EASY;
    level.durationX = DURATION_NORMAL;
    ret = std::pair<StageRequest, PlayerProgress>();
    ret.first = level;
    ret.second.nBackSkill = nBackLevelE;
    tutorialLevels.push_back(ret);
    
    // All-signal 1-Back
    level = StageRequest();
    level.stageNo = -1;
    level.nback = 1;
    level.stageTime = 100.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    for (int i = 0; i < 8; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(1));
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    level.phaseX = PHASE_ALL_SIGNAL;
    level.difficultyX = DIFFICULTY_EASY;
    level.durationX = DURATION_NORMAL;
    ret = std::pair<StageRequest, PlayerProgress>();
    ret.first = level;
    ret.second.nBackSkill = nBackLevelD;
    tutorialLevels.push_back(ret);
}
//________________________________________________________________________________________

void setHoldoutToCandidates(std::vector<Bin*> & candidates, int num)
{
    while (num > 0 && candidates.size() > 0)
    {
        int rindex = rand() % candidates.size();
        candidates[rindex]->holdout = true;
        candidates[rindex] = candidates[candidates.size() - 1];
        candidates.pop_back();
        num--;
    }
}

void addToHoldoutCandidates(std::vector<Bin*> & candidates, std::list<Bin>* v)
{
    for (std::list<Bin>::iterator it = v->begin(); it != v->end(); ++it)
    {
        if (it->durationX != DURATION_SHORT) {
            candidates.push_back(&(*it));
        }
    }
}

void LevelScheduler::populateBins()
{
    // Modifiers for each difficulty nBack
    const double N_BACK_EASY = 0.7;
    const double N_BACK_NORMAL = 0.0;
    const double N_BACK_HARD = -0.7;
    
    const int NUM_DIFFICULTIES = 3;
    
    if(binA) delete binA;
    binA = new std::list<Bin>();
    if(binB) delete binB;
    binB = new std::list<Bin>();
    if(binC) delete binC;
    binC = new std::list<Bin>();
    if (binD) delete binD;
    binD = new std::list<Bin>();
    if(binE) binE = new std::list<Bin>();
    binE = new std::list<Bin>();
    
    /////////////////////
    // Easy Difficulty //
    /////////////////////
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    
    ///////////////////////
    // Normal Difficulty //
    ///////////////////////
    // COLOR SOUND:
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));     // normal           normal          N
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_EASY, DURATION_NORMAL, false, N_BACK_NORMAL));      // normal           normal          Y
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_EASY, DURATION_LONG, false, N_BACK_NORMAL));       // long             normal          N
    
    // SHAPE SOUND:
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));     // normal           normal          N
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_EASY, DURATION_NORMAL, false, N_BACK_NORMAL));      // normal           normal          Y
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_EASY, DURATION_LONG, false, N_BACK_NORMAL));       // long             normal          N
    
    // SOUND ONLY:
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));      // normal           normal          N
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY, DURATION_NORMAL, false, N_BACK_NORMAL));          // long             easy            N
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY, DURATION_LONG, false, N_BACK_NORMAL));        // long             normal          N
    
    // ALL SIGNAL:
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));         // normal           normal          N
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_EASY, DURATION_NORMAL, false, N_BACK_NORMAL));          // normal           normal          Y
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_EASY, DURATION_LONG, false, N_BACK_NORMAL));           // long             normal          N
    
    /////////////////////
    // Hard Difficulty //
     /////////////////////
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binD->push_back(Bin(PHASE_ALL_SIGNAL, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    
    std::vector<Bin*> holdoutCandidates;
    addToHoldoutCandidates(holdoutCandidates, binA);
    addToHoldoutCandidates(holdoutCandidates, binB);
    addToHoldoutCandidates(holdoutCandidates, binD);
    setHoldoutToCandidates(holdoutCandidates, holdoutCandidates.size() / 2);
    
    //setHoldout(binA);
    //setHoldout(binB);
    // setHoldout(binC); // no holdout for sound only
    //setHoldout(binD);
    // setHoldout(binE); // no holdout for recess (collection level)
}
//________________________________________________________________________________________


void LevelScheduler::removeBin(LevelPhase phaseX, StageDifficulty difficultyX, StageDuration durationX, bool hasHoldout)
{
    switch (phaseX) {
        case PHASE_COLLECT:
            binE->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
            break;
        
        case PHASE_COLOR_SOUND:
            binA->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
            break;
            
        case PHASE_SHAPE_SOUND:
            binB->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
            break;
            
        case PHASE_SOUND_ONLY:
            binC->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
            break;
            
        case PHASE_ALL_SIGNAL:
            binD->remove(Bin(phaseX, difficultyX, durationX, hasHoldout));
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
    if (totalMarbles < 3) {
        std::cout << "Repopulating " << std::endl;
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
//________________________________________________________________________________________


// Update holdout values inside the bins (40% of the marbles/bin is holdout)
void LevelScheduler::setHoldout( std::list<Bin>* b )
{
    int holdoutCounter = 0;
    while ( holdoutCounter != 2 ) {
        for (std::list<Bin>::iterator it = b->begin(); (it != b->end()) && (holdoutCounter != 2); ++it) {
            if ( (it->durationX != DURATION_SHORT) && rand_num(0, 1) && !it->holdout ) {
                it->holdout = true;
                holdoutCounter++;
            }
            cout << "holdout counter: " << holdoutCounter << endl;
        }
    }
}
//________________________________________________________________________________________


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
//________________________________________________________________________________________


std::vector< std::pair<StageRequest, PlayerProgress> > LevelScheduler::generateChoices(bool holdoutEnabled, bool newNavEnabled, bool indRecessEnabled, double indRecessNBackLevel,bool holdoutDelayEnabled, float holdoutDelayNumber)
{
    
    /* For debugging purposes */
    cout <<  "/--------------------------------\\" << endl
         <<  "|       Current nBack Levels     |" << endl
         << "\\--------------------------------/" << endl
         << "A: " << nBackLevelA << endl
         << "B: " << nBackLevelB << endl
         << "C: " << nBackLevelC << endl
         << "D: " << nBackLevelD << endl
         << "E: " << nBackLevelE << endl
         << "IND_DRECESS: " << indRecessNBackLevel << endl
         <<  "__________________________________" << endl;
    // */
    
    std::vector<Bin> choices;
    std::vector< std::pair<StageRequest, PlayerProgress> > result;
    std::pair<StageRequest, PlayerProgress> node;
    LevelPhase phase;
    StageDifficulty difficulty;
    StageDuration duration;
    bool holdout;
    double shift;
    double playerSkill;
    double playerOffset;
    int nBackRounded;
    
    // If the player has played enough stages, provide one
    // of the three choices to be a recess overriding
    // the randomly selected marble
    int recessIndex = -1;
    if (playCount >= 5)
    {
        playCount = 0;
        recessIndex = rand() % 3;
    }
    
    for(int i = 0; i < 3; ++i)
    {
        pickRandomMarble( choices );
        if (recessIndex == i)
        {
            phase = PHASE_COLLECT;
            difficulty = DIFFICULTY_HARD;
            duration = DURATION_NORMAL;
            holdout = false;
            shift = 0.0;
        }
        else
        {
            phase = choices[i].phaseX;
            difficulty = choices[i].difficultyX;
            duration = choices[i].durationX;
            holdout = choices[i].holdout;
            shift = choices[i].nbackShift;
        }
        
//        cout << "\n\n================================\n\nPhase: " << phase << endl;
//        cout << "Difficulty: " << difficulty << endl;
        bool readyForHoldout = true;
        
        switch ( phase ) {
            case PHASE_COLLECT:
                playerSkill = nBackLevelE;
                if((playerSkill < holdoutDelayNumber) && holdoutDelayEnabled)
                {
                    readyForHoldout = false;
                }
                playerOffset = 0.0;
                break;
            case PHASE_COLOR_SOUND:
                playerSkill = nBackLevelA;
                if((playerSkill < holdoutDelayNumber) && holdoutDelayEnabled)
                {
                    readyForHoldout = false;
                }
                playerOffset = holdoutOffsetA;
                break;
            case PHASE_SHAPE_SOUND:
                playerSkill = nBackLevelB;
                if((playerSkill < holdoutDelayNumber) && holdoutDelayEnabled)
                {
                    readyForHoldout = false;
                }
                playerOffset = holdoutOffsetB;
                break;
            case PHASE_SOUND_ONLY:
                playerSkill = nBackLevelC;
                if((playerSkill < holdoutDelayNumber) && holdoutDelayEnabled)
                {
                    readyForHoldout = false;
                }
                playerOffset = 0.0;
                break;
            case PHASE_ALL_SIGNAL:
                playerSkill = nBackLevelD;
                if((playerSkill < holdoutDelayNumber) && holdoutDelayEnabled)
                {
                    readyForHoldout = false;
                }
                playerOffset = holdoutOffsetD;
                break;
            default:
                playerSkill = 1.0;
                playerOffset = 0.0;
                break;
        }
        //std::cout << "PHASE: " << phase << std::endl;
        //std::cout << "SKILL: " << playerSkill << std::endl;
        //std::cout << "SHIFT: " << shift << std::endl;
        
        //Only if holdout is Enabled!
        
        int currentUNL = (int)round(nBackLevelE);
        
        //If Recess and indRecess Enabled, we use another UNL!
        if((phase == PHASE_COLLECT) && indRecessEnabled)
        {
            playerSkill = indRecessNBackLevel;
            currentUNL = (int)round(indRecessNBackLevel);
            std::cout << "IND RECESS: " << indRecessNBackLevel << std::endl;

        }
                if(holdoutEnabled)
        {
            if (holdout)
                nBackRounded = (int)round(playerSkill + playerOffset + shift);
            else
                nBackRounded = (int)round(playerSkill + shift);
        }
        else
        {
            nBackRounded = (int)round(playerSkill + shift);
        }
        

        
        if(nBackRounded < 1) nBackRounded = 1;
        
        if(holdoutEnabled)
        {
            if (holdout)
            {
                if(readyForHoldout)
                {
                    node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 100.0, (int)round(currentHoldout), currentUNL,newNavEnabled);
                }
                else{
                    node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 0.0, 0, currentUNL,newNavEnabled);
                }
            }
            else
            {
                node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 0.0, 0, currentUNL,newNavEnabled);
            }
        }
        else
        {
            node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 0.0, 0, currentUNL,newNavEnabled);
        }
        
        

        node.second.nBackSkill = playerSkill;
        node.second.nBackOffset = playerOffset;
        // binRef.remove(*binIt); // can't remove here... until they pick
        result.push_back(node);
    }
    
    //Push Recess Level
    holdout = false;
    shift = 0.0;
    int currentUNL = (int)round(nBackLevelE);
    //If Recess and indRecess Enabled, we use another UNL!
    if(indRecessEnabled)
    {
        playerSkill = indRecessNBackLevel;
        currentUNL = (int)round(indRecessNBackLevel);
        
    }
    nBackRounded = (int)round(playerSkill + shift);
    
    std::pair<StageRequest, PlayerProgress> Recessnode;
    Recessnode.first.generateStageRequest(nBackRounded, PHASE_COLLECT, DIFFICULTY_HARD, DURATION_NORMAL, 0.0, 0, currentUNL,newNavEnabled);
    result.push_back(Recessnode);
    
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

// Returns an average of all columns that have already been played, otherwise it returns default from study settings
int LevelScheduler::predictAverageStartingSpeed(int initVel)
{
    int cnt = 0; // number of first times
    double total = 0.0;
    if (!firstTimeA) {
        total += speedA;
        cnt++;
    }
    if (!firstTimeB) {
        total += speedB;
        cnt++;
    }
    if (!firstTimeC) {
        total += speedC;
        cnt++;
    }
    if (!firstTimeD) {
        total += speedD;
        cnt++;
    }
    if (!firstTimeE) {
        total += speedE;
        cnt++;
    }
    return cnt > 0 ? total / cnt : initVel;
}

//________________________________________________________________________________________


std::ostream& operator<<(std::ostream& out, const LevelScheduler& sch)
{
    std::cout << "Saving Scheduler\n";
    out << sch.nBackLevelA << " "
        << sch.nBackLevelB << " "
        << sch.nBackLevelC << " "
        << sch.nBackLevelD << " "
        << sch.nBackLevelE << " "
        << sch.scoreCurr << " "
        << sch.currentHoldout << " "
        << sch.holdoutOffsetA << " "
        << sch.holdoutOffsetB << " "
        << sch.holdoutOffsetD << " "
        << sch.speedA << " "
        << sch.speedB << " "
        << sch.speedC << " "
        << sch.speedD << " "
        << sch.speedE << " "
        << sch.firstTimeA << " "
        << sch.firstTimeB << " "
        << sch.firstTimeC << " "
        << sch.firstTimeD << " "
        << sch.firstTimeE << " "
        << sch.playCount << " ";
    
    std::cout << sch.binA->size() << " "
    << sch.binB->size() << " "
    << sch.binC->size() << " "
    << sch.binD->size() << " "
    << sch.binE->size() << "\n";
    
    // save binA
    if( sch.binA )  // if bin is null don't save
    {
        out << sch.binA->size() << " ";
        for (std::list<Bin>::iterator it = sch.binA->begin(); it != sch.binA->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    // save binB
    if( sch.binB )  // if bin is null don't save
    {
        out << sch.binB->size() << " ";
        for (std::list<Bin>::iterator it = sch.binB->begin(); it != sch.binB->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    // save binC
    if( sch.binC )  // if bin is null don't save
    {
        out << sch.binC->size() << " ";
        for (std::list<Bin>::iterator it = sch.binC->begin(); it != sch.binC->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    // save binD
    if( sch.binD )  // if bin is null don't save
    {
        out << sch.binD->size() << " ";
        for (std::list<Bin>::iterator it = sch.binD->begin(); it != sch.binD->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    // save binE
    if( sch.binE )  // if bin is null don't save
    {
        out << sch.binE->size() << " ";
        for (std::list<Bin>::iterator it = sch.binE->begin(); it != sch.binE->end(); ++it) {
            out << it->phaseX << " "
                << it->difficultyX << " "
                << it->durationX << " "
                << it->holdout << " "
                << it->nbackShift << " ";
        }
    }
    else
        out << 0 << " ";
    
    if (sch.scheduleHistoryA.size() > 0)
    {
        out << 1 << " ";
        out << sch.scheduleHistoryA.back().first << " " << sch.scheduleHistoryA.back().second << " ";
    }
    else
        out << 0 << " ";
    
    if (sch.scheduleHistoryB.size() > 0)
    {
        out << 1 << " ";
        out << sch.scheduleHistoryB.back().first << " " << sch.scheduleHistoryB.back().second << " ";
    }
    else
        out << 0 << " ";
    
    if (sch.scheduleHistoryC.size() > 0)
    {
        out << 1 << " ";
        out << sch.scheduleHistoryC.back().first << " " << sch.scheduleHistoryC.back().second << " ";
    }
    else
        out << 0 << " ";
    
    if (sch.scheduleHistoryD.size() > 0)
    {
        out << 1 << " ";
        out << sch.scheduleHistoryD.back().first << " " << sch.scheduleHistoryD.back().second << " ";
    }
    else
        out << 0 << " ";
    
    if (sch.scheduleHistoryE.size() > 0)
    {
        out << 1 << " ";
        out << sch.scheduleHistoryE.back().first << " " << sch.scheduleHistoryE.back().second << " ";
    }
    else
        out << 0 << " ";
    
    return out;
}
//________________________________________________________________________________________


std::istream& operator>>(std::istream& in, LevelScheduler& sch)
{
    std::cout << "Loading Scheduler\n";
    in  >> sch.nBackLevelA
        >> sch.nBackLevelB
        >> sch.nBackLevelC
        >> sch.nBackLevelD
        >> sch.nBackLevelE
        >> sch.scoreCurr
        >> sch.currentHoldout
        >> sch.holdoutOffsetA
        >> sch.holdoutOffsetB
        >> sch.holdoutOffsetD
        >> sch.speedA
        >> sch.speedB
        >> sch.speedC
        >> sch.speedD
        >> sch.speedE
        >> sch.firstTimeA
        >> sch.firstTimeB
        >> sch.firstTimeC
        >> sch.firstTimeD
        >> sch.firstTimeE
        >> sch.playCount;
    sch.initTutorialLevels();
    
    int size;
    int tmpPhase;
    int tmpDifficulty;
    int tmpDuration;
    bool tmpHoldout;
    double tmpnbackShift;
    
    // read binA
    in >> size;
    if (sch.binA) delete sch.binA;
    sch.binA = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binA->push_back(tmpBin);
        cout << "Marble A: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binB
    in >> size;
    if (sch.binB) delete sch.binB;
    sch.binB = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binB->push_back(tmpBin);
        cout << "Marble B: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binC
    in >> size;
    if (sch.binC) delete sch.binC;
    sch.binC = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binC->push_back(tmpBin);
        cout << "Marble C: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binD
    in >> size;
    if (sch.binD) delete sch.binD;
    sch.binD = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binD->push_back(tmpBin);
        cout << "Marble D: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    // read binE
    in >> size;
    if (sch.binE) delete sch.binE;
    sch.binE = new std::list<Bin>();
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binE->push_back(tmpBin);
        cout << "Marble E: " << tmpPhase << " " << tmpDifficulty << " " << tmpDuration << " " << tmpHoldout << " " << tmpnbackShift << std::endl;
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        sch.scheduleHistoryA.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        sch.scheduleHistoryB.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        sch.scheduleHistoryC.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        sch.scheduleHistoryD.push_back(level);
    }
    
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        std::pair<StageRequest, PlayerProgress> level;
        in >> level.first >> level.second;
        sch.scheduleHistoryE.push_back(level);
    }
    return in;
}
//________________________________________________________________________________________
