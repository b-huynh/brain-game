#include "Player.h"

Player::Player()
	: name(""), hp(STARTING_HP), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), cursor(NULL), vines(), dir(SOUTH), mousePos(), oldPos(), camPos(), oldRot(), oldRoll(0), camRot(), camRoll(0), desireRot(), desireRoll(0), camSpeed(0.0), vineOffset(0)
{}

Player::Player(CollisionScene *scene, const string & name, Vector3 camPos, Quaternion camRot, double camSpeed, Number offset)
	: name(name), hp(STARTING_HP), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), cursor(NULL), vines(), dir(SOUTH), mousePos(), oldPos(camPos), camPos(camPos), oldRot(camRot), oldRoll(0), camRot(camRot), camRoll(0), desireRot(camRot), desireRoll(0), camSpeed(camSpeed), vineOffset(offset)
{
	cursor = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, 0.5, 5, 5);
	cursor->renderWireframe = true;
	cursor->setColor(1.0, 0.0, 0.0, 1.0);
	scene->addChild(cursor);
    cursor->enabled = false;
    
    light = new SceneLight(SceneLight::AREA_LIGHT, scene, 5);
    light->setPosition( camPos + getCamForward() * 3 );
    scene->addLight(light);
    
    light2 = new SceneLight(SceneLight::AREA_LIGHT, scene, 3);
    light2->setPosition( camPos + getCamForward() * 5 );
    scene->addLight(light2);
    
    light3 = new SceneLight(SceneLight::AREA_LIGHT, scene, 1);
    light3->setPosition( camPos + getCamForward() * 10 );
    scene->addLight(light3);
}

double Player::getHP() const
{
    return hp;
}

double Player::getScore() const
{
	return score;
}

Direction Player::getDir() const
{
	return dir;
}

bool Player::getMouseLeft() const
{
	return mouseLeft;
}

bool Player::getKeyUp() const
{
    return keyUp;
}

bool Player::getKeyDown() const
{
    return keyDown;
}

bool Player::getKeyLeft() const
{
    return keyLeft;
}

bool Player::getKeyRight() const
{
    return keyRight;
}

Vector2 Player::getMousePos() const
{
	return mousePos;
}

Vector3 Player::getOldPos() const
{
    return oldPos;
}

Vector3 Player::getCamPos() const
{
	return camPos;
}

Quaternion Player::getOldRot() const
{
	return oldRot;
}

int Player::getOldRoll() const
{
    return oldRoll;
}

Quaternion Player::getCamRot() const
{
	return camRot;
}

int Player::getCamRoll() const
{
    return camRoll;
}

Quaternion Player::getDesireRot() const
{
	return desireRot;
}

int Player::getDesireRoll() const
{
    return desireRoll;
}

//http://nic-gamedev.blogspot.com/2011/11/quaternion-math-getting-local-axis.html
//link is the second method and is supposedly faster.
Vector3 Player::getCamForward()
{
    Quaternion rot = getCombinedRotAndRoll();
    Quaternion forward;
    forward.set(0, 0, 0, -1);
    forward = rot * forward * rot.Inverse();
    return Vector3(forward.x, forward.y, forward.z);
    //return Vector3(-2 * (camRot.x * camRot.z + camRot.w * camRot.y),
    //               -2 * (camRot.y * camRot.x - camRot.w * camRot.x),
    //               -1 + 2 * (camRot.x * camRot.x + camRot.y * camRot.y));
}
Vector3 Player::getCamUpward()
{
    Quaternion rot = getCombinedRotAndRoll();
    Quaternion upward;
    upward.set(0, 0, 1, 0);
    upward = rot * upward * rot.Inverse();
    return Vector3(upward.x, upward.y, upward.z);
    //return Vector3(2 * (camRot.x * camRot.y - camRot.w * camRot.z),
    //               1 - 2 * (camRot.x * camRot.x + camRot.z * camRot.z),
    //               2 * (camRot.y * camRot.z + camRot.w * camRot.x));
}
Vector3 Player::getCamRight()
{
    Quaternion rot = getCombinedRotAndRoll();
    Quaternion right;
    right.set(0, 1, 0, 0);
    right = rot * right * rot.Inverse();
    return Vector3(right.x, right.y, right.z);
    //return Vector3(1 - 2 * (camRot.y * camRot.y + camRot.z * camRot.z),
    //               2 * (camRot.x * camRot.y + camRot.w * camRot.z),
    //               2 * (camRot.x * camRot.z - camRot.w * camRot.y));
}

double Player::getCamSpeed() const
{
    return camSpeed;
}

Vector3 Player::getVineOffset()
{
	return getCamForward() * vineOffset;
}

void Player::setHP(double value)
{
    hp = value;
}

void Player::setScore(double value)
{
	score = value;
}

void Player::setMouseLeft(bool value)
{
	mouseLeft = value;
}

void Player::setKeyUp(bool value)
{
	keyUp = value;
}

void Player::setKeyDown(bool value)
{
	keyDown = value;
}

void Player::setKeyLeft(bool value)
{
	keyLeft = value;
}

void Player::setKeyRight(bool value)
{
	keyRight = value;
}

void Player::setDir(Direction value)
{
	dir = value;
}

void Player::setMousePos(Vector2 value)
{
	mousePos = value;
}

void Player::setOldPos(Vector3 value)
{
	oldPos = value;
}

void Player::setCamPos(Vector3 value)
{
	camPos = value;
}

void Player::setOldRot(Quaternion value)
{
	oldRot = value;
}

void Player::setOldRoll(int value)
{
    oldRoll = value;
}

void Player::setCamRot(Quaternion value)
{
	camRot = value;
}

void Player::setCamRoll(int value)
{
    camRoll = value;
}

void Player::setDesireRot(Quaternion value)
{
	desireRot = value;
}

void Player::setDesireRoll(int value)
{
    desireRoll = value;
}

void Player::setCamSpeed(double value)
{
    camSpeed = value;
}

void Player::move(Vector3 delta)
{
	camPos += delta;
    light->setPosition( camPos + getCamForward() * 3 );
    light2->setPosition( camPos + getCamForward() * 5 );
    light3->setPosition( camPos + getCamForward() * 10 );
    // vines move independently and have their own destination
}

Quaternion Player::getCombinedRotAndRoll()
{
    Quaternion q;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, camRoll);
    return camRot * q;
}

void Player::addVine(Vine *vine)
{
	vines.push_back(vine);
}

void Player::checkCollisions(Tunnel *tunnel)	
{
    TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset);
	if (closest == NULL)
		return;
	for (int i = 0; i < vines.size(); ++i)
	{
		vector<Pod *> collided = closest->findCollisions(tunnel->getScene(), vines[i]->getTip());
		for (int j = 0; j < collided.size() && collided[j]->getHead()->enabled; ++j)
		{
            PodType NBack = tunnel->getNBackTest(tunnel->getPodIndex());
            switch (NBack)
            {
                case POD_PINK:
                    hp += 250;
                    break;
                case POD_BLUE:
                    hp += 250;
                    break;
                case POD_GREEN:
                    hp += 250;
                    break;
                case POD_YELLOW:
                    hp += 250;
                    break;
                case POD_BLACK:
                    hp -= 250;
                    break;
                case POD_NONE:
                    hp -= 250;
                    // No NBack at the end
                    break;
            }
            for (int k = 0; k < closest->getPods().size(); ++k)
                closest->getPods()[k]->getHead()->enabled = false;
            //collided[j]->getHead()->enabled = false;
            break;
		}
	}
}

void Player::update(Number elapsed, Tunnel *tunnel)
{
    // Speed up, slow down keys
    double moveSpeed = camSpeed;
    
    if (!tunnel->isDone()) {
        hp -= DRAIN_SPEED * moveSpeed * elapsed;
        score += moveSpeed * elapsed;
    }
    
    if (keyUp)
        moveSpeed *= 2;
    if (keyDown)
        moveSpeed /= 2;
    
    // Determine which tunnel segment and corner each vine should be in
    // This is done by calculating a t from 0 to 1
    for (int i = 0; i < vines.size(); ++i)
    {
        TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset);
        if (closest) {
            Number t = closest->getT(camPos) + vineOffset;
            Vector3 targetPos = closest->requestPosition(closest->getCenter(t), dir);
            vines[i]->setDest(targetPos);
            cursor->setPosition(targetPos);
        }
    }
    
    // Linearly interpoloate the camera to get smooth transitions
    /*
    TunnelSlice* current = tunnel->getCurrent();
    if (current)
    {
        Vector3 endOfSlice = current->getCenter() + current->getForward() * (tunnel->getSegmentDepth());
        Vector3 dir = endOfSlice - camPos;
        dir.Normalize();
        Vector3 delta = dir * (CAM_SPEED * elapsed);
        move(delta);
        camRot = oldRot.Slerp(1 - (endOfSlice - camPos).length() / (endOfSlice - oldPos).length(), oldRot, desireRot);
    }
    */
    TunnelSlice* next = tunnel->getNext(1);
    if (next)
    {
        Vector3 endOfSlice = next->getCenter() - next->getForward() * (tunnel->getSegmentDepth() / 2);
        Vector3 dir = endOfSlice - camPos;
        dir.Normalize();
        Vector3 delta = dir * (moveSpeed * elapsed);
        move(delta);
        camRot = oldRot.Slerp(1 - (endOfSlice - camPos).length() / (endOfSlice - oldPos).length(), oldRot, desireRot);
    }
    if (camRoll < desireRoll) {
        camRoll += max(5, (desireRoll - camRoll) / 2);
    }
    if (camRoll > desireRoll) {
        camRoll -= max(5, (camRoll - desireRoll) / 2);
    }
    
	for (int i = 0; i < vines.size(); ++i)
		vines[i]->update(elapsed);
}