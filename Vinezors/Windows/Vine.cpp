#include "Vine.h"

Vine::Vine()
	: tip(NULL), dest()
{}

Vine::Vine(CollisionScene *scene, Vector3 pos, Number length, Number radius)
	: tip(NULL), dest()
{
	tip = new ScenePrimitive(ScenePrimitive::TYPE_CONE, length, radius, 10);
	tip->setPitch(-45);
	tip->setPosition(pos);
	//tip->setMaterialByName("VineMaterial");
	tip->loadTexture("resources/green_solid.png");

	addToCollisionScene(scene);
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