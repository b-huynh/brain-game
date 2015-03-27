//
//  EngineStateManager.cpp
//  Vinezors2.0
//
//  Created by Calvin Phung on 4/17/14.
//
//

#include "EngineStateManager.h"
#include "EngineStage.h"
#include "EngineSchedulerMenu.h"
#include "EngineLevelSelection.h"
#include "EngineMainMenu.h"
#include "EngineCredits.h"
#include "EngineMainSettings.h"
#include "EngineControlSettings.h"
#include "EngineStudySettings.h"
#include "EngineInformation.h"
#include <iostream>

EngineStateManager::EngineStateManager()
: gameEngineStack(), cleanup()
{
}

void EngineStateManager::update(float elapsed)
{
    for (int i = 0; i < cleanup.size(); ++i)
        delete cleanup[i];
    cleanup.clear();
}

Engine* EngineStateManager::peek(int depth)
{
    int ind = gameEngineStack.size() - depth - 1;
    if (ind < 0) return NULL;
    else return gameEngineStack[ind];
}

Engine* EngineStateManager::getActiveEngine() const
{
    return gameEngineStack.size() > 0 ? gameEngineStack[gameEngineStack.size() - 1] : NULL;
}

void EngineStateManager::requestPushEngine(EngineState engineType, Player* player)
{
    switch (engineType)
    {
        case ENGINE_STAGE:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            top = new EngineStage(this, player);
            gameEngineStack.push_back(top);
            top->enter();
            break;
        }
        case ENGINE_SCHEDULER_MENU:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            top = new EngineSchedulerMenu(this, player);
            gameEngineStack.push_back(top);
            top->enter();
            break;
        }
        case ENGINE_LEVEL_SELECTION:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            top = new EngineLevelSelection(this, player);
            gameEngineStack.push_back(top);
            top->enter();
            break;
        }
        case ENGINE_MAIN_MENU:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            top = new EngineMainMenu(this, player);
            gameEngineStack.push_back(top);
            top->enter();
            break;
        }
        case ENGINE_CREDITS:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            top = new EngineCredits(this, player);
            gameEngineStack.push_back(top);
            top->enter();
            break;
        }
        case ENGINE_MAIN_SETTINGS:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            top = new EngineMainSettings(this, player);
            gameEngineStack.push_back(top);
            top->enter();
            break;
        }
        case ENGINE_CONTROL_SETTINGS:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            top = new EngineControlSettings(this, player);
            gameEngineStack.push_back(top);
            top->enter();
            break;
        }
        case ENGINE_STUDY_SETTINGS:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            top = new EngineStudySettings(this, player);
            gameEngineStack.push_back(top);
            top->enter();
            break;
        }
        case ENGINE_INFORMATION:
        {
            Engine* top = getActiveEngine();
            if (top) top->exit();
            top = new EngineInformation(this, player);
            gameEngineStack.push_back(top);
            top->enter();
            break;
        }
        default:
            std::cout << "WARNING: Unknown engine type push request!\n";
            break;
    }
}

void EngineStateManager::requestPopEngine()
{
    if (gameEngineStack.size() > 0)
    {
        Engine* top = getActiveEngine();
        if (top)
        {
            top->exit();
            cleanup.push_back(top);
            gameEngineStack.pop_back();
            top = getActiveEngine();
            if (top) top->enter();
        }
    }
    else
        std::cout << "WARNING: No more engines to pop!\n";
}
