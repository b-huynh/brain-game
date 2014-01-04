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

#include <string>

enum Evaluation { PASS, FAIL, EVEN };
enum SpeedControlMode { SPEED_CONTROL_FLEXIBLE, SPEED_CONTROL_AUTO };
enum MessageType { MESSAGE_NONE, MESSAGE_NORMAL, MESSAGE_NOTIFY, MESSAGE_ERROR, MESSAGE_FINAL };
enum MusicMode { MUSIC_ENABLED, MUSIC_DISABLED };
enum SidebarLocation { SIDEBAR_NONE, SIDEBAR_RIGHT, SIDEBAR_BOTTOM_LTR, SIDEBAR_BOTTOM_RTL };
enum Direction { NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NO_DIRECTION };
#define NUM_DIRECTIONS 8
enum PodType { POD_TYPE_COLOR, POD_TYPE_SHAPE, POD_TYPE_SOUND }; // POD_TYPE_LOC, POD_TYPE_SIZE, POD_TYPE_ANIMATION, POD_TYPE_TYPE
#define NUM_POD_TYPES 3
enum PodColor { POD_COLOR_BLUE, POD_COLOR_GREEN, POD_COLOR_PINK, POD_COLOR_YELLOW, POD_COLOR_UNKNOWN };
enum PodShape { POD_SHAPE_SPHERE, POD_SHAPE_TRIANGLE, POD_SHAPE_DIAMOND, POD_SHAPE_CONE, POD_SHAPE_UNKNOWN }; // POD_CYLINDER, POD_BOX
enum PodSound { POD_SOUND_1, POD_SOUND_2, POD_SOUND_3, POD_SOUND_4, POD_SOUND_UNKNOWN };
enum PodSignal { POD_SIGNAL_1, POD_SIGNAL_2, POD_SIGNAL_3, POD_SIGNAL_4, POD_SIGNAL_UNKNOWN };
#define NUM_POD_SIGNALS 5

// Forward Declarations of main components of the game
class Tunnel;
class Player;
class Hud;

namespace Util
{
    static const float EPSILON = 0.0075f;
    
    struct ConfigGlobal
    {
        int stageID;
        float sessionTime;
        float stageTime;
        int stageTotalSignals;
        int stageTotalTargets;
        int stageTotalTargetsVariance;
        float stageProficiencyThreshold1;
        float stageProficiencyThreshold2;
        float stageProficiencyThreshold3;
        int stageTimeThreshold1;
        int stageTimeThreshold2;
        int stageTimeThreshold3;
        int stageNavigationThreshold1;
        int stageNavigationThreshold2;
        int stageNavigationThreshold3;
        int stageStarPassThreshold;
        int gameMode;
        int revealColor;
        int revealSound;
        int revealShape;
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
        int podBinSize; // This is for tunnels that pre-generate the pod sequence
        int podNBackChance; // This is for tunnels that don't pre-generate the pod sequence
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
        int HPPositiveCorrectAnswer;
        int HPPositiveWrongAnswer;
        float distractorSpeedPenalty;
        float distractorTimePenalty;
        float initCamSpeed;
        float startupCamSpeed;
        float modifierCamSpeed;
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
        float timedRunControlUpDist1;
        float timedRunControlUpDist2;
        float timedRunControlUpDist3;
        int setSkyBox;
        int setWallPanelTexture;
        int setVineShip;
        int setPodMesh;
        float swipeSensitivity;
        int swipeInverted;
        
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
        
        int currStageID;
        std::string configPath;
        std::string configBackup;
        std::string logPath;
        std::string savePath;
        std::string playerName;
        std::string message;
        MessageType messageType;
        
        ConfigGlobal();
        void set();
        
        void initPaths(const char* name);
        void setConfigValue(std::istream& in, std::string paramName);
        bool loadConfig(int stageID);
        
        void setMessage(std::string msg, MessageType type);
        void clearMessage();
        bool setName(const char* name);
        std::string buildLogPath(std::string playerName);
    };
    
    float clamp(float val, float min, float max);
    int getDegrees(Direction dir);
    Direction leftOf(Direction dir);
    Direction rightOf(Direction dir);
    Direction oppositeOf(Direction dir);
    void setSides(bool sides[NUM_DIRECTIONS], int level, Direction dir);
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
    
    
    void generateMaterials();
};


#endif /* defined(__Testing__Util__) */

