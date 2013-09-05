//
//  Pod.cpp
//  Testing
//
//  Created by Calvin Phung on 9/3/13.
//
//

#include "Pod.h"

#include <cstdlib>

Pod::Pod()
: stem(NULL), head(NULL), shell(NULL)
{
}

Pod::Pod(Vector3 base, Vector3 tip, PodType type, double stemRadius, double headRadius)
    : base(base), tip(tip), type(type), stemRadius(stemRadius), headRadius(headRadius), stem(NULL), head(NULL), shell(NULL)
{
	double stemLength = base.distance(tip);
    
    shell = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("ShellNode");
    shell->scale(1.2, 1.2, 1.2);
    
    Entity* shellEntity = shell->getCreator()->createEntity("shellEntity", "sphereMesh");
    shellEntity->setMaterialName("General/PodShell");
    shell->attachObject(shellEntity);
    shell->setPosition(tip);
    shell->setVisible(false);
    
    stem = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("Stem");
    
    Entity* stemEntity = stem->getCreator()->createEntity("stemEntity", "cylinderMesh");
    stemEntity->setMaterialName("General/PodStem");
    stem->attachObject(stemEntity);
    stem->setPosition((base + tip) / 2);
    
    Radian angle;
    Vector3 v = base - tip;
    
    // Quaternion rot and q is used to find the orientation of the stem segment
    angle = Math::ATan2(v.y, v.x) + (Radian)(Math::PI / 2);
    Quaternion q;
    q.FromAngleAxis(angle, Util::TUNNEL_REFERENCE_FORWARD);
    stem->setOrientation(q);
    
    head = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("headNode");
    
    Entity* headEntity = head->getCreator()->createEntity("headEntity", "sphereMesh");
    headEntity->setMaterialName("General/PodUnknown");
    head->attachObject(headEntity);
    head->setPosition(tip);
    
    setToGrowth(0.0);
    
}

Pod::Pod(Vector3 base, Vector3 tip, Quaternion rot, PodType type, double stemRadius, double headRadius)
    : base(base), tip(tip), type(type), stemRadius(stemRadius), headRadius(headRadius), stem(NULL), head(NULL), shell(NULL)
{
	double stemLength = base.distance(tip);
    
    shell = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("ShellNode");
    shell->scale(1.2, 1.2, 1.2);
    
    Entity* shellEntity = shell->getCreator()->createEntity("shellEntity", "sphereMesh");
    shellEntity->setMaterialName("General/PodShell");
    shell->attachObject(shellEntity);
    shell->setPosition(tip);
    shell->setVisible(false);
    
    stem = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("Stem");
    
    Entity* stemEntity = stem->getCreator()->createEntity("stemEntity", "cylinderMesh");
    stemEntity->setMaterialName("General/PodStem");
    stem->attachObject(stemEntity);
    stem->setPosition((base + tip) / 2);
    
    Radian angle;
    Vector3 v = base - tip;
    
    // Quaternion rot and q is used to find the orientation of the stem segment
    v = rot.Inverse() * v; // Reverse to old coordinates to find x-y angle
    angle = Math::ATan2(v.y, v.x) + (Radian)(Math::PI / 2);
    Quaternion q;
    q.FromAngleAxis(angle, Util::TUNNEL_REFERENCE_FORWARD);
    q = q.Inverse();
    q = rot * q;
    stem->setOrientation(q);
    
    head = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("headNode");
    
    Entity* headEntity = head->getCreator()->createEntity("headEntity", "sphereMesh");
    headEntity->setMaterialName("General/PodUnknown");
    head->attachObject(headEntity);
    head->setPosition(tip);
    
    setToGrowth(0.0);
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
    Vector3 baseToTip = tip - base;
    double totalLength = baseToTip.length();
    baseToTip.normalise();
    
    double stemLength = totalLength;
    Vector3 nBase = base + baseToTip * (-stemLength * (1 - t));
    Vector3 nTip = nBase + baseToTip * (stemLength);
    
    stem->setPosition((base + tip) / 2);
    stem->setPosition((nBase + nTip) / 2);
	head->setPosition(nTip);
    shell->setPosition(nTip);
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

void Pod::move(Vector3 delta)
{
	stem->translate(delta);
	head->translate(delta);
}

void Pod::hidePod()
{
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

void Pod::showShell()
{
    shell->setVisible(true);
}

void Pod::removeFromScene()
{
    stem->getCreator()->destroyMovableObject(stem->getAttachedObject(0)); // Assuming only one entity
    head->getCreator()->destroyMovableObject(head->getAttachedObject(0));
    shell->getCreator()->destroyMovableObject(shell->getAttachedObject(0));
    stem->removeAndDestroyAllChildren();
    head->removeAndDestroyAllChildren();
    shell->removeAndDestroyAllChildren();
    stem->getCreator()->destroySceneNode(stem);
    head->getCreator()->destroySceneNode(head);
    shell->getCreator()->destroySceneNode(shell);
	stem = NULL;
	head = NULL;
    shell = NULL;
}

PodType Pod::getPodType()
{
    return type;
}
