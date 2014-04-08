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
static int glowID = 0;


float mushroomGummyAnimationTotalTime = 0.5f;//1.1f;   // Length of time for the gummy animation
float hazard_wobble_amplitude = 0.3f;//0.2f;           // Amplitude of gummy animation
float hazard_wobble_speed = 7.0f;//6.0f;               // Speed of gummy animation
float hazard_wobble_decay_rate = 0.55f;//0.525f;         // Rate at which the amplitude of the wobble decays (higher = longer to decay)
float wobble_speed_increment = -0.075f;//0.2f;            // Increment to speed up animation as the wobble decays


Pod::Pod()
: parentNode(NULL), entirePod(NULL), stem(NULL), head(NULL), shell(NULL), mushroomGummyAnimationTimer(0.0f), mushroomGummyAnimationFlag(false), mushroomGummyAnimationDone(false), wobbleSpeed(hazard_wobble_speed), flowerSproutAnimationFlag(false), flowerHitAnimationFlag(false), flowerHitAnimationDone(false), flowerHitAnimationTimer(0.0f), flowerHitWobbleSpeed(0.0f), retractAnimationFlag(false), retractAnimationTimer(1.0)
{
}

Pod::Pod(Ogre::SceneNode* parentNode, Vector3 base, Vector3 tip, PodMeshType mtype, PodSignal podSignal, PodColor podColor, PodShape podShape, PodSound podSound, Direction loc, float stemRadius, float headRadius)
: parentNode(parentNode), mtype(mtype), materialName(""), headContentEntity(NULL), glowNode(NULL), glowEffect(NULL), base(base), tip(tip), podSignal(podSignal), podColor(podColor), podShape(podShape), podSound(podSound), stemRadius(stemRadius), stemLength(base.distance(tip)), headRadius(headRadius), entirePod(NULL), stem(NULL), head(NULL), shell(NULL), moveSpeed(0.0), rotateSpeed(0.0, 0.0, 0.0), loc(loc), podTested(false), podTaken(false), podGood(false), dest(), mushroomGummyAnimationTimer(0.0f), mushroomGummyAnimationFlag(false), mushroomGummyAnimationDone(false), wobbleSpeed(hazard_wobble_speed), flowerSproutAnimationFlag(false), flowerHitAnimationFlag(false), flowerHitAnimationDone(false), flowerHitAnimationTimer(0.0f), flowerHitWobbleSpeed(0.0f), retractAnimationFlag(false), retractAnimationTimer(1.0)
{
    loadPod();
}

void Pod::loadPod()
{
    switch (mtype)
    {
        case POD_BASIC:
            loadBasicShape();
            break;
        case POD_FUEL:
            loadFuelCell();
            break;
        case POD_FLOWER:
            loadFlower();
            break;
        default:
            loadHazard();
            break;
    }
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
    
    switch (podShape)
    {
        case POD_SHAPE_CONE:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "cylinderMesh");
            head->scale(headRadius * 1.5, headRadius * 3, headRadius * 1.5);
            break;
        case POD_SHAPE_SPHERE:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "sphereMesh");
            head->scale(headRadius, headRadius, headRadius);
//            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "column.mesh");
//            head->scale(0.1, 0.1, 0.1);
            break;
        case POD_SHAPE_DIAMOND:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "diamondMesh");
            head->scale(headRadius * 1.5, headRadius * 2, headRadius * 1.5);
            setRotateSpeed(Vector3(globals.podRotateSpeed, 0, 0));
            break;
        case POD_SHAPE_TRIANGLE:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "boxMesh");
            head->scale(headRadius, headRadius, headRadius);
            setRotateSpeed(Vector3(globals.podRotateSpeed, 0, 0));
            break;
        default:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "cylinderMesh");
            head->scale(headRadius * 1.5, headRadius * 3, headRadius * 1.5);
            break;
    }
    materialName = "General/PodUnknown";
    headContentEntity->setMaterialName(materialName);
    head->attachObject(headContentEntity);
    head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    
    setToGrowth(0.0);
}

void Pod::loadFuelCell()
{
    removeFromScene();
    
	float stemLength = base.distance(tip);
    entirePod = parentNode->createChildSceneNode("entirePodNode" + Util::toStringInt(podID));
    Vector3 v = tip - base;
    
    head = entirePod->createChildSceneNode("headNode" + Util::toStringInt(podID));
    
    switch (podShape)
    {
        case POD_SHAPE_CONE:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "FuelCell/fuelCylinder.mesh");
            break;
        case POD_SHAPE_SPHERE:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "FuelCell/fuelSphere.mesh");
            break;
        case POD_SHAPE_DIAMOND:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "FuelCell/fuelCube.mesh");
            break;
        case POD_SHAPE_TRIANGLE:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "FuelCell/fuelTri.mesh");
            break;
        default:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "FuelCell/fuelCylinder.mesh");
            break;
    }
    headContentEntity->getSubEntity(0)->setMaterialName("General/PodMetal");
    materialName = "General/PodUnknown";
    headContentEntity->getSubEntity(1)->setMaterialName(materialName);
    head->attachObject(headContentEntity);
    head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    head->setPosition(base);
    head->translate(v / 2);
    setRotateSpeed(Vector3(globals.podRotateSpeed, 0, 0));
    
    setToGrowth(0.0);
}

void Pod::loadFlower()
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
    
	float stemLength = base.distance(tip);
    //entirePod = parentNode->createChildSceneNode("entirePodNode" + Util::toStringInt(podID));
    //Vector3 v = tip - base;
    
    head = entirePod->createChildSceneNode("headNode" + Util::toStringInt(podID));
    
    switch (podShape)
    {
        case POD_SHAPE_ROSE:
        {
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "Flowers/rose.mesh");
            if (podColor == POD_COLOR_BLUE)
                headContentEntity->setMaterialName("tempiris_old");
            else if (podColor == POD_COLOR_YELLOW)
                headContentEntity->setMaterialName("tempdaisy");
            else if (podColor == POD_COLOR_GREEN)
                headContentEntity->setMaterialName("BalloonFBXASC032Flower");
            else if (podColor == POD_COLOR_ORANGE)
                headContentEntity->setMaterialName("OrangeFlower");
            break;
        }
        case POD_SHAPE_IRIS:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "Flowers/Iris.mesh");
            headContentEntity->setMaterialName("tempIris");
            if (podColor == POD_COLOR_ORANGE)
                headContentEntity->setMaterialName("OrangeFlower");
            break;
        case POD_SHAPE_BUBBLE:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "Flowers/BubbleFlower.mesh");
            if (podColor == POD_COLOR_BLUE)
                headContentEntity->setMaterialName("tempiris_old");
            else if (podColor == POD_COLOR_YELLOW)
                headContentEntity->setMaterialName("tempdaisy");
            else if (podColor == POD_COLOR_PINK)
                headContentEntity->setMaterialName("Rose");
            else if (podColor == POD_COLOR_ORANGE)
                headContentEntity->setMaterialName("OrangeFlower");
            break;
        case POD_SHAPE_DAISY:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "Flowers/daisy.mesh");
            headContentEntity->setMaterialName("tempdaisy");
            if (podColor == POD_COLOR_ORANGE)
                headContentEntity->setMaterialName("OrangeFlower");
            break;
        default:
            headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "Flowers/bud.mesh");
            if (podColor == POD_COLOR_BLUE)
                headContentEntity->setMaterialName("tempiris_old");
            else if (podColor == POD_COLOR_YELLOW)
                headContentEntity->setMaterialName("tempdaisy");
            else if (podColor == POD_COLOR_PINK)
                headContentEntity->setMaterialName("Rose");
            else if (podColor == POD_COLOR_GREEN)
                headContentEntity->setMaterialName("BalloonFBXASC032Flower");
            else if (podColor == POD_COLOR_ORANGE)
                headContentEntity->setMaterialName("OrangeFlower");
            break;
    }
    if (podColor == POD_COLOR_UNKNOWN)
        headContentEntity->setMaterialName("General/PodUnknown");
    
    head->attachObject(headContentEntity);
    head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(Vector3(0,1,1))); // We angle the flower heads 45 Degrees towards the camera for a better look //v));
    head->translate(Vector3(0,1,1));
    
    //head->setPosition(base);
    //head->translate(v / 2);
    setRotateSpeed(Vector3(globals.podRotateSpeed, 0, 0));
    
    setToGrowth(0.0);
}

void Pod::loadHazard()
{
    removeFromScene();
    
	float stemLength = base.distance(tip);
    entirePod = parentNode->createChildSceneNode("entirePodNode" + Util::toStringInt(podID));
    Vector3 v = tip - base;
    
    head = entirePod->createChildSceneNode("headNode" + Util::toStringInt(podID));
    headContentEntity = head->getCreator()->createEntity("headEntity" + Util::toStringInt(podID), "Barriers/Mushroom.mesh");
    
    SubEntity* headSub = headContentEntity->getSubEntity(1);
    SubEntity* stemSub = headContentEntity->getSubEntity(0);
    headSub->setMaterialName("tempMushroomHead");
    stemSub->setMaterialName("tempMushroomStem");

    head->attachObject(headContentEntity);
    head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(v));
    head->setPosition(base);
    //head->translate(v / 2);
    head->yaw(Degree(180.0)); // Correction for model which faces opposite direction
    
    setToGrowth(0.0);
    
}

void Pod::assertRetractAnimationFlag() {
    retractAnimationFlag = true;
}

void Pod::desertRetractAnimationFlag() {
    retractAnimationFlag = false;
}

PodMeshType Pod::getMeshType() const
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

void Pod::setToGrowth(float t)
{
    if (mtype == POD_BASIC)
    {
        entirePod->setPosition(tip);
        entirePod->translate((base - tip) * (1 - t));
        stem->setScale(Vector3(t * stemRadius, t * stemLength, t * stemRadius));
    }
    else if (mtype == POD_FUEL)
    {
        head->setScale(Vector3(headRadius / 1.5, t * headRadius / 1.5, headRadius / 1.5));
    }
    else if (mtype == POD_FLOWER)
    {
        head->setScale(Vector3(t * headRadius / 22, t * headRadius / 22, t * headRadius / 22)); // I multiplied each dimension by time t to animate the flower head growing as it enters the scene
        entirePod->setPosition(tip);
        entirePod->translate((base - tip) * (1 - t));
        stem->setScale(Vector3(t * stemRadius, t * stemLength, t * stemRadius));
        if( t >= 0.8 && t <= 0.9 && !retractAnimationFlag ) {
            flowerSproutAnimationFlag = true;
        }
    }
    else
    {
        if( t < 0.1 ) {
            float grow_t = 0.3 * (t / 0.1) + 0.7;
            head->setScale(Vector3(0.5, 0.5 * grow_t, 0.5));
        }
        else {
            if( !mushroomGummyAnimationDone ) {
                mushroomGummyAnimationFlag = true;
                mushroomGummyAnimationDone = true;
            }
        }
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

SceneNode* Pod::getEntirePod() const
{
    return entirePod;
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
    return PodInfo(podSignal, mtype, podColor, podShape, podSound, loc, podGood, podTrigger, podTaken);
}

float Pod::getStemRadius() const
{
    return stemRadius;
}

float Pod::getStemLength() const
{
    return stemLength;
}

float Pod::getHeadRadius() const
{
    return headRadius;
}

SceneNode* Pod::getGlowNode() const
{
    return glowNode;
}

bool Pod::isPodTested() const
{
    return podTested;
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

void Pod::setSkin()
{
    if (mtype == POD_BASIC)
        headContentEntity->setMaterialName(materialName);
    else if (mtype == POD_FUEL)
        // Based on Maya model, SubEntity1 is the content in the fuel cell
        headContentEntity->getSubEntity(1)->setMaterialName(materialName);
}

void Pod::takePod()
{
    if (!podTaken)
    {
        podTaken = true;
    
        if( mtype == POD_FLOWER && !flowerHitAnimationDone ) {
            flowerHitAnimationFlag = true;
            flowerHitAnimationDone = true;
        }
        
        materialName += "Transparent";
        setSkin();
    }
}

void Pod::hidePod()
{
    entirePod->setVisible(false);
}

void Pod::revealPod()
{
    entirePod->setVisible(true);
}

void Pod::uncloakPod()
{
    if (mtype == POD_HAZARD)
        return;
    switch (podColor)
    {
        case POD_COLOR_BLUE:
            materialName = "General/PodBlue";
            break;
        case POD_COLOR_GREEN:
            materialName = "General/PodGreen";
            break;
        case POD_COLOR_PINK:
            materialName = "General/PodRed";
            break;
        case POD_COLOR_YELLOW:
            materialName = "General/PodYellow";
            break;
        default:
            materialName = "General/PodWhite";
            break;
    }
    if (podTaken)
        materialName += "Transparent";
    setSkin();
    if (!podTrigger) generateGlow(podColor, podShape);
}

void Pod::generateGlow(PodColor color, PodShape shape)
{
    if( mtype == POD_FLOWER ) return;
    
    if (!glowNode)
    {
        std::string particleName = "General/GlowPod";
        Ogre::ColourValue particleValue;
        switch (color)
        {
            case POD_COLOR_BLUE:
                particleValue.r = 0.1;
                particleValue.g = 0.3;
                particleValue.b = 1.0;
                break;
            case POD_COLOR_GREEN:
                particleValue.r = 0.0;
                particleValue.g = 1.0;
                particleValue.b = 0.0;
                break;
            case POD_COLOR_PINK:
                particleValue.r = 1.0;
                particleValue.g = 0.0;
                particleValue.b = 0.0;
                break;
            case POD_COLOR_YELLOW:
                particleValue.r = 1.0;
                particleValue.g = 1.0;
                particleValue.b = 0.0;
                break;
            default:
                particleValue.r = 0.5;
                particleValue.g = 0.5;
                particleValue.b = 1.0;
                break;
        }
        
        switch (shape)
        {
            case POD_SHAPE_CONE:
                particleName += "Cylinder";
                break;
            case POD_SHAPE_DIAMOND:
                particleName += "Box";
                break;
            case POD_SHAPE_SPHERE:
                particleName += "Ellipsoid";
                break;
            case POD_SHAPE_TRIANGLE:
                particleName += "Ellipsoid"; // No ogre particle shape for tris
                break;
            default:
                particleName += "Cylinder";
                break;
        }
        
        // Make independent to avoid rotation orientions when translating.
        glowNode = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("GlowNode" + Util::toStringInt(glowID));
        glowEffect = glowNode->getCreator()->createParticleSystem("GlowEffect" + Util::toStringInt(glowID), particleName);
        ParticleEmitter* glowEmitter = glowEffect->getEmitter(0); // Assuming only one emitter
        
        glowEmitter->setColour(particleValue);
        
        glowNode->attachObject(glowEffect);
        glowNode->setPosition(head->_getDerivedPosition());
        ++glowID;
    }
}

void Pod::setDest(Vector3 value)
{
    dest = value;
}

void Pod::setMoveSpeed(float value)
{
    moveSpeed = value;
}

void Pod::setRotateSpeed(Vector3 value)
{
    rotateSpeed = value;
}

void Pod::setPodTested(bool value)
{
    podTested = value;
}

void Pod::setPodGood(bool value)
{
    podGood = value;
}

void Pod::setPodTrigger(bool value)
{
    podTrigger = value;
}

void Pod::removeGlow()
{
    if (glowNode)
    {
        glowNode->getCreator()->destroyParticleSystem(glowEffect);
        glowNode->removeAndDestroyAllChildren();
        glowNode->getCreator()->destroySceneNode(glowNode);
        glowNode = NULL;
        glowEffect = NULL;
    }
}

void Pod::removeFromScene()
{
    removeGlow();
    if (stem)
    {
        stem->getCreator()->destroyMovableObject(stem->getAttachedObject(0)); // Assuming only one entity
        stem = NULL;
    }
    if (head)
    {
        head->getCreator()->destroyEntity(headContentEntity);
        head = NULL;
        headContentEntity = NULL;
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


void Pod::update(float elapsed)
{
    if( mtype == POD_HAZARD ) {
        if( mushroomGummyAnimationFlag ) {
            mushroomGummyAnimationTimer += elapsed;
        
            if( mushroomGummyAnimationTimer > mushroomGummyAnimationTotalTime ) {
                mushroomGummyAnimationTimer = 0.0f;
                mushroomGummyAnimationFlag = false;
                wobbleSpeed = hazard_wobble_speed;
            }
            else {
                
                wobbleSpeed += wobble_speed_increment;
                
                float hazard_t_x = -1.0 * hazard_wobble_amplitude * Ogre::Math::Sin(Ogre::Math::PI*mushroomGummyAnimationTimer*wobbleSpeed) * Ogre::Math::Exp(-mushroomGummyAnimationTimer/hazard_wobble_decay_rate) + 1.0;
                float hazard_t_y = hazard_wobble_amplitude * Ogre::Math::Sin(Ogre::Math::PI*mushroomGummyAnimationTimer*wobbleSpeed) * Ogre::Math::Exp(-mushroomGummyAnimationTimer/hazard_wobble_decay_rate) + 1.0;
            
                head->setScale(Vector3(0.5 * hazard_t_x, 0.5 * hazard_t_y, 0.5));
            }
        }
    }
    
    
    if( mtype == POD_FLOWER ) {
        
        /* Bobbing forward animation
        if( flowerSproutAnimationFlag ) {
            head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(Vector3(0,1,1)));
            flowerHitAnimationTimer += elapsed;
            flowerHitWobbleSpeed += elapsed;
            if (flowerHitWobbleSpeed > 0.4)
                flowerHitWobbleSpeed = 0.4;
            if( podShape == POD_SHAPE_BUBBLE ) head->pitch(75 * Degree(sin(8 * (1.4-flowerHitWobbleSpeed) * flowerHitAnimationTimer)*Ogre::Math::Exp(-flowerHitAnimationTimer*2.8)));
            else head->pitch(80 * Degree(sin(8 * (1.4-flowerHitWobbleSpeed) * flowerHitAnimationTimer)*Ogre::Math::Exp(-flowerHitAnimationTimer*2.85)));
            if (flowerHitAnimationTimer >= 1.0) {
                head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(Vector3(0,1,1)));
                flowerSproutAnimationFlag = false;
                flowerHitAnimationTimer = 0.0;
                flowerHitWobbleSpeed = 0.0;
            }
        }
         */
        
        if( flowerSproutAnimationFlag ) {
            head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(Vector3(0,1,1)));
            flowerHitAnimationTimer += elapsed;
            flowerHitWobbleSpeed += elapsed;
            if (flowerHitWobbleSpeed > 2.0)
                flowerHitWobbleSpeed = 2.0;
            if( podShape == POD_SHAPE_DAISY ) head->roll(15 * Degree(sin(128 * flowerHitWobbleSpeed * flowerHitAnimationTimer)));
            else head->roll(10 * Degree(sin(128 * flowerHitWobbleSpeed * flowerHitAnimationTimer)));
            if (flowerHitAnimationTimer >= 0.5) {
                head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(Vector3(0,1,1)));
                flowerSproutAnimationFlag = false;
                flowerHitAnimationTimer = 0.0;
                flowerHitWobbleSpeed = 0.0;
            }
        }
        
        else if( retractAnimationFlag ) {
            retractAnimationTimer -= 7.0*elapsed;
            if( retractAnimationTimer < 0.0 ) retractAnimationFlag = false;
            else setToGrowth(retractAnimationTimer);
        }
        
        else if( flowerHitAnimationFlag ) {
            head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(Vector3(0,1,1)));
            flowerHitAnimationTimer += elapsed;
            flowerHitWobbleSpeed += elapsed;
            if (flowerHitWobbleSpeed > 2.0)
                flowerHitWobbleSpeed = 2.0;
            if( podShape == POD_SHAPE_ROSE ) head->roll(20 * Degree(sin(128 * flowerHitWobbleSpeed * flowerHitAnimationTimer)));
            else head->roll(30 * Degree(sin(128 * flowerHitWobbleSpeed * flowerHitAnimationTimer)));
            if (flowerHitAnimationTimer >= 0.5) {
                head->setOrientation(globals.tunnelReferenceUpward.getRotationTo(Vector3(0,1,1)));
                flowerHitAnimationFlag = false;
                flowerHitAnimationTimer = 0.0;
                flowerHitWobbleSpeed = 0.0;
            }
        }
        
    }
    
    
    if (glowNode)
    {
        glowNode->setOrientation(head->_getDerivedOrientation());
    }
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
