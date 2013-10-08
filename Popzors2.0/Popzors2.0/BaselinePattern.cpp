#include "BaselinePattern.h"

#include <cstdlib>
#include <cmath>
#include <sstream>
#include <fstream>

BaselinePattern::BaselinePattern(Stage* stage, Player* player)
: PopzorsPattern(stage, player), signaled(false), signalStart(SIGNAL_START), signalLength(SIGNAL_LENGTH), timer(0), numImportantPoppies(0), numDistractingPoppies(0),
    poppyRadius(POPPY_RADIUS), backwardsOrder(false), blinkPoppyIndex(0), playerPoppyIndex(0)
{
    totalElapsed = 0.0;
}

void BaselinePattern::setup()
{
    stage->ground = new Ground(Vector3(0, 0, 0), GROUND_COLOR, GROUND_COLOR, SIGNAL_LENGTH);
    
	//Make some pots
	Pot* pot1 = new Pot(Vector3(2.5, POT_HEIGHT / 2, 2.5), POT_RADIUS, Cpot1, Cpot1, 1, Spot1);
    pot1->setId(0);
    Pot* pot2 = new Pot(Vector3(2.5, POT_HEIGHT / 2, -2.5), POT_RADIUS, Cpot2, Cpot2, 1, Spot2);
    pot2->setId(1);
    Pot* pot3 = new Pot(Vector3(-2.5, POT_HEIGHT / 2, 2.5), POT_RADIUS, Cpot3, Cpot3, 1, Spot3);
    pot3->setId(2);
    Pot* pot4 = new Pot(Vector3(-2.5, POT_HEIGHT / 2, -2.5), POT_RADIUS, Cpot4, Cpot4, 1, Spot4);
    pot4->setId(3);
	
	stage->pots.push_back(pot1);
	stage->pots.push_back(pot2);
	stage->pots.push_back(pot3);
	stage->pots.push_back(pot4);
    
	//Make some poppies
    numImportantPoppies = player->level;
    numDistractingPoppies = 0;
    poppyRadius = POPPY_RADIUS * pow(0.95, numImportantPoppies - 1);
    
    int assignCount[stage->pots.size()];
    for (int i = 0; i < stage->pots.size(); ++i)
        assignCount[i] = 0;
    
    player->totalProblems = numImportantPoppies; // For base class Pattern
    
	for (int i = 0; i < numImportantPoppies; ++i) {
		Poppy* poppy = new Poppy(Vector3(randRangeDouble(-1,1),POPPY_RADIUS,randRangeDouble(-1,1)), BLAND_COLOR, BLAND_COLOR, 0, (POPPY_RADIUS * pow(0.95, numImportantPoppies - 1)));
        poppy->setId(i);
        
        if (stage->pots.size() > 0)
        {
            // Do not assign the majority of the poppies to the same pot
            int r;
            do {
                r = rand() % stage->pots.size();
            } while (assignCount[r] > 0 && assignCount[r] >= (numImportantPoppies + 1) / 2);
            poppy->setPotIdRef(stage->pots[r]->getId()); // Used tempoarily to play pot sound
            poppy->setBlinkColor(stage->pots[r]->getBaseColor());
            assignCount[r]++;;
        }
		stage->poppies.push_back(poppy);
        
        poppyData data;
        data.playerLevel = player->level;
        data.stageId = (int)player->progression.size();
        data.poppyID = poppy->getId();
        data.poppyType = getColorId(poppy->getBlinkColor());
        data.poppyFlashTime = -999; // Instantiate with bad data
        data.binPlaceIn = -999;
        data.binPlaceTime = -999;
        pData.push_back(data);
	}
    
        
    for (int i = 0; i < numDistractingPoppies; ++i) {
		Poppy* poppy = new Poppy(Vector3(randRangeDouble(-1,1),POPPY_RADIUS,randRangeDouble(-1,1)),
                                 BLAND_COLOR, BLAND_COLOR, 0, (POPPY_RADIUS * pow(0.95, numImportantPoppies - 1)));
        poppy->setId(i);
        stage->poppies.push_back(poppy);
	}
    
    //stage.negativeFeedback = new Sound(SOUNDFILE_NEGATIVE_FEEDBACK);
    //stage.positiveFeedback = new Sound(SOUNDFILE_POSITIVE_FEEDBACK);
}

void BaselinePattern::reset()
{
    PopzorsPattern::reset();
    signaled = false;
    signalStart = SIGNAL_START;
    signalLength = SIGNAL_LENGTH;
    timer = 0;
    numImportantPoppies = 0;
    numDistractingPoppies = 0;
    backwardsOrder = false;
    blinkPoppyIndex = 0;
    playerPoppyIndex = 0;
    pData.clear();
    pData.resize(0);
}

void BaselinePattern::setPattern()
{
    reset();
    setup();
    
    // Initialize poppy blink time length
    int count = 0;
    while (count < numImportantPoppies)
    {
        if (backwardsOrder)
            stage->poppies[numImportantPoppies - count - 1]->setTimeBlinkLength(signalLength * (numImportantPoppies - count));
        else
            stage->poppies[count]->setTimeBlinkLength(signalLength * (numImportantPoppies - count));
        ++count;
    }
    
    if (backwardsOrder)
        blinkPoppyIndex = numImportantPoppies - 1;
}

bool BaselinePattern::isFinished() const
{
    return playerPoppyIndex >= numImportantPoppies;
}

void BaselinePattern::processSelect(ClickedResult res)
{
//    if (res.poppy && res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
//        
//        if (!res.poppy->isSelectable()) {
//            res.pot = stage.pots[res.poppy->getPotIdRef()]; // Otherwise, select the pot
//            if (selected == res.poppy)
//            {
//                selected = NULL;
//            }
//        } else {
//            res.poppy->setColor(SELECT_COLOR);
//            selected = res.poppy;
//        }
//    }
//    
//    if (res.pot && res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
//        res.pot->playSound();
//        if (selected && selected != res.pot && isReady())
//            if (selected->getType() == Selectable::TYPE_POPPY) {
//                Poppy * old = (Poppy*)selected;
//                Vector3 potpos = res.pot->getPosition();
//                old->setPosition(potpos.x + randRangeDouble(-0.5, 0.5), potpos.y, potpos.z + randRangeDouble(-0.5, 0.5));
//                old->setSelectable(false);
//                //old->setPotIdRef(res.pot->getId());
//                old->deactivateJump();
//                selected = NULL;
//                
//                updatePlayerChoice(old, res.pot);
//            }
//    }

    //if (selected) {
    //    Poppy* pop = (Poppy*)(selected);
    //    pop->setPosition(res.ray.position.x, 0,res.ray.position.z);
    //}
}

void BaselinePattern::update(double elapsed)
{
    totalElapsed += elapsed;
    
    if (isFinished() && !stage->ground->isBlinking()) {
        updateScore();
        saveData.push_back(getFinishedStageData());
        setPattern();
    }
    
    for (int i = 0; i < stage->poppies.size(); ++i)
        for (int j = 0; j < stage->pots.size(); ++j)
            updatePlayerChoice(stage->poppies[i], stage->pots[j]);
    
    updateFeedback();
    
    updatePoppyBlinks(elapsed);
    
    stage->update(elapsed);
    stage->handlePoppyCollisions(elapsed);
    
    stage->label1->setCaption("Time: " + toStringInt(totalElapsed));
}

void BaselinePattern::updateScore()
{
    if (player->numCorrect >= player->totalProblems)
    {
        player->score += player->level;
        player->updateLevel(PLAYER_SUCCESS);
    }
    else
    {
        player->updateLevel(PLAYER_FAILURE);
    }
}

void BaselinePattern::updateFeedback()
{
    if (isFinished() && !stage->ground->isBlinking())
    {
        for (int i = 0; i < stage->poppies.size(); ++i)
            stage->poppies[i]->setBaseColor(stage->poppies[i]->getBlinkColor());
        if (player->numCorrect >= player->totalProblems)
        {
            player->updateSuccess(PLAYER_SUCCESS);
            stage->ground->setBlinkColor(FEEDBACK_COLOR_GOOD);
            
            //stage.positiveFeedback->Play();
        }
        else
        {
            double correctness = getPlayerCorrectness();
            player->updateSuccess(PLAYER_FAILURE);
            stage->ground->setBlinkColor(FEEDBACK_COLOR_BAD);
            
            //stage.negativeFeedback->Play();
        }
        std::cout << "Is activating blink" << std::endl;
        stage->ground->activateBlink();
    }
    
    double barWidth = Util::HP_BAR_WIDTH;
    if (player->numConsecutiveSuccess > 0) {
        barWidth *= player->numConsecutiveSuccess / (double)(player->levelUpCeiling);
        
        stage->barHP->setDimensions(barWidth, Util::HP_BAR_HEIGHT);
        if (player->numConsecutiveSuccess >= Player::levelUpCeiling)
            stage->barHP->setMaterialName("General/BaseBlue");
        else
            stage->barHP->setMaterialName("General/BaseGreen");
    } else {
        if (stage->ground->getBlinkColor() == FEEDBACK_COLOR_BAD)
            stage->barHP->setDimensions(barWidth, Util::HP_BAR_HEIGHT);
        else
            stage->barHP->setDimensions(0.0, Util::HP_BAR_HEIGHT);
        stage->barHP->setMaterialName("General/BaseRed");
    }
    stage->label2->setCaption("Score: " + toStringInt(player->score));
}

void BaselinePattern::updatePlayerChoice(Poppy* poppy, Pot* pot)
{
    /********* TODO:
     * FIX COLLECTED DATA FOR BIN AND PLACE TIME. THIS TIME IS WRONG
     *********/
    //Store bin and place time
    int id = poppy->getId();
    for (int i = 0; i < pData.size(); ++i)
        if (pData[i].poppyID == id) {
            pData[i].binPlaceIn = getColorId(pot->getBlinkColor());
            pData[i].binPlaceTime = totalElapsed;
        }
    

    if ( ((pot->getPosition() - poppy->getPosition()).length() <= (POT_RADIUS-POPPY_RADIUS + 0.05)) && (poppy != stage->selected))
    {
        if (poppy->isActive()) {
            playerPoppyIndex++;
            poppy->setActive(false);
            if (pot->getId() == poppy->getPotIdRef())
            {
                player->numCorrect++;
            }
        }
    }
}

void BaselinePattern::updatePoppyBlinks(double elapsed)
{
    timer += elapsed;
	if (!signaled && timer > signalStart && stage->poppies.size() > 0)
    {
        signaled = true;
        stage->poppies[blinkPoppyIndex]->activateBlink();
        
        //Store flash time
        int id = stage->poppies[blinkPoppyIndex]->getId();
        for (int i = 0; i < pData.size(); ++i)
            if (pData[i].poppyID == id)
                pData[i].poppyFlashTime = totalElapsed;
        
        if (stage->poppies[blinkPoppyIndex] != stage->selected) {
            stage->poppies[blinkPoppyIndex]->activateJump();
            stage->pots[stage->poppies[blinkPoppyIndex]->getPotIdRef()]->playSound();
        }
        timer = 0;
    }
	if (signaled && blinkPoppyIndex >= 0 && blinkPoppyIndex < numImportantPoppies && timer >= signalLength) {
        if (backwardsOrder)
            blinkPoppyIndex--;
        else
            blinkPoppyIndex++;
        
        if (blinkPoppyIndex >= 0 && blinkPoppyIndex < numImportantPoppies)
        {
            stage->poppies[blinkPoppyIndex]->activateBlink();
            
            //Store flash time
            int id = stage->poppies[blinkPoppyIndex]->getId();
            for (int i = 0; i < pData.size(); ++i)
                if (pData[i].poppyID == id)
                    pData[i].poppyFlashTime = totalElapsed;
            
            
            if (stage->poppies[blinkPoppyIndex] != stage->selected) {
                stage->poppies[blinkPoppyIndex]->activateJump();
                stage->pots[stage->poppies[blinkPoppyIndex]->getPotIdRef()]->playSound();
            }

        }
        else ready = true;
        timer = 0;
	}
    
    if (blinkPoppyIndex >= numImportantPoppies) {
        for (int i = 0; i < stage->poppies.size(); ++i)
            stage->poppies[i]->deactivateJump();
    }
}

std::string BaselinePattern::getFinishedStageData()
{
    std::ostringstream ss;
    
    //Outputs times in milliseconds
    for (int i = 0; i < pData.size(); ++i) {
        ss << pData[i].playerLevel << " " << pData[i].stageId << " ";
        ss << pData[i].poppyID << " " << pData[i].poppyType << " " << pData[i].binPlaceIn << " ";
        ss << pData[i].poppyFlashTime * 1000 << " " << pData[i].binPlaceTime * 1000 << std::endl;
    }
    
    return ss.str();
}

bool BaselinePattern::save(std::string file)
{
    file = getSaveDir() + file;
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    
    if (out.good()) {
        out << "% debug seed: " << player->seed << std::endl;
        out << "% " << std::endl;
        out << "% " << "PlayerLevel, StageID, PoppyID, PoppyType, BinPlaceIn, PoppyFlashTime, BinPlaceTime" << std::endl;
        
        for (int i = 0; i < saveData.size(); ++i)
            out << saveData[i];
        
        out.close();
    }
    else {
        out.close();
        return false;
    }
    
    return true;
}

bool BaselinePattern::mouseMoved(const OIS::MouseEvent &evt)
{
    Ogre::Ray ray = OgreFramework::getSingletonPtr()->getCursorRay();
    Ogre::RaySceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createRayQuery(ray);
    
    query->setSortByDistance(true);
    Ogre::RaySceneQueryResult result = query->execute();
    
    if (stage->selected && stage->selected->getType() == Selectable::TYPE_POPPY)
        for (int i = 0; i < result.size(); ++i)
            if (stage->ground->hasEntity( (Entity*)result[i].movable )) {
                Poppy* toMove = (Poppy*)stage->selected;
                Vector3 hoverPos = ray * result[i].distance;
                hoverPos.y = POPPY_RADIUS;
                toMove->setPosition(hoverPos);
            }
    return true;
}

bool BaselinePattern::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    Ogre::Ray ray = OgreFramework::getSingletonPtr()->getCursorRay();
    Ogre::RaySceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createRayQuery(ray);
    
    query->setSortByDistance(true);
    Ogre::RaySceneQueryResult result = query->execute();
    
    //Handle Selection
    if (stage->selected == NULL) {
        if (result.size() > 0 && result[0].movable != NULL) {
            for (int i = 0; i < stage->poppies.size(); ++i)
                if ( stage->poppies[i]->hasEntity( (Ogre::Entity*)result[0].movable ) ) {
                    stage->poppies[i]->setColor(getRandomPotColor());
                    stage->selected = stage->poppies[i];
                    stage->poppies[i]->deactivateJump();
                    //soundPickUp->play();
                }
            
            for (int i = 0; i < stage->pots.size(); ++i)
                if (stage->pots[i]->hasEntity( (Ogre::Entity*)result[0].movable) )
                    stage->pots[i]->setColor(getRandomPotColor());
        }
    }
    
    return true;
}

bool BaselinePattern::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    Ogre::Ray ray = OgreFramework::getSingletonPtr()->getCursorRay();
    Ogre::RaySceneQuery* query = OgreFramework::getSingletonPtr()->m_pSceneMgr->createRayQuery(ray);
    
    query->setSortByDistance(true);
    Ogre::RaySceneQueryResult result = query->execute();
    
    if (stage->selected->getType() == Selectable::TYPE_POPPY && result.size() > 0) {
        Poppy* old = (Poppy*)stage->selected;
        stage->selected = NULL;
        Vector3 placeDest = ray * result[0].distance;
        placeDest.y = POPPY_RADIUS;
        old->setPosition(placeDest);
    }
    
    return true;
}