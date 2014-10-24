//
//  LevelSet.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/25/14.
//
//

#include "LevelSet.h"

#include "Util.h"

extern Util::ConfigGlobal globals;

void StageRequest::generateStageRequest(int nback, LevelPhase PHASE_X, StageDifficulty DIFFICULTY_X, float holdout, int UNL)
{
    // These are set for all levels regardless of phase/diffuculty
    // Not entirely sure on collection requirements as of now
    const double EASY_TIME = 60.0, NORMAL_TIME = 90.0, HARD_TIME = 120.0;
    const int EASY_COLLECTIONS = 4, NORMAL_COLLECTIONS = 8, HARD_COLLECTIONS = 13;
    StageRequest* ret = this;
    ret->init(); // Reset everything to clear lists if they're still populated
    ret->nback = nback;
    ret->nameSkybox = "General/BlankStarrySkyPlane";
    ret->tunnelSectionsPerNavLevel = 10;
    ret->initCamSpeed = 10;
    ret->minCamSpeed = 10;
    ret->maxCamSpeed = 40;
    ret->phaseX = PHASE_X;
    ret->difficultyX = DIFFICULTY_X;
    ret->holdoutPerc = holdout / 100.0;
    ret->UserNavLevel = UNL;
    
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
    
    
   
    
    if(UNL<2)
    {
        UNL=2;
    }
    else if(UNL>globals.navMap.size()-2)
    {
        UNL=globals.navMap.size()-2;
    }
    
    
    int randSpot= rand()%4;
    int randSpot2=rand()%4;
    int randSpot3= rand()%4;
    while(randSpot2==randSpot)randSpot2=rand()%4;
    while(randSpot3==randSpot&&randSpot3==randSpot2)randSpot3=rand()%4;
    
    int randSpot4= 6-randSpot-randSpot2-randSpot3;
    std::cout<<"spots: "<<randSpot<<std::endl<<randSpot2<<std::endl<<randSpot3<<std::endl<<randSpot4<<std::endl;
    
    ret->navLevels.push_back(globals.navMap[UNL-2+randSpot]);
    ret->navLevels.push_back(globals.navMap[UNL-2+randSpot2]);
    ret->navLevels.push_back(globals.navMap[UNL-2+randSpot3]);
    ret->navLevels.push_back(globals.navMap[UNL-2+randSpot4]);
    
    
    
    // Chooses what phase and difficulty to generate for ret
    switch( PHASE_X )
    {
        case PHASE_COLLECT:
            ret->nback = 0;
            switch (DIFFICULTY_X)
        {
            case DIFFICULTY_EASY:
                ret->stageTime = NORMAL_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                break;
                
            case DIFFICULTY_NORMAL:
                ret->stageTime = NORMAL_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                break;
                
            case DIFFICULTY_HARD:
                ret->stageTime = NORMAL_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
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
                ret->stageTime = EASY_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < EASY_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            case DIFFICULTY_NORMAL:
                ret->stageTime = NORMAL_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < NORMAL_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            case DIFFICULTY_HARD:
                ret->stageTime = HARD_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < HARD_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            default:
                break;
        }
            // These are always set for color sound levels
            ret->powerups.push_back(POWERUP_TIME_WARP);
            ret->nameTunnelTile = "General/WallBindingA";
            ret->nameMusic = "Music2";
            ret->phase = 'A';
            break;
            //_____________________________________________________________
            
        case PHASE_SHAPE_SOUND:
            switch (DIFFICULTY_X)
        {
            case DIFFICULTY_EASY:
                ret->stageTime = EASY_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < EASY_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            case DIFFICULTY_NORMAL:
                ret->stageTime = NORMAL_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < NORMAL_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            case DIFFICULTY_HARD:
                ret->stageTime = HARD_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < HARD_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            default:
                break;
        }
            // These are allways set for shape sound levels
            ret->powerups.push_back(POWERUP_TIME_WARP);
            ret->nameTunnelTile = "General/WallBindingB";
            ret->nameMusic = "Music1";
            ret->phase = 'B';
            break;
            //_____________________________________________________________
            
        case PHASE_SOUND_ONLY:
            switch (DIFFICULTY_X)
        {
            case DIFFICULTY_EASY:
                ret->stageTime = EASY_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < EASY_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            case DIFFICULTY_NORMAL:
                ret->stageTime = NORMAL_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < NORMAL_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            case DIFFICULTY_HARD:
                ret->stageTime = HARD_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < HARD_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            default:
                break;
        }
            // These are always set for sound only levels
            ret->powerups.push_back(POWERUP_TIME_WARP);
            ret->nameTunnelTile = "General/WallBindingC";
            ret->nameMusic = "Music5";
            ret->phase = 'C';
            break;
            //_____________________________________________________________
            
        case PHASE_HOLDOUT:
            switch (DIFFICULTY_X)
        {
            case DIFFICULTY_EASY:
                ret->stageTime = EASY_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < EASY_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            case DIFFICULTY_NORMAL:
                ret->stageTime = NORMAL_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < NORMAL_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
                
            case DIFFICULTY_HARD:
                ret->stageTime = HARD_TIME;
                //ret->navLevels = generateNavigationLevels(DIFFICULTY_X);
                for (int i = 0; i < HARD_COLLECTIONS; ++i)
                    ret->collectionCriteria.push_back(CollectionCriteria(nback));
                break;
            default:
                break;
        }
            // These are always set for holdout level
            ret->powerups.push_back(POWERUP_TIME_WARP);
            ret->nameTunnelTile = "General/WallBindingD";
            ret->nameMusic = "Music3";
            ret->phase = 'D';
            break;
            //_____________________________________________________________
            
        default:
            break;
    }
    // Just in no init nav levels
    if (ret->navLevels.size() <= 0)
    {
        ret->navLevels.push_back(NavigationLevel(0, 1, 0));
        ret->navLevels.push_back(NavigationLevel(0, 2, 0));
        ret->navLevels.push_back(NavigationLevel(0, 3, 0));
        ret->navLevels.push_back(NavigationLevel(0, 4, 0));
    }
}


bool LevelSet::hasLevel(int levelSelect) const
{
    return hasLevel(levelSelect / NUM_TASKS, levelSelect % NUM_TASKS);
}

bool LevelSet::hasLevel(int row, int col) const
{
    return row >= 0 && row < stageList.size() && col >= 0 && col < stageList[row].size();
}

bool LevelSet::hasLevelRow(int row) const
{
    return row >= 0 && row < stageList.size();
}

StageRequest LevelSet::retrieveLevel(int levelSelect) const
{
    return stageList[levelSelect / NUM_TASKS][levelSelect % NUM_TASKS];
}

StageRequest LevelSet::retrieveLevel(int row, int col) const
{
    return stageList[row][col];
}

int LevelSet::getLevelNo(int row, int col) const
{
    return NUM_TASKS * row + col;
}

int LevelSet::getLevelRow(int level) const
{
    return level / NUM_TASKS;
}

float LevelSet::get_holdoutFreqvar(){
    return LevelSet::holdoutFreqvar;
}

int LevelSet::getLevelCol(int level) const
{
    return level % NUM_TASKS;
}

int LevelSet::getNumLevels() const
{
    return NUM_TASKS * NUM_LEVELS;
}

int LevelSet::getTotalRowRequirement(int row) const
{
    return Util::clamp(10 + 2 * row, 8, 25);
}

std::vector<std::vector<StageRequest> > LevelSet::getStageList() const
{
    return stageList;
}

void LevelSet::ManLevelSet( int lvl, int phs,  int pds, float hldperc, float hldstrt, float hldend, String hlds, String hldc, String hldsh, std::vector<int> sds, std::vector<int> obs)
{
    StageRequest level;
    level = stageList[lvl][phs];
    level.pods = pds;
    level.holdoutStart = hldstrt/ 100.0f;
    level.holdoutEnd = hldend/ 100.0f;
    if(hlds == "yes")level.holdoutSound = 1;
    else level.holdoutSound = 0;
    if(hldc == "yes")level.holdoutColor = 1;
    else level.holdoutColor = 0;
    if(hldsh == "yes")level.holdoutShape = 1;
    else level.holdoutShape = 0;
    
    
    for(int i=0; i<level.navLevels.size(); i++)level.navLevels[i].control = sds[i];
    for(int i=0; i<level.navLevels.size(); i++)level.navLevels[i].obstacles = obs[i];
    for (int i =0; i<level.navLevels.size(); i++) {
            std::cout<<"Sides: "<<level.navLevels[i].control<<"               Obstacles: "<<level.navLevels[i].obstacles;
    }
    level.holdoutPerc=hldperc;
    stageList[lvl][phs] = level;    //ALWAYS KEEP LAST
}

void LevelSet::initializeLevelSet()
{
    stageList = std::vector<std::vector<StageRequest> >(NUM_LEVELS, std::vector<StageRequest>(NUM_TASKS));
    StageRequest level;
    
    const int TOTAL_COLLECTIONS = 13;
    
#ifdef DEMO_BUILD
    /////////////////////
    /// Level DEMO  0 ///
    /////////////////////
    
    level = StageRequest();
    level.stageNo++;
    level.nback = 1;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(2));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[0][0] = level;
#else
    /////////////////////
    /// Level 0   1-A ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
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
    stageList[0][0] = level;
    
    /////////////////////
    /// Level 1   1-B ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 1;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(1));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;    
    stageList[0][1] = level;
    
    
    /////////////////////
    /// Level 2   1-C ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 1;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(1));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[0][2] = level;
    
    /////////////////////
    /// Level 3   1-D ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 1;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)                 //collection criteria
        level.collectionCriteria.push_back(CollectionCriteria(1));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[0][3] = level;
    
    /////////////////////
    /// Level 4   1-E ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 1;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(1));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[0][4] = level;
    
    /////////////////////
    /// Level 5   1-F ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 1;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(1));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(2));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[0][5] = level;
    
    /////////////////////
    /// Level 6   2-A ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 15;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[1][0] = level;
    
    /////////////////////
    /// Level 7   2-B ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 2;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(2));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[1][1] = level;
    
    /////////////////////
    /// Level 8   2-C ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 2;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(2));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[1][2] = level;
    
    /////////////////////
    /// Level 9   2-D ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 2;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(2));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[1][3] = level;
    
    /////////////////////
    /// Level 10  2-E ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 2;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(2));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[1][4] = level;
    
    /////////////////////
    /// Level 11  2-F ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 2;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(2));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(3));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[1][5] = level;
    
    /////////////////////
    /// Level 12  3-A ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 15;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[2][0] = level;
    
    /////////////////////
    /// Level 13  3-B ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(3));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[2][1] = level;
    
    /////////////////////
    /// Level 14  3-C ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(3));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[2][2] = level;
    
    /////////////////////
    /// Level 15  3-D ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(3));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[2][3] = level;
    
    /////////////////////
    /// Level 16  3-E ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(3));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[2][4] = level;
    
    /////////////////////
    /// Level 17  3-F ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(3));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(4));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[2][5] = level;
    
    /////////////////////
    /// Level 18  4-A ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 15;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[3][0] = level;
    
    /////////////////////
    /// Level 19  4-B ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(4));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[3][1] = level;
    
    /////////////////////
    /// Level 20  4-C ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(4));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[3][2] = level;
    
    /////////////////////
    /// Level 21  4-D ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(4));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[3][3] = level;
    
    /////////////////////
    /// Level 22  4-E ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(4));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[3][4] = level;
    
    /////////////////////
    /// Level 23  4-F ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(4));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(5));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[3][5] = level;
    
    /////////////////////
    /// Level 24  5-A ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 15;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[4][0] = level;
    
    /////////////////////
    /// Level 25  5-B ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 5;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(5));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[4][1] = level;
    
    /////////////////////
    /// Level 26  5-C ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 5;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(5));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[4][2] = level;
    
    /////////////////////
    /// Level 27  5-D ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 5;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(5));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[4][3] = level;
    
    /////////////////////
    /// Level 28  5-E ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 5;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(5));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[4][4] = level;
    
    /////////////////////
    /// Level 29  5-F ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 5;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(5));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(6));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[4][5] = level;
    
    /////////////////////
    /// Level 30  6-A ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 2));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 15;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[5][0] = level;
    
    /////////////////////
    /// Level 31  6-B ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 6;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 2));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(6));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[5][1] = level;
    
    /////////////////////
    /// Level 32  6-C ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 6;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(6));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[5][2] = level;
    
    /////////////////////
    /// Level 33  6-D ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 6;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 2));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(6));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[5][3] = level;
    
    /////////////////////
    /// Level 34  6-E ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 6;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(6));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[5][4] = level;
    
    /////////////////////
    /// Level 35  6-F ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 6;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(6));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(7));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[5][5] = level;
    
    /////////////////////
    /// Level 36  7-A ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 15;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[6][0] = level;
    
    /////////////////////
    /// Level 37  7-B ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 7;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 2));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(7));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[6][1] = level;
    
    /////////////////////
    /// Level 38  7-C ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 7;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(7));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[6][2] = level;
    
    /////////////////////
    /// Level 39  7-D ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 7;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(7));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[6][3] = level;
    
    /////////////////////
    /// Level 40  7-E ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 7;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 2));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(7));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[6][4] = level;
    
    /////////////////////
    /// Level 41  7-F ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 7;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 2));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(7));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(8));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[6][5] = level;
    
    /////////////////////
    /// Level 42  8-A ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 15;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[7][0] = level;
    
    /////////////////////
    /// Level 43  8-B ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 8;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(8));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[7][1] = level;
    
    /////////////////////
    /// Level 44  8-C ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 8;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(8));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[7][2] = level;
    
    /////////////////////
    /// Level 45  8-D ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 8;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(8));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[7][3] = level;
    
    /////////////////////
    /// Level 46  8-E ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 8;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(8));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[7][4] = level;
    
    /////////////////////
    /// Level 47  8-F ///
    /////////////////////
    level = StageRequest();
    level.stageNo++;
    level.nback = 8;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(8));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(9));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[7][5] = level;
    
    
    
    
    // UNTESTED NEW LEVELS
    
    /////////////////////
    /// Level 48  9-A ///
    /////////////////////
    level = StageRequest();
    level.nback = 0;
    level.stageNo++;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(10, 0, 0));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 15;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[8][0] = level;
    
    /////////////////////
    /// Level 49  9-B ///
    /////////////////////
    level = StageRequest();
    level.nback = 9;
    level.stageNo++;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(9));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[8][1] = level;
    
    /////////////////////
    /// Level 50  9-C ///
    /////////////////////
    level = StageRequest();
    level.nback = 9;
    level.stageNo++;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(9));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[8][2] = level;
    
    /////////////////////
    /// Level 51  9-D ///
    /////////////////////
    level = StageRequest();
    level.nback = 9;
    level.stageNo++;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(9));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[8][3] = level;
    
    /////////////////////
    /// Level 52  9-E ///
    /////////////////////
    level = StageRequest();
    level.nback = 9;
    level.stageNo++;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(9));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[8][4] = level;
    
    /////////////////////
    /// Level 53  9-F ///
    /////////////////////
    level = StageRequest();
    level.nback = 9;
    level.stageNo++;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 2));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(9));
    for (int i = 0; i < TOTAL_COLLECTIONS / 2; ++i)
        level.collectionCriteria.push_back(CollectionCriteria(10));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[8][5] = level;
#endif
}

std::vector<NavigationLevel> StageRequest::generateRandomEasyNavigation()
{
    std::vector<NavigationLevel> ret;
    const int NUM_NAVIGATION_SETS = 10;
    int rvalue = std::rand() % NUM_NAVIGATION_SETS;
    switch (rvalue)
    {
        case 0:
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            break;
        case 1:
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            break;
        case 2:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            break;
        case 3:
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            break;
        case 4:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            break;
        case 5:
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            break;
        case 6:
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            break;
        case 7:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            break;
        case 8:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            break;
        case 9:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            break;
        default:
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            break;
    }
    return ret;
}
std::vector<NavigationLevel> StageRequest::generateRandomNormalNavigation()
{
    std::vector<NavigationLevel> ret;
    const int NUM_NAVIGATION_SETS = 10;
    int rvalue = std::rand() % NUM_NAVIGATION_SETS;
    switch (rvalue)
    {
        case 0:
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            break;
        case 1:
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 4, 0));
            break;
        case 2:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 2, 1));
            break;
        case 3:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 2, 1));
            break;
        case 4:
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 1));
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 1, 0));
            break;
        case 5:
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 2, 1));
            break;
        case 6:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 1, 0));
            break;
        case 7:
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 1, 1));
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 1, 0));
            break;
        case 8:
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            break;
        case 9:
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 3, 0));
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 3, 0));
            break;
        default:
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 4, 0));
            break;
    }
    return ret;
}

std::vector<NavigationLevel> StageRequest::generateRandomHardNavigation()
{
    std::vector<NavigationLevel> ret;
    const int NUM_NAVIGATION_SETS = 10;
    int rvalue = std::rand() % NUM_NAVIGATION_SETS;
    switch (rvalue)
    {
        case 0:
            ret.push_back(NavigationLevel(0, 1, 0));
            ret.push_back(NavigationLevel(0, 2, 2));
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 1, 1));
            break;
        case 1:
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 3, 1));
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 1, 0));
            break;
        case 2:
            ret.push_back(NavigationLevel(0, 3, 1));
            ret.push_back(NavigationLevel(0, 3, 2));
            ret.push_back(NavigationLevel(0, 4, 2));
            ret.push_back(NavigationLevel(0, 4, 1));
            break;
        case 3:
            ret.push_back(NavigationLevel(0, 4, 0));
            ret.push_back(NavigationLevel(0, 3, 1));
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 3, 2));
            break;
        case 4:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 3, 2));
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 1, 1));
            break;
        case 5:
            ret.push_back(NavigationLevel(0, 2, 0));
            ret.push_back(NavigationLevel(0, 1, 1));
            ret.push_back(NavigationLevel(0, 2, 2));
            ret.push_back(NavigationLevel(0, 1, 1));
            break;
        case 6:
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 3, 2));
            ret.push_back(NavigationLevel(0, 3, 1));
            ret.push_back(NavigationLevel(0, 2, 2));
            break;
        case 7:
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 3, 1));
            ret.push_back(NavigationLevel(0, 4, 2));
            ret.push_back(NavigationLevel(0, 3, 0));
            break;
        case 8:
            ret.push_back(NavigationLevel(0, 3, 1));
            ret.push_back(NavigationLevel(0, 2, 1));
            ret.push_back(NavigationLevel(0, 2, 2));
            ret.push_back(NavigationLevel(0, 2, 0));
            break;
        case 9:
            ret.push_back(NavigationLevel(0, 3, 1));
            ret.push_back(NavigationLevel(0, 4, 1));
            ret.push_back(NavigationLevel(0, 4, 2));
            ret.push_back(NavigationLevel(0, 4, 1));
            break;
        default:
            ret.push_back(NavigationLevel(0, 4, 2));
            ret.push_back(NavigationLevel(0, 4, 2));
            ret.push_back(NavigationLevel(0, 4, 2));
            ret.push_back(NavigationLevel(0, 4, 2));
            break;
    }
    return ret;
}
