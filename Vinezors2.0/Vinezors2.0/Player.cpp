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
: seed(0), name(""), hp(globals.startingHP), numCorrectTotal(0), numSafeTotal(0), numMissedTotal(0), numWrongTotal(0), numCorrectBonus(0), numCorrectCombo(0), numWrongCombo(0), score(0), points(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), vines(), movementMode(MOVEMENT_ROTATING), camDir(SOUTH), vineDir(SOUTH), mousePos(), oldPos(), camPos(), oldRot(), oldRoll(0), camRot(), camRoll(0), desireRot(), desireRoll(0), camSpeed(0.0), finalSpeed(0.0), vineOffset(0), lookback(NULL), speedControl(SPEED_CONTROL_FLEXIBLE), level(), results(), totalElapsed(0), totalDistanceTraveled(0.0), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundPods(NUM_POD_SIGNALS), triggerStartup(true), inputTotalX(0.0), inputMoved(false)
{
    for (int i = 0; i < soundPods.size(); ++i)
        soundPods[i] = NULL;
}

Player::Player(const std::string & name, const PlayerLevel & level, Vector3 camPos, Quaternion camRot, double camSpeed, double offset, SpeedControlMode speedControl, unsigned seed, const std::string & filename)
: seed(seed), name(name), hp(globals.startingHP), numCorrectTotal(0), numSafeTotal(0), numCorrectBonus(0), numMissedTotal(0), numWrongTotal(0), numCorrectCombo(0), numWrongCombo(0), score(0), points(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), vines(), movementMode(MOVEMENT_ROTATING), camDir(SOUTH), vineDir(SOUTH), mousePos(), oldPos(camPos), camPos(camPos), oldRot(camRot), oldRoll(0), camRot(camRot), camRoll(0), desireRot(camRot), desireRoll(0), camSpeed(camSpeed), finalSpeed(camSpeed),vineOffset(offset), lookback(NULL), speedControl(speedControl), level(level), results(), totalElapsed(0), totalDistanceTraveled(0.0), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundPods(NUM_POD_SIGNALS), triggerStartup(true), inputTotalX(0.0), inputMoved(false)
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

int Player::getNumSafeTotal() const
{
    return numSafeTotal;
}

int Player::getNumMissedTotal() const
{
    return numMissedTotal;
}

int Player::getNumWrongTotal() const
{
    return numWrongTotal;
}

int Player::getNumCorrectBonus() const
{
    return numCorrectBonus;
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

int Player::getPoints() const
{
	return points;
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

double Player::getFinalSpeed() const
{
    return finalSpeed;
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
    if (numCorrectTotal == 0 && numWrongTotal == 0)
        return 0.0;
    return static_cast<double>(numCorrectTotal) / (numCorrectTotal + numMissedTotal + numWrongTotal);
}

double Player::getProgress(Tunnel* tunnel) const
{
    return static_cast<double>(numCorrectTotal) / (tunnel->getNumTargets() + numWrongTotal);
}

Evaluation Player::getEvaluation(Tunnel* tunnel) const
{
    if (tunnel->getMode() == GAME_PROFICIENCY)
    {
        if (getProgress(tunnel) >= globals.stageProficiencyThreshold2)
            return PASS;
        else if (getProgress(tunnel) < globals.stageProficiencyThreshold3)
            return FAIL;
        else
            return EVEN;
        //return getHP() > 0 ? PASS : FAIL;
    }
    else return EVEN;
    
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
    TunnelSlice* closest = tunnel->getCurrentOffset();
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

void Player::setLevel(PlayerLevel value)
{
    level = value;
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

void Player::newTunnel(Tunnel* tunnel, bool setmusic, bool resetscore)
{
    hp = globals.startingHP;
    if (globals.initCamSpeed <= 0.0)
        camSpeed = camSpeed * globals.nlevelSpeedModifier;
    else
        camSpeed = globals.initCamSpeed;
    if (resetscore)
        score = 0.0;
    Util::clamp(camSpeed, globals.minCamSpeed, globals.maxCamSpeed);
    
    totalDistanceTraveled = 0.0;
    numCorrectTotal = 0;
    numSafeTotal = 0;
    numMissedTotal = 0;
    numWrongTotal = 0;
    numCorrectBonus = 0;
    numCorrectCombo = 0;
    numWrongCombo = 0;
    
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
    
    tunnel->setOffsetIterators(camPos, vineOffset);
    for (int i = 0; i < vines.size(); ++i)
    {
        TunnelSlice* closest = tunnel->getCurrentOffset();
        if (closest) {
            Vector3 targetPos = targetPos =
            closest->requestPosition(closest->getCenter(tunnel->getTLeftOffsetCurrent()), vineDir, closest->getWallLength() / 1.5);
            vines[i]->setDest(targetPos);
            vines[i]->setPos(targetPos);
        }
    }
    
    triggerStartup = true;
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
    {
        soundPods[index]->stop();
        soundPods[index]->play();
    }
}

void Player::setSounds(bool mode)
{
    if (mode) // true means all pod sounds
    {
        soundFeedbackGreat = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGreatFeedback");
        soundFeedbackGood = NULL;
        soundFeedbackBad = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundBadFeedback");
        soundPods[POD_SIGNAL_1] = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundPod1");
        soundPods[POD_SIGNAL_2] = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundPod2");
        soundPods[POD_SIGNAL_3] = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundPod3");
        soundPods[POD_SIGNAL_4] = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundPod4");
        soundPods[POD_SIGNAL_UNKNOWN] = NULL;
        soundCollision = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundCollision");
        soundStartup = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundStartup");
    }
    else // false means no pod sounds
    {
        soundFeedbackGreat = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGreatFeedback");
        soundFeedbackGood = NULL;
        soundFeedbackBad = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundBadFeedback");
        for (int i = 0; i < NUM_POD_SIGNALS; ++i)
            soundPods[i] = NULL;
        soundCollision = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundCollision");
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
    std::vector<TunnelSlice*> slices = tunnel->findSlicesSincePreviousOffset();
	for (int i = 0; i < vines.size(); ++i)
    {
        for (int j = 0; j < slices.size(); ++j)
        {
            std::vector<Pod*> hits = slices[j]->findCollisions(vines[i]);
            if (hits.size() > 0)
                for (int i = 0; i < hits.size(); ++i)
                {
                    if (!tunnel->isDone() && hits[i]->getPodTrigger())
                    {
                        if (hits[i]->isPodGood())
                        {
                            hp += globals.HPPositiveCorrectAnswer;
                            if (soundFeedbackGreat)
                            {
                                soundFeedbackGreat->stop();
                                soundFeedbackGreat->play();
                            }
                        }
                        else
                        {
                            hp += globals.HPPositiveWrongAnswer;
                            if (soundCollision)
                            {
                                soundCollision->stop();
                                soundCollision->play();
                            }
                            camSpeed += globals.stepsizeSpeedDown;
                            camSpeed = Util::clamp(camSpeed, globals.minCamSpeed, globals.maxCamSpeed);
        
                            vines[i]->setWobble(true);
                        }
                    }
                }
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
    if (tunnel->isDone() || tunnel->getTotalElapsed() <= 0.5)
    {
        finalSpeed = globals.startupCamSpeed;
    }
    else if (tunnel->getTotalElapsed() <= 1.0)
    {
        // Animate slow down
        double speedRange = 20 * 2 - camSpeed;
        double timeRange = 1.0 - 0.5;
        finalSpeed = camSpeed + speedRange * (1.0 - (tunnel->getTotalElapsed() - 0.5) / timeRange);
    }
    else
    {
        finalSpeed = camSpeed;
        
        // Update scores if any
        double distTraveled = finalSpeed * elapsed;
        if (tunnel->getMode() == GAME_TIMED)
            score += distTraveled / 10.0;
        totalDistanceTraveled += distTraveled;
    }
    
    // Linearly interpolate the camera to get smooth transitions
    TunnelSlice* next = tunnel->getNext(1);
    if (next)
    {
        Vector3 endOfSlice = next->getEnd();
        Vector3 dir = (endOfSlice - camPos).normalisedCopy();
        Vector3 delta = dir * (globals.modifierCamSpeed * finalSpeed * elapsed);
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
    tunnel->update(this, hud ,elapsed);
    tunnel->setOffsetIterators(camPos, vineOffset);
    
    for (int i = 0; i < vines.size(); ++i)
    {
        // Determine which tunnel segment and corner each vine should be
        // in. This is done by calculating a t from 0 to 1
        TunnelSlice* closest = tunnel->getCurrent();
        if (closest && closest->getPods().size() > 0 && !closest->isPodHistory() && !tunnel->isDone())
        {
            if (tunnel->getTLeftCurrent() > 0.5)
            {
                History* history = tunnel->getHistory();
                if (history) history->addPod(closest->getPods()[0]->getPodInfo());
                
                closest->setPodHistory(true);
            }
        }
        
        closest = tunnel->getCurrentOffset();
        vines[i]->previousID = vines[i]->afterID;
        vines[i]->previoust = vines[i]->aftert;
        vines[i]->afterID = closest->getTunnelSliceID();
        vines[i]->aftert = tunnel->getTLeftOffsetCurrent();
        vines[i]->speed = finalSpeed;
        vines[i]->setQuaternion(getCombinedRotAndRoll());
        if (closest) {
            Vector3 targetPos;
            if (tunnel->getTLeftOffsetCurrent() >= 0.0 || vines[i]->previousID + 1 < vines[i]->afterID)
            {
                targetPos = closest->getCenter(tunnel->getTLeftOffsetCurrent()) +
                closest->requestMove(vineDir, closest->getWallLength() / 2);
            } else
            {
                if (tunnel->getPreviousOffset() != closest)
                    lookback = tunnel->getPreviousOffset();
                if (!lookback)
                    lookback = closest;
                
                // This section is to make the movement of slow ships a little smoother
                Vector3 p1 = lookback->getEnd() + lookback->requestMove(vineDir, lookback->getWallLength() / 2);
                Vector3 p2 = closest->getStart() + closest->requestMove(vineDir, closest->getWallLength() / 2);
                
                targetPos = p1 + (p2 - p1) * (1.0 + tunnel->getTLeftOffsetCurrent());
            }
            //vines[i]->setDest(targetPos);
            vines[i]->setForward(closest->getForward());
            vines[i]->setPos(targetPos);
        }
		vines[i]->update(elapsed); // Mostly for animating the navigation around the tunnel
    }
    checkCollisions(tunnel);
    if (!tunnel->isDone())
    {
        for (int i = 0; i < vines.size(); ++i)
        {
            // If player vine is halfway through a segment with a pod, we can get results
            std::vector<TunnelSlice*> slices = tunnel->findSlicesSincePreviousOffset();
            for (int j = 0; j < slices.size(); ++j)
            {
                TunnelSlice* targetSlice = slices[j];
                
                std::vector<Pod*> pods = targetSlice->getPods();
                if (pods.size() > 0 && !pods[0]->getPodTrigger() && !targetSlice->isInfoStored() &&
                    ((vines[i]->previousID < targetSlice->getTunnelSliceID() && vines[i]->afterID > targetSlice->getTunnelSliceID()) ||
                     (vines[i]->previousID == targetSlice->getTunnelSliceID() && vines[i]->previoust < 0.55 && (vines[i]->aftert >= 0.55 || vines[i]->afterID > vines[i]->previousID)) ||
                     (vines[i]->afterID == targetSlice->getTunnelSliceID() && vines[i]->aftert >= 0.55)))
                {
                    History* history = tunnel->getHistory();
                    
                    // This code block is to record data of the pods
                    Result result;
                    result.stageID = globals.stageID;
                    result.timestamp = (int)(totalElapsed * 1000);
                    result.sectionInfo = targetSlice->getSectionInfo();
                    result.podInfo = targetSlice->getPods()[0]->getPodInfo();
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
                        {
                            soundFeedbackGreat->stop();
                            soundFeedbackGreat->play();
                        }
                        ++numCorrectTotal;
                        ++numCorrectCombo;
                        numWrongCombo = 0;
                        
                        if (tunnel->getMode() == GAME_PROFICIENCY)
                        {
                            double plus = tunnel->getNBack();
                            for (int i = 0; i < numCorrectCombo - 1 && i < globals.numToSpeedUp; ++i)
                                plus *= 2;
                            score += plus;
                        }
                        points += numCorrectBonus;
                        numCorrectBonus++;
                        if (numCorrectBonus > 5)
                            numCorrectBonus = 5;
                        
                        if (tunnel->getMode() == GAME_NAVIGATION)
                        {
                            camSpeed += globals.stepsizeSpeedUp;
                            camSpeed = Util::clamp(camSpeed, globals.minCamSpeed, globals.maxCamSpeed);
                        }
                        else
                        {
                            if (numCorrectCombo >= globals.numToSpeedUp)
                            {
                                camSpeed += globals.stepsizeSpeedUp;
                                camSpeed = Util::clamp(camSpeed, globals.minCamSpeed, globals.maxCamSpeed);
                            }
                        }
                        
                        if (history) history->determineCoverLoc(true);
                    }
                    else if (!result.podInfo.goodPod && !result.podInfo.podTaken)
                    {
                        numSafeTotal++;
                        numCorrectBonus++;
                        
                        if (numCorrectBonus > 5)
                            numCorrectBonus = 5;
                        else
                        {
                            if (soundFeedbackGood)
                            {
                                soundFeedbackGood->stop();
                                soundFeedbackGood->play();
                            }
                        }
                    }
                    else if ((result.podInfo.goodPod && !result.podInfo.podTaken) ||
                             (!result.podInfo.goodPod && result.podInfo.podTaken)) {
                        if (soundFeedbackBad && tunnel->getMode() != GAME_NAVIGATION)
                        {
                            soundFeedbackBad->stop();
                            soundFeedbackBad->play();
                        }
                        if (result.podInfo.goodPod && !result.podInfo.podTaken) // Missed good
                            ++numMissedTotal;
                        else //if ((!result.podInfo.goodPod && result.podInfo.podTaken) //Took bad
                            ++numWrongTotal;
                        ++numWrongCombo;
                        numCorrectCombo = 0;
                        
                        if (tunnel->getMode() == GAME_NAVIGATION)
                        {
                            camSpeed += globals.stepsizeSpeedUp;
                            camSpeed = Util::clamp(camSpeed, globals.minCamSpeed, globals.maxCamSpeed);
                        }
                        else
                        {
                            if (numWrongCombo >= globals.numToSpeedDown)
                            {
                                camSpeed += globals.stepsizeSpeedDown;
                                camSpeed = Util::clamp(camSpeed, globals.minCamSpeed, globals.maxCamSpeed);
                            }
                        }
                    
                        numCorrectBonus = 0;
                        
                        if (history) history->determineCoverLoc(false);
                    }
                    
                    // Flag to trigger only once
                    targetSlice->setInfoStored(true);
                }
            }
        }
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
    int fileno = 0;
    
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
            out << "% Nback {0, inf}" << endl;
            out << "% IsNBackPod {no, yes}" << endl;
            out << "% PlayerTookPod {no, yes}" << endl;
            out << "% Speed {-inf, inf}" << endl;
            out << "% GameMode { TIMED, NORMAL }" << endl;
            out << "% ProgressionMode { SIMPLE_PROGRESSIVE, MULTISENSORY_PROGRESSIVE, DISTRIBUTIVE_ADAPATIVE }" << endl;
            out << "% Score {0, inf}" << endl;
            out << "% Timestamp (ms)" << endl;
            out << "%" << endl;
            out << "% StageID PodLocation PodColor Nback IsNBackPod PlayerTookPod Speed GameMode ProgressionMode Score Timestamp" << endl;
        }
        
        for (int i = 0; i < results.size(); ++i) {
            //out << SOUTH << " "
            out << results[i].stageID << " "
            << results[i].podInfo.podLoc << " "
            << results[i].podInfo.podColor << " "
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
