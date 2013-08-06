#include "HerdPattern.h"

#include <iostream>

HerdPattern::HerdPattern(Screen * screen, CollisionScene * scene)
:PopzorsPattern(screen, scene), selected(NULL)
{}

void HerdPattern::setup()
{
    stage.ground = new Ground(Vector3(0, -0.5, 0), GROUND_COLOR, GROUND_COLOR, SIGNAL_LENGTH);
	stage.ground->addToCollisionScene(stage.scene);
    
    //Stage variables
    numPoppies = player.level * 2;
    numDistractors = 20;
    player.totalProblems = numPoppies;
    
	//Make some pots
    const Number POT_RADIUS = 0.75;
	Pot* pot1 = new Pot(Vector3(randRangeDouble(-3, 3), 0.0, randRangeDouble(-3, -1)), POT_RADIUS, Cpot1, Cpot1, 1, Spot1);
    pot1->setId(0);
    Pot* pot2 = new Pot(Vector3(randRangeDouble(-3, 3), 0.0, randRangeDouble(-3, -1)), POT_RADIUS, Cpot2, Cpot2, 1, Spot2);
    pot2->setId(1);
    Pot* pot3 = new Pot(Vector3(randRangeDouble(-3, 3), 0.0, randRangeDouble(-3, -1)), POT_RADIUS, Cpot3, Cpot3, 1, Spot3);
    pot3->setId(2);
    Pot* pot4 = new Pot(Vector3(randRangeDouble(-3, 3), 0.0, randRangeDouble(-3, -1)), POT_RADIUS, Cpot4, Cpot4, 1, Spot4);
    pot4->setId(3);
    
	pot1->addToCollisionScene(stage.scene);
	pot2->addToCollisionScene(stage.scene);
	pot3->addToCollisionScene(stage.scene);
	pot4->addToCollisionScene(stage.scene);
	
	stage.pots.push_back(pot1);
	stage.pots.push_back(pot2);
	stage.pots.push_back(pot3);
	stage.pots.push_back(pot4);
    
	for (int i = 0; i < numPoppies + numDistractors; ++i) {
		Poppy* poppy = new Poppy(Vector3(randRangeDouble(-1.5, 1.5),0,randRangeDouble(1, 2)),
                                 BLAND_COLOR, BLAND_COLOR);
        poppy->setId(i);
		poppy->addToCollisionScene(stage.scene);
        poppy->deactivateJumpAtDest(true);
        
        if (stage.pots.size() > 0 && stage.poppies.size() < numPoppies)
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

void HerdPattern::reset()
{
    PopzorsPattern::reset();
    selected = NULL;
    
    numPoppies = 0;
    numDistractors = 0;
    numAnswered = 0;
    
    blinkLength = 0;
    blinkInterval = 0;
    timeSinceLastBlink = 0;
}

void HerdPattern::setPattern()
{
    reset();
    setup();
    
    //Pattern variables
    blinkLength = 1;
    blinkInterval = 3;
    
    for (int i = 0; i < stage.poppies.size(); ++i)
        stage.poppies[i]->setTimeBlinkLength(blinkLength);
    
    stage.ground->setTimeBlinkLength(blinkLength);
}

bool HerdPattern::isFinished() const
{
    return numAnswered >= numPoppies;
}

// This function calculates the maximum traversal for a given poppy based on the users mouse input
// returns destination vector
Vector3 findDragTraversal(Poppy * poppy, ClickedResult res)
{
    Vector3 mouseGroundPos = Vector3(res.ray.position.x, 0, res.ray.position.z);
    Vector3 popPos = poppy->getPosition();
    popPos.y = 0;
    
    Vector3 dir = mouseGroundPos - popPos;
    dir.Normalize();
    
    dir = dir * 1.2;
    
    return popPos + dir;
}

void HerdPattern::processSelect(ClickedResult res)
{
    if (selected && res.eventCode == InputEvent::EVENT_MOUSEUP
                 && selected->getType() == Selectable::TYPE_POPPY)
    {
        Poppy * toMove = (Poppy*)selected;
        toMove->setDest(findDragTraversal(toMove, res));
        toMove->setMoving(true);
        toMove->activateJump();
        selected = NULL;
    }
    
    if (res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
        if (res.poppy) {
            if ( (selected != res.poppy) && (selected != NULL) )
                selected = NULL;
            res.poppy->setColor(SELECT_COLOR);
            selected = res.poppy;
        }
    }
}

void HerdPattern::update(Number elapsed)
{
    updatePoppiesBlink(elapsed);
    updatePoppiesHerdState();
    
    for (int i = numPoppies; i < stage.poppies.size(); ++i) {
        stage.poppies[i]->setMoving(true);
        stage.poppies[i]->deactivateJump();
        Vector3 off = stage.poppies[i]->getPosition() - stage.poppies[i]->getDest();
        if (off.length() <= 0.1)
            stage.poppies[i]->setDest(stage.poppies[i]->getPosition() + Vector3(randRangeDouble(-1,1), 0, randRangeDouble(-1,1)));
    }
    
    for (int i = 0; i < stage.poppies.size(); ++i)
        stage.poppies[i]->update(elapsed);
    stage.ground->update(elapsed);
    
    stage.handlePoppyCollisions(elapsed);
    stage.handlePotCollisions(elapsed);
    
    if (isFinished() && !stage.ground->isBlinking()) {
        setPattern();
    }
}

void HerdPattern::updatePoppiesBlink(Number elapsed)
{
    timeSinceLastBlink += elapsed;
    if (timeSinceLastBlink >= blinkInterval) {
        for (int i = 0; i < stage.poppies.size(); ++i)
            stage.poppies[i]->activateBlink();
        timeSinceLastBlink = 0;
    }
}

void HerdPattern::updatePoppiesHerdState()
{
    for (int i = 0; i < stage.pots.size(); ++i) 
        for (int j = 0; j < stage.poppies.size(); ++j) {
                updatePlayerChoice(stage.poppies[j], stage.pots[i]);
        }
}

void HerdPattern::updatePlayerChoice(Poppy * poppy, Pot * pot)
{
    if (poppy->getMoving()) {
        Vector3 dist = pot->getPosition() - poppy->getPosition();
        if (dist.length() <= pot->getRadius()) {
            poppy->setPosition(pot->getPosition());
            poppy->deactivateJump();
            poppy->setMoving(false);
            ++numAnswered;
            
            if (pot->getBaseColor() == poppy->getBlinkColor())
                ++player.numCorrect;
        }
    }
    
    if (isFinished() && !stage.ground->isBlinking()) {
        if (player.numCorrect >= player.totalProblems) {
            stage.ground->setBlinkColor(FEEDBACK_COLOR_GOOD);
            ++player.level;
        } else {
            stage.ground->setBlinkColor(FEEDBACK_COLOR_BAD);
            if (player.level > 1) --player.level;
        }
        
        stage.ground->activateBlink();
    }
}
