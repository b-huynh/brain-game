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
    void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);

    void generateMaterials();
};


#endif /* defined(__Testing__Util__) */

