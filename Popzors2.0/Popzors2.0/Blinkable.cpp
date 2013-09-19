#include "Blinkable.h"

int Blinkable::BlinkableObjCount = 0;

Blinkable::Blinkable(Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime)
: currentColor(baseColor), baseColor(baseColor), blinkColor(blinkColor), timeBlinkLength(blinktime), timeBlinked(0), ontime(blinktime), offtime(0), toggle(false), timeToggle(0), sceneID(BlinkableObjCount++)
{
}

Blinkable::Blinkable(Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime, double ontime, double offtime)
    : currentColor(baseColor), baseColor(baseColor), blinkColor(blinkColor), timeBlinkLength(blinktime), timeBlinked(0), ontime(ontime), offtime(offtime), toggle(false), timeToggle(0), sceneID(BlinkableObjCount++)
{
}

void Blinkable::activateBlink()
{
    timeBlinked = timeBlinkLength;
    toggle = true;
    timeToggle = ontime;
}

void Blinkable::setTimeBlinkLength(double blinktime)
{
    timeBlinkLength = blinktime;
    this->ontime = blinktime;
    this->offtime = 0;
}

void Blinkable::setTimeBlinkLength(double blinktime, double ontime, double offtime)
{
    timeBlinkLength = blinktime;
    this->ontime = ontime;
    this->offtime = offtime;
}

void Blinkable::setBaseColor(int r, int g, int b, int a)
{
    baseColor = intToFloatColor(r, g, b, a);
}

void Blinkable::setBaseColor(Ogre::ColourValue color)
{
    baseColor = color;
}

void Blinkable::setBlinkColor(int r, int g, int b, int a)
{
    blinkColor = intToFloatColor(r, g, b, a);
}

void Blinkable::setBlinkColor(Ogre::ColourValue color)
{
    blinkColor = color;
}

void Blinkable::updateBlinkTime(double elapsed)
{
	if (timeBlinked > 0)
		timeBlinked -= elapsed;
	if (timeToggle > 0)
		timeToggle -= elapsed;
}

void Blinkable::reset()
{
    timeBlinked = 0;
    this->setColor(baseColor);
    toggle = false;
    timeToggle = 0;
}

void Blinkable::update(double elapsed)
{
    updateBlinkTime(elapsed);
    if (isBlinking())
    {
        if (!isToggling())
        {
            toggle = !toggle;
            timeToggle = toggle ? ontime : offtime;
        }
        if (toggle)
            this->setColor(blinkColor);
        else
            this->setColor(baseColor);
    }
    else
        this->setColor(baseColor);
}
