//
//  Vine.cpp
//  Testing
//
//  Created by Calvin Phung on 9/13/13.
//
//
#include "Vine.h"

static int vineID = 0;

Vine::Vine()
: parentNode(NULL), entireVine(NULL), tip(NULL), base(NULL), dest(), radius(0.0), speed(0.0), loc(NO_DIRECTION), previoust(0.0), previousID(0), aftert(0.0), afterID(0)
{}

Vine::Vine(Ogre::SceneNode* parentNode, Vector3 pos, double radius)
: parentNode(parentNode), entireVine(NULL), tip(NULL), base(NULL), dest(), forward(), radius(radius), speed(0.0)
{
    loadRunnerShip();
    ++vineID;
}

void Vine::loadBasicShip()
{
    removeFromScene();
    entireVine = parentNode->createChildSceneNode("entireVineNode" + Util::toStringInt(vineID));
    
    tip = entireVine->createChildSceneNode("vineTipNode" + Util::toStringInt(vineID));
    
    Entity* tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineID), "sphereMesh");
    tipEntity->setMaterialName("General/VineTop");
    tip->attachObject(tipEntity);
    tip->scale(radius,radius,radius);
    
    base = entireVine->createChildSceneNode("vineBaseNode" + Util::toStringInt(vineID));
     
    Entity* baseEntity = base->getCreator()->createEntity("vineBaseEntity" + Util::toStringInt(vineID), "cylinderMesh");
    baseEntity->setMaterialName("General/VineBase");
    base->attachObject(baseEntity);
    base->translate(0, -radius / 3.0, 0);
    base->scale(radius * 1.5, radius / 1.5, radius * 1.5);
}

void Vine::loadRunnerShip()
{
    removeFromScene();
    entireVine = parentNode->createChildSceneNode("entireVineNode" + Util::toStringInt(vineID));
    
    tip = entireVine->createChildSceneNode("vineTipNode" + Util::toStringInt(vineID));
    
    Entity* tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineID), "runnerShip.mesh");
    tip->attachObject(tipEntity);
    tip->scale(radius / 1.5, radius / 1.5, radius / 1.5);
    tip->yaw(Degree(180.0));
    
//    tipEntity->getSubEntity(0)->setMaterialName("General/PodYellow");
//    tipEntity->getSubEntity(1)->setMaterialName("General/PodRed");
//    tipEntity->getSubEntity(2)->setMaterialName("General/PodBlue");
//    tipEntity->getSubEntity(3)->setMaterialName("General/PodGreen");
}

void Vine::loadFlowerShip()
{
    removeFromScene();
    entireVine = parentNode->createChildSceneNode("entireVineNode" + Util::toStringInt(vineID));
    
    tip = entireVine->createChildSceneNode("vineTipNode" + Util::toStringInt(vineID));
    
    Entity* tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineID), "flowerVehicle.mesh");
    tip->attachObject(tipEntity);
    tip->yaw(Degree(180.0));
    tip->scale(0.5, 0.5, 0.5);
}

SceneNode* Vine::getTip() const
{
	return tip;
}

Vector3 Vine::getPos() const
{
	return entireVine->getPosition();
}

void Vine::setPos(Vector3 value)
{
	entireVine->setPosition(value);
}

Vector3 Vine::getDest() const
{
	return dest;
}

Vector3 Vine::getForward() const
{
	return forward;
}

void Vine::setDest(Vector3 value)
{
	dest = value;
}

void Vine::setForward(Vector3 value)
{
	forward = value;
}

void Vine::move(Vector3 delta)
{
	entireVine->translate(delta);
}

double Vine::getRadius() const
{
    return radius;
}

void Vine::setQuaternion(Quaternion rot)
{
    entireVine->setOrientation(rot);
}

void Vine::update(double elapsed)
{
    double moveSpeed = speed;
    
	Vector3 dist = dest - entireVine->getPosition();
    // This should never happen, but we will ensure the player will never fall behind
    if (dist.length() > 1.1 * speed) {
        moveSpeed = dist.length();
    }
    
	Vector3 norm = dist;
	norm.normalise();
	Vector3 delta = dist * moveSpeed * elapsed;
    
    // Ensure going forward at moveSpeed
    Vector3 forwardLim = forward * moveSpeed * elapsed;
    double missingDist = delta.dotProduct(forwardLim) / forwardLim.length();
    delta += forward * missingDist;
    
    if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z >
        dist.x * dist.x + dist.y * dist.y + dist.z * dist.z)
		delta = dist;
    
	move(delta);
}

void Vine::removeFromScene()
{
    if (base)
    {
        base->getCreator()->destroyMovableObject(base->getAttachedObject(0)); // Assuming only one entity
        base->removeAndDestroyAllChildren();
        base->getCreator()->destroySceneNode(base);
        base = NULL;
    }
    if (tip)
    {
        tip->getCreator()->destroyMovableObject(tip->getAttachedObject(0)); // Assuming only one entity
        tip->removeAndDestroyAllChildren();
        tip->getCreator()->destroySceneNode(tip);
        tip = NULL;
    }
    
    if (entireVine)
    {
        entireVine->removeAndDestroyAllChildren();
        entireVine->getCreator()->destroySceneNode(base);
        entireVine = NULL;
    }
}
