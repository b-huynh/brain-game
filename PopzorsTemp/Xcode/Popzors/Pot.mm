#include "Pot.h"

Pot::Pot(Vector3 pos, Color baseColor, Color blinkColor, Number blinktime, const String & soundFile)
	: Selectable(baseColor, blinkColor, blinktime), potId(-1), pos(pos), sound(NULL)
{
	body = new ScenePrimitive(ScenePrimitive::TYPE_UNCAPPED_CYLINDER, 0.3, 1.0, 100.0);
    body->setPosition(pos);
    body->backfaceCulled = false;
    
    if (soundFile != "")
        sound = new Sound(soundFile);
}

bool Pot::hasEntity(SceneEntity * entity)
{
	return body == entity;
}

void Pot::setId(int val)
{
    this->potId = val;
}

void Pot::setColor(int r, int g, int b, int a)
{
	body->setColor(r,g,b,a);
}

void Pot::setColor(Color color)
{
	body->setColor(color);
}

void Pot::setPosition(Number x, Number y, Number z)
{
	body->setPosition(x,y,z);
}

void Pot::setPosition(Vector3 vec)
{
	body->setPosition(vec);
}

void Pot::addToCollisionScene(CollisionScene * scene)
{
	scene->addCollisionChild(body,CollisionSceneEntity::SHAPE_CYLINDER);
}

void Pot::playSound()
{
    sound->Play(false);
}
               
void Pot::reset()
{
    Blinkable::reset();
    Selectable::reset();
    body->setPosition(pos);
}

void Pot::update(Number elapsed)
{
    Blinkable::update(elapsed);
    Selectable::update(elapsed);
}