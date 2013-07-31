#include "Pod.h"

#include <cmath>
#include <cstdlib>

#include <iostream>
using namespace std;

Pod::Pod()
	: stem(NULL), head(NULL)
{
}

Pod::Pod(CollisionScene *scene, Vector3 center, Quaternion rot, PodType type, Vector3 base, Vector3 tip, Number stemRadius, Number headRadius)
    : type(type), stem(NULL), head(NULL), shell(NULL), pastFog(false)
{
	Number stemLength = base.distance(tip);

	stem = new ScenePrimitive(ScenePrimitive::TYPE_CYLINDER, stemLength, stemRadius, 10);
	stem->setPosition((base + tip) / 2);
	stem->loadTexture("resources/green_solid.png");
    double angle;
    Vector3 v = base - tip;
    v = (rot.Inverse()).applyTo(v); // Reverse to old coordinates to find x-y angle
    angle = (std::atan2(v.y, v.x) + PI / 2) * (180.0 / PI);
    Quaternion q;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, angle);
    q = q.Inverse();
    q = rot * q;
    stem->setRotationQuat(q.w, q.x, q.y, q.z);
    
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

/*
Quaternion Pod::getTunnelQuaternion(Vector3 center, Vector3 forward)
{
    Quaternion rot;
    Vector3 axis = forward.crossProduct(TUNNEL_REFERENCE_FORWARD);
    if (axis == Vector3(0,0,0)) axis = TUNNEL_REFERENCE_FORWARD;
    axis.Normalize();
    Number angle = forward.angleBetween(TUNNEL_REFERENCE_FORWARD) * (180.0/ PI);
    //if (axis.dot(Vector3(1,1,1)) >= 0) angle += 180;
    rot.createFromAxisAngle(axis.x, axis.y, axis.z, angle);
    //if (angle <= 180)
        rot = rot.Inverse();
    return rot;
}
 */

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