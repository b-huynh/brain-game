#include "Vine.h"

Vine::Vine()
	: tip(NULL), dest()
{}

Vine::Vine(CollisionScene *scene, Vector3 pos, Number length, Number radius)
	: tip(NULL), dest()
{
	tip = new ScenePrimitive(ScenePrimitive::TYPE_CONE, length, radius, 10);
	tip->setPitch(-90);
	tip->setPosition(pos);
	//tip->setMaterialByName("VineMaterial");
	tip->loadTexture("resources/green_solid.png");

	addToCollisionScene(scene);
}

ScenePrimitive *Vine::getTip() const
{
	return tip;
}

Vector3 Vine::getPos() const
{
	return tip->getPosition();
}

void Vine::setPos(Vector3 value)
{
	tip->setPosition(value);
}

Vector3 Vine::getDest() const
{
	return dest;
}

void Vine::setDest(Vector3 value)
{
	dest = value;
}

void Vine::move(Vector3 delta)
{
	tip->Translate(delta);
}

void Vine::update(Number elapsed)
{
	const Number VINE_SPEED = 100.0;

	Vector2 dist = Vector2(dest.x - tip->getPosition().x, dest.y - tip->getPosition().y);
	
	if (dist.length() < 0.1)
	{
		dist.x = 0;
		dist.y = 0;
	}
	Vector2 norm = dist;
	norm.Normalize();
	tip->setPitch(-90 + norm.y * 180 / PI);
	tip->setRoll(norm.x * 180 / PI);
	Vector2 move = norm * VINE_SPEED;
	if (move.length() > dist.length())
		move = dist;
	tip->Translate(Vector3(move.x, move.y, 0.0));
	
	//tip->Translate(Vector3(dist.x, dist.y, 0.0));
}

void Vine::addToCollisionScene(CollisionScene *scene)
{
	scene->addCollisionChild(tip, CollisionSceneEntity::SHAPE_CYLINDER);
}

void Vine::removeFromCollisionScene(CollisionScene * scene)
{
	scene->removeEntity(tip);
	delete tip;
}