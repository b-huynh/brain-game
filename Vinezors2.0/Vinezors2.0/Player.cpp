//
//  Player.cpp
//
//  Created by Calvin Phung on 9/13/13.
//
//

#include "Player.h"

#include <cmath>

using namespace std;

Player::Player()
: seed(0), name(""), hp(Util::STARTING_HP), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), vines(), camDir(SOUTH), vineDir(SOUTH), mousePos(), oldPos(), camPos(), oldRot(), oldRoll(0), camRot(), camRoll(0), desireRot(), desireRoll(0), camSpeed(0.0), vineOffset(0), level(), results(), totalElapsed(0), vineSlice(NULL), vineT(0.0)
{}

Player::Player(const std::string & name, const PlayerLevel & level, Vector3 camPos, Quaternion camRot, int camSpeed, double offset, unsigned seed, const std::string & filename)
: seed(seed), name(name), hp(Util::STARTING_HP), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), vines(), camDir(SOUTH), vineDir(SOUTH), mousePos(), oldPos(camPos), camPos(camPos), oldRot(camRot), oldRoll(0), camRot(camRot), camRoll(0), desireRot(camRot), desireRoll(0), camSpeed(camSpeed), vineOffset(offset), level(level), results(), totalElapsed(0), vineSlice(NULL), vineT(0.0)
{
}

unsigned Player::getSeed() const
{
    return seed;
}

std::string Player::getName() const
{
    return name;
}

int Player::getHP() const
{
    return hp;
}

double Player::getScore() const
{
	return score;
}

Direction Player::getCamDir() const
{
	return camDir;
}

Direction Player::getVineDir() const
{
	return vineDir;
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

// http://nic-gamedev.blogspot.com/2011/11/quaternion-math-getting-local-axis.html
// link is the second method and is supposedly faster.
Vector3 Player::getCamForward(bool combined) const
{
    Quaternion rot = combined ? getCombinedRotAndRoll() : camRot;
    Quaternion forward = Quaternion(0, 0, 0, -1);
    forward = rot * forward * rot.Inverse();
    return Vector3(forward.x, forward.y, forward.z);
    //return Vector3(-2 * (camRot.x * camRot.z + camRot.w * camRot.y),
    //               -2 * (camRot.y * camRot.x - camRot.w * camRot.x),
    //               -1 + 2 * (camRot.x * camRot.x + camRot.y * camRot.y));
}
Vector3 Player::getCamUpward(bool combined) const
{
    Quaternion rot = combined ? getCombinedRotAndRoll() : camRot;
    Quaternion upward = Quaternion(0, 0, 1, 0);
    upward = rot * upward * rot.Inverse();
    return Vector3(upward.x, upward.y, upward.z);
    //return Vector3(2 * (camRot.x * camRot.y - camRot.w * camRot.z),
    //               1 - 2 * (camRot.x * camRot.x + camRot.z * camRot.z),
    //               2 * (camRot.y * camRot.z + camRot.w * camRot.x));
}
Vector3 Player::getCamRight(bool combined) const
{
    Quaternion rot = combined ? getCombinedRotAndRoll() : camRot;
    Quaternion right = Quaternion(0, 1, 0, 0);
    right = rot * right * rot.Inverse();
    return Vector3(right.x, right.y, right.z);
    //return Vector3(1 - 2 * (camRot.y * camRot.y + camRot.z * camRot.z),
    //               2 * (camRot.x * camRot.y + camRot.w * camRot.z),
    //               2 * (camRot.x * camRot.z - camRot.w * camRot.y));
}

int Player::getCamSpeed() const
{
    return camSpeed;
}

Vector3 Player::getVineOffset() const
{
	return getCamForward() * vineOffset;
}

PlayerLevel Player::getLevel() const
{
    return level;
}

double Player::getTotalElapsed() const
{
    return totalElapsed;
}

void Player::setSeed(unsigned value)
{
    seed = value;
}

void Player::setName(const std::string & value)
{
    name = value;
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

void Player::setCamDir(Direction value)
{
	vineDir = value;
}

void Player::setVineDir(Direction value)
{
	vineDir = value;
}

void Player::setVineDirRequest(Direction value, Tunnel* tunnel)
{
    if (tunnel->hasAvailableSide(value))
        vineDir = value;
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

void Player::setCamSpeed(int value)
{
    camSpeed = value;
}

void Player::newTunnel(Tunnel* tunnel)
{
    hp = Util::STARTING_HP;
    vineSlice = NULL;
    vineT = 0.0;
    double tLeft;
    
    
    for (int i = 0; i < vines.size(); ++i)
    {
        TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
        if (closest) {
            Vector3 targetPos = targetPos = closest->requestPosition(closest->getCenter(tLeft), vineDir);
            vines[i]->setDest(targetPos);
            vines[i]->setPos(targetPos);
        }
    }
}

void Player::move(Vector3 delta)
{
	camPos += delta;
    // vines move independently and have their own destination
}

Quaternion Player::getCombinedRotAndRoll() const
{
    Quaternion q;
    q.FromAngleAxis(Degree(camRoll), Util::TUNNEL_REFERENCE_FORWARD);
    return camRot * q;
}

void Player::addVine(Vine *vine)
{
	vines.push_back(vine);
}

void Player::checkCollisions(Tunnel *tunnel)
{
    double tLeft;
    TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
	if (closest == NULL)
		return;
	for (int i = 0; i < vines.size(); ++i)
	{
        std::vector<Pod*> collided = closest->findCollisions(vines[i]->getTip());
		for (int j = 0; j < collided.size() && !collided[j]->isPodTaken(); ++j)
		{
            collided[j]->takePod();
            
            PodInfo info = closest->getPodInfo();
            info.podTaken = true;
            closest->setPodInfo(info);
            break;
		}
	}
}

void Player::update(double elapsed, Tunnel *tunnel)
{
    totalElapsed += elapsed;
    
    // Speed up, slow down keys
    double moveSpeed = camSpeed;
    
    if (tunnel->isDone())
        moveSpeed = Util::MAX_CAM_SPEED * 2;
    else {
        //hp -= DRAIN_SPEED * moveSpeed * elapsed;
        score += tunnel->getNBack() * moveSpeed * elapsed;
    }
    
    for (int i = 0; i < vines.size(); ++i)
    {
        //// Hardcode the vine where to go
        //if (keyLeft)
        //    setDir(WEST);
        //else
        //    setDir(SOUTH);
        
        // Determine which tunnel segment and corner each vine should be
        // in. This is done by calculating a t from 0 to 1
        double tLeft;
        TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
        if (closest) {
            Vector3 targetPos;
            if (tLeft >= 0.0) {
                vineSlice = closest;
                targetPos = closest->getCenter(tLeft) + closest->requestMove(vineDir);
            } else {
                if (vineSlice) {
                    if (closest->getPrerangeT() == 0.0)
                        closest->setPrerangeT(tLeft);
                    Vector3 p1 = vineSlice->getEnd() + vineSlice->requestMove(vineDir);
                    Vector3 p2 = closest->getStart() + closest->requestMove(vineDir);
                    
                    targetPos = p1 + (p2 - p1) * (1.0 + tLeft);
                    //targetPos = p1 + (p2 - p1) * ((closest->getPrerangeT() - tLeft) / closest->getPrerangeT());
                    
                } else {
                    targetPos = closest->getCenter(tLeft) + closest->requestMove(vineDir);
                }
            }
            vines[i]->setDest(targetPos);
            //vines[i]->setPos(targetPos);
            vines[i]->setForward(closest->getForward());
        }
        
        closest = tunnel->findSliceFromCurrent(vines[i]->getPos(), 0, tLeft);
        if (closest && closest->getType() < NORMAL_BLANK && !closest->isInfoStored() && !tunnel->isDone()) {
            double t = closest->getT(vines[i]->getPos() - closest->getForward() * vines[i]->getRadius());
            
            // If player vine is halfway through a segment with a pod, we can get results
            if (t > 0.5) {
                // This code block is to record data of the pods
                Result result;
                result.timestamp = (int)(totalElapsed * 1000);
                result.sectionInfo = closest->getSectionInfo();
                result.podInfo = closest->getPodInfo();
                result.nback = tunnel->getNBack();
                results.push_back(result);
                
                // Determine whether the player got it right or not
                if (result.podInfo.goodPod && result.podInfo.podTaken) {
                    if (hp < 0)
                        hp = 0;
                    hp++;
                    if (hp > Util::HP_POSITIVE_LIMIT)
                        hp = Util::HP_POSITIVE_LIMIT;
                }
                else if ((result.podInfo.goodPod && !result.podInfo.podTaken) ||
                         (!result.podInfo.goodPod && result.podInfo.podTaken)) {
                    if (hp > 0)
                        hp = 0;
                    hp--;
                    if (hp < Util::HP_NEGATIVE_LIMIT)
                        hp = Util::HP_NEGATIVE_LIMIT;
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
        Vector3 endOfSlice = next->getEnd();
        Vector3 dir = (endOfSlice - camPos).normalisedCopy();
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
        vines[i]->speed = moveSpeed / 2;
		vines[i]->update(elapsed);
    }
    
}

void Player::evaluatePlayerLevel(bool pass)
{
    if (pass) {
        ++level.control;
        if (level.control > 4)
        {
            level.control = 1;
            ++level.nback;
        }
    } else {
        --level.control;
        if (level.control <= 0)
        {
            if (level.nback != 0)
                level.control = 4;
            else
                level.control = 1;
            
            if (level.nback > 0)
                --level.nback;
        }
    }
}

//Returns false if failed to save to file, true otherwise
bool Player::saveProgress(std::string file)
{
    file = Util::getSaveDir() + file;
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    
    if (out.good()) {
        out << "% debug seed: " << seed << endl;
        out << "%" << endl;
        out << "% PodLocation { NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST }" << endl;
        out << "% PodType { BLUE, GREEN, PINK, YELLOW }" << endl;
        out << "% Level {-inf, inf}" << endl;
        out << "% NBack {no, yes}" << endl;
        out << "% PlayerTookPod {no, yes}" << endl;
        out << "%" << endl;
        out << "% PodLocation PodType Level NBack PlayerTookPod Timestamp" << endl;
        
        for (int i = 0; i < results.size(); ++i) {
            //            out << results[i].podInfo.podLoc << " "
            out << SOUTH << " "
            << results[i].podInfo.podType << " "
            << results[i].nback << " "
            << results[i].podInfo.goodPod << " "
            << results[i].podInfo.podTaken << " "
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
}
