#include "Vine.h"

Vine::Vine()
	: tip(NULL), dest()
{}

Vine::Vine(CollisionScene *scene, Vector3 pos, double radius)
	: tip(NULL), dest(), radius(radius)
{
	tip = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, radius, 10, 10);
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

double Vine::getRadius() const
{
    return radius;
}

void Vine::update(Number elapsed)
{
	const Number VINE_SPEED = 30.0;

	Vector3 dist = dest - tip->getPosition();
    
	if (dist.length() < 0.1)
	{
		dist = Vector3(0,0,0);
	}
	Vector3 norm = dist;
	norm.Normalize();
	Vector3 delta = norm * VINE_SPEED * elapsed;
	if (delta.length() > dist.length())
		delta = dist;
    
	move(delta);
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