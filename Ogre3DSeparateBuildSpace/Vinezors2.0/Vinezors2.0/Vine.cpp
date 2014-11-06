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
: parentNode(NULL), meshType(VINE_FLOWER_SHIP), entireVine(NULL), tip(NULL), tipEntity(NULL), base(NULL), shockwaveEffect(NULL), boostEffect(NULL), radius(0.0), loc(NO_DIRECTION), dest(NO_DIRECTION), transition(0.0), previoust(0.0), previousID(0), aftert(0.0), afterID(0)
{}

Vine::Vine(Ogre::SceneNode* parentNode, Vector3 pos, float radius)
: parentNode(parentNode), meshType(VINE_FLOWER_SHIP), entireVine(NULL), tip(NULL), tipEntity(NULL), base(NULL), shockwaveEffect(NULL), boostEffect(NULL), forward(), radius(radius), loc(NO_DIRECTION), dest(NO_DIRECTION), transition(0.0), totalElapsed(0.0), wobbleSpeed(0.0), wobbling(false)
{
    loadShip();
}

Vine::~Vine()
{
    removeFromScene();
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
        default:
            loadFlowerShip();
            break;
    }
    ++vineID;
}

void Vine::loadBasicShip()
{
    removeFromScene();
    entireVine = parentNode->createChildSceneNode("entireVineNode" + Util::toStringInt(vineID));
    
    tip = entireVine->createChildSceneNode("vineTipNode" + Util::toStringInt(vineID));
    
    tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineID), "sphereMesh");
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

/*
SceneNode* gateNode;
Entity* gateEntity;

SceneNode* gateDoorNodes[8];
Entity* gateDoorEntities[8];

bool gateOpen = true;
float tVal = 0.0f;

float tSpeedOpen = 10.0f;
float tSpeedClose = 20.0f;

float tSpeed = 0.0f;
float tAccel = 2.0f;

float gateDelayTimer = 0.0f;
float gateDelay = 1.0f;

void Vine::updateGate(float elapsed)
{
    if( gateOpen ) {
        if( tVal + tSpeed*elapsed >= 4.0f ) {
            tVal = 4.0f;
            if( gateDelayTimer >= gateDelay ) {
                gateOpen = false;
                gateDelayTimer = 0.0f;
                tSpeed = 0.0f;
            }
            else {
                gateDelayTimer += elapsed;
            }
        }
        else {
            if( tSpeed+tAccel >= tSpeedOpen ) {
                tSpeed = tSpeedOpen;
            }
            else {
                tSpeed += tAccel;
            }
            tVal += tSpeed*elapsed;
        }
    }
    else {
        if( tVal - tSpeed*elapsed <= 0.0f ) {
            tVal = 0.0f;
            if( gateDelayTimer >= gateDelay ) {
                gateOpen = true;
                gateDelayTimer = 0.0f;
                tSpeed = 0.0f;
            }
            else {
                gateDelayTimer += elapsed;
            }
        }
        else {
            if( tSpeed+tAccel >= tSpeedClose ) {
                tSpeed = tSpeedClose;
            }
            else {
                tSpeed += tAccel;
            }
            tVal -= tSpeed*elapsed;
        }
    }
    
    for( int i = 0; i < 8; ++i ) {
        Vector3 t = Vector3(0,tVal,0);
        Vector3 r = Vector3(Math::Cos(Degree(45*i))*t.x - Math::Sin(Degree(45*i))*t.y,Math::Sin(Degree(45*i))*t.x + Math::Cos(Degree(45*i))*t.y,0);
        gateDoorNodes[i]->setPosition(r);
    }
}
*/
void Vine::loadRunnerShip()
{
    removeFromScene();
    entireVine = parentNode->createChildSceneNode("entireVineNode" + Util::toStringInt(vineID));
    
    tip = entireVine->createChildSceneNode("vineTipNode" + Util::toStringInt(vineID));
    
    tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineID), "Ships/new_ship_mesh.mesh");
    tip->attachObject(tipEntity);
    tip->scale(radius / 1.5, radius / 1.5, radius / 1.5);
    tip->yaw(Degree(180.0));
    tipEntity->getSubEntity(7)->setMaterialName("new_ship_mesh/GlassHull");
    tipEntity->getSubEntity(3)->setMaterialName("new_ship_mesh/GlassHull");
    tipEntity->getSubEntity(2)->setMaterialName("new_ship_mesh/EndWing");
    tipEntity->getSubEntity(4)->setMaterialName("new_ship_mesh/FrontLine");
    
    tipEntity->getSubEntity(0)->setMaterialName("new_ship_mesh/EndWing");
    tipEntity->getSubEntity(1)->setMaterialName("new_ship_mesh/FrontWing");
    tipEntity->getSubEntity(5)->setMaterialName("new_ship_mesh/FrontWing");
    tipEntity->getSubEntity(6)->setMaterialName("new_ship_mesh/ThrusterColor");
    
    
    base = entireVine->createChildSceneNode("vineBaseNode" + Util::toStringInt(vineID));
    base->translate(0, 0.0, radius * 2.0);
    base->yaw(Degree(180.0));
    
    /*
    gateNode = entireVine->createChildSceneNode("gateNode" + Util::toStringInt(vineID));
    gateEntity = gateNode->getCreator()->createEntity("gateEntity" + Util::toStringInt(vineID), "ExitGate/ExitGate.mesh");
    
    gateEntity->getSubEntity(0)->setMaterialName("Gate/TransparentNeonAqua");
    gateEntity->getSubEntity(1)->setMaterialName("Gate/NeonAqua");
    gateEntity->getSubEntity(2)->setMaterialName("Gate/LightGray");
    gateEntity->getSubEntity(3)->setMaterialName("Gate/DarkGray");
    
    for( int i = 0; i < 8; ++i )
    {
        gateDoorNodes[i] = gateNode->createChildSceneNode("gateDoorNode" + Util::toStringInt(vineID) + Util::toStringInt(i));
        gateDoorEntities[i] = gateDoorNodes[i]->getCreator()->createEntity("gateDoorEntity" + Util::toStringInt(vineID) + Util::toStringInt(i), "ExitGate/ExitGateDoor.mesh");
    
        gateDoorEntities[i]->getSubEntity(0)->setMaterialName("Gate/DarkestGray");
        gateDoorEntities[i]->getSubEntity(1)->setMaterialName("Gate/DarkGray");
    
        gateDoorNodes[i]->attachObject(gateDoorEntities[i]);
        
        gateDoorNodes[i]->roll(Degree(45*i));
    }
    
    */
    /*
    gateEntity->getSubEntity(0)->setMaterialName("Gate/DarkGray");
    gateEntity->getSubEntity(1)->setMaterialName("Gate/NeonAqua");
    
    for( int i = 2; i <= 16; i += 2 )
        gateEntity->getSubEntity(i)->setMaterialName("Gate/DarkestGray");
    
    for( int i = 3; i <= 17; i += 2 )
        gateEntity->getSubEntity(i)->setMaterialName("Gate/DarkGray");
    
    gateEntity->getSubEntity(18)->setMaterialName("Gate/TransparentNeonAqua");
    
    gateEntity->getSubEntity(19)->setMaterialName("Gate/NeonAqua");
    gateEntity->getSubEntity(20)->setMaterialName("Gate/LightGray");
    
     */
    /*
    gateNode->attachObject(gateEntity);
    gateNode->translate(Vector3(0,0,-10));
     */
}

void Vine::loadFlowerShip()
{
    removeFromScene();
    entireVine = parentNode->createChildSceneNode("entireVineNode" + Util::toStringInt(vineID));
    
    tip = entireVine->createChildSceneNode("vineTipNode" + Util::toStringInt(vineID));
    
    tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineID), "Ships/flowerVehicle.mesh");
    tip->attachObject(tipEntity);
    tip->yaw(Degree(180.0));
    tip->scale(0.5, 0.5, 0.5);
    
    // Problem with transparency not carrying over... assign custom material
    tipEntity->getSubEntity(0)->setMaterialName("General/VineShellActive");
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

void Vine::setVisible(bool value)
{
    entireVine->setVisible(value);
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
    if (!boostEffect && base)
    {
        boostEffect = parentNode->getCreator()->createParticleSystem("StarBoost", "General/StarBoost");
        base->attachObject(boostEffect);
    }
}

void Vine::move(Vector3 delta)
{
	entireVine->translate(delta);
}

float length = 1.0f;
float originalHeight = 0.0f;

void Vine::update(float elapsed)
{
    tip->resetOrientation();
    tip->yaw(Degree(180.0));
    if (wobbling)
    {
        totalElapsed += elapsed;
        wobbleSpeed += elapsed;
        if (wobbleSpeed > 2.0)
            wobbleSpeed = 2.0;
        
        tip->pitch(-8 * Degree(sin(8 * (2.2-wobbleSpeed) * totalElapsed)));
        tip->roll(16 * Degree(sin(8 * (2.2-wobbleSpeed) * totalElapsed)));
        tip->translate(0.0,-0.1*sin(4*(2.1-wobbleSpeed) * totalElapsed),0.0);
        
        if (totalElapsed >= length) {
            setWobble(false);
            Vector3 fixHeight = tip->getPosition();
            fixHeight.y = originalHeight;
            tip->setPosition(fixHeight);
        }
    }
    else {
        Vector3 pos = tip->getPosition();
        originalHeight = pos.y;
    }
    
    //updateGate(elapsed);
}

// Display effect for some powerup. Currently used to show godmode is on
void Vine::setPowerIndication(int indication)
{
    if (meshType == VINE_RUNNER_SHIP)
    {
        if (indication > 0)
        {
            if (tipEntity->getNumSubEntities() >= 7)
                tipEntity->getSubEntity(6)->setMaterialName("new_ship_mesh/ThrusterColorYellow");
        }
        else
        {
            if (tipEntity->getNumSubEntities() >= 7)
                tipEntity->getSubEntity(6)->setMaterialName("new_ship_mesh/ThrusterColor");
        }
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
        base->detachObject(boostEffect);
        base->getCreator()->destroyParticleSystem(boostEffect);
        boostEffect = NULL;
    }
}

void Vine::removeFromScene()
{
    removeBoost();
    if (base)
    {
        if (base->numAttachedObjects() > 0)
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
        removeShockwave();
        entireVine->removeAndDestroyAllChildren();
        entireVine->getCreator()->destroySceneNode(entireVine);
        entireVine = NULL;
    }
}
