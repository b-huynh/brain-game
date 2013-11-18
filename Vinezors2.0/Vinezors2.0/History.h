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

enum CoverMode { FIRST_TIME, PLAYING };

struct History
{
    Ogre::SceneManager* sceneMgr;
    
    int nback;
    std::vector<SceneNode*> seats;
    std::vector<Pod*> list;
    SceneNode* coverNode;
    Vector3 coverDest;
    
    History(Ogre::SceneManager* sceneMgr, int nback);
    
    void initSeats(int nback);
    void addPod(const PodInfo & podInfo);
    void revealPod();
    void update(double elapsed);
    void setCoverDest(int seatInd);
    void determineCoverLoc(bool success);
    
    double panelHeight;
    double panelX;
    double panelY;
    double reservedHeight;
    double coverHeight;
    double coverOffsetY;
    
    int saveInd;
    int coverInd;
    
    CoverMode mode;
    
    ~History();
};

#endif /* defined(__Vinezors2_0__History__) */
