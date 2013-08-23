#include "PoppyPattern.h"

#include <cstdlib>

PoppyPattern::PoppyPattern(Screen *screen, CollisionScene *scene, unsigned seed)
: PopzorsPattern(screen, scene, seed), signaled(false), signalStart(SIGNAL_START), signalLength(SIGNAL_LENGTH), timer(0), numImportantPoppies(0), numDistractingPoppies(0), backwardsOrder(false), blinkPoppyIndex(0), playerPoppyIndex(0), selected(NULL)
{
}

void PoppyPattern::setup()
{
    stage.ground = new Ground(Vector3(0, -0.5, 0), GROUND_COLOR, GROUND_COLOR, SIGNAL_LENGTH);
	stage.ground->addToCollisionScene(stage.scene);
    
	//Make some pots
    const Number POT_RADIUS = 1.0;
	Pot* pot1 = new Pot(Vector3(2.5, 0.0, 2.5), POT_RADIUS, Cpot1, Cpot1, 1, Spot1);
    pot1->setId(0);
    Pot* pot2 = new Pot(Vector3(2.5, 0.0, -2.5), POT_RADIUS, Cpot2, Cpot2, 1, Spot2);
    pot2->setId(1);
    Pot* pot3 = new Pot(Vector3(-2.5, 0.0, 2.5), POT_RADIUS, Cpot3, Cpot3, 1, Spot3);
    pot3->setId(2);
    Pot* pot4 = new Pot(Vector3(-2.5, 0.0, -2.5), POT_RADIUS, Cpot4, Cpot4, 1, Spot4);
    pot4->setId(3);
    
	pot1->addToCollisionScene(stage.scene);
	pot2->addToCollisionScene(stage.scene);
	pot3->addToCollisionScene(stage.scene);
	pot4->addToCollisionScene(stage.scene);
	
	stage.pots.push_back(pot1);
	stage.pots.push_back(pot2);
	stage.pots.push_back(pot3);
	stage.pots.push_back(pot4);
    
	//Make some poppies
    numImportantPoppies = player.level;
    numDistractingPoppies = 0;
    
    player.totalProblems = numImportantPoppies; // For base class Pattern
	for (int i = 0; i < numImportantPoppies + numDistractingPoppies; ++i) {
		Poppy* poppy = new Poppy(Vector3(randRangeDouble(-1.5, 1.5),0,randRangeDouble(-1.5, 1.5)),
                     BLAND_COLOR, BLAND_COLOR);
        poppy->setId(i);
		poppy->addToCollisionScene(stage.scene);
        
        if (stage.pots.size() > 0)
        {
            int r = rand() % stage.pots.size();
            poppy->setPotIdRef(stage.pots[r]->getId()); // Used tempoarily to play pot sound
            poppy->setBlinkColor(stage.pots[r]->getBaseColor());
        }
		stage.poppies.push_back(poppy);
	}
    
    
    SceneLight * light = new SceneLight(SceneLight::AREA_LIGHT, stage.scene, 5);
    light->setPosition(3, 3.5, 0);
    stage.scene->addLight(light);
    
    SceneLight * light1 = new SceneLight(SceneLight::AREA_LIGHT, stage.scene, 5);
    light1->setPosition(-3, 3.5, 0);
    stage.scene->addLight(light1);
    
    SceneLight * light2 = new SceneLight(SceneLight::AREA_LIGHT, stage.scene, 5);
    light2->setPosition(3, 3.5, 3);
    stage.scene->addLight(light2);
    
    SceneLight * light3 = new SceneLight(SceneLight::AREA_LIGHT, stage.scene, 5);
    light3->setPosition(-3, 3.5, 3);
    stage.scene->addLight(light3);
    
    stage.scene->ambientColor = Color(0.2, 0.2, 0.2, 0.2);
}

void PoppyPattern::reset()
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
}

void PoppyPattern::setPattern()
{
    reset();
    setup();
    
    // Make pots flash red to notify a backwards pattern
    backwardsOrder = rand() % 2;
    if (numImportantPoppies > 1 && backwardsOrder)
    {
        for (int i = 0; i < stage.pots.size(); ++i)
        {
            stage.pots[i]->setBlinkColor(Color(1.0, 0.0, 0.0, 1.0));
            stage.pots[i]->setTimeBlinkLength(signalStart / 2, 0.1, 0.1);
            stage.pots[i]->activateBlink();
        }
    }
    
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

bool PoppyPattern::isFinished() const
{
    return playerPoppyIndex >= numImportantPoppies;
}

void PoppyPattern::processSelect(ClickedResult res)
{
    if (res.poppy && res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
        
        if (!res.poppy->isSelectable())
            res.pot = stage.pots[res.poppy->getPotIdRef()]; // Otherwise, select the pot
        
        //deselect
        if ( (selected != res.poppy) && (selected != NULL) )
            selected = NULL;
        res.poppy->setColor(SELECT_COLOR);
        selected = res.poppy;
        selected->getType();
    }
    
    if (res.pot && res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
        res.pot->playSound();
        if (selected && selected != res.pot && isReady())
            if (selected->getType() == Selectable::TYPE_POPPY) {
                Poppy * old = (Poppy*)selected;
                Vector3 potpos = res.pot->getPosition();
                old->setPosition(potpos.x + randRangeDouble(-0.5, 0.5), potpos.y, potpos.z + randRangeDouble(-0.5, 0.5));
                old->setSelectable(false);
                //old->setPotIdRef(res.pot->getId());
                old->deactivateJump();
                selected = NULL;
                
                updatePlayerChoice(old, res.pot);
            }
    }
}

void PoppyPattern::update(Number elapsed)
{
    updatePoppyBlinks(elapsed);
    stage.ground->update(elapsed);
    for (int i = 0; i < stage.pots.size(); ++i)
        stage.pots[i]->update(elapsed);
    for (int i = 0; i < stage.poppies.size(); ++i)
        stage.poppies[i]->update(elapsed);
    
    stage.handlePoppyCollisions(elapsed);
    
    if (isFinished() && !stage.ground->isBlinking())
        setPattern();
}

void PoppyPattern::updatePlayerChoice(Poppy* poppy, Pot* pot)
{
    /*
    if (pot->getId() == poppy->getPotIdRef() && poppy->getId() == playerPoppyIndex)
    {
        player.numCorrect++;
    }
    */
    
    if (pot->getId() == poppy->getPotIdRef())
    {
        player.numCorrect++;
    }
    playerPoppyIndex++;
    
    if (isFinished())
    {
        if (player.numCorrect >= player.totalProblems)
        {
            stage.ground->setBlinkColor(FEEDBACK_COLOR_GOOD);
            player.updateLevel(PLAYER_SUCCESS);
        }
        else
        {
            const double PASSING_CHECK = 0.75;
            double correctness = getPlayerCorrectness();
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
            player.updateLevel(PLAYER_FAILURE);
        }
        stage.ground->activateBlink();
    }
}

void PoppyPattern::updatePoppyBlinks(Number elapsed)
{
    timer += elapsed;
	if (!signaled && timer > signalStart && stage.poppies.size() > 0)
    {
        signaled = true;
        stage.poppies[blinkPoppyIndex]->activateBlink();
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
            stage.poppies[blinkPoppyIndex]->activateJump();
            stage.pots[stage.poppies[blinkPoppyIndex]->getPotIdRef()]->playSound();
        }
        else ready = true;
        timer = 0;
	}
}
