//
//  LevelManager.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 2/3/14.
//
//

#include "LevelManager.h"

#include "Player.h"
#include "Tunnel.h"

extern Util::ConfigGlobal globals;

LevelManager::LevelManager(Player* player, std::string schedule)
: player(player), schedule(schedule), schedIndex(0)
{
}

GamePhase LevelManager::getPhaseAt(int index) const
{
    // Determine which phase we are in now that schedIndex has been updated
    if (index < schedule.length())
    {
        switch (schedule[index])
        {
            case 'A':
            return PHASE_SET1;
            case 'B':
            return PHASE_SET2;
            case 'C':
            return PHASE_SET3;
            case 'D':
            return PHASE_NAVIGATION;
            case 'E':
            return PHASE_TIMED;
            default:
            return PHASE_SET1;
        }
    }
    return PHASE_DONE;
}

GamePhase LevelManager::getCurrentPhase() const
{
    return getPhaseAt(schedIndex);
}

GamePhase LevelManager::getNextPhase() const
{
    return getPhaseAt(schedIndex + 1);
}

void LevelManager::levelFinishedA(Tunnel* tunnel, Evaluation forced)
{
    Evaluation eval = tunnel->getEval();
    
    PlayerLevel skillLevel = player->getSkillLevel();
    if (eval == PASS || forced == PASS)
    {
        skillLevel.trial++;
        skillLevel.navigation++;
    }
    else if (eval == FAIL || forced == FAIL)
    {
        skillLevel.trial--;
        skillLevel.navigation--;
    }
    player->setSkillLevel(skillLevel);
    
    // Previous version where stages were hardcoded in config file
    //if (eval == PASS || forced == PASS) globals.currStageID++;
    //else if (eval == FAIL || forced == FAIL) globals.currStageID--;
    //if (globals.currStageID < 1) globals.currStageID = 1;
}

// This version does not follow any schedule, it is good for debugging and for testing easy configurations
Tunnel* LevelManager::getNextLevelA(Tunnel* previousTunnel)
{
    // Extract previous information as the previous tunnel still exists
    Vector3 newOrigin = Vector3(0, 0, 0) + globals.tunnelReferenceForward * (globals.tunnelSegmentWidth / 2);
    Quaternion newRot = Quaternion(1, 0, 0, 0);
    Vector3 newForward = globals.tunnelReferenceForward;
    int oldNBack = previousTunnel ? previousTunnel->getNBack() : 0;
    GameMode oldGameMode = previousTunnel ? previousTunnel->getMode() : GAME_TIMED;
    if (previousTunnel)
    {
        delete previousTunnel;
    }
    
    // Load configuration
    if (!globals.loadConfig(globals.currStageID))
    {
        std::cout << "WARNING: Config File could not be loaded correctly" << std::endl;
        globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
    }
    
    PlayerLevel skillLevel = player->getSkillLevel();
    GameMode nmode = GAME_PROFICIENCY;
    int nlevel = skillLevel.trial;
    int ncontrol = 1;
    Tunnel* ret = new Tunnel(
                             OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(),
                             newOrigin + newForward * (globals.tunnelSegmentWidth / 2),
                             newRot,
                             globals.tunnelSegmentWidth,
                             globals.tunnelSegmentDepth,
                             globals.tunnelMinAngleTurn,
                             globals.tunnelMaxAngleTurn,
                             nmode,
                             nlevel,
                             ncontrol,
                             SOUTH,
                             globals.tunnelSegmentsPerSection,
                             globals.tunnelSegmentsPerPod,
                             globals.tunnelSegmentsPerDistractors,
                             globals.signalTypes);
    
    return ret;
}

void LevelManager::levelFinishedB(Tunnel* tunnel, Evaluation forced)
{
    Evaluation eval = tunnel->getEval();
    int signals = tunnel->getTotalCollections();
    int timeLeft = tunnel->getTimeLeft();
    
    // Update Skill Level Here
    PlayerLevel skillLevel = player->getSkillLevel();
    switch (getCurrentPhase())
    {
        case PHASE_SET1:
        {
            if (eval == PASS) skillLevel.set1++;
            break;
        }
        case PHASE_SET2:
        {
            if (eval == PASS) skillLevel.set2++;
            break;
        }
        case PHASE_SET3:
        {
            if (eval == PASS) skillLevel.set3++;
            break;
        }
        case PHASE_NAVIGATION:
        {
            int ds = signals / 6;
            int picked = player->getNumCorrectTotal();
            if (picked < ds) skillLevel.navigation -= 3;
            else if (picked < 2 * ds) skillLevel.navigation -= 2;
            else if (picked < 3 * ds) skillLevel.navigation -= 1;
            else if (picked < 4 * ds) skillLevel.navigation += 0;
            else if (picked < 5 * ds) skillLevel.navigation += 1;
            else if (picked < 6 * ds) skillLevel.navigation += 2;
            else skillLevel.navigation += 3;
            skillLevel.navigation = Util::clamp(skillLevel.navigation, 0, 22);
            break;
        }
        case PHASE_TIMED:
        {
            if (eval == PASS) skillLevel.trial++;
            break;
        }
        case PHASE_DONE:
        break;
    }
    player->setSkillLevel(skillLevel);
    player->saveStage(globals.logPath);
    player->saveProgress(globals.savePath, globals.currStageID);
    
    ++schedIndex;
}

// This version follows a schedule specified when LevelManager was initialized
//
// Very big function...
// Hardcoded settings for five different area. Until the json files are implemented, this will
// be more modular. For now, it'll do.
Tunnel* LevelManager::getNextLevelB(Tunnel* previousTunnel)
{
    // Extract previous information as the previous tunnel still exists
    Vector3 newOrigin = Vector3(0, 0, 0) + globals.tunnelReferenceForward * (globals.tunnelSegmentWidth / 2);
    Quaternion newRot = Quaternion(1, 0, 0, 0);
    Vector3 newForward = globals.tunnelReferenceForward;
    int oldNBack = previousTunnel ? previousTunnel->getNBack() : 0;
    GameMode oldGameMode = previousTunnel ? previousTunnel->getMode() : GAME_TIMED;
    if (previousTunnel)
    {
        delete previousTunnel;
    }
    
    GameMode nmode = GAME_PROFICIENCY;
    int nlevel = 0;
    int ncontrol = 1;
    
    // Load configuration
    if (!globals.loadConfig(globals.currStageID))
    {
        std::cout << "WARNING: Config File could not be loaded correctly" << std::endl;
        globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
    }
    
    PlayerLevel skillLevel = player->getSkillLevel(); // Updated in previous tunnel and referenced for new tunnel
    switch (getCurrentPhase())
    {
        case PHASE_SET1:
        {
            nlevel = skillLevel.set1;
            ncontrol = skillLevel.navigation;
            nmode = GAME_PROFICIENCY;
            
            globals.stageTotalSignals = 60;
            globals.stageTotalTargets1 = 20;
            globals.stageTotalTargets2 = 35;
            globals.stageTotalTargets3 = 45;
            globals.stageTotalTargetsVariance = 2;
            globals.stageTotalCollections = 20;
            globals.initCamSpeed = skillLevel.averageSpeed;
            globals.minCamSpeed = skillLevel.minSpeed;
            globals.maxCamSpeed = skillLevel.maxSpeed;
            globals.stageTime = 120.0;
            
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_UNKNOWN, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_UNKNOWN, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_UNKNOWN, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_UNKNOWN, POD_SOUND_4));
            break;
        }
        case PHASE_SET2:
        {
            nlevel = skillLevel.set2;
            ncontrol = skillLevel.navigation;
            nmode = GAME_PROFICIENCY;
            
            globals.stageTotalSignals = 60;
            globals.stageTotalTargets1 = 20;
            globals.stageTotalTargets2 = 35;
            globals.stageTotalTargets3 = 45;
            globals.stageTotalTargetsVariance = 2;
            globals.stageTotalCollections = 20;
            globals.initCamSpeed = skillLevel.averageSpeed;
            globals.minCamSpeed = skillLevel.minSpeed;
            globals.maxCamSpeed = skillLevel.maxSpeed;
            globals.stageTime = 120.0;
            
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_DIAMOND, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_SPHERE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            break;
        }
        case PHASE_SET3:
        {
            nlevel = skillLevel.set3;
            ncontrol = skillLevel.navigation;
            nmode = GAME_PROFICIENCY;
            
            globals.stageTotalSignals = 60;
            globals.stageTotalTargets1 = 20;
            globals.stageTotalTargets2 = 35;
            globals.stageTotalTargets3 = 45;
            globals.stageTotalTargetsVariance = 2;
            globals.stageTotalCollections = 20;
            globals.initCamSpeed = skillLevel.averageSpeed;
            globals.minCamSpeed = skillLevel.minSpeed;
            globals.maxCamSpeed = skillLevel.maxSpeed;
            globals.stageTime = 120.0;
            
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_4));
            break;
        }
        case PHASE_NAVIGATION:
        {
            nlevel = std::max(0, skillLevel.getMasteredNBack() - 1);
            ncontrol = skillLevel.navigation;
            nmode = GAME_NAVIGATION;
            
            globals.stageTotalSignals = 0;
            globals.stageTotalTargets1 = 0;
            globals.stageTotalTargets2 = 0;
            globals.stageTotalTargets3 = 0;
            globals.stageTotalTargetsVariance = 0;
            globals.initCamSpeed = skillLevel.averageSpeed;
            globals.minCamSpeed = skillLevel.minSpeed;
            globals.maxCamSpeed = skillLevel.maxSpeed;
            globals.stageTime = 120.0;
            globals.stageTotalCollections = Util::computeTotalPossibleOpportunities(skillLevel.averageSpeed,  skillLevel.maxSpeed, globals.tunnelSegmentsPerPod * (globals.tunnelSegmentDepth + globals.tunnelSegmentBuffer) / globals.modifierCamSpeed, globals.stageTime);
            
            break;
        }
        case PHASE_TIMED:
        {
            nlevel = std::max(0, skillLevel.getMasteredNBack() - 1);
            ncontrol = 1;
            nmode = GAME_TIMED;
            
            globals.stageTotalSignals = 75;
            globals.stageTotalTargets1 = 25;
            globals.stageTotalTargets2 = 40;
            globals.stageTotalTargets3 = 60;
            globals.stageTotalTargetsVariance = 2;
            globals.stageTotalCollections = 0;
            globals.stageTotalDistractorsMin = 0;
            globals.stageTotalDistractorsMax = 0;
            globals.initCamSpeed = 15.0;
            globals.minCamSpeed = 10.0;
            globals.maxCamSpeed = 100.0;
            globals.stageTime = 180.0;
            
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_CONE, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_DIAMOND, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_SPHERE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            break;
        }
        default:
            break;
    }
    
    Tunnel* ret = new Tunnel(
                             OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(),
                             newOrigin + newForward * (globals.tunnelSegmentWidth / 2),
                             newRot,
                             globals.tunnelSegmentWidth,
                             globals.tunnelSegmentDepth,
                             globals.tunnelMinAngleTurn,
                             globals.tunnelMaxAngleTurn,
                             nmode,
                             nlevel,
                             ncontrol,
                             SOUTH,
                             globals.tunnelSegmentsPerSection,
                             globals.tunnelSegmentsPerPod,
                             globals.tunnelSegmentsPerDistractors,
                             globals.signalTypes);
    
    return ret;
}
