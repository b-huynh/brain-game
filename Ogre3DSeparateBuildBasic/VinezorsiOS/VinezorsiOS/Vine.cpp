//
//  Vine.cpp
//  Testing
//
//  Created by Calvin Phung on 9/13/13.
//
//
#include "Vine.h"

extern Util::ConfigGlobal globals;

static int vineID = 0;

Vine::Vine()
: parentNode(NULL), meshType(VINE_NONE), entireVine(NULL), tip(NULL), base(NULL), shell(NULL), shockwaveEffect(NULL), boostEffect(NULL), radius(0.0), loc(NO_DIRECTION), dest(NO_DIRECTION), transition(0.0), previoust(0.0), previousID(0), aftert(0.0), afterID(0)
{}

Vine::Vine(Ogre::SceneNode* parentNode, Vector3 pos, float radius)
: parentNode(parentNode), meshType(VINE_NONE), entireVine(NULL), tip(NULL), base(NULL), shell(NULL), shockwaveEffect(NULL), boostEffect(NULL), forward(), radius(radius), loc(NO_DIRECTION), dest(NO_DIRECTION), transition(0.0), totalElapsed(0.0), wobbleSpeed(0.0), wobbling(false)
{
    loadShip();
}

void Vine::reloadIfNecessary(VineMeshType newMeshType)
{
    if (meshType != newMeshType)
    {
        meshType = newMeshType;
        loadShip();
    }
}

void Vine::loadShip()
{
    switch (meshType)
    {
        case VINE_BASIC_SHIP:
            loadBasicShip();
            break;
        case VINE_RUNNER_SHIP:
            loadRunnerShip();
            break;
        case VINE_FLOWER_SHIP:
            loadFlowerShip();
            break;
        default:
            loadNothing();
            break;
    }
    ++vineID;
}

void Vine::loadNothing()
{
    removeFromScene();
    entireVine = parentNode->createChildSceneNode("entireVineNode" + Util::toStringInt(vineID));
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
    
    Entity* tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineID), "Ships/runnerShip.mesh");
    tip->attachObject(tipEntity);
    tip->scale(radius / 1.5, radius / 1.5, radius / 1.5);
    tip->yaw(Degree(180.0));
    
    /*
     // Transparent shell for maybe a shield look around the ship
    shell = entireVine->createChildSceneNode("shellNode" + Util::toStringInt(vineID));
    Entity* shellEntity = shell->getCreator()->createEntity("vineShellEntity" + Util::toStringInt(vineID), "sphereMesh");
    shellEntity->setMaterialName("General/VineShell");
    shell->attachObject(shellEntity);
    shell->scale(
        globals.podAppearance * (globals.tunnelSegmentDepth + globals.tunnelSegmentBuffer),
        globals.podAppearance * (globals.tunnelSegmentDepth + globals.tunnelSegmentBuffer),
        globals.podAppearance * (globals.tunnelSegmentDepth + globals.tunnelSegmentBuffer));
    shell->setScale(radius * 2.5, radius * 2.5, radius * 2.5);
    */
}

void Vine::loadFlowerShip()
{
    removeFromScene();
    entireVine = parentNode->createChildSceneNode("entireVineNode" + Util::toStringInt(vineID));
    
    tip = entireVine->createChildSceneNode("vineTipNode" + Util::toStringInt(vineID));
    
    Entity* tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineID), "Ships/flowerVehicle.mesh");
    tip->attachObject(tipEntity);
    tip->yaw(Degree(180.0));
    tip->scale(0.5, 0.5, 0.5);
    
    // Problem with transparency not carrying over... assign custom material
    tipEntity->getSubEntity(0)->setMaterialName("General/VineShell");
}

VineMeshType Vine::getMeshType() const
{
    return meshType;
}

SceneNode* Vine::getEntireVine() const
{
	return entireVine;
}

SceneNode* Vine::getTip() const
{
	return tip;
}

SceneNode* Vine::getBase() const
{
	return base;
}

Vector3 Vine::getPos() const
{
	return entireVine->getPosition();
}

void Vine::setPos(Vector3 value)
{
	entireVine->setPosition(value);
}

Vector3 Vine::getForward() const
{
	return forward;
}

void Vine::setForward(Vector3 value)
{
	forward = value;
}

float Vine::getRadius() const
{
    return radius;
}

void Vine::setQuaternion(Quaternion rot)
{
    entireVine->setOrientation(rot);
}

void Vine::setWobble(bool value)
{
    wobbling = value;
    totalElapsed = 0.0;
    wobbleSpeed = 0.0;
}

void Vine::setShockwave()
{
    if (!shockwaveEffect)
    {
        shockwaveEffect = parentNode->getCreator()->createParticleSystem("StarShockwave", "General/StarShockwave");
        entireVine->attachObject(shockwaveEffect);
    }
}

void Vine::setBoost()
{
    if (!boostEffect)
    {
        boostEffect = parentNode->getCreator()->createParticleSystem("StarBoost", "General/StarBoost");
        entireVine->attachObject(boostEffect); 
    }
}

void Vine::move(Vector3 delta)
{
	entireVine->translate(delta);
}

void Vine::update(float elapsed)
{
    if (tip)
    {
        tip->resetOrientation();
        tip->yaw(Degree(180.0));
    }
    if (wobbling)
    {
        totalElapsed += elapsed;
        wobbleSpeed += elapsed;
        if (wobbleSpeed > 2.0)
            wobbleSpeed = 2.0;
        
        if (tip) tip->roll(15 * Degree(sin(64 * wobbleSpeed * totalElapsed)));
        if (totalElapsed >= 0.5)
            setWobble(false);
    }
}

void Vine::removeShockwave()
{
    if (shockwaveEffect)
    {
        entireVine->detachObject(shockwaveEffect);
        entireVine->getCreator()->destroyParticleSystem(shockwaveEffect);
        shockwaveEffect = NULL;
    }
}

void Vine::removeBoost()
{
    if (boostEffect)
    {
        entireVine->detachObject(boostEffect);
        entireVine->getCreator()->destroyParticleSystem(boostEffect);
        boostEffect = NULL;
    }
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
    if (shell)
    {
        shell->getCreator()->destroyMovableObject(tip->getAttachedObject(0)); // Assuming only one entity
        shell->removeAndDestroyAllChildren();
        shell->getCreator()->destroySceneNode(shell);
        shell = NULL;
    }
    if (entireVine)
    {
        removeShockwave();
        removeBoost();
        entireVine->removeAndDestroyAllChildren();
        entireVine->getCreator()->destroySceneNode(entireVine);
        entireVine = NULL;
    }
}
