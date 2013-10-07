//
//  Util.h
//  Testing
//
//  Created by Calvin Phung on 8/27/13.
//
//

#ifndef __Testing__Util__
#define __Testing__Util__

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
    static const int TUNNEL_MIN_ANGLE_TURN = 3;
    static const int TUNNEL_MAX_ANGLE_TURN = 3;
    static const double TUNNEL_WIDTH = 25.0;
    static const double TUNNEL_DEPTH = 25.0;
    static const double TUNNEL_SEGMENT_BUFFER = 25;
    static const double TUNNEL_WALL_LENGTH = TUNNEL_WIDTH / (2 * Math::Cos(Ogre::Radian(Math::PI) / 4) + 1);
    static const int TUNNEL_SEGMENTS_PER_SECTION = 5;
    static const int TUNNEL_SEGMENTS_PER_POD = 3;
    static const int TUNNEL_SEGMENTS_BEFORE_REFRESH = TUNNEL_SEGMENTS_PER_SECTION * 2;
    static const int INITIATION_SECTIONS = 1;
    static const double VINE_T_OFFSET = 1.0;
    static const double VINE_RADIUS = 2.5;
    static const int POD_APPEARANCE = 2;
    static const double POD_HEAD_RADIUS = TUNNEL_WIDTH / 25;
    static const double POD_STEM_RADIUS = TUNNEL_WIDTH / 100;
    static const double POD_STEM_LENGTH = TUNNEL_WALL_LENGTH / 2;
    
    static const int TUNNEL_SECTIONS = 5;
    static const int NBACK = 2;
    
    static const int STARTING_HP = 0;
    static const int HP_NEGATIVE_LIMIT = -3;
    static const int HP_POSITIVE_LIMIT = 6;
    static const double DRAIN_SPEED = 2;    
    static const int CAM_SPEED = 50; //100;
    static const int MIN_CAM_SPEED = 25;
    static const int MAX_CAM_SPEED = 125;
    
    static const double HP_BAR_XREF = 0.05;
    static const double HP_BAR_YREF = 0.05;
    static const double HP_BAR_WIDTH = 0.9;
    static const double HP_BAR_HEIGHT = 0.05;
    
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 578;
    static const int LABEL1_POSX = 10;
    static const int LABEL1_POSY = SCREEN_HEIGHT - 50;
    static const int LABEL2_POSX = 10;
    static const int LABEL2_POSY = SCREEN_HEIGHT - 100;
    static const int LABEL3_POSX = SCREEN_WIDTH - 275;
    static const int LABEL3_POSY = SCREEN_HEIGHT - 50;
    static const int LABEL4_POSX = SCREEN_WIDTH - 275;
    static const int LABEL4_POSY = SCREEN_HEIGHT - 100;
    
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
    
    void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);
    void createCylinder(const std::string& strName, const float r, const float h, const int nSegments = 16);
    void createPlane(const std::string& strName, const float length, const float depth);

    void generateMaterials();
};


#endif /* defined(__Testing__Util__) */

