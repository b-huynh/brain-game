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
//bookmark
//enum EasyLevels { 0,1 };
//enum MediumLevels { 2,3,4,5 };
//enum HardLevels { DIFFICULTY_EASY, DIFFICULTY_NORMAL, DIFFICULTY_HARD };
//enum ExpertLevels { DIFFICULTY_EASY, DIFFICULTY_NORMAL, DIFFICULTY_HARD };

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
    
    float UserNavLevel;
    int Elevel, Mlevel, Hlevel, Xlevel =0;
    
    
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
    
    void setDiffPerc(int easy, int medium, int hard, int expert){
        Elevel=easy;
        Mlevel=medium;
        Hlevel=hard;
        Xlevel=expert;
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
    
    void generateStageRequest(int nback, LevelPhase PHASE_X, StageDifficulty DIFFICULTY_X, StageDuration DURATION_X, float holdout, int UNL);
    
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
