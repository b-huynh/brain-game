//
//  Vine.cpp
//  Testing
//
//  Created by Calvin Phung on 9/13/13.
//
//
#include "Vine.h"

static int vineTipID = 0;

Vine::Vine()
: parentNode(NULL), tip(NULL), dest(), radius(0.0), speed(0.0)
{}

Vine::Vine(Ogre::SceneNode* parentNode, Vector3 pos, double radius)
: parentNode(parentNode), tip(NULL), dest(), forward(), radius(radius), speed(0.0)
{
    tip = parentNode->createChildSceneNode("vineTipNode" + Util::toStringInt(vineTipID));
    
    Entity* tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineTipID), "vineMesh");
    tipEntity->setMaterialName("General/VineTip");
    tip->attachObject(tipEntity);
    
    ++vineTipID;
}

SceneNode* Vine::getTip() const
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
	tip->translate(delta);
}

double Vine::getRadius() const
{
    return radius;
}

void Vine::update(double elapsed)
{
    double moveSpeed = speed;
    
	Vector3 dist = dest - tip->getPosition();
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
    tip->getCreator()->destroyMovableObject(tip->getAttachedObject(0)); // Assuming only one entity
    tip->removeAndDestroyAllChildren();
    tip->getCreator()->destroySceneNode(tip);
    tip = NULL;
}
