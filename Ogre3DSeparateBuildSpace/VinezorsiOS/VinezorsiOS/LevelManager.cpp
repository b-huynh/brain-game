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
#include "Networking.h"

extern Util::ConfigGlobal globals;

LevelManager::LevelManager(Player* player, std::string scheduleMain, std::string scheduleRepeat, std::string scheduleRepeatRandomPool)
: player(player), scheduleMainTemplate(scheduleMain), scheduleRepeatTemplate(scheduleRepeat), scheduleRepeatRandomPool(scheduleRepeatRandomPool), scheduleMain(scheduleMain), scheduleRepeat(scheduleRepeat), schedIndexMain(0), schedIndexRepeat(0), schedCount(0)
{
}

GamePhase LevelManager::getPhaseAt(const std::string sched, int index) const
{
    // Determine which phase we are in now that schedIndex has been updated
    if (index < sched.length())
    {
        switch (sched[index])
        {
            case 'A':
                return PHASE_SET1;
            case 'B':
                return PHASE_SET2;
            case 'C':
                return PHASE_SET3;
            case 'F':
                return PHASE_TEACHING;
            case 'G':
                return PHASE_RECESS;
            case 'a':
                return PHASE_SET1;
            case 'b':
                return PHASE_SET2;
            case 'c':
                return PHASE_SET3;
            case 'f':
                return PHASE_TEACHING;
            case 'g':
                return PHASE_RECESS;
            default:
                return PHASE_SET1;
        }
    }
    return PHASE_DONE;
}

GamePhase LevelManager::getCurrentPhase() const
{
    if (schedIndexMain < scheduleMain.size())
        return getPhaseAt(scheduleMain, schedIndexMain);
    else if (schedIndexRepeat < scheduleRepeat.size())
        return getPhaseAt(scheduleRepeat, schedIndexRepeat);
    else
        return PHASE_DONE;
}

char LevelManager::getScheduleValue() const
{
    if (schedIndexMain < scheduleMain.size())
        return scheduleMain[schedIndexMain];
    else if (schedIndexRepeat < scheduleRepeat.size())
        return scheduleRepeat[schedIndexRepeat];
    else
        return 'X';
}

int LevelManager::getSchedNo() const
{
    return schedCount;
}

bool LevelManager::isDoneWithMainSchedule() const
{
    return schedIndexMain >= scheduleMain.size();
}

bool LevelManager::isRepeatingPhase() const
{
    return getScheduleValue() >= 'a' && getScheduleValue() <= 'z';
}

float LevelManager::extractTime(char c) const
{
    PlayerLevel skillLevel = player->getSkillLevel();
    switch (c)
    {
        case 'A':
        {
            if (skillLevel.set1Rep <= 0)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Arep1"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else if (skillLevel.set1Rep <= 1)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Arep2"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Arep3"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            break;
        }
        case 'B':
        {
            if (skillLevel.set2Rep <= 0)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Brep1"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else if (skillLevel.set2Rep <= 1)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Brep2"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Brep3"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            break;
        }
        case 'C':
        {
            if (skillLevel.set3Rep <= 0)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Crep1"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else if (skillLevel.set3Rep <= 1)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Crep2"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Crep3"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            break;
        }
        case 'D':
        {
            if (!configStageType(globals.configPath, globals.configBackup, "D"))
                globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            break;
        }
        case 'E':
        {
            // Ignore Speed Trial...
            return 0.0;
        }
        case 'F':
        {
            if (!configStageType(globals.configPath, globals.configBackup, "F"))
                globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            break;
        }
        case 'G':
        {
            if (!configStageType(globals.configPath, globals.configBackup, "G"))
                globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            break;
        }
        case 'H':
        {
            if (!configStageType(globals.configPath, globals.configBackup, "H"))
                globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            break;
        }
    }
    return globals.stageTime;
}

float LevelManager::extractTotalTime() const
{
    /*
     float totalT = 0.0;
     int numSpeedTrials = 0;
     for (int i = 0; i < scheduleMainTemplate.size(); ++i)
     {
     if (scheduleMainTemplate[i] == 'E')
     ++numSpeedTrials;
     else
     totalT += extractTime(scheduleMainTemplate[i]);
     }
     if (numSpeedTrials <= 0) numSpeedTrials = 1;
     std::cout << "Total Time W/O Speed Trials: " << totalT << std::endl;
     std::cout << "# Speed Trials: " << numSpeedTrials << std::endl;
     float speedTrialLength = (globals.sessionTime - totalT) / numSpeedTrials;
     speedTrialLength = Util::clamp(speedTrialLength, globals.timeTrialLengthMin, globals.timeTrialLengthMax);
     */
    float speedTrialLength = (globals.timeTrialLengthMin + ((globals.timeTrialLengthMax - globals.timeTrialLengthMin) / globals.expectedNumSessions) * player->getSkillLevel().sessionID);
    
    std::cout << "Length Assigned: " << speedTrialLength << std::endl;
    return speedTrialLength;
}

void LevelManager::resetRepeatingSchedule()
{
    schedIndexRepeat = 0;
    scheduleRepeat = scheduleRepeatTemplate;
    
    // Unnecessary to use a vector, can improve by just using string
    std::vector<char> list;
    for (int i = 0; i < scheduleRepeatRandomPool.length(); ++i)
        list.push_back(scheduleRepeatRandomPool[i]);
    
    // Randomize the list
    while (list.size() > 0)
    {
        int rind = rand() % list.size();
        scheduleRepeat += list[rind];
        list[rind] = list[list.size() - 1];
        list.pop_back();
    }
}

void LevelManager::repeatPreviousPhase()
{
    if (schedIndexMain < scheduleMain.size())
    {
        scheduleMain =
            scheduleMain.substr(0, schedIndexMain + 1) +
            char(scheduleMain[schedIndexMain] + ('a' - 'A')) +
            scheduleMain.substr(schedIndexMain + 1);
    }
    else if (schedIndexRepeat < scheduleRepeat.size())
    {
        scheduleRepeat =
            scheduleRepeat.substr(0, schedIndexRepeat + 1) +
            char(scheduleRepeat[schedIndexRepeat] + ('a' - 'A')) +
            scheduleRepeat.substr(schedIndexRepeat + 1);
    }
}

void LevelManager::incrementSchedIndex()
{
    ++schedCount;
    if (schedIndexMain < scheduleMain.size())
    {
        ++schedIndexMain;
        if (schedIndexMain >= scheduleMain.size())
            resetRepeatingSchedule();
    }
    else
    {
        ++schedIndexRepeat;
        if (schedIndexRepeat >= scheduleRepeat.size())
            resetRepeatingSchedule();
    }
}

void LevelManager::decrementSchedIndex()
{
    if (schedIndexMain > 0 && schedIndexMain < scheduleMain.size())
    {
        --schedCount;
        --schedIndexMain;
        if (schedIndexMain >= scheduleMain.size())
            resetRepeatingSchedule();
    }
}

bool LevelManager::levelFinished(Tunnel* tunnel, Evaluation forced)
{
    Evaluation eval = tunnel->getEval();
    bool allowRetry = false;
    switch (getCurrentPhase())
    {
        case PHASE_SET1:
        {
            if (eval == PASS || forced == PASS)
            {
            }
            else allowRetry = forced == EVEN && !isRepeatingPhase();
            break;
        }
        case PHASE_SET2:
        {
            if (eval == PASS || forced == PASS)
            {
            }
            else allowRetry = forced == EVEN && !isRepeatingPhase();
            break;
        }
        case PHASE_SET3:
        {
            if (eval == PASS || forced == PASS)
            {
            }
            else allowRetry = forced == EVEN && !isRepeatingPhase();
            break;
        }
        case PHASE_TEACHING:
        {
            if (eval == PASS || forced == PASS)
            {}
            else allowRetry = forced == EVEN;
            break;
        }
        case PHASE_DONE:
            break;
    }
    return allowRetry;
}

void LevelManager::updatePlayerSkill(Tunnel* tunnel, Evaluation forced)
{
    Evaluation eval = tunnel->getEval();
    PlayerLevel skillLevel = player->getSkillLevel();
    switch (getCurrentPhase())
    {
        case PHASE_SET1:
        {
            skillLevel.runSpeed1 = player->getBaseSpeed();
            skillLevel.set1Notify = 0;
            if (eval == PASS || forced == PASS)
            {
                skillLevel.set1Rep++;
                if (skillLevel.set1Rep >= globals.set1Repetitions)
                {
                    skillLevel.runSpeed1 *= globals.nlevelSpeedModifier;
                    skillLevel.runSpeed1 = Util::clamp(skillLevel.runSpeed1, globals.minCamSpeed, globals.maxCamSpeed);
                    skillLevel.set1Rep = 0;
                    skillLevel.set1Notify = 1;
                    skillLevel.set1++;
                }
            }
            else if (eval == FAIL || forced == FAIL)
            {
                skillLevel.set1Rep--;
                if (skillLevel.set1Rep < 0)
                {
                    if (skillLevel.set1 > 1)
                    {
                        skillLevel.set1Rep = globals.set1Repetitions - 1;
                        skillLevel.set1--;
                    }
                    else
                        skillLevel.set1Rep = 0;
                }
            }
            break;
        }
        case PHASE_SET2:
        {
            skillLevel.runSpeed2 = player->getBaseSpeed();
            skillLevel.set2Notify = 0;
            if (eval == PASS || forced == PASS)
            {
                skillLevel.set2Rep++;
                if (skillLevel.set2Rep >= globals.set2Repetitions)
                {
                    skillLevel.runSpeed2 *= globals.nlevelSpeedModifier;
                    skillLevel.runSpeed2 = Util::clamp(skillLevel.runSpeed2, globals.minCamSpeed, globals.maxCamSpeed);
                    skillLevel.set2Rep = 0;
                    skillLevel.set2Notify = 1;
                    skillLevel.set2++;
                }
            }
            else if (eval == FAIL || forced == FAIL)
            {
                skillLevel.set2Rep--;
                if (skillLevel.set2Rep < 0)
                {
                    if (skillLevel.set2 > 1)
                    {
                        skillLevel.set2Rep = globals.set2Repetitions - 1;
                        skillLevel.set2--;
                    }
                    else
                        skillLevel.set2Rep = 0;
                }
            }
            break;
        }
        case PHASE_SET3:
        {
            skillLevel.runSpeed3 = player->getBaseSpeed();
            skillLevel.set3Notify = 0;
            if (eval == PASS || forced == PASS)
            {
                skillLevel.set3Rep++;
                if (skillLevel.set3Rep >= globals.set3Repetitions)
                {
                    skillLevel.runSpeed3 *= globals.nlevelSpeedModifier;
                    skillLevel.runSpeed3 = Util::clamp(skillLevel.runSpeed3, globals.minCamSpeed, globals.maxCamSpeed);
                    skillLevel.set3Rep = 0;
                    skillLevel.set3Notify = 1;
                    skillLevel.set3++;
                }
            }
            else if (eval == FAIL || forced == FAIL)
            {
                skillLevel.set3Rep--;
                if (skillLevel.set3Rep < 0)
                {
                    if (skillLevel.set3 > 1)
                    {
                        skillLevel.set3Rep = globals.set3Repetitions - 1;
                        skillLevel.set3--;
                    }
                    else
                        skillLevel.set3Rep = 0;
                }
            }
            break;
        }
        default:
            break;
    }
    player->setSkillLevel(skillLevel);
    player->saveStage(globals.logPath);
    player->saveActions(globals.actionPath);
    player->saveSession(globals.sessionPath);
    player->saveProgress(globals.savePath, globals.currStageID);
}

// This version follows a schedule specified when LevelManager was initialized
//
// Very big function...
// Hardcoded settings for five different area. Until the json files are implemented, this will
// be more modular. For now, it'll do.
Tunnel* LevelManager::getNextLevel(Tunnel* previousTunnel)
{
    // Extract previous information as the previous tunnel still exists
    Vector3 newOrigin = Vector3(0, 0, 0) + globals.tunnelReferenceForward * (globals.tunnelSegmentWidth / 2);
    Quaternion newRot = Quaternion(1, 0, 0, 0);
    Vector3 newForward = globals.tunnelReferenceForward;
    int oldNBack = previousTunnel ? previousTunnel->getNBack() : 0;
    GameMode oldGameMode = previousTunnel ? previousTunnel->getMode() : GAME_PROFICIENCY;
    if (previousTunnel)
    {
        delete previousTunnel;
    }
    
    GameMode nmode = GAME_PROFICIENCY;
    int nlevel = 0;
    
    if (!configStageType(globals.configPath, globals.configBackup, "globalConfig"))
        globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
    
    PlayerLevel skillLevel = player->getSkillLevel(); // Updated in previous tunnel and referenced for new tunnel
    switch (getCurrentPhase())
    {
        case PHASE_SET1:
        {
            nlevel = skillLevel.set1;
            nmode = GAME_PROFICIENCY;
            
            globals.initCamSpeed = skillLevel.runSpeed1;
            globals.maxCamSpeed = skillLevel.maxSpeed;
            
            if (skillLevel.set1Rep <= 0)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Arep1"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else if (skillLevel.set1Rep <= 1)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Arep2"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Arep3"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            
            globals.stageTotalTargets1 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
            globals.stageTotalTargets2 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
            globals.stageTotalTargets3 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
            
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_CONE, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_CONE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_CONE, POD_SOUND_4));
            
            if (skillLevel.set1Notify)
                globals.setBigMessage("Congratulations! You earned " + Util::toStringInt(nlevel) + "-Back!");
            else
            {
                if (skillLevel.set1Rep >= 2)
                    globals.setBigMessage(Util::toStringInt(nlevel) + "-Back. Challenge Round!");
                else
                    globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            }
            globals.setMessage("Match by Color!", MESSAGE_NORMAL);
            break;
        }
        case PHASE_SET2:
        {
            nlevel = skillLevel.set2;
            nmode = GAME_PROFICIENCY;
            
            globals.initCamSpeed = skillLevel.runSpeed2;
            globals.maxCamSpeed = skillLevel.maxSpeed;
            
            if (skillLevel.set2Rep <= 0)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Brep1"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else if (skillLevel.set2Rep <= 1)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Brep2"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Brep3"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            
            globals.stageTotalTargets1 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
            globals.stageTotalTargets2 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
            globals.stageTotalTargets3 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
            
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_DIAMOND, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_SPHERE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_CONE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            
            if (skillLevel.set2Notify)
                globals.setBigMessage("Congratulations! You earned " + Util::toStringInt(nlevel) + "-Back!");
            else
            {
                if (skillLevel.set2Rep >= 2)
                    globals.setBigMessage(Util::toStringInt(nlevel) + "-Back. Challenge Round!");
                else
                    globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            }
            globals.setMessage("Match by Shape!", MESSAGE_NORMAL);
            break;
        }
        case PHASE_SET3:
        {
            nlevel = skillLevel.set3;
            nmode = GAME_PROFICIENCY;
            
            globals.initCamSpeed = skillLevel.runSpeed3;
            globals.maxCamSpeed = skillLevel.maxSpeed;
            
            if (skillLevel.set3Rep <= 0)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Crep1"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else if (skillLevel.set3Rep <= 1)
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Crep2"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            else
            {
                if (!configStageType(globals.configPath, globals.configBackup, "Crep3"))
                    globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            }
            
            globals.stageTotalTargets1 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
            globals.stageTotalTargets2 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
            globals.stageTotalTargets3 = globals.stageTotalSignals * (globals.podNBackChance / 100.0);
            
            globals.signalTypes = std::vector<std::vector<PodInfo> >(4);
            globals.signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_TRIANGLE, POD_SOUND_1));
            globals.signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_TRIANGLE, POD_SOUND_2));
            globals.signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_TRIANGLE, POD_SOUND_3));
            globals.signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_UNKNOWN, POD_SHAPE_TRIANGLE, POD_SOUND_4));
            
            if (skillLevel.set3Notify)
                globals.setBigMessage("Congratulations! You earned " + Util::toStringInt(nlevel) + "-Back!");
            else
            {
                if (skillLevel.set3Rep >= 2)
                    globals.setBigMessage(Util::toStringInt(nlevel) + "-Back. Challenge Round!");
                else
                    globals.setBigMessage(Util::toStringInt(nlevel) + "-Back");
            }
            globals.setMessage("Match by Only Sound!", MESSAGE_NORMAL);
            break;
        }
        case PHASE_TEACHING:
        {
            nlevel = 1;
            nmode = GAME_TEACHING;
            
            if (!configStageType(globals.configPath, globals.configBackup, "F"))
                globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            
            globals.setBigMessage("Introduction");
            globals.setMessage("Move to Grab Fuel Cells!", MESSAGE_NORMAL);
            break;
        }
        case PHASE_RECESS:
        {
            nlevel = std::max(0, skillLevel.getMasteredNBack() - 1);
            if (nlevel < 1) nlevel = 1;
            nmode = GAME_RECESS;
            
            globals.initCamSpeed = skillLevel.averageSpeed;
            
            if (!configStageType(globals.configPath, globals.configBackup, "G"))
                globals.setMessage("WARNING: Failed to read configuration", MESSAGE_ERROR);
            
            globals.stageTotalCollections = skillLevel.averageSpeed * globals.stageTime / Util::getModdedLengthByNumSegments(globals, globals.tunnelSegmentsPerPod);
            
            globals.setBigMessage("Recess!");
            globals.setMessage("Avoid Obstacles. Grab Fuel Cells!", MESSAGE_NORMAL);
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
                             getSchedNo(),
                             nmode,
                             getScheduleValue(),
                             nlevel,
                             SOUTH,
                             globals.tunnelSegmentsPerSection,
                             globals.tunnelSegmentsPerPod,
                             globals.tunnelSegmentsPerDistractors,
                             globals.signalTypes);
    
    return ret;
}
