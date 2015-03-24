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
    tempfuelEnabled = globals.fuelEnabled;
    tempholdoutEnabled = globals.holdoutEnabled;
    tempinitialVelocity = globals.initialVelocity;
    tempmanRecessEnabled = globals.manRecessEnabled;
    tempmanRecessLevelLimit = globals.manRecessLevelLimit;
    tempnewNavEnabled = globals.newNavEnabled;
    tempnewNavIncrement = globals.newNavIncrement;
    tempindRecessEnabled = globals.indRecessEnabled;
    tempindRecessIncrement = globals.indRecessIncrement;
    tempholdoutdelayEnabled = globals.holdoutdelayEnabled;
    tempholdoutdelayNumber = globals.holdoutdelayNumber;
    tempenableSettingsPasscode= globals.enableSettingsPasscode;
    tempsessionStartTime = globals.sessionStartTime;
    tempsessionEndTime = globals.sessionEndTime;
    tempnumOfSessions = globals.numOfSessions;
    tempnewSounds= globals.newSounds;
    tempenableIndRecessFixed = globals.enableIndRecessFixed;
    tempholdoutoffsetA = player->scheduler->holdoutOffsetA;
    tempholdoutoffsetB = player->scheduler->holdoutOffsetB;
    tempholdoutoffsetD = player->scheduler->holdoutOffsetD;
    tempsessionScreenEnabled = globals.sessionScreenEnabled;
    
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
    {
        if(!hud->popUpisOut)
        {
            player->reactGUI();

        }

    }
    if (queryGUI == "popupok")
    {
        if(hud->popUpisOut)
        {
            player->reactGUI();
            std::cout<<"Pressed Continue" << std::endl;
            
            if(tempmanRecessEnabled != globals.manRecessEnabled)
            {
                //If they are different then we changed from to another so we
                //should restart the counter
                globals.manRecessCount = 0;
            }
            if(tempholdoutdelayEnabled && !globals.holdoutdelayEnabled)
            {
                player->scheduler->holdoutOffsetA = 0;
                player->scheduler->holdoutOffsetB = 0;
                player->scheduler->holdoutOffsetD = 0;
            }
            /* Need to do this when I press ok!*/
             player->feedLevelRequestFromSchedule();
             player->sessionStarted = false;
             globals.saveGlobalSettings(globals.globalPath);
             player->saveProgress(globals.savePath);
             engineStateMgr->requestPopEngine();
            
        }
    }
    else if (queryGUI == "popuprevert")
    {
        if(hud->popUpisOut)
        {
            player->reactGUI();
            std::cout<<"Pressed Revert "<< std::endl;
            //Put all temp settings back
            //TempStudySettings
            
            globals.fuelEnabled = tempfuelEnabled;
            globals.holdoutEnabled =tempholdoutEnabled;
            globals.initialVelocity=tempinitialVelocity;
            globals.manRecessEnabled=tempmanRecessEnabled;
            globals.manRecessLevelLimit=tempmanRecessLevelLimit;
            globals.newNavEnabled=tempnewNavEnabled;
            globals.newNavIncrement=tempnewNavIncrement;
            globals.indRecessEnabled=tempindRecessEnabled;
            globals.indRecessIncrement=tempindRecessIncrement;
            globals.holdoutdelayEnabled=tempholdoutdelayEnabled;
            globals.holdoutdelayNumber=tempholdoutdelayNumber;
            globals.enableSettingsPasscode=tempenableSettingsPasscode;
            globals.sessionStartTime=tempsessionStartTime;
            globals.sessionEndTime=tempsessionEndTime;
            globals.numOfSessions=tempnumOfSessions;
            globals.newSounds=tempnewSounds;
            globals.enableIndRecessFixed=tempenableIndRecessFixed;
            player->scheduler->holdoutOffsetA=tempholdoutoffsetA;
            player->scheduler->holdoutOffsetB=tempholdoutoffsetB;
            player->scheduler->holdoutOffsetD=tempholdoutoffsetD;
            globals.sessionScreenEnabled=tempsessionScreenEnabled;
            /* Need to do this when I press ok!*/
             
             
            globals.saveGlobalSettings(globals.globalPath);
            player->saveProgress(globals.savePath);
            hud->initStrings();
            hud->popUpisOut = false;
            hud->hidePopUp();
            
        }
    }
    else if (queryGUI == "back")
    {
        if(!hud->popUpisOut)
        {

            // Check if anything changes
            if(tempfuelEnabled == globals.fuelEnabled &&
               tempholdoutEnabled == globals.holdoutEnabled &&
               tempinitialVelocity == globals.initialVelocity &&
               tempmanRecessEnabled == globals.manRecessEnabled &&
               tempmanRecessLevelLimit == globals.manRecessLevelLimit &&
               tempnewNavEnabled == globals.newNavEnabled &&
               tempnewNavIncrement == globals.newNavIncrement &&
               tempindRecessEnabled == globals.indRecessEnabled &&
               tempindRecessIncrement == globals.indRecessIncrement &&
               tempholdoutdelayEnabled == globals.holdoutdelayEnabled &&
               tempholdoutdelayNumber == globals.holdoutdelayNumber &&
               tempenableSettingsPasscode== globals.enableSettingsPasscode &&
               tempsessionStartTime == globals.sessionStartTime &&
               tempsessionEndTime == globals.sessionEndTime &&
               tempnumOfSessions == globals.numOfSessions &&
               tempnewSounds== globals.newSounds &&
               tempsessionScreenEnabled == globals.sessionScreenEnabled &&
               tempenableIndRecessFixed == globals.enableIndRecessFixed)
                
            {
                //Still the same

                std::cout << "Nothing Changed!" <<std::endl;
                globals.saveGlobalSettings(globals.globalPath);
                player->saveProgress(globals.savePath);
                std::cout<<player->scheduler->holdoutOffsetA<<std::endl;
                engineStateMgr->requestPopEngine();
            }
            else
            {
                
                //Something Changed
                std::cout << "Something Changed!" << std::endl;
                //Show PopUp
                hud->showPopUp();
                hud->popUpisOut = true;
                //DisableButton
                
                /* Need to do this when I press ok!
                 player->feedLevelRequestFromSchedule();
                 player->sessionStarted = false;*/
                
            }
            
            
        }
        
    }
    else if (queryGUI == "checksessionid")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = false;
            hud->showDecimal = false;
            hud->nStatus = hud->NONE;
            
            if(globals.sessionScreenEnabled)
            {
                globals.sessionScreenEnabled = false;
            }
            else
            {
                globals.sessionScreenEnabled = true;
            }
   
        }
        
    }
    else if (queryGUI == "checkfuel")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = false;
            hud->showDecimal = false;
            hud->nStatus = hud->NONE;
            
            if(globals.fuelEnabled)
            {
                globals.fuelEnabled = false;
            }
            else
            {
                globals.fuelEnabled = true;
            }

        }
        
    }
    else if (queryGUI == "checksettingspasscode")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = true;
            hud->showDecimal = false;
            hud->nStatus = hud->PASSCODE;
            
            
            
            //Show Passcode option, check if password inputted
            
            /*if(player->enableSettingsPasscode)
            {
                player->enableSettingsPasscode = false;
            }
            else
            {
                player->enableSettingsPasscode = true;
            }*/

        }
    }
    else if (queryGUI == "checknewsounds")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
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

    }
    else if (queryGUI == "checkholdout")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = false;
            hud->showDecimal = false;
            hud->nStatus = hud->NONE;
            
            
            if(globals.holdoutEnabled)
            {
                globals.holdoutEnabled = false;
            }
            else
            {
                globals.holdoutEnabled = true;
                globals.holdoutdelayEnabled = false;
            }
        }
        
    }
    else if (queryGUI == "checkholdoutdelay")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = false;
            hud->showDecimal = false;
            hud->nStatus = hud->NONE;
            
            if(globals.holdoutEnabled)
            {
                if(globals.holdoutdelayEnabled)
                {
                    globals.holdoutdelayEnabled = false;
                    //If holdoutdelay is disabled then we must revert to the old holdout offset.
                    player->scheduler->holdoutOffsetA = tempholdoutoffsetA;
                    player->scheduler->holdoutOffsetB = tempholdoutoffsetB;
                    player->scheduler->holdoutOffsetD = tempholdoutoffsetD;
                }
                else
                {
                    globals.holdoutdelayEnabled = true;
                    //If holdoutdelay is enabled then we use the new offset
                    //Update the holdout offset
                    if(globals.holdoutdelayNumber == 1)
                    {
                        player->scheduler->holdoutOffsetA = 0;
                        player->scheduler->holdoutOffsetB = 0;
                        player->scheduler->holdoutOffsetD = 0;
                        
                        
                    }
                    else
                    {
                        player->scheduler->holdoutOffsetA = -(globals.holdoutdelayNumber-1);
                        player->scheduler->holdoutOffsetB = -(globals.holdoutdelayNumber-1);
                        player->scheduler->holdoutOffsetD = -(globals.holdoutdelayNumber-1);
                        
                        
                    }
                    
                }
            }
        }
        
        
    }
    else if (queryGUI == "sessionstarttime")
    {
        if(!hud->popUpisOut)
        {
            //Fix OTHER input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = true;
            hud->showDecimal = false;
            hud->nStatus = hud->SESSION_START_TIME;
            hud->sessionStartTimeString = "";
        }
        

    }
    else if (queryGUI == "sessionendtime")
    {
        if(!hud->popUpisOut)
        {
            //Fix OTHER input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = true;
            hud->showDecimal = false;
            hud->nStatus = hud->SESSION_END_TIME;
            hud->sessionEndtimeString = "";
        }
        
        
    }
    else if (queryGUI == "sessionnum")
    {
        if(!hud->popUpisOut)
        {
            //Fix OTHER input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            hud->enableNumpad = true;
            hud->showDecimal = false;
            hud->nStatus = hud->NUM_OF_SESSIONS;
            hud->numOfSessionsString = "";
        }
        
        
    }

    else if (queryGUI == "checkholdoutdelaynumber")
    {
        if(!hud->popUpisOut)
        {
            //Fix OTHER input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            
            if( globals.holdoutEnabled && globals.holdoutdelayEnabled)
            {
                //std::cout<<"IND RECESS NUMBER" << std::endl;
                hud->enableNumpad = true;
                hud->showDecimal = true;
                hud->nStatus = hud->HOLDOUT_DELAY;
                hud->holdoutDelayString = "";
                
            }
        }
        
    }
    else if (queryGUI == "checknewnav")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = false;
            hud->showDecimal = false;
            hud->nStatus = hud->NONE;
            
            
            if(globals.newNavEnabled)
            {
                globals.newNavEnabled = false;
            }
            else
            {
                globals.newNavEnabled = true;
            }
        }
        
    }
    else if(queryGUI == "checknewnavnumber")
    {
        if(!hud->popUpisOut)
        {
            //Fix OTHER input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            if(globals.newNavEnabled)
            {
                std::cout<<"New_Nav Number" << std::endl;
                hud->enableNumpad = true;
                hud->showDecimal = true;
                hud->nStatus = hud->NEW_NAV_INC;
                hud->newNavigationIncAmountString = "";
                
            }
        }
        
    }
    else if (queryGUI == "checkindrecess")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = false;
            hud->showDecimal = false;
            hud->nStatus = hud->NONE;
            
            
            if(globals.indRecessEnabled)
            {
                globals.indRecessEnabled = false;
            }
            else
            {
                globals.indRecessEnabled = true;
            }
        }
        
    }
    else if (queryGUI == "checkindrecessfixed")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = false;
            hud->showDecimal = false;
            hud->nStatus = hud->NONE;
            std::cout << "Fixed Pressed" << std::endl;
            
            if(globals.indRecessEnabled){
                if(globals.enableIndRecessFixed)
                {
                    globals.enableIndRecessFixed = false;
                }
                else
                {
                    globals.enableIndRecessFixed = true;
                }
            }
        }
        
    }
    else if(queryGUI == "checkindrecessnumber")
    {
        if(!hud->popUpisOut)
        {
            //Fix OTHER input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            if(globals.indRecessEnabled)
            {
                std::cout<<"IND RECESS NUMBER" << std::endl;
                hud->enableNumpad = true;
                hud->showDecimal = true;
                hud->nStatus = hud->IND_RECESS_INC;
                hud->indRecessString = "";
                
            }
        }
        
    }

    else if (queryGUI == "checkinitspeed")
    {
        if(!hud->popUpisOut)
        {
            //Fix OTHER input fields
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            //globals.initialVelocity = 20;
            hud->enableNumpad = true;
            hud->showDecimal = false;
            hud->nStatus = hud->INIT_VELOCITY;
            hud->initSpeedString = "";
        }
        
    }
    else if (queryGUI == "checkmandatoryrecess")
    {
        if(!hud->popUpisOut)
        {
            //Fix input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            hud->enableNumpad = false;
            hud->showDecimal = false;
            hud->nStatus = hud->NONE;
            
            if(globals.manRecessEnabled)
            {
                globals.manRecessEnabled = false;
            }
            else
            {
                globals.manRecessEnabled= true;
            }
        }
        
    }
    else if (queryGUI == "mandatoryrecessnumber")
    {
        if(!hud->popUpisOut)
        {
            //Fix OTHER input fields
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
            }
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
            }
            //globals.initialVelocity = 20;
            if(globals.manRecessEnabled)
            {
                std::cout<<"Man_recess_number" << std::endl;
                hud->enableNumpad = true;
                hud->showDecimal = false;
                hud->nStatus = hud->MAN_RECESS;
                hud->manRecessString = "";
            }
        }
        
    }
    else if(queryGUI == "numpadbuttonback")
    {
        if(!hud->popUpisOut)
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
        
        
        
    }
    else if(queryGUI == "numpadbutton0")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 0;
                    hud->Passcode_counter++;
                }
            }
        }
        
    }
    else if(queryGUI == "numpadbutton1")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 1;
                    if(hud->Passcode_counter == 3)
                    {
                        //engineStateMgr->requestPushEngine(ENGINE_MAIN_SETTINGS, player);
                        bool valid = true;
                        for(int i = 0; i < hud->PASSWORD_LENGTH; i++)
                        {
                            if(hud->user_password[i] != hud->PASSWORD[i])
                            {
                                valid = false;
                            }
                        }
                        
                        if(valid)
                        {
                            if(globals.enableSettingsPasscode)
                            {
                                globals.enableSettingsPasscode = false;
                            }
                            else
                            {
                                globals.enableSettingsPasscode = true;
                            }
                            hud->enableNumpad = false;
                            hud->nStatus = hud->NONE;
                            hud->Passcode_counter = 0;
                        }
                        else
                        {
                            hud->Passcode_counter++;
                        }
                        
                    }
                    else
                    {
                        hud->Passcode_counter++;
                    }
                }

            }
        }
        
    }
    else if(queryGUI == "numpadbutton2")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 2;
                    hud->Passcode_counter++;
                }
            }
        }
        
    }
    else if(queryGUI == "numpadbutton3")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 3;
                    hud->Passcode_counter++;
                }
            }
        }
        
        
    }
    else if(queryGUI == "numpadbutton4")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 4;
                    hud->Passcode_counter++;
                }
            }
        }
        
        
    }
    else if(queryGUI == "numpadbutton5")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 5;
                    hud->Passcode_counter++;
                }
            }
        }
        
        
        
    }
    else if(queryGUI == "numpadbutton6")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 6;
                    hud->Passcode_counter++;
                }
            }
        }
        
        
    }
    else if(queryGUI == "numpadbutton7")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 7;
                    hud->Passcode_counter++;
                }
            }
        }
        
    }
    else if(queryGUI == "numpadbutton8")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 8;
                    hud->Passcode_counter++;
                }
            }
        }
        
        
    }
    else if(queryGUI == "numpadbutton9")
    {
        if(!hud->popUpisOut)
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
            if(hud->nStatus == hud->PASSCODE)
            {
                if(hud->Passcode_counter < 4)
                {
                    hud->user_password[hud->Passcode_counter] = 9;
                    hud->Passcode_counter++;
                }
            }
        }
        
        
    }
    else if(queryGUI == "numpadbuttonsave" )
    {
        if(!hud->popUpisOut)
        {
            std::cout<<"SAVE"<<std::endl;
            if(hud->nStatus == hud->INIT_VELOCITY)
            {
                //std::cout << hud->initSpeedString[0] << std::endl;
                if(hud->initSpeedString != "")
                {
                    
                    globals.initialVelocity = std::atoi(hud->initSpeedString.c_str());
                    if(globals.initialVelocity > globals.maxCamSpeed) //What should I use!
                    {
                        globals.initialVelocity = globals.maxCamSpeed;
                    }
                    if(globals.initialVelocity < 10) //What do I use!
                    {
                        globals.initialVelocity = 10;
                    }
                }
                hud->initSpeedString = Util::toStringInt(globals.initialVelocity);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->MAN_RECESS)
            {
                if(hud->manRecessString != "")
                {
                    
                    globals.manRecessLevelLimit= std::atoi(hud->manRecessString.c_str());
                    if(globals.manRecessLevelLimit < 1) //What do I use!
                    {
                        globals.manRecessLevelLimit = 1;
                    }
                }
                hud->manRecessString = Util::toStringInt(globals.manRecessLevelLimit);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->NEW_NAV_INC)
            {
                if(hud->newNavigationIncAmountString != "")
                {
                    
                    globals.newNavIncrement= std::atof(hud->newNavigationIncAmountString.c_str());
                    if(globals.newNavIncrement < 0.01f) //What do I use!
                    {
                        globals.newNavIncrement = 0.01f;
                    }
                    if(globals.newNavIncrement > 1.0f) //What do I use!
                    {
                        globals.newNavIncrement = 1.0f;
                    }
                    
                }
                hud->newNavigationIncAmountString = Util::toStringFloat(globals.newNavIncrement,2);
                hud->enableNumpad = false;
                
            }
            
            if(hud->nStatus == hud->IND_RECESS_INC)
            {
                if(hud->indRecessString != "")
                {
                    
                    globals.indRecessIncrement= std::atof(hud->indRecessString.c_str());
                    if(globals.indRecessIncrement < 0.01f) //What do I use!
                    {
                        globals.indRecessIncrement = 0.01f;
                    }
                    if(globals.indRecessIncrement > 1.0f) //What do I use!
                    {
                        globals.indRecessIncrement = 1.0f;
                    }
                    
                }
                hud->indRecessString = Util::toStringFloat(globals.indRecessIncrement,2);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->HOLDOUT_DELAY)
            {
                if(hud->holdoutDelayString != "")
                {
                    
                    globals.holdoutdelayNumber= std::atof(hud->holdoutDelayString.c_str());
                    if(globals.holdoutdelayNumber < 1.00f) //What do I use!
                    {
                        globals.holdoutdelayNumber = 1.00f;
                    }
                    if(globals.holdoutdelayNumber > 99.9f) //What do I use!
                    {
                        globals.holdoutdelayNumber = 99.9f;
                    }
                    //Update the holdout offset
                    if(globals.holdoutdelayNumber == 1)
                    {
                        player->scheduler->holdoutOffsetA = 0;
                        player->scheduler->holdoutOffsetB = 0;
                        player->scheduler->holdoutOffsetD = 0;
                        
                        
                    }
                    else
                    {

                        player->scheduler->holdoutOffsetA = -(globals.holdoutdelayNumber-1);
                        player->scheduler->holdoutOffsetB = -(globals.holdoutdelayNumber-1);
                        player->scheduler->holdoutOffsetD = -(globals.holdoutdelayNumber-1);
                        
                        
                    }
                    
                }
                hud->holdoutDelayString = Util::toStringFloat(globals.holdoutdelayNumber,1);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->SESSION_START_TIME)
            {
                if(hud->sessionStartTimeString != "")
                {
                    
                    globals.sessionStartTime= std::atoi(hud->sessionStartTimeString.c_str());
                    if(globals.sessionStartTime < 1) //What do I use!
                    {
                        globals.sessionStartTime = 1;
                    }
                    if(globals.sessionStartTime > globals.sessionEndTime) //What do I use!
                    {
                        globals.sessionStartTime = globals.sessionEndTime;
                    }
                    
                }
                
                hud->sessionStartTimeString = Util::toStringInt(globals.sessionStartTime);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->SESSION_END_TIME)
            {
                if(hud->sessionEndtimeString != "")
                {
                    
                    globals.sessionEndTime= std::atoi(hud->sessionEndtimeString.c_str());
                    if(globals.sessionEndTime < 1) //What do I use!
                    {
                        globals.sessionEndTime = 1;
                    }
                    if(globals.sessionEndTime > 99) //What do I use!
                    {
                        globals.sessionEndTime = 99;
                    }
                    if(globals.sessionEndTime < globals.sessionStartTime)
                    {
                        globals.sessionStartTime = globals.sessionEndTime;
                        hud->sessionStartTimeString = Util::toStringInt(globals.sessionStartTime);

                    }
                    
                }
                hud->sessionEndtimeString = Util::toStringInt(globals.sessionEndTime);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->NUM_OF_SESSIONS)
            {
                if(hud->numOfSessionsString != "")
                {
                    
                    globals.numOfSessions= std::atoi(hud->numOfSessionsString.c_str());
                    if(globals.numOfSessions < 1) //What do I use!
                    {
                        globals.numOfSessions = 1;
                    }
                    if(globals.numOfSessions > 99) //What do I use!
                    {
                        globals.numOfSessions = 99;
                    }
                    
                }
                hud->numOfSessionsString = Util::toStringInt(globals.numOfSessions);
                hud->enableNumpad = false;
                
            }
        }
        

    }
    else if(queryGUI == "numpadbuttondecimal")
    {
        if(!hud->popUpisOut)
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
        
    }
    else if(queryGUI == "numpad")
    {
        
    }
    
    else
    {
        if(!hud->popUpisOut)
        {
            if(hud->enableNumpad)
            {
                if(hud->nStatus == hud->INIT_VELOCITY)
                {
                    hud->initSpeedString =Util::toStringInt(globals.initialVelocity);
                }
                if(hud->nStatus == hud->MAN_RECESS)
                {
                    hud->manRecessString =Util::toStringInt(globals.manRecessLevelLimit);
                }
                if(hud->nStatus == hud->NEW_NAV_INC)
                {
                    hud->newNavigationIncAmountString =Util::toStringFloat(globals.newNavIncrement,2);
                }
                if(hud->nStatus == hud->IND_RECESS_INC)
                {
                    hud->indRecessString =Util::toStringFloat(globals.indRecessIncrement,2);
                }
                if(hud->nStatus == hud->HOLDOUT_DELAY)
                {
                    hud->holdoutDelayString =Util::toStringFloat(globals.holdoutdelayNumber,1);
                }
                if(hud->nStatus == hud->SESSION_START_TIME)
                {
                    hud->sessionStartTimeString =Util::toStringInt(globals.sessionStartTime);
                }
                if(hud->nStatus == hud->SESSION_END_TIME)
                {
                    hud->sessionEndtimeString =Util::toStringInt(globals.sessionEndTime);
                }
                if(hud->nStatus == hud->NUM_OF_SESSIONS)
                {
                    hud->numOfSessionsString =Util::toStringInt(globals.numOfSessions);
                }
                hud->enableNumpad = false;
                hud->nStatus = hud->NONE;
                hud->showDecimal = false;
            }
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
