//
//  PlayerLevel.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/29/14.
//
//

#include "PlayerLevel.h"
#include "Util.h"

#include <fstream>

extern Util::ConfigGlobal globals;

int PlayerLevel::getMasteredNBack() const
{
    return std::min(set1, std::min(set2, set3));
}

int PlayerLevel::getHighestNBack() const
{
    return std::max(set1, std::max(set2, set3));
}

int PlayerLevel::getNavLimit() const
{
    int best = getHighestNBack();
    if (best >= globals.navUnlockNBack2)
        return globals.navUnlockMax2;
    else if (best >= globals.navUnlockNBack1)
        return globals.navUnlockMax1;
    else
        return globals.navUnlockMax0;
}

PlayerLevel::PlayerLevel()
: sessionID(0), set1Rep(0), set2Rep(0), set3Rep(0), set1(2), set2(2), set3(2), set1Notify(0), set2Notify(0), set3Notify(0), navigation(2), minSpeed(15), averageSpeed(20), maxSpeed(25), runSpeed1(15), runSpeed2(15), runSpeed3(15), navigationScores(), speedScores()
{
}

void PlayerLevel::calculateNavigation()
{
    navigationScores.updateAccuracies();
    
    int nextNavLevel = navigationScores.findMax();
    if (navigationScores.find(navigation) >= 0) // Did the player reach his/her current nav level?
        nextNavLevel = navigation;
    
    // Starting at current nav level, determine whether we should go up or down
    const float THRESHOLD_PASS = 0.85;
    const float THRESHOLD_DROP = 0.70;
    while (navigationScores.find(nextNavLevel) >= 0 &&
           navigationScores[nextNavLevel].accuracy >= THRESHOLD_PASS)
        ++nextNavLevel;
    while (navigationScores.find(nextNavLevel) >= 0 &&
           navigationScores[nextNavLevel].accuracy < THRESHOLD_DROP)
        --nextNavLevel;
    nextNavLevel = Util::clamp(nextNavLevel, navigationScores.findMin(), navigationScores.findMax());
    if (nextNavLevel > navigation + 2)
        navigation = navigation + 2;
    else
        navigation = nextNavLevel;
    navigation = Util::clamp(navigation, 0, getNavLimit());
}

void PlayerLevel::calculateSpeed()
{
    int nmin;
    int nmax;
    int noptimal;
    
    int lowerbound = minSpeed;
    int upperbound = maxSpeed;
    speedScores.setMinOptMax(nmin, noptimal, nmax);
    minSpeed = Util::clamp(nmin, lowerbound, upperbound);
    averageSpeed = Util::clamp(noptimal, lowerbound, upperbound);
    maxSpeed = Util::clamp(nmax, lowerbound, upperbound);
    
    std::cout << "New Min: " << nmin << std::endl;
    std::cout << "New Opt: " << noptimal << std::endl;
    std::cout << "New Max: " << nmax << std::endl;
}

std::string PlayerLevel::getCurrentStats() const
{
    return
    "Color/Sound " + Util::toStringInt(set1) + "\n" +
    "Shape/Sound " + Util::toStringInt(set2) + "\n" +
    " Sound Only " + Util::toStringInt(set3) + "\n" +
    " Navigation " + Util::toStringInt(navigation) + "\n" +
    " Time Speed " + Util::toStringInt(averageSpeed) + "\n" +
    "  Max Speed " + Util::toStringInt(maxSpeed) + "\n" +
    " Run Speed1 " + Util::toStringInt(runSpeed1) + "\n" +
    " Run Speed2 " + Util::toStringInt(runSpeed2) + "\n" +
    " Run Speed3 " + Util::toStringInt(runSpeed3);
}
