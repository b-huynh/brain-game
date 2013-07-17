#include "Blinkable.h"

Blinkable::Blinkable(Color baseColor, Color blinkColor, Number blinktime)
: baseColor(baseColor), blinkColor(blinkColor), timeBlinkLength(blinktime), timeBlinked(0), ontime(blinktime), offtime(0), toggle(false), timeToggle(0)
{
}

Blinkable::Blinkable(Color baseColor, Color blinkColor, Number blinktime, Number ontime, Number offtime)
    : baseColor(baseColor), blinkColor(blinkColor), timeBlinkLength(blinktime), timeBlinked(0), ontime(ontime), offtime(offtime), toggle(false), timeToggle(0)
{
}

void Blinkable::activateBlink()
{
    timeBlinked = timeBlinkLength;
    toggle = true;
    timeToggle = ontime;
}

void Blinkable::setTimeBlinkLength(Number blinktime)
{
    timeBlinkLength = blinktime;
    this->ontime = blinktime;
    this->offtime = 0;
}

void Blinkable::setTimeBlinkLength(Number blinktime, Number ontime, Number offtime)
{
    timeBlinkLength = blinktime;
    this->ontime = ontime;
    this->offtime = offtime;
}

void Blinkable::setBaseColor(int r, int g, int b, int a)
{
    blinkColor = Color(r, g, b, a);
}

void Blinkable::setBaseColor(Color color)
{
    blinkColor = color;
}

void Blinkable::setBlinkColor(int r, int g, int b, int a)
{
    blinkColor = Color(r, g, b, a);
}

void Blinkable::setBlinkColor(Color color)
{
    blinkColor = color;
}

void Blinkable::updateBlinkTime(Number elapsed)
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

void Blinkable::update(Number elapsed)
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
