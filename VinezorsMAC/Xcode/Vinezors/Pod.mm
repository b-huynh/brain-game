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
    : type(type), base(base), tip(tip), stemRadius(stemRadius), headRadius(headRadius), stem(NULL), head(NULL), shell(NULL), pastFog(false)
{
	Number stemLength = base.distance(tip);
    
    stem = new ScenePrimitive(ScenePrimitive::TYPE_CYLINDER, stemLength, stemRadius, 10);
    //stem->loadTexture("resources/green_solid.png");
    //stem->setMaterialByName("PodUnknownMat");
    stem->setColor(0.5, 0.5, 0.5, 1.0);
    double angle;
    Vector3 v = base - tip;
    v = (rot.Inverse()).applyTo(v); // Reverse to old coordinates to find x-y angle
    angle = (std::atan2(v.y, v.x) + PI / 2) * (180.0 / PI);
    Quaternion q;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z,angle);
    q = q.Inverse();
    q = rot * q;
    stem->setRotationQuat(q.w, q.x, q.y, q.z);
    
	head = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, headRadius, 10, 10);
    //head->enabled = false;
    
    shell = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, headRadius + 0.1, 10, 10);
    shell->setColor(1.0 ,0.0, 0.0, 0.1);
    shell->enabled = false;
    
    //head->loadTexture("resources/black_solid.png");
    head->setColor(0.0, 0.0, 0.0, 1.0);
    //head->setMaterialByName("PodUnknownMat");
    setToGrowth(0.0);
    
    /*
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
    case POD_BROWN:
        head->loadTexture("resources/brown_solid.png");
        break;
	default:
		head->loadTexture("resources/black_solid.png");	
		break;
	}
    */
    
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

void Pod::setToGrowth(double t)
{
    Vector3 baseToTip = tip - base;
    double totalLength = baseToTip.length();
    baseToTip.Normalize();
    
    double stemLength = totalLength;
    Vector3 nBase = base + baseToTip * (-stemLength * (1 - t));
    Vector3 nTip = nBase + baseToTip * (stemLength);
    
    stem->setPosition((nBase + nTip) / 2);
	head->setPosition(nTip);
    shell->setPosition(nTip);
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

void Pod::showPod()
{
    head->enabled = true;
	switch (type)
	{
        case POD_BLUE:
            //head->loadTexture("resources/blue_fixed.png");
            head->setColor(0.0, 0.0, 1.0, 1.0);
            break;
        case POD_GREEN:
            //head->loadTexture("resources/green_fixed.png");
            head->setColor(0.0, 1.0, 0.0, 1.0);
            break;
        case POD_PINK:
            //head->loadTexture("resources/red_fixed.png");
            head->setColor(1.0, 0.0, 0.0, 1.0);
            break;
        case POD_YELLOW:
            //head->loadTexture("resources/yellow_fixed.png");
            head->setColor(1.0, 1.0, 0.0, 1.0);
            break;
        case POD_BLACK:
            //head->loadTexture("resources/black_solid.png");
            head->setColor(0.0, 0.0, 0.0, 1.0);
            break;
        default:
            //head->loadTexture("resources/black_solid.png");
            head->setColor(0.0, 0.0, 0.0, 1.0);
            break;
	}
    /*
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
}

void Pod::showShell()
{
    shell->enabled = true;
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