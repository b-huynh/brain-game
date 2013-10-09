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

void Pot::handleCollision(double elapsed, Pot* rhs)
{
    if (this == rhs)
        return;
    
    // Use the collision algorithm for spheres while zeroing out the y-axis since it is sufficient for our needs
    const double RESOLUTION_SPEED = randRangeDouble(2.9, 3.0);
    const double EPSILON = 0.001;
    
    double colDist = POT_RADIUS + POT_RADIUS - this->getPosition().distance(rhs->getPosition()) + EPSILON;
    Vector3 colVector = (this->getPosition() - rhs->getPosition()).normalisedCopy();
    Vector3 dmove = colVector * RESOLUTION_SPEED * elapsed / 2;
    colVector = colVector * colDist / 2;
    if (dmove.squaredLength() > colVector.squaredLength()) // maximum distance to move should not be exceeded
        dmove = colVector;
    dmove = Vector3(dmove.x, 0, dmove.z); // zero out the y-axis
    move(dmove);
    rhs->move(-dmove);
}

void Pot::playSound() const
{
    //if (sound)
    //    sound->Play(false);
}

void Pot::addToScene()
{
    potNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("PotNode_" + toStringInt(sceneID));
    potNode->attachObject(body);
}

void Pot::removeFromScene()
{
    OgreFramework::getSingletonPtr()->m_pSceneMgr->destroySceneNode("PotNode_" + toStringInt(sceneID));
}

void Pot::move(const Vector3 & dValue)
{
    potNode->translate(dValue);
}

bool Pot::hasEntity(Entity * entity)
{
	return body == entity;
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