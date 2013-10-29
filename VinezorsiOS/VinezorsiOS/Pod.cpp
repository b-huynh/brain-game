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
: entirePod(NULL), stem(NULL), head(NULL), shell(NULL)
{
}

Pod::Pod(Vector3 base, Vector3 tip, PodType type, double stemRadius, double headRadius)
    : base(base), tip(tip), type(type), stemRadius(stemRadius), headRadius(headRadius), entirePod(NULL), stem(NULL), head(NULL), shell(NULL), podTaken(false)
{
	double stemLength = base.distance(tip);
    
    entirePod = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("entirePodNode" + Util::toStringInt(podID));
    
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

bool Pod::isPodTaken() const
{
    return podTaken;
}

void Pod::move(Vector3 delta)
{
	stem->translate(delta);
	head->translate(delta);
}

void Pod::takePod()
{
    podTaken = true;
    head->setVisible(false);
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

PodType Pod::getPodType()
{
    return type;
}
