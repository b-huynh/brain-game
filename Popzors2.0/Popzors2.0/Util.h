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
};


#endif /* defined(__Testing__Util__) */

