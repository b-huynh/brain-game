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

enum Direction { NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NO_DIRECTION };
#define NUM_DIRECTIONS 8

namespace Util
{    
    static const double EPSILON = 0.001;
    
    struct ConfigGlobal
    {
        int stageID;
        int soundMode;
        int gameMode;
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
        int tunnelSegmentsBeforeRefresh;
        int initiationSections;
        double vineTOffset;
        double vineRadius;
        int podAppearance;
        double podHeadRadius;
        double podStemRadius;
        double podStemLength;
        double seatLength;
        int tunnelSections;
        int nback;
        int control;
        int historyMode;
        int startingHP;
        int HPNegativeLimit;
        int HPPositiveLimit;
        double drainSpeed;
        double initCamSpeed;
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
        
        ConfigGlobal();
        void set();
    };
    
    int getDegrees(Direction dir);
    Direction leftOf(Direction dir);
    Direction rightOf(Direction dir);
    Direction oppositeOf(Direction dir);
    void setSides(bool sides[NUM_DIRECTIONS], int level, Direction dir);
    Direction randDirection();
    Direction randDirection(const bool sides[NUM_DIRECTIONS]);
    
    int randRangeInt(int min, int max);
    double randRangeDouble(double min, double max);
    std::string toStringInt(int value);
    std::string toStringDouble(double value);
    std::string getSaveDir();
    std::string getIOSDir();
    
    //Config Values
    void setConfigValue (std::string param, double paramVal);
    
    void drawRect(ManualObject* obj, double x, double y, double width, double height, const ColourValue & col = ColourValue(1.0, 1.0, 1.0, 0.0), bool filled = false); // (x, y) is bottom left in this function
    
    void createSphere(Ogre::SceneManager* sceneMgr, const std::string& strName, float r, int nRings = 16, int nSegments = 16);
    void createCylinder(Ogre::SceneManager* sceneMgr, const std::string& strName, float r,  float h, int nSegments = 16);
    void createPlane(Ogre::SceneManager* sceneMgr, const std::string& strName, float length, float depth);

    void generateMaterials();
};


#endif /* defined(__Testing__Util__) */

