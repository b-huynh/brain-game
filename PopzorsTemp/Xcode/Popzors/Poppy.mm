#include "Poppy.h"

Poppy::Poppy(Vector3 pos, Color baseColor, Color blinkColor, Number blinktime)
	: Selectable(baseColor, blinkColor, blinktime), popId(-1), pos(pos), potIdRef(-1), jumping(false), timeJumped(0), moving(false), dest(pos), stopJumpAtDest(false), moveSpeed(1)
{
	body = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, 0.25, 10, 10);
	body->setPosition(pos);
    //this->setColor(baseColor);
    body->setMaterialByName("PodBlueMat");
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
    //body->setColor(Color(r,g,b,a));
	body->setMaterialByName(getTextureNameByColor(Color(r,g,b,a)));
}

void Poppy::setColor(Color color)
{
    //body->setColor(color);
	body->setMaterialByName(getTextureNameByColor(color));
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

void Poppy::setMoving(bool val)
{
    moving = val;
}

void Poppy::setDest(Vector3 val)
{
    dest = val;
}

void Poppy::setMoveSpeed(Number speed)
{
    moveSpeed = speed;
}

void Poppy::handleCollision(Number elapsed, CollisionScene *scene, Poppy* rhs)
{
    const Number RESOLUTION_SPEED = randRangeDouble(9.9, 10.1);
    
    CollisionResult res = scene->testCollision(body, rhs->body);
    if (res.collided)
    {
        Vector3 lhsPos = body->getPosition();
        Vector3 rhsPos = rhs->body->getPosition();
        
        Vector3 dmove = res.colNormal * RESOLUTION_SPEED * elapsed * res.colDist / 2;
        if (dmove.length() < 0.1)
            dmove = res.colNormal * res.colDist / 2;
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
    jumping = false;
    timeJumped = 0;
}

void Poppy::activateJump()
{
    jumping = true;
}

void Poppy::deactivateJump()
{
    jumping = false;
}

void Poppy::deactivateJumpAtDest(bool stop)
{
    stopJumpAtDest = stop;
}

void Poppy::jump(Number elapsed)
{
    const Number JUMP_HEIGHT = 0.5;
    const Number JUMP_TIME = 0.35;
    
    Number previousTime = timeJumped;
    timeJumped += elapsed;
    if (timeJumped > JUMP_TIME)
    {
        timeJumped = 0;
        //jumping = false;
    }
    
    // y = a(bx - c)^2 + d
    const Number A = -1;
    const Number B = (sqrt(-JUMP_HEIGHT / A) + sqrt(JUMP_HEIGHT)) / JUMP_TIME;
    const Number C = sqrt(JUMP_HEIGHT);
    const Number D = JUMP_HEIGHT;
    // Uses a parabolic curve to animate the jump
    Number diff = (A * pow(B * timeJumped - C, 2) + D) - (A * pow(B * previousTime - C, 2) + D);
    //Number diff = -pow(JUMP_TIME * timeJumped - JUMP_HEIGHT_SQRT, 2) + pow(JUMP_TIME * previousTime - JUMP_HEIGHT_SQRT, 2);
    //printf("%f: %f\n", timeJumped, diff);
    body->Translate(0, diff, 0);
}

void Poppy::update(Number elapsed)
{
    Blinkable::update(elapsed);
    Selectable::update(elapsed);
    if (jumping)
        jump(elapsed);
    
    // Update poppy to move towards destination if one exists
    if (moving) {
        Vector3 dist = dest - body->getPosition();
        if (dist.length() > 0.1)
        {
            dist.Normalize();
            body->Translate(dist * elapsed * moveSpeed);
        }
    }
    
    if (stopJumpAtDest){
        Vector3 dist = dest - body->getPosition();
        if (dist.length() < 0.1)
            deactivateJump();
    }
}