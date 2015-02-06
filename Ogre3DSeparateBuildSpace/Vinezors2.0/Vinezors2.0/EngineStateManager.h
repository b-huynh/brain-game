//
//  EngineStateManager.h
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#ifndef __Vinezors2_0__EngineStateManager__
#define __Vinezors2_0__EngineStateManager__

#include "Engine.h"
#include "Player.h" // Player currently acts as the global message object between game states
#include <vector>

// Maintains a stack of Game States. The state at the top of the stack is the currently active
class EngineStateManager
{
public:
    EngineStateManager();
    
    void update(float elapsed);
    
    Engine* peek(int depth = 0); // depth of 0 is the top of stack
    Engine* getActiveEngine() const;
    
    void requestPushEngine(EngineState engineType, Player* player);
    void requestPopEngine();
private:
    std::vector<Engine*> gameEngineStack;
    std::vector<Engine*> cleanup;
};

#endif /* defined(__Vinezors2_0__EngineStateManager__) */
