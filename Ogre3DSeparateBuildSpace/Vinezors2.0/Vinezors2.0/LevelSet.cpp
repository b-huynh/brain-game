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
    return stageList.size();
}

int LevelSet::getTotalRowRequirement(int row) const
{
    return Util::clamp(7 + row, 7, 12);
}

void LevelSet::initializeLevelSet()
{
    StageRequest level;
    
    /////////////////////
    /// Level 0   1-A ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
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
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
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
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
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
    /// Level 3   1-D ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.collectionCriteria.push_back(CollectionCriteria(1));
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 4   1-E ///
    /////////////////////
    level = StageRequest();
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 0, 0));
    level.navLevels.push_back(NavigationLevel(0, 0, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 17;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 55;
    stageList.push_back(level);
    
    
    /////////////////////
    /// Level 5   2-A ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 6   2-B ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 7   2-C ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
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
    /// Level 8   2-D ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.collectionCriteria.push_back(CollectionCriteria(2));
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 9   2-E ///
    /////////////////////
    level = StageRequest();
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 17;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 55;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 10  3-A ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.nameTunnelTile = "General/WallBindingA";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music2";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'A';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 11  3-B ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.nameTunnelTile = "General/WallBindingB";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music1";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'B';
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 12  3-C ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 3, 0));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 1));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
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
    /// Level 13  3-D ///
    /////////////////////
    level = StageRequest();
    level.nback = 3;
    level.stageTime = 120.0;
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 3, 1));
    level.navLevels.push_back(NavigationLevel(0, 4, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.collectionCriteria.push_back(CollectionCriteria(3));
    level.nameTunnelTile = "General/WallBindingD";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music3";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'D';
    level.hasHoldout = true;
    level.holdoutFrequency = 4;
    level.initCamSpeed = 15;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 40;
    stageList.push_back(level);
    
    /////////////////////
    /// Level 14  3-E ///
    /////////////////////
    level = StageRequest();
    level.nback = 0;
    level.stageTime = 90.0;
    level.navLevels.push_back(NavigationLevel(0, 1, 0));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.navLevels.push_back(NavigationLevel(0, 1, 1));
    level.navLevels.push_back(NavigationLevel(0, 2, 1));
    level.nameTunnelTile = "General/WallBindingG";
    level.nameSkybox = "General/BlankStarrySkyPlane";
    level.nameMusic = "Music4";
    level.tunnelSectionsPerNavLevel = 10;
    level.phase = 'E';
    level.initCamSpeed = 17;
    level.minCamSpeed = 15;
    level.maxCamSpeed = 55;
    stageList.push_back(level);
}