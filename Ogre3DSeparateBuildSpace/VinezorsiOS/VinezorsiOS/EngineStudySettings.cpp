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
    
    tempholdoutMinUpperBound = globals.holdoutMinUpperBound;
    tempholdoutMaxUpperBound = globals.holdoutMaxUpperBound;
    tempholdoutLowerBoundTime = globals.holdoutLowerBoundTime;
    tempholdoutUpperBoundMinTime = globals.holdoutUpperBoundMinTime;
    tempholdoutUpperBoundMaxTime = globals.holdoutUpperBoundMaxTime;
    tempholdoutSteps = globals.holdoutSteps;
    tempOverallTimerEnabled = globals.OverallTimerEnabled;
    tempaccelEnabled = globals.accelEnabled;
    
    // Set skybox
    OgreFramework::getSingletonPtr()->m_pSceneMgrMain->setSkyPlaneEnabled(false);
    OgreFramework::getSingletonPtr()->m_pViewportMain->setBackgroundColour(ColourValue(0.11f, 0.11f, 0.11f, 1.0f));
    OgreFramework::getSingletonPtr()->m_pCameraMain->setPosition(Vector3(0, 0, 50));
    OgreFramework::getSingletonPtr()->m_pCameraMain->lookAt(Vector3(0, 0, 0));
    if (OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode())
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getSkyPlaneNode()->resetOrientation();
    
}

void EngineStudySettings::exit()
{
    player->isNextPage = false;
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
        if( (!hud->enableNumpad && (queryGUI == "numpad")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton0")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton1")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton2")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton3")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton4")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton5")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton6")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton7")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton8")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbutton9")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbuttondecimal")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbuttonsave")) ||
            (!hud->enableNumpad && (queryGUI == "numpadbuttonback"))
           )
        {
            //Dont sound off
        }
        else if(!hud->popUpisOut)
        {
            player->reactGUI();

        }
        

    }
    if(queryGUI == "nextsettings")
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
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                hud->holdoutMinUpperBoundString =Util::toStringFloat(globals.holdoutMinUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                hud->holdoutMaxUpperBoundString =Util::toStringFloat(globals.holdoutMaxUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                hud->holdoutLowerBoundTimeString = Util::toStringFloat(globals.holdoutLowerBoundTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                hud->holdoutMinUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMinTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                hud->holdoutMaxUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMaxTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_STEPS)
            {
                hud->holdoutStepsString = Util::toStringInt(globals.holdoutSteps);
            }
            
            hud->enableNumpad = false;
            hud->showDecimal = false;
            hud->nStatus = hud->NONE;
            
            Overlay* overlay1 = OgreFramework::getSingletonPtr()->m_pOverlayMgr->getByName("StudySettingsOverlay");
            
            if(!player->isNextPage)
            {
                
                //HIDE PAGE 1
                
                //Hide UnlimitedFuel
                overlay1->remove2D(hud->enableUnlimitedFuelBackground);
                hud->buttons[hud->BUTTON_ENABLE_UNLIMITED_FUEL].hide();
                
                //Hide Holdout and Holdout Delay
                overlay1->remove2D(hud->enableHoldoutBackground);
                overlay1->remove2D(hud->enableHoldoutDelayBackground);
                overlay1->remove2D(hud->enableHoldoutDelayButtonBackground);
                overlay1->remove2D(hud->enableHoldoutDelayNumberButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_HOLDOUT].hide();
                hud->buttons[hud->BUTTON_ENABLE_HOLDOUT_DELAY].hide();
                hud->buttons[hud->BUTTON_HOLDOUT_DELAY_NUMBER].hide();
                
                //Hide ManRecess
                overlay1->remove2D(hud->enableMandatoryRecessBackground);
                overlay1->remove2D(hud->enableMandatoryRecessButtonBackground);
                overlay1->remove2D(hud->enableMandatoryRecessNumberBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_MANDATORY_RECESS].hide();
                hud->buttons[hud->BUTTON_MANDATORY_RECESS_NUMBER].hide();
                
                //Hide init Velocity
                overlay1->remove2D(hud->initSpeedBackground);
                hud->buttons[hud->BUTTON_INPUT_INIT_SPEED].hide();
                
                //Hide New Nav
                overlay1->remove2D(hud->enableNewNavBackground);
                overlay1->remove2D(hud->enableNewNavButtonBackground);
                overlay1->remove2D(hud->enableNewNavNumberButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_NEW_NAV].hide();
                hud->buttons[hud->BUTTON_ENABLE_NEW_NAV_NUMBER].hide();
                
                //Hide New Sounds
                overlay1->remove2D(hud->enableNewSoundsBackground);
                overlay1->remove2D(hud->enableNewSoundsButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_NEW_SOUNDS].hide();
                
                
                //Hide Independent Recess
                overlay1->remove2D(hud->enableIndRecessBackground);
                overlay1->remove2D(hud->enableIndRecessButtonBackground);
                overlay1->remove2D(hud->enableIndRecessNumberButtonBackground);
                overlay1->remove2D(hud->enableIndRecessFixedButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_IND_RECESS].hide();
                hud->buttons[hud->BUTTON_ENABLE_IND_RECESS_NUMBER].hide();
                hud->buttons[hud->BUTTON_ENABLE_IND_RECESS_FIXED].hide();
                
                //Hide Passcode
                overlay1->remove2D(hud->enableSettingsPasscodeBackground);
                overlay1->remove2D(hud->enableSettingsPasscodeButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_PASSCODE].hide();
                
                //Hide Session ID
                overlay1->remove2D(hud->enableSessionScreenBackground);
                overlay1->remove2D(hud->enableSessionScreenButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_SESSION_SCREEN].hide();
                
                //Hide Session Time Settings
                overlay1->remove2D(hud->sessionTimeSettingsBackground);
                overlay1->remove2D(hud->sessionStartTimeNumberBackground);
                overlay1->remove2D(hud->sessionEndTimeNumberBackground);
                overlay1->remove2D(hud->sessionNumNumberBackground);
                
                hud->buttons[hud->BUTTON_SESSION_START_NUMBER].hide();
                hud->buttons[hud->BUTTON_SESSION_END_NUMBER].hide();
                hud->buttons[hud->BUTTON_NUM_OF_SESSIONS_NUMBER].hide();
                
                //Hide Overall Timer
                overlay1->remove2D(hud->enableOverallTimerBackground);
                overlay1->remove2D(hud->enableOverallTimerButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_OVERALL_TIMER].hide();
                
                //Hide Accel Enabled
                overlay1->remove2D(hud->enableAccelBackground);
                overlay1->remove2D(hud->enableAccelButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_ACCEL].hide();
                
                
                //SHOW PAGE 2
                
                //Show Holdout Min Upper Bound
                overlay1->add2D(hud->holdoutMinUpperBoundNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_MIN_UPPER_BOUND].show();
                
                //Show Holdout Max Upper Bound
                overlay1->add2D(hud->holdoutMaxUpperBoundNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_MAX_UPPER_BOUND].show();
                
                
                //Show Holdout Lower Bound Time
                overlay1->add2D(hud->holdoutLowerBoundTimeNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_LOWER_BOUND_TIME].show();
                //Show Holdout Min Upper Bound Time
                overlay1->add2D(hud->holdoutMinUpperBoundTimeNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_MIN_UPPER_BOUND_TIME].show();
                //Show Holdout Max Upper Bound Time
                overlay1->add2D(hud->holdoutMaxUpperBoundTimeNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_MAX_UPPER_BOUND_TIME].show();
                
                //Show Holdout Steps
                overlay1->add2D(hud->holdoutStepsNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_STEPS].show();
                
                
                
                
                
                
            }
            else
            {
                //SHOW PAGE 1
                
                //Show UnlimitedFuel
                overlay1->add2D(hud->enableUnlimitedFuelBackground);
                hud->buttons[hud->BUTTON_ENABLE_UNLIMITED_FUEL].show();
                
                //Show Holdout and Holdout Delay
                overlay1->add2D(hud->enableHoldoutBackground);
                overlay1->add2D(hud->enableHoldoutDelayBackground);
                overlay1->add2D(hud->enableHoldoutDelayButtonBackground);
                overlay1->add2D(hud->enableHoldoutDelayNumberButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_HOLDOUT].show();
                hud->buttons[hud->BUTTON_ENABLE_HOLDOUT_DELAY].show();
                hud->buttons[hud->BUTTON_HOLDOUT_DELAY_NUMBER].show();
                
                //Show ManRecess
                overlay1->add2D(hud->enableMandatoryRecessBackground);
                overlay1->add2D(hud->enableMandatoryRecessButtonBackground);
                overlay1->add2D(hud->enableMandatoryRecessNumberBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_MANDATORY_RECESS].show();
                hud->buttons[hud->BUTTON_MANDATORY_RECESS_NUMBER].show();
                
                //Show init Velocity
                overlay1->add2D(hud->initSpeedBackground);
                hud->buttons[hud->BUTTON_INPUT_INIT_SPEED].show();
                
                //Show New Nav
                overlay1->add2D(hud->enableNewNavBackground);
                overlay1->add2D(hud->enableNewNavButtonBackground);
                overlay1->add2D(hud->enableNewNavNumberButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_NEW_NAV].show();
                hud->buttons[hud->BUTTON_ENABLE_NEW_NAV_NUMBER].show();
                
                //Show New Sounds
                overlay1->add2D(hud->enableNewSoundsBackground);
                overlay1->add2D(hud->enableNewSoundsButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_NEW_SOUNDS].show();
                
                //Show Indpendent Recess
                overlay1->add2D(hud->enableIndRecessBackground);
                overlay1->add2D(hud->enableIndRecessButtonBackground);
                overlay1->add2D(hud->enableIndRecessNumberButtonBackground);
                overlay1->add2D(hud->enableIndRecessFixedButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_IND_RECESS].show();
                hud->buttons[hud->BUTTON_ENABLE_IND_RECESS_NUMBER].show();
                hud->buttons[hud->BUTTON_ENABLE_IND_RECESS_FIXED].show();
                
                //Show Passcode
                overlay1->add2D(hud->enableSettingsPasscodeBackground);
                overlay1->add2D(hud->enableSettingsPasscodeButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_PASSCODE].show();
                
                //Show SessionID
                overlay1->add2D(hud->enableSessionScreenBackground);
                overlay1->add2D(hud->enableSessionScreenButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_SESSION_SCREEN].show();
                
                //Show Session Time Settings
                overlay1->add2D(hud->sessionTimeSettingsBackground);
                overlay1->add2D(hud->sessionStartTimeNumberBackground);
                overlay1->add2D(hud->sessionEndTimeNumberBackground);
                overlay1->add2D(hud->sessionNumNumberBackground);
                
                hud->buttons[hud->BUTTON_SESSION_START_NUMBER].show();
                hud->buttons[hud->BUTTON_SESSION_END_NUMBER].show();
                hud->buttons[hud->BUTTON_NUM_OF_SESSIONS_NUMBER].show();
                
                //Hide Overall Timer
                overlay1->add2D(hud->enableOverallTimerBackground);
                overlay1->add2D(hud->enableOverallTimerButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_OVERALL_TIMER].show();
                
                //Hide Accel
                overlay1->add2D(hud->enableAccelBackground);
                overlay1->add2D(hud->enableAccelButtonBackground);
                
                hud->buttons[hud->BUTTON_ENABLE_ACCEL].show();
                
                //HIDE PAGE 2
                
                //Hide Holdout Min Upper Bound
                overlay1->remove2D(hud->holdoutMinUpperBoundNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_MIN_UPPER_BOUND].hide();
                
                overlay1->remove2D(hud->holdoutMaxUpperBoundNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_MAX_UPPER_BOUND].hide();
                
                //Hide Holdout Lower Bound Time
                overlay1->remove2D(hud->holdoutLowerBoundTimeNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_LOWER_BOUND_TIME].hide();
                //Hide Holdout Min Upper Bound Time
                overlay1->remove2D(hud->holdoutMinUpperBoundTimeNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_MIN_UPPER_BOUND_TIME].hide();
                //Hide Holdout Max Upper Bound Time
                overlay1->remove2D(hud->holdoutMaxUpperBoundTimeNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_MAX_UPPER_BOUND_TIME].hide();
                
                //Hide Holdout Steps
                overlay1->remove2D(hud->holdoutStepsNumberBackground);
                hud->buttons[hud->BUTTON_HOLDOUT_STEPS].hide();
                
            }
            
            player->isNextPage = !player->isNextPage;
        }
        
    }
    if(queryGUI == "popupok")
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
            
            globals.holdoutMinUpperBound =tempholdoutMinUpperBound;
            globals.holdoutMaxUpperBound= tempholdoutMaxUpperBound;
            globals.holdoutLowerBoundTime= tempholdoutLowerBoundTime;
            globals.holdoutUpperBoundMinTime= tempholdoutUpperBoundMinTime;
            globals.holdoutUpperBoundMaxTime= tempholdoutUpperBoundMaxTime;
            globals.holdoutSteps=tempholdoutSteps ;
            globals.OverallTimerEnabled = tempOverallTimerEnabled;
            globals.accelEnabled = tempaccelEnabled;
            
            /* Need to do this when I press ok!*/
             
             
            globals.saveGlobalSettings(globals.globalPath);
            player->saveProgress(globals.savePath);
            hud->initStrings();
            hud->popUpisOut = false;
            hud->hidePopUp();
            //player->isNextPage = false;
            //Show everything again
            
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
               tempenableIndRecessFixed == globals.enableIndRecessFixed &&
               tempholdoutMinUpperBound == globals.holdoutMinUpperBound &&
               tempholdoutMaxUpperBound == globals.holdoutMaxUpperBound &&
               tempholdoutLowerBoundTime == globals.holdoutLowerBoundTime &&
               tempholdoutUpperBoundMinTime == globals.holdoutUpperBoundMinTime &&
               tempholdoutUpperBoundMaxTime == globals.holdoutUpperBoundMaxTime &&
               tempholdoutSteps == globals.holdoutSteps &&
               tempOverallTimerEnabled == globals.OverallTimerEnabled &&
               tempaccelEnabled == globals.accelEnabled)
                
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
    else if(queryGUI == "checkaccel")
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
            
            if(globals.accelEnabled)
            {
                globals.accelEnabled = false;
            }
            else
            {
                globals.accelEnabled = true;
            }
            
        }

    }
    
    else if (queryGUI == "checkoveralltimer")
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
            
            if(globals.OverallTimerEnabled)
            {
                globals.OverallTimerEnabled = false;
            }
            else
            {
                globals.OverallTimerEnabled = true;
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
            
            if(globals.enableSettingsPasscode)
            {
                hud->enableNumpad = false;
                hud->showDecimal = false;
                hud->nStatus = hud->NONE;
                globals.enableSettingsPasscode = false;
            }
            else
            {
                hud->enableNumpad = true;
                hud->showDecimal = false;
                hud->nStatus = hud->PASSCODE;
            }
            
            
            
            
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() != 0)
                {
                    hud->holdoutMinUpperBoundString.erase(hud->holdoutMinUpperBoundString.size()-1,1);

                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() != 0)
                {
                    hud->holdoutMaxUpperBoundString.erase(hud->holdoutMaxUpperBoundString.size()-1,1);
                    
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() != 0)
                {
                    hud->holdoutLowerBoundTimeString.erase(hud->holdoutLowerBoundTimeString.size()-1,1);
                    
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() != 0)
                {
                    hud->holdoutMinUpperBoundTimeString.erase(hud->holdoutMinUpperBoundTimeString.size()-1,1);
                    
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() != 0)
                {
                    hud->holdoutMaxUpperBoundTimeString.erase(hud->holdoutMaxUpperBoundTimeString.size()-1,1);
                    
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() != 0)
                {
                    hud->holdoutStepsString.erase(hud->holdoutStepsString.size()-1,1);
                    
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "0";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "0";
                }
            }
            
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "0";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "0";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "0";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "0";
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "1";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "1";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "1";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "1";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "1";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "1";
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "2";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "2";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "2";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "2";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "2";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "2";
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "3";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "3";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "3";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "3";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "3";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "3";
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "4";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "4";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "4";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "4";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "4";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "4";
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "5";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "5";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "5";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "5";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "5";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "5";
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "6";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "6";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "6";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "6";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "6";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "6";
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "7";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "7";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "7";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "7";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "7";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "7";
                }
            }
        }
        
    }
    else if(queryGUI == "numpadbutton8")
    {
        if(!hud->popUpisOut )// To remove sound! && hud->enableNumpad)
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "8";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "8";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "8";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "8";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "8";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "8";
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= "9";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= "9";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= "9";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= "9";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= "9";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString.size() < 2)
                {
                    hud->holdoutStepsString+= "9";
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
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString != "")
                {
                    
                    globals.holdoutMinUpperBound= std::atof(hud->holdoutMinUpperBoundString.c_str());
                    if(globals.holdoutMinUpperBound < 0) //What do I use!
                    {
                        globals.holdoutMinUpperBound = 0;
                    }
                    if(globals.holdoutMinUpperBound > 100.0) //What do I use!
                    {
                        globals.holdoutMinUpperBound = 100.0;
                    }
                    if(globals.holdoutMinUpperBound > globals.holdoutMaxUpperBound) //What do I use!
                    {
                        globals.holdoutMinUpperBound = globals.holdoutMaxUpperBound;
                    }
                    
                }
                
                hud->holdoutMinUpperBoundString = Util::toStringFloat(globals.holdoutMinUpperBound,1);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString != "")
                {
                    
                    globals.holdoutMaxUpperBound= std::atof(hud->holdoutMaxUpperBoundString.c_str());
                    if(globals.holdoutMaxUpperBound < 0) //What do I use!
                    {
                        globals.holdoutMaxUpperBound = 0;
                    }
                    if(globals.holdoutMaxUpperBound > 100.0) //What do I use!
                    {
                        globals.holdoutMaxUpperBound = 100.0;
                    }
                    if(globals.holdoutMaxUpperBound < globals.holdoutMinUpperBound) //What do I use!
                    {
                        globals.holdoutMaxUpperBound = globals.holdoutMinUpperBound;
                    }
                    
                    
                }
                
                hud->holdoutMaxUpperBoundString = Util::toStringFloat(globals.holdoutMaxUpperBound,1);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString != "")
                {
                    
                    globals.holdoutLowerBoundTime= std::atof(hud->holdoutLowerBoundTimeString.c_str());
                    if(globals.holdoutLowerBoundTime < 0) //What do I use!
                    {
                        globals.holdoutLowerBoundTime = 0;
                    }
                    if(globals.holdoutLowerBoundTime > 1.0) //What do I use!
                    {
                        globals.holdoutLowerBoundTime = 1.0;
                    }
                    if(globals.holdoutLowerBoundTime > globals.holdoutUpperBoundMinTime) //What do I use!
                    {
                        globals.holdoutLowerBoundTime = globals.holdoutUpperBoundMinTime;
                    }
                    
                    
                }
                
                hud->holdoutLowerBoundTimeString = Util::toStringFloat(globals.holdoutLowerBoundTime,3);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString != "")
                {
                    
                    globals.holdoutUpperBoundMinTime= std::atof(hud->holdoutMinUpperBoundTimeString.c_str());
                    if(globals.holdoutUpperBoundMinTime < 0) //What do I use!
                    {
                        globals.holdoutUpperBoundMinTime = 0;
                    }
                    if(globals.holdoutUpperBoundMinTime > 1.0) //What do I use!
                    {
                        globals.holdoutUpperBoundMinTime = 1.0;
                    }
                    if(globals.holdoutUpperBoundMinTime > globals.holdoutUpperBoundMaxTime) //What do I use!
                    {
                        globals.holdoutUpperBoundMinTime = globals.holdoutUpperBoundMaxTime;
                    }
                    if(globals.holdoutUpperBoundMinTime < globals.holdoutLowerBoundTime) //What do I use!
                    {
                        globals.holdoutUpperBoundMinTime = globals.holdoutLowerBoundTime;
                    }
                    
                    
                }
                
                hud->holdoutMinUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMinTime,3);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString != "")
                {
                    
                    globals.holdoutUpperBoundMaxTime= std::atof(hud->holdoutMaxUpperBoundTimeString.c_str());
                    if(globals.holdoutUpperBoundMaxTime < 0) //What do I use!
                    {
                        globals.holdoutUpperBoundMaxTime = 0;
                    }
                    if(globals.holdoutUpperBoundMaxTime > 1.0) //What do I use!
                    {
                        globals.holdoutUpperBoundMaxTime = 1.0;
                    }
                    if(globals.holdoutUpperBoundMaxTime < globals.holdoutUpperBoundMinTime) //What do I use!
                    {
                        globals.holdoutUpperBoundMaxTime = globals.holdoutUpperBoundMinTime;
                    }
                    
                }
                
                hud->holdoutMaxUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMaxTime,3);
                hud->enableNumpad = false;
                
            }
            if(hud->nStatus == hud->HOLDOUT_STEPS)
            {
                if(hud->holdoutStepsString != "")
                {
                    
                    globals.holdoutSteps= std::atoi(hud->holdoutStepsString.c_str());
                    if(globals.holdoutSteps < 1) //What do I use!
                    {
                        globals.holdoutSteps = 1;
                    }
                    if(globals.holdoutSteps > 100) //What do I use!
                    {
                        globals.holdoutSteps = 99;
                    }

                    
                }
                
                hud->holdoutStepsString = Util::toStringInt(globals.holdoutSteps);
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
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                if(hud->holdoutMinUpperBoundString.size() < 5)
                {
                    hud->holdoutMinUpperBoundString+= ".";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                if(hud->holdoutMaxUpperBoundString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundString+= ".";
                }
            }
            
            
            if( hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                if(hud->holdoutLowerBoundTimeString.size() < 5)
                {
                    hud->holdoutLowerBoundTimeString+= ".";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMinUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMinUpperBoundTimeString+= ".";
                }
            }
            if( hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                if(hud->holdoutMaxUpperBoundTimeString.size() < 5)
                {
                    hud->holdoutMaxUpperBoundTimeString+= ".";
                }
            }

        }
        
    }
    else if(queryGUI == "numpad")
    {
        std::cout << "Touched numpad" << std::endl;
    }
    
    
    
    
    
    
    else if(queryGUI == "holdoutminupperbound")
    {
        if(!hud->popUpisOut)
        {
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                hud->holdoutMaxUpperBoundString =Util::toStringFloat(globals.holdoutMaxUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                hud->holdoutLowerBoundTimeString = Util::toStringFloat(globals.holdoutLowerBoundTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                hud->holdoutMinUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMinTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                hud->holdoutMaxUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMaxTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_STEPS)
            {
                hud->holdoutStepsString = Util::toStringInt(globals.holdoutSteps);
            }
            
            hud->enableNumpad = true;
            hud->showDecimal = true;
            hud->nStatus = hud->HOLDOUT_MIN_UPPER_BOUND;
            hud->holdoutMinUpperBoundString = "";
        }
    }
    else if(queryGUI == "holdoutmaxupperbound")
    {
        if(!hud->popUpisOut)
        {
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                hud->holdoutMinUpperBoundString =Util::toStringFloat(globals.holdoutMinUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                hud->holdoutLowerBoundTimeString = Util::toStringFloat(globals.holdoutLowerBoundTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                hud->holdoutMinUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMinTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                hud->holdoutMaxUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMaxTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_STEPS)
            {
                hud->holdoutStepsString = Util::toStringInt(globals.holdoutSteps);
            }
            
            hud->enableNumpad = true;
            hud->showDecimal = true;
            hud->nStatus = hud->HOLDOUT_MAX_UPPER_BOUND;
            hud->holdoutMaxUpperBoundString = "";
        }
    }
    else if(queryGUI == "holdoutlowerboundtime")
    {
        if(!hud->popUpisOut)
        {
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                hud->holdoutMinUpperBoundString =Util::toStringFloat(globals.holdoutMinUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                hud->holdoutMaxUpperBoundString =Util::toStringFloat(globals.holdoutMaxUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                hud->holdoutMinUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMinTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                hud->holdoutMaxUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMaxTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_STEPS)
            {
                hud->holdoutStepsString = Util::toStringInt(globals.holdoutSteps);
            }
            
            hud->enableNumpad = true;
            hud->showDecimal = true;
            hud->nStatus = hud->HOLDOUT_LOWER_BOUND_TIME;
            hud->holdoutLowerBoundTimeString = "";
        }
    }
    else if(queryGUI == "holdoutminupperboundtime")
    {
        if(!hud->popUpisOut)
        {
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                hud->holdoutMinUpperBoundString =Util::toStringFloat(globals.holdoutMinUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                hud->holdoutMaxUpperBoundString =Util::toStringFloat(globals.holdoutMaxUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                hud->holdoutLowerBoundTimeString = Util::toStringFloat(globals.holdoutLowerBoundTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                hud->holdoutMaxUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMaxTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_STEPS)
            {
                hud->holdoutStepsString = Util::toStringInt(globals.holdoutSteps);
            }
            hud->enableNumpad = true;
            hud->showDecimal = true;
            hud->nStatus = hud->HOLDOUT_MIN_UPPER_BOUND_TIME;
            hud->holdoutMinUpperBoundTimeString = "";
        }
    }
    else if(queryGUI == "holdoutmaxupperboundtime")
    {
        if(!hud->popUpisOut)
        {
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                hud->holdoutMinUpperBoundString =Util::toStringFloat(globals.holdoutMinUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                hud->holdoutMaxUpperBoundString =Util::toStringFloat(globals.holdoutMaxUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                hud->holdoutLowerBoundTimeString = Util::toStringFloat(globals.holdoutLowerBoundTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                hud->holdoutMinUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMinTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_STEPS)
            {
                hud->holdoutStepsString = Util::toStringInt(globals.holdoutSteps);
            }
            hud->enableNumpad = true;
            hud->showDecimal = true;
            hud->nStatus = hud->HOLDOUT_MAX_UPPER_BOUND_TIME;
            hud->holdoutMaxUpperBoundTimeString = "";
        }
    }
    else if(queryGUI == "holdoutsteps")
    {
        
        
        if(!hud->popUpisOut)
        {
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
            {
                hud->holdoutMinUpperBoundString =Util::toStringFloat(globals.holdoutMinUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
            {
                hud->holdoutMaxUpperBoundString =Util::toStringFloat(globals.holdoutMaxUpperBound,1);
            }
            if(hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
            {
                hud->holdoutLowerBoundTimeString = Util::toStringFloat(globals.holdoutLowerBoundTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
            {
                hud->holdoutMinUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMinTime,3);
            }
            if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
            {
                hud->holdoutMaxUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMaxTime,3);
            }
            hud->enableNumpad = true;
            hud->showDecimal = false;
            hud->nStatus = hud->HOLDOUT_STEPS;
            hud->holdoutStepsString = "";
        }
        

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
                if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND)
                {
                    hud->holdoutMinUpperBoundString =Util::toStringFloat(globals.holdoutMinUpperBound,1);
                }
                if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND)
                {
                    hud->holdoutMaxUpperBoundString =Util::toStringFloat(globals.holdoutMaxUpperBound,1);
                }
                if(hud->nStatus == hud->HOLDOUT_LOWER_BOUND_TIME)
                {
                    hud->holdoutLowerBoundTimeString = Util::toStringFloat(globals.holdoutLowerBoundTime,3);
                }
                if(hud->nStatus == hud->HOLDOUT_MIN_UPPER_BOUND_TIME)
                {
                    hud->holdoutMinUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMinTime,3);
                }
                if(hud->nStatus == hud->HOLDOUT_MAX_UPPER_BOUND_TIME)
                {
                    hud->holdoutMaxUpperBoundTimeString = Util::toStringFloat(globals.holdoutUpperBoundMaxTime,3);
                }
                if(hud->nStatus == hud->HOLDOUT_STEPS)
                {
                    hud->holdoutStepsString = Util::toStringInt(globals.holdoutSteps);
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
