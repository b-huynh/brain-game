//
//  LevelSet.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/25/14.
//
//

#include "LevelSet.h"

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
    return Util::clamp(8 + row, 8, 15);
}

void LevelSet::initializeLevelSet()
{
    stageList = std::vector<std::vector<StageRequest> >(NUM_LEVELS, std::vector<StageRequest>(NUM_TASKS));
    StageRequest level;
    
    const int TOTAL_COLLECTIONS = 12;
    
#ifdef DEMO_BUILD
    /////////////////////
    /// Level DEMO  0 ///
    /////////////////////
    
    level = StageRequest();
    level.nback = 3;
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
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 0, 0));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    //level.tunnelSectionsPerNavLevel = 5;
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
    level.nback = 3;
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
    level.nback = 3;
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
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    for (int i = 0; i < TOTAL_COLLECTIONS; ++i)
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
    level.nback = 3;
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
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[0][4] = level;
    
    /////////////////////
    /// Level 5   1-F ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
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
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(2, 0, 0));
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
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
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
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
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
    level.nback = 3;
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
    level.nback = 3;
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
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[1][4] = level;
    
    /////////////////////
    /// Level 11  2-F ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
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
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(4, 0, 0));
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
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
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
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
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
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
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
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
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
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[2][4] = level;
    
    /////////////////////
    /// Level 17  3-F ///
    /////////////////////
    level = StageRequest();
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
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
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(5, 0, 0));
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
    level.nback = 5;
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
    level.nback = 5;
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
    level.nback = 5;
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
    level.nback = 5;
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
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[3][4] = level;
    
    /////////////////////
    /// Level 23  4-F ///
    /////////////////////
    level = StageRequest();
    level.nback = 5;
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
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(6, 0, 0));
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
    level.nback = 6;
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
    level.nback = 6;
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
    level.nback = 6;
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
    level.nback = 6;
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
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[4][4] = level;
    
    /////////////////////
    /// Level 29  5-F ///
    /////////////////////
    level = StageRequest();
    level.nback = 6;
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
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(7, 0, 0));
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
    level.nback = 7;
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
    level.nback = 7;
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
    level.nback = 7;
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
    level.nback = 7;
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
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[5][4] = level;
    
    /////////////////////
    /// Level 35  6-F ///
    /////////////////////
    level = StageRequest();
    level.nback = 7;
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
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(8, 0, 0));
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
    level.nback = 8;
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
    level.nback = 8;
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
    level.nback = 8;
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
    level.nback = 8;
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
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[6][4] = level;
    
    /////////////////////
    /// Level 41  7-F ///
    /////////////////////
    level = StageRequest();
    level.nback = 8;
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
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(9, 0, 0));
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
    level.nback = 9;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
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
    level.nback = 9;
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
    level.nback = 9;
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
    level.nback = 9;
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
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 10;
    level.minCamSpeed = 10;
    level.maxCamSpeed = 40;
    stageList[7][4] = level;
    
    /////////////////////
    /// Level 47  8-F ///
    /////////////////////
    level = StageRequest();
    level.nback = 9;
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
#endif
}