#include "Poppy.h"

Poppy::Poppy(Vector3 pos, Ogre::ColourValue baseColor, Ogre::ColourValue blinkColor, double blinktime, double radius)
: Selectable(TYPE_POPPY, pos, baseColor, blinkColor, blinktime), popId(-1), potIdRef(-1), jumping(false), timeJumped(0), moving(false), dest(pos), stopJumpAtDest(false), moveSpeed(1)
{
    //body = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("Poppy_" + toStringInt(sceneID), "poppyMesh");
    body = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("Poppy_" + toStringInt(sceneID), "ogrehead.mesh");
    body->setUserAny(Any((Selectable*)(this)));
    addToScene();
    setColor(baseColor);
    body->setMaterialName(getTextureNameByColor(baseColor));
    //mainNode->scale(radius,radius,radius);
    mainNode->scale(0.015,0.015,0.015);
    setPosition(pos);
    
    active = true;
}

Poppy::~Poppy()
{
}

void Poppy::setId(int val)
{
    this->popId = val;
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

void Poppy::setMoveSpeed(double speed)
{
    moveSpeed = speed;
}

void Poppy::handleCollision(double elapsed, Poppy* rhs)
{
    if (this == rhs) return;
    
    const double RESOLUTION_SPEED = randRangeDouble(9.9, 10.0);
    const double EPSILON = 0.001;
    
    double colDist = POPPY_RADIUS + POPPY_RADIUS - getPosition().distance(rhs->getPosition()) + EPSILON;
    Vector3 colVector = (getPosition() - rhs->getPosition()).normalisedCopy();
    Vector3 dmove = colVector * RESOLUTION_SPEED * elapsed / 2;
    colVector = colVector * colDist / 2;
    if (dmove.squaredLength() > colVector.squaredLength()) // maximum distance to move should not be exceeded
        dmove = colVector;
    dmove = Vector3(dmove.x, 0, dmove.z); // zero out the y-axis
    move(dmove);
    rhs->move(-dmove);
}

void Poppy::activateJump()
{
    jumping = true;
    timeJumped = 0;
}

void Poppy::deactivateJump()
{
    jumping = false;
}

void Poppy::deactivateJumpAtDest(bool stop)
{
    stopJumpAtDest = stop;
}

void Poppy::jump(double elapsed)
{
    const double JUMP_HEIGHT = 0.5;
    const double JUMP_TIME = 0.35;
    
    if (!jumping && timeJumped <= 0)
        return;
    
    double previousTime = timeJumped;
    timeJumped += elapsed;
    if (timeJumped > JUMP_TIME)
    {
        timeJumped = 0;
        //jumping = false;
    }
    
    // y = a(bx - c)^2 + d
    const double A = -1;
    const double B = (sqrt(-JUMP_HEIGHT / A) + sqrt(JUMP_HEIGHT)) / JUMP_TIME;
    const double C = sqrt(JUMP_HEIGHT);
    const double D = JUMP_HEIGHT;
    // Uses a parabolic curve to animate the jump
    double term1 = (A * pow(B * timeJumped - C, 2) + D);
    double term2 = (A * pow(B * previousTime - C, 2) + D);
    double diff = term1 - term2;
    //double diff = (A * pow(B * timeJumped - C, 2) + D) - (A * pow(B * previousTime - C, 2) + D);
    //Number diff = -pow(JUMP_TIME * timeJumped - JUMP_HEIGHT_SQRT, 2) + pow(JUMP_TIME * previousTime - JUMP_HEIGHT_SQRT, 2);
    //printf("%f: %f\n", timeJumped, diff);
    
    move(Vector3(0, diff, 0));
    
    /* DEBUG
    std::cout << "elapsed: " << elapsed << std::endl;
    std::cout << "timeJumped: " << timeJumped << std::endl;
    std::cout << "previousTime: " << previousTime << std::endl;
    std::cout << "Diff: " << diff << std::endl;
    std::cout << "A: " << A << std::endl;
    std::cout << "B: " << B << std::endl;
    std::cout << "C: " << C << std::endl;
    std::cout << "D: " << D << std::endl;
    std::cout << "Term1: " << term1 << std::endl;
    std::cout << "Term2: " << term2 << std::endl;
     */
}

void Poppy::setMaterialByColor(Ogre::ColourValue color)
{
    if (currentColor != color)
        body->setMaterialName(getTextureNameByColor(color));
    setColor(color);
}

void Poppy::addToScene()
{
    mainNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("PoppyNode_" + toStringInt(sceneID));
    mainNode->attachObject(body);
}

void Poppy::removeFromScene()
{
    OgreFramework::getSingletonPtr()->m_pSceneMgr->destroySceneNode("PoppyNode_" + toStringInt(sceneID));
}

void Poppy::reset()
{
    Selectable::reset();
    selectable = true;
    potIdRef = -1;
    jumping = false;
    timeJumped = 0;
}

void Poppy::update(double elapsed)
{
    Selectable::update(elapsed);
    
    jump(elapsed);
    
    // Update poppy to move towards destination if one exists
    if (moving) {
        const double WEIGHT = 0.1;
        
        Vector3 dist = dest - getPosition();
        if (dist.length() > 0.1)
        {
            dist.y = 0; // Zero out the y movement (let jumping deal with that)
            dist.normalise();
            vel += dist * WEIGHT;
            vel.normalise();
            
            //move(dist * elapsed * moveSpeed);
        }
        else
            vel = Ogre::Vector3::ZERO;
    }
    
    if (stopJumpAtDest){
        Vector3 dist = dest - getPosition();
        if (dist.length() < 0.1)
            deactivateJump();
    }
}
