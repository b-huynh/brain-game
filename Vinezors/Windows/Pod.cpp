#include "Pod.h"

#include <cmath>

Pod::Pod()
	: stem(NULL), head(NULL)
{
}

Pod::Pod(CollisionScene *scene, Vector3 base, Vector3 tip, Number stemRadius, Number headRadius)
	: stem(NULL), head(NULL)
{
	Number stemLength = base.distance(tip);

	stem = new ScenePrimitive(ScenePrimitive::TYPE_CYLINDER, stemLength, stemRadius, 10);
	stem->setPosition((base + tip) / 2);
	stem->loadTexture("resources/green_solid.png");

	Number angle = std::atan2(base.y - tip.y, base.x - tip.x);
	stem->setRoll((angle + PI / 2) * 180 / PI);

	head = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, headRadius, 10, 10);
	head->setPosition(tip);
	head->loadTexture("resources/pink_texture.png");

	addToCollisionScene(scene);
}

void Pod::move(Vector3 delta)
{
	stem->Translate(delta);
	head->Translate(delta);
}

void Pod::addToCollisionScene(CollisionScene *scene)
{
	scene->addChild(stem);
	scene->addCollisionChild(head, CollisionSceneEntity::SHAPE_SPHERE);
}

void Pod::removeFromCollisionScene(CollisionScene * scene)
{
	scene->removeEntity(stem);
	scene->removeEntity(head);
	delete stem;
	delete head;	
}