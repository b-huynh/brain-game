#include "PotPattern.h"

#include <algorithm>
#include <cstdlib>

bool comparePotX(Pot* lhs, Pot* rhs)
{
    return lhs->getPosition().x < rhs->getPosition().x;
}

PotPattern::PotPattern(Screen *screen, CollisionScene *scene)
: PopzorsPattern(screen, scene), signaled(false), signalStart(SIGNAL_START), signalLength(SIGNAL_LENGTH), timer(0), stages(0), stageIndex(0), potsPerStage(0), potsLeft(), playerNumAnswers(0), blinkPotIndex(0), spawnPoppyTimer(0), usefulPotIndex(0), selected(NULL)
{
}

void PotPattern::setup()
{
    stage.ground = new Ground(Vector3(0, -0.5, 0), GROUND_COLOR, GROUND_COLOR, SIGNAL_LENGTH);
	stage.ground->addToCollisionScene(stage.scene);
    
    const Number POT_RADIUS = 0.5;
    stages = 3;
    potsPerStage = player.level + 2;
    player.totalProblems = stages * potsPerStage;
    
    for (int i = 0; i < stages * potsPerStage; ++i)
    {
        Pot* pot = new Pot(Vector3(randRangeDouble(-2, 2), 0.0, randRangeDouble(-2, 2)), POT_RADIUS,
                           BASE_COLOR, BLAND_COLOR, 1, getSoundAccordingToColor(BLAND_COLOR));
        pot->addToCollisionScene(stage.scene);
        pot->setId(i);
        stage.pots.push_back(pot);
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

void PotPattern::reset()
{
    PopzorsPattern::reset();
    signaled = false;
    signalStart = SIGNAL_START;
    signalLength = SIGNAL_LENGTH;
    stages = 0;
    stageIndex = 0;
    timer = 0;
    potsLeft.clear();
    playerNumAnswers = 0;
    blinkPotIndex = 0;
    spawnPoppyTimer = 0;
    usefulPotIndex = 0;
    selected = NULL;
    
}

void PotPattern::setPattern()
{
    if (stageIndex <= 0) {
        reset();
        setup();
    }
    for (int i = stageIndex * potsPerStage; i < stageIndex * potsPerStage + potsPerStage; ++i) {
        if (stage.pots[i]->getBlinkColor() == BLAND_COLOR) {
            Color col = getRandomPotColor();
            stage.pots[i]->setBlinkColor(col);
            stage.pots[i]->setSound(getSoundAccordingToColor(col));
        }
    }
    
    vector<Pot*> temp;
    for (int i = stageIndex * potsPerStage; i < stage.pots.size(); ++i)
        temp.push_back(stage.pots[i]);
    while (temp.size() > 0)
    {
        int r = rand() % temp.size();
        potsLeft.push_back(temp[r]);
        temp[r] = temp[temp.size() - 1];
        temp.pop_back();
    }
    for (int i = 0; i < potsLeft.size(); ++i)
        potsLeft[i]->setTimeBlinkLength(signalLength * (stage.pots.size() - i));
}

bool PotPattern::isFinished() const
{
    return playerNumAnswers >= player.totalProblems;
}

void PotPattern::processSelect(ClickedResult res)
{
    if (res.poppy && res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
        
        if (res.poppy->isSelectable())
        {
            //deselect
            if ( (selected = res.poppy) && (selected != NULL) )
                selected = NULL;
            res.poppy->setColor(SELECT_COLOR);
            selected = res.poppy;
        }
    }
    
    if (res.pot && res.eventCode == InputEvent::EVENT_MOUSEDOWN) {
        
        if (res.pot->isSelectable())
        {
            if (selected && selected != res.pot && isReady())
                if (selected->getType() == Selectable::TYPE_POPPY)
                {
                    Poppy * old = (Poppy*)selected;
                    Vector3 potpos = res.pot->getPosition();
                    old->setPosition(potpos.x, potpos.y, potpos.z);
                    old->setSelectable(false);
                    old->deactivateJump();
                    old->setMoving(false);
                    res.pot->setSelectable(false);
                    selected = NULL;
                    
                    updatePlayerChoice(old, res.pot);
                }
        }
    }
}

void PotPattern::update(Number elapsed)
{
    if (ready && usefulPotIndex < potsPerStage && stageIndex < stages)
    {
        spawnPoppyTimer += elapsed;
        const Number SPAWN_RATE = 1.0;
        if (spawnPoppyTimer >= SPAWN_RATE) {
            addPoppy();
            spawnPoppyTimer = 0;
        }
    }
    updatePotBlinks(elapsed);
    
    stage.ground->update(elapsed);
    for (int i = 0; i < stage.pots.size(); ++i)
        stage.pots[i]->update(elapsed);
    for (int i = 0; i < stage.poppies.size(); ++i)
    {
        stage.poppies[i]->update(elapsed);
        
        // If poppy is at destination, deactivate jump animation
        Vector3 dist = stage.poppies[i]->getDest() - stage.poppies[i]->getPosition();
        if (dist.length() <= 0.1)
        {
            stage.poppies[i]->setMoving(false);
            stage.poppies[i]->deactivateJump();
        }
    }
    
    stage.handlePoppyCollisions(elapsed);
    stage.handlePotCollisions(elapsed);
    
    if (isFinished() && !stage.ground->isBlinking())
    {
        // Done with all stages, reset to new level
        stageIndex = 0;
        setPattern();
    }
}

void PotPattern::updatePlayerChoice(Poppy* poppy, Pot* pot)
{
    if (pot->getBlinkColor() != BLAND_COLOR && pot->getBlinkColor() == poppy->getBlinkColor())
    {
        player.numCorrect++;
    }
    printf("%d %d\n", player.numCorrect, player.totalProblems);
    playerNumAnswers++;
    if (playerNumAnswers % potsPerStage == 0)
    {
        // Reorganize the pots in the right order to refer to the next problem set
        vector<Pot*> temp;
        for (int i = 0; i < stage.pots.size(); ++i)
            if (!stage.pots[i]->isSelectable()) {
                stage.pots[i]->setBaseColor(stage.pots[i]->getBlinkColor());
                temp.push_back(stage.pots[i]);
            }
        for (int i = 0; i < stage.pots.size(); ++i)
            if (stage.pots[i]->isSelectable() && stage.pots[i]->getBlinkColor() != BLAND_COLOR)
                temp.push_back(stage.pots[i]);
        for (int i = 0; i < stage.pots.size(); ++i)
            if (stage.pots[i]->isSelectable() && stage.pots[i]->getBlinkColor() == BLAND_COLOR)
                temp.push_back(stage.pots[i]);
        stage.pots = temp;
        
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
        else
        {
            ready = false;
            signaled = false;
            timer = 0;
            potsLeft.clear();
            blinkPotIndex = 0;
            spawnPoppyTimer = 0;
            ++stageIndex;
            usefulPotIndex = 0;
            setPattern();
        }
    }
}

void PotPattern::updatePotBlinks(Number elapsed)
{
    timer += elapsed;
	if (!signaled && timer > signalStart)
    {
        signaled = true;
        potsLeft[blinkPotIndex]->activateBlink();
        potsLeft[blinkPotIndex]->playSound();
        timer = 0;
    }
	if (signaled && blinkPotIndex >= 0 && blinkPotIndex < potsLeft.size() && timer >= signalLength) {
        blinkPotIndex++;
        
        if (blinkPotIndex >= 0 && blinkPotIndex < potsLeft.size())
        {
            potsLeft[blinkPotIndex]->activateBlink();
            potsLeft[blinkPotIndex]->playSound();
        }
        else ready = true;
        timer = 0;
	}
}

void PotPattern::addPoppy()
{
    int r = rand() % (unsigned)(usefulPotIndex + 2);
    Color potColor = getRandomPotColor();
    
    if (r <= usefulPotIndex) {
        potColor = stage.pots[stageIndex * potsPerStage + usefulPotIndex]->getBlinkColor();
        ++usefulPotIndex;
    }
    
    Poppy* poppy = new Poppy(Vector3(randRangeDouble(-6, 6),0,randRangeDouble(4, 4)), potColor, potColor);
    poppy->setId(stage.poppies.size());
    poppy->setMoving(true);
    poppy->setDest(Vector3(randRangeDouble(-3, 3),0,randRangeDouble(3, 3)));
    poppy->addToCollisionScene(stage.scene);
    poppy->activateJump();
    stage.poppies.push_back(poppy);
}
