#include "PoppyPattern.h"

#include <cstdlib>

PoppyPattern::PoppyPattern()
: signaled(false), signalStart(SIGNAL_START), signalLength(SIGNAL_LENGTH), timer(0), poppyOrderList(), backwardsOrder(false), blinkPoppyIndex(0), playerPoppyIndex(0), playerIsRight(true), playerLevel(1)
{
    
}

void PoppyPattern::reset()
{
    signaled = false;
    signalStart = SIGNAL_START;
    signalLength = SIGNAL_LENGTH;
    timer = 0;
    poppyOrderList.clear();
    backwardsOrder = false;
    blinkPoppyIndex = 0;
    playerPoppyIndex = 0;
    playerIsRight = true;
    ready = false;
}

void PoppyPattern::setPoppyPattern(CollisionScene *scene, Ground & ground, vector<Pot> & pots, vector<Poppy> & poppies)
{
    reset();
    
    for (int i = 0; i < poppies.size(); ++i)
        poppies[i].removeFromCollisionScene(scene);
    poppies.clear();
 
	//Make some poppies
    int numPoppies = playerLevel;;
	for (int i = 0; i < numPoppies; ++i) {
		Poppy dummy (Vector3(randRangeDouble(-1, 1),0,randRangeDouble(-1, 1)),
                     POPPY_COLOR, POPPY_COLOR);
        dummy.setId(i);
		dummy.addToCollisionScene(scene);
        
        if (pots.size() > 0)
        {
            int r = rand() % pots.size();
            dummy.setPotIdRef(pots[r].getId()); // Used tempoarily to play pot sound
            dummy.setBlinkColor(pots[r].getBaseColor());
        }
        
		poppies.push_back(dummy);
	}
    
    backwardsOrder = rand() % 2;
    if (poppies.size() > 1 && backwardsOrder)
    {
        for (int i = 0; i < pots.size(); ++i)
        {
            pots[i].setBlinkColor(Color(1.0, 0.0, 0.0, 1.0));
            pots[i].setTimeBlinkLength(signalStart / 2, 0.1, 0.1);
            pots[i].activateBlink();
        }
    }
    
    vector<int> tempIds(poppies.size());
    for (int i = 0; i < poppies.size(); ++i)
    {
        tempIds[i] = poppies[i].getId();
    }

    int count = 0;
    while (tempIds.size() > 0)
    {
        int r = rand() % tempIds.size();
        poppyOrderList.push_back(tempIds[r]);
        
        if (backwardsOrder)
        {
            poppies[tempIds[r]].setTimeBlinkLength(signalLength * (count + 1));
        }
        else
        {
            poppies[tempIds[r]].setTimeBlinkLength(signalLength * (poppies.size() - count));
        }
        
        tempIds[r] = tempIds[tempIds.size() - 1];
        tempIds.pop_back();
        
        ++count;
    }
    
    if (backwardsOrder && poppyOrderList.size() > 0)
        blinkPoppyIndex = (int)poppyOrderList.size() - 1;
}

void PoppyPattern::updatePlayerChoice(Ground & ground, Poppy & poppy, Pot & pot)
{
    if (pot.getBaseColor() != poppy.getBlinkColor() || poppy.getId() != poppyOrderList[playerPoppyIndex])
    {
        playerIsRight = false;
    }
    playerPoppyIndex++;
    if (isPlayerDone())
    {
        if (playerIsRight)
            playerLevel++;
        if (playerLevel <= 0)
            playerLevel = 1;
    }
    
    if (isPlayerDone())
    {
        if (isPlayerRight())
            ground.setBlinkColor(FEEDBACK_COLOR_GOOD);
        else
            ground.setBlinkColor(FEEDBACK_COLOR_BAD);
        ground.activateBlink();
    }
}

bool PoppyPattern::isPlayerDone()
{
    return playerPoppyIndex >= poppyOrderList.size();
}

bool PoppyPattern::isPlayerRight()
{
    return playerIsRight;
}

bool PoppyPattern::isReady()
{
    return ready;
}

void PoppyPattern::updatePoppyBlinks(Number elapsed, vector<Pot> & pots, vector<Poppy> & poppies)
{
    timer += elapsed;
	if (!signaled && timer > signalStart && poppyOrderList.size() > 0)
    {
        signaled = true;
        poppies[poppyOrderList[blinkPoppyIndex]].activateBlink();
        pots[poppies[poppyOrderList[blinkPoppyIndex]].getPotIdRef()].playSound();
        timer = 0;
    }
	if (signaled && blinkPoppyIndex >= 0 && blinkPoppyIndex < poppyOrderList.size() && timer >= signalLength) {
        if (backwardsOrder)
            blinkPoppyIndex--;
        else
            blinkPoppyIndex++;
        
        if (blinkPoppyIndex >= 0 && blinkPoppyIndex < poppyOrderList.size())
        {
            poppies[poppyOrderList[blinkPoppyIndex]].activateBlink();
            pots[poppies[poppyOrderList[blinkPoppyIndex]].getPotIdRef()].playSound();
        }
        else ready = true;
        timer = 0;
	}
}

void PoppyPattern::update(Number elapsed, Ground & ground, vector<Pot> & pots, vector<Poppy> & poppies)
{
    updatePoppyBlinks(elapsed, pots, poppies);        
}
