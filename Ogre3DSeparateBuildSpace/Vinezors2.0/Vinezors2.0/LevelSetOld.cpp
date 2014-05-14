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
    return levelSelect >= 0 && levelSelect < stageList.size();
}

bool LevelSet::hasLevel(int row, int col) const
{
    return NUM_TASKS * row + col >= 0 && NUM_TASKS * row + col < stageList.size();
}

StageRequest LevelSet::retrieveLevel(int levelSelect) const
{
    return stageList[levelSelect];
}

StageRequest LevelSet::retrieveLevel(int row, int col) const
{
    return stageList[NUM_TASKS * row + col];
}

int LevelSet::getLevelNo(char row, int col) const
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
    return stageList.size();
}

void LevelSet::initializeLevelSet()
{
    StageRequest level;
    
    /////////////////////
    /// Level 0   A-1 ///
    /////////////////////
    level = StageRequest();
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 0, 0));
    level.nameTunnelTile = "General/WallBindingStart";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 25;
    level.phase = 'E';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    /*
    level = StageRequest();
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.nameTunnelTile = "General/WallBindingStart";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'F';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 50;
    stageList.push_back(level);
     */
    
    /////////////////////
    /// Level 1   A-2 ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 2   A-3 ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(0));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 3   A-4 ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 4   A-5 ///
    /////////////////////
    level = StageRequest();
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 0, 0));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 17;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 50;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 5   A-6 ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 6   A-7 ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 0, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 0, 0));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.hasHoldout = true;
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 7   A-8 ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 8   B-1 ///
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
    level.initCamSpeed = 17;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 50;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 9   B-2 ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.nameTunnelTile = "General/WallBindingE";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 10  B-3 ///
    /////////////////////
    level = StageRequest();
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 11  B-4 ///
    /////////////////////
    level = StageRequest();
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 12  B-5 ///
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
    level.initCamSpeed = 17;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 50;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 13  B-6 ///
    /////////////////////
    level = StageRequest();
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 2));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.hasHoldout = true;
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 14  B-7 ///
    /////////////////////
    level = StageRequest();
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.nameTunnelTile = "General/WallBindingC";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music5";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'C';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 15  B-8 ///
    /////////////////////
    level = StageRequest();
    level.nback = 4;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.collectionCriteria.push_back(CollectionCriteria(4));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 16  C-1 ///
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
    level.initCamSpeed = 17;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 50;
    stageList.push_back(level);
    
    /*
    /////////////////////
    /// Level ??? ?-? ///
    /////////////////////
    level = StageRequest();
    level.nback = 6;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 3));
    level.navLevels.push_back(NavigationLevel(0, 4, 2));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.collectionCriteria.push_back(CollectionCriteria(5));
    level.collectionCriteria.push_back(CollectionCriteria(5));
    level.collectionCriteria.push_back(CollectionCriteria(5));
    level.collectionCriteria.push_back(CollectionCriteria(5));
    level.collectionCriteria.push_back(CollectionCriteria(5));
    level.collectionCriteria.push_back(CollectionCriteria(6));
    level.collectionCriteria.push_back(CollectionCriteria(6));
    level.collectionCriteria.push_back(CollectionCriteria(6));
    level.powerups.push_back(POWERUP_TIME_WARP);
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
     */
}