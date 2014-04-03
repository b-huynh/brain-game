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

enum GamePhase { PHASE_SET1, PHASE_SET2, PHASE_SET3, PHASE_NAVIGATION, PHASE_TIMED, PHASE_TEACHING, PHASE_DONE, PHASE_RECESS, PHASE_SETSPECIAL };
class LevelManager
{
private:
    
    Player* player;
    
    std::string scheduleMainTemplate;
    std::string scheduleRepeatTemplate;
    
    std::string scheduleMain;
    std::string scheduleRepeat;
    std::string scheduleRepeatRandomPool;
    int schedIndexMain;
    int schedIndexRepeat;
    int schedCount;
public:
    LevelManager(Player* player, std::string scheduleMain, std::string scheduleRepeat, std::string scheduleRepeatRandomPool);
    
    GamePhase getPhaseAt(const std::string sched, int index) const;
    GamePhase getCurrentPhase() const;
    char getScheduleValue() const;
    int getSchedNo() const;
    bool isDoneWithMainSchedule() const;
    bool isRepeatingPhase() const;
    float extractTime(char c) const;
    float extractTotalTime() const;
    
    void resetRepeatingSchedule();
    void repeatPreviousPhase();
    void incrementSchedIndex();
    void decrementSchedIndex();
    
    // Follows a special schedule of game modes
    bool levelFinished(Tunnel* tunnel, Evaluation forced);
    void updatePlayerSkill(Tunnel* tunnel, Evaluation forced);
    Tunnel* getNextLevel(Tunnel* previousTunnel);
};

#endif /* defined(__Vinezors2_0__LevelManager__) */
