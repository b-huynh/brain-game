#include "Pot.h"

Pot::Pot(Vector3 pos)
	:pos(pos)
{
	body = new ScenePrimitive(ScenePrimitive::TYPE_UNCAPPED_CYLINDER, 0.3, 1.0, 100.0);
}

bool Pot::hasEntity(SceneEntity * entity)
{
	return body == entity;
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
	pos = Vector3(x,y,z);
	body->setPosition(x,y,z);
}

void Pot::setPosition(Vector3 vec)
{
	this->pos = vec;
	body->setPosition(vec);
}

void Pot::addToCollisionScene(CollisionScene * scene)
{
	scene->addCollisionChild(body,CollisionSceneEntity::SHAPE_CYLINDER);
}