#include "Player.h"

#include <cmath>

Player::Player()
: seed(0), name(""), hp(STARTING_HP), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), cursor(NULL), vines(), dir(SOUTH), mousePos(), oldPos(), camPos(), oldRot(), oldRoll(0), camRot(), camRoll(0), desireRot(), desireRoll(0), camSpeed(0.0), vineOffset(0), results(), totalElapsed(0), vineSlice(NULL), vineT(0.0), outfile()
{}

Player::Player(CollisionScene *scene, const string & name, Vector3 camPos, Quaternion camRot, double camSpeed, Number offset, unsigned seed, const string & filename)
	: seed(seed), name(name), hp(STARTING_HP), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), cursor(NULL), vines(), dir(SOUTH), mousePos(), oldPos(camPos), camPos(camPos), oldRot(camRot), oldRoll(0), camRot(camRot), camRoll(0), desireRot(camRot), desireRoll(0), camSpeed(camSpeed), vineOffset(offset), results(), totalElapsed(0), vineSlice(NULL), vineT(0.0), outfile()
{
	cursor = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, 0.5, 5, 5);
	cursor->renderWireframe = true;
	cursor->setColor(1.0, 0.0, 0.0, 1.0);
	scene->addChild(cursor);
    cursor->enabled = false;
    
    light = new SceneLight(SceneLight::AREA_LIGHT, scene, 5);
    light->setPosition( camPos + getCamForward() * 5 );
    scene->addLight(light);
    
    light2 = new SceneLight(SceneLight::AREA_LIGHT, scene, 3);
    light2->setPosition( camPos + getCamForward() * 10 );
    scene->addLight(light2);
    
    light3 = new SceneLight(SceneLight::AREA_LIGHT, scene, 2);
    light3->setPosition( camPos + getCamForward() * 15 );
    scene->addLight(light3);
    
    outfile.open((getSaveDir() + filename).c_str(), std::ofstream::out | std::ofstream::trunc);
    if (outfile.good()) {
        outfile << "% debug seed: " << seed << endl;
        outfile << "%" << endl;
        outfile << "% PodLocation { NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST" << endl;
        outfile << "% PodType { BLUE, GREEN, PINK, YELLOW }" << endl;
        outfile << "% Level {-inf, inf}" << endl;
        outfile << "% NBack {no, yes}" << endl;
        outfile << "% PlayerTookPod {no, yes}" << endl;
        outfile << "%" << endl;
        outfile << "% PodLocation PodType Level NBack PlayerTookPod Timestamp" << endl;
    }
}

unsigned Player::getSeed() const
{
    return seed;
}

int Player::getHP() const
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

Number Player::getTotalElapsed() const
{
    return totalElapsed;
}

void Player::setSeed(unsigned value)
{
    seed = value;
}

void Player::setHP(int value)
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

void Player::newTunnel(Tunnel* tunnel)
{
    hp = STARTING_HP;
    vineSlice = NULL;
    vineT = 0.0;
    Number tLeft;
    for (int i = 0; i < vines.size(); ++i)
    {
        TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
        if (closest) {
            Vector3 targetPos = targetPos = closest->requestPosition(closest->getCenter(tLeft), dir);
            vines[i]->setDest(targetPos);
            vines[i]->setPos(targetPos);
        }
    }
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
    Number tLeft;
    TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
	if (closest == NULL)
		return;
	for (int i = 0; i < vines.size(); ++i)
	{
		vector<Pod *> collided = closest->findCollisions(tunnel->getScene(), vines[i]->getTip());
		for (int j = 0; j < collided.size() && collided[j]->getHead()->enabled; ++j)
		{
            for (int k = 0; k < closest->getPods().size(); ++k)
                closest->getPods()[k]->getHead()->enabled = false;
            closest->setPodTaken(true);
            break;
		}
	}
}

void Player::update(Number elapsed, Tunnel *tunnel)
{
    totalElapsed += elapsed;
    
    // Speed up, slow down keys
    double moveSpeed = camSpeed;
   
    if (tunnel->isDone())
        moveSpeed *= 2.5;
    else
    {
        if (keyUp)
            moveSpeed *= 2;
        if (keyDown)
            moveSpeed /= 2;
    }
    
    if (!tunnel->isDone()) {
        //hp -= DRAIN_SPEED * moveSpeed * elapsed;
        score += tunnel->getNBack() * moveSpeed * elapsed;
    }
    
    for (int i = 0; i < vines.size(); ++i)
    {
        // Determine which tunnel segment and corner each vine should be
        // in. This is done by calculating a t from 0 to 1
        Number tLeft;
        TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
        if (closest) {
            if (closest != vineSlice)
            {
                vineT = tLeft;
                vineSlice = closest;
            }
            Vector3 targetPos = targetPos = closest->requestPosition(closest->getCenter(tLeft), dir);
            vines[i]->setDest(targetPos);
            if ((vines[i]->getPos() - camPos).dot(getCamForward()) < 0)
                vines[i]->setPos(targetPos);
            //cursor->setPosition(targetPos);
        }
        
        closest = tunnel->findSliceFromCurrent(vines[i]->getPos(), 0, tLeft);
        if (closest && closest->getType() < NORMAL_BLANK && !closest->isInfoStored() && !tunnel->isDone()) {
            Number t = closest->getT(vines[i]->getPos() - closest->getForward() * vines[i]->getRadius());
            
            if (t > 0.5) {
                // This code block is to record data of the pods
                Result result;
                result.timestamp = (int)(totalElapsed * 1000);
                result.sectionInfo = closest->getSectionInfo();
                result.podInfo = closest->getPodInfo();
                result.playerTookPod = closest->isPodTaken();
                
                PodType NBack = tunnel->getNBackTest(tunnel->getPodIndex());
                result.nback = tunnel->getNBack();
                result.goodPod = NBack != POD_NONE;
                
                if (outfile.good())
                {
                    outfile << result.podInfo.podLoc << " "
                            << result.podInfo.podType << " "
                            << result.nback << " "
                            << result.goodPod << " "
                            << result.playerTookPod << " "
                            << result.timestamp << endl;
                }
                
                results.push_back(result);
                
                // Determine whether the player got it right or not
                if (result.goodPod && result.playerTookPod) {
                    if (hp < 0)
                        hp = 0;
                    hp++;
                    if (hp > HP_POSITIVE_LIMIT)
                        hp = HP_POSITIVE_LIMIT;
                }
                else if ((result.goodPod && !result.playerTookPod) ||
                         (!result.goodPod && result.playerTookPod)) {
                    if (hp > 0)
                        hp = 0;
                    hp--;
                    if (hp < HP_NEGATIVE_LIMIT)
                        hp = HP_NEGATIVE_LIMIT;
                }
                
                // Flag to trigger only once
                closest->setInfoStored(true);
            }
        }
    }

    // Linearly interpoloate the camera to get smooth transitions
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
    // Animate camera rolls
    if (camRoll < desireRoll) {
        camRoll += max(5, (desireRoll - camRoll) / 2);
    }
    if (camRoll > desireRoll) {
        camRoll -= max(5, (camRoll - desireRoll) / 2);
    }
    
	for (int i = 0; i < vines.size(); ++i)
    {
        vines[i]->speed = 30;
		vines[i]->update(elapsed);
    }
}

//Returns false if failed to save to file, true otherwise
bool Player::saveProgress(std::string file)
{
    file = getSaveDir() + file;
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    
    if (out.good()) {
        out << "% debug seed: " << seed << endl;
        out << "%" << endl;
        out << "% PodLocation { NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST" << endl;
        out << "% PodType { BLUE, GREEN, PINK, YELLOW }" << endl;
        out << "% Level {-inf, inf}" << endl;
        out << "% NBack {no, yes}" << endl;
        out << "% PlayerTookPod {no, yes}" << endl;
        out << "%" << endl;
        out << "% PodLocation PodType Level NBack PlayerTookPod Timestamp" << endl;
        
        for (int i = 0; i < results.size(); ++i) {
            out << results[i].podInfo.podLoc << " "
                << results[i].podInfo.podType << " "
                << results[i].nback << " "
                << results[i].goodPod << " "
                << results[i].playerTookPod << " "
                << results[i].timestamp << endl;
        }
        out.close();
    }
    else {
        out.close();
        return false;
    }
    
    return true;
}

Player::~Player()
{
    outfile.close();
}
