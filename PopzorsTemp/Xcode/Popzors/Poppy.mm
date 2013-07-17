#include "Poppy.h"

Poppy::Poppy(Vector3 pos, Color baseColor, Color blinkColor, Number blinktime)
	: Selectable(baseColor, blinkColor, blinktime), popId(-1), pos(pos), potIdRef(-1)
{
	body = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 0.5, 0.5, 0.5);
	this->setPosition(pos);
}

Poppy::~Poppy()
{
}

void Poppy::setId(int val)
{
    this->popId = val;
}

void Poppy::setColor(int r, int g, int b, int a)
{
	body->setColor(Color(r, g, b, a));
}

void Poppy::setColor(Color color)
{
	body->setColor(color);
}

void Poppy::setPosition(Number x, Number y, Number z)
{
	body->setPosition(x, y, z);
}
    
void Poppy::setPosition(Vector3 vec)
{
	body->setPosition(vec);
}

void Poppy::setPotIdRef(int val)
{
    potIdRef = val;
}

void Poppy::handleCollision(Number elapsed, CollisionScene *scene, Poppy & rhs)
{
    const Number RESOLUTION_SPEED = 5;
    
    CollisionResult res = scene->testCollision(body, rhs.body);
    if (res.collided)
    {
        Vector3 lhsPos = body->getPosition();
        Vector3 rhsPos = rhs.body->getPosition();
        if ((rhsPos - lhsPos).dot(res.colNormal) >= 0)
        {
            body->Translate(res.colNormal * RESOLUTION_SPEED * elapsed * res.colDist / 2);
            rhs.body->Translate(res.colNormal * -RESOLUTION_SPEED * elapsed * res.colDist / 2);
        }
        else
        {
            body->Translate(res.colNormal * -RESOLUTION_SPEED * elapsed * res.colDist / 2);
            rhs.body->Translate(res.colNormal * RESOLUTION_SPEED * elapsed * res.colDist / 2);
        }
    }
}

void Poppy::addToCollisionScene(CollisionScene * scene)
{
	scene->addCollisionChild(body, CollisionSceneEntity::SHAPE_BOX);
}

void Poppy::removeFromCollisionScene(CollisionScene * scene)
{
    scene->removeEntity(body);
    delete body;
}

bool Poppy::hasEntity(SceneEntity * entity)
{
	if (body == entity) return true;
	return false;
}

void Poppy::reset()
{
    Blinkable::reset();
    Selectable::reset();
    body->setPosition(pos);
    selectable = true;
    potIdRef = -1;
}

void Poppy::update(Number elapsed)
{
    Blinkable::update(elapsed);
    Selectable::update(elapsed);
}
