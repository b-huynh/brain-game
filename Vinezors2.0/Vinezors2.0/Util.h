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
    static const Vector3 TUNNEL_REFERENCE_FORWARD = Vector3(0, 0, -1);
    static const Vector3 TUNNEL_REFERENCE_UPWARD = Vector3(0, 1, 0);
    static const Vector3 TUNNEL_REFERENCE_RIGHT = Vector3(1, 0, 0);
    static const int TUNNEL_MIN_ANGLE_TURN = 0;
    static const int TUNNEL_MAX_ANGLE_TURN = 3;
    static const double TUNNEL_SEGMENT_WIDTH = 25.0;
    static const double TUNNEL_SEGMENT_DEPTH = 25.0;
    static const double TUNNEL_SEGMENT_BUFFER = 25;
    static const double TUNNEL_WALL_LENGTH = TUNNEL_SEGMENT_WIDTH / (2 * Math::Cos(Ogre::Radian(Math::PI) / 4) + 1);
    static const int TUNNEL_SEGMENTS_PER_SECTION = 5;
    static const int TUNNEL_SEGMENTS_PER_POD = 3;
    static const int TUNNEL_SEGMENTS_BEFORE_REFRESH = TUNNEL_SEGMENTS_PER_SECTION * 2;
    static const int INITIATION_SECTIONS = 1;
    static const double VINE_T_OFFSET = 1.5;
    static const double VINE_RADIUS = 2.5;
    static const int POD_APPEARANCE = 2;
    static const double POD_HEAD_RADIUS = TUNNEL_SEGMENT_WIDTH / 25;
    static const double POD_STEM_RADIUS = TUNNEL_SEGMENT_WIDTH / 100;
    static const double POD_STEM_LENGTH = TUNNEL_WALL_LENGTH / 2;
    static const double SEAT_LENGTH = POD_HEAD_RADIUS * 2;
    
    static const int TUNNEL_SECTIONS = 5;
    static const int NBACK = 2;
    
    static const int STARTING_HP = 0;
    static const int HP_NEGATIVE_LIMIT = -3;
    static const int HP_POSITIVE_LIMIT = 3;
    static const double DRAIN_SPEED = 2;    
    static const int CAM_SPEED = 50; //100;
    static const int MIN_CAM_SPEED = 25;
    static const int MAX_CAM_SPEED = 125;
    
    static const double HP_BAR_XREF = 0.05;
    static const double HP_BAR_YREF = 0.05;
    static const double HP_BAR_WIDTH = 0.9;
    static const double HP_BAR_HEIGHT = 0.05;
    
    static const int SCREEN_WIDTH = 1024;
    static const int SCREEN_HEIGHT = 800;
    
    static const int VIEWPORT_MAIN_WIDTH_MODERIGHT = 800;
    static const int VIEWPORT_MAIN_HEIGHT_MODERIGHT = 800;
    static const int VIEWPORT_SIDE_WIDTH_MODERIGHT = 224;
    static const int VIEWPORT_SIDE_HEIGHT_MODERIGHT = 800;
    
    static const int VIEWPORT_MAIN_WIDTH_MODEBOTTOM = 1024;
    static const int VIEWPORT_MAIN_HEIGHT_MODEBOTTOM = 700;
    static const int VIEWPORT_SIDE_WIDTH_MODEBOTTOM = 1024;
    static const int VIEWPORT_SIDE_HEIGHT_MODEBOTTOM = 100;
    
    static const int VIEWPORT_MAIN_WIDTH_MODENONE = 1024;
    static const int VIEWPORT_MAIN_HEIGHT_MODENONE = 800;
    static const int VIEWPORT_SIDE_WIDTH_MODENONE = 0;
    static const int VIEWPORT_SIDE_HEIGHT_MODENONE = 0;

    static const int LABEL1_POSX = 10;
    static const int LABEL1_POSY = SCREEN_HEIGHT - 70;
    static const int LABEL2_POSX = 10;
    static const int LABEL2_POSY = SCREEN_HEIGHT - 110;
    static const int LABEL3_POSX = 10;
    static const int LABEL3_POSY = SCREEN_HEIGHT - 150;
    static const int LABEL4_POSX = 10;
    static const int LABEL4_POSY = SCREEN_HEIGHT - 190;
    
    static const double EPSILON = 0.001;
    
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
    
    void drawRect(ManualObject* obj, double x, double y, double width, double height, const ColourValue & col = ColourValue(1.0, 1.0, 1.0, 0.0), bool filled = false); // (x, y) is bottom left in this function
    
    void createSphere(Ogre::SceneManager* sceneMgr, const std::string& strName, float r, int nRings = 16, int nSegments = 16);
    void createCylinder(Ogre::SceneManager* sceneMgr, const std::string& strName, float r,  float h, int nSegments = 16);
    void createPlane(Ogre::SceneManager* sceneMgr, const std::string& strName, float length, float depth);

    void generateMaterials();
};


#endif /* defined(__Testing__Util__) */

