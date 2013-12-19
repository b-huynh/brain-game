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
: parentNode(NULL), entirePod(NULL), stem(NULL), head(NULL), shell(NULL)
{
}

Pod::Pod(Ogre::SceneNode* parentNode, Vector3 base, Vector3 tip, MeshType mtype, PodSignal podSignal, PodColor podColor, PodShape podShape, PodSound podSound, Direction loc, double stemRadius, double headRadius)
: parentNode(parentNode), mtype(mtype), base(base), tip(tip), podSignal(podSignal), podColor(podColor), podShape(podShape), podSound(podSound), stemRadius(stemRadius), stemLength(base.distance(tip)), headRadius(headRadius), entirePod(NULL), stem(NULL), head(NULL), shell(NULL), moveSpeed(0.0), rotateSpeed(0.0, 0.0, 0.0), loc(loc), podTaken(false), podGood(false), dest()
{
    if (mtype == BASIC) loadBasicShape();
    else if (mtype == FUEL) loadFuelCell();
    
    ++podID;
}

void Pod::loadBasicShape()
{
    removeFromScene();
    
    entirePod = parentNode->createChildSceneNode("entirePodNode" + Util::toStringInt(podID));
    
    stem = entirePod->createChildSceneNode("stemNode" + Util::toStringInt(podID));
    
    Entity* stemEntity = stem->getCreator()->createEntity("stemEntity" + Util::toStringInt(podID), "cylinderMesh");
    stemEntity->setMaterialName("General/PodStem");
    stem->attachObject(stemEntity);
    
    Vector3 v = tip - base;
    stem->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    
    stem->translate(v / -2);
    
    head = entirePod->createChildSceneNode("headNode" + Util::toStringInt(podID));
    
    //Entity* headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "sphereMesh");
    Entity* headEntity = NULL;
    switch (podShape)
    {
        case POD_SHAPE_CONE:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "cylinderMesh");
            break;
        case POD_SHAPE_SPHERE:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "sphereMesh");
            break;
        case POD_SHAPE_DIAMOND:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "diamondMesh");
            break;
        case POD_SHAPE_TRIANGLE:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "boxMesh");
            break;
        default:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "cylinderMesh");
            break;
    }
    headEntity->setMaterialName("General/PodUnknown");
    head->attachObject(headEntity);
    head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    head->scale(headRadius, headRadius, headRadius);
    //head->scale(headRadius * 1.5, headRadius * 3, headRadius * 1.5); // For diamond or cylinder
    //head->scale(headRadius * 1.5, headRadius * 1.5, headRadius * 1.5);
    
    setToGrowth(0.0);
}

void Pod::loadFuelCell()
{
    removeFromScene();
    
	double stemLength = base.distance(tip);
    entirePod = parentNode->createChildSceneNode("entirePodNode" + Util::toStringInt(podID));
    /*
    stem = entirePod->createChildSceneNode("stemNode" + Util::toStringInt(podID));
    
    Entity* stemEntity = stem->getCreator()->createEntity("stemEntity" + Util::toStringInt(podID), "cylinderMesh");
    stemEntity->setMaterialName("General/PodStem");
    stem->attachObject(stemEntity);
     */
    Vector3 v = tip - base;
    /*
    //stem->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    //stem->translate(v / -2);
    */
    
    head = parentNode->createChildSceneNode("headNode" + Util::toStringInt(podID));
    
    Entity* headEntity = NULL;
    switch (podShape)
    {
        case POD_SHAPE_CONE:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "fuelCylinder.mesh");
            break;
        case POD_SHAPE_SPHERE:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "fuelSphere.mesh");
            break;
        case POD_SHAPE_DIAMOND:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "fuelCube.mesh");
            break;
        case POD_SHAPE_TRIANGLE:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "fuelTri.mesh");
            break;
        default:
            headEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "fuelCylinder.mesh");
            break;
    }
    headEntity->getSubEntity(1)->setMaterialName("General/PodUnknown");
    head->attachObject(headEntity);
    head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    head->scale(headRadius / 1.5, 0.05, headRadius / 1.5);
    head->setPosition(base);
    head->translate(v / 2);
    setRotateSpeed(Vector3(5, 0, 0));
    
    // The follow is to make a fuel cell randomly float
    //head->pitch(Degree(Util::randRangeDouble(0.0, 90.0)));
    //head->roll(Degree(Util::randRangeDouble(0.0, 90.0)));
    //setRotateSpeed(Util::randVector3() * Util::randRangeDouble(1.0, 5.0));
    //rotateSpeed.x = 0;
    //head->translate(v / -1.75);
    
    setToGrowth(0.0);
    
}

MeshType Pod::getMeshType() const
{
    return mtype;
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
    if (mtype == BASIC)
    {
        entirePod->setPosition(tip);
        entirePod->translate((base - tip) * (1 - t));
        stem->setScale(Vector3(t * stemRadius, t * stemLength, t * stemRadius));
    }
    else
    {
        entirePod->setPosition(tip);
        entirePod->translate((base - tip) * (1 - t));
        head->setScale(Vector3(headRadius / 1.5, t * headRadius / 1.5, headRadius / 1.5));
    }
}

PodSignal Pod::getPodSignal() const
{
	return podSignal;
}

PodColor Pod::getPodColor() const
{
	return podColor;
}

PodShape Pod::getPodShape() const
{
	return podShape;
}

PodSound Pod::getPodSound() const
{
	return podSound;
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

PodInfo Pod::getPodInfo() const
{
    return PodInfo(mtype, podSignal, podColor, podShape, podSound, loc, podGood, podTrigger, podTaken);
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

bool Pod::getPodTrigger() const
{
    return podTrigger;
}

bool Pod::isPodGood() const
{
    return podGood;
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
        switch (podColor)
        {
            case POD_COLOR_BLUE:
                static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodBlue");
                break;
            case POD_COLOR_GREEN:
                static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodGreen");
                break;
            case POD_COLOR_PINK:
                static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodRed");
                break;
            case POD_COLOR_YELLOW:
                static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodYellow");
                break;
            default:
                //static_cast<Entity*>(head->getAttachedObject(0))->setMaterialName("General/PodWhite");
                break;
        }
        
    } else {
        Entity* headEntity = static_cast<Entity*>(head->getAttachedObject(0));
        // Based on Maya model, SubEntity1 is the content in the fuel cell
        switch (podColor)
        {
            case POD_COLOR_BLUE:
                headEntity->getSubEntity(1)->setMaterialName("General/PodBlue");
                break;
            case POD_COLOR_GREEN:
                headEntity->getSubEntity(1)->setMaterialName("General/PodGreen");
                break;
            case POD_COLOR_PINK:
                headEntity->getSubEntity(1)->setMaterialName("General/PodRed");
                break;
            case POD_COLOR_YELLOW:
                headEntity->getSubEntity(1)->setMaterialName("General/PodYellow");
                break;
            default:
                // Have the entity light up still, just with unknown color
                headEntity->getSubEntity(1)->setMaterialName("General/PodWhite");
                break;
        }
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

void Pod::setRotateSpeed(Vector3 value)
{
    rotateSpeed = value;
}

void Pod::setPodGood(bool value)
{
    podGood = value;
}

void Pod::setPodTrigger(bool value)
{
    podTrigger = value;
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
    if (shell)
    {
        shell->getCreator()->destroyMovableObject(shell->getAttachedObject(0));
        shell = NULL;
    }
    if (entirePod)
    {
        entirePod->removeAndDestroyAllChildren();
        entirePod->getCreator()->destroySceneNode(entirePod);
        entirePod = NULL;
    }
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
    if (rotateSpeed != Vector3::ZERO)
    {
        head->yaw(Degree(rotateSpeed.x));
        head->pitch(Degree(rotateSpeed.y));
        head->roll(Degree(rotateSpeed.z));
    }
}

Pod::~Pod()
{
    
}
