#include "Pot.h"

Pot::Pot(Vector3 pos, Number radius, Color baseColor, Color blinkColor, Number blinktime, const String & soundFile)
	: Selectable(baseColor, blinkColor, blinktime), potId(-1), pos(pos), sound(NULL), radius(radius)
{
	body = new ScenePrimitive(ScenePrimitive::TYPE_UNCAPPED_CYLINDER, 0.3, radius, 100.0);
    body->setPosition(pos);
    body->backfaceCulled = false;
    this->setColor(baseColor);
    //body->setMaterialByName(getTextureNameByColor(baseColor));
    
    if (soundFile != "")
        sound = new Sound(soundFile);
}

Pot::~Pot()
{
}

bool Pot::hasEntity(SceneEntity * entity)
{
	return body == entity;
}

void Pot::setId(int val)
{
    this->potId = val;
}

void Pot::setColor(int r, int g, int b, int a)
{
	body->setColor(r,g,b,a);
}

void Pot::setColor(Color color)
{
	body->setColor(color);
}

void Pot::setPosition(Number x, Number y, Number z)
{
	body->setPosition(x,y,z);
}

void Pot::setPosition(Vector3 vec)
{
	body->setPosition(vec);
}

void Pot::handleCollision(Number elapsed, CollisionScene *scene, Pot* rhs)
{
    const Number RESOLUTION_SPEED = randRangeDouble(0.9, 1.1);;
    
    CollisionResult res = scene->testCollision(body, rhs->body);
    if (res.collided)
    {
        Vector3 lhsPos = body->getPosition();
        Vector3 rhsPos = rhs->body->getPosition();
        Vector3 dmove = res.colNormal * RESOLUTION_SPEED * res.colDist / 2;
        dmove.x += dmove.y / 2;
        dmove.z += dmove.y / 2;
        dmove.y = 0;
        
        if ((rhsPos - lhsPos).dot(res.colNormal) >= 0)
        {
            body->Translate(dmove);
            rhs->body->Translate(dmove * -1);
        }
        else
        {
            body->Translate(dmove * -1);
            rhs->body->Translate(dmove);
        }
    }
}

void Pot::addToCollisionScene(CollisionScene * scene)
{
	scene->addCollisionChild(body,CollisionSceneEntity::SHAPE_CYLINDER);
}

void Pot::removeFromCollisionScene(CollisionScene * scene)
{
    scene->removeEntity(body);
    delete body;
    delete sound;
}

void Pot::playSound()
{
    sound->Play(false);
}
               
void Pot::reset()
{
    Blinkable::reset();
    Selectable::reset();
    body->setPosition(pos);
}

void Pot::update(Number elapsed)
{
    Blinkable::update(elapsed);
    Selectable::update(elapsed);
}