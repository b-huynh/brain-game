#include "Poppy.h"

Poppy::Poppy(Vector3 pos, Color color, Color blinkColor, Number blinktime)
	:Selectable(), pos(pos), color(color), blinkColor(blinkColor), timeBlinked(blinktime)
{
	body = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 0.5, 0.5, 0.5);
	this->setColor(color);
	this->setPosition(pos);
}

Poppy::~Poppy()
{
}

void Poppy::setColor(int r, int g, int b, int a)
{
	body->setColor(r,g,b,a);
}

void Poppy::setColor(Color color)
{
	body->setColor(color);
}

void Poppy::setPosition(Number x, Number y, Number z)
{
	this->pos = Vector3(x,y,z);
	body->setPosition(x, y, z);
}

void Poppy::setPosition(Vector3 vec)
{
	this->pos = vec;
	body->setPosition(vec);
}

void Poppy::addToCollisionScene(CollisionScene * scene)
{
	scene->addCollisionChild(body, CollisionSceneEntity::SHAPE_BOX);
}

bool Poppy::hasEntity(SceneEntity * entity)
{
	if (body == entity) return true;
	return false;
}

bool Poppy::blink(Number elapsed)
{
	if (timeBlinked > 0) {
		timeBlinked -= elapsed;
		this->setColor(blinkColor);
	}
	return timeBlinked > 0;
}

void Poppy::unblink()
{
	this->setColor(color);
}