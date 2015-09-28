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
#define NUM_TASKS 6
#define NUM_LEVELS 9

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
    int pods;
    std::string nameTunnelTile;
    std::string nameSkybox;
    std::string nameMusic;
    int tunnelSectionsPerNavLevel;
    LevelPhase phaseX;
    StageDifficulty difficultyX;
    StageDuration durationX;
    
    float holdoutPerc;
    float holdoutStart; // obsolete
    float holdoutEnd;   // obsolete
    int holdoutLevel;
    bool holdoutSound;
    bool holdoutColor;
    bool holdoutShape;
    
    float UserNavLevel;
    
    float initCamSpeed;
    float minCamSpeed;
    float maxCamSpeed;
    
    std::vector<NavigationLevel> navLevels;
    std::vector<CollectionCriteria> collectionCriteria;
    std::vector<PowerupType> powerups;
    
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
        holdoutPerc = 0.0f;
        holdoutStart = 0.0f;
        holdoutEnd = 0.0f;
        holdoutLevel = 0;
        holdoutSound = false;
        holdoutColor = false;
        holdoutShape = false;
        UserNavLevel = 1.0;
        initCamSpeed = 15.0f;
        minCamSpeed = 10.0f;
        maxCamSpeed = 40.0f;
    }
    
    void generateStageRequest(int nback, LevelPhase PHASE_X, StageDifficulty DIFFICULTY_X, StageDuration DURATION_X, float holdoutPerc, float holdColor, float holdShape, float holdSound, int hlevel, int UNL, bool newNavEnabled, bool indRecessEnabled, bool indRecessFixedEnabled);
    
    StageRequest()
    {
        init();
    }
};

std::ostream& operator<<(std::ostream& outfile, const StageRequest & sr);
std::istream& operator>>(std::istream& infile, StageRequest & sr);

class LevelSet
{
private:
    //std::vector<std::vector<StageRequest> > stageList;
public:
    std::vector<std::vector<StageRequest> > stageList;

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
