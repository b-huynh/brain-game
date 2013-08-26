#include "HerdPattern.h"

#include <iostream>

HerdPattern::HerdPattern(Screen * screen, CollisionScene * scene, unsigned seed)
:PopzorsPattern(screen, scene, seed), selected(NULL)
{}

void HerdPattern::setup()
{
    stage.ground = new Ground(Vector3(0, -0.5, 0), GROUND_COLOR, GROUND_COLOR, SIGNAL_LENGTH);
	stage.ground->addToCollisionScene(stage.scene);
    
    //Stage variables
    numPoppies = player.level * 2;
    numDistractors = 3;
    player.totalProblems = numPoppies;
    
	//Make some pots
    const Number POT_RADIUS = 0.75;
	Pot* pot1 = new Pot(Vector3(randRangeDouble(-3, -3), 0.0, randRangeDouble(-1, -1)), POT_RADIUS, Cpot1, Cpot1, 1, Spot1);
    pot1->setId(0);
    Pot* pot2 = new Pot(Vector3(randRangeDouble(3, 3), 0.0, randRangeDouble(-3, -3)), POT_RADIUS, Cpot2, Cpot2, 1, Spot2);
    pot2->setId(1);
    Pot* pot3 = new Pot(Vector3(randRangeDouble(3, 3), 0.0, randRangeDouble(-1, -1)), POT_RADIUS, Cpot3, Cpot3, 1, Spot3);
    pot3->setId(2);
    Pot* pot4 = new Pot(Vector3(randRangeDouble(-3, -3), 0.0, randRangeDouble(-3, -3)), POT_RADIUS, Cpot4, Cpot4, 1, Spot4);
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
        poppy->setMoving(true);
        poppy->setMoveSpeed(2);
        poppy->activateJump();
        poppy->setDest(Vector3(poppy->getPosition().x, poppy->getPosition().y,-7));
        
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
    if (selected && res.eventCode == InputEvent::EVENT_MOUSEMOVE)
    {
        if (selected->getType() == Selectable::TYPE_POPPY) {
            Poppy * toMove = (Poppy*)selected;
            toMove->setPosition(Vector3(res.ray.position.x, 0,res.ray.position.z));
        }
        else if (selected->getType() == Selectable::TYPE_POT) {
            Pot * toMove = (Pot*)selected;
            toMove->setPosition(Vector3(res.ray.position.x, 0,res.ray.position.z));
        }
    }
    
    if (res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
        if (selected) 
            selected = NULL;
        else if (res.poppy) {
            res.poppy->setColor(SELECT_COLOR);
            selected = res.poppy;
        }
        else if (res.pot) {
            selected = res.pot;
        }
    }
}

void HerdPattern::update(Number elapsed)
{
    updatePoppiesBlink(elapsed);
    updatePoppiesHerdState();
    
    for (int i = 0; i < stage.poppies.size(); ++i) {
        Vector3 dest = stage.poppies[i]->getDest();
        Vector3 dist = stage.poppies[i]->getPosition() - dest;
        if (dist.length() <= 0.2)
            stage.poppies[i]->setPosition(dest.x,dest.y,7);
        stage.poppies[i]->update(elapsed);
    }
    stage.ground->update(elapsed);
    
    //poppies handle collision code with exception for selected poppy
    for (int i = 0; i < stage.poppies.size(); ++i)
        for (int j = i + 1; j < stage.poppies.size(); ++j)
            //if (selected != stage.poppies[i])
                stage.poppies[i]->handleCollision(elapsed, stage.scene, stage.poppies[j]);
    
    stage.handlePotCollisions(elapsed);
    
    if (isFinished() && !stage.ground->isBlinking()) {
        setPattern();
    }
}

void HerdPattern::updatePoppiesBlink(Number elapsed)
{
    timeSinceLastBlink += elapsed;
    if (timeSinceLastBlink >= blinkInterval && timeSinceLastBlink <= blinkInterval + blinkLength) {
        for (int i = 0; i < stage.poppies.size(); ++i)
            stage.poppies[i]->activateBlink();
    }
}

void HerdPattern::updatePoppiesHerdState()
{
    for (int i = 0; i < stage.pots.size(); ++i) 
        for (int j = 0; j < stage.poppies.size(); ++j)
                updatePlayerChoice(stage.poppies[j], stage.pots[i]);
}

void HerdPattern::updatePlayerChoice(Poppy * poppy, Pot * pot)
{
    if (selected != poppy) {
        Vector3 dist = pot->getPosition() - poppy->getPosition();
        if (dist.length() <= pot->getRadius()) {
            if (poppy->getMoving()) {
                ++numAnswered;
                if (pot->getBaseColor() == poppy->getBlinkColor()) {
                    ++player.numCorrect;
                }
            }
            
            poppy->setMoving(false);
            poppy->setPosition(pot->getPosition());
            poppy->deactivateJump();
        }
    }
    
    if (isFinished() && !stage.ground->isBlinking()) {
        if (player.numCorrect >= player.totalProblems) {
            stage.ground->setBlinkColor(FEEDBACK_COLOR_GOOD);
            player.updateLevel(PLAYER_SUCCESS);
        } else {
            stage.ground->setBlinkColor(FEEDBACK_COLOR_BAD);
            player.updateLevel(PLAYER_FAILURE);
        }
        
        stage.ground->activateBlink();
    }
}
