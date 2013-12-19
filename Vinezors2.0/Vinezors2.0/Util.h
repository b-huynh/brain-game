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
enum ProgressionMode { SIMPLE_PROGRESSIVE, SIMPLE_MULTISENSORY, DISTRIBUTIVE_ADAPTIVE };
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
    static const double EPSILON = 0.001;
    
    struct ConfigGlobal
    {
        int stageID;
        double sessionTime;
        double stageTime;
        int stageTotalSignals;
        int stageTotalTargets;
        int stageTotalTargetsVariance;
        double stageProficiencyThreshold1;
        double stageProficiencyThreshold2;
        double stageProficiencyThreshold3;
        double stageTimeThreshold1;
        double stageTimeThreshold2;
        double stageTimeThreshold3;
        double stageNavigationThreshold1;
        double stageNavigationThreshold2;
        double stageNavigationThreshold3;
        int progressionMode;
        int gameMode;
        int podTestType;
        int revealColor;
        int revealSound;
        int revealShape;
        Vector3 tunnelReferenceForward;
        Vector3 tunnelReferenceUpward;
        Vector3 tunnelReferenceRight;
        int tunnelMinAngleTurn;
        int tunnelMaxAngleTurn;
        double tunnelSegmentWidth;
        double tunnelSegmentDepth;
        double tunnelSegmentBuffer;
        double tunnelWallLength;
        int tunnelSegmentsPerSection;
        int tunnelSegmentsPerPod;
        int tunnelSegmentsPerDistractors;
        int tunnelSegmentsBeforeRefresh;
        int initialSegmentsFirstPod;
        int initialSegmentsFirstDistractors;
        int initiationSections;
        double vineTOffset;
        double vineRadius;
        int podAppearance;
        double podHeadRadius;
        double podStemRadius;
        double podStemLength;
        int podBinSize; // This is for tunnels that pre-generate the pod sequence
        int podNBackChance; // This is for tunnels that don't pre-generate the pod sequence
        double seatLength;
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
        double drainSpeed;
        double initCamSpeed;
        double startupCamSpeed;
        double modifierCamSpeed;
        double minCamSpeed;
        double maxCamSpeed;
        double nlevelSpeedModifier;
        int numToSpeedUp;
        int numToSpeedDown;
        double stepsizeSpeedUp;
        double stepsizeSpeedDown;
        double HPBarXRef;
        double HPBarYRef;
        double HPBarWidth;
        double HPBarHeight;
        double timedRunControlUpDist1;
        double timedRunControlUpDist2;
        double timedRunControlUpDist3;
        int timedRunNMax;
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
        bool loadSaveFile(std::string savePath);
        
        void setMessage(std::string msg, MessageType type);
        void clearMessage();
        bool setName(const char* name);
        std::string buildLogPath(std::string playerName);
    };
    
    double clamp(double val, double min, double max);
    int getDegrees(Direction dir);
    Direction leftOf(Direction dir);
    Direction rightOf(Direction dir);
    Direction oppositeOf(Direction dir);
    void setSides(bool sides[NUM_DIRECTIONS], int level, Direction dir);
    Direction randDirection();
    Direction randDirection(const bool sides[NUM_DIRECTIONS]);
    Vector3 randVector3();
    
    int randRangeInt(int min, int max);
    double randRangeDouble(double min, double max);
    std::string toStringInt(int value);
    std::string toStringDouble(double value);
    std::string getOSXDir();
    std::string getIOSDir();
    
    void drawRect(ManualObject* obj, double x, double y, double width, double height, const ColourValue & col = ColourValue(1.0, 1.0, 1.0, 0.0), bool filled = false); // (x, y) is bottom left in this function
    
    void createSphere(Ogre::SceneManager* sceneMgr, const std::string& strName, float r, int nRings = 16, int nSegments = 16);
    void createCylinder(Ogre::SceneManager* sceneMgr, const std::string& strName, float r,  float h, int nSegments = 16);
    void createDiamond(Ogre::SceneManager* sceneMgr, const std::string& strName, float w, float h);
    void createBox(Ogre::SceneManager* sceneMgr, const std::string& strName, float l, float w, float h);
    void createPlane(Ogre::SceneManager* sceneMgr, const std::string& strName, float length, float depth);
    
    void generateMaterials();
};


#endif /* defined(__Testing__Util__) */

