//
//  History.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 10/14/13.
//
//

#ifndef __Vinezors2_0__History__
#define __Vinezors2_0__History__

#include "Util.h"
#include "TunnelSlice.h"
#include "Pod.h"
#include <vector>

struct History
{
    Ogre::SceneManager* sceneMgr;

    int nback;
    std::vector<bool> covered;
    std::vector<SceneNode*> seats;
    std::vector<Pod*> list;
    
    History(Ogre::SceneManager* sceneMgr, int nback);
    
    void initSeats(int nback);
    void addPod(const PodInfo & podInfo);
    void update(double elapsed);
    
    ~History();
};

#endif /* defined(__Vinezors2_0__History__) */
