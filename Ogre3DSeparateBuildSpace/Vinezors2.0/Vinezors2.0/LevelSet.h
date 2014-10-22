//
//  LevelSet.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/25/14.
//
//

#ifndef __Vinezors2_0__LevelSet__
#define __Vinezors2_0__LevelSet__

#include "Util.h"

// Number of Columns and Rows
#ifdef DEMO_BUILD
#define NUM_TASKS 1
#define NUM_LEVELS 1
#else
#define NUM_TASKS 6
#define NUM_LEVELS 9
#endif

enum StageDifficulty { DIFFICULTY_EASY, DIFFICULTY_NORMAL, DIFFICULTY_HARD };
enum StageDuration {DURATION_SHORT, DURATION_NORMAL, DURATION_LONG};
enum LevelPhase { PHASE_COLLECT, PHASE_COLOR_SOUND, PHASE_SHAPE_SOUND, PHASE_SOUND_ONLY, PHASE_HOLDOUT };

struct CollectionCriteria
{
    int nback;
    int collected;
    
    CollectionCriteria(int n) : nback(n), collected(0) {}
};

struct StageRequest
{
    int nback;
    int stageNo;
    float stageTime;
    std::vector<NavigationLevel> navLevels;
    int pods;
    std::vector<CollectionCriteria> collectionCriteria;
    std::vector<PowerupType> powerups;
    std::string nameTunnelTile;
    std::string nameSkybox;
    std::string nameMusic;
    int tunnelSectionsPerNavLevel;
    char phase;
    LevelPhase phaseX;
    StageDifficulty difficultyX;
    
    float holdoutPerc;
    float holdoutStart;
    float holdoutEnd;
    bool holdoutSound;
    bool holdoutColor;
    bool holdoutShape;
    
    float initCamSpeed;
    float minCamSpeed;
    float maxCamSpeed;
    
    // equivalence operator
    bool operator==(const StageRequest sr) const
    {
        if ( stageNo == sr.stageNo )
            return true;
        return false;
    }
    
    bool hasHoldout() const
    {
        return (holdoutSound || holdoutColor || holdoutShape) && holdoutPerc > 0.0f;
    }
    
    // clear everything and set defaults
    void init()
    {
        nback = 0;
        stageNo = 0;
        stageTime = 120.0f;
        navLevels.clear();
        pods = 0;
        collectionCriteria.clear();
        powerups.clear();
        nameTunnelTile = "";
        nameSkybox = "";
        nameMusic = "";
        tunnelSectionsPerNavLevel = 10;
        phase = 'D';
        holdoutPerc = 0.0f;
        holdoutStart = 0.0f;
        holdoutEnd = 0.0f;
        holdoutSound = false;
        holdoutColor = false;
        holdoutShape = false;
        initCamSpeed = 15.0f;
        minCamSpeed = 10.0f;
        maxCamSpeed = 40.0f;
    }
    
    void generateStageRequest(int nback, LevelPhase PHASE_X, StageDifficulty DIFFICULTY_X, StageDuration DURATION_X, double holdout)
    {
        // These are set for all levels regardless of phase/diffuculty
        // Not entirely sure on collection requirements as of now
        const double EASY_TIME = 60.0, NORMAL_TIME = 90.0, HARD_TIME = 120.0;
        const int EASY_COLLECTIONS = 4, NORMAL_COLLECTIONS = 8, HARD_COLLECTIONS = 13;
        
        StageRequest* ret = this;
        ret->init(); // Reset everything to clear lists if they're still populated
        double duration;
        switch ( DURATION_X )
        {
            case DURATION_SHORT:
            {
                duration = EASY_TIME;
                if (PHASE_X != PHASE_COLLECT)
                {
                    for (int i = 0; i < EASY_COLLECTIONS; ++i)
                        ret->collectionCriteria.push_back(CollectionCriteria(nback));
                }
                break;
            }
            case DURATION_NORMAL:
            {
                duration = NORMAL_TIME;
                if (PHASE_X != PHASE_COLLECT)
                {
                    for (int i = 0; i < NORMAL_COLLECTIONS; ++i)
                        ret->collectionCriteria.push_back(CollectionCriteria(nback));
                }
                break;
            }
            case DURATION_LONG:
                duration = HARD_TIME;
                if (PHASE_X != PHASE_COLLECT)
                {
                    for (int i = 0; i < HARD_COLLECTIONS; ++i)
                        ret->collectionCriteria.push_back(CollectionCriteria(nback));
                }
                break;
            default:
                duration = NORMAL_TIME;
                if (PHASE_X != PHASE_COLLECT)
                {
                    for (int i = 0; i < NORMAL_COLLECTIONS; ++i)
                        ret->collectionCriteria.push_back(CollectionCriteria(nback));
                }
                break;
        }
        
        ret->nback = nback;
        ret->nameSkybox = "General/BlankStarrySkyPlane";
        ret->tunnelSectionsPerNavLevel = 10;
        ret->initCamSpeed = 10;
        ret->minCamSpeed = 10;
        ret->maxCamSpeed = 40;
        ret->phaseX = PHASE_X;
        ret->difficultyX = DIFFICULTY_X;
        ret->holdoutPerc = holdout / 100.0;
        ret->stageTime = duration;
        if (holdoutPerc > 0.0)
        {
            ret->holdoutStart = 0.20;
            ret->holdoutEnd = 0.70;
            ret->holdoutSound = 1;
            ret->holdoutColor = 1;
            ret->holdoutShape = 1;
        }
        else
        {
            ret->holdoutStart = 0.0;
            ret->holdoutEnd = 0.0;
            ret->holdoutSound = 0;
            ret->holdoutColor = 0;
            ret->holdoutShape = 0;
        }
        
        // Chooses what phase and difficulty to generate for ret
        switch( PHASE_X )
        {
            case PHASE_COLLECT:
                ret->nback = 0;
                switch (DIFFICULTY_X)
                {
                    case DIFFICULTY_EASY:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_NORMAL:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_HARD:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    default:
                        break;
                }
                // These are always set for all recess levels
                ret->nameTunnelTile = "General/WallBindingG";
                ret->nameMusic = "Music4";
                ret->phase = 'E';
                break;
                //_____________________________________________________________
                
            case PHASE_COLOR_SOUND:
                switch (DIFFICULTY_X)
                {
                    case DIFFICULTY_EASY:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_NORMAL:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_HARD:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    default:
                        break;
                }
                // These are always set for color sound levels
                ret->nameTunnelTile = "General/WallBindingA";
                ret->nameMusic = "Music2";
                ret->phase = 'A';
                break;
                //_____________________________________________________________
                
            case PHASE_SHAPE_SOUND:
                switch (DIFFICULTY_X)
                {
                    case DIFFICULTY_EASY:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_NORMAL:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_HARD:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    default:
                        break;
                }
                // These are allways set for shape sound levels
                ret->nameTunnelTile = "General/WallBindingB";
                ret->nameMusic = "Music1";
                ret->phase = 'B';
                break;
                //_____________________________________________________________
                
            case PHASE_SOUND_ONLY:
                switch (DIFFICULTY_X)
                {
                    case DIFFICULTY_EASY:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_NORMAL:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_HARD:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    default:
                        break;
                }
                // These are always set for sound only levels
                ret->nameTunnelTile = "General/WallBindingC";
                ret->nameMusic = "Music5";
                ret->phase = 'C';
                break;
                //_____________________________________________________________
                
            case PHASE_HOLDOUT:
                switch (DIFFICULTY_X)
                {
                    case DIFFICULTY_EASY:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_NORMAL:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                        
                    case DIFFICULTY_HARD:
                        ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                        break;
                    default:
                        break;
                }
                // These are always set for holdout level
                ret->nameTunnelTile = "General/WallBindingD";
                ret->nameMusic = "Music3";
                ret->phase = 'D';
                break;
                //_____________________________________________________________
                
            default:
                break;
        }
        // Just in case a nav levels
        if (ret->navLevels.size() <= 0)
        {
            ret->navLevels.push_back(NavigationLevel(0, 1, 0));
            ret->navLevels.push_back(NavigationLevel(0, 2, 0));
            ret->navLevels.push_back(NavigationLevel(0, 3, 0));
            ret->navLevels.push_back(NavigationLevel(0, 4, 0));
        }
    }
    
    std::vector<NavigationLevel> generateNavigationLevels(StageDifficulty navigationDifficulty)
    {
        std::vector<NavigationLevel> ret;
        switch (navigationDifficulty)
        {
            case DIFFICULTY_EASY:
                ret = generateRandomEasyNavigation();
                break;
            case DIFFICULTY_NORMAL:
                ret = generateRandomNormalNavigation();
                break;
            case DIFFICULTY_HARD:
                ret = generateRandomHardNavigation();
                break;
            default:
                ret.push_back(NavigationLevel(0, 1, 0));
                ret.push_back(NavigationLevel(0, 2, 0));
                ret.push_back(NavigationLevel(0, 3, 0));
                ret.push_back(NavigationLevel(0, 4, 0));
                break;
        }
        return ret;
    }
    std::vector<NavigationLevel> generateRandomEasyNavigation();
    std::vector<NavigationLevel> generateRandomNormalNavigation();
    std::vector<NavigationLevel> generateRandomHardNavigation();
    
    StageRequest()
    {
        init();
    }
};

class LevelSet
{
private:
    std::vector<std::vector<StageRequest> > stageList;
public:
    float holdoutFreqvar;
    LevelSet() : stageList(), holdoutFreqvar(4) {}
    
    // Returns true whether the level exists.
    // Refer to player.h isLevelAvailable to determine whether the player reqs meet playability
    bool hasLevel(int levelSelect) const;
    bool hasLevel(int row, int col) const;
    bool hasLevelRow(int row) const;
    
    StageRequest retrieveLevel(int levelSelect) const;
    StageRequest retrieveLevel(int row, int col) const;
    float get_holdoutFreqvar();
    int getLevelNo(int row, int col) const; // Level Selection (row, col) -> (n-back, task)
    int getLevelRow(int level) const;
    int getLevelCol(int level) const;
    int getNumLevels() const;
    int getTotalRowRequirement(int row) const; // Star requirement to unlock next row
    // Simple accessor function for private stageList
    std::vector<std::vector<StageRequest> > getStageList() const;
    void ManLevelSet( int lvl, int phs,  int pds, float hldperc, float hldstrt, float hldend, String hlds, String hldc, String hldsh, std::vector<int> sds, std::vector<int> obs);
    void initializeLevelSet();
};

#endif /* defined(__Vinezors2_0__LevelSet__) */
