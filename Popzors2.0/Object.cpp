//
//  Object.cpp
//  Popzors2.0
//
//  Created by Calvin Phung on 10/10/13.
//
//

#include "Object.h"

int Object::BlinkableObjCount = 0;

Object::Object() : type(TYPE_NONE), pos(), mainNode(NULL), body(NULL), currentColor(), baseColor(), blinkColor(), timeBlinkLength(0), timeBlinked(0), ontime(0), offtime(0), toggle(false), timeToggle(0), sceneID(BlinkableObjCount++)
{
}

Object::Object(int type, Vector3 pos, Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime)
    : type(type), pos(pos), mainNode(mainNode), body(body), currentColor(baseColor), baseColor(baseColor), blinkColor(blinkColor), timeBlinkLength(blinktime), timeBlinked(0), ontime(blinktime), offtime(0), toggle(false), timeToggle(0), sceneID(BlinkableObjCount++)
{
}

Object::Object(int type, Vector3 pos, Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime, double ontime, double offtime)
: type(type), pos(pos), mainNode(mainNode), body(body), currentColor(baseColor), baseColor(baseColor), blinkColor(blinkColor), timeBlinkLength(blinktime), timeBlinked(0), ontime(ontime), offtime(offtime), toggle(false), timeToggle(0), sceneID(BlinkableObjCount++)
{
}

int Object::getType() const
{
    return type;
}

SceneNode* Object::getNode() const
{
    return mainNode;
}

Entity* Object::getBody() const
{
    return body;
}

Vector3 Object::getPosition() const
{
    return mainNode->getPosition();
}

void Object::move(const Vector3 & dValue)
{
    mainNode->translate(dValue);
}

bool Object::hasEntity(Entity* entity)
{
	if (body == entity) return true;
	return false;
}

void Object::setPosition(double x, double y, double z)
{
    mainNode->setPosition(x, y, z);
}

void Object::setPosition(Vector3 vec)
{
    mainNode->setPosition(vec);
}

void Object::setColor(Ogre::ColourValue color)
{
    if (currentColor != color)
        body->setMaterialName(getTextureNameByColor(color));
    currentColor = color;
}

void Object::setColor(int r, int g, int b, int a)
{
    Ogre::ColourValue color = Ogre::ColourValue(r, g, b, a);
    if (currentColor != color)
        body->setMaterialName(getTextureNameByColor(color));
    currentColor = color;
}

void Object::setBaseColor(int r, int g, int b, int a)
{
    baseColor = intToFloatColor(r, g, b, a);
}

void Object::setBaseColor(Ogre::ColourValue color)
{
    baseColor = color;
}

void Object::setBlinkColor(int r, int g, int b, int a)
{
    blinkColor = intToFloatColor(r, g, b, a);
}

void Object::setBlinkColor(Ogre::ColourValue color)
{
    blinkColor = color;
}

void Object::activateBlink()
{
    timeBlinked = timeBlinkLength;
    toggle = true;
    timeToggle = ontime;
}

void Object::setTimeBlinkLength(double blinktime)
{
    timeBlinkLength = blinktime;
    this->ontime = blinktime;
    this->offtime = 0;
}

void Object::setTimeBlinkLength(double blinktime, double ontime, double offtime)
{
    timeBlinkLength = blinktime;
    this->ontime = ontime;
    this->offtime = offtime;
}

void Object::updateBlinkTime(double elapsed)
{
	if (timeBlinked > 0)
		timeBlinked -= elapsed;
	if (timeToggle > 0)
		timeToggle -= elapsed;
}

void Object::reset()
{
    timeBlinked = 0;
    setColor(baseColor);
    setPosition(pos);
    toggle = false;
    timeToggle = 0;
}

void Object::update(double elapsed)
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
            this->setMaterialByColor(blinkColor);
        else
            this->setMaterialByColor(baseColor);
    }
    else
        this->setMaterialByColor(baseColor);
}

Object::~Object()
{}