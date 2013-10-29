//
//  Pod.cpp
//  Testing
//
//  Created by Calvin Phung on 9/3/13.
//
//

#include "Pod.h"

#include <cstdlib>

static int podID = 0;

Pod::Pod()
: parentNode(NULL), entirePod(NULL), stem(NULL), head(NULL), shell(NULL)
{
}

Pod::Pod(Ogre::SceneNode* parentNode, Vector3 base, Vector3 tip, PodType type, double stemRadius, double headRadius)
: parentNode(parentNode), base(base), tip(tip), type(type), stemRadius(stemRadius), headRadius(headRadius), entirePod(NULL), stem(NULL), head(NULL), shell(NULL), podTaken(false), dest()
{
	double stemLength = base.distance(tip);
    
    entirePod = parentNode->createChildSceneNode("entirePodNode" + Util::toStringInt(podID));
    
    shell = entirePod->createChildSceneNode("ShellNode" + Util::toStringInt(podID));
    
    Entity* shellEntity = shell->getCreator()->createEntity("shellEntity" + Util::toStringInt(podID), "podMesh");
    shellEntity->setMaterialName("General/PodShell");
    shell->attachObject(shellEntity);
    shell->scale(1.2, 1.2, 1.2);
    shell->setVisible(false);
    
    stem = entirePod->createChildSceneNode("stemNode" + Util::toStringInt(podID));
    
    Entity* stemEntity = stem->getCreator()->createEntity("stemEntity" + Util::toStringInt(podID), "stemMesh");
    stemEntity->setMaterialName("General/PodStem");
    stem->attachObject(stemEntity);
    
    Vector3 v = tip - base;
    stem->setOrientation(Util::TUNNEL_REFERENCE_UPWARD.getRotationTo(v));
    
    stem->translate(v / -2);
    
    head = entirePod->createChildSceneNode("headNode" + Util::toStringInt(podID));
    
    Entity* headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "podMesh");
    headEntity->setMaterialName("General/PodUnknown");
    head->attachObject(headEntity);
    
    entirePod->setPosition(tip);
    
    setToGrowth(0.0);
    
    ++podID;
}

Vector3 Pod::getBase() const
{
    return base;
}

Vector3 Pod::getTip() const
{
    return tip;
}

void Pod::setToGrowth(double t)
{
    entirePod->setPosition(tip);
    entirePod->translate((base - tip) * (1 - t));
    stem->setScale(Vector3(t, t, t));
}

PodType Pod::getType() const
{
	return type;
}

SceneNode* Pod::getStem() const
{
	return stem;
}

SceneNode* Pod::getHead() const
{
	return head;
}

Vector3 Pod::getDest() const
{
	return dest;
}

Vector3 Pod::getPosition() const
{
	return entirePod->getPosition();
}

bool Pod::isPodTaken() const
{
    return podTaken;
}

void Pod::move(Vector3 delta)
{
    entirePod->translate(delta);
}

void Pod::takePod()
{
    podTaken = true;
    head->setVisible(false);
}

void Pod::hidePod()
{
    static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodUnknown");
}

void Pod::revealPod()
{
    //shell->setVisible(true);
     switch (type)
     {
         case POD_BLUE:
             static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodBlue");
             break;
         case POD_GREEN:
             static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodGreen");
             break;
         case POD_PINK:
             static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodRed");
             break;
         case POD_YELLOW:
             static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodYellow");
             break;
         case POD_BLACK:
             static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodUnknown");
             break;
         default:
             static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodUnknown");
             break;
     }
}

void Pod::setDest(Vector3 value)
{
    dest = value;
}

void Pod::removeFromScene()
{
    stem->getCreator()->destroyMovableObject(stem->getAttachedObject(0)); // Assuming only one entity
    head->getCreator()->destroyMovableObject(head->getAttachedObject(0));
    shell->getCreator()->destroyMovableObject(shell->getAttachedObject(0));
    
    entirePod->removeAndDestroyAllChildren();
    entirePod->getCreator()->destroySceneNode(entirePod);
    
    entirePod = NULL;
	stem = NULL;
	head = NULL;
    shell = NULL;
}

PodType Pod::getPodType() const
{
    return type;
}

void Pod::update(double elapsed)
{
    double moveSpeed = 5.0;
    
	Vector3 dist = dest - entirePod->getPosition();
    
	Vector3 norm = dist;
	norm.normalise();
	Vector3 delta = dist * moveSpeed * elapsed;
    
    //	if (delta.length() > dist.length())
    if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z >
        dist.x * dist.x + dist.y * dist.y + dist.z * dist.z)
		delta = dist;
    
	move(delta);
}

Pod::~Pod()
{
    
}
