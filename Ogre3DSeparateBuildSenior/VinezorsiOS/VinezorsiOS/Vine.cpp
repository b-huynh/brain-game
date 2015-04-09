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
: parentNode(NULL), meshType(VINE_BEE), entireVine(NULL), tip(NULL), base(NULL), shell(NULL), boost(NULL), shockwaveEffect(NULL), boostEffect(NULL), radius(0.0), loc(NO_DIRECTION), dest(NO_DIRECTION), transition(0.0), previoust(0.0), previousID(0), aftert(0.0), afterID(0), shadow(NULL)
{}

Vine::Vine(Ogre::SceneNode* parentNode, Vector3 pos, float radius)
: parentNode(parentNode), meshType(VINE_BEE), entireVine(NULL), tip(NULL), base(NULL), shell(NULL), boost(NULL), shockwaveEffect(NULL), boostEffect(NULL), forward(), radius(radius), loc(NO_DIRECTION), dest(NO_DIRECTION), transition(0.0), totalElapsed(0.0), wobbleSpeed(0.0), wobbling(false), shadow(NULL)
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
        case VINE_BEE:
            loadBee();
            break;
        default:
            loadBee();
            //loadFlowerShip();
            break;
    }
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

void Vine::loadBee()
{
    removeFromScene();
    entireVine = parentNode->createChildSceneNode("entireVineNode" + Util::toStringInt(vineID));
    
    tip = entireVine->createChildSceneNode("vineTipNode" + Util::toStringInt(vineID));
    
    Ogre::Plane plane;
    plane.normal = Ogre::Vector3::UNIT_Z;
    plane.d = 0;
    
    float bee_plane_size = 5.0f;
    
    Ogre::MeshPtr planeMeshPtr = Ogre::MeshManager::getSingleton().createPlane("beePlane",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, bee_plane_size, bee_plane_size, 10, 10, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
    
    Entity* tipEntity = tip->getCreator()->createEntity("vineTipEntity" + Util::toStringInt(vineID), planeMeshPtr);
    tip->attachObject(tipEntity);
    
    tipEntity->getSubEntity(0)->setMaterialName("Bee/Center2");
    
    boost = tip->createChildSceneNode("boostNode" + Util::toStringInt(vineID));
    boost->translate(0.0, 0.0, 1.0);
    
    Ogre::Plane shadowPlane;
    shadowPlane.normal = Ogre::Vector3::UNIT_Z;
    shadowPlane.d = 0;
    
    float bee_shadow_size = 2.0f;
    
    Ogre::MeshPtr shadowPlaneMeshPtr = Ogre::MeshManager::getSingleton().createPlane("beeShadowPlane",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, shadowPlane, bee_shadow_size, bee_shadow_size, 10, 10, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
    
    Entity* tipShadowEntity = entireVine->getCreator()->createEntity("shadowEntity" + Util::toStringInt(vineID), shadowPlaneMeshPtr);
    shadow = entireVine->createChildSceneNode("shadowNode");
    shadow->attachObject(tipShadowEntity);
    
    shadow->translate(Vector3(0,-4.5,0));
    shadow->pitch(Ogre::Degree(45));
    
    tipShadowEntity->getSubEntity(0)->setMaterialName("Bee/Shadow");
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
        boostEffect = parentNode->getCreator()->createParticleSystem("StarBoost", "General/BeeBoost");
        boost->attachObject(boostEffect);
    }
}

void Vine::move(Vector3 delta)
{
	entireVine->translate(delta);
}


// Variables used for Bee Animations (We should probably find a better place for these. I am hesitant to store them in the Globals Namespace since they are only needed in Vine::update()). Maybe making them static variables in the function will suffice????  --Manny
int next = 0;                       // State machine index control for animation sequences
bool leftAnimation = false;         // Flag used to animate through "turn left" animation sequence
bool rightAnimation = false;        // Flag used to animate through "turn right" animation sequence
int left_right_loop_counter = 0;    // Counter to handle repeating the left/right animation texture loop
int left_right_loop_amount = 1;     // Amount to repeat left/right animation texture loop

float cumulative_elapsed_time = 0.0f;   // Used to keep track of time and animate a sinusoidal bobbing bee

void Vine::update(float elapsed)
{
    tip->resetOrientation();
    if (wobbling)
    {
        totalElapsed += elapsed;
        wobbleSpeed += elapsed;
        if (wobbleSpeed > 2.0)
            wobbleSpeed = 2.0;
        tip->roll(15 * Degree(sin(64 * wobbleSpeed * totalElapsed)));
        if (totalElapsed >= 0.5)
            setWobble(false);
    }
    
    // ----------------- Bobbing Animation for Bee -------------------------
    
    cumulative_elapsed_time += elapsed;
    
    float amplitude = 1.0f; // Adjusts the amplitude of the bobbing animation of the Bee
    float speed = 1.2f;     // Adjusts the speed of the bobbing animation of the Bee
    
    float newY = amplitude * Ogre::Math::Sin(cumulative_elapsed_time * Math::PI * speed);   // Calculate position in sinusoid using the total cumulative elapsed time
    tip->setPosition(Vector3(0,newY,0));   // Translate the Bee's Y-Position to simulate bobbing in the wind
    
    
    float sizeAmplitude = 0.15f; // Adjusts the amplitude of the shadow size animation
    float newSize = sizeAmplitude * Ogre::Math::Sin(cumulative_elapsed_time * Math::PI * speed);
    
    shadow->setScale((sizeAmplitude-newSize)+0.85f, (sizeAmplitude-newSize)+0.85f, 0);
    
    // ---------------------------------------------------------------------
    
    
    
    // ---------------- Texture Animation for Bee --------------------------
    
    Entity* tipEntity = tip->getCreator()->getEntity("vineTipEntity1");
    
    std::string mat;
    

    if( globals.moveLeft ) {    // If player just signaled to move left, set left animation flag to sequence through the turning left animation
        next = 0;
        leftAnimation = true;
        globals.moveLeft = false;
    }
    else if( globals.moveRight ) {   // If player just signaled to move right, set right animation flag to sequence through the turning right animation
        next = 0;
        rightAnimation = true;
        globals.moveRight = false;
    }
    
    if( leftAnimation ) {
        switch(next) {  // State machine for turning left animation
            case 0:
                mat = "Bee/Left1";
                next = 1;
                break;
            case 1:
                mat = "Bee/Left2";
                next = 2;
                break;
            case 2:
                mat = "Bee/Left3";
                if( left_right_loop_counter < left_right_loop_amount ) {
                    left_right_loop_counter++;
                    next = 0;
                }
                else {
                    left_right_loop_counter = 0;
                    next = 0;
                    leftAnimation = false;
                }
                break;
            default:
                mat = "Bee/Left1";
                next = 0;
                leftAnimation = false;
                break;
        }
    }
    else if( rightAnimation ) {
        switch(next) {  // State machine for turning right animation
            case 0:
                mat = "Bee/Right1";
                next = 1;
                break;
            case 1:
                mat = "Bee/Right2";
                next = 2;
                break;
            case 2:
                mat = "Bee/Right3";
                if( left_right_loop_counter < left_right_loop_amount ) {
                    left_right_loop_counter++;
                    next = 0;
                }
                else {
                    left_right_loop_counter = 0;
                    next = 0;
                    rightAnimation = false;
                }
                break;
            default:
                mat = "Bee/Right1";
                next = 0;
                rightAnimation = false;
                break;
        }
    }
    else {
        switch(next) {  // State machine for normal (aka center) animation
            case 0:
                mat = "Bee/Center1";
                next = 1;
                break;
            case 1:
                mat = "Bee/Center2";
                next = 2;
                break;
            case 2:
                mat = "Bee/Center3";
                next = 3;
                break;
            case 3:
                mat = "Bee/Center2";
                next = 0;
                break;
            default:
                mat = "Bee/Center1";
                break;
        }
    }
    
    tipEntity->getSubEntity(0)->setMaterialName(mat);
    
    // ------------------------------------------------------------------------
    
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
        boost->detachObject(boostEffect);
        boost->getCreator()->destroyParticleSystem(boostEffect);
        boostEffect = NULL;
    }
}

void Vine::removeFromScene()
{
    if (boost)
    {
        removeBoost();
        boost->removeAndDestroyAllChildren();
        boost->getCreator()->destroySceneNode(boost);
        boost = NULL;
    }
    if (base)
    {
        base->getCreator()->destroyMovableObject(base->getAttachedObject(0)); // Assuming only one entity
        base->removeAndDestroyAllChildren();
        base->getCreator()->destroySceneNode(base);
        base = NULL;
    }
    if (tip)
    {
        removeBoost();
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
        entireVine->removeAndDestroyAllChildren();
        entireVine->getCreator()->destroySceneNode(entireVine);
        entireVine = NULL;
    }
}
