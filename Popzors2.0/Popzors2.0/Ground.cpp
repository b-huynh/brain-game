#include "Ground.h"

Ground::Ground(Vector3 pos, Ogre::ColourValue baseColor,
               Ogre::ColourValue blinkColor, double blinktime)
    : Selectable(TYPE_GROUND, pos, baseColor, blinkColor, blinktime)
{
    Plane plane (Vector3::UNIT_Y, 0);
    OgreFramework::getSingletonPtr()->m_pMeshMgr->createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                              plane, 12, 12, 20, 20, true, 1, 12, 12, Ogre::Vector3::UNIT_Z);
    body = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("GroundEntity_" + toStringInt(sceneID), "ground");
    body->setUserAny(Any((Selectable*)(this)));
    addToScene();
    setColor(baseColor);
    body->setMaterialName("General/PodUnknown");
    setPosition(pos);
}

Ground::~Ground()
{
}

void Ground::setMaterialByColor(Ogre::ColourValue color)
{
    if (currentColor == color)
        return;
    if (color == FEEDBACK_COLOR_BAD) {
        currentColor = color;
        body->setMaterialName("General/PodRed");
    }
    else if (color == FEEDBACK_COLOR_GOOD) {
        currentColor = color;
        body->setMaterialName("General/PodGreen");
    }
    else {
        currentColor = color;
        body->setMaterialName("General/PodUnknown");
    }
    setColor(color);
}

void Ground::addToScene()
{
    mainNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("GroundNode_" + toStringInt(sceneID));
    mainNode->attachObject(body);
}

void Ground::removeFromScene()
{
    OgreFramework::getSingletonPtr()->m_pSceneMgr->destroySceneNode("GroundNode_" + toStringInt(sceneID));
}

