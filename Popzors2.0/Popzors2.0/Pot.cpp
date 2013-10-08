#include "Pot.h"

Pot::Pot(Vector3 pos, double radius, Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime, const std::string & soundFile)
: Selectable(baseColor, blinkColor, blinktime), potId(-1), pos(pos), radius(radius) //, sound(NULL)
{
    body = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("PotEntity_" + toStringInt(sceneID), "potMesh");
    body->setUserAny(Any((Selectable*)(this)));
    this->addToScene();
    this->setPosition(pos);
    this->setColor(baseColor);
    body->setMaterialName(getTextureNameByColor(baseColor));
    potNode->scale(radius,POT_HEIGHT,radius);
    
    //if (soundFile != "")
    //    sound = new Sound(soundFile);
}

Pot::~Pot()
{
}

bool Pot::hasEntity(Entity * entity)
{
	return body == entity;
}

void Pot::setId(int val)
{
    this->potId = val;
}

void Pot::setColor(Ogre::ColourValue color)
{
    if (currentColor != color)
        body->setMaterialName(getTextureNameByColor(color));
	currentColor = color;
}

void Pot::setColor(int r, int g, int b, int a)
{
    this->setColor(intToFloatColor(r,g,b,a));
}

void Pot::setPosition(Vector3 vec)
{
	potNode->setPosition(vec);
}

void Pot::setPosition(double x, double y, double z)
{
	this->setPosition(Vector3(x,y,z));
}

void Pot::setSound(const std::string & soundFile)
{
//    if (sound)
//        delete sound;
//    sound = NULL;
//    if (soundFile != "")
//        sound = new Sound(soundFile);
}

//void Pot::handleCollision(double elapsed, CollisionScene *scene, Pot* rhs)
//{
//    const double RESOLUTION_SPEED = randRangeDouble(0.9, 1.1);;
//    
//    CollisionResult res = scene->testCollision(body, rhs->body);
//    if (res.collided)
//    {
//        Vector3 lhsPos = body->getPosition();
//        Vector3 rhsPos = rhs->body->getPosition();
//        Vector3 dmove = res.colNormal * RESOLUTION_SPEED * res.colDist / 2;
//        dmove.x += dmove.y / 2;
//        dmove.z += dmove.y / 2;
//        dmove.y = 0;
//        
//        if ((rhsPos - lhsPos).dot(res.colNormal) >= 0)
//        {
//            body->Translate(dmove);
//            rhs->body->Translate(dmove * -1);
//        }
//        else
//        {
//            body->Translate(dmove * -1);
//            rhs->body->Translate(dmove);
//        }
//    }
//}

void Pot::addToScene()
{
    potNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("PotNode_" + toStringInt(sceneID));
    potNode->attachObject(body);
}

void Pot::removeFromScene()
{
    OgreFramework::getSingletonPtr()->m_pSceneMgr->destroySceneNode("PotNode_" + toStringInt(sceneID));
}

void Pot::playSound()
{
    //if (sound)
    //    sound->Play(false);
}

void Pot::reset()
{
    Blinkable::reset();
    Selectable::reset();
    potNode->setPosition(pos);
}

void Pot::update(double elapsed)
{
    Blinkable::update(elapsed);
    Selectable::update(elapsed);
}