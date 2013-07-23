#include "Pod.h"

#include <cmath>
#include <cstdlib>

Pod::Pod()
	: stem(NULL), head(NULL)
{
}

Pod::Pod(CollisionScene *scene, PodType type, Vector3 base, Vector3 tip, Number stemRadius, Number headRadius)
    : type(type), stem(NULL), head(NULL), shell(NULL), pastFog(false)
{
	Number stemLength = base.distance(tip);

	stem = new ScenePrimitive(ScenePrimitive::TYPE_CYLINDER, stemLength, stemRadius, 10);
	stem->setPosition((base + tip) / 2);
	stem->loadTexture("resources/green_solid.png");

	Number angle = std::atan2(base.y - tip.y, base.x - tip.x);
	stem->setRoll((angle + PI / 2) * 180 / PI);

	head = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, headRadius, 10, 10);
	head->setPosition(tip);
    
    shell = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, headRadius + 0.1, 10, 10);
    shell->setColor(1.0 ,0.0, 0.0, 0.1);
    shell->setPosition(tip);
    
	switch (type)
	{
	case POD_BLUE:
		head->loadTexture("resources/blue_fixed.png");
		break;
	case POD_GREEN:
		head->loadTexture("resources/green_fixed.png");
		break;
	case POD_PINK:
		head->loadTexture("resources/red_solid.png");
		break;
	case POD_YELLOW:
		head->loadTexture("resources/yellow_solid.png");
		break;
	case POD_BLACK:
		head->loadTexture("resources/black_solid.png");
		break;
	default:
		head->loadTexture("resources/black_solid.png");	
		break;
	}
    
    /*
    //Testing Pod Material and Lighting
    switch (type)
	{
        case POD_BLUE:
            head->setMaterialByName("PodBlueMat");
            break;
        case POD_GREEN:
            head->setMaterialByName("PodGreenMat");
            break;
        case POD_PINK:
            head->setMaterialByName("PodPinkMat");
            break;
        case POD_YELLOW:
            head->setMaterialByName("PodYellowMat");
            break;
        case POD_BLACK:
            head->setMaterialByName("PodBlackMat");
            break;
        default:
            head->setMaterialByName("PodUnknownMat");
            break;
	}
     */
 
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
    scene->addCollisionChild(shell);
}

void Pod::removeFromCollisionScene(CollisionScene * scene)
{
	scene->removeEntity(stem);
	scene->removeEntity(head);
    scene->removeEntity(shell);
	delete stem; stem = NULL;
	delete head; head = NULL;
    delete shell; shell = NULL;
}

PodType Pod::getPodType()
{
    return type;
}

bool Pod::getPastFog()
{
    return pastFog;
}
void Pod::setPastFog(bool past)
{
    pastFog = past;
}