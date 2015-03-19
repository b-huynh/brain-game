//
//  EngineStudySettings.cpp
//  Vinezors2.0
//
//  Created by BrainGameCenter on 3/3/15.
//
//

#include "EngineStudySettings.h"
#include "EngineStateManager.h"
#include "Player.h"

extern Util::ConfigGlobal globals;

EngineStudySettings::EngineStudySettings(EngineStateManager* engineStateMgr, Player* player)
: Engine(engineStateMgr)
{
    this->player = player;
    this->hud = NULL;
}

EngineStudySettings::~EngineStudySettings()
{
}

void EngineStudySettings::enter()
{
    
    alloc();
    player->startMenu();
    
    
    //TempStudySettings
    tempfuelEnabled = player->fuelEnabled;
    tempholdoutEnabled = player->holdoutEnabled;
    tempinitialVelocity = player->initialVelocity;
    tempmanRecessEnabled = player->manRecessEnabled;
    tempmanRecessLevelLimit = player->manRecessLevelLimit;
    tempnewNavEnabled = player->newNavEnabled;
    tempnewNavIncrement = player->newNavIncrement;
    tempindRecessEnabled = player->indRecessEnabled;
    tempindRecessIncrement = player->indRecessIncrement;
    tempholdoutdelayEnabled = player->holdoutdelayEnabled;
    tempholdoutdelayNumber = player->holdoutdelayNumber;
    tempenableSettingsPasscode= player->enableSettingsPasscode;
    tempsessionStartTime = player->sessionStartTime;
    tempsessionEndTime = player->sessionEndTime;
    tempnumOfSessions = player->numOfSessions;
    tempnewSounds= globals.newSounds;
    tempsessionScreenEnabled = player->sessionScreenEnabled;
    
    // Set skybox
    OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
    OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
    
}

void EngineStudySettings::exit()
{
    dealloc();
}

void EngineStudySettings::update(float elapsed)
{
    hud->update(elapsed);
    
    player->updateMusicVolume();
}

void EngineStudySettings::activatePerformSingleTap(float x, float y)
{
    std::string queryGUI = hud->queryButtons(Vector2(x, y));
    if (queryGUI != "")
        player->reactGUI();
    if (queryGUI == "back")
    {
        player->saveProgress(globals.savePath);
        engineStateMgr->requestPopEngine();
        
        // Check if anything changes
        if(tempfuelEnabled == player->fuelEnabled &&
           tempholdoutEnabled == player->holdoutEnabled &&
           tempinitialVelocity == player->initialVelocity &&
           tempmanRecessEnabled == player->manRecessEnabled &&
           tempmanRecessLevelLimit == player->manRecessLevelLimit &&
           tempnewNavEnabled == player->newNavEnabled &&
           tempnewNavIncrement == player->newNavIncrement &&
           tempindRecessEnabled == player->indRecessEnabled &&
           tempindRecessIncrement == player->indRecessIncrement &&
           tempholdoutdelayEnabled == player->holdoutdelayEnabled &&
           tempholdoutdelayNumber == player->holdoutdelayNumber &&
           tempenableSettingsPasscode== player->enableSettingsPasscode &&
           tempsessionStartTime == player->sessionStartTime &&
           tempsessionEndTime == player->sessionEndTime &&
           tempnumOfSessions == player->numOfSessions &&
           tempnewSounds== globals.newSounds &&
           tempsessionScreenEnabled == player->sessionScreenEnabled)
        {
            //Still the same
            std::cout << "Nothing Changed!" <<std::endl;
        }
        else
        {
            //Something Changed
            std::cout << "Something Changed!" << std::endl;
            player->sessionStarted = false;

        }
           
           
        
    }
    else if (queryGUI == "checksessionid")
    {
        //Fix input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = false;
        hud->showDecimal = false;
        hud->nStatus = hud->NONE;
        
        if(player->sessionScreenEnabled)
        {
            player->sessionScreenEnabled = false;
        }
        else
        {
            player->sessionScreenEnabled = true;
        }
    }
    else if (queryGUI == "checkfuel")
    {
        //Fix input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
           hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = false;
        hud->showDecimal = false;
        hud->nStatus = hud->NONE;
        
        if(player->fuelEnabled)
        {
            player->fuelEnabled = false;
        }
        else
        {
            player->fuelEnabled = true;
        }
    }
    else if (queryGUI == "checksettingspasscode")
    {
        //Fix input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = false;
        hud->showDecimal = false;
        hud->nStatus = hud->NONE;
        
        if(player->enableSettingsPasscode)
        {
            player->enableSettingsPasscode = false;
        }
        else
        {
            player->enableSettingsPasscode = true;
        }
    }
    else if (queryGUI == "checknewsounds")
    {
        //Fix input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = false;
        hud->showDecimal = false;
        hud->nStatus = hud->NONE;
        
        if(globals.newSounds)
        {
            globals.newSounds = false;
        }
        else
        {
            globals.newSounds = true;
        }
    }
    else if (queryGUI == "checkholdout")
    {
        //Fix input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = false;
        hud->showDecimal = false;
        hud->nStatus = hud->NONE;
        
        
        if(player->holdoutEnabled)
        {
            player->holdoutEnabled = false;
        }
        else
        {
            player->holdoutEnabled = true;
        }
    }
    else if (queryGUI == "checkholdoutdelay")
    {
        //Fix input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = false;
        hud->showDecimal = false;
        hud->nStatus = hud->NONE;
        
        
        if(player->holdoutdelayEnabled)
        {
            player->holdoutdelayEnabled = false;
        }
        else
        {
            player->holdoutdelayEnabled = true;
        }
    }
    else if (queryGUI == "sessionstarttime")
    {
        //Fix OTHER input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = true;
        hud->showDecimal = false;
        hud->nStatus = hud->SESSION_START_TIME;
        hud->sessionStartTimeString = "";

    }
    else if (queryGUI == "sessionendtime")
    {
        //Fix OTHER input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = true;
        hud->showDecimal = false;
        hud->nStatus = hud->SESSION_END_TIME;
        hud->sessionEndtimeString = "";
        
    }
    else if (queryGUI == "sessionnum")
    {
        //Fix OTHER input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        hud->enableNumpad = true;
        hud->showDecimal = false;
        hud->nStatus = hud->NUM_OF_SESSIONS;
        hud->numOfSessionsString = "";
        
    }

    else if (queryGUI == "checkholdoutdelaynumber")
    {
        //Fix OTHER input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        if(player->holdoutdelayEnabled)
        {
            //std::cout<<"IND RECESS NUMBER" << std::endl;
            hud->enableNumpad = true;
            hud->showDecimal = true;
            hud->nStatus = hud->HOLDOUT_DELAY;
            hud->holdoutDelayString = "";
            
        }
    }
    else if (queryGUI == "checknewnav")
    {
        //Fix input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = false;
        hud->showDecimal = false;
        hud->nStatus = hud->NONE;
        
        
        if(player->newNavEnabled)
        {
            player->newNavEnabled = false;
        }
        else
        {
            player->newNavEnabled = true;
        }
    }
    else if(queryGUI == "checknewnavnumber")
    {
        //Fix OTHER input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        if(player->newNavEnabled)
        {
            std::cout<<"New_Nav Number" << std::endl;
            hud->enableNumpad = true;
            hud->showDecimal = true;
            hud->nStatus = hud->NEW_NAV_INC;
            hud->newNavigationIncAmountString = "";
            
        }
    }
    else if (queryGUI == "checkindrecess")
    {
        //Fix input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = false;
        hud->showDecimal = false;
        hud->nStatus = hud->NONE;
        
        
        if(player->indRecessEnabled)
        {
            player->indRecessEnabled = false;
        }
        else
        {
            player->indRecessEnabled = true;
        }
    }
    else if(queryGUI == "checkindrecessnumber")
    {
        //Fix OTHER input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        if(player->indRecessEnabled)
        {
            std::cout<<"IND RECESS NUMBER" << std::endl;
            hud->enableNumpad = true;
            hud->showDecimal = true;
            hud->nStatus = hud->IND_RECESS_INC;
            hud->indRecessString = "";
            
        }
    }

    else if (queryGUI == "checkinitspeed")
    {
        //Fix OTHER input fields
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        //player->initialVelocity = 20;
        hud->enableNumpad = true;
        hud->showDecimal = false;
        hud->nStatus = hud->INIT_VELOCITY;
        hud->initSpeedString = "";
    }
    else if (queryGUI == "checkmandatoryrecess")
    {
        //Fix input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        hud->enableNumpad = false;
        hud->showDecimal = false;
        hud->nStatus = hud->NONE;
        
        if(player->manRecessEnabled)
        {
            player->manRecessEnabled = false;
        }
        else
        {
            player->manRecessEnabled= true;
        }
    }
    else if (queryGUI == "mandatoryrecessnumber")
    {
        //Fix OTHER input fields
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            hud->initSpeedString =Util::toStringInt(player->initialVelocity);
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
        }
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
        }
        //player->initialVelocity = 20;
        if(player->manRecessEnabled)
        {
            std::cout<<"Man_recess_number" << std::endl;
            hud->enableNumpad = true;
            hud->showDecimal = false;
            hud->nStatus = hud->MAN_RECESS;
            hud->manRecessString = "";
        }
    }
    else if(queryGUI == "numpadbuttonback")
    {
        //make numpad go away
        std::cout<<"back pressed";
        //hud->enableNumpad = false;
        
        //Initial Velocity Update
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() != 0)
            {
                hud->initSpeedString.erase(hud->initSpeedString.size()-1,1);
            }
        }
        //ManRecess Update
        if( hud->nStatus == hud->MAN_RECESS )
        {
            if(hud->manRecessString.size() != 0)
            {
                hud->manRecessString.erase(hud->manRecessString.size()-1,1);
            }
        }
        //NewNav Update
        if( hud->nStatus == hud->NEW_NAV_INC )
        {
            if(hud->newNavigationIncAmountString.size() != 0)
            {
                hud->newNavigationIncAmountString.erase(hud->newNavigationIncAmountString.size()-1,1);
            }
        }
        //IndRecess Update
        if( hud->nStatus == hud->IND_RECESS_INC )
        {
            if(hud->indRecessString.size() != 0)
            {
                hud->indRecessString.erase(hud->indRecessString.size()-1,1);
            }
        }
        //DelayHoldout Update
        if( hud->nStatus == hud->HOLDOUT_DELAY )
        {
            if(hud->holdoutDelayString.size() != 0)
            {
                hud->holdoutDelayString.erase(hud->holdoutDelayString.size()-1,1);
            }
        }
        //SessionStartTime Update
        if( hud->nStatus == hud->SESSION_START_TIME )
        {
            if(hud->sessionStartTimeString.size() != 0)
            {
                hud->sessionStartTimeString.erase(hud->sessionStartTimeString.size()-1,1);
            }
        }
        //SessionEndTime Update
        if( hud->nStatus == hud->SESSION_END_TIME )
        {
            if(hud->sessionEndtimeString.size() != 0)
            {
                hud->sessionEndtimeString.erase(hud->sessionEndtimeString.size()-1,1);
            }
        }
        //SessionNum Update
        if( hud->nStatus == hud->NUM_OF_SESSIONS )
        {
            if(hud->numOfSessionsString.size() != 0)
            {
                hud->numOfSessionsString.erase(hud->numOfSessionsString.size()-1,1);
            }
        }
        
        
    }
    else if(queryGUI == "numpadbutton0")
    {
        std::cout<<"Pressed 0\n";
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "0";
            }
        }
        if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "0";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "0";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "0";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "0";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "0";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "0";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "0";
            }
        }
        
        
    }
    else if(queryGUI == "numpadbutton1")
    {
        std::cout<<"Pressed 1\n";
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "1";
            }
        }
        if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "1";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "1";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "1";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "1";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "1";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "1";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "1";
            }
        }
    }
    else if(queryGUI == "numpadbutton2")
    {
        std::cout<<"Pressed 2\n";
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "2";
            }
        }if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "2";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "2";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "2";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "2";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "2";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "2";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "2";
            }
        }
    }
    else if(queryGUI == "numpadbutton3")
    {
        std::cout<<"Pressed 3\n";
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "3";
            }
        }
        if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "3";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "3";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "3";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "3";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "3";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "3";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "3";
            }
        }
        
    }
    else if(queryGUI == "numpadbutton4")
    {
        std::cout<<"Pressed 4\n";
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "4";
            }
        }
        if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "4";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "4";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "4";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "4";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "4";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "4";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "4";
            }
        }
        
    }
    else if(queryGUI == "numpadbutton5")
    {
        std::cout<<"Pressed 5\n";
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "5";
            }
        }
        if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "5";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "5";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "5";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "5";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "5";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "5";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "5";
            }
        }
        
        
    }
    else if(queryGUI == "numpadbutton6")
    {
        std::cout<<"Pressed 6\n";
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "6";
            }
        }
        if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "6";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "6";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "6";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "6";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "6";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "6";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "6";
            }
        }
        
    }
    else if(queryGUI == "numpadbutton7")
    {
        std::cout<<"Pressed 7\n";
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "7";
            }
        }
        if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "7";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "7";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "7";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "7";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "7";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "7";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "7";
            }
        }
    }
    else if(queryGUI == "numpadbutton8")
    {
        std::cout<<"Pressed 8\n";
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "8";
            }
        }
        if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "8";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "8";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "8";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "8";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "8";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "8";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "8";
            }
        }
        
    }
    else if(queryGUI == "numpadbutton9")
    {
        std::cout<<"Pressed 9\n"; 
        if( hud->nStatus == hud->INIT_VELOCITY)
        {
            if(hud->initSpeedString.size() < 2)
            {
                hud->initSpeedString += "9";
            }
        }
        if( hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString.size() < 1)
            {
                hud->manRecessString+= "9";
            }
        }
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= "9";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= "9";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= "9";
            }
        }
        if( hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString.size() < 2)
            {
                hud->sessionStartTimeString+= "9";
            }
        }
        if( hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString.size() < 2)
            {
                hud->sessionEndtimeString+= "9";
            }
        }
        if( hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString.size() < 2)
            {
                hud->numOfSessionsString+= "9";
            }
        }
        
    }
    else if(queryGUI == "numpadbuttonsave" )
    {
        std::cout<<"SAVE"<<std::endl;
        if(hud->nStatus == hud->INIT_VELOCITY)
        {
            //std::cout << hud->initSpeedString[0] << std::endl;
            if(hud->initSpeedString != "")
            {
            
                player->initialVelocity = std::atoi(hud->initSpeedString.c_str());
                if(player->initialVelocity > globals.maxCamSpeed) //What should I use!
                {
                    player->initialVelocity = globals.maxCamSpeed;
                }
                if(player->initialVelocity < 10) //What do I use!
                {
                    player->initialVelocity = 10;
                }
            }
            hud->initSpeedString = Util::toStringInt(player->initialVelocity);
            hud->enableNumpad = false;
            
        }
        if(hud->nStatus == hud->MAN_RECESS)
        {
            if(hud->manRecessString != "")
            {
                
                player->manRecessLevelLimit= std::atoi(hud->manRecessString.c_str());
                if(player->manRecessLevelLimit < 1) //What do I use!
                {
                    player->manRecessLevelLimit = 1;
                }
            }
            hud->manRecessString = Util::toStringInt(player->manRecessLevelLimit);
            hud->enableNumpad = false;
            
        }
        if(hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString != "")
            {
                
                player->newNavIncrement= std::atof(hud->newNavigationIncAmountString.c_str());
                if(player->newNavIncrement < 0.00f) //What do I use!
                {
                    player->newNavIncrement = 0.00f;
                }
                if(player->newNavIncrement > 1.0f) //What do I use!
                {
                    player->newNavIncrement = 1.0f;
                }
                
            }
            hud->newNavigationIncAmountString = Util::toStringFloat(player->newNavIncrement,2);
            hud->enableNumpad = false;
            
        }
        
        if(hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString != "")
            {
                
                player->indRecessIncrement= std::atof(hud->indRecessString.c_str());
                if(player->indRecessIncrement < 0.00f) //What do I use!
                {
                    player->indRecessIncrement = 0.00f;
                }
                if(player->indRecessIncrement > 1.0f) //What do I use!
                {
                    player->indRecessIncrement = 1.0f;
                }
                
            }
            hud->indRecessString = Util::toStringFloat(player->indRecessIncrement,2);
            hud->enableNumpad = false;
            
        }
        if(hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString != "")
            {
                
                player->holdoutdelayNumber= std::atof(hud->holdoutDelayString.c_str());
                if(player->holdoutdelayNumber < 0.00f) //What do I use!
                {
                    player->indRecessIncrement = 0.00f;
                }
                if(player->holdoutdelayNumber > 99.9f) //What do I use!
                {
                    player->holdoutdelayNumber = 99.9f;
                }
                
            }
            hud->holdoutDelayString = Util::toStringFloat(player->holdoutdelayNumber,1);
            hud->enableNumpad = false;
            
        }
        if(hud->nStatus == hud->SESSION_START_TIME)
        {
            if(hud->sessionStartTimeString != "")
            {
                
                player->sessionStartTime= std::atoi(hud->sessionStartTimeString.c_str());
                if(player->sessionStartTime < 1) //What do I use!
                {
                    player->sessionStartTime = 1;
                }
                if(player->sessionStartTime > 99) //What do I use!
                {
                    player->sessionStartTime = 99;
                }
                
            }
            
            hud->sessionStartTimeString = Util::toStringInt(player->sessionStartTime);
            hud->enableNumpad = false;
            
        }
        if(hud->nStatus == hud->SESSION_END_TIME)
        {
            if(hud->sessionEndtimeString != "")
            {
                
                player->sessionEndTime= std::atoi(hud->sessionEndtimeString.c_str());
                if(player->sessionEndTime < 1) //What do I use!
                {
                    player->sessionEndTime = 1;
                }
                if(player->sessionEndTime > 99) //What do I use!
                {
                    player->sessionEndTime = 99;
                }
                
            }
            hud->sessionEndtimeString = Util::toStringInt(player->sessionEndTime);
            hud->enableNumpad = false;
            
        }
        if(hud->nStatus == hud->NUM_OF_SESSIONS)
        {
            if(hud->numOfSessionsString != "")
            {
                
                player->numOfSessions= std::atoi(hud->numOfSessionsString.c_str());
                if(player->numOfSessions < 1) //What do I use!
                {
                    player->numOfSessions = 1;
                }
                if(player->numOfSessions > 99) //What do I use!
                {
                    player->numOfSessions = 99;
                }
                
            }
            hud->numOfSessionsString = Util::toStringInt(player->numOfSessions);
            hud->enableNumpad = false;
            
        }

    }
    else if(queryGUI == "numpadbuttondecimal")
    {
        std::cout<<"Pressed Decimal"<<std::endl;
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 3)
            {
                hud->newNavigationIncAmountString+= ".";
            }
        }
        if( hud->nStatus == hud->IND_RECESS_INC)
        {
            if(hud->indRecessString.size() < 3)
            {
                hud->indRecessString+= ".";
            }
        }
        if( hud->nStatus == hud->HOLDOUT_DELAY)
        {
            if(hud->holdoutDelayString.size() < 3)
            {
                hud->holdoutDelayString+= ".";
            }
        }
    }
    else if(queryGUI == "numpad")
    {
        
    }
    
    else
    {
        //std::cout<<"Pressed something else "<<std::endl;
        //Close without Saving!
        if(hud->enableNumpad)
        {
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(player->initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(player->manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(player->newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(player->indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(player->holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(player->sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(player->sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(player->numOfSessions);
            }
            hud->enableNumpad = false;
            hud->showDecimal = false;
        }
        
        
        
        
    }

}

// The following deal with injecting coordinates to simulate a slider
//
void EngineStudySettings::activateMoved(float x, float y, float dx, float dy)
{
    if (hud)
    {
        
#ifdef DEBUG_MODE
        
#endif
    }
}

void EngineStudySettings::activatePressed(float x, float y)
{
    if (hud)
    {
        
#ifdef DEBUG_MODE
        
#endif
    }
}

void EngineStudySettings::activateReleased(float x, float y, float dx, float dy)
{
    if (hud)
    {
       
#ifdef DEBUG_MODE
        
#endif
    }
}

#if !defined(OGRE_IS_IOS)
void EngineStudySettings::mouseMoved(const OIS::MouseEvent &evt)
{
}

void EngineStudySettings::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            activatePerformSingleTap(evt.state.X.abs, evt.state.Y.abs);
            break;
        default:
            break;
    }
}

void EngineStudySettings::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    switch (id)
    {
        case OIS::MB_Left:
            break;
        default:
            break;
    }
}

void EngineStudySettings::keyPressed(const OIS::KeyEvent &keyEventRef)
{
}

void EngineStudySettings::keyReleased(const OIS::KeyEvent &keyEventRef)
{
}
#endif

void EngineStudySettings::requestResize()
{
    if (hud) hud->adjust();
}

void EngineStudySettings::alloc()
{
    hud = new HudStudySettings(player);
}

void EngineStudySettings::dealloc()
{
    if (hud)
    {
        delete hud;
        hud = NULL;
    }
}
