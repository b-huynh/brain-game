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

namespace Util
{
    static const Vector3 TUNNEL_REFERENCE_FORWARD = Vector3(0, 0, -1);
    static const Vector3 TUNNEL_REFERENCE_UPWARD = Vector3(0, 1, 0);
    static const Vector3 TUNNEL_REFERENCE_RIGHT = Vector3(1, 0, 0);
    static const int TUNNEL_MIN_ANGLE_TURN = 1;
    static const int TUNNEL_MAX_ANGLE_TURN = 1;
    static const double TUNNEL_WIDTH = 5.0;
    static const double TUNNEL_DEPTH = 5.0;
    static const double TUNNEL_SEGMENT_BUFFER = 1;
    static const int TUNNEL_SEGMENTS_PER_SECTION = 5;
    static const int TUNNEL_SEGMENTS_PER_POD = 3;
    static const int INITIATION_SECTIONS = 1;
    static const double VINE_T_OFFSET = 1.0;
    static const double VINE_RADIUS = 0.5;
    static const int POD_APPEARANCE = 3;
    static const double POD_HEAD_RADIUS = 2;
    static const double POD_STEM_RADIUS = 0.5;
    static const double POD_STEM_LENGTH = 10;
    
    static const int TUNNEL_SECTIONS = 3;
    static const int NBACK = 2;
    
    int randRangeInt(int min, int max);
    double randRangeDouble(double min, double max);
    std::string toStringInt(int value);
    std::string toStringDouble(double value);
    
    void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);
    void createCylinder(const std::string& strName, const float r, const float h, const int nSegments = 16);

    void generateMaterials();
};


#endif /* defined(__Testing__Util__) */

