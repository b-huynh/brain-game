//
//  Pod.cpp
//  Testing
//
//  Created by Calvin Phung on 9/3/13.
//
//

#include "Pod.h"

#include <cstdlib>

extern Util::ConfigGlobal globals;

static int podID = 0;

Pod::Pod()
: parentNode(NULL), entirePod(NULL), stem(NULL), head(NULL)
{
}

Pod::Pod(Ogre::SceneNode* parentNode, Vector3 base, Vector3 tip, PodType type, double stemRadius, double headRadius, Direction loc)
: parentNode(parentNode), base(base), tip(tip), type(type), stemRadius(stemRadius), stemLength(base.distance(tip)), headRadius(headRadius), entirePod(NULL), stem(NULL), head(NULL), moveSpeed(0.0), rotateSpeed(0.0), loc(loc), podTaken(false), dest()
{
    loadFuelCell();
    
    ++podID;
}

void Pod::loadBasicShape()
{
    removeFromScene();
    mtype = BASIC;
    
    entirePod = parentNode->createChildSceneNode("entirePodNode" + Util::toStringInt(podID));
    
    stem = entirePod->createChildSceneNode("stemNode" + Util::toStringInt(podID));
    
    Entity* stemEntity = stem->getCreator()->createEntity("stemEntity" + Util::toStringInt(podID), "cylinderMesh");
    stemEntity->setMaterialName("General/PodStem");
    stem->attachObject(stemEntity);
    
    Vector3 v = tip - base;
    stem->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    
    stem->translate(v / -2);
    
    head = entirePod->createChildSceneNode("headNode" + Util::toStringInt(podID));
    
    Entity* headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "boxMesh");
    headEntity->setMaterialName("General/PodUnknown");
    head->attachObject(headEntity);
    head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    //head->scale(headRadius, headRadius, headRadius);
    //head->scale(headRadius * 1.5, headRadius * 3, headRadius * 1.5); // For diamond or cylinder
    //head->scale(headRadius * 1.5, headRadius * 1.5, headRadius * 1.5);
    
    setToGrowth(0.0);
}

void Pod::loadFuelCell()
{
    removeFromScene();
    mtype = FUEL;
    
	double stemLength = base.distance(tip);
    entirePod = parentNode->createChildSceneNode("entirePodNode" + Util::toStringInt(podID));
    
    stem = entirePod->createChildSceneNode("stemNode" + Util::toStringInt(podID));
    
    Entity* stemEntity = stem->getCreator()->createEntity("stemEntity" + Util::toStringInt(podID), "cylinderMesh");
    stemEntity->setMaterialName("General/PodStem");
    stem->attachObject(stemEntity);
    
    Vector3 v = tip - base;
    stem->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    
    stem->translate(v / -2);
    
    head = parentNode->createChildSceneNode("headNode" + Util::toStringInt(podID));
    
    Entity* headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "emptyFuel.mesh");
    head->attachObject(headEntity);
    head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    head->scale(headRadius / 1.5, headRadius / 1.5, headRadius / 1.5);
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
    entirePod->setPosition(tip);
    entirePod->translate((base - tip) * (1 - t));
    stem->setScale(Vector3(t * stemRadius, t * stemLength, t * stemRadius));
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

Direction Pod::getLoc() const
{
    return loc;
}

double Pod::getStemRadius() const
{
    return stemRadius;
}

double Pod::getStemLength() const
{
    return stemLength;
}

double Pod::getHeadRadius() const
{
    return headRadius;
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
    //static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodUnknown");
    //static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("fuelCellBland.material");
}

void Pod::revealPod()
{
    if (mtype == BASIC)
    {
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
        
    } else {
        Entity* headEntity = static_cast<Entity*>(head->getAttachedObject(0));
        // Based on Maya model, SubEntity1 is the content in the fuel cell
        switch (type)
        {
            case POD_BLUE:
                headEntity->getSubEntity(1)->setMaterialName("General/PodBlue");
                break;
            case POD_GREEN:
                headEntity->getSubEntity(1)->setMaterialName("General/PodGreen");
                break;
            case POD_PINK:
                headEntity->getSubEntity(1)->setMaterialName("General/PodRed");
                break;
            case POD_YELLOW:
                headEntity->getSubEntity(1)->setMaterialName("General/PodYellow");
                break;
            case POD_BLACK:
                headEntity->getSubEntity(1)->setMaterialName("General/PodBlack");
                break;
            default:
                headEntity->getSubEntity(1)->setMaterialName("General/PodUnknown");
                break;
        }
        /*
        head->getCreator()->destroyMovableObject(head->getAttachedObject(0));
        Entity * headEntity;
        switch (type)
        {
            case POD_BLUE:
                headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "blueFuel.mesh");
                break;
            case POD_GREEN:
                headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "greenFuel.mesh");
                break;
            case POD_PINK:
                headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "redFuel.mesh");
                break;
            case POD_YELLOW:
                headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "yellowFuel.mesh");
                break;
            case POD_BLACK:
                headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "emptyFuel.mesh");
                break;
            default:
                headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "emptyFuel.mesh");
            break;
        }
        head->attachObject(headEntity);
         */
        podID++;
    }
}

void Pod::setDest(Vector3 value)
{
    dest = value;
}

void Pod::setMoveSpeed(double value)
{
    moveSpeed = value;
}

void Pod::setRotateSpeed(double value)
{
    rotateSpeed = value;
}

void Pod::removeFromScene()
{
    if (stem)
    {
        stem->getCreator()->destroyMovableObject(stem->getAttachedObject(0)); // Assuming only one entity
        stem = NULL;
    }
    if (head)
    {
        head->getCreator()->destroyMovableObject(head->getAttachedObject(0));
        head = NULL;
    }
    if (entirePod)
    {
        entirePod->removeAndDestroyAllChildren();
        entirePod->getCreator()->destroySceneNode(entirePod);
        entirePod = NULL;
    }
}

PodType Pod::getPodType() const
{
    return type;
}

void Pod::update(double elapsed)
{
    if (moveSpeed != 0.0)
    {
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
    if (rotateSpeed != 0.0)
    {
        head->yaw(Degree(rotateSpeed));
    }
}

Pod::~Pod()
{
    
}
