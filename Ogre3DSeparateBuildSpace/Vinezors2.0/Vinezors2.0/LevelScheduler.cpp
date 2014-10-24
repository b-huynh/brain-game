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
: tutorialLevels(), scheduleHistory(), binA(NULL), binB(NULL), binC(NULL), binD(NULL), binE(NULL), totalMarbles(0), timePlayed(0), sessionFinished(false), sessionFinishedAcknowledged(false)
{
    this->nBackLevelA = nBackLevelA;
    this->nBackLevelB = nBackLevelB;
    this->nBackLevelC = nBackLevelC;
    this->nBackLevelD = nBackLevelD;
    this->nBackLevelE = nBackLevelE;
    this->currentHoldout = currentHoldout;
    this->speedA = 15.0f;
    this->speedB = 15.0f;
    this->speedC = 15.0f;
    this->speedD = 15.0f;
    this->speedE = 15.0f;
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
    
    // A recess for all navigation
    level = StageRequest();
    level.stageNo = 0;
    level.nback = 0;
    level.stageTime = 90.0;
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
    level.phase = 'E';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    level.phaseX = PHASE_COLLECT;
    level.difficultyX = DIFFICULTY_EASY;
    ret = std::pair<StageRequest, PlayerProgress>();
    ret.first = level;
    ret.second.nBackSkill = nBackLevelE;
    tutorialLevels.push_back(ret);
    
    // All-signal 1-Back
    level = StageRequest();
    level.stageNo = 0;
    level.nback = 1;
    level.stageTime = 90.0;
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
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    level.phaseX = PHASE_HOLDOUT;
    level.difficultyX = DIFFICULTY_EASY;
    ret = std::pair<StageRequest, PlayerProgress>();
    ret.first = level;
    ret.second.nBackSkill = nBackLevelA;
    tutorialLevels.push_back(ret);
}
//________________________________________________________________________________________


void LevelScheduler::populateBins()
{
    // Modifiers for each difficulty nBack
    const double N_BACK_EASY = 0.7;
    const double N_BACK_NORMAL = 0.0;
    const double N_BACK_HARD = -0.7;
    
    const int NUM_DIFFICULTIES = 3;
    
    if(!binA) binA = new std::list<Bin>();
    if(!binB) binB = new std::list<Bin>();
    if(!binC) binC = new std::list<Bin>();
    if(!binD) binD = new std::list<Bin>();
    if(!binE) binE = new std::list<Bin>();
    
    /////////////////////
    // Easy Difficulty //
    /////////////////////
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_EASY, DURATION_SHORT, false, N_BACK_EASY));
    binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_EASY, DURATION_NORMAL, false, N_BACK_EASY));
    
    ///////////////////////
    // Normal Difficulty //
    ///////////////////////
    // COLOR SOUND:
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));     // normal           normal          N
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));      // normal           normal          Y
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_NORMAL, DURATION_LONG, false, N_BACK_NORMAL));       // long             normal          N
    
    // SHAPE SOUND:
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));     // normal           normal          N
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));      // normal           normal          Y
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_NORMAL, DURATION_LONG, false, N_BACK_NORMAL));       // long             normal          N
    
    // SHAPE SOUND:
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));      // normal           normal          N
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_EASY, DURATION_LONG, false, N_BACK_NORMAL));          // long             easy            N
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_NORMAL, DURATION_LONG, false, N_BACK_NORMAL));        // long             normal          N
    
    // ALL SIGNAL:
    binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));         // normal           normal          N
    binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));          // normal           normal          Y
    binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_NORMAL, DURATION_LONG, false, N_BACK_NORMAL));           // long             normal          N
    
    // RECESS:
    binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_NORMAL, DURATION_NORMAL, false, N_BACK_NORMAL));         // normal           normal          N
    
    /////////////////////
    // Hard Difficulty //
    /////////////////////
    binA->push_back(Bin(PHASE_COLOR_SOUND, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binB->push_back(Bin(PHASE_SHAPE_SOUND, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binC->push_back(Bin(PHASE_SOUND_ONLY, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binD->push_back(Bin(PHASE_HOLDOUT, DIFFICULTY_HARD, DURATION_LONG, false, N_BACK_HARD));
    binE->push_back(Bin(PHASE_COLLECT, DIFFICULTY_HARD, DURATION_NORMAL, false, N_BACK_HARD));
    
    setHoldout(binA);
    setHoldout(binB);
    // setHoldout(binC); // no holdout for sound only
    setHoldout(binD);
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
            
        case PHASE_HOLDOUT:
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


std::vector< std::pair<StageRequest, PlayerProgress> > LevelScheduler::generateChoices()
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
    int nBackRounded;
    
    for(int i = 0; i < 3; ++i)
    {
        pickRandomMarble( choices );
        phase = choices[i].phaseX;
        difficulty = choices[i].difficultyX;
        duration = choices[i].durationX;
        holdout = choices[i].holdout;
        shift = choices[i].nbackShift;
        
//        cout << "\n\n================================\n\nPhase: " << phase << endl;
//        cout << "Difficulty: " << difficulty << endl;
        
        switch ( phase ) {
            case PHASE_COLLECT:
                playerSkill = nBackLevelE;
                break;
            case PHASE_COLOR_SOUND:
                playerSkill = nBackLevelA;
                break;
            case PHASE_SHAPE_SOUND:
                playerSkill = nBackLevelB;
                break;
            case PHASE_SOUND_ONLY:
                playerSkill = nBackLevelC;
                break;
            case PHASE_HOLDOUT:
                playerSkill = nBackLevelD;
                break;
            default:
                playerSkill = 1.0;
                break;
        }
        //std::cout << "PHASE: " << phase << std::endl;
        //std::cout << "SKILL: " << playerSkill << std::endl;
        //std::cout << "SHIFT: " << shift << std::endl;
        nBackRounded = (int)round(playerSkill + shift);
        
        int currentUNL = (int)round(nBackLevelE);
        
        if(nBackRounded < 1) nBackRounded = 1;
        if (holdout)

            node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, currentHoldout, currentUNL);
        else
            node.first.generateStageRequest(nBackRounded, phase, difficulty, duration, 0.0, currentUNL);

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
    out << sch.nBackLevelA << " "
        << sch.nBackLevelB << " "
        << sch.nBackLevelC << " "
        << sch.nBackLevelD << " "
        << sch.nBackLevelE << " "
        << sch.currentHoldout << " "
        << sch.speedA << " "
        << sch.speedB << " "
        << sch.speedC << " "
        << sch.speedD << " "
        << sch.speedE << " ";
    
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
        out << 0;
    
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
        out << 0;
    
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
        out << 0;
    
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
        out << 0;
    
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
        out << 0;
    
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
        >> sch.currentHoldout
        >> sch.speedA
        >> sch.speedB
        >> sch.speedC
        >> sch.speedD
        >> sch.speedE;
    sch.initTutorialLevels();
    
    int size;
    int tmpPhase;
    int tmpDifficulty;
    int tmpDuration;
    bool tmpHoldout;
    double tmpnbackShift;
    
    // read binA
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binA->push_back(tmpBin);
    }
    
    // read binB
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binB->push_back(tmpBin);
    }
    
    // read binC
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binC->push_back(tmpBin);
    }
    
    // read binD
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binD->push_back(tmpBin);
    }
    
    // read binE
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        in >> tmpPhase >> tmpDifficulty >> tmpDuration >> tmpHoldout >> tmpnbackShift;
        Bin tmpBin = Bin((LevelPhase)tmpPhase, (StageDifficulty)tmpDifficulty, (StageDuration)tmpDuration, tmpHoldout, tmpnbackShift);
        sch.binE->push_back(tmpBin);
    }
    
    return in;
}
//________________________________________________________________________________________
