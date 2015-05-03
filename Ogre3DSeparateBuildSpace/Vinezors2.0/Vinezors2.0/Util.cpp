//
//  Util.cpp
//  Testing
//
//  Created by Calvin Phung on 8/27/13.
//
//

#include "Util.h"

#include <vector>
#include <cstdlib>
#include <sstream>
#include <iomanip>

Util::ConfigGlobal::ConfigGlobal()
{
    scheduleMain = "GABC";
    scheduleRepeat = "G";
    scheduleRepeatRandomPool = "ABC";
    stageID = 0;
    sessionTime = 1200.0;
    stageTime = 120.0;
    stageTotalSignals = 120;
    stageTotalTargets = 40;
    stageTotalCollections = 25;
    stageTotalTargetsVariance = 0;
    set1Repetitions = 3;
    set2Repetitions = 3;
    set3Repetitions = 3;
    set1StartingSpeed = 15;
    set2StartingSpeed = 15;
    set3StartingSpeed = 15;
    trialStartingSpeed = 20;
    startingNav = 2;
    tunnelReferenceForward = Vector3(0, 0, -1);
    tunnelReferenceUpward = Vector3(0, 1, 0);
    tunnelReferenceRight = Vector3(1, 0, 0);
    tunnelMinAngleTurn = 0;
    tunnelMaxAngleTurn = 3;
    tunnelSegmentWidth = 25.0;
    tunnelSegmentDepth = 25.0;
    tunnelSegmentBuffer = 25.0;
    tunnelWallLength = tunnelSegmentWidth / (2 * Math::Cos(Ogre::Radian(Math::PI) / 4) + 1);
    tunnelSegmentsPerSection = 4;
    tunnelSegmentsPerPod = 4;
    tunnelSegmentsPerDistractors = 4;
    tunnelSegmentsPerPowerup = 4;
    tunnelSegmentsBeforeRefresh = tunnelSegmentsPerSection * 2;
    tunnelSectionsPerNavigationUpgrade = 10;
    initialSegmentsFirstPod = 3;
    initialSegmentsFirstDistractors = 5;
    initialSegmentsFirstPowerup = 6;
    initiationSections = 1;
    vineTOffset = 1.1;
    vineRadius = tunnelSegmentWidth / 15.0;
    vineTransitionSpeed = 5.0;
    podAppearance = 3;
    podHeadRadius = tunnelSegmentWidth / 25.0;
    podStemRadius = tunnelSegmentWidth / 100.0;
    podStemLength = tunnelWallLength / 2.0;
    podRotateSpeed = 5.0;
    podCollisionMin = 0.25; // 0.20
    podCollisionMax = 0.65; // 0.40
    distractorCollisionMin = 0.40; // 0.25
    distractorCollisionMax = 0.50; // 0.35
    podBinSize = 10;
    podNBackChance = 33.0;
    span = 1;
    stageTotalDistractorsMin = 1;
    stageTotalDistractorsMax = 1;
    seatLength = podHeadRadius * 2;
    tunnelSections = 5;
    nback = 2;
    control = 1;
    historyMode = -1;
    startingHP = 3;
    HPNegativeLimit = 0;
    HPPositiveLimit = 999;
    HPNegativeCorrectAnswer = 0;
    HPNegativeWrongAnswer = -1;
    HPNegativeDistractor = 0;
    HPPositiveCorrectAnswer = 0;
    HPPositiveWrongAnswer = -1;
    HPPositiveDistractor = 0;
    wrongAnswerTimePenalty = 0.0;
    distractorSpeedPenalty = 1.0;
    distractorTimePenalty = 0.0;
    //fuelMax = 450.0;
    //fuelReturn = 60.0;
    fuelMax = 30.0;
    fuelReturn = 5.0;
    initCamSpeed = 15.0;
    startupCamSpeed = 60.0;
    globalModifierCamSpeed = 5.0;
    boostModifierCamSpeed = 1.25;
    minCamSpeed = 15.0;
    maxCamSpeed = 40.0;
    //baselineSpeed = 15.0 * globalModifierCamSpeed;
    baselineSpeed = 15.0;
    nlevelSpeedModifier = 0.8;
    numToSpeedUp = 2;
    numToSpeedDown = 1;
    stepsizeSpeedUp = 1.0;
    stepsizeSpeedDown = -1.0;
    HPBarXRef = 0.05;
    HPBarYRef = 0.05;
    HPBarWidth = 0.9;
    HPBarHeight = 0.05;
    screenWidth = 1024;
    screenHeight = 800;
    setSkyBox = 2;
    setWallPanelTexture = 0;
    setVineShip = VINE_RUNNER_SHIP;
    swipeSensitivity = 12.0;
    swipeInverted = 0;
    boostEnabled = 1;
    combo1MinA = 5;
    combo2MinA = 10;
    combo1MinB = 3;
    combo2MinB = 6;
    numNavPhases = 4;
    numTimePhases = 4;
    expectedNumSessions = 20;
    sessionTimeMin = 480;
    sessionTimeMax = 1800;
    timeTrialLengthMin = 60;
    timeTrialLengthMax = 300;
    navUnlockMax0 = 7;
    navUnlockNBack1 = 4;
    navUnlockMax1 = 15;
    navUnlockNBack2 = 6;
    navUnlockMax2 = 20;
    volumeMusic = 0.25;
    volumeSignal1 = 0.25;
    volumeSignal2 = 0.25;
    volumeSignal3 = 0.25;
    volumeSignal4 = 0.25;
    volumeFeedbackGood = 0.25;
    volumeFeedbackBad = 0.25;
    volumeFeedbackCollision = 0.25;
    volumeBoost = 0.25;
    volumeStartup = 0.25;
    numSegmentsWithObstacles = 0;
    previousNumSegmentsWithObstacles = 0;
    
    signalTypes = std::vector<std::vector<PodInfo> >(4);
    signalTypes[POD_SIGNAL_1].push_back(PodInfo(POD_SIGNAL_1, POD_FUEL, POD_COLOR_BLUE, POD_SHAPE_CONE, POD_SOUND_1));
    signalTypes[POD_SIGNAL_2].push_back(PodInfo(POD_SIGNAL_2, POD_FUEL, POD_COLOR_GREEN, POD_SHAPE_SPHERE, POD_SOUND_2));
    signalTypes[POD_SIGNAL_3].push_back(PodInfo(POD_SIGNAL_3, POD_FUEL, POD_COLOR_PINK, POD_SHAPE_DIAMOND, POD_SOUND_3));
    signalTypes[POD_SIGNAL_4].push_back(PodInfo(POD_SIGNAL_4, POD_FUEL, POD_COLOR_YELLOW, POD_SHAPE_TRIANGLE, POD_SOUND_4));
    
    //navMap = std::vector<NavigationLevel>(1);
    //navMap[0] = NavigationLevel(0, 4, 3);
    
    
    fixedNavMap = std::vector<std::vector<NavigationLevel> >(23);
    fixedNavMap[0].push_back(NavigationLevel(0, 1, 0));
    fixedNavMap[0].push_back(NavigationLevel(1, 1, 0));
    fixedNavMap[0].push_back(NavigationLevel(2, 1, 0));
    fixedNavMap[0].push_back(NavigationLevel(3, 1, 0));
    
    fixedNavMap[1].push_back(NavigationLevel(0, 1, 0));
    fixedNavMap[1].push_back(NavigationLevel(1, 1, 0));
    fixedNavMap[1].push_back(NavigationLevel(2, 2, 0));
    fixedNavMap[1].push_back(NavigationLevel(3, 1, 0));
    
    fixedNavMap[2].push_back(NavigationLevel(0, 1, 0));
    fixedNavMap[2].push_back(NavigationLevel(1, 2, 0));
    fixedNavMap[2].push_back(NavigationLevel(2, 1, 0));
    fixedNavMap[2].push_back(NavigationLevel(3, 2, 0));
    
    fixedNavMap[3].push_back(NavigationLevel(0, 2, 0));
    fixedNavMap[3].push_back(NavigationLevel(1, 2, 0));
    fixedNavMap[3].push_back(NavigationLevel(2, 2, 0));
    fixedNavMap[3].push_back(NavigationLevel(3, 2, 0));
    
    fixedNavMap[4].push_back(NavigationLevel(0, 1, 0));
    fixedNavMap[4].push_back(NavigationLevel(1, 2, 0));
    fixedNavMap[4].push_back(NavigationLevel(2, 2, 1));
    fixedNavMap[4].push_back(NavigationLevel(3, 2, 1));
    
    fixedNavMap[5].push_back(NavigationLevel(0, 2, 0));
    fixedNavMap[5].push_back(NavigationLevel(1, 2, 1));
    fixedNavMap[5].push_back(NavigationLevel(2, 2, 0));
    fixedNavMap[5].push_back(NavigationLevel(3, 2, 1));
    
    fixedNavMap[6].push_back(NavigationLevel(0, 2, 1));
    fixedNavMap[6].push_back(NavigationLevel(1, 2, 1));
    fixedNavMap[6].push_back(NavigationLevel(2, 2, 1));
    fixedNavMap[6].push_back(NavigationLevel(3, 2, 1));
    
    fixedNavMap[7].push_back(NavigationLevel(0, 1, 0));
    fixedNavMap[7].push_back(NavigationLevel(1, 2, 1));
    fixedNavMap[7].push_back(NavigationLevel(2, 1, 1));
    fixedNavMap[7].push_back(NavigationLevel(3, 2, 1));
    
    fixedNavMap[8].push_back(NavigationLevel(0, 1, 0));
    fixedNavMap[8].push_back(NavigationLevel(1, 1, 1));
    fixedNavMap[8].push_back(NavigationLevel(2, 1, 1));
    fixedNavMap[8].push_back(NavigationLevel(3, 1, 0));
    
    fixedNavMap[9].push_back(NavigationLevel(0, 2, 0));
    fixedNavMap[9].push_back(NavigationLevel(1, 1, 1));
    fixedNavMap[9].push_back(NavigationLevel(2, 2, 1));
    fixedNavMap[9].push_back(NavigationLevel(3, 1, 1));
    
    fixedNavMap[10].push_back(NavigationLevel(0, 1, 1));
    fixedNavMap[10].push_back(NavigationLevel(1, 1, 1));
    fixedNavMap[10].push_back(NavigationLevel(2, 1, 1));
    fixedNavMap[10].push_back(NavigationLevel(3, 1, 1));
    
    fixedNavMap[11].push_back(NavigationLevel(0, 1, 0));
    fixedNavMap[11].push_back(NavigationLevel(1, 2, 1));
    fixedNavMap[11].push_back(NavigationLevel(2, 2, 2));
    fixedNavMap[11].push_back(NavigationLevel(3, 1, 1));
    
    fixedNavMap[12].push_back(NavigationLevel(0, 2, 0));
    fixedNavMap[12].push_back(NavigationLevel(1, 2, 2));
    fixedNavMap[12].push_back(NavigationLevel(2, 2, 0));
    fixedNavMap[12].push_back(NavigationLevel(3, 2, 2));
    
    fixedNavMap[13].push_back(NavigationLevel(0, 1, 1));
    fixedNavMap[13].push_back(NavigationLevel(1, 2, 2));
    fixedNavMap[13].push_back(NavigationLevel(2, 1, 1));
    fixedNavMap[13].push_back(NavigationLevel(3, 2, 2));
    
    fixedNavMap[14].push_back(NavigationLevel(0, 2, 2));
    fixedNavMap[14].push_back(NavigationLevel(1, 2, 2));
    fixedNavMap[14].push_back(NavigationLevel(2, 2, 2));
    fixedNavMap[14].push_back(NavigationLevel(3, 2, 2));
    
    fixedNavMap[15].push_back(NavigationLevel(0, 1, 1));
    fixedNavMap[15].push_back(NavigationLevel(1, 2, 2));
    fixedNavMap[15].push_back(NavigationLevel(2, 1, 2));
    fixedNavMap[15].push_back(NavigationLevel(3, 2, 2));
    
    fixedNavMap[16].push_back(NavigationLevel(0, 1, 1));
    fixedNavMap[16].push_back(NavigationLevel(1, 1, 2));
    fixedNavMap[16].push_back(NavigationLevel(2, 1, 1));
    fixedNavMap[16].push_back(NavigationLevel(3, 1, 2));
    
    fixedNavMap[17].push_back(NavigationLevel(0, 2, 2));
    fixedNavMap[17].push_back(NavigationLevel(1, 1, 2));
    fixedNavMap[17].push_back(NavigationLevel(2, 1, 2));
    fixedNavMap[17].push_back(NavigationLevel(3, 2, 2));
    
    fixedNavMap[18].push_back(NavigationLevel(0, 1, 2));
    fixedNavMap[18].push_back(NavigationLevel(1, 1, 2));
    fixedNavMap[18].push_back(NavigationLevel(2, 1, 2));
    fixedNavMap[18].push_back(NavigationLevel(3, 1, 2));
    
    fixedNavMap[19].push_back(NavigationLevel(0, 2, 2));
    fixedNavMap[19].push_back(NavigationLevel(1, 2, 2));
    fixedNavMap[19].push_back(NavigationLevel(2, 2, 3));
    fixedNavMap[19].push_back(NavigationLevel(3, 2, 3));
    
    fixedNavMap[20].push_back(NavigationLevel(0, 2, 2));
    fixedNavMap[20].push_back(NavigationLevel(1, 2, 3));
    fixedNavMap[20].push_back(NavigationLevel(2, 1, 2));
    fixedNavMap[20].push_back(NavigationLevel(3, 2, 3));
    
    fixedNavMap[21].push_back(NavigationLevel(0, 2, 3));
    fixedNavMap[21].push_back(NavigationLevel(1, 2, 3));
    fixedNavMap[21].push_back(NavigationLevel(2, 2, 3));
    fixedNavMap[21].push_back(NavigationLevel(3, 2, 3));
    
    fixedNavMap[22].push_back(NavigationLevel(0, 2, 3));
    fixedNavMap[22].push_back(NavigationLevel(1, 2, 3));
    fixedNavMap[22].push_back(NavigationLevel(2, 2, 4));
    fixedNavMap[22].push_back(NavigationLevel(3, 2, 3));
    
    
    navMap = std::vector<NavigationLevel>(23);
    

    
     // Debug Nav Levels
    /*
    navMap[0] = NavigationLevel(0, 4, 3);
    navMap[1] = NavigationLevel(1, 3, 3);
    navMap[2] = NavigationLevel(2, 2, 2);
    navMap[3] = NavigationLevel(3, 1, 1);
    navMap[4] = NavigationLevel(4, 4, 3);
    navMap[5] = NavigationLevel(5, 2, 2);
    navMap[6] = NavigationLevel(6, 4, 3);
    navMap[7] = NavigationLevel(7, 1, 1);
    navMap[8] = NavigationLevel(8, 1, 1);
    navMap[9] = NavigationLevel(9, 3, 3);
    navMap[10] = NavigationLevel(10, 3, 3);
    navMap[11] = NavigationLevel(11, 4, 3);
     */
    
    navMap[0] = NavigationLevel(0, 1, 0);
    navMap[1] = NavigationLevel(1, 2, 0);
    navMap[2] = NavigationLevel(2, 3, 0);
    navMap[3] = NavigationLevel(3, 4, 0);
    navMap[4] = NavigationLevel(4, 2, 1);
    navMap[5] = NavigationLevel(5, 1, 1);
    
    navMap[6] = NavigationLevel(6, 4, 1);
    navMap[7] = NavigationLevel(7, 2, 2);
    navMap[8] = NavigationLevel(8, 1, 2);
    navMap[9] = NavigationLevel(9, 4, 2);
    navMap[10] = NavigationLevel(10, 3, 1);
    navMap[11] = NavigationLevel(11, 3, 2);
    
    navMap[12] = NavigationLevel(12, 4, 3);
    navMap[13] = NavigationLevel(13, 2, 3);
    navMap[14] = NavigationLevel(14, 2, 4);
    navMap[15] = NavigationLevel(15, 3, 3);
    navMap[16] = NavigationLevel(16, 4, 4);
    navMap[17] = NavigationLevel(17, 4, 5);
    navMap[18] = NavigationLevel(18, 3, 4);
    navMap[19] = NavigationLevel(19, 4, 6);
    navMap[20] = NavigationLevel(20, 3, 5);
    navMap[21] = NavigationLevel(21, 3, 6);
    navMap[22] = NavigationLevel(22, 4, 7);
    navIndex = 0;
    
    speedMap[1] = 0.5;
    speedMap[2] = 0.5;
    speedMap[3] = 0.5;
    speedMap[4] = 0.5;
    speedMap[5] = 0.5;
    speedMap[6] = 0.5;
    speedMap[7] = 0.5;
    speedMap[8] = 0.5;
    speedMap[9] = 0.5;
    speedMap[10] = 3.0;
    speedMap[11] = 2.0;
    speedMap[12] = 1.0;
    speedMap[13] = 2.0;
    speedMap[14] = 1.0;
    speedMap[15] = 2.0;
    speedMap[16] = 1.0;
    speedMap[17] = 2.0;
    speedMap[18] = 1.0;
    speedMap[19] = 1.0;
    speedMap[20] = 1.0;
    speedMap[21] = 1.0;
    speedMap[22] = 1.0;
    speedMap[23] = 1.0;
    speedMap[24] = 1.0;
    speedMap[25] = 0.5;
    speedMap[26] = 0.5;
    speedMap[27] = 0.5;
    speedMap[28] = 0.5;
    speedMap[29] = 0.5;
    speedMap[30] = 0.5;
    speedMap[31] = 0.5;
    speedMap[32] = 0.5;
    speedMap[33] = 0.5;
    speedMap[34] = 0.5;
    speedMap[35] = 0.5;
    speedMap[36] = 0.5;
    speedMap[37] = 0.5;
    speedMap[38] = 0.5;
    speedMap[39] = 0.5;
    speedMap[40] = 0.5;
    speedMap[41] = 0.0;
    speedMap[42] = 0.0;
    speedMap[43] = 0.0;
    speedMap[44] = 0.0;
    speedMap[45] = 0.0;
    speedMap[46] = 0.0;
    speedMap[47] = 0.0;
    speedMap[48] = 0.0;
    speedMap[49] = 0.0;
    speedMap[50] = 0.0;
    /*
    speedMap[1] = 5.0;
    speedMap[2] = 5.0;
    speedMap[3] = 5.0;
    speedMap[4] = 5.0;
    speedMap[5] = 5.0;
    speedMap[6] = 5.5;
    speedMap[7] = 6.0;
    speedMap[8] = 6.5;
    speedMap[9] = 7.0;
    speedMap[10] = 7.5;
    speedMap[11] = 8.0;
    speedMap[12] = 8.5;
    speedMap[13] = 9.0;
    speedMap[14] = 9.5;
    speedMap[15] = 10.0;
    speedMap[16] = 13.0;
    speedMap[17] = 15.0;
    speedMap[18] = 17.0;
    speedMap[19] = 19.0;
    speedMap[20] = 20.0;
    speedMap[21] = 21.0;
    speedMap[22] = 22.0;
    speedMap[23] = 23.0;
    speedMap[24] = 24.0;
    speedMap[25] = 25.0;
    speedMap[26] = 25.5;
    speedMap[27] = 26.0;
    speedMap[28] = 26.5;
    speedMap[29] = 27.0;
    speedMap[30] = 27.5;
    speedMap[31] = 28.0;
    speedMap[32] = 28.5;
    speedMap[33] = 29.0;
    speedMap[34] = 29.5;
    speedMap[35] = 30.0;
    speedMap[36] = 30.5;
    speedMap[37] = 31.0;
    speedMap[38] = 31.5;
    speedMap[39] = 32.0;
    speedMap[40] = 32.5;
    speedMap[41] = 33.0;
    speedMap[42] = 33.5;
    speedMap[43] = 34.0;
    speedMap[44] = 34.5;
    speedMap[45] = 35.0;
    speedMap[46] = 35.5;
    speedMap[47] = 36.0;
    speedMap[48] = 36.5;
    speedMap[49] = 37.0;
    speedMap[50] = 37.5;
    speedMap[51] = 38.0;
    speedMap[52] = 38.5;
    speedMap[53] = 39.0;
    speedMap[54] = 39.5;
    speedMap[55] = 40.0;
    speedMap[56] = 40.0;
    speedMap[57] = 40.0;
    speedMap[58] = 40.0;
    speedMap[59] = 40.0;
    speedMap[60] = 40.0;
     */
    playerName = "subject100";
    sessionScreenEnabled = false;
}

// Updates variables that depend on other globals, should call this if a game global has changed
void Util::ConfigGlobal::set()
{
    viewportMainWidth_modeRight = screenWidth - 224;
    viewportMainHeight_modeRight = screenHeight;
    viewportSideWidth_modeRight = 224;
    viewportSideHeight_modeRight = screenHeight;
    
    viewportMainWidth_modeBottom = screenWidth;
    viewportMainHeight_modeBottom = screenHeight - 100;
    viewportSideWidth_modeBottom = screenWidth;
    viewportSideHeight_modeBottom = 100;
    
    viewportMainWidth_modeNone = screenWidth;
    viewportMainHeight_modeNone = screenHeight;
    viewportSideWidth_modeNone = 0;
    viewportSideHeight_modeNone = 0;
    
    label1_posX = screenWidth / 2;
#ifdef DEBUG_MODE
    label1_posY = screenHeight / 2 - 5 * screenHeight / 5;
#else
    label1_posY = screenHeight / 2 - 1.25 * screenHeight / 5;
#endif
    label2_posX = screenWidth / 80;
    label2_posY = 7 * screenHeight / 40;
    label3_posX = screenWidth - screenWidth / 40;
    label3_posY = 5 * screenHeight / 40;
    label4_posX = screenWidth / 80;
    label4_posY = 5 * screenHeight / 40;
    label5_posX = screenWidth - screenWidth / 40;
    label5_posY = 7 * screenHeight / 40;
    label6_posX = screenWidth / 2;
    label6_posY = screenHeight / 2 - screenHeight / 3;
    label7_posX = screenWidth / 7.5;
    label7_posY = screenHeight - screenHeight / 10;
    
    pauseButton_posX = screenWidth - screenWidth / 6;
    pauseButton_posY = screenHeight - screenHeight / 6;
    pauseButton_width = screenWidth / 20;
    pauseButton_height = pauseButton_width;
}

Vector2 Util::ConfigGlobal::convertToPercentScreen(Vector2 p)
{
    return Vector2(p.x / screenWidth, p.y / screenHeight);
}

void Util::ConfigGlobal::initPaths()
{
#if defined(OGRE_IS_IOS)
    savePath = Util::getIOSDir() + "/" + playerName + "/" + playerName + ".save";
    configPath = Util::getIOSDir() + "/" + playerName + "/config.json";
    configBackup = Util::getIOSDir() + "/config.json";
    globalPath = Util::getIOSDir() + "/globalSettings.save";
#else
    savePath = Util::getOSXDir() + "/" + playerName + "/" + playerName + ".save";
    configPath = Util::getOSXDir() + "/" + playerName + "/" + playerName + ".json";
    configBackup = Util::getOSXDir() + "/config.json";
    globalPath = Util::getOSXDir() + "/globalSettings.save";
#endif
}

void Util::ConfigGlobal::initLogs(int session)
{
    // This is computed after player save is loaded for session ID
    //Build log path
    logPath = buildPath(".log", playerName, session);
    actionPath = buildPath(".act", playerName, session);
    sessionPath = buildPath(".session", playerName, session);
}

void Util::ConfigGlobal::setConfigValue(std::istream& in, std::string paramName)
{
    if (paramName == "playerName")
        in >> playerName;
    else if (paramName == "scheduleMain")
        in >> scheduleMain;
    else if (paramName == "scheduleRepeat")
        in >> scheduleRepeat;
    else if (paramName == "scheduleRepeatRandomPool")
        in >> scheduleRepeatRandomPool;
    else if (paramName == "stageID")
        in >> stageID;
    else if (paramName == "sessionTime")
        in >> sessionTime;
    else if (paramName == "stageTime")
        in >> stageTime;
    else if (paramName == "stageTotalSignals")
        in >> stageTotalSignals;
    else if (paramName == "stageTotalTargets")
        in >> stageTotalTargets;
    else if (paramName == "stageTotalCollections")
        in >> stageTotalCollections;
    else if (paramName == "stageTotalTargetsVariance")
        in >> stageTotalTargetsVariance;
    else if (paramName == "set1Repetitions")
        in >> set1Repetitions;
    else if (paramName == "set2Repetitions")
        in >> set2Repetitions;
    else if (paramName == "set3Repetitions")
        in >> set3Repetitions;
    else if (paramName == "set1StartingSpeed")
        in >> set1StartingSpeed;
    else if (paramName == "set2StartingSpeed")
        in >> set2StartingSpeed;
    else if (paramName == "set3StartingSpeed")
        in >> set3StartingSpeed;
    else if (paramName == "trialStartingSpeed")
        in >> trialStartingSpeed;
    else if (paramName == "startingNav")
        in >> startingNav;
    else if (paramName == "nback")
        in >> nback;
    else if (paramName == "control")
        in >> control;
    else if (paramName == "tunnelMinAngleTurn")
        in >> tunnelMinAngleTurn;
    else if (paramName == "tunnelMaxAngleTurn")
        in >> tunnelMaxAngleTurn;
    else if (paramName == "tunnelSegmentsPerSection")
        in >> tunnelSegmentsPerSection;
    else if (paramName == "tunnelSegmentsPerPod")
        in >> tunnelSegmentsPerPod;
    else if (paramName == "tunnelSegmentsPerDistractors")
        in >> tunnelSegmentsPerDistractors;
    else if (paramName == "tunnelSegmentsPerPowerup")
        in >> tunnelSegmentsPerPowerup;
    else if (paramName == "initialSegmentsFirstPod")
        in >> initialSegmentsFirstPod;
    else if (paramName == "initialSegmentsFirstDistractors")
        in >> initialSegmentsFirstDistractors;
    else if (paramName == "initialSegmentsFirstPowerup")
        in >> initialSegmentsFirstPowerup;
    else if (paramName == "podRotateSpeed")
        in >> podRotateSpeed;
    else if (paramName == "podAppearance")
        in >> podAppearance;
    else if (paramName == "podBinSize")
        in >> podBinSize;
    else if (paramName == "span")
        in >> span;
    else if (paramName == "podCollisionMin")
        in >> podCollisionMin;
    else if (paramName == "podCollisionMax")
        in >> podCollisionMax;
    else if (paramName == "distractorCollisionMin")
        in >> distractorCollisionMin;
    else if (paramName == "distractorCollisionMax")
        in >> distractorCollisionMax;
    else if (paramName == "podNBackChance")
        in >> podNBackChance;
    else if (paramName == "stageTotalDistractorsMin")
        in >> stageTotalDistractorsMin;
    else if (paramName == "stageTotalDistractorsMax")
        in >> stageTotalDistractorsMax;
    else if (paramName == "startingHP")
        in >> startingHP;
    else if (paramName == "HPNegativeLimit")
        in >> HPNegativeLimit;
    else if (paramName == "HPPositiveLimit")
        in >> HPPositiveLimit;
    else if (paramName == "HPPositiveCorrectAnswer")
        in >> HPPositiveCorrectAnswer;
    else if (paramName == "HPPositiveWrongAnswer")
        in >> HPPositiveWrongAnswer;
    else if (paramName == "HPPositiveDistractor")
        in >> HPPositiveDistractor;
    else if (paramName == "HPNegativeCorrectAnswer")
        in >> HPNegativeCorrectAnswer;
    else if (paramName == "HPNegativeWrongAnswer")
        in >> HPNegativeWrongAnswer;
    else if (paramName == "HPNegativeDistractor")
        in >> HPNegativeDistractor;
    else if (paramName == "wrongAnswerTimePenalty")
        in >> wrongAnswerTimePenalty;
    else if (paramName == "distractorSpeedPenalty")
        in >> distractorSpeedPenalty;
    else if (paramName == "distractorTimePenalty")
        in >> distractorTimePenalty;
    else if (paramName == "initCamSpeed")
        in >> initCamSpeed;
    else if (paramName == "globalModifierCamSpeed")
        in >> globalModifierCamSpeed;
    else if (paramName == "boostModifierCamSpeed")
        in >> boostModifierCamSpeed;
    else if (paramName == "minCamSpeed")
        in >> minCamSpeed;
    else if (paramName == "maxCamSpeed")
        in >> maxCamSpeed;
    else if (paramName == "nlevelSpeedModifier")
        in >> nlevelSpeedModifier;
    else if (paramName == "numToSpeedUp")
        in >> numToSpeedUp;
    else if (paramName == "numToSpeedDown")
        in >> numToSpeedDown;
    else if (paramName == "stepsizeSpeedUp")
        in >> stepsizeSpeedUp;
    else if (paramName == "stepsizeSpeedDown")
        in >> stepsizeSpeedDown;
    else if (paramName == "setSkyBox")
        in >> setSkyBox;
    else if (paramName == "setWallPanelTexture")
        in >> setWallPanelTexture;
    else if (paramName == "setVineShip")
    {
        int input;
        in >> input;
        setVineShip = (VineMeshType)input;
    }
    else if (paramName == "swipeSensitivity")
        in >> swipeSensitivity;
    else if (paramName == "swipeInverted")
        in >> swipeInverted;
    else if (paramName == "boostEnabled")
        in >> boostEnabled;
    else if (paramName == "combo1MinA")
        in >> combo1MinA;
    else if (paramName == "combo2MinA")
        in >> combo2MinA;
    else if (paramName == "combo1MinB")
        in >> combo1MinB;
    else if (paramName == "combo2MinB")
        in >> combo2MinB;
    else if (paramName == "numNavPhases")
        in >> numNavPhases;
    else if (paramName == "numTimePhases")
        in >> numTimePhases;
    else if (paramName == "expectedNumSessions")
        in >> expectedNumSessions;
    else if (paramName == "sessionTimeMin")
        in >> sessionTimeMin;
    else if (paramName == "sessionTimeMax")
        in >> sessionTimeMax;
    else if (paramName == "timeTrialLengthMin")
        in >> timeTrialLengthMin;
    else if (paramName == "timeTrialLengthMax")
        in >> timeTrialLengthMax;
    else if (paramName == "navUnlockMax0")
        in >> navUnlockMax0;
    else if (paramName == "navUnlockNBack1")
        in >> navUnlockNBack1;
    else if (paramName == "navUnlockMax1")
        in >> navUnlockMax1;
    else if (paramName == "navUnlockNBack2")
        in >> navUnlockNBack2;
    else if (paramName == "navUnlockMax2")
        in >> navUnlockMax2;
    else if (paramName == "volumeMusic")
        in >> volumeMusic;
    else if (paramName == "volumeSignal1")
        in >> volumeSignal1;
    else if (paramName == "volumeSignal2")
        in >> volumeSignal2;
    else if (paramName == "volumeSignal3")
        in >> volumeSignal3;
    else if (paramName == "volumeSignal4")
        in >> volumeSignal4;
    else if (paramName == "volumeFeedbackGood")
        in >> volumeFeedbackGood;
    else if (paramName == "volumeFeedbackBad")
        in >> volumeFeedbackBad;
    else if (paramName == "volumeFeedbackCollision")
        in >> volumeFeedbackCollision;
    else if (paramName == "volumeBoost")
        in >> volumeBoost;
    else if (paramName == "volumeStartup")
        in >> volumeStartup;
    else if (paramName == "numSignalTypes")
    {
        int input;
        in >> input;
        signalTypes.clear();
        signalTypes = std::vector<std::vector<PodInfo> >(input);
    }
    else if (paramName == "signalType")
    {
        int input1, input2, input3, input4, input5;
        in >> input1 >> input2 >> input3 >> input4 >> input5;
        assert(input1 >= 0 && input1 < signalTypes.size());
        signalTypes[input1].push_back(PodInfo((PodSignal)input1, (PodMeshType)input2, (PodColor)input3, (PodShape)input4, (PodSound)input5));
    }
    else if (paramName == "speedMap")
    {
        int input1;
        float input2;
        
        std::string s;
        std::stringstream ss;
        in >> s;
        ss << s;
        ss >> input1;
        ss.clear();
        in >> s;
        ss << s;
        ss >> input2;
        ss.clear();
        
        speedMap[input1] = input2;
    }
    else
    {
        std::cout << "WARNING: UNKNOWN PARAMETER... " << paramName << " IGNORED" << std::endl;
        setMessage("WARNING: Unknown config parameter", MESSAGE_NOTIFY);
    }
}

bool Util::ConfigGlobal::loadConfig(int sid)
{
    std::string check, paramName, colon;
    char nextVal;
    
    std::ifstream in (configPath.c_str());
    if (!in.good())
    {
        in.open(configBackup.c_str());
        std::cout << "Loading config: " << configBackup << std::endl;
    }
    else
        std::cout << "Loading config: " << configPath << std::endl;
    if (!in.good()) return false;
    
    int count = 0;
    do {
        in >> check;
        if (check != "{") {
            std::cout << "ERROR: Config file missing \'{\' in "
            << "at least one Stage configuration" << std::endl;
            return false;
        }
        
        in >> paramName;
        while (paramName != "}" && !in.eof()) {
            in >> colon;
            nextVal = in.peek();
            setConfigValue(in, paramName);
            in >> paramName;
            
            if (colon == "{" || nextVal == '{' || paramName == "{" || in.eof()) {
                std::cout << "ERROR: Config file missing \'}\' in "
                << "at least one Stage configuration." << std::endl;
                in.close();
                return false;
            }
        }
        
        // Sanity check, make sure stageID follows standard value 0.. 1.. 2.. 3
        if (stageID != count)
        {
            std::cout << "ERROR: Config file does not follow standard enumerated stageID starting at 0" << std::endl;
            in.close();
            return false;
        }
        ++count;
        
    } while (stageID != sid && !in.eof());
    
    in.close();
    
    if (stageID == sid) {
        std::cout << "Loaded stageID " << stageID << std::endl;
        return true;
    } else {
        std::cout << "Failed to load stageID " << stageID << std::endl;
        return false;
    }
    
    return false;
}

void Util::ConfigGlobal::setBigMessage(std::string msg)
{
    messageBig = msg;
}

void Util::ConfigGlobal::setMessage(std::string msg, MessageType type)
{
    if (type > messageType)
    {
        message = msg;
        messageType = type;
    }
}

void Util::ConfigGlobal::appendMessage(std::string msg, MessageType type)
{
    if (type >= messageType)
    {
        message += msg;
        messageType = type;
    }
}

void Util::ConfigGlobal::clearMessage()
{
    messageBig = "";
    message = "";
    messageType = MESSAGE_NONE;
}

bool Util::ConfigGlobal::setName(const char* name)
{
    std::string test = "subject000";
    playerName = name;
    if (playerName.length() != test.length())
        return false;
    
    if (playerName.substr(0, 7) != "subject")
        return false;
    
    int id = atoi(playerName.substr(7, 3).c_str());
    if (!((id >= 100 && id <= 200) || (id >= 900 && id <= 999)))
        return false;
    return true;
}

std::string Util::ConfigGlobal::buildPath(std::string ext, std::string playerName, int session)
{
    //Get Date then 24-hour then minutes
    time_t raw = time(0);
    struct tm * timeinfo = localtime( &raw );
    char buffer [80];
    strftime(buffer, 80, "%F-%H-%M", timeinfo);
    
#if defined(OGRE_IS_IOS)
    std::string userPath = Util::getIOSDir() + "/" + playerName;
    std::string logPath = userPath + "/"
    + playerName + "-session" + Util::toStringInt(session) + "-" + std::string(buffer);
#else
    std::string userPath = Util::getOSXDir() + "/" + playerName;
    std::string logPath = userPath + "/"
    + playerName + "-session" + Util::toStringInt(session) + "-" + std::string(buffer);
#endif
    int x = mkdir(userPath.c_str(), 0777);
    
    int i = 1;
    
    std::ifstream testExist (std::string(logPath + ext).c_str());
    while (testExist) {
        testExist.close();
        logPath = logPath + "_" + Util::toStringInt(i);
        ++i;
        testExist.open(std::string(logPath + ext).c_str());
    }
    
    logPath = logPath + ext;
    
    return logPath;
}

void Util::ConfigGlobal::initGlobalSettingsPath()
{
#if defined(OGRE_IS_IOS)
    globalPath = Util::getIOSDir() + "/globalSettings.save";
#else
    globalPath = Util::getOSXDir() + "/globalSettings.save";
#endif
}

bool Util::ConfigGlobal::saveGlobalSettings(std::string file)
{
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    bool ret = true;
    
    out << "V1.0" << std::endl;
    
    out << "sessionScreenEnabled" << " " << sessionScreenEnabled << std::endl;
    out << "playerName" << " " << playerName << std::endl;
    out << "fuelEnabled" << " " << fuelEnabled << std::endl;
    out << "holdoutEnabled" << " " << holdoutEnabled << std::endl;
    out << "initialVelocity" << " " << initialVelocity << std::endl;
    out << "manRecessEnabled" << " " << manRecessEnabled << std::endl;
    out << "manRecessLevelLimit" << " " << manRecessLevelLimit << std::endl;
    out << "newNavEnabled" << " " << newNavEnabled << std::endl;
    out << "newNavIncrement" << " " << newNavIncrement << std::endl;
    out << "indRecessEnabled" << " " << indRecessEnabled << std::endl;
    out << "indRecessIncrement" << " " << indRecessIncrement << std::endl;
    out << "holdoutdelayEnabled" << " " << holdoutdelayEnabled << std::endl;
    out << "holdoutdelayNumber" << " " << holdoutdelayNumber << std::endl;
    out << "newSounds" << " " << newSounds << std::endl;
    out << "enableSettingsPasscode" << " " << enableSettingsPasscode << std::endl;
    out << "sessionStartTime" << " " << sessionStartTime << std::endl;
    out << "sessionEndTime" << " " << sessionEndTime << std::endl;
    out << "numOfSessions" << " " << numOfSessions << std::endl;
    out << "enableIndRecessFixed" << " " << enableIndRecessFixed << std::endl;
    out << "syncDataToServer" << " " << syncDataToServer << std::endl;

    out << "holdoutMinUpperBound" << " " << holdoutMinUpperBound << std::endl;
    out << "holdoutMaxUpperBound" << " " << holdoutMaxUpperBound << std::endl;
    out << "holdoutLowerBoundTime" << " " << holdoutLowerBoundTime << std::endl;
    out << "holdoutUpperBoundMinTime" << " " << holdoutUpperBoundMinTime << std::endl;
    out << "holdoutUpperBoundMaxTime" << " " << holdoutUpperBoundMaxTime << std::endl;
    out << "holdoutSteps" << " " << holdoutSteps << std::endl;
    
    std::cout << "Save Global Settings: " << file << std::endl;
    ret = out.good();
    
    out.close();
    return ret;
}

// Load based on player results in level progression
// Version 1.0
bool Util::ConfigGlobal::loadGlobalSettings1_0(std::string savePath)
{
    std::ifstream saveFile (savePath.c_str());
    
    if (saveFile.good()) {
        std::string input;
        saveFile >> input; // Receive version string
        
        while (saveFile >> input)
        {
            if (input == "sessionScreenEnabled")
                saveFile >> sessionScreenEnabled;
            else if (input == "playerName")
                saveFile >> playerName;
            else if(input == "fuelEnabled")
                saveFile >> fuelEnabled;
            else if(input == "holdoutEnabled")
                saveFile >> holdoutEnabled;
            else if(input == "initialVelocity")
                saveFile >> initialVelocity;
            else if(input == "manRecessEnabled")
                saveFile >> manRecessEnabled;
            else if(input == "manRecessLevelLimit")
                saveFile >> manRecessLevelLimit;
            else if(input == "newNavEnabled")
                saveFile >> newNavEnabled;
            else if(input == "newNavIncrement")
                saveFile >> newNavIncrement;
            else if(input == "indRecessEnabled")
                saveFile >> indRecessEnabled;
            else if(input == "indRecessIncrement")
                saveFile >> indRecessIncrement;
            else if(input == "holdoutdelayEnabled")
                saveFile >> holdoutdelayEnabled;
            else if(input == "holdoutdelayNumber")
                saveFile >> holdoutdelayNumber;
            else if (input == "newSounds")
                saveFile >> newSounds;
            else if (input == "enableSettingsPasscode")
                saveFile >> enableSettingsPasscode;
            else if (input == "sessionStartTime")
                saveFile >> sessionStartTime;
            else if (input == "sessionEndTime")
                saveFile >> sessionEndTime;
            else if (input == "numOfSessions")
                saveFile >> numOfSessions;
            else if (input == "enableIndRecessFixed")
                saveFile >> enableIndRecessFixed;
            else if (input == "syncDataToServer")
                saveFile >> syncDataToServer;
            else if (input == "holdoutMinUpperBound")
                saveFile >> holdoutMinUpperBound;
            else if (input == "holdoutMaxUpperBound")
                saveFile >> holdoutMaxUpperBound;
            else if (input == "holdoutLowerBoundTime")
                saveFile >> holdoutLowerBoundTime;
            else if (input == "holdoutUpperBoundMinTime")
                saveFile >> holdoutUpperBoundMinTime;
            else if (input == "holdoutUpperBoundMaxTime")
                saveFile >> holdoutUpperBoundMaxTime;
            else if (input == "holdoutSteps")
                saveFile >> holdoutSteps;
            
        }
    }
    return saveFile.eof();
}

// Loads global settings. First it decides which version,
// the save file is and calls the correct function
bool Util::ConfigGlobal::loadGlobalSettings(std::string savePath)
{
    std::ifstream saveFile (savePath.c_str());
    
    if (saveFile.good()) {
        std::string input;
        saveFile >> input;
        
        saveFile.close();
        if (input == "V1.0")
            return loadGlobalSettings1_0(savePath);
        else
            return false;
    }
    return false;
}

float Util::clamp(float val, float min, float max)
{
    if (val < min)
        val = min;
    if (val > max)
        val = max;
    return val;
}

// Returns the degrees from 0-359 for a direction where SOUTH is 0
float Util::getDegrees(Direction dir)
{
	switch (dir)
	{
        case NORTHWEST:
            return 135;
        case NORTH:
            return 180;
        case NORTHEAST:
            return 225;
        case EAST:
            return 270;
        case SOUTHEAST:
            return 315;
        case SOUTH:
            return 0;
        case SOUTHWEST:
            return 45;
        case WEST:
            return 90;
        default:
            return 0;
	}
}

Direction Util::leftOf(Direction dir)
{
	switch (dir)
	{
        case NORTHWEST:
            return WEST;
        case NORTH:
            return NORTHWEST;
        case NORTHEAST:
            return NORTH;
        case EAST:
            return NORTHEAST;
        case SOUTHEAST:
            return EAST;
        case SOUTH:
            return SOUTHEAST;
        case SOUTHWEST:
            return SOUTH;
        case WEST:
            return SOUTHWEST;
        default:
            return NO_DIRECTION;
	}
}

Direction Util::rightOf(Direction dir)
{
	switch (dir)
	{
        case NORTHWEST:
            return NORTH;
        case NORTH:
            return NORTHEAST;
        case NORTHEAST:
            return EAST;
        case EAST:
            return SOUTHEAST;
        case SOUTHEAST:
            return SOUTH;
        case SOUTH:
            return SOUTHWEST;
        case SOUTHWEST:
            return WEST;
        case WEST:
            return NORTHWEST;
        default:
            return NO_DIRECTION;
	}
}

Direction Util::oppositeOf(Direction dir)
{
	switch (dir)
	{
        case NORTHWEST:
            return SOUTHEAST;
        case NORTH:
            return SOUTH;
        case NORTHEAST:
            return SOUTHWEST;
        case EAST:
            return WEST;
        case SOUTHEAST:
            return NORTHWEST;
        case SOUTH:
            return NORTH;
        case SOUTHWEST:
            return NORTHEAST;
        case WEST:
            return EAST;
        default:
            return NO_DIRECTION;
	}
}

bool Util::doSidesMatch(bool sides1[NUM_DIRECTIONS], bool sides2[NUM_DIRECTIONS])
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        if (sides1[i] != sides2[i])
            return false;
    return true;
}

void Util::setSides(bool sides[NUM_DIRECTIONS], int level, Direction dir)
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sides[i] = false;
    switch (level)
    {
        case 1:
            sides[leftOf(dir)] = true;
            sides[dir] = true;
            sides[rightOf(dir)] = true;
            break;
        case 2:
            sides[leftOf(leftOf(dir))] = true;
            sides[leftOf(dir)] = true;
            sides[dir] = true;
            sides[rightOf(dir)] = true;
            sides[rightOf(rightOf(dir))] = true;
            break;
        case 3:
            sides[leftOf(leftOf(leftOf(dir)))] = true;
            sides[leftOf(leftOf(dir))] = true;
            sides[leftOf(dir)] = true;
            sides[dir] = true;
            sides[rightOf(dir)] = true;
            sides[rightOf(rightOf(dir))] = true;
            sides[rightOf(rightOf(rightOf(dir)))] = true;
            break;
        default:
            sides[NORTHWEST] = true;
            sides[WEST] = true;
            sides[SOUTHWEST] = true;
            sides[SOUTH] = true;
            sides[SOUTHEAST] = true;
            sides[EAST] = true;
            sides[NORTHEAST] = true;
            sides[NORTH] = true;
            break;
    }
    
}

void Util::copySides(bool dest[NUM_DIRECTIONS], bool src[NUM_DIRECTIONS]) // C.P.
{
    memcpy(dest, src, NUM_DIRECTIONS);
}

int Util::getNumSides(int level)
{
    switch (level)
    {
        case 1:
            return 3;
        case 2:
            return 5;
        case 3:
            return 7;
        case 4:
            return 8;
        default:
            return 8;
    }
}

int Util::getNumSides(bool sides[NUM_DIRECTIONS])
{
    int count = 0;
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        if (sides[i]) ++count;
    return count;
}

int Util::getControlLevel(bool sides[NUM_DIRECTIONS])
{
    int count = getNumSides(sides);
    switch (count)
    {
        case 3:
            return 1;
        case 5:
            return 2;
        case 7:
            return 3;
        case 8:
            return 4;
        default:
            return 0; // should never happen
    }
}

Direction Util::randDirection()
{
    return (Direction)(rand() % NUM_DIRECTIONS);
}

Direction Util::randDirection(const bool sides[NUM_DIRECTIONS])
{
    std::vector<Direction> dirs;
    if (sides[NORTHWEST])
        dirs.push_back(NORTHWEST);
    if (sides[NORTH])
        dirs.push_back(NORTH);
    if (sides[NORTHEAST])
        dirs.push_back(NORTHEAST);
    if (sides[EAST])
        dirs.push_back(EAST);
    if (sides[SOUTHEAST])
        dirs.push_back(SOUTHEAST);
    if (sides[SOUTH])
        dirs.push_back(SOUTH);
    if (sides[SOUTHWEST])
        dirs.push_back(SOUTHWEST);
    if (sides[WEST])
        dirs.push_back(WEST);
    
    int randDirIndex = rand() % dirs.size();
    
    return dirs[randDirIndex];
}

Vector3 Util::randVector3()
{
    Vector3 temp(0, 1, 0);
    return temp.randomDeviant(Degree(randRangeFloat(0.0, 180.0)));
}

// Returns the name of the music file given the music name in game
std::string Util::getMusicFile(const std::string & musicName)
{
    if (musicName == "MusicMenu")
        return "TheReturn.ogg";
        //return "VideoGameSong4.ogg";
    else if (musicName == "Music1")
        return "Dots5_converted.ogg";
    else if (musicName == "Music2")
        return "Squares5_converted.ogg";
    else if (musicName == "Music3")
        return "Fireworks2_converted.ogg";
    else if (musicName == "Music4")
        return "Flourish2_converted.ogg";
    else if (musicName == "Music5")
        return "SoundOfWind.ogg";
    return "";
}

// Returns a random number from min and max [inclusive, inclusive]
int Util::randRangeInt(int min, int max)
{
    return min + rand () % (max - min + 1);
}

// Returns a random float from min and max
float Util::randRangeFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}

std::string Util::toStringInt(int value)
{
    std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Util::toStringFloat(float value)
{
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed;
	ss << value;
	return ss.str();
}

std::string Util::toStringFloat(float value,int precision)
{
    std::stringstream ss;
    ss << std::setprecision(precision) << std::fixed;
    ss << value;
    return ss.str();
}

std::string Util::getOSXDir()
{
    char * dir = getenv("HOME");
    std::string result = "";
    if (dir)
        result = std::string(dir) + "/braingame/Documents";
    else
        return "";
    
    mkdir(result.c_str(), 0777);
    return result;
}

std::string Util::getIOSDir()
{
    //const char* dir = OgreFramework::getSingletonPtr()->getMacBundlePath().c_str();
    //std::string result = "";
    //if (dir)
    //    result = std::string(dir) + "/../Documents";
    //else
    //    return "";
    //
    //mkdir(OgreFramework::getSingletonPtr()->getMacBundlePath().c_str(), 0777);
    std::string result = OgreFramework::getSingletonPtr()->getMacBundlePath();
    return result;
}

void Util::drawRect(ManualObject* obj, float x, float y, float width, float height, const ColourValue & col, bool filled)
{
    float left = x;
    float right = x + width;
    float top = -y;
    float bottom = -(y + height);
    
    obj->clear();
    // Range is from x [-1, 1], y [-1, 1]
    obj->setUseIdentityProjection(true);
    obj->setUseIdentityView(true);
    obj->setQueryFlags(0);
    if (filled)
    {
        obj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
        obj->position(left, top, -1);
        obj->colour(col);
        obj->position(right, top, -1);
        obj->colour(col);
        obj->position(right, bottom, -1);
        obj->colour(col);
        obj->position(left, bottom, -1);
        obj->colour(col);
        obj->quad(3, 2, 1, 0);
        obj->end();
    }
    else
    {
        obj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
        obj->position(left, top, -1);
        obj->colour(col);
        obj->position(right, top, -1);
        obj->colour(col);
        obj->position(right, bottom, -1);
        obj->colour(col);
        obj->position(left, bottom, -1);
        obj->colour(col);
        obj->position(left, top, -1);
        obj->colour(col);
        obj->end();
    }
    obj->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
}

// http://www.ogre3d.org/tikiwiki/tiki-index.php?page=manualspheremeshes
void Util::createSphere(Ogre::SceneManager* sceneMgr, const std::string& strName, float r, int nRings, int nSegments)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    float fDeltaRingAngle = (Math::PI / nRings);
    float fDeltaSegAngle = (2 * Math::PI / nSegments);
    unsigned short wVerticeIndex = 0 ;
    
    // Generate the group of rings for the sphere
    for( int ring = 0; ring <= nRings; ring++ ) {
        float r0 = r * sinf (ring * fDeltaRingAngle);
        float y0 = r * cosf (ring * fDeltaRingAngle);
        
        // Generate the group of segments for the current ring
        for(int seg = 0; seg <= nSegments; seg++) {
            float x0 = r0 * sinf(seg * fDeltaSegAngle);
            float z0 = r0 * cosf(seg * fDeltaSegAngle);
            
            // Add one vertex to the strip which makes up the sphere
            manual->position( x0, y0, z0);
            
            manual->normal(Vector3(x0, y0, z0).normalisedCopy());
            manual->textureCoord((float) seg / (float) nSegments, (float) ring / (float) nRings);
            
            if (ring != nRings) {
                // each vertex (except the last) has six indicies pointing to it
                manual->index(wVerticeIndex + nSegments + 1);
                manual->index(wVerticeIndex);
                manual->index(wVerticeIndex + nSegments);
                manual->index(wVerticeIndex + nSegments + 1);
                manual->index(wVerticeIndex + 1);
                manual->index(wVerticeIndex);
                wVerticeIndex ++;
            }
        }; // end for seg
    } // end for ring
    manual->end();
    MeshPtr mesh = manual->convertToMesh(strName);
    mesh->_setBounds( AxisAlignedBox( Vector3(-r, -r, -r), Vector3(r, r, r) ), false );
    
    mesh->_setBoundingSphereRadius(r);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createCylinder(Ogre::SceneManager* sceneMgr, const std::string& strName, float r, float h, int nSegments)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    float fDeltaSegAngle = (2 * Math::PI / nSegments);
    
    float x0 = 0;
    float y0 = -h / 2;
    float y1 = h / 2;
    float z0 = 0;
    
    manual->position(x0, y0, z0);
    manual->normal(Vector3(x0, y0, z0).normalisedCopy());
    manual->textureCoord(0.0, 0.0);
    manual->position(x0, y1, z0);
    manual->normal(Vector3(x0, y1, z0).normalisedCopy());
    manual->textureCoord(0.0, 1.0);
    
    unsigned short botVerticeIndex = 0;
    unsigned short topVerticeIndex = 1;
    unsigned short iVerticeIndex = 2;
    unsigned short wVerticeIndex = iVerticeIndex;
    
    for (int seg = 0; seg <= nSegments; ++seg)
    {
        x0 = r * sinf(seg * fDeltaSegAngle);
        z0 = r * cosf(seg * fDeltaSegAngle);
        
        manual->position(x0, y0, z0);
        manual->normal(Vector3(x0, 0, z0).normalisedCopy());
        manual->textureCoord((float) seg / (float) nSegments, 0.5);
        manual->position(x0, y1, z0);
        manual->normal(Vector3(x0, 0, z0).normalisedCopy());
        manual->textureCoord((float) seg / (float) nSegments, 0.5);
        
        if (seg > 1)
        {
            manual->triangle(botVerticeIndex, wVerticeIndex + 2, wVerticeIndex);
            manual->triangle(topVerticeIndex, wVerticeIndex + 1, wVerticeIndex + 3);
            manual->triangle(wVerticeIndex, wVerticeIndex + 2, wVerticeIndex + 1);
            manual->triangle(wVerticeIndex + 2, wVerticeIndex + 3, wVerticeIndex + 1);
            
            wVerticeIndex += 2;
        }
    }
    manual->triangle(botVerticeIndex, iVerticeIndex, wVerticeIndex); // last triangles
    manual->triangle(topVerticeIndex, wVerticeIndex + 1, iVerticeIndex + 1);
    manual->triangle(wVerticeIndex, iVerticeIndex, wVerticeIndex + 1);
    manual->triangle(iVerticeIndex, iVerticeIndex + 1, wVerticeIndex + 1);
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    Vector3 bl = Vector3(-r, -h / 2, -r);
    Vector3 tr = Vector3(r, h / 2, r);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), false );
    
    mesh->_setBoundingSphereRadius((tr - bl).length() / 2);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createDiamond(Ogre::SceneManager* sceneMgr, const std::string& strName, float w, float h)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    manual->position(0, h, 0);
    manual->normal(0, 1, 0);
    manual->textureCoord(0.5, 1.0);
    manual->position(-w, 0, -w);
    manual->normal(-sqrt(2), 0, -sqrt(2));
    manual->textureCoord(0.0, 0.0);
    manual->position(w, 0, -w);
    manual->normal(sqrt(2), 0, -sqrt(2));
    manual->textureCoord(1.0, 0.0);
    manual->position(w, 0, w);
    manual->normal(sqrt(2), 0, sqrt(2));
    manual->textureCoord(1.0, 1.0);
    manual->position(-w, 0, w);
    manual->normal(-sqrt(2), 0, sqrt(2));
    manual->textureCoord(1.0, 0.0);
    manual->position(0, -h, 0);
    manual->normal(0, -1, 0);
    manual->textureCoord(0.5, 0.0);
    
    manual->triangle(4, 3, 0);
    manual->triangle(3, 2, 0);
    manual->triangle(2, 1, 0);
    manual->triangle(1, 4, 0);
    manual->triangle(1, 2, 5);
    manual->triangle(2, 3, 5);
    manual->triangle(3, 4, 5);
    manual->triangle(4, 1, 5);
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    Vector3 bl = Vector3(-w, -h, -w);
    Vector3 tr = Vector3(w, h, w);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), false );
    
    mesh->_setBoundingSphereRadius((tr - bl).length() / 2);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createBox(Ogre::SceneManager* sceneMgr, const std::string& strName, float l, float w, float h)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    float mag = sqrt(l * l + w * w + h * h);
    
    
    manual->position(-l, -w, -h);   // 0
    manual->textureCoord(0.0, 0.0);
    manual->normal(Vector3(0, -1, 0));
    manual->position(l, -w, -h);    // 1
    manual->textureCoord(0.5, 0.0);
    manual->normal(Vector3(0, -1, 0));
    manual->position(l, w, -h);     // 2
    manual->textureCoord(0.5, 0.5);
    manual->normal(Vector3(0, -1, 0));
    manual->position(-l, w, -h);    // 3
    manual->textureCoord(0.0, 0.5);
    manual->normal(Vector3(0, -1, 0));
    
    manual->position(-l, w, h);     // 7
    manual->textureCoord(0.5, 1.0);
    manual->normal(Vector3(0, 0, 1));
    manual->position(l, w, h);      // 6
    manual->textureCoord(1.0, 1.0);
    manual->normal(Vector3(0, 0, 1));
    manual->position(l, w, -h);     // 2
    manual->textureCoord(0.5, 0.5);
    manual->normal(Vector3(0, -1, 0));
    manual->position(-l, w, -h);    // 3
    manual->textureCoord(0.0, 0.5);
    manual->normal(Vector3(0, -1, 0));
    
    manual->position(-l, -w, h);    // 4
    manual->textureCoord(0.5, 0.5);
    manual->normal(Vector3(0, 1, 0));
    manual->position(l, -w, h);     // 5
    manual->textureCoord(1.0, 0.5);
    manual->normal(Vector3(0, 1, 0));
    manual->position(l, w, h);      // 6
    manual->textureCoord(1.0, 1.0);
    manual->normal(Vector3(0, 1, 0));
    manual->position(-l, w, h);     // 7
    manual->textureCoord(0.5, 1.0);
    manual->normal(Vector3(0, 1, 0));
    
    manual->position(-l, -w, -h);   // 0
    manual->textureCoord(0.0, 0.0);
    manual->normal(Vector3(0, -1, 0));
    manual->position(l, -w, -h);    // 1
    manual->textureCoord(0.5, 0.0);
    manual->normal(Vector3(0, -1, 0));
    manual->position(l, -w, h);     // 5
    manual->textureCoord(1.0, 0.5);
    manual->normal(Vector3(0, 0, -1));
    manual->position(-l, -w, h);    // 4
    manual->textureCoord(0.5, 0.5);
    manual->normal(Vector3(0, 0, -1));
    
    manual->position(l, -w, -h);    // 1
    manual->textureCoord(0.5, 0.0);
    manual->normal(Vector3(0, -1, 0));
    manual->position(l, w, -h);     // 2
    manual->textureCoord(0.5, 0.5);
    manual->normal(Vector3(0, -1, 0));
    manual->position(l, w, h);      // 6
    manual->textureCoord(1.0, 1.0);
    manual->normal(Vector3(1, 0, 0));
    manual->position(l, -w, h);     // 5
    manual->textureCoord(1.0, 0.5);
    manual->normal(Vector3(1, 0, 0));
    
    manual->position(-l, w, -h);    // 3
    manual->textureCoord(0.0, 0.5);
    manual->normal(Vector3(0, -1, 0));
    manual->position(-l, -w, -h);   // 0
    manual->textureCoord(0.0, 0.0);
    manual->normal(Vector3(0, -1, 0));
    manual->position(-l, -w, h);    // 4
    manual->textureCoord(0.5, 0.5);
    manual->normal(Vector3(-1, 0, 0));
    manual->position(-l, w, h);     // 7
    manual->textureCoord(0.5, 1.0);
    manual->normal(Vector3(-1, 0, 0));
    
    manual->quad(0, 1, 2, 3);
    manual->quad(4, 5, 6, 7);
    manual->quad(8, 9, 10, 11);
    manual->quad(12, 13, 14, 15);
    manual->quad(16, 17, 18, 19);
    manual->quad(20, 21, 22, 23);
    
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    Vector3 bl = Vector3(-l, -w, -h);
    Vector3 tr = Vector3(l, w, h);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), false );
    
    mesh->_setBoundingSphereRadius((tr - bl).length() / 2);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createPlane(Ogre::SceneManager* sceneMgr, const std::string& strName, float length, float depth)
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    length += EPSILON;
    depth += EPSILON;
    
    manual->position(-length / 2, 0, -depth / 2);
    manual->normal(0, 1, 0);
    //manual->normal(sqrt(2.0f) / 2.0f, sqrt(2.0f) / 2.0f, 0);
    manual->textureCoord(0, 0);
    manual->position(length / 2, 0, -depth / 2);
    manual->normal(0, 1, 0);
    //manual->normal(sqrt(2.0f) / 2.0f, sqrt(2.0f) / 2.0f, 0);
    manual->textureCoord(1, 0);
    manual->position(length / 2, 0, depth / 2);
    manual->normal(0, 1, 0);
    //manual->normal(sqrt(2.0f) / 2.0f, sqrt(2.0f) / 2.0f, 0);
    manual->textureCoord(1, 1);
    manual->position(-length / 2, 0, depth / 2);
    manual->normal(0, 1, 0);
    //manual->normal(sqrt(2.0f) / 2.0f, sqrt(2.0f) / 2.0f, 0);
    manual->textureCoord(0, 1);
    manual->quad(3, 2, 1, 0);
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    float len = length / 2;
    float dep = depth / 2;
    float diag1 = (Vector3(length, 0, depth) - Vector3(-length, 0, -depth)).length() / 2;
    float diag2 = (Vector3(length, 0, -depth) - Vector3(-length, 0, depth)).length() / 2;
    Vector3 bl = Vector3(-len, 0, -dep);
    Vector3 tr = Vector3(len, 0, dep);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), true );
    
    mesh->_setBoundingSphereRadius(diag1 > diag2 ? diag1 : diag2);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    sceneMgr->destroyManualObject(manual);
}

void Util::createSubPlane(Ogre::SceneManager* sceneMgr, ManualObject* manual, float length, float depth, Vector3 loc, Quaternion rot, Vector3 & bl, Vector3 & tr)
{
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    length += 2 * Util::EPSILON;
    depth += 4 * Util::EPSILON;
    
    Vector3 p1 = Vector3(-length / 2, 0, -depth / 2);
    Vector3 p2 = Vector3(length / 2, 0, -depth / 2);
    Vector3 p3 = Vector3(length / 2, 0, depth / 2);
    Vector3 p4 = Vector3(-length / 2, 0, depth / 2);
    
    Vector3 normal = Vector3(0, 1, 0);
    
    p1 = loc + rot * p1;
    p2 = loc + rot * p2;
    p3 = loc + rot * p3;
    p4 = loc + rot * p4;
    normal = rot * normal;
    
    manual->position(p1.x, p1.y, p1.z);
    manual->normal(normal.x, normal.y, normal.z);
    manual->textureCoord(0, 0);
    manual->position(p2.x, p2.y, p2.z);
    manual->normal(normal.x, normal.y, normal.z);
    manual->textureCoord(1, 0);
    manual->position(p3.x, p3.y, p3.z);
    manual->normal(normal.x, normal.y, normal.z);
    manual->textureCoord(1, 1);
    manual->position(p4.x, p4.y, p4.z);
    manual->normal(normal.x, normal.y, normal.z);
    manual->textureCoord(0, 1);
    manual->quad(3, 2, 1, 0);
    manual->end();
    
    bl = Vector3(fmin(bl.x, p1.x), fmin(bl.y, p2.y), fmin(bl.z, p1.z));
    bl = Vector3(fmin(bl.x, p2.x), fmin(bl.y, p2.y), fmin(bl.z, p2.z));
    bl = Vector3(fmin(bl.x, p3.x), fmin(bl.y, p3.y), fmin(bl.z, p3.z));
    bl = Vector3(fmin(bl.x, p4.x), fmin(bl.y, p4.y), fmin(bl.z, p4.z));
    tr = Vector3(fmax(tr.x, p1.x), fmax(tr.y, p2.y), fmax(tr.z, p1.z));
    tr = Vector3(fmax(tr.x, p2.x), fmax(tr.y, p2.y), fmax(tr.z, p2.z));
    tr = Vector3(fmax(tr.x, p3.x), fmax(tr.y, p3.y), fmax(tr.z, p3.z));
    tr = Vector3(fmax(tr.x, p4.x), fmax(tr.y, p4.y), fmax(tr.z, p4.z));
}

void Util::createSegment(Ogre::SceneManager* sceneMgr, const std::string& strName, float length, float depth, const bool sides[NUM_DIRECTIONS])
{
    ManualObject * manual = sceneMgr->createManualObject(strName);
    
	float wallLength = length / (2 * Math::Cos(Ogre::Radian(Math::PI) / 4) + 1);
    
    Vector3 bl = Vector3(Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY);
    Vector3 tr = Vector3(Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY);
    
    Vector3 move;
    Quaternion rot;
    
    if (sides[NORTHWEST])
    {
        move = Vector3(-(length + wallLength) / 4, (length + wallLength) / 4, 0);
        rot.FromAngleAxis(Degree(225), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[NORTH])
    {
        move = Vector3(0, length / 2, 0);
        rot.FromAngleAxis(Degree(180), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[NORTHEAST])
    {
        move = Vector3((length + wallLength) / 4, (length + wallLength) / 4, 0);
        rot.FromAngleAxis(Degree(135), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[EAST])
    {
        move = Vector3(length / 2, 0, 0);
        rot.FromAngleAxis(Degree(90), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[SOUTHEAST])
    {
        move = Vector3((length + wallLength) / 4, -(length + wallLength) / 4, 0);
        rot.FromAngleAxis(Degree(45), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[SOUTH])
    {
        move = Vector3(0, -length / 2, 0);
        rot.FromAngleAxis(Degree(0), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[SOUTHWEST])
    {
        move = Vector3(-(length + wallLength) / 4, -(length + wallLength) / 4, 0);
        rot.FromAngleAxis(Degree(315), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    if (sides[WEST])
    {
        move = Vector3(-length / 2, 0, 0);
        rot.FromAngleAxis(Degree(270), Vector3(0, 0, 1));
        createSubPlane(sceneMgr, manual, wallLength, depth, move, rot, bl, tr);
    }
    
    MeshPtr mesh = manual->convertToMesh(strName);
    mesh->_setBounds( AxisAlignedBox( bl, tr ), true );
    
    float l = (tr - bl).length() / 2;
    mesh->_setBoundingSphereRadius(l);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }

    sceneMgr->destroyManualObject(manual);
    
    //MeshSerializer ser;
    //ser.exportMesh(mesh.getPointer(), strName + ".mesh");
}

void Util::createDefaultSegments(Ogre::SceneManager* sceneMgr)
{
    bool sides[NUM_DIRECTIONS];
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sides[i] = false;
    
    sides[SOUTHEAST] = true;
    sides[SOUTH] = true;
    sides[SOUTHWEST] = true;
    Util::createSegment(sceneMgr, "segmentMesh3", 1.0, 1.0, sides);
    
    sides[WEST] = true;
    sides[EAST] = true;
    Util::createSegment(sceneMgr, "segmentMesh5", 1.0, 1.0, sides);
    
    sides[NORTHWEST] = true;
    sides[NORTHEAST] = true;
    Util::createSegment(sceneMgr, "segmentMesh7", 1.0, 1.0, sides);
    
    sides[NORTH] = true;
    Util::createSegment(sceneMgr, "segmentMesh8", 1.0, 1.0, sides);
}

void Util::setSkyboxAndFog(std::string nameSkybox)
{
    // Set skybox, fog, and background
    Plane plane;
    plane.d = 80;
    plane.normal = Ogre::Vector3(0, 0, 1);
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlane(true, plane, nameSkybox, 1, 4, true);
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue::ZERO, 0.0, 300.0, 600.0);
    OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 1.0f));
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetToInitialState();
}

// Step function to increase speed by 1 or decrease by 1 is not included but could be.
void Util::tuneProficiencyExam(ConfigGlobal & globals, float initSpeed, float lengthPerTarget, float approxTotalTime, float bestTime)
{
    // (NumTargets * LengthPerTarget) / Speed = TimeTaken
    // NumTargets = TimeTaken * Speed / LengthPerTarget
    // NumTargets = 90 * 20 / 60
    //
    // NumTargets = TimeTaken * (InitSpeed + NumTargets / 2) / LengthPerTarget
    //
    // (NumTargets * LengthPerTarget) / (InitSpeed + NumTargets / 2) = TimeTaken
    //
    // NumTargets = 2 * TimeTaken * InitSpeed / (2 * LengthPerTarget - TimeTaken)
    //
    //
    // At tbuff = 30 and Si = 15, then N = 10,
    globals.initCamSpeed = initSpeed;
    
    globals.HPPositiveLimit = (2*bestTime*globals.initCamSpeed)/(2*lengthPerTarget-bestTime);
    globals.HPNegativeLimit = -globals.HPPositiveLimit / 3;
    
    globals.minCamSpeed = globals.initCamSpeed + globals.HPNegativeLimit / 2;
    globals.maxCamSpeed = globals.initCamSpeed + globals.HPPositiveLimit / 2;
    globals.stageTime = approxTotalTime - bestTime;
    
    globals.stageTotalSignals = 180; // Enough for constant speed of 30
    globals.stageTotalTargets = globals.stageTotalSignals / 3;    
    globals.stageTotalTargetsVariance = 0;
    
    globals.startingHP = 3;
    globals.HPPositiveCorrectAnswer = 0;
    globals.HPNegativeCorrectAnswer = 0;
    globals.HPPositiveWrongAnswer = -1;
    globals.HPNegativeWrongAnswer = -1;
    globals.HPPositiveDistractor = -1;
    globals.HPNegativeDistractor = -1;
}

float Util::getModdedLengthByNumSegments(const ConfigGlobal & globals, int numSegments)
{
    return (globals.tunnelSegmentDepth + globals.tunnelSegmentBuffer) / globals.globalModifierCamSpeed * numSegments;
}

bool PodInfo::hasHoldout() const
{
    return podColor == POD_COLOR_HOLDOUT || podShape == POD_SHAPE_HOLDOUT || podSound == POD_SOUND_HOLDOUT;
}

void PodInfo::performHoldout(LevelPhase phase, bool sound)
{
    //float rand_holdOut = Ogre::Math::UnitRandom();
    float rand_signal = Ogre::Math::UnitRandom();
    
    if( true ) {//|| rand_holdOut < 0.1f ) {
        switch(phase) {
            case PHASE_COLOR_SOUND:   // levels that normally have color and sound active
                if( rand_signal < 0.5f ) {
                    if (sound)
                        podColor = POD_COLOR_HOLDOUT;
                    else
                        podSound = POD_SOUND_HOLDOUT;
                    std::cout << "Hold out: color" << std::endl;
                }
                else {
                    podSound = POD_SOUND_HOLDOUT;
                    std::cout << "Hold out: sound" << std::endl;
                }
                break;
            case PHASE_SHAPE_SOUND:   // levels that normally have shape and sound active
                if( rand_signal < 0.5f ) {
                    if (sound)
                        podShape = POD_SHAPE_HOLDOUT;
                    else
                        podSound = POD_SOUND_HOLDOUT;
                    std::cout << "Hold out: shape" << std::endl;
                }
                else {
                    podSound = POD_SOUND_HOLDOUT;
                    std::cout << "Hold out: sound" << std::endl;
                }
                break;
            case PHASE_SOUND_ONLY:   // levels that normally have sound only active
                break;
            case PHASE_ALL_SIGNAL:   // levels that normally have all three signals active
                if (sound)
                {
                    if( rand_signal < 0.167f ) {
                        podColor = POD_COLOR_HOLDOUT;
                        podShape = POD_SHAPE_HOLDOUT;
                        std::cout << "Hold out: color and shape (sound only)" << std::endl;
                    }
                    else if( rand_signal < 0.333f ) {
                        podSound = POD_SOUND_HOLDOUT;
                        podShape = POD_SHAPE_HOLDOUT;
                        std::cout << "Hold out: sound and shape (color only)" << std::endl;
                    }
                    else if( rand_signal < 0.500f ) {
                        podColor = POD_COLOR_HOLDOUT;
                        podSound = POD_SOUND_HOLDOUT;
                        std::cout << "Hold out: color and sound (shape only)" << std::endl;
                    }
                    else if( rand_signal < 0.667f ) {
                        podColor = POD_COLOR_HOLDOUT;
                        std::cout << "Hold out: color (shape and sound only)" << std::endl;
                    }
                    else if( rand_signal < 0.824f ) {
                        podShape = POD_SHAPE_HOLDOUT;
                        std::cout << "Hold out: shape (color and sound only)" << std::endl;
                    }
                    else {
                        podSound = POD_SOUND_HOLDOUT;
                        std::cout << "Hold out: sound (color and shape only)" << std::endl;
                    }
                }
                else
                {
                    if (rand_signal < 0.5f)
                        podColor = POD_COLOR_HOLDOUT;
                    else
                        podShape = POD_SHAPE_HOLDOUT;
                }
                break;
            case PHASE_COLLECT:   // recess levels
                break;
            default:
                break;
        }
    }
}

void PodInfo::performHoldout(LevelPhase phase, bool sound, bool holdsound, bool holdcolor, bool holdshape, bool applyConstraints, PodInfo prevMatch)
{
    
    //Phases:
    // ColorSound, ShapeSound, Sound Only, AllSignal, Recess
    
    // For a ColorSound level, ignore shape holdout boolean. (Dont make it possible. Dont set it true.)
    // For a ShapeSound level, ignore color holdout boolean. (Dont make it possible. Dont set it true.)

 
    
    //List of possible holdouts!
    //0: Color Holdout Only
    //1: Sound Holdout Only
    //2: Shape Holdout Only
    //3: ShapeSound Holdout Only
    //4: ShapeColor Holdout Only
    //5: ColorSound Holdout Only
    //6: No Holdout!
    bool PossibleHoldouts[7] = {false,false,false,false,false,false,false};
    int sizeOfPossibilites = 0;
    
    
    if((phase != PHASE_SOUND_ONLY) && (phase != PHASE_COLLECT))
    {
        if(applyConstraints) //This pod is a match and it may have a feature held out
        {
            
            if(phase != PHASE_SHAPE_SOUND)
            {
                PossibleHoldouts[0] = holdcolor;
            }
            
            PossibleHoldouts[1] = holdsound;
            
            
            if(phase != PHASE_COLOR_SOUND)
            {
                PossibleHoldouts[2] = holdshape;
            }
            
            //All Signals Cases
            if(phase == PHASE_ALL_SIGNAL)
            {
                if(holdshape && holdcolor && holdsound)
                {
                    PossibleHoldouts[3] = true;
                    PossibleHoldouts[4] = true;
                    PossibleHoldouts[5] = true;
                }
                if(holdshape && !holdcolor && holdsound)
                {
                    PossibleHoldouts[3] = true;
                }
                if(holdshape && holdcolor && !holdsound)
                {
                    PossibleHoldouts[4] = true;
                    
                }
                if(!holdshape && holdcolor && holdsound)
                {
                    PossibleHoldouts[5] = true;
                }
            }//End All Signals Case
            
            //Check Previous Match Pod
            bool prevHoldSound = (prevMatch.podSound == POD_SOUND_HOLDOUT);
            bool prevHoldColor = (prevMatch.podColor == POD_COLOR_HOLDOUT);
            bool prevHoldShape= (prevMatch.podShape == POD_SHAPE_HOLDOUT);
            
            if (prevHoldShape && prevHoldSound)
            {
                PossibleHoldouts[3] = false;
            }
            else if(prevHoldColor && prevHoldShape)
            {
                PossibleHoldouts[4] = false;
            }
            else if(prevHoldColor && prevHoldSound)
            {
                PossibleHoldouts[5] = false;
            }
            else if(prevHoldColor && !prevHoldSound && !prevHoldShape)
            {
                PossibleHoldouts[0] = false;
            }
            else if(!prevHoldColor && prevHoldSound && !prevHoldShape)
            {
                PossibleHoldouts[1] = false;
            }
            else if(!prevHoldColor && !prevHoldSound && prevHoldShape)
            {
                PossibleHoldouts[2] = false;
            }
            
            for(int i =0; i < 7; i++) //Count the number of Possibilities
            {
                if(PossibleHoldouts[i])
                {
                    std::cout << "THIS WAS TRUE: " << i << std::endl;
                    sizeOfPossibilites++;
                }
            }
            std::cout << "***********Size of Possibilities: " << sizeOfPossibilites << std::endl;
            
            if(sizeOfPossibilites > 0)
            {
                //Choose a random holdout type and apply to the signal
                
                int rand_holdout_type = Util::randRangeInt(1, sizeOfPossibilites);
                int chosenHoldoutTypeIndex = 0;
                std::cout << "***********The random holdout type chosen: " << rand_holdout_type << std::endl;
                
                for(int i = 0; i < 7; i++)
                {
                    if(PossibleHoldouts[i])
                    {
                        rand_holdout_type--;
                        if(rand_holdout_type == 0)
                        {
                            chosenHoldoutTypeIndex = i;
                        }
                    }
                    
                }
                std::cout << "***********Index Chosen: " << chosenHoldoutTypeIndex << std::endl;
                
                //Apply the holdout
                //List of possible holdouts!
                //0: Color Holdout Only
                //1: Sound Holdout Only
                //2: Shape Holdout Only
                //3: ShapeSound Holdout Only
                //4: ShapeColor Holdout Only
                //5: ColorSound Holdout Only
                if(chosenHoldoutTypeIndex == 0)
                {
                    podColor = POD_COLOR_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 1)
                {
                    podSound = POD_SOUND_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 2)
                {
                    podShape = POD_SHAPE_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 3)
                {
                    podShape = POD_SHAPE_HOLDOUT;
                    podSound = POD_SOUND_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 4)
                {
                    podShape = POD_SHAPE_HOLDOUT;
                    podColor = POD_COLOR_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 5)
                {
                    podSound = POD_SOUND_HOLDOUT;
                    podColor = POD_COLOR_HOLDOUT;
                }
                
            }
            
            
        }// End applyConstraints
        else  //Distractors that can have holdout may not have holdout.
        {
            //Same thing as above but dont worry about matching because it wont be the same signal anyway.
            //Create the same list of possible holdouts except we dont remove the match and add the possiblity of having
            //All possible features
            if(phase != PHASE_SHAPE_SOUND)
            {
                PossibleHoldouts[0] = holdcolor;
            }
            
            PossibleHoldouts[1] = holdsound;
            
            
            if(phase != PHASE_COLOR_SOUND)
            {
                PossibleHoldouts[2] = holdshape;
            }
            PossibleHoldouts[6] = true;
            
            //All Signals Cases
            if(phase == PHASE_ALL_SIGNAL)
            {
                if(holdshape && holdcolor && holdsound)
                {
                    PossibleHoldouts[3] = true;
                    PossibleHoldouts[4] = true;
                    PossibleHoldouts[5] = true;
                }
                if(holdshape && !holdcolor && holdsound)
                {   
                    PossibleHoldouts[3] = true;
                }
                if(holdshape && holdcolor && !holdsound)
                {
                    PossibleHoldouts[4] = true;
                    
                }
                if(!holdshape && holdcolor && holdsound)
                {
                    PossibleHoldouts[5] = true;
                }
            }//End All Signals Case
            
            for(int i =0; i < 7; i++) //Count the number of Possibilities
            {
                if(PossibleHoldouts[i])
                {
                    std::cout << "Distractor Holdout THIS WAS TRUE: " << i << std::endl;
                    sizeOfPossibilites++;
                }
            }
            std::cout << "***********Distractor Size of Possibilities: " << sizeOfPossibilites << std::endl;
            
            if(sizeOfPossibilites > 0)
            {
                //Choose a random holdout type and apply to the signal
                
                int rand_holdout_type = Util::randRangeInt(1, sizeOfPossibilites);
                int chosenHoldoutTypeIndex = 0;
                std::cout << "***********Distractor random holdout type chosen: " << rand_holdout_type << std::endl;
                
                for(int i = 0; i < 7; i++)
                {
                    if(PossibleHoldouts[i])
                    {
                        rand_holdout_type--;
                        if(rand_holdout_type == 0)
                        {
                            chosenHoldoutTypeIndex = i;
                        }
                    }
                    
                }
                std::cout << "*********** Distractor Index Chosen: " << chosenHoldoutTypeIndex << std::endl;
                
                //Apply the holdout
                //List of possible holdouts!
                //0: Color Holdout Only
                //1: Sound Holdout Only
                //2: Shape Holdout Only
                //3: ShapeSound Holdout Only
                //4: ShapeColor Holdout Only
                //5: ColorSound Holdout Only
                //6: NoHoldout
                if(chosenHoldoutTypeIndex == 0)
                {
                    podColor = POD_COLOR_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 1)
                {
                    podSound = POD_SOUND_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 2)
                {
                    podShape = POD_SHAPE_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 3)
                {
                    podShape = POD_SHAPE_HOLDOUT;
                    podSound = POD_SOUND_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 4)
                {
                    podShape = POD_SHAPE_HOLDOUT;
                    podColor = POD_COLOR_HOLDOUT;
                    
                }
                else if(chosenHoldoutTypeIndex == 5)
                {
                    podSound = POD_SOUND_HOLDOUT;
                    podColor = POD_COLOR_HOLDOUT;
                }
                //else if (chosenHoldoutTypeIndex == 6) No Holdout
                
            }
            
            
        }

    }
    
    
    /*
    
    //float rand_holdOut = Ogre::Math::UnitRandom();
    float rand_signal = Ogre::Math::UnitRandom();
    
    if(!applyConstraints ){//true ) {//|| rand_holdOut < 0.1f ) {      Do this for not a match.
        switch(phase) {
            case PHASE_COLOR_SOUND:   // levels that normally have color and sound active
                //if sound is not available, holdout is not availabe
                if(sound) {
                    if(holdcolor&&!holdsound) //Color Holdout Only
                    {
                        podColor = POD_COLOR_HOLDOUT;
                        std::cout << "Hold out: color" << std::endl;
                    }
                    else if(!holdcolor&&holdsound) //Sound Holdout Only
                    {
                        podSound = POD_SOUND_HOLDOUT;
                        std::cout << "Hold out: sound" << std::endl;
                    }
                    else if( rand_signal < 0.5f)
                    {
                        podColor = POD_COLOR_HOLDOUT;
                        std::cout << "Hold out: color" << std::endl;
                    }
                    else
                    {
                        podSound = POD_SOUND_HOLDOUT;
                        std::cout << "Hold out: sound" << std::endl;
                    }
                }
                break;
            case PHASE_SHAPE_SOUND:   // levels that normally have shape and sound active
                if(sound) {
                    if(holdshape&&!holdsound){
                        
                        podShape = POD_SHAPE_HOLDOUT;
                        std::cout << "Hold out: shape" << std::endl;
                    }
                    else if(!holdshape&&holdsound){
                        
                        podSound = POD_SOUND_HOLDOUT;
                        std::cout << "Hold out: sound" << std::endl;
                    }
                    else if( rand_signal < 0.5f) {
                        
                        podShape = POD_SHAPE_HOLDOUT;
                        std::cout << "Hold out: shape" << std::endl;
                    }
                    else {
                        podSound = POD_SOUND_HOLDOUT;
                        std::cout << "Hold out: sound" << std::endl;
                    }
                }
                break;
            case PHASE_SOUND_ONLY:   // levels that normally have sound only active
                break;
            case PHASE_ALL_SIGNAL:   // levels that normally have all three signals active
                if(sound){
                    if (holdshape&&holdcolor&&holdsound) // color shape sound enabled
                    {
                        if( rand_signal < 0.167f ) {
                            podColor = POD_COLOR_HOLDOUT;
                            podShape = POD_SHAPE_HOLDOUT;
                            std::cout << "Hold out: color and shape (sound only)" << std::endl;
                        }
                        else if( rand_signal < 0.333f ) {
                            podSound = POD_SOUND_HOLDOUT;
                            podShape = POD_SHAPE_HOLDOUT;
                            std::cout << "Hold out: sound and shape (color only)" << std::endl;
                        }
                        else if( rand_signal < 0.500f ) {
                            podColor = POD_COLOR_HOLDOUT;
                            podSound = POD_SOUND_HOLDOUT;
                            std::cout << "Hold out: color and sound (shape only)" << std::endl;
                        }
                        else if( rand_signal < 0.667f ) {
                            podColor = POD_COLOR_HOLDOUT;
                            std::cout << "Hold out: color (shape and sound only)" << std::endl;
                        }
                        else if( rand_signal < 0.824f ) {
                            podShape = POD_SHAPE_HOLDOUT;
                            std::cout << "Hold out: shape (color and sound only)" << std::endl;
                        }
                        else {
                            podSound = POD_SOUND_HOLDOUT;
                            std::cout << "Hold out: sound (color and shape only)" << std::endl;
                        }
                    }
                    else if(!holdcolor&&holdshape&&holdsound) { //shape sound enabled
                        if( rand_signal < 0.5f) {
                            
                            podShape = POD_SHAPE_HOLDOUT;
                            std::cout << "Hold out: shape" << std::endl;
                        }
                        else {
                            podSound = POD_SOUND_HOLDOUT;
                            std::cout << "Hold out: sound" << std::endl;
                        }
                    }
                    else if(!holdshape&&holdcolor&&holdsound) { //color and sound enabled
                        if( rand_signal < 0.5f) {
                            
                            podColor = POD_COLOR_HOLDOUT;
                            std::cout << "Hold out: color" << std::endl;
                        }
                        else {
                            podSound = POD_SOUND_HOLDOUT;
                            std::cout << "Hold out: sound" << std::endl;
                        }
                    }
                    else if(holdshape&&holdcolor&&!holdsound) {     //shape color enabled
                        if( rand_signal < 0.5f) {
                            
                            podColor = POD_COLOR_HOLDOUT;
                            std::cout << "Hold out: color" << std::endl;
                        }
                        else {
                            podShape = POD_SHAPE_HOLDOUT;
                            std::cout << "Hold out: shape" << std::endl;
                        }
                    }
                    else if (holdcolor){
                        podColor = POD_COLOR_HOLDOUT;
                        std::cout << "Hold out: color" << std::endl;
                    }
                    else if (holdshape){
                        podShape = POD_SHAPE_HOLDOUT;
                        std::cout << "Hold out: shape" << std::endl;
                    }
                    else if (holdsound){
                        podSound = POD_SOUND_HOLDOUT;
                        std::cout << "Hold out: sound" << std::endl;
                    }
                    
                }
                else{   //if sound is disabled
                    if(holdshape&&holdcolor) {     //shape color enabled
                        if( rand_signal < 0.5f) {
                            
                            podColor = POD_COLOR_HOLDOUT;
                            std::cout << "Hold out: color" << std::endl;
                        }
                        else {
                            podShape = POD_SHAPE_HOLDOUT;
                            std::cout << "Hold out: shape" << std::endl;
                        }
                    }
                    else if (holdcolor){
                        podColor = POD_COLOR_HOLDOUT;
                        std::cout << "Hold out: color" << std::endl;
                    }
                    else if (holdshape){
                        podShape = POD_SHAPE_HOLDOUT;
                        std::cout << "Hold out: shape" << std::endl;
                    }
                    
                }
                break;
            case PHASE_COLLECT:   // recess levels
                break;
            default:
                break;
        }//end switch
    }//end if
     */
}

Vector3 Util::EulerRotate(Vector3 v, Degree d, char Axis)
{
    switch(Axis) {
        case 'x':
            return Vector3(v.x, v.y * Math::Cos(d) - v.z * Math::Sin(d), v.y * Math::Sin(d) + v.z * Math::Cos(d));
        case 'y':
            return Vector3(v.x * Math::Cos(d) + v.z * Math::Sin(d), v.y, -v.x * Math::Sin(d) + v.z * Math::Cos(d));
        case 'z':
            return Vector3(v.x * Math::Cos(d) - v.y * Math::Sin(d), v.x * Math::Sin(d) + v.y * Math::Cos(d), v.z);
        default:
            return Vector3(0,0,0);
    }
}
