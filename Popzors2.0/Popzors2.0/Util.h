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
    int randRangeInt(int min, int max);
    double randRangeDouble(double min, double max);
    std::string toStringInt(int value);
    std::string toStringDouble(double value);
    
    void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);
    void createCylinder(const std::string& strName, const float r, const float h, const int nSegments = 16);
    void createUncappedCylinder(const std::string& strName, const float r, const float h, const int nSegments = 16);

    void generateMaterials();
    
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
};


#endif /* defined(__Testing__Util__) */

