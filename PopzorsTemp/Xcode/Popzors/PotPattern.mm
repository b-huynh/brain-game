#include "PotPattern.h"

#include <algorithm>
#include <cstdlib>

bool comparePotX(Pot* lhs, Pot* rhs)
{
    return lhs->getPosition().x < rhs->getPosition().x;
}

PotPattern::PotPattern(Screen *screen, CollisionScene *scene)
: PopzorsPattern(screen, scene), signaled(false), signalStart(SIGNAL_START), signalLength(SIGNAL_LENGTH), timer(0), numPots(0), playerNumAnswers(0), blinkPotIndex(0), spawnPoppyTimer(0), usefulPotIndex(0), selected(NULL)
{
}

void PotPattern::setup()
{
    stage.ground = new Ground(Vector3(0, -0.5, 0), GROUND_COLOR, GROUND_COLOR, SIGNAL_LENGTH);
	stage.ground->addToCollisionScene(stage.scene);
    
    const Number POT_RADIUS = 0.5;
    playerTotalProblems = playerLevel;
    numPots = playerLevel * 2;
    
    for (int i = 0; i < numPots; ++i)
    {
        Color col = getRandomPotColor();
        Pot* pot = new Pot(Vector3(randRangeDouble(-2, 2), 0.0, randRangeDouble(-2, 2)), POT_RADIUS,
                BLAND_COLOR, col, 1, getSoundAccordingToColor(col));
        pot->addToCollisionScene(stage.scene);
        pot->setId(i);
        stage.pots.push_back(pot);
    }
    sort(stage.pots.begin(), stage.pots.end(), comparePotX);
}

void PotPattern::reset()
{
    PopzorsPattern::reset();
    signaled = false;
    signalStart = SIGNAL_START;
    signalLength = SIGNAL_LENGTH;
    timer = 0;
    numPots = 0;
    playerNumAnswers = 0;
    blinkPotIndex = 0;
    spawnPoppyTimer = 0;
    usefulPotIndex = 0;
    selected = NULL;
}

void PotPattern::setPattern()
{
    reset();
    setup();

    for (int i = 0; i < stage.pots.size(); ++i)
        stage.pots[i]->setTimeBlinkLength(signalLength * (numPots - i));
}

bool PotPattern::isFinished() const
{
    return playerNumAnswers >= playerTotalProblems;
}

void PotPattern::processSelect(ClickedResult res)
{
    if (res.poppy) {
        
        if (res.poppy->isSelectable())
        {
            //deselect
            if ( (selected = res.poppy) && (selected != NULL) )
                selected = NULL;
            res.poppy->setColor(SELECT_COLOR);
            selected = res.poppy;
        }
    }
    
    if (res.pot) {
        
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
    if (ready && usefulPotIndex < playerTotalProblems)
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
        setPattern();
}

void PotPattern::updatePlayerChoice(Poppy* poppy, Pot* pot)
{
    if (pot->getBlinkColor() == poppy->getBlinkColor())
    {
        playerNumCorrect++;
    }
    playerNumAnswers++;
    if (isFinished())
    {
        if (playerNumCorrect >= playerTotalProblems)
        {
            stage.ground->setBlinkColor(FEEDBACK_COLOR_GOOD);
            playerLevel++;
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
        }
        stage.ground->activateBlink();
    }
}

void PotPattern::updatePotBlinks(Number elapsed)
{
    timer += elapsed;
	if (!signaled && timer > signalStart)
    {
        signaled = true;
        stage.pots[blinkPotIndex]->activateBlink();
        stage.pots[blinkPotIndex]->playSound();
        timer = 0;
    }
	if (signaled && blinkPotIndex >= 0 && blinkPotIndex < numPots && timer >= signalLength) {
        blinkPotIndex++;
        
        if (blinkPotIndex >= 0 && blinkPotIndex < numPots)
        {
            stage.pots[blinkPotIndex]->activateBlink();
            stage.pots[blinkPotIndex]->playSound();
        }
        else ready = true;
        timer = 0;
	}
}

void PotPattern::addPoppy()
{
    int r = rand() % (unsigned)(usefulPotIndex + 2);
    Color potColor = getRandomPotColor();
    if (r <= usefulPotIndex)
    {
        potColor = stage.pots[usefulPotIndex]->getBlinkColor();
        ++usefulPotIndex;
    }
        
    Poppy* poppy = new Poppy(Vector3(randRangeDouble(-6, 6),0,randRangeDouble(4, 6)), potColor, potColor);
    poppy->setId(stage.poppies.size());
    poppy->setMoving(true);
    poppy->setDest(Vector3(randRangeDouble(-2, 2),0,randRangeDouble(3, 4)));
    poppy->addToCollisionScene(stage.scene);
    poppy->activateJump();
    stage.poppies.push_back(poppy);
}
