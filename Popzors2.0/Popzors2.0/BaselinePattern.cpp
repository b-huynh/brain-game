#include "BaselinePattern.h"

#include <cstdlib>
#include <cmath>
#include <sstream>
#include <fstream>

BaselinePattern::BaselinePattern(unsigned seed, int width, int height)
: PopzorsPattern(seed), screenWidth(width), screenHeight(height), signaled(false), signalStart(SIGNAL_START), signalLength(SIGNAL_LENGTH), timer(0), numImportantPoppies(0), numDistractingPoppies(0),
    poppyRadius(POPPY_RADIUS), backwardsOrder(false), blinkPoppyIndex(0), playerPoppyIndex(0), selected(NULL)
{
    totalElapsed = 0.0;
    score = 0;
}

void BaselinePattern::setup()
{
    SCREEN_WIDTH = screenWidth;
    SCREEN_HEIGHT = screenHeight;
    BAR_XPOS = 20;
    BAR_YPOS = 20;
    BAR_WIDTH = screenWidth - BAR_XPOS * 2;
    BAR_HEIGHT = screenHeight / 20;
    LABEL1_POSX = 0;
    LABEL1_POSY = screenHeight - 55;
    LABEL2_POSX = 0;
    LABEL2_POSY = screenHeight - 105;
    LABEL3_POSX = screenWidth - 255;
    LABEL3_POSY = screenHeight - 55;
    
    stage.ground = new Ground(Vector3(0, -0.5, 0), GROUND_COLOR, GROUND_COLOR, SIGNAL_LENGTH);
	stage.ground->addToScene();
    
	//Make some pots
    const double POT_RADIUS = 1.0;
	Pot* pot1 = new Pot(Vector3(2.5, 0.0, 1.5), POT_RADIUS, Cpot1, Cpot1, 1, Spot1);
    pot1->setId(0);
    Pot* pot2 = new Pot(Vector3(2.5, 0.0, -3.5), POT_RADIUS, Cpot2, Cpot2, 1, Spot2);
    pot2->setId(1);
    Pot* pot3 = new Pot(Vector3(-2.5, 0.0, 1.5), POT_RADIUS, Cpot3, Cpot3, 1, Spot3);
    pot3->setId(2);
    Pot* pot4 = new Pot(Vector3(-2.5, 0.0, -3.5), POT_RADIUS, Cpot4, Cpot4, 1, Spot4);
    pot4->setId(3);
    
	pot1->addToScene();
	pot2->addToScene();
	pot3->addToScene();
	pot4->addToScene();
	
	stage.pots.push_back(pot1);
	stage.pots.push_back(pot2);
	stage.pots.push_back(pot3);
	stage.pots.push_back(pot4);
    
	//Make some poppies
    numImportantPoppies = player.level;
    numDistractingPoppies = 0;
    poppyRadius = POPPY_RADIUS * pow(0.95, numImportantPoppies - 1);
    
    int assignCount[stage.pots.size()];
    for (int i = 0; i < stage.pots.size(); ++i)
        assignCount[i] = 0;
    
    player.totalProblems = numImportantPoppies; // For base class Pattern
	for (int i = 0; i < numImportantPoppies; ++i) {
		Poppy* poppy = new Poppy(Vector3(randRangeDouble(-1.5, 1.5),0,randRangeDouble(-2.5, 0.5)), BLAND_COLOR, BLAND_COLOR, 0, poppyRadius);
        poppy->setId(i);
		poppy->addToScene();
        
        if (stage.pots.size() > 0)
        {
            // Do not assign the majority of the poppies to the same pot
            int r;
            do {
                r = rand() % stage.pots.size();
            } while (assignCount[r] > 0 && assignCount[r] >= (numImportantPoppies + 1) / 2);
            poppy->setPotIdRef(stage.pots[r]->getId()); // Used tempoarily to play pot sound
            poppy->setBlinkColor(stage.pots[r]->getBaseColor());
            assignCount[r]++;;
        }
		stage.poppies.push_back(poppy);
        
        poppyData data;
        data.playerLevel = player.level;
        data.stageId = (int)player.progression.size();
        data.poppyID = poppy->getId();
        data.poppyType = getColorId(poppy->getBlinkColor());
        data.poppyFlashTime = -999; // Instantiate with bad data
        data.binPlaceIn = -999;
        data.binPlaceTime = -999;
        pData.push_back(data);
	}
    
        
    for (int i = 0; i < numDistractingPoppies; ++i) {
		Poppy* poppy = new Poppy(Vector3(randRangeDouble(-1.5, 1.5),0,randRangeDouble(-1.5, 1.5)),
                                 BLAND_COLOR, BLAND_COLOR, 0, (POPPY_RADIUS * pow(0.95, numImportantPoppies - 1)));
        poppy->setId(i);
		poppy->addToScene();
	}
    
//    SceneLight* light = new SceneLight(SceneLight::AREA_LIGHT, stage.scene, 5);
//    light->setPosition(3, 3.5, 0);
//    stage.scene->addLight(light);
//    
//    SceneLight* light1 = new SceneLight(SceneLight::AREA_LIGHT, stage.scene, 5);
//    light1->setPosition(-3, 3.5, 0);
//    stage.scene->addLight(light1);
//    
//    SceneLight* light2 = new SceneLight(SceneLight::AREA_LIGHT, stage.scene, 5);
//    light2->setPosition(3, 3.5, 3);
//    stage.scene->addLight(light2);
//    
//    SceneLight* light3 = new SceneLight(SceneLight::AREA_LIGHT, stage.scene, 5);
//    light3->setPosition(-3, 3.5, 3);
//    stage.scene->addLight(light3);
    
    //stage.scene->ambientColor = Color(0.5, 0.5, 0.5, 0.5);
    
    //stage.progressBar = new ScreenShape(ScreenShape::SHAPE_RECT, BAR_WIDTH + 400, BAR_HEIGHT);
    //stage.progressBar->setPosition((Vector2(BAR_XPOS, BAR_YPOS) + Vector2(BAR_XPOS + BAR_WIDTH, BAR_YPOS + BAR_HEIGHT)) / 2);
    
    if (player.numConsecutiveSuccess > 0) {
        //stage.progressBar->setScale(static_cast<double>(player.numConsecutiveSuccess) / Player::levelUpCeiling, 1.0);
        //stage.progressBar->setColor(0.0, 1.0, 0.0, 1.0);
        if (player.numConsecutiveSuccess >= Player::levelUpCeiling);
            //stage.progressBar->setColor(0.0, 0.0, 1.0, 1.0);
    } else {
        //stage.progressBar->setScale(0.0, 0.0);
        //stage.progressBar->setColor(1.0, 0.0, 0.0, 1.0);
    }
    
    //stage.screen->addChild(stage.progressBar);
    
//    stage.label1 = new ScreenLabel("Time: " + toStringInt(totalElapsed), 36);
//    stage.label1->setPosition(LABEL1_POSX, LABEL1_POSY);
//    stage.screen->addChild(stage.label1);
//    
//	stage.label2 = new ScreenLabel("Score: " + toStringInt(score), 36);
//    stage.label2->setPosition(LABEL2_POSX, LABEL2_POSY);
//	stage.screen->addChild(stage.label2);
    
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
    selected = NULL;
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
            stage.poppies[numImportantPoppies - count - 1]->setTimeBlinkLength(signalLength * (numImportantPoppies - count));
        else
            stage.poppies[count]->setTimeBlinkLength(signalLength * (numImportantPoppies - count));
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
    
    updatePoppyBlinks(elapsed);
    stage.ground->update(elapsed);
    for (int i = 0; i < stage.pots.size(); ++i)
        stage.pots[i]->update(elapsed);
    for (int i = 0; i < stage.poppies.size(); ++i)
        stage.poppies[i]->update(elapsed);
    
    stage.handlePoppyCollisions(elapsed);
    
    if (isFinished() && !stage.ground->isBlinking()) {
        saveData.push_back(getFinishedStageData());
        setPattern();
    }

    //stage.label1->setText("Time: " + toStringInt(totalElapsed));
    //stage.label2->setText("Score: " + toStringInt(score));
}

void BaselinePattern::updatePlayerChoice(Poppy* poppy, Pot* pot)
{
    //Store bin and place time
    int id = poppy->getId();
    for (int i = 0; i < pData.size(); ++i)
        if (pData[i].poppyID == id) {
            pData[i].binPlaceIn = getColorId(pot->getBlinkColor());
            pData[i].binPlaceTime = totalElapsed;
        }
    
    if (pot->getId() == poppy->getPotIdRef())
    {
        player.numCorrect++;
    }
    playerPoppyIndex++;
    
    if (isFinished())
    {
        for (int i = 0; i < stage.poppies.size(); ++i)
            stage.poppies[i]->setBaseColor(stage.poppies[i]->getBlinkColor());
//            stage.poppies[i]->setBaseColor(stage.pots[stage.poppies[i]->getPotIdRef()]->getBaseColor());
        if (player.numCorrect >= player.totalProblems)
        {
            score += player.level;
            
            stage.ground->setBlinkColor(FEEDBACK_COLOR_GOOD);
            
            if (player.numConsecutiveSuccess + 1 == Player::levelUpCeiling) {
                //stage.progressBar->setColor(0.0, 0.0, 1.0, 1.0);
            }
            else
            {
                //stage.progressBar->setScale(static_cast<double>(player.numConsecutiveSuccess + 1) / Player::levelUpCeiling, 1.0);
                //stage.progressBar->setColor(0.0, 1.0, 0.0, 1.0);
            }
            player.updateLevel(PLAYER_SUCCESS);
            //stage.positiveFeedback->Play();
        }
        else
        {
            const double PASSING_CHECK = 0.75;
            double correctness = getPlayerCorrectness();
            stage.ground->setBlinkColor(FEEDBACK_COLOR_BAD);
            /*
            if (correctness > PASSING_CHECK)
            {
                double range = 1 - PASSING_CHECK;
                double value = correctness - PASSING_CHECK;
                stage.ground->setBlinkColor(FEEDBACK_COLOR_GOOD +
                                            Color(198, 0, 198, 0) * (1 - (value / range)));
            }
            else
            {
                double range = PASSING_CHECK;
                double value = PASSING_CHECK - correctness;
                stage.ground->setBlinkColor(FEEDBACK_COLOR_BAD +
                                            Color(0, 198, 198, 0) * (1 - (value / range)));
            }
             */
            
            //stage.progressBar->setScale(0.667, 1.0);
            //stage.progressBar->setColor(1.0, 0.0, 0.0, 1.0);
            
            player.updateLevel(PLAYER_FAILURE);
            //stage.negativeFeedback->Play();
        }
        stage.ground->activateBlink();
    }
}

void BaselinePattern::updatePoppyBlinks(double elapsed)
{
    timer += elapsed;
	if (!signaled && timer > signalStart && stage.poppies.size() > 0)
    {
        signaled = true;
        stage.poppies[blinkPoppyIndex]->activateBlink();
        
        //Store flash time
        int id = stage.poppies[blinkPoppyIndex]->getId();
        for (int i = 0; i < pData.size(); ++i)
            if (pData[i].poppyID == id)
                pData[i].poppyFlashTime = totalElapsed;
        
        stage.poppies[blinkPoppyIndex]->activateJump();
        stage.pots[stage.poppies[blinkPoppyIndex]->getPotIdRef()]->playSound();
        timer = 0;
    }
	if (signaled && blinkPoppyIndex >= 0 && blinkPoppyIndex < numImportantPoppies && timer >= signalLength) {
        if (backwardsOrder)
            blinkPoppyIndex--;
        else
            blinkPoppyIndex++;
        
        if (blinkPoppyIndex >= 0 && blinkPoppyIndex < numImportantPoppies)
        {
            stage.poppies[blinkPoppyIndex]->activateBlink();
            
            //Store flash time
            int id = stage.poppies[blinkPoppyIndex]->getId();
            for (int i = 0; i < pData.size(); ++i)
                if (pData[i].poppyID == id)
                    pData[i].poppyFlashTime = totalElapsed;
            
            stage.poppies[blinkPoppyIndex]->activateJump();
            stage.pots[stage.poppies[blinkPoppyIndex]->getPotIdRef()]->playSound();

        }
        else ready = true;
        timer = 0;
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
        out << "% debug seed: " << player.seed << std::endl;
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