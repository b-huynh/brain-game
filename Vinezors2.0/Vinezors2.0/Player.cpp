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

Player::Player()
: seed(0), name(""), hp(globals.startingHP), numCorrectTotal(0), numSafeTotal(0), numMissedTotal(0), numWrongTotal(0), numCorrectBonus(0), numCorrectCombo(0), numWrongCombo(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), keySpace(false), vines(), movementMode(MOVEMENT_ROTATING), showCombo(true), camDir(SOUTH), mousePos(), oldPos(), camPos(), oldRot(), oldRoll(0), camRot(), camRoll(0), desireRot(), desireRoll(0), baseSpeed(0.0), bonusSpeed(0.0), finalSpeed(0.0), minSpeed(0.0), maxSpeed(0.0), vineOffset(0), lookback(NULL), selectedTarget(NULL), glowSpeed(0.0), speedControl(SPEED_CONTROL_FLEXIBLE), results(), skillLevel(), totalElapsed(0), totalDistanceTraveled(0.0), animationTimer(0.0), speedTimer(0.0), shockwaveTimer(0.0), boostTimer(0.0), selectTimer(0.0), startMusicTimer(0.0), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundPods(NUM_POD_SIGNALS), triggerStartup(true), inputTotalX(0.0), inputMoved(false)
{
    for (int i = 0; i < soundPods.size(); ++i)
        soundPods[i] = NULL;
}

Player::Player(const std::string & name, Vector3 camPos, Quaternion camRot, float camSpeed, float offset, SpeedControlMode speedControl, unsigned seed, const std::string & filename)
: seed(seed), name(name), hp(globals.startingHP), numCorrectTotal(0), numSafeTotal(0), numCorrectBonus(0), numMissedTotal(0), numWrongTotal(0), numCorrectCombo(0), numWrongCombo(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), keySpace(false), vines(), movementMode(MOVEMENT_ROTATING), showCombo(true), camDir(SOUTH), mousePos(), oldPos(camPos), camPos(camPos), oldRot(camRot), oldRoll(0), camRot(camRot), camRoll(0), desireRot(camRot), desireRoll(0), baseSpeed(camSpeed), bonusSpeed(0.0), finalSpeed(camSpeed), minSpeed(0.0), maxSpeed(0.0), vineOffset(offset), lookback(NULL), selectedTarget(NULL), glowSpeed(0.0), speedControl(speedControl), results(), skillLevel(), totalElapsed(0), totalDistanceTraveled(0.0), animationTimer(0.0), speedTimer(0.0), shockwaveTimer(0.0), boostTimer(0.0), selectTimer(0.0), startMusicTimer(0.0), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundPods(NUM_POD_SIGNALS), triggerStartup(true), inputTotalX(0.0), inputMoved(false)
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

Direction Player::getCamDir() const
{
	return camDir;
}

Direction Player::getVineDir() const
{
	return vines[0]->loc;
}

Direction Player::getVineDest() const
{
	return vines[0]->dest;
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

bool Player::getKeySpace() const
{
    return keySpace;
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
}
Vector3 Player::getCamUpward(bool combined) const
{
    Quaternion rot = combined ? getCombinedRotAndRoll() : camRot;
    Quaternion upward = Quaternion(0, 0, 1, 0);
    upward = rot * upward * rot.Inverse();
    return Vector3(upward.x, upward.y, upward.z);
}
Vector3 Player::getCamRight(bool combined) const
{
    Quaternion rot = combined ? getCombinedRotAndRoll() : camRot;
    Quaternion right = Quaternion(0, 1, 0, 0);
    right = rot * right * rot.Inverse();
    return Vector3(right.x, right.y, right.z);
}

float Player::getBaseSpeed() const
{
    return baseSpeed;
}

float Player::getFinalSpeed() const
{
    return finalSpeed;
}

float Player::getTotalSpeed() const
{
    float currentTotal = baseSpeed + bonusSpeed;
    float timeRange = 1.0;
    if (boostTimer >= 0.0)
    {
        float speedRange = currentTotal * globals.boostModifierCamSpeed - currentTotal;
        if (boostTimer >= timeRange)
            return currentTotal + speedRange;
        else
            return currentTotal + speedRange * (boostTimer) / timeRange;
    }
    return currentTotal;
}

Vector3 Player::getVineOffset() const
{
	return getCamForward() * vineOffset;
}

SpeedControlMode Player::getSpeedControl() const
{
    return speedControl;
}

float Player::getTotalElapsed() const
{
    return totalElapsed;
}

float Player::getTotalDistanceTraveled() const
{
    return totalDistanceTraveled;
}

float Player::getAnimationTimer() const
{
    return animationTimer;
}

float Player::getAccuracy() const
{
    if (numCorrectTotal == 0 && numWrongTotal == 0)
        return 0.0;
    return static_cast<float>(numCorrectTotal) / (numCorrectTotal + numMissedTotal + numWrongTotal);
}

float Player::getProgress() const
{
    int value = numCorrectTotal - numWrongTotal;
    float progress = static_cast<float>(value) / tunnel->getNumTargets();
    progress = Util::clamp(progress, 0.0, 1.0);
    return progress <= 1.0 ? progress : 1.0;
}

bool Player::getShowCombo() const
{
    return showCombo;
}

PlayerLevel Player::getSkillLevel() const
{
    return skillLevel;
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

void Player::updateGlowExtraction(float elapsed)
{
    if (selectedTarget)
    {
        SceneNode* glowNode = selectedTarget->getGlowNode();
        
        if (selectedTarget->isPodTaken() && glowNode)
        {
            Vector3 vinePos = vines[0]->getEntireVine()->_getDerivedPosition();
            Vector3 podPos = selectedTarget->getEntirePod()->_getDerivedPosition();
            Vector3 glowPos = glowNode->_getDerivedPosition();
            Vector3 glowMove = (vinePos - glowPos);
            float glowMoveLen = glowMove.length();
            if (glowMoveLen <= glowSpeed * elapsed)
            {
                glowNode->translate(glowMove);
            }
            else
            {
                glowMove *= glowSpeed / glowMoveLen * elapsed;
                glowNode->translate((glowMove));
            }
            
        }
        selectTimer -= elapsed;
        if (selectTimer <= 0.0)
        {
            if (selectedTarget)
                testPodGiveFeedback(selectedTarget);
            selectTimer = 0.0;
            selectedTarget = NULL;
        }
    }
}

void Player::setGlowGrabParameters()
{
    // Extract a glow that is in the form of a sphere always, not shape of pod.
    selectedTarget->removeGlow();
    selectedTarget->generateGlow(selectedTarget->getPodInfo().podColor, POD_SHAPE_SPHERE);
    
    // Assign a glow speed towards the player
    glowSpeed = 2 * (vines[0]->getEntireVine()->_getDerivedPosition() - selectedTarget->getGlowNode()->_getDerivedPosition()).length();
    if (finalSpeed > minSpeed)
        glowSpeed *= (finalSpeed / minSpeed);
}

void Player::performShockwave()
{
    if (shockwaveTimer <= 0.0)
    {
        if (!selectedTarget)
        {
            selectedTarget = tunnel->getNearestPod(globals.tunnelSegmentsPerPod);
            if (selectedTarget && selectedTarget->getGlowNode() && !selectedTarget->isPodTaken())
            {
                selectedTarget->takePod();
                setGlowGrabParameters();
                if (selectTimer <= 0.0)
                    selectTimer = 1.0 / (finalSpeed / globals.initCamSpeed);
            } else
                selectedTarget = NULL;
        }
        shockwaveTimer = 1.0;
        //vines[0]->setShockwave();
    }
}
 
void Player::updateShockwave(float elapsed)
{
        // Move the glow towards the vine
        if (shockwaveTimer > 0)
        {
            shockwaveTimer -= elapsed;
            if (shockwaveTimer <= 0.0)
            {
                vines[0]->removeShockwave();
                shockwaveTimer = 0.0;
            }
        }
}

void Player::performBoost()
{
    float timeRange = 1.0; // Range to linearly decrease back to normal speed
    if (boostTimer <= timeRange)
    {
        boostTimer = 5.0;
        vines[0]->setBoost();
    }
}

void Player::updateBoost(float elapsed)
{
    float timeRange = 1.0;
    if (boostTimer > 0.0)
    {
        boostTimer -= elapsed;
        if (boostTimer <= 0.0)
            boostTimer = 0.0;
        else if (boostTimer <= timeRange)
            vines[0]->removeBoost();
    }
}

void Player::testPodGiveFeedback(Pod* test)
{
    if (test->isPodTested())
        return;
    test->setPodTested(true);
    
    History* history = tunnel->getHistory();
    
    // Determine whether the player got it right or not
    if (test->isPodGood() && test->isPodTaken()) {
        if (tunnel->getMode() == GAME_TIMED)
        {
            skillLevel.speedScores[baseSpeed].right++;
        }
        else if (tunnel->getMode() == GAME_NAVIGATION)
        {
            skillLevel.navigationScores[tunnel->getCurrentNavLevel()].right++;
        }
        
        if (tunnel->getMode() == GAME_NAVIGATION)
        {
            if (soundFeedbackGood)
            {
                soundFeedbackGood->stop();
                soundFeedbackGood->play();
            }
        }
        else
        {
            if (soundFeedbackGreat)
            {
                soundFeedbackGreat->stop();
                soundFeedbackGreat->play();
            }
        }
        ++numCorrectTotal;
        ++numCorrectCombo;
        numWrongCombo = 0;

        // Combo Speed
        //if (tunnel->getMode() == GAME_PROFICIENCY)
        //    bonusSpeed += 1.0;
        
        if (numCorrectCombo % globals.numToSpeedUp == 0 && tunnel->getMode() != GAME_NAVIGATION)
        {
            baseSpeed += globals.stepsizeSpeedUp;
            baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
        }
        
        if (hp >= 0)
            hp += globals.HPPositiveCorrectAnswer;
        else
            hp += globals.HPNegativeCorrectAnswer;
        hp = Util::clamp(hp, globals.HPNegativeLimit, globals.HPPositiveLimit);
        
        // For proficiency tests, need its own checkIfDone function so stage doesn't end when time is out
        if (tunnel->getMode() == GAME_PROFICIENCY)
        {
            // Only check if player has passed the proficiency test
            if (getHP() >= globals.HPPositiveLimit)
                tunnel->setDone(PASS);
        }
        
        if (history) history->determineCoverLoc(true);
    }
    else if (!test->isPodGood() && !test->isPodTaken())
    {
        numSafeTotal++;
        numCorrectBonus++;
    }
    else if ((test->isPodGood() && !test->isPodTaken()) ||
             (!test->isPodGood() && test->isPodTaken())) {
        
        if (tunnel->getMode() == GAME_TIMED)
        {
            skillLevel.speedScores[baseSpeed].wrong++;
        }
        else if (tunnel->getMode() == GAME_NAVIGATION)
        {
            skillLevel.navigationScores[tunnel->getCurrentNavLevel()].wrong++;
        }
        
        if (soundFeedbackBad)
        {
            soundFeedbackBad->stop();
            soundFeedbackBad->play();
        }
        if (test->isPodGood() && !test->isPodTaken()) // Missed good
            ++numMissedTotal;
        else //if ((!test->isPodGood() && test->isPodTaken()) //Took bad
            ++numWrongTotal;
        
        if (tunnel->getMode() != GAME_NAVIGATION)
        {
            ++numWrongCombo;
            numCorrectCombo -= 3;
            if (numCorrectCombo < 0) numCorrectCombo = 0;
        }
        // Combo Speed reduction
        //if (tunnel->getMode() == GAME_PROFICIENCY)
        //{
        //    bonusSpeed -= 3.0;
        //    if (bonusSpeed < 0.0) bonusSpeed = 0.0;
        //}
        
        if (numWrongCombo % globals.numToSpeedDown == 0 && tunnel->getMode() != GAME_NAVIGATION)
        {
            baseSpeed += globals.stepsizeSpeedDown;
            baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
        }
        
        numCorrectBonus = 0;
        
        if (hp >= 0)
            hp += globals.HPPositiveWrongAnswer;
        else
            hp += globals.HPNegativeWrongAnswer;
        hp = Util::clamp(hp, globals.HPNegativeLimit, globals.HPPositiveLimit);

        if (tunnel->getMode() == GAME_PROFICIENCY)
        {
            if (globals.stageTime > 0 && tunnel->getTimeLeft() <= 0)
                tunnel->setDone(EVEN);
        }
        
        if (history) history->determineCoverLoc(false);
    }
    // Check for combo mode
    //if (tunnel->getMode() == GAME_PROFICIENCY) determineSpawnCombo();
}

void Player::determineSpawnCombo()
{
    tunnel->setSpawnCombo(1);
    if (getProgress() <= 0.5)
    {
        if (numCorrectCombo >= globals.combo1MinA)
            tunnel->setSpawnCombo(2);
        if (numCorrectCombo >= globals.combo2MinA)
            tunnel->setSpawnCombo(3);
    }
    else
    {
        if (numCorrectCombo >= globals.combo1MinB)
            tunnel->setSpawnCombo(2);
        if (numCorrectCombo >= globals.combo2MinB)
            tunnel->setSpawnCombo(3);
    }
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

void Player::setKeySpace(bool value)
{
	keySpace = value;
}

void Player::setCamDir(Direction value)
{
	camDir = value;
}

bool Player::setVineDirRequest(Direction value, bool force)
{
    TunnelSlice* closest = tunnel->getCurrentOffset();
    if (closest)
    {
        if (closest->hasAvailableSide(value) || force)
        {
            if (vines[0]->loc != vines[0]->dest)
                vines[0]->loc = vines[0]->dest;
            vines[0]->dest = value;
            vines[0]->transition = 0.0;
            return true;
        }
    }
    else if (tunnel->hasAvailableSide(value) || force)
    {
        if (vines[0]->loc != vines[0]->dest)
            vines[0]->loc = vines[0]->dest;
        vines[0]->dest = value;
        vines[0]->transition = 0.0;
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

void Player::setBaseSpeed(float value)
{
    baseSpeed = value;
}

void Player::setSkillLevel(PlayerLevel value)
{
    skillLevel = value;
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

void Player::unlink()
{
    this->tunnel = NULL;
    this->hud = NULL;
}

void Player::link(Tunnel* tunnel, Hud* hud)
{
    this->tunnel = tunnel;
    this->hud = hud;
}

void Player::newTunnel(bool setmusic)
{
    hp = globals.startingHP;
    minSpeed = globals.minCamSpeed;
    maxSpeed = globals.maxCamSpeed;
    if (globals.initCamSpeed <= 0.0)
        baseSpeed = baseSpeed * globals.nlevelSpeedModifier;
    else
        baseSpeed = globals.initCamSpeed;
    baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
    
    totalDistanceTraveled = 0.0;
    animationTimer = 5.0;
    speedTimer = 0.0;
    numCorrectTotal = 0;
    numSafeTotal = 0;
    numMissedTotal = 0;
    numWrongTotal = 0;
    numCorrectBonus = 0;
    numCorrectCombo = 0;
    numWrongCombo = 0;
    camDir = SOUTH;
    lookback = NULL;
    selectedTarget = NULL;
    
    if (setmusic)
    {
        OgreOggSound::OgreOggISound* soundMusicTemp = NULL;
        if (tunnel->getNBack() <= 1)
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
            startMusicTimer = 2.0;
        }
    }
    
    tunnel->setOffsetIterators(camPos, vineOffset);
    for (int i = 0; i < vines.size(); ++i)
    {
        vines[i]->reloadIfNecessary(globals.setVineShip);
        
        TunnelSlice* closest = tunnel->getCurrentOffset();
        if (closest)
        {
            Vector3 centerPos = closest->getCenter(tunnel->getTLeftOffsetCurrent());
            Vector3 southVec = closest->requestWallDirection(SOUTH);
            Vector3 rightVec = closest->requestWallDirection(EAST);
            vines[i]->loc = SOUTH;
            vines[i]->dest = SOUTH;
            vines[i]->setPos(centerPos + closest->requestWallDirection(vines[i]->loc) * closest->getWallLength() / 1.5);
            vines[i]->setQuaternion(closest->getQuaternion());
            Vector3 vineVec = (vines[i]->getPos() - centerPos).normalisedCopy();
            camRoll = Degree(vineVec.angleBetween(southVec)).valueDegrees();
            if (vineVec.dotProduct(rightVec) > 0)
                camRoll = -camRoll;
            desireRoll = camRoll;
        }
    }
    tunnel->setSpawnCombo(1);
    triggerStartup = true;
    results.clear();
}

void Player::move(Vector3 delta)
{
	camPos += delta;
}

void Player::changeMovementMode()
{
    if (movementMode == MOVEMENT_STATIC)
        movementMode = MOVEMENT_ROTATING;
    else
        movementMode = MOVEMENT_STATIC;
}

void Player::setShowCombo(bool value)
{
    showCombo = value;
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

void Player::unpause()
{
    revertCam();
}

void Player::pause()
{
    saveCam();
}

void Player::setSounds(bool mode)
{
    if (mode) // true means all pod sounds
    {
        soundFeedbackGreat = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGreatFeedback");
        soundFeedbackGood = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGoodFeedback");
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
        soundFeedbackGood = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGoodFeedback");
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

void Player::checkCollisions()
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
                    if (hits[i]->getPodTrigger())
                    {
                        if (tunnel->getMode() == GAME_NAVIGATION)
                        {
                            skillLevel.navigationScores[tunnel->getCurrentNavLevel()].wrong++;
                        }
                        if (soundCollision)
                        {
                            soundCollision->stop();
                            soundCollision->play();
                        }
                        if (hp >= 0)
                            hp += globals.HPPositiveDistractor;
                        else
                            hp += globals.HPNegativeDistractor;
                        hp = Util::clamp(hp, globals.HPNegativeLimit, globals.HPPositiveLimit);
                        
                        baseSpeed -= globals.distractorSpeedPenalty;
                        baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
                        tunnel->addToTimePenalty(globals.distractorTimePenalty);
                        speedTimer = 5.0;
                        
                        vines[i]->setWobble(true);
                        
                        if (tunnel->getMode() == GAME_PROFICIENCY)
                        {
                            if (globals.stageTime > 0 && tunnel->getTimeLeft() <= 0)
                                tunnel->setDone(EVEN);
                        }
                    }
                }
        }
    }
}

void Player::resetCursorMoved()
{
    inputTotalX = 0.0;
    inputMoved = false;
}

void Player::setCursorMoved()
{
    inputMoved = true;
}

void Player::updateCursorCooldown(float elapsed)
{
    // Cooldown before next swipe can be read
    if (inputMoved)
    {
        if (inputTotalX > 0.0)
        {
            inputTotalX -= globals.screenWidth / 2 * elapsed;
            if (inputTotalX <= 0.0)
            {
                inputTotalX = 0.0;
                inputMoved = false;
            }
        }
        else if (inputTotalX < 0.0)
        {
            inputTotalX += globals.screenWidth / 2 * elapsed;
            if (inputTotalX >= 0.0)
            {
                inputTotalX = 0.0;
                inputMoved = false;
            }
        }
    }
    else
    {
        inputLeftBound = -globals.screenWidth / globals.swipeSensitivity;
        inputRightBound = globals.screenWidth / globals.swipeSensitivity;
    }
}

void Player::checkCursorMove(float dx, float dy)
{
    std::cout << inputTotalX << std::endl;
    if (!inputMoved)
        inputTotalX += dx;
}

bool Player::checkPerformLeftMove(bool force)
{
    if (force || (!inputMoved && ((!globals.swipeInverted && inputTotalX < inputLeftBound) || (globals.swipeInverted && inputTotalX > inputRightBound))))
    {
        return true;
    }
    else return false;
}

bool Player::checkPerformRightMove(bool force)
{
    if (force || (!inputMoved && ((globals.swipeInverted && inputTotalX < inputLeftBound) || (!globals.swipeInverted && inputTotalX > inputRightBound))))
    {
        return true;
    }
    else return false;
}

void Player::update(float elapsed)
{
    totalElapsed += elapsed;
    
    if (triggerStartup && soundStartup && !soundStartup->isPlaying())
    {
        soundStartup->play();
        triggerStartup = false;
    }
    if (soundMusic && !soundMusic->isPlaying())
    {
        startMusicTimer -= elapsed;
        if (startMusicTimer <= 0.0)
            soundMusic->play();
    }
    
    if (tunnel->isDone())
    {
        if (tunnel->getEval() != PASS)
        {
            float totalSpeed = getTotalSpeed();
            float speedRange = totalSpeed;
            float timeRange = 3.0;
            float offset = 5.0 - timeRange;
            if (animationTimer > offset)
                finalSpeed = speedRange * ((animationTimer - offset) / timeRange);           
            else
                finalSpeed = 0.0;
            animationTimer -= elapsed;
        }
        else
            finalSpeed = globals.startupCamSpeed;
    }
    // Beginning quick startup part 1
    else if (tunnel->getTotalElapsed() <= 0.5)
    {
        finalSpeed = globals.startupCamSpeed;
    }
    // Easing to actual speed part 2
    else if (tunnel->getTotalElapsed() <= 1.0)
    {
        // Animate slow down
        float totalSpeed = getTotalSpeed();
        float speedRange = globals.startupCamSpeed - totalSpeed;
        float timeRange = 1.0 - 0.5;
        finalSpeed = totalSpeed + speedRange * (1.0 - (tunnel->getTotalElapsed() - 0.5) / timeRange);
    }
    // In-game return to normal speed from stop (hitting obstacles)
    else if (speedTimer > 0.0)
    {
        float totalSpeed = getTotalSpeed();
        float speedRange = 3 * totalSpeed / 4;
        float timeRange = 5.0;
        finalSpeed = totalSpeed / 4 + speedRange * (1.0 - (speedTimer / timeRange));
        speedTimer -= elapsed;
    }
    // Game speed
    else
    {
        finalSpeed = getTotalSpeed();
        totalDistanceTraveled += finalSpeed * elapsed;
    }
    
    // Linearly interpolate the camera to get smooth transitions
    TunnelSlice* next = tunnel->getNext(1);
    if (next)
    {
        Vector3 endOfSlice = next->getEnd();
        Vector3 dir = (endOfSlice - camPos).normalisedCopy();
        Vector3 delta = dir * (globals.globalModifierCamSpeed * finalSpeed * elapsed);
        move(delta);
        camRot = oldRot.Slerp(1 - (endOfSlice - camPos).length() / (endOfSlice - oldPos).length(), oldRot, desireRot);
    }
    
    updateCursorCooldown(elapsed);
    tunnel->update(elapsed);
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
        vines[i]->setQuaternion(getCombinedRotAndRoll());
        if (closest) {
            Vector3 centerPos = closest->getCenter(tunnel->getTLeftOffsetCurrent());
            Vector3 targetPos1;
            Vector3 targetPos2;
            
            // If on empty panel, find closest one.
            if (!closest->hasAvailableSide(vines[i]->dest))
            {
                Direction best = vines[i]->dest;
                Direction lo = best;
                Direction ro = best;
                do
                {
                    lo = Util::leftOf(lo);
                    ro = Util::rightOf(ro);
                    if (closest->hasAvailableSide(lo))
                        best = lo;
                    else if (closest->hasAvailableSide(ro))
                        best = ro;
                } while (!closest->hasAvailableSide(best) && lo != best && ro != best);
                vines[i]->loc = vines[i]->dest;
                vines[i]->dest = best;
                vines[i]->transition = 0.0;
            }
            
            vines[i]->transition += globals.vineTransitionSpeed * elapsed;
            if (vines[i]->transition >= 1.0)
            {
                vines[i]->loc = vines[i]->dest;
                vines[i]->transition = 0.0;
            }
            if (tunnel->getTLeftOffsetCurrent() >= 0.0 || vines[i]->previousID + 1 < vines[i]->afterID)
            {
                targetPos1 = centerPos + closest->requestWallDirection(vines[i]->loc) * closest->getWallLength() / 1.5;
                targetPos2 = centerPos + closest->requestWallDirection(vines[i]->dest) * closest->getWallLength() / 1.5;
            }
            else
            {
                if (tunnel->getPreviousOffset() != closest)
                    lookback = tunnel->getPreviousOffset();
                if (!lookback)
                    lookback = closest;
                
                Vector3 exit = lookback->getEnd();
                Vector3 entry = closest->getStart();
                // This section is to make the movement of slow ships a little smoother
                Vector3 p1 = exit + lookback->requestWallDirection(vines[i]->loc) * lookback->getWallLength() / 1.5;
                Vector3 p2 = entry + closest->requestWallDirection(vines[i]->loc) * closest->getWallLength() / 1.5;
                Vector3 p3 = exit + lookback->requestWallDirection(vines[i]->dest) * lookback->getWallLength() / 1.5;
                Vector3 p4 = entry + closest->requestWallDirection(vines[i]->dest) * closest->getWallLength() / 1.5;
                
                targetPos1 = p1 + (p2 - p1) * (1.0 + tunnel->getTLeftOffsetCurrent());
                targetPos2 = p3 + (p4 - p3) * (1.0 + tunnel->getTLeftOffsetCurrent());
            }
            vines[i]->setForward(closest->getForward());
            vines[i]->setPos(targetPos1 + (targetPos2 - targetPos1) * vines[i]->transition);
            
            vines[i]->update(elapsed); // Mostly for animating the navigation around the tunnel
            if (movementMode == MOVEMENT_ROTATING)
            {
                // Rotate camera strictly around vine. Assuming only one vine
                if (vines[i]->dest != vines[i]->loc && vines[i]->transition != 0.0)
                {
                    Vector3 southVec = closest->requestWallDirection(SOUTH);
                    Vector3 rightVec = closest->requestWallDirection(EAST);
                    Vector3 vineVec = (vines[i]->getPos() - centerPos).normalisedCopy();
                
                    camRoll = Degree(vineVec.angleBetween(southVec)).valueDegrees();
                    if (vineVec.dotProduct(rightVec) > 0)
                        camRoll = -camRoll;
                    desireRoll = camRoll;
                }
            }
        }
    }
    if (!tunnel->isDone())
    {
        checkCollisions();
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
                     (vines[i]->previousID == targetSlice->getTunnelSliceID() && vines[i]->previoust < globals.podCollisionMax && (vines[i]->aftert >= globals.podCollisionMax || vines[i]->afterID > vines[i]->previousID)) ||
                     (vines[i]->afterID == targetSlice->getTunnelSliceID() && vines[i]->aftert >= globals.podCollisionMax)))
                {
                    Pod* test = targetSlice->getPods()[0];
                    
                    // This code block is to record data of the pods
                    Result result;
                    result.stageID = globals.stageID;
                    result.timestamp = (int)(totalElapsed * 1000);
                    result.sectionInfo = targetSlice->getSectionInfo();
                    result.podInfo = test->getPodInfo();
                    result.nback = tunnel->getNBack();
                    result.gameMode = tunnel->getMode();
                    result.speed = baseSpeed;
                    results.push_back(result);
                    
                    //printf("%d %d\n", result.podInfo.goodPod, result.podInfo.podTaken);
                    
                    // For very fast players
                    if (selectedTarget && selectedTarget != test)
                        testPodGiveFeedback(selectedTarget);
                    
                    if (!test->isPodTested())
                    {
                        selectedTarget = test;
                        if (selectTimer <= 0.0) // Make sure it's not being grabbed already
                            setGlowGrabParameters();
                        if (selectTimer <= 0.0 || selectTimer > 0.1)
                            selectTimer = 0.1 / (finalSpeed / globals.initCamSpeed);
                    
                        // Flag to trigger only once
                        targetSlice->setInfoStored(true);
                    }
                }
            }
        }
    }
    updateGlowExtraction(elapsed);
    updateShockwave(elapsed);
    updateBoost(elapsed);
}

void Player::calculateNavigationScores()
{
    skillLevel.calculateNavigationScores();
}

void Player::calculateSpeedScores()
{
    skillLevel.calculateSpeedScores();
}

//Returns false if failed to save to file, true otherwise
bool Player::saveStage(std::string file)
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
            out << "% Timestamp (ms)" << endl;
            out << "%" << endl;
            out << "% StageID PodLocation PodColor Nback IsNBackPod PlayerTookPod Speed GameMode Score Timestamp" << endl;
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

bool Player::saveProgress(std::string file, int lastStageID)
{
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    bool ret = false;
    
    out << 0 << std::endl;
    out << skillLevel.set1 << std::endl;
    out << skillLevel.set2 << std::endl;
    out << skillLevel.set3 << std::endl;
    out << skillLevel.set1Rep << std::endl;
    out << skillLevel.set2Rep << std::endl;
    out << skillLevel.set3Rep << std::endl;
    out << skillLevel.navigation << std::endl;
    out << skillLevel.minSpeed << std::endl;
    out << skillLevel.averageSpeed << std::endl;
    out << skillLevel.maxSpeed << std::endl;
    std::cout << "Writing Stage ID: " << file << std::endl;
    ret = out.good();
    
    out.close();
    return ret;
}

bool Player::loadProgress(std::string savePath)
{
    std::ifstream saveFile (savePath.c_str());
    bool ret = false;
    
    if (saveFile.good()) {
        saveFile >> globals.currStageID;
        saveFile >> skillLevel.set1;
        saveFile >> skillLevel.set2;
        saveFile >> skillLevel.set3;
        saveFile >> skillLevel.set1Rep;
        saveFile >> skillLevel.set2Rep;
        saveFile >> skillLevel.set3Rep;
        saveFile >> skillLevel.navigation;
        saveFile >> skillLevel.minSpeed;
        saveFile >> skillLevel.averageSpeed;
        saveFile >> skillLevel.maxSpeed;
        
        std::cout << "Starting from last session StageID " << globals.currStageID << std::endl;
        globals.setMessage("Loaded Save " + globals.playerName + "\nSwipe to Continue", MESSAGE_NORMAL);
        ret = true;
    } else {
        globals.currStageID = 0;
        std::cout << "Starting from StageID " << globals.currStageID << std::endl;
        globals.setMessage("New Save " + globals.playerName + "\nSwipe to Continue", MESSAGE_NORMAL);
        ret = false;
    }
    saveFile.close();
    return ret;
}

Player::~Player()
{
}
