//
//  Player.cpp
//
//  Created by Calvin Phung on 9/13/13.
//
//

#include "Player.h"

#include <cmath>

using namespace std;

extern Util::ConfigGlobal globals;

PlayerLevel::PlayerLevel()
: nback(globals.nback), control(globals.control)
{
    
}

Player::Player()
: seed(0), name(""), hp(globals.startingHP), numCorrectTotal(0), numWrongTotal(0), numCorrectCombo(0), numWrongCombo(0), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), lookback(NULL), vines(), movementMode(MOVEMENT_ROTATING), camDir(SOUTH), vineDir(SOUTH), mousePos(), oldPos(), camPos(), oldRot(), oldRoll(0), camRot(), camRoll(0), desireRot(), desireRoll(0), camSpeed(0.0), vineOffset(0), speedControl(SPEED_CONTROL_FLEXIBLE), level(), results(), totalElapsed(0), totalDistanceTraveled(0.0), vineSlice(NULL), vineT(0.0), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundPods(NUM_POD_TYPES), triggerStartup(true), inputTotalX(0.0), inputMoved(false)
{
    for (int i = 0; i < soundPods.size(); ++i)
        soundPods[i] = NULL;
}

Player::Player(const std::string & name, const PlayerLevel & level, Vector3 camPos, Quaternion camRot, double camSpeed, double offset, SpeedControlMode speedControl, unsigned seed, const std::string & filename)
: seed(seed), name(name), hp(globals.startingHP), numCorrectTotal(0), numWrongTotal(0), numCorrectCombo(0), numWrongCombo(0), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), lookback(NULL), vines(), movementMode(MOVEMENT_ROTATING), camDir(SOUTH), vineDir(SOUTH), mousePos(), oldPos(camPos), camPos(camPos), oldRot(camRot), oldRoll(0), camRot(camRot), camRoll(0), desireRot(camRot), desireRoll(0), camSpeed(camSpeed), vineOffset(offset), speedControl(speedControl), level(level), results(), totalElapsed(0), totalDistanceTraveled(0.0), vineSlice(NULL), vineT(0.0), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundPods(NUM_POD_TYPES), triggerStartup(true), inputTotalX(0.0), inputMoved(false)
{
    for (int i = 0; i < soundPods.size(); ++i)
        soundPods[i] = NULL;
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

int Player::getNumCorrectTotal() const
{
    return numCorrectTotal;
}

int Player::getNumWrongTotal() const
{
    return numWrongTotal;
}

int Player::getNumCorrectCombo() const
{
    return numCorrectCombo;
}

int Player::getNumWrongCombo() const
{
    return numWrongCombo;
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

double Player::getCamSpeed() const
{
    return camSpeed;
}

Vector3 Player::getVineOffset() const
{
	return getCamForward() * vineOffset;
}

SpeedControlMode Player::getSpeedControl() const
{
    return speedControl;
}

PlayerLevel Player::getLevel() const
{
    return level;
}

double Player::getTotalElapsed() const
{
    return totalElapsed;
}

double Player::getTotalDistanceTraveled() const
{
    return totalDistanceTraveled;
}

double Player::getAccuracy() const
{
    return static_cast<double>(numCorrectTotal) / (numCorrectTotal + numWrongTotal);
}

Evaluation Player::getEvaluation(GameMode emode) const
{
    if (emode == GAME_NORMAL)
    {
        if (getHP() > 0)
            return PASS;
        else
            return FAIL;
    }
    else return EVEN;
    //(emode == GAME_TIMED && player->getAccuracy() > 0.8)
    
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

void Player::setNumCorrectTotal(int value)
{
    numCorrectTotal = value;
}

void Player::setNumWrongTotal(int value)
{
    numWrongTotal = value;
}

void Player::setNumCorrectCombo(int value)
{
    numCorrectCombo = value;
}

void Player::setNumWrongCombo(int value)
{
    numWrongCombo = value;
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

bool Player::setVineDirRequest(Direction value, Tunnel* tunnel)
{
    double tLeft;
    TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
    if (closest)
    {
        if (closest->hasAvailableSide(value))
        {
            vineDir = value;
            vines[0]->loc = value;
            return true;
        }
    }
    else if (tunnel->hasAvailableSide(value))
    {
        vineDir = value;
        vines[0]->loc = value;
        return true;
    }
    return false;
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

void Player::saveCam()
{
    oldPos = camPos;
    oldRot = camRot;
    oldRoll = camRoll;
}

void Player::revertCam()
{
    camPos = oldPos;
    camRot = oldRot;
    camRoll = oldRoll;
}

void Player::newTunnel(Tunnel* tunnel, bool setmusic, bool fixspeed, bool resetscore)
{
    hp = globals.startingHP;
    if (!fixspeed)
    {
        if (tunnel->getMode() == GAME_TIMED)
            camSpeed = globals.initCamSpeed;// * globals.nlevelSpeedModifier;
        else
            camSpeed = camSpeed * globals.nlevelSpeedModifier;
    }
    if (resetscore)
        score = 0.0;
    if (camSpeed < globals.minCamSpeed)
        camSpeed = globals.minCamSpeed;
    if (camSpeed > globals.maxCamSpeed)
        camSpeed = globals.maxCamSpeed;
    camSpeed = (int)camSpeed;
    
    lookback = NULL;
    totalDistanceTraveled = 0.0;
    numCorrectTotal = 0;
    numWrongTotal = 0;
    numCorrectCombo = 0;
    numWrongCombo = 0;
    vineSlice = NULL;
    vineT = 0.0;
    double tLeft;
    
    if (setmusic)
    {
        OgreOggSound::OgreOggISound* soundMusicTemp = NULL;
        if (tunnel->getNBack() == 1)
            soundMusicTemp = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("Music1");
        else
        {
            switch ((tunnel->getNBack() + 2) % 4)
            {
                case 0:
                    soundMusicTemp = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("Music1");
                    break;
                case 1:
                    soundMusicTemp = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("Music2");
                    break;
                case 2:
                    soundMusicTemp = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("Music3");
                    break;
                case 3:
                    soundMusicTemp = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("Music4");
                    break;
            }
        }
        if (soundMusic != soundMusicTemp)
        {
            if (soundMusic) soundMusic->stop();
            soundMusic = soundMusicTemp;
        }
    }
    
    for (int i = 0; i < vines.size(); ++i)
    {
        TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
        if (closest) {
            Vector3 targetPos = targetPos = closest->requestPosition(closest->getCenter(tLeft), vineDir, closest->getWallLength() / 1.5);
            vines[i]->setDest(targetPos);
            vines[i]->setPos(targetPos);
        }
    }
    results.clear();
}

void Player::move(Vector3 delta)
{
	camPos += delta;
    // vines move independently and have their own destination
}

void Player::changeMovementMode()
{
    if (movementMode == MOVEMENT_STATIC)
        movementMode = MOVEMENT_ROTATING;
    else
        movementMode = MOVEMENT_STATIC;
}

Quaternion Player::getRot() const
{
    return camRot;
}

Quaternion Player::getRoll() const
{
    Quaternion q;
    q.FromAngleAxis(Degree(camRoll), globals.tunnelReferenceForward);
    return q;
}

Quaternion Player::getCombinedRotAndRoll() const
{
    return camRot * getRoll();
}

void Player::playPodSound(int index) const
{
    if (soundPods[index])
        soundPods[index]->play();
}

void Player::setSounds(bool mode)
{
    if (mode) // true means all pod sounds
    {
        soundFeedbackGreat = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGreatFeedback");
        soundFeedbackGood = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGoodFeedback");
        soundFeedbackBad = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundBadFeedback");
        soundPods[POD_BLUE] = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundPod1");
        soundPods[POD_GREEN] = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundPod2");
        soundPods[POD_PINK] = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundPod3");
        soundPods[POD_YELLOW] = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundPod4");
        soundStartup = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundStartup");
    }
    else // false means no pod sounds
    {
        soundFeedbackGood = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGoodFeedback");
        soundFeedbackBad = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundBadFeedback");
        for (int i = 0; i < NUM_POD_TYPES; ++i)
            soundPods[i] = NULL;
        soundStartup = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundStartup");
    }
}

void Player::addVine(Vine *vine)
{
	vines.push_back(vine);
    vine->loc = camDir;
}

void Player::checkCollisions(Tunnel *tunnel)
{
    double tLeft;
    TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
	if (closest == NULL)
		return;
	for (int i = 0; i < vines.size(); ++i)
	{
        std::vector<Pod*> collided = closest->findCollisions(vines[i]);
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

void Player::resetCursorMoved()
{
    inputTotalX = 0;
    inputMoved = false;
}

void Player::setCursorMoved()
{
    inputMoved = true;
}

void Player::updateCursorCooldown(double elapsed)
{
    // Cooldown before next swipe can be read
    if (inputMoved)
    {
        if (inputTotalX > 0.0)
        {
            inputTotalX -= globals.screenWidth / 2.0 * elapsed;
            if (inputTotalX <= 0.0)
            {
                inputTotalX = 0.0;
                inputMoved = false;
            }
        }
        else if (inputTotalX < 0.0)
        {
            inputTotalX += globals.screenWidth / 2.0 * elapsed;
            if (inputTotalX >= 0.0)
            {
                inputTotalX = 0.0;
                inputMoved = false;
            }
        }
    }
}

void Player::checkCursorMove(double dx, double dy)
{
    inputTotalX += dx;
}

bool Player::checkPerformLeftMove()
{
#if defined(OGRE_IS_IOS)
    if (!inputMoved && inputTotalX >= globals.screenWidth / 12.0)
    {
        return true;
    }
    else return false;
#else
    return true;
#endif
}

bool Player::checkPerformRightMove()
{
#if defined(OGRE_IS_IOS)
    if (!inputMoved && inputTotalX <= globals.screenWidth / 12.0)
    {
        return true;
    }
    else return false;
#else
    return true;
#endif
}

void Player::update(Tunnel* tunnel, Hud* hud, double elapsed)
{
    totalElapsed += elapsed;
    
    if (triggerStartup && soundStartup && !soundStartup->isPlaying())
    {
        soundStartup->play();
        triggerStartup = false;
    }
    if (soundMusic && !soundMusic->isPlaying() && tunnel->getTotalElapsed() > 2.0)
        soundMusic->play();
    
    // Speed up, slow down keys options
    double moveSpeed = globals.modifierCamSpeed * camSpeed;
    if (tunnel->isDone() || tunnel->getTotalElapsed() <= 0.5)
    {
        moveSpeed = globals.modifierCamSpeed * globals.maxCamSpeed * 2;
    }
    else {
        
        // Update scores if any
        double distTraveled = moveSpeed * elapsed;
        if (tunnel->getMode() == GAME_TIMED)
            score += distTraveled / 10.0;
        totalDistanceTraveled += distTraveled;
        
        if (tunnel->getMode() == GAME_TIMED)
        {
            int currentControlLevel = tunnel->getControl();
            if ((currentControlLevel == 1 && totalDistanceTraveled >= globals.timedRunControlUpDist1) ||
                (currentControlLevel == 2 && totalDistanceTraveled >= globals.timedRunControlUpDist2) ||
                (currentControlLevel == 3 && totalDistanceTraveled >= globals.timedRunControlUpDist3))
            {
                currentControlLevel++;
            }
            tunnel->setNewControl(currentControlLevel);
        }
    }
    
    for (int i = 0; i < vines.size(); ++i)
    {
        // Determine which tunnel segment and corner each vine should be
        // in. This is done by calculating a t from 0 to 1
        double tLeft;
        TunnelSlice* closest = tunnel->findSliceFromCurrent(camPos, 0, tLeft);
        if (closest && closest->getType() < NORMAL_BLANK && !closest->isPodHistory() && !tunnel->isDone())
        {
            if (tLeft > 0.5)
            {
                History* history = tunnel->getHistory();
                if (history) history->addPod(closest->getPodInfo());
                
                closest->setPodHistory(true);
            }
        }
        
        closest = tunnel->findSliceFromCurrent(camPos, vineOffset, tLeft);
        if (closest) {
            Vector3 targetPos;
            if (tLeft >= 0.0) {
                vineSlice = closest;
                targetPos = closest->getCenter(tLeft) + closest->requestMove(vineDir, closest->getWallLength() / 1.5);
            } else {
                if (vineSlice) {
                    if (closest->getPrerangeT() == 0.0)
                        closest->setPrerangeT(tLeft);
                    Vector3 p1 = vineSlice->getEnd() + vineSlice->requestMove(vineDir, closest->getWallLength() / 1.5);
                    Vector3 p2 = closest->getStart() + closest->requestMove(vineDir, closest->getWallLength() / 1.5);
                    
                    targetPos = p1 + (p2 - p1) * (1.0 + tLeft);
                    
                } else {
                    targetPos = closest->getCenter(tLeft) + closest->requestMove(vineDir, closest->getWallLength() / 1.5);
                }
            }
            vines[i]->setDest(targetPos);
            vines[i]->setForward(closest->getForward());
        }
        vines[i]->previousID = vines[i]->afterID;
        vines[i]->previoust = vines[i]->aftert;
        vines[i]->afterID = closest->getTunnelSliceID();
        vines[i]->aftert = tLeft;
        vines[i]->speed = moveSpeed;
        vines[i]->setQuaternion(getCombinedRotAndRoll());
		vines[i]->update(elapsed);
        
        // If player vine is halfway through a segment with a pod, we can get results
        //
        // Lookback is used to check for players going very fast incase they skipped the
        // other entirely.
        TunnelSlice* targetSlice = NULL;
        if (closest && closest->getType() < NORMAL_BLANK && !closest->isInfoStored() && !tunnel->isDone() && vines[i]->previoust > 0.54 && vines[i]->previousID == vines[i]->afterID)
            targetSlice = closest;
        if (lookback && lookback->getType() < NORMAL_BLANK && !lookback->isInfoStored() && !tunnel->isDone() && vines[i]->previoust > 0.54 && vines[i]->previousID != vines[i]->afterID)
            targetSlice = lookback;
        if (targetSlice) {
            //            printf("%d %f %f\n", closest->getTunnelSliceID(), vines[i]->previoust, vines[i]->aftert);
            
                History* history = tunnel->getHistory();
                
                // This code block is to record data of the pods
                Result result;
                result.stageID = globals.stageID;
                result.timestamp = (int)(totalElapsed * 1000);
                result.sectionInfo = targetSlice->getSectionInfo();
                result.podInfo = targetSlice->getPodInfo();
                result.nback = tunnel->getNBack();
                result.gameMode = tunnel->getMode();
                result.progressionMode = (ProgressionMode)globals.progressionMode;
                result.score = score;
                result.speed = camSpeed;
                results.push_back(result);
                
                //printf("%d %d\n", result.podInfo.goodPod, result.podInfo.podTaken);
                
                // Determine whether the player got it right or not
                if (result.podInfo.goodPod && result.podInfo.podTaken) {
                    if (soundFeedbackGreat)
                        soundFeedbackGreat->play();
                    hp = hp < 0 ? hp + globals.HPNegativeCorrectAnswer : hp + globals.HPPositiveCorrectAnswer;
                    if (hp > globals.HPPositiveLimit)
                        hp = globals.HPPositiveLimit;
                    ++numCorrectTotal;
                    ++numCorrectCombo;
                    numWrongCombo = 0;
                    
                    if (tunnel->getMode() == GAME_NORMAL)
                    {
                        double plus = tunnel->getNBack();
                        for (int i = 0; i < numCorrectCombo - 1 && i < globals.numToSpeedUp; ++i)
                            plus *= 2;
                        score += plus;
                    }
                    
                    if (speedControl == SPEED_CONTROL_AUTO && numCorrectCombo >= globals.numToSpeedUp)
                    {
                        camSpeed += globals.stepsizeSpeedUp;
                        if (camSpeed > globals.maxCamSpeed)
                            camSpeed = globals.maxCamSpeed;
                        camSpeed = (int)camSpeed;
                    }
                    
                    if (history) history->determineCoverLoc(true);
                }
                else if (!result.podInfo.goodPod && !result.podInfo.podTaken)
                {
                    if (soundFeedbackGood)
                        soundFeedbackGood->play();
                    
                }
                else if ((result.podInfo.goodPod && !result.podInfo.podTaken) ||
                         (!result.podInfo.goodPod && result.podInfo.podTaken)) {
                    if (soundFeedbackBad)
                        soundFeedbackBad->play();
                    hp = hp > 0 ? hp + globals.HPPositiveWrongAnswer : hp + globals.HPNegativeWrongAnswer;
                    if (hp < globals.HPNegativeLimit)
                        hp = globals.HPNegativeLimit;
                    ++numWrongTotal;
                    ++numWrongCombo;
                    numCorrectCombo = 0;
                    if (speedControl == SPEED_CONTROL_AUTO && numWrongCombo >= globals.numToSpeedDown)
                    {
                        camSpeed += globals.stepsizeSpeedDown;
                        if (camSpeed < globals.minCamSpeed)
                            camSpeed = globals.minCamSpeed;
                        camSpeed = (int)camSpeed;
                    }
                    
                    if (history) history->determineCoverLoc(false);
                }
                
                // Flag to trigger only once
                targetSlice->setInfoStored(true);
            
        }
        lookback = closest;
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
    
    if (movementMode == MOVEMENT_ROTATING)
    {
        // Animate camera rolls
        if (camRoll < desireRoll) {
            camRoll += max(5, (desireRoll - camRoll) / 2);
        }
        if (camRoll > desireRoll) {
            camRoll -= max(5, (camRoll - desireRoll) / 2);
        }
        
    }
    
    updateCursorCooldown(elapsed);
    checkCollisions(tunnel);
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
    int fileno = 0;
    /*
     ifstream intest;
     unsigned test;
     do {
     fileno++;
     file += "_f" + Util::toStringInt(fileno);
     intest.open(file.c_str());
     std::cout << "testing " << file << std::endl;
     if (intest)
     {
     std::string junk;
     intest >> junk >> junk >> junk;
     intest >> test;
     }
     } while (intest && test != seed && !intest.good()); // same seed means same session
     */
    std::ofstream out;
    
    bool newFile = false;
    std::ifstream testOpen (file.c_str());
    if (!testOpen) {
        testOpen.close();
        newFile = true;
        out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    } else {
        testOpen.close();
        newFile = false;
        out.open(file.c_str(), std::ofstream::out | std::ofstream::app);
    }
    
    std::cout << "Saving tunnel log: " << file << std::endl;
    
    if (out.good()) {
        if (newFile) {
            out << "% debug seed: " << seed << endl;
            out << "%" << endl;
            out << "% StageID {0, inf}" << endl;
            out << "% PodLocation { NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST }" << endl;
            out << "% PodType { BLUE, GREEN, PINK, YELLOW }" << endl;
            out << "% Nback {-inf, inf}" << endl;
            out << "% IsNBackPod {no, yes}" << endl;
            out << "% PlayerTookPod {no, yes}" << endl;
            out << "% GameMode { TIMED, NORMAL }" << endl;
            out << "% ProgressionMode { SIMPLE_PROGRESSIVE, MULTISENSORY_PROGRESSIVE, DISTRIBUTIVE_ADAPATIVE }" << endl;
            out << "% Score {0, inf}" << endl;
            out << "% Timestamp (ms)" << endl;
            out << "%" << endl;
            out << "% StageID PodLocation PodType Nback IsNBackPod PlayerTookPod Speed GameMode ProgressionMode Score Timestamp" << endl;
        }
        
        for (int i = 0; i < results.size(); ++i) {
            //out << SOUTH << " "
            out << results[i].stageID << " "
            << results[i].podInfo.podLoc << " "
            << results[i].podInfo.podType << " "
            << results[i].nback << " "
            << results[i].podInfo.goodPod << " "
            << results[i].podInfo.podTaken << " "
            << results[i].speed << " "
            << results[i].gameMode << " "
            << results[i].progressionMode + 1 << " "
            << results[i].score << " "
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

bool Player::saveStage(std::string file, int lastStageID)
{
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    bool ret = false;
    
    std::cout << "Writing Stage ID: " << file << std::endl;
    if (out.good()) {
        out << lastStageID;
        ret = true;
    }
    out.close();
    return ret;
}

void Player::setConfigValues()
{
    level.nback = globals.nback;
    level.control = globals.control;
}

Player::~Player()
{
}
