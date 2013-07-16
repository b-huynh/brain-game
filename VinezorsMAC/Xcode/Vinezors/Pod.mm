#include "Pod.h"

#include <cmath>
#include <cstdlib>

Pod::Pod()
	: stem(NULL), head(NULL)
{
}

Pod::Pod(CollisionScene *scene, PodType type, Vector3 base, Vector3 tip, Number stemRadius, Number headRadius)
    : type(type), stem(NULL), head(NULL), pastFog(false)
{
	Number stemLength = base.distance(tip);

	stem = new ScenePrimitive(ScenePrimitive::TYPE_CYLINDER, stemLength, stemRadius, 10);
	stem->setPosition((base + tip) / 2);
	stem->loadTexture("resources/green_solid.png");

	Number angle = std::atan2(base.y - tip.y, base.x - tip.x);
	stem->setRoll((angle + PI / 2) * 180 / PI);

	head = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, headRadius, 10, 10);
	head->setPosition(tip);

	switch (type)
	{
	case POD_BLUE:
		head->loadTexture("resources/blue_texture.png");
		break;
	case POD_GREEN:
		head->loadTexture("resources/green_texture.png");
		break;
	case POD_PINK:
		head->loadTexture("resources/pink_texture.png");
		break;
	case POD_YELLOW:
		head->loadTexture("resources/yellow_texture.png");
		break;
	case POD_BLACK:
		head->loadTexture("resources/grey_texture.png");
		break;
	default:
		head->loadTexture("resources/grey_texture.png");	
		break;
	}

	addToCollisionScene(scene);
}

PodType Pod::getType() const
{
	return type;
}

ScenePrimitive *Pod::getStem() const
{
	return stem;
}

ScenePrimitive *Pod::getHead() const
{
	return head;
}

void Pod::move(Vector3 delta)
{
	stem->Translate(delta);
	head->Translate(delta);
}

void Pod::hidePod()
{
	head->enabled = false;
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
	delete stem; stem = NULL;
	delete head; head = NULL;
}

PodType Pod::getPodType()
{
    return type;
}