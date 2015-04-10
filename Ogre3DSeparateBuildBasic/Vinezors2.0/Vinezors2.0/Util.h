//
//  Util.h
//  Testing
//
//  Created by Calvin Phung on 8/27/13.
//
//

#ifndef __Vinezors2_0__Util__
#define __Vinezors2_0__Util__

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreFramework.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

//#define DEBUG_MODE 1

#include <string>

enum GameState { STATE_PLAY, STATE_PROMPT };
enum Evaluation { PASS, FAIL, EVEN };
enum SpeedControlMode { SPEED_CONTROL_FLEXIBLE, SPEED_CONTROL_AUTO };
enum MessageType { MESSAGE_NONE, MESSAGE_NORMAL, MESSAGE_NOTIFY, MESSAGE_ERROR, MESSAGE_FINAL };
enum MusicMode { MUSIC_ENABLED, MUSIC_DISABLED };
enum SidebarLocation { SIDEBAR_NONE, SIDEBAR_RIGHT, SIDEBAR_BOTTOM_LTR, SIDEBAR_BOTTOM_RTL };
enum ActionCode { ACTION_NONE, ACTION_SINGLE_TAP, ACTION_DOUBLE_TAP, ACTION_TAP_HOLD, ACTION_SWIPE_LEFT, ACTION_SWIPE_RIGHT, ACTION_PINCH };

enum Direction { NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NO_DIRECTION };
#define NUM_DIRECTIONS 8

// Tunnel Section/Segment Info
enum TunnelType { NORMAL, BLANK, CHECKPOINT_PASS, CHECKPOINT_FAIL, CHECKPOINT_EVEN };
struct SectionInfo
{
    TunnelType tunnelType;
    Direction tunnelDir; // The direction each segment is turning
    int tunnelDirAngle; // The amount of turning degrees for each segment
    Quaternion tunnelRot;
    bool sidesUsed[NUM_DIRECTIONS];
    
    SectionInfo()
    : tunnelType(NORMAL), tunnelDir(NO_DIRECTION), tunnelDirAngle(0), tunnelRot(), sidesUsed()
    {
        for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = false;
    }
    
    SectionInfo(const SectionInfo & info)
    : tunnelType(info.tunnelType), tunnelDir(info.tunnelDir), tunnelDirAngle(info.tunnelDirAngle), tunnelRot(info.tunnelRot), sidesUsed()
    {
        for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = info.sidesUsed[i];
        
    }
    
    SectionInfo(TunnelType tt, Direction td, int tda, Quaternion rot, const bool used[NUM_DIRECTIONS])
    : tunnelType(tt), tunnelDir(td), tunnelDirAngle(tda), tunnelRot(rot), sidesUsed()
    {
        for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = used[i];
    }
};

// Pod Info
enum PodMeshType { POD_BASIC, POD_FUEL, POD_FLOWER, POD_HAZARD };
enum PodType { POD_TYPE_COLOR, POD_TYPE_SHAPE, POD_TYPE_SOUND }; // POD_TYPE_LOC, POD_TYPE_SIZE, POD_TYPE_ANIMATION, POD_TYPE_TYPE
#define NUM_POD_TYPES 3
enum PodColor { POD_COLOR_BLUE, POD_COLOR_GREEN, POD_COLOR_PINK, POD_COLOR_YELLOW, POD_COLOR_PURPLE, POD_COLOR_UNKNOWN };
enum PodSound { POD_SOUND_1, POD_SOUND_2, POD_SOUND_3, POD_SOUND_4, POD_SOUND_UNKNOWN };
enum PodShape { POD_SHAPE_DIAMOND, POD_SHAPE_SPHERE, POD_SHAPE_CONE, POD_SHAPE_TRIANGLE, POD_SHAPE_UNKNOWN }; // POD_CYLINDER, POD_BOX
enum PodSignal { POD_SIGNAL_1, POD_SIGNAL_2, POD_SIGNAL_3, POD_SIGNAL_4, POD_SIGNAL_UNKNOWN };
#define NUM_POD_SIGNALS 5 // For assigning sounds to each index
struct PodInfo
{
    bool podExists;
    PodSignal podSignal;
    PodMeshType meshType;
    PodColor podColor;
    PodShape podShape;
    PodSound podSound;
    Direction podLoc;
    bool goodPod; // is the pod good to take?
    bool podTrigger; // trigger on: false = after pod has past, true = on collision
    bool podTaken; // is the pod gone?
    
    PodInfo()
    : podExists(true), podSignal(POD_SIGNAL_UNKNOWN), meshType(POD_BASIC), podColor(POD_COLOR_UNKNOWN), podShape(POD_SHAPE_UNKNOWN), podSound(POD_SOUND_UNKNOWN),
    podLoc(NO_DIRECTION), goodPod(false), podTrigger(false), podTaken(false)
    {}
    
    PodInfo(PodSignal psig, PodMeshType mtype, PodColor pcol, PodShape pshp, PodSound psod, Direction pl = NO_DIRECTION, bool good = false, bool trigger = false, bool taken = false)
    : podExists(true), podSignal(psig), meshType(mtype), podColor(pcol), podShape(pshp), podSound(psod), podLoc(pl), goodPod(good), podTrigger(trigger), podTaken(taken)
    {}
};

// Vine Info
enum VineMeshType { VINE_BASIC_SHIP, VINE_RUNNER_SHIP, VINE_FLOWER_SHIP, VINE_NONE };

struct NavigationLevel
{
    int level;
    int control;
    int obstacles;
    
    NavigationLevel() : level(0), control(0), obstacles(0) {}
    NavigationLevel(int l, int c, int o) : level(l), control(c), obstacles(o) {}
};

// Forward Declarations of main components of the game
class Tunnel;
class Player;
class Hud;
class LevelManager;

namespace Util
{
    static const float EPSILON = 0.005f;

    struct ConfigGlobal
    {
        std::string scheduleMain;
        std::string scheduleRepeat;
        std::string scheduleRepeatRandomPool;
        int stageID;
        float sessionTime;
        float stageTime;
        int stageTotalSignals;
        int stageTotalTargets1;
        int stageTotalTargets2;
        int stageTotalTargets3;
        int stageTotalCollections;
        int stageTotalTargetsVariance;
        int stageTimeThreshold1;
        int stageTimeThreshold2;
        int stageTimeThreshold3;
        int set1Repetitions;
        int set2Repetitions;
        int set3Repetitions;
        int set1StartingSpeed;
        int set2StartingSpeed;
        int set3StartingSpeed;
        int trialStartingSpeed;
        int startingNav;
        Vector3 tunnelReferenceForward;
        Vector3 tunnelReferenceUpward;
        Vector3 tunnelReferenceRight;
        int tunnelMinAngleTurn;
        int tunnelMaxAngleTurn;
        float tunnelSegmentWidth;
        float tunnelSegmentDepth;
        float tunnelSegmentBuffer;
        float tunnelWallLength;
        int tunnelSegmentsPerSection;
        int tunnelSegmentsPerPod;
        int tunnelSegmentsPerDistractors;
        int tunnelSegmentsBeforeRefresh;
        int tunnelSectionsPerNavigationUpgrade;
        int initialSegmentsFirstPod;
        int initialSegmentsFirstDistractors;
        int initiationSections;
        float vineTOffset;
        float vineRadius;
        float vineTransitionSpeed;
        int podAppearance;
        float podHeadRadius;
        float podStemRadius;
        float podStemLength;
        float podRotateSpeed;
        float podCollisionMin;
        float podCollisionMax;
        float distractorCollisionMin;
        float distractorCollisionMax;
        int podBinSize1; // This is for tunnels that pre-generate the pod sequence
        int podBinSize2;
        int podBinSize3;
        float podNBackChance;
        int span; // The range left and right for a target to spawn from the previous target
        int stageTotalDistractorsMin;
        int stageTotalDistractorsMax;
        float seatLength;
        int tunnelSections;
        int nback;
        int control;
        int historyMode;
        int startingHP;
        int HPNegativeLimit;
        int HPPositiveLimit;
        int HPNegativeCorrectAnswer;
        int HPNegativeWrongAnswer;
        int HPNegativeDistractor;
        int HPPositiveCorrectAnswer;
        int HPPositiveWrongAnswer;
        int HPPositiveDistractor;
        float distractorSpeedPenalty;
        float distractorTimePenalty;
        float initCamSpeed;
        float startupCamSpeed;
        float globalModifierCamSpeed;
        float boostModifierCamSpeed;
        float minCamSpeed;
        float maxCamSpeed;
        float nlevelSpeedModifier;
        int numToSpeedUp;
        int numToSpeedDown;
        float stepsizeSpeedUp;
        float stepsizeSpeedDown;
        float HPBarXRef;
        float HPBarYRef;
        float HPBarWidth;
        float HPBarHeight;
        int setSkyBox;
        int setWallPanelTexture;
        VineMeshType setVineShip;
        int setPodMesh;
        float swipeSensitivity;
        int swipeInverted;
        int boostEnabled;
        int combo1MinA;
        int combo2MinA;
        int combo1MinB;
        int combo2MinB;
        int numNavPhases;
        int numTimePhases;
        int expectedNumSessions;
        float sessionTimeMin;
        float sessionTimeMax;
        float timeTrialLengthMin;
        float timeTrialLengthMax;
        int navUnlockMax0;
        int navUnlockNBack1;
        int navUnlockMax1;
        int navUnlockNBack2;
        int navUnlockMax2;
        float volumeMusic;
        float volumeSignal1;
        float volumeSignal2;
        float volumeSignal3;
        float volumeSignal4;
        float volumeFeedbackGood;
        float volumeFeedbackBad;
        float volumeFeedbackCollision;
        float volumeBoost;
        float volumeStartup;
        
        int screenWidth;
        int screenHeight;
        int viewportMainWidth_modeRight;
        int viewportMainHeight_modeRight;
        int viewportSideWidth_modeRight;
        int viewportSideHeight_modeRight;
        int viewportMainWidth_modeBottom;
        int viewportMainHeight_modeBottom;
        int viewportSideWidth_modeBottom;
        int viewportSideHeight_modeBottom;
        int viewportMainWidth_modeNone;
        int viewportMainHeight_modeNone;
        int viewportSideWidth_modeNone;
        int viewportSideHeight_modeNone;
        int label1_posX;
        int label1_posY;
        int label2_posX;
        int label2_posY;
        int label3_posX;
        int label3_posY;
        int label4_posX;
        int label4_posY;
        int label5_posX;
        int label5_posY;
        int label6_posX;
        int label6_posY;
        int label7_posX;
        int label7_posY;
        float pauseButton_posX;
        float pauseButton_posY;
        float pauseButton_width;
        float pauseButton_height;
        
        std::vector<std::vector<PodInfo> > signalTypes;
        std::vector<NavigationLevel> navMap;
        std::map<int, float> speedMap;
        int navIndex;
        int numSegmentsWithObstacles;
        int previousNumSegmentsWithObstacles;
        
        int currStageID;
        std::string configPath;
        std::string configBackup;
        std::string logPath;
        std::string actionPath;
        std::string sessionPath;
        std::string savePath;
        std::string playerName;
        std::string messageBig;
        std::string message;
        MessageType messageType;
        
        ConfigGlobal();
        void set();
        
        void initPaths();
        void initLogs(int session);
        void setConfigValue(std::istream& in, std::string paramName);
        bool loadConfig(int stageID);
        
        void setBigMessage(std::string msg);
        void setMessage(std::string msg, MessageType type);
        void appendMessage(std::string msg, MessageType type);
        void clearMessage();
        bool setName(const char* name);
        std::string buildPath(std::string ext, std::string playerName, int session);
    };
    
    float clamp(float val, float min, float max);
    int getDegrees(Direction dir);
    Direction leftOf(Direction dir);
    Direction rightOf(Direction dir);
    Direction oppositeOf(Direction dir);
    bool doSidesMatch(bool sides1[NUM_DIRECTIONS], bool sides2[NUM_DIRECTIONS]);
    void setSides(bool sides[NUM_DIRECTIONS], int level, Direction dir);
    void copySides(bool dest[NUM_DIRECTIONS], bool src[NUM_DIRECTIONS]); // C.P.
    int getNumSides(int level);
    int getNumSides(bool sides[NUM_DIRECTIONS]);
    Direction randDirection();
    Direction randDirection(const bool sides[NUM_DIRECTIONS]);
    Vector3 randVector3();
    
    int randRangeInt(int min, int max);
    float randRangeFloat(float min, float max);
    std::string toStringInt(int value);
    std::string toStringFloat(float value);
    std::string getOSXDir();
    std::string getIOSDir();
    
    void drawRect(ManualObject* obj, float x, float y, float width, float height, const ColourValue & col = ColourValue(1.0, 1.0, 1.0, 0.0), bool filled = false); // (x, y) is bottom left in this function
    
    void createSphere(Ogre::SceneManager* sceneMgr, const std::string& strName, float r, int nRings = 16, int nSegments = 16);
    void createCylinder(Ogre::SceneManager* sceneMgr, const std::string& strName, float r,  float h, int nSegments = 16);
    void createDiamond(Ogre::SceneManager* sceneMgr, const std::string& strName, float w, float h);
    void createBox(Ogre::SceneManager* sceneMgr, const std::string& strName, float l, float w, float h);
    void createPlane(Ogre::SceneManager* sceneMgr, const std::string& strName, float length, float depth);
    void createSubPlane(Ogre::SceneManager* sceneMgr, ManualObject* manual, float length, float depth, Vector3 loc, Quaternion rot, Vector3 & bl, Vector3 & tr);
    void createSegment(Ogre::SceneManager* sceneMgr, const std::string& strName, float length, float depth,  const bool sides[NUM_DIRECTIONS]);
    void createDefaultSegments(Ogre::SceneManager* sceneMgr);
    
    void tuneProficiencyExam(ConfigGlobal & globals, float initSpeed, float lengthPerTarget, float approxTotalTime, float bestTime);
    float getModdedLengthByNumSegments(const ConfigGlobal & globals, int numSegments);
    
    void generateMaterials();
};


#endif /* defined(__Testing__Util__) */

