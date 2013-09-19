#include "Ground.h"

Ground::Ground(Vector3 pos, Ogre::ColourValue baseColor,
               Ogre::ColourValue blinkColor, double blinktime)
    : Blinkable(baseColor, blinkColor, blinktime), pos(pos)
{
    Plane plane (Vector3::UNIT_Y, 0);
    OgreFramework::getSingletonPtr()->m_pMeshMgr->createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                              plane, 5, 5, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    
    body = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("GroundEntity_" + toStringInt(sceneID), "ground");
    this->setColor(baseColor);
    this->addToScene();
    groundNode->setPosition(pos);
}

Ground::~Ground()
{
}

void Ground::setColor(Ogre::ColourValue color)
{
    if (color == FEEDBACK_COLOR_BAD) {
        currentColor = color;
        body->setMaterialName("General/PodRed");
    }
    else if (color == FEEDBACK_COLOR_GOOD) {
        currentColor = color;
        body->setMaterialName("General/PodGreen");
    }
    else {
        currentColor = GROUND_COLOR;
        body->setMaterialName("General/PodYellow");
    }
}

void Ground::setColor(int r, int g, int b, int a)
{
    Ogre::ColourValue color = intToFloatColor(r,g,b,a);
    this->setColor(color);
}

void Ground::addToScene()
{
    groundNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("GroundNode_" + toStringInt(sceneID));
    groundNode->attachObject(body);
}

void Ground::removeFromScene()
{
    OgreFramework::getSingletonPtr()->m_pSceneMgr->destroySceneNode("GroundNode_" + toStringInt(sceneID));
}

bool Ground::hasEntity(Entity* entity)
{
    return body == entity;
}

void Ground::reset()
{
    Blinkable::reset();
    groundNode->setPosition(pos);
}

void Ground::update(double elapsed)
{
    Blinkable::update(elapsed);
}

