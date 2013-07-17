#pragma once

#include "Pch.h"
#include "Ground.h"
#include "Poppy.h"
#include "Pot.h"
#include <vector>

using namespace std;

class PoppyPattern
{
public:
    PoppyPattern();
    
    void reset();
    void setPoppyPattern(CollisionScene *scene, Ground & ground, vector<Pot> & pots, vector<Poppy> & poppies);
    void updatePlayerChoice(Ground & ground, Poppy & poppy, Pot & pot);
    bool isPlayerDone();
    bool isPlayerRight();
    bool isReady();
    void updatePoppyBlinks(Number elapsed, vector<Pot> & pots, vector<Poppy> & poppies);
    
    void update(Number elapsed, Ground & ground, vector<Pot> & pots, vector<Poppy> & poppies);
private:
	bool signaled;
	Number signalStart;
    Number signalLength;
    Number timer;
    bool ready;
    
    vector<int> poppyOrderList;
    bool backwardsOrder;
	int blinkPoppyIndex;
    int playerPoppyIndex;
    bool playerIsRight;
    int playerLevel;
};
