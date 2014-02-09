//
//  LevelManager.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 2/3/14.
//
//

#ifndef __Vinezors2_0__LevelManager__
#define __Vinezors2_0__LevelManager__

#include "Util.h"

enum GamePhase { PHASE_SET1, PHASE_SET2, PHASE_SET3, PHASE_NAVIGATION, PHASE_TIMED, PHASE_DONE };
class LevelManager
{
private:
    
    Player* player;
    
    std::string schedule;
    int schedIndex;
public:
    LevelManager(Player* player, std::string schedule);
    
    GamePhase getPhaseAt(int index) const;
    GamePhase getCurrentPhase() const;
    GamePhase getNextPhase() const;
    
    // Simple progressive model, good for debugging different stages
    void levelFinishedA(Tunnel* tunnel, Evaluation forced);
    Tunnel* getNextLevelA(Tunnel* previousTunnel);
    
    // Follows a special schedule of game modes
    void levelFinishedB(Tunnel* tunnel, Evaluation forced);
    Tunnel* getNextLevelB(Tunnel* previousTunnel);
    
};

#endif /* defined(__Vinezors2_0__LevelManager__) */
