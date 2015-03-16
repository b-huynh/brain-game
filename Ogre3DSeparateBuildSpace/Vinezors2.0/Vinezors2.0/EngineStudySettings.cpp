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
        
        if(player->newNavEnabled)
        {
            std::cout<<"New_Nav Number" << std::endl;
            hud->enableNumpad = true;
            hud->showDecimal = true;
            hud->nStatus = hud->NEW_NAV_INC;
            hud->newNavigationIncAmountString = "";
            
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "0";
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "1";
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "2";
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "3";
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "4";
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "5";
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "6";
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "7";
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "8";
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
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= "9";
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
                if(player->newNavIncrement > 5.0f) //What do I use!
                {
                    player->newNavIncrement = 5.0f;
                }
                
            }
            hud->newNavigationIncAmountString = Util::toStringFloat(player->newNavIncrement,2);
            hud->enableNumpad = false;
            
        }

    }
    else if(queryGUI == "numpadbuttondecimal")
    {
        std::cout<<"Pressed Decimal"<<std::endl;
        if( hud->nStatus == hud->NEW_NAV_INC)
        {
            if(hud->newNavigationIncAmountString.size() < 4)
            {
                hud->newNavigationIncAmountString+= ".";
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
