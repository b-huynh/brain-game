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

float flyOutCounter;
float flyOutDuration;
float endAnimationSuccessDuration;
float endAnimationFailDuration;
bool endAnimationBegin;

float flyOutSpeed;
float flyOutIncr;
bool flyOutSpeedUp;

float flyOutCamSpeed;
float flyOutAngleY;
float flyOutAngleX;

bool flyLeft;

bool soundStart;

Player::Player()
: seed(0), sessionID(0), name(""), hp(globals.startingHP), numCorrectTotal(0), numSafeTotal(0), numMissedTotal(0), numWrongTotal(0), numIgnoredTotal(0), numPickupsTotal(0), numAvoidancesTotal(0), numCollisionsTotal(0), numCorrectBonus(0), numCorrectCombo(0), numWrongCombo(0), score(0.0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), keySpace(false), vines(), movementMode(MOVEMENT_ROTATING), showCombo(true), camDir(SOUTH), mousePos(), oldPos(), camPos(), oldRot(), oldRoll(0), camRot(), camRoll(0), desireRot(), desireRoll(0), baseSpeed(0.0), bonusSpeed(0.0), finalSpeed(0.0), initSpeed(0.0), minSpeed(0.0), maxSpeed(0.0), vineOffset(0), lookback(NULL), selectedTarget(NULL), glowSpeed(0.0), toggleBack(0), results(), actions(), sessions(), logged(false), skillLevel(), totalElapsed(0), totalDistanceTraveled(0.0), animationTimer(0.0), speedTimer(0.0), badFuelPickUpTimer(0.0), boostTimer(0.0), selectTimerFlag(false), selectTimer(0.0), startMusicTimer(0.0), godMode(false), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundFeedbackMiss(NULL), triggerStartup(true), numStagesWon(0), levelRequestRow(0), levelRequestCol(0), menuRowIndex(0), levelProgress(), tutorialMgr(NULL), offsetRoll(0.0), offsetRollDest(0.0), endFlag(false)
{
    
    tunnel = NULL;
    
    scheduler = new LevelScheduler();
    levelRequest = NULL;    
    
    levelProgress = std::vector<std::vector<PlayerProgress> >(NUM_LEVELS, std::vector<PlayerProgress>(NUM_TASKS));
    initPowerUps();
    tutorialMgr = new TutorialManager();
    
    lastPlayed = PHASE_UNKNOWN;
    rerollCounter = 2;
    
    fadeMusic = false;
    xsTimer = 0.0f;
    musicVolume = 0.50f;
    soundVolume = 0.50f;
    holdout = 0.40f;
    holdoutLB = 1.0f;
    holdoutUB = 1.0f;
    syncDataToServer = false;
    inverted = true;
    initSettings();
}

Player::Player(const std::string & name, Vector3 camPos, Quaternion camRot, float camSpeed, float offset, unsigned seed, const std::string & filename)
: seed(seed), sessionID(0), name(name), hp(globals.startingHP), numCorrectTotal(0), numSafeTotal(0), numCorrectBonus(0), numMissedTotal(0), numWrongTotal(0), numIgnoredTotal(0), numPickupsTotal(0), numAvoidancesTotal(0), numCollisionsTotal(0), numCorrectCombo(0), numWrongCombo(0), score(0.0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), keySpace(false), vines(), movementMode(MOVEMENT_ROTATING), showCombo(true), camDir(SOUTH), mousePos(), oldPos(camPos), camPos(camPos), oldRot(camRot), oldRoll(0), camRot(camRot), camRoll(0), desireRot(camRot), desireRoll(0), baseSpeed(camSpeed), bonusSpeed(0.0), finalSpeed(camSpeed), initSpeed(0.0), minSpeed(0.0), maxSpeed(0.0), vineOffset(offset), lookback(NULL), selectedTarget(NULL), glowSpeed(0.0), toggleBack(0), results(), actions(), sessions(), logged(false), skillLevel(), totalElapsed(0), totalDistanceTraveled(0.0), animationTimer(0.0), speedTimer(0.0), badFuelPickUpTimer(0.0), boostTimer(0.0), selectTimerFlag(false), selectTimer(0.0), startMusicTimer(0.0), godMode(false), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundFeedbackMiss(NULL), triggerStartup(true), numStagesWon(0), levelRequestRow(0), levelRequestCol(0), menuRowIndex(0), levelProgress(), tutorialMgr(NULL), offsetRoll(0.0), offsetRollDest(0.0), endFlag(false)
{
    
    levels = new LevelSet();
    levels->initializeLevelSet();
    
    scheduler = new LevelScheduler();
    levelRequest = NULL;
    
    tunnel = NULL;
    levelProgress = std::vector<std::vector<PlayerProgress> >(NUM_LEVELS, std::vector<PlayerProgress>(NUM_TASKS));
    initPowerUps();
    tutorialMgr = new TutorialManager();
    
    lastPlayed = PHASE_UNKNOWN;
    rerollCounter = 2;
    
    fadeMusic = true;
    xsTimer = 0.0f;
    musicVolume = 0.50f;
    soundVolume = 0.50f;
    holdout = 0.40f;
    holdoutLB = 1.0f;
    holdoutUB = 1.0f;
    syncDataToServer = false;
    inverted = true;
    initSettings();
}

Tunnel* Player::getTunnel() const
{
    return tunnel;
}

LevelSet* Player::getLevels() const
{
    return levels;
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

int Player::getNumPickupsTotal() const
{
    return numPickupsTotal;
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

int Player::getNumCollisionsTotal() const
{
    return numCollisionsTotal;
}

float Player::getScore() const
{
    return score;
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

Vector3 Player::getPos() const
{
    return vines[0]->getPos();
}

Quaternion Player::getOldRot() const
{
	return oldRot;
}

float Player::getOldRoll() const
{
    return oldRoll;
}

Quaternion Player::getCamRot() const
{
	return camRot;
}

float Player::getCamRoll() const
{
    return camRoll;
}

Quaternion Player::getDesireRot() const
{
	return desireRot;
}

float Player::getDesireRoll() const
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
    float timeRange = 0.5;
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

float Player::getVineOffset() const
{
	return vineOffset;
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

int Player::getSessionID() const
{
    return sessionID;
}

std::string Player::getStageStats() const
{
    
}

std::string Player::getSessionStats() const
{
    std::string ret = "";
    ret += " Stages Completed: " + Util::toStringInt(numStagesWon) + "\n";
    ret += " Color/Sound: " + Util::toStringFloat(scheduler->nBackLevelA) + "\n";
    ret += " Shape/Sound: " + Util::toStringFloat(scheduler->nBackLevelB) + "\n";
    ret += " Sound Only: " + Util::toStringFloat(scheduler->nBackLevelC) + "\n";
    ret += " All Signal: " + Util::toStringFloat(scheduler->nBackLevelD) + "\n";
    ret += " Navigation: " + Util::toStringFloat(scheduler->nBackLevelE) + "\n";
    return ret;
}

int Player::getToggleBack() const
{
    return toggleBack;
}

bool Player::getGodMode() const
{
    return godMode;
}

int Player::getNumStagesWon() const
{
    return numStagesWon;
}

int Player::getLevelRequestRow() const
{
    return levelRequestRow;
}

int Player::getLevelRequestCol() const
{
    return levelRequestCol;
}

bool Player::hasLevelProgress(int level) const
{
    int row = levels->getLevelRow(level);
    int col = levels->getLevelCol(level);
    return hasLevelProgress(row, col);
}

bool Player::hasLevelProgress(int row, int col) const
{
    return row >= 0 && col >= 0 && row < levelProgress.size() && col < levelProgress[row].size();
}

PlayerProgress Player::getLevelProgress(int level) const
{
    int row = levels->getLevelRow(level);
    int col = levels->getLevelCol(level);
    return levelProgress[row][col];
}

PlayerProgress Player::getLevelProgress(int row, int col) const
{
    return levelProgress[row][col];
}

// Is the level available to the player based on player stats?
bool Player::isLevelAvailable(int level) const
{    // Satisfy previous row star total and
    // previous column level must be 3 stars
    if (!levels->hasLevel(level))
        return false;
    
    int levelRow = levels->getLevelRow(level);
    int levelCol = levels->getLevelCol(level);
    
    int totalRatingCur = getTotalLevelRating(levelRow);
    int rowRequirementCur = levels->getTotalRowRequirement(levelRow);
    
    // Only check scheduler if at least a scheduler level has been played
    if (scheduler->scheduleHistoryA.size() > 0 ||
        scheduler->scheduleHistoryB.size() > 0 ||
        scheduler->scheduleHistoryC.size() > 0 ||
        scheduler->scheduleHistoryD.size() > 0 ||
        scheduler->scheduleHistoryE.size() > 0)
    {
        switch (levelCol)
        {
            case 0:
            {
                if (levelRow < scheduler->nBackLevelA) return true;
                break;
            }
            case 1:
            {
                if (levelRow < scheduler->nBackLevelB) return true;
                break;
            }
            case 2:
            {
                if (levelRow < scheduler->nBackLevelC) return true;
                break;
            }
            case 3:
            {
                if (levelRow < scheduler->nBackLevelD) return true;
                break;
            }
            case 4:
            {
                if (levelRow < scheduler->nBackLevelE) return true;
                break;
            }
            default:
                break;
        }
    }
    
    // Player must have played the first level to play any other one
    if (level != 0 && getLevelProgress(0).rating < 0)
        return false;
    
    // This column is sound only, deactivate it if sound volume in settings is off
    //if (levelCol == 3 && soundVolume <= 0.0)
    //    return false;
    if (!levels->hasLevel(levelRow - 1, levelCol))
        return (levelCol != 5 || totalRatingCur >= rowRequirementCur - 3);
    if (!hasLevelProgress(levelRow - 1, levelCol))
        return false;
    int totalRatingPrev = getTotalLevelRating(levelRow - 1);
    int previousRating = getLevelProgress(levelRow - 1, levelCol).rating;
    int rowRequirementPrev = levels->getTotalRowRequirement(levelRow - 1);
    
    //std::cout << levelRow << "," << levelCol << " " << rowRequirementCur << " " << totalRatingCur << std::endl;
    if (totalRatingPrev >= rowRequirementPrev && previousRating >= 5)
    {
        // For the last level, unlock it only if we are close
        return (levelCol != 5 || totalRatingCur >= rowRequirementCur - 3);
    }
    else
        return false;
}

bool Player::isLevelAvailable(int row, int col) const
{
    int level = levels->getLevelNo(row, col);
    return isLevelAvailable(level);
}

// Given level request row and col, is the next level available to play in the 2D level select grid?
bool Player::isNextLevelAvailable() const
{
    return isLevelAvailable(getNextLevel());
}

// Given level request row and col, return the next level in the sequence for the 2D level select grid
int Player::getNextLevel() const
{
    int row = getLevelRequestRow();
    int col = getLevelRequestCol();
    int level = levels->getLevelNo(row, col);
    return ((level + 1) % NUM_TASKS) != 5 ? level + 1 : level + 2;
}

int Player::getMenuRowIndex() const
{
    return menuRowIndex;
}

// Returns the total rating of a row-set of levels
int Player::getTotalLevelRating(int row) const
{
    if (row < 0 || row >= levelProgress.size()) return 0;
    int total = 0;
    for (int col = 0; col < levelProgress[row].size(); ++col)
        if (levelProgress[row][col].rating >= 0)
            total += levelProgress[row][col].rating;
    return total;
}

// Returns the total score of a row-set of levels
float Player::getTotalLevelScore(int row) const
{
    if (row < 0 || row >= levelProgress.size()) return 0.0;
    float total = 0.0;
    for (int col = 0; col < levelProgress[row].size(); ++col)
        if (levelProgress[row][col].score >= 0.0)
            total += levelProgress[row][col].score;
    return total;
}

// Returns the total score of all levels
float Player::getTotalLevelScore() const
{
    float total = 0.0;
    for (int row = 0; row < levelProgress.size(); ++row)
        total += getTotalLevelScore(row);
    return total;
}

Ogre::ColourValue Player::getBoostColor() const
{
    return boostColor;
}


// Returns true if the player hasn't started the tunnel yet
//
// This flag is set to false the moment player update runs once.
// Then, the ship plays its startup sound.
bool Player::hasTriggeredStartup() const
{
    return !triggerStartup;
}

// Returns the score of a positive target using the tunnel n-back
//
// Later, if points vary by target in a stage, we should pass in
// the signal that was selected.
float Player::getScoring() const
{
    if (tunnel->getMode() == STAGE_MODE_RECESS || tunnel->getMode() == STAGE_MODE_TEACHING)
        return 50.0;
    
    int nvalue = tunnel->getNBack();
    switch (nvalue)
    {
        case 0:
            return 50.0;
        case 1:
            return 100.0;
        case 2:
            return 250.0;
        case 3:
            return 500.0;
        case 4:
            return 1000.0;
        case 5:
            return 2000.0;
        case 6:
            return 4000.0;
        case 7:
            return 6000.0;
        case 8:
            return 8000.0;
        case 9:
            return 10000.0;
        case 10:
            return 15000.0;
    }
    return 5000.0 * nvalue;
}

TutorialManager* Player::getTutorialMgr() const
{
    return tutorialMgr;
}

void Player::setRunningSpeed(int val1, int val2, int val3, int val4, int nav)
{
    skillLevel.runSpeed1 = val1;
    skillLevel.runSpeed2 = val2;
    skillLevel.runSpeed3 = val3;
    skillLevel.averageSpeed = val4;
    skillLevel.navigation = nav;
}

void Player::setSpeedParameters(int initSpeed, int minSpeed, int maxSpeed)
{
    this->initSpeed = initSpeed;
    this->minSpeed = minSpeed;
    this->maxSpeed = maxSpeed;
    baseSpeed = Util::clamp(initSpeed, minSpeed, maxSpeed);
    finalSpeed = getTotalSpeed();
    //tunnel->updateTSModifier();
    
    // Update for logs
    if (sessions.size() > 0)
    {
        sessions.back().runSpeedIn = baseSpeed;
        sessions.back().runSpeedOut = -1;
    }
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

void Player::setScore(float value)
{
    score = value;
}

void Player::updateTractorBeam(float elapsed)
{
    TractorBeam* t = dynamic_cast<TractorBeam*>(powerups["TractorBeam"]);
    
    if (t && t->active && t->expireTimer > 0.0)
    {
        Vector3 camForward = getCamForward(false);
        Vector3 camDown = -getCamUpward(false);
        
        t->effectModifier = finalSpeed / 20.0f;
        
        Vector3 vinePos = vines[0]->entireVine->_getDerivedPosition() + camForward * 5.0 + camDown * 0.25;
        Vector3 podPos = t->targetPod->getHead()->_getDerivedPosition();
        
        if (t->expireTimer <= t->expireCheckpoint)
        {
            t->tractorBeamEffect->getEmitter(0)->setEmissionRate(Real(0.0f));
            t->tractorBeamPulseEffect->getEmitter(0)->setEmissionRate(Real(0.0f));
        }
        
        // Move the pulse node
        Vector3 glowPos = t->tractorBeamPulseNode->_getDerivedPosition();
        Vector3 glowLerp = vinePos + (podPos - vinePos) * (t->pulseLen - t->pulseTimer) / t->pulseLen;
        t->tractorBeamPulseNode->setPosition(glowLerp);
        
        // Rotate the beam effect
        Vector3 rotator = (podPos - vinePos).normalisedCopy();
        
        Quaternion q = camForward.getRotationTo(rotator);
        
        float distance = (podPos - vinePos).length();
        t->tractorBeamEffect->getEmitter(0)->setParameter("depth", Util::toStringFloat(distance - distance / 10 * t->effectModifier));
        t->tractorBeamEffect->getEmitter(0)->setParameter("time_to_live", Util::toStringFloat(0.1 / t->effectModifier));
        t->tractorBeamPulseEffect->getEmitter(0)->setParameter("time_to_live", Util::toStringFloat(0.1 / t->effectModifier));
        t->tractorBeamNode->setPosition(0,0,-distance / 2.0);
        //t->tractorBeamEffect->getEmitter(0)->setParameter("depth", Util::toStringFloat(distance - distance / 18.0 * t->effectModifier));
        //t->tractorBeamEffect->getEmitter(0)->setParameter("time_to_live", Util::toStringFloat(0.2 / t->effectModifier));
        //t->tractorBeamNode->setPosition(0,0,-distance / 2.0 - distance / 18.0  * t->effectModifier);
        
        t->tractorBeamRotatorNode->setPosition(vinePos);
        t->tractorBeamRotatorNode->setOrientation(getCamRot() * q);
        
        t->tractorBeamNode->roll(Radian(Degree(15.0f)));

        // Update timer
        t->expireTimer -= (elapsed * t->effectModifier);
        t->pulseTimer -= (elapsed * t->effectModifier);
        if (t->pulseTimer <= 0.0) t->pulseTimer = t->pulseLen;
        // If pod is recorded and past, turn off tractor beam
        // Or better yet, if the ship's forward vector is not in the same direction as (podPos - vinePos)
        if (camForward.dotProduct(podPos - vinePos) < 0.0)
            t->expireTimer = 0.0;
    }
    if (t && t->active && t->tractorBeamEffect && t->expireTimer <= 0.0f) {
        t->tractorBeamPulseNode->detachObject(t->tractorBeamPulseEffect);
        t->tractorBeamPulseNode->getCreator()->destroyParticleSystem(t->tractorBeamPulseEffect);
        t->tractorBeamPulseEffect = NULL;
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->removeAndDestroyChild("PulseNode");
        t->tractorBeamPulseNode = NULL;
        
        t->tractorBeamNode->detachObject(t->tractorBeamEffect);
        t->tractorBeamNode->getCreator()->destroyParticleSystem(t->tractorBeamEffect);
        t->tractorBeamEffect = NULL;
        t->tractorBeamRotatorNode->removeAndDestroyChild("BeamNode");
        t->tractorBeamNode = NULL;
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->removeAndDestroyChild("BeamRotatorNode");
        t->tractorBeamRotatorNode = NULL;
        t->active = false;
        t->targetPod = NULL;
    }
}

void Player::performTractorBeam()
{
    TractorBeam* t = dynamic_cast<TractorBeam*>(powerups["TractorBeam"]);
    
    if (!t->targetPod && !selectedTarget && t && !t->active)
    {
        t->targetPod = tunnel->getNearestPod(globals.tunnelSegmentsPerPod);
        if (getToggleBack() > 0 &&
            t->targetPod && !t->targetPod->isPodTested() && !t->targetPod->isIndicatorVisible() &&
            !t->targetPod->getGlowNode() && !t->targetPod->isPodTaken())
        {
            setToggleBack(0);
            tunnel->respondToToggleCheat();

            t->targetPod->takePod();
            t->targetPod->zapPod();
            selectedTarget = t->targetPod;
            setGlowGrabParameters(t->targetPod);
            if (selectTimer <= 0.0)
            {
                selectTimerFlag = true;
                selectTimer = 0.1;//1.0 / (finalSpeed / globals.initCamSpeed);
            }
            
            // Generate a pulse glow effect
            Ogre::ColourValue particleValue = Ogre::ColourValue(0.0, 0.5, 0.3);
            std::string particleName = "General/GlowPodEllipsoid";
            
            // Make independent to avoid rotation orientions when translating.
            t->tractorBeamPulseNode = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("PulseNode");
            t->tractorBeamPulseEffect = t->tractorBeamPulseNode->getCreator()->createParticleSystem("TractorBeamPulseEffect", particleName);
            ParticleEmitter* tractorBeamGlowEmitter = t->tractorBeamPulseEffect->getEmitter(0); // Assuming only one emitter
            tractorBeamGlowEmitter->setColour(particleValue);
            
            t->tractorBeamPulseNode->attachObject(t->tractorBeamPulseEffect);
            
            // Change the tractor beam effect based on speed (faster ships should have beams that expire quicker)
            t->effectModifier = finalSpeed / 20.0f;
            // Assign a glow speed for the pulse towards
            t->pulseLen = 0.30;
            t->pulseTimer = t->pulseLen;
            
            // Assign powerup info
            t->active = true;
            t->available = false;
            t->expireTimer = 1.0f;
            t->expireCheckpoint = 0.5;
            
            // Generate a beam
            t->tractorBeamRotatorNode = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("BeamRotatorNode");
            t->tractorBeamNode = t->tractorBeamRotatorNode->createChildSceneNode("BeamNode");
            t->tractorBeamEffect = t->tractorBeamNode->getCreator()->createParticleSystem("Beam", "General/Beam");
            t->tractorBeamEffect->getEmitter(0)->setParameter("depth", "0");
            t->tractorBeamNode->attachObject(t->tractorBeamEffect);
        } else
            t->targetPod = NULL;
    }
}

void Player::updateTimeWarp(float elapsed)
{
    TimeWarp* t = (TimeWarp*)powerups["TimeWarp"];
    
    if( t && t->active ) {
        if( t->zoomIn == 0 ) {
            t->currentAngle += t->angleIncrement;
            if( t->currentAngle >= t->maxAngle ) {
                t->currentAngle = t->maxAngle;
                if( t->mainTimer >= t->mainTimeout ) {
                    t->mainTimer = 0.0f;
                    t->zoomIn = 1;
                }
                else {
                    t->mainTimer += elapsed;
                }
            }
            if( t->currentOffset <= t->offsetLimit ) {
                t->currentOffset = t->offsetLimit;
            }
            else {
                t->currentOffset -= t->offsetIncrement;
            }
            finalSpeed *= t->slowdown;
        }
        else if( t->zoomIn == 1 ) {
            t->currentAngle -= t->angleIncrement;
            if( t->currentOffset >= t->origOffset ) {
                t->currentOffset = t->origOffset;
            }
            else {
                t->currentOffset += t->offsetIncrement;
            }
            if( t->currentAngle <= t->origAngle ) {
                t->currentAngle = t->origAngle;
                t->currentOffset = t->origOffset;
                t->zoomIn = 2;
            }
        }
        else {
            if( t->mainTimer >= t->timeBonusTimeout ) {
                t->zoomIn = 0;
                t->active = false;
                
                //tutorialMgr->setSlides(TutorialManager::TUTORIAL_SLIDES_TIME_WARP);
            }
            else {
                t->mainTimer += t->timeBonusTimeout/t->timeVal;
                t->currentTimeVal--;
                
                // Only add to the clock if the stage is not over,
                // or, add it ot a winning player for more points
                if( !tunnel->isDone() || tunnel->getEval() == PASS ) tunnel->addToTimePenalty(-1);
            }
        }
        Camera* cam = OgreFramework::getSingletonPtr()->m_pCameraMain;
        cam->setFOVy(Degree(t->currentAngle));
        vineOffset = t->currentOffset;
    }
}

void Player::performTimeWarp()
{
    
    TimeWarp* t = dynamic_cast<TimeWarp*>(powerups["TimeWarp"]);
    
    if ( t && !t->active ) {
        t->timeBonus = globals.wrongAnswerTimePenalty * 2;
        
        Camera* cam = OgreFramework::getSingletonPtr()->m_pCameraMain;
        
        t->origFov = Degree(cam->getFOVy());
        t->origAngle = t->origFov.valueDegrees();
        t->currentAngle = t->origFov.valueDegrees();
        
        t->origOffset = vineOffset;
        t->currentOffset = t->origOffset;
        
        t->mainTimer = 0.0f;
        t->active = true;
        t->available = false;
        
        t->currentTimeVal = t->timeVal;
    }
}

void Player::updateBoost(float elapsed)
{
    float timeRange = 0.5;
    if (vines[0]->boostEffect)
    {
        float dropOffPecentage = 0.9;
        float percentFuel = tunnel->getFuelTimer() / (globals.fuelMax * dropOffPecentage);
        if (percentFuel >= 1.0f)
            percentFuel = 1.0f;
        float origHue = 0.55f;
        float origSat = 1.00f;
        float origLit = 1.00f;
        boostColor = Ogre::ColourValue(0.0, 0.7, 1.0);
        if (percentFuel <= 0.0 && tunnel->isDone())
            boostColor = Ogre::ColourValue(0.0, 0.0, 0.0);
        else
            boostColor.setHSB(origHue * percentFuel, origSat, origLit);
        
        vines[0]->boostEffect->getEmitter(0)->setColour(boostColor);
        Ogre::MaterialPtr mat = OgreFramework::getSingletonPtr()->m_pMaterialMgr->getByName("Arwing/ThrusterColor");
        //mat->setDiffuse(boostColor);
        mat->setSelfIllumination(boostColor);
        
        if (percentFuel < 0.70)
            tutorialMgr->setSlides(TutorialManager::TUTORIAL_SLIDES_TEXTBOX_FUEL);
        
        if (boostTimer > 0.0)
        {
            boostTimer -= elapsed;

            if (boostTimer < 0.0)
            {
                boostTimer = 0.0;
                soundBoost->stop();
                vines[0]->removeBoost();
            }
            else if (boostTimer <= timeRange)
            {
                vines[0]->boostEffect->getEmitter(0)->setEmissionRate(100 * (boostTimer / timeRange));
            }
        }
    }
}

void Player::setBoostTimer(float value)
{
    boostTimer = value;
}

void Player::performBoost()
{
    float timeRange = 0.5; // Range to linearly decrease back to normal speed
    if (boostTimer <= timeRange)
    {
        boostTimer = 2.0;
        vines[0]->setBoost();
        playSound(soundBoost);
    }
}

void Player::performShields()
{
    Shields* t = dynamic_cast<Shields*>(powerups["Shields"]);
    
    if ( t && t->shieldScaleTarget < 1.0) {
        
        if (!t->shieldNode)
        {
            t->shieldNode = vines[0]->entireVine->createChildSceneNode("shieldNode");
            t->shieldEntity = t->shieldNode->getCreator()->createEntity("shieldEntity", "sphereMesh");
            
            t->shieldEntity->setMaterialName("General/VineShellInactive");
            t->shieldNode->attachObject(t->shieldEntity);
        }
        t->active = true;
        t->available = false;
        t->shieldScaleTarget = 1.0;
    }
    
}

bool Player::triggerShields()
{
    Shields* t = dynamic_cast<Shields*>(powerups["Shields"]);
    if ( t && t->active && t->shieldScaleTarget >= 1.0 )
    {
        t->shieldScaleTarget = 0.0;
        t->active = false;
        return true;
    }
    return false;
}

void Player::updateShields(float elapsed)
{
    Shields* t = dynamic_cast<Shields*>(powerups["Shields"]);
    
    if ( t && t->shieldNode ) {
        // Move value towards target destination
        if (t->shieldScaleValue < t->shieldScaleTarget)
        {
            t->shieldScaleValue += 1.5 * elapsed;
            if (t->shieldScaleValue > t->shieldScaleTarget)
                t->shieldScaleValue = t->shieldScaleTarget;
        }
        else if (t->shieldScaleValue > t->shieldScaleTarget)
        {
            t->shieldScaleValue -= 2 * elapsed;
            if (t->shieldScaleValue < t->shieldScaleTarget)
                t->shieldScaleValue = t->shieldScaleTarget;
        }
        
        t->shieldNode->setScale(
                                vines[0]->getRadius() * 2 * t->shieldScaleValue,
                                vines[0]->getRadius() * 2 * t->shieldScaleValue,
                                vines[0]->getRadius() * 2 * t->shieldScaleValue);
        
        if (t->shieldScaleValue >= 1.0)
            t->shieldEntity->setMaterialName("General/VineShellActive");
        else
            t->shieldEntity->setMaterialName("General/VineShellInactive");
        
        if (t->shieldScaleValue <= 0.0 && t->shieldScaleTarget <= 0.0)
        {
            t->shieldNode->detachObject(t->shieldEntity);
            t->shieldNode->getCreator()->destroyEntity(t->shieldEntity);
            t->shieldEntity = NULL;
            t->shieldNode->getParent()->removeChild(t->shieldNode);
            t->shieldNode->getCreator()->destroySceneNode(t->shieldNode);
            t->shieldNode = NULL;
            t->active = false;
        }
    }
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
                if (selectTimer <= 0.0 && !selectTimerFlag) {
                    selectTimer = 0.1;
                    selectTimerFlag = true;
                }
            }
            else
            {
                glowMove *= glowSpeed / glowMoveLen * elapsed;
                glowNode->translate((glowMove));
            }
        }
        if( selectTimerFlag ) {
            selectTimer -= elapsed;
            if (selectTimer <= 0.0)
            {
                if (selectedTarget)
                    testPodGiveFeedback(selectedTarget);
                selectTimer = 0.0;
                selectedTarget = NULL;
                selectTimerFlag = false;
            }
        }
    }
}

void Player::setGlowGrabParameters(Pod* pod)
{
    // Extract a glow that is in the form of a sphere always, not shape of pod.
    //pod->removeGlow();
    //pod->generateGlow(pod->getPodInfo().podColor, POD_SHAPE_SPHERE);
    
    // Assign a glow speed towards the player
    if (pod->getGlowNode())
    {
        glowSpeed = 2 * (vines[0]->getEntireVine()->_getDerivedPosition() - pod->getGlowNode()->_getDerivedPosition()).length();
        if (finalSpeed > minSpeed)
            glowSpeed *= (finalSpeed / minSpeed);
    }
}

void Player::beginBadFuelPickUp()
{
    badFuelPickUpTimer = 1.0;
    //vines[0]->setExplode();
    vines[0]->setWobble(true);
}

void Player::updateBadFuelPickUp(float elapsed)
{
    // Move the glow towards the vine
    if (badFuelPickUpTimer > 0)
    {
        badFuelPickUpTimer -= elapsed;
        if (badFuelPickUpTimer <= 0.0)
        {
            //vines[0]->removeExplode();
            badFuelPickUpTimer = 0.0;
        }
    }
}

void Player::testPodGiveFeedback(Pod* test)
{
    if (test->isPodTested())
        return;
    test->setPodTested(true);
    
    bool nbackPod = tunnel->getPodIsGood(0);                        // Is it a target?
    bool correctSelection = (nbackPod && getToggleBack() == 0) ||   // Did the player make the right choice
                            (!nbackPod && getToggleBack() != 0) ||  // whether to zap or not to zap?
                            tunnel->getMode() == STAGE_MODE_RECESS;
    bool podTaken = test->isPodTaken();                             // Was the pod picked up? (Zap includes pickup)

    // This boolean is used to make the game check whether the game
    // should have ended. This is generally done in the Tunnel class,
    // however, this helps with certain cases with fuel where you
    // can only lose if you're at zero fuel AND just missed a potential
    // fuel boost
    bool forceCheckEnd = false;
    
    if (nbackPod && correctSelection)
    {
        if (podTaken)
        {
            // You zapped the right target
            ++numCorrectTotal;
            ++numCorrectCombo;
            numWrongCombo = 0;
            
            if (tunnel->getMode() == STAGE_MODE_RECESS)
            {
                playSound(soundFeedbackGood);
            }
            else
            {
                playSound(soundFeedbackGreat);
            }
            
            updateSpeed(initSpeed, true);
            if (tunnel->satisfyCriteria(tunnel->getNBackToggle(getToggleBack()), 3) && tunnel->getMode() != STAGE_MODE_RECESS)
            {
                if (tunnel->areCriteriaFilled()) // Done for last minute pick-ups for zap. (i.e. Time ran out, but you zapped the next pod)
                    tunnel->setEval(PASS);
            }
            
            if (getToggleBack() == 0)
            {
                if (hp >= 0) hp += globals.HPPositiveCorrectAnswer;
                else hp += globals.HPNegativeCorrectAnswer;
                hp = Util::clamp(hp, globals.HPNegativeLimit, globals.HPPositiveLimit);
            }
            
            // Add to fuel gauge for correct zaps
            if (tunnel->getMode() == STAGE_MODE_RECESS)
                tunnel->addToFuelTimer(tunnel->getFuelReturn());
            else
                tunnel->addToFuelBuffer(2.667); //
        }
        else
        {
            if (!tunnel->isDone())
            {
                // You missed zapping the target
                numCorrectCombo = 0;
                ++numMissedTotal;
                ++numWrongCombo;
                
                if (tunnel->getMode() != STAGE_MODE_RECESS)
                {
                    playSound(soundFeedbackMiss);
                    tunnel->addToTimePenalty(globals.wrongAnswerTimePenalty);
                
                    numCorrectBonus = 0;
                    if (numWrongCombo % globals.numToSpeedDown == 0)
                    {
                        baseSpeed -= globals.speedMap[baseSpeed];
                        baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
                    }
                }
                
                forceCheckEnd = true;
            }
        }
    }
    else if ((nbackPod && !correctSelection) ||
             (!nbackPod && !correctSelection))
    {
        if (podTaken)
        {
            if (!nbackPod && !correctSelection)
            {
                // You zapped the wrong target
                playSound(soundFeedbackBad);
                numCorrectCombo = 0;
                numCorrectBonus = 0;
                ++numWrongTotal;
                
                //beginBadFuelPickUp();
                xsTimer = 1.0f;
                
                if (hp >= 0) hp += globals.HPPositiveWrongAnswer;
                else hp += globals.HPNegativeWrongAnswer;
                hp = Util::clamp(hp, globals.HPNegativeLimit, globals.HPPositiveLimit);
                tunnel->addToFuelTimer(0.0); // Done to limit fuel to new maximum
                
                //tunnel->addToTimePenalty(globals.wrongAnswerTimePenalty);
                //tunnel->loseRandomCriteria();
                
                /*
                // Turn the zap beam orange for wrong fuel
                TractorBeam* t = dynamic_cast<TractorBeam*>(powerups["TractorBeam"]);
                Ogre::ColourValue failColor = Ogre::ColourValue(1.0, 0.5, 0.3);
                if (t && t->tractorBeamEffect)
                    t->tractorBeamEffect->getEmitter(0)->setColour(failColor);
                if (t && t->tractorBeamPulseEffect)
                    t->tractorBeamPulseEffect->getEmitter(0)->setColour(failColor);
                if (test->getIndicatorEffect())
                    test->getIndicatorEffect()->getEmitter(0)->setColour(failColor);
                if (test->getGlowEffect())
                    test->getGlowEffect()->getEmitter(0)->setColour(failColor);
                 */
                
                updateSpeed(initSpeed, false);
                
                forceCheckEnd = true;
            }
            else
            {
                // You picked up the wrong target but should've zapped
                if (!tunnel->isDone())
                {
                    playSound(soundFeedbackMiss);
                    tunnel->addToTimePenalty(globals.wrongAnswerTimePenalty);
                    
                    numCorrectCombo = 0;
                    numCorrectBonus = 0;
                    ++numMissedTotal;
                    ++numWrongCombo;
                    ++numIgnoredTotal;
                    if (numWrongCombo % globals.numToSpeedDown == 0)
                    {
                        updateSpeed(initSpeed, false);
                    }
                    
                    forceCheckEnd = true;
                }
            }
        }
        else if (!podTaken && nbackPod)
        {
            // You missed zapping the target and didn't pick up as well
            if (!tunnel->isDone())
            {
                playSound(soundFeedbackMiss);
                tunnel->addToTimePenalty(globals.wrongAnswerTimePenalty);
                
                numCorrectCombo = 0;
                numCorrectBonus = 0;
                ++numMissedTotal;
                ++numWrongCombo;
                
                updateSpeed(initSpeed, false);
                
                forceCheckEnd = true;
            }
        }
    }
    else if (!nbackPod && correctSelection)
    {
        // You picked up the right target
        numSafeTotal++;
        numCorrectBonus++;
        if (podTaken)
        {
            numPickupsTotal++;
            playSound(soundFeedbackGood);
            
            // Add to fuel gauge for correct pickups
            tunnel->addToFuelTimer(tunnel->getFuelReturn());
        }
    }
    
    // Check after passing a pod so that they might obtain fuel and be safe
    if (!tunnel->isDone() && tunnel->getFuelTimer() <= 0.0f && (tunnel->getFuelBuffer() <= 0.0 || forceCheckEnd))
        tunnel->setDone(EVEN);
    
    // Check for combo mode
    //if (tunnel->getMode() == STAGE_MODE_PROFICIENCY) determineSpawnCombo();
    // Reset toggle-back to 0-back
    if (tunnel->getMode() != STAGE_MODE_RECESS) setToggleBack(3);
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

void Player::offsetShip(float elapsed)
{
    tunnel->setOffsetIterators(camPos, vineOffset); // Important in order to cleanly move camera and vine
    TunnelSlice* closest = tunnel->getCurrentOffset();
    if (closest) {
        vines[0]->previousID = vines[0]->afterID;
        vines[0]->previoust = vines[0]->aftert;
        vines[0]->afterID = closest->getTunnelSliceID();
        vines[0]->aftert = tunnel->getTLeftOffsetCurrent();
        
#if !defined(OGRE_IS_IOS)
        vines[0]->setQuaternion(getCombinedRotAndRoll());
        Vector3 centerPos = closest->getCenter(vines[0]->aftert);
        
        // For desktop builds, don't include Brandon's swipe controller,
        // This is the original standard 1 swipe per panel movement.
        // For desktop builds, this is left or right arrow key.
        Vector3 targetPos1;
        Vector3 targetPos2;
        
        // If on empty panel, find closest one.
        if (!closest->hasAvailableSide(vines[0]->dest))
        {
            Direction best = vines[0]->dest;
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
            vines[0]->loc = vines[0]->dest;
            vines[0]->dest = best;
            vines[0]->transition = 0.0;
        }
        
        // Transition from panel to the next panel
        vines[0]->transition += globals.vineTransitionSpeed * elapsed;
        if (vines[0]->transition >= 1.0)
        {
            vines[0]->loc = vines[0]->dest;
            vines[0]->transition = 0.0;
        }
        // Moving through the segment. Has to deal with transition between each segment
        if (tunnel->getTLeftOffsetCurrent() >= 0.0 || vines[0]->previousID + 1 < vines[0]->afterID || (tunnel->getPreviousOffset() == closest && !lookback))
        {
            targetPos1 = centerPos + closest->requestWallDirection(vines[0]->loc) * closest->getWallLength() / 1.5;
            targetPos2 = centerPos + closest->requestWallDirection(vines[0]->dest) * closest->getWallLength() / 1.5;
        }
        else
        {
            if (tunnel->getPreviousOffset() != closest)
                lookback = tunnel->getPreviousOffset();
            //            if (!lookback)
            //                lookback = closest;
            
            Vector3 exit = lookback->getEnd();
            Vector3 entry = closest->getStart();
            // This section is to make the movement of slow ships a little smoother
            Vector3 p1 = exit + lookback->requestWallDirection(vines[0]->loc) * lookback->getWallLength() / 1.5;
            Vector3 p2 = entry + closest->requestWallDirection(vines[0]->loc) * closest->getWallLength() / 1.5;
            Vector3 p3 = exit + lookback->requestWallDirection(vines[0]->dest) * lookback->getWallLength() / 1.5;
            Vector3 p4 = entry + closest->requestWallDirection(vines[0]->dest) * closest->getWallLength() / 1.5;
            
            targetPos1 = p1 + (p2 - p1) * (1.0 + tunnel->getTLeftOffsetCurrent());
            targetPos2 = p3 + (p4 - p3) * (1.0 + tunnel->getTLeftOffsetCurrent());
         }
         vines[0]->setForward(closest->getForward());
         vines[0]->setPos(targetPos1 + (targetPos2 - targetPos1) * vines[0]->transition);
#else
        Vector3 centerPos = closest->getCenter(vines[0]->aftert);
        
        // Brandon's swipe control offsets ship based on camera
        float radius = tunnel->getCurrent()->getWallLength() / 1.5;
        vines[0]->setForward(closest->getForward());
    
        Quaternion vineRot;
        // Offset the ship from the camera roll
        vineRot = Quaternion(Degree(offsetRoll), getCamForward());
        Vector3 vineDir = vineRot * (getCamUpward() * -1);
        vines[0]->setPos(getCamPos() + getCamForward() * globals.tunnelSegmentDepth * vineOffset + (vineDir * radius));
        
        // Orient the ship for the offset
        vineRot = Quaternion(Degree(offsetRoll / 2), getCamForward());
        vines[0]->setQuaternion(getCombinedRotAndRoll() * vineRot);
#endif
        
        vines[0]->update(elapsed); // Mostly for animating the navigation around the tunnel
        if (movementMode == MOVEMENT_ROTATING)
        {
            // Rotate camera strictly around vine. Assuming only one vine
            if (vines[0]->dest != vines[0]->loc && vines[0]->transition != 0.0)
            {
                Vector3 southVec = closest->requestWallDirection(SOUTH);
                Vector3 rightVec = closest->requestWallDirection(EAST);
                Vector3 vineVec = (vines[0]->getPos() - centerPos).normalisedCopy();
                
                camRoll = Degree(vineVec.angleBetween(southVec)).valueDegrees();
                if (vineVec.dotProduct(rightVec) > 0)
                    camRoll = -camRoll;
                desireRoll = camRoll;
            }
        }
    }
}

void Player::recordInfo()
{
    // If player vine is halfway through a segment, we can get results
    std::vector<TunnelSlice*> slices = tunnel->findSlicesSincePreviousOffset();
    for (int j = 0; j < slices.size(); ++j)
    {
        TunnelSlice* targetSlice = slices[j];
        
        if (!targetSlice->isInfoStored() &&
            ((vines[0]->previousID < targetSlice->getTunnelSliceID() && vines[0]->afterID > targetSlice->getTunnelSliceID()) ||
             (vines[0]->previousID == targetSlice->getTunnelSliceID() && vines[0]->previoust < globals.podCollisionMax && (vines[0]->aftert >= globals.podCollisionMax || vines[0]->afterID > vines[0]->previousID)) ||
             (vines[0]->afterID == targetSlice->getTunnelSliceID() && vines[0]->aftert >= globals.podCollisionMax)))
        {
            // Encoding on each panel where a lower number can be overridden by a higher number
            std::vector<Pod*> pods = targetSlice->getPods();
            PodInfo targetinfo;
            SectionInfo sliceInfo = targetSlice->getSectionInfo();
            targetinfo.podExists = false;
            char segmentEncoding[NUM_DIRECTIONS];
            // Set segment information
            for (int i = 0;i < NUM_DIRECTIONS; ++i)
                segmentEncoding[i] = sliceInfo.sidesUsed[i] ? ENC_EMPTY_PANEL : ENC_NO_PANEL;
            segmentEncoding[vines[0]->transition < 0.50 ? vines[0]->loc : vines[0]->dest] = ENC_PLAYER;
            if (pods.size() > 0)
            {
                int ind = 0;
                // Set up target info and feedback
                if (pods.size() > 0 && !pods[0]->getPodTrigger())
                {
                    Pod* test = pods[0];
                    
                    targetinfo = test->getPodInfo();
                    targetinfo.podExists = true;
                    ind++;
                    
                    // For very fast players
                    if (selectedTarget && selectedTarget != test)
                        testPodGiveFeedback(selectedTarget);
                    
                    if (!test->isPodTested())
                    {
                        selectTimerFlag = true;
                        selectedTarget = test;;
                        //if (selectTimer <= 0.0) // Make sure it's not being grabbed already
                        //    setGlowGrabParameters(selectedTarget);
                        if (selectTimer <= 0.0 || selectTimer > 0.1)
                            selectTimer = 0.1;
                    }
                    
                    segmentEncoding[test->getLoc()] = test->isPodGood() ? ENC_MATCH : ENC_NONMATCH;
                }
                // Set obstacle info, assuming the first index can sometimes be signal
                bool avoided = true;
                for (; ind < pods.size(); ++ind)
                {
                    if (pods[ind]->getPodTrigger())
                    {
                        if (pods[ind]->getMeshType() == POD_HAZARD)
                        {
                            segmentEncoding[pods[ind]->getLoc()] = ENC_OBSTACLE;
                            if (pods[ind]->isPodTaken())
                                avoided = false;
                        }
                        else if (pods[ind]->getMeshType() == POD_POWERUP)
                            segmentEncoding[pods[ind]->getLoc()] = ENC_TIMEWARP;
                    }
                }
                // If the player has collided with an obstacle on a panel, that player
                // failed to avoid all of the obstacles set for that segment.
                // A counter is recorded to track that value
                if (avoided) numAvoidancesTotal++;
                else numCollisionsTotal++;
            }
            
            // This code block is to record data of the pods
            Result result;
            memcpy(result.segmentEncoding, segmentEncoding, NUM_DIRECTIONS);
            result.eventID = globals.stageID;
            result.levelID = tunnel->getStageNo();
            result.taskType = tunnel->getPhase();
            result.nback = tunnel->getNBack();
            result.playerRollBase = camRoll;
            result.playerRollOffset = offsetRoll;
            result.playerRollSpeed = rollSpeed;
            result.playerFuelTimer = tunnel->getFuelTimer();
            result.playerFuelBuffer = tunnel->getFuelBuffer();
            result.playerLoc = vines[0]->transition < 0.50 ? vines[0]->loc : vines[0]->dest;
            result.podInfo = targetinfo;
            result.sectionInfo = sliceInfo;
            result.timestamp = (int)(OgreFramework::getSingletonPtr()->totalElapsed * 1000);
            result.levelEnded = tunnel->isDone();
            result.minSpeed = minSpeed;
            result.maxSpeed = maxSpeed;
            result.baseSpeed = baseSpeed;
            result.finalSpeed = finalSpeed;
            results.push_back(result);
            
            // Flag to trigger only once
            targetSlice->setInfoStored(true);
        }
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
#if defined(OGRE_IS_IOS)
    // For iOS special swipe controls, refer to
    // updateSpin in EngineStage and offsetShip in Player as well
    vines[0]->loc = value;
    vines[0]->dest = value;
    
    return true;
#else
    // OSX OG controls which need animation
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
#endif
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

void Player::setPos(Vector3 value)
{
    vines[0]->setPos(value);
}

void Player::setOldRot(Quaternion value)
{
	oldRot = value;
}

void Player::setOldRoll(float value)
{
    oldRoll = value;
}

void Player::setCamRot(Quaternion value)
{
	camRot = value;
}

void Player::setCamRoll(float value)
{
    camRoll = value;
}

void Player::setDesireRot(Quaternion value)
{
	desireRot = value;
}

void Player::setDesireRoll(float value)
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

void Player::setSessionID(int value)
{
    sessionID = value;
}

void Player::setToggleBack(int value)
{
    toggleBack = value;
}

void Player::setGodMode(bool value)
{
    godMode = value;
    vines[0]->setPowerIndication(godMode);
}

void Player::setLevelRequestRow(int value)
{
    levelRequestRow = value;
}

void Player::setLevelRequestCol(int value)
{
    levelRequestCol = value;
}

void Player::setLevelRequest(int row, int col)
{
    levelRequestRow = row;
    levelRequestCol = col;
}

void Player::setMenuRowIndex(int value)
{
    menuRowIndex = value;
}

void Player::setAllProgressTo(const PlayerProgress & value)
{
    for (int i = 0; i < levelProgress.size(); ++i)
        for (int j = 0; j < levelProgress[i].size(); ++j)
            levelProgress[i][j] = value;
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
}

void Player::link(Tunnel* tunnel)
{
    this->tunnel = tunnel;
}

// Initializes the toggle based on the criterias of the tunnel and the n-back's on the toggle
void Player::initToggleBack()
{
    //int minNBack = tunnel->getLowestCriteria();
    //int toggle = 0;
    //if (minNBack <= 0) // 0-backs always the last toggle
    //    toggle = 3;
    //else
    //    toggle = tunnel->getNBack() - minNBack;
    //setToggleBack(toggle);
    
    //setToggleBack(toggle);
    setToggleBack(3);
}

void Player::newTunnel(const std::string & nameMusic)
{
    setCamPos(tunnel->getStart() + tunnel->getCurrent()->getForward() * globals.tunnelSegmentDepth);
    setCamRot(tunnel->getCurrent()->getQuaternion());
    setDesireRot(getCamRot());
	addVine(new Vine(OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode(), camPos, globals.vineRadius));
    
    hp = globals.startingHP;
    
    totalDistanceTraveled = 0.0;
    animationTimer = 5.0;
    speedTimer = 0.0;
    numCorrectTotal = 0;
    numSafeTotal = 0;
    numMissedTotal = 0;
    numWrongTotal = 0;
    numIgnoredTotal = 0;
    numPickupsTotal = 0;
    numCorrectBonus = 0;
    numCorrectCombo = 0;
    numWrongCombo = 0;
    numAvoidancesTotal = 0;
    numCollisionsTotal = 0;
    score = 0.0;
    camDir = SOUTH;
    lookback = NULL;
    selectedTarget = NULL;
    
    Session sess;
    sess.sessionID = sessionID;
    sess.eventID = globals.stageID;
    sess.levelID = tunnel->getStageNo();
    sess.taskType = tunnel->getPhase();
    sess.hasHoldout = tunnel->levelHasHoldout();
    sess.timestampIn = (int)(OgreFramework::getSingletonPtr()->totalElapsed * 1000);
    sess.timestampOut = -1;
    if (tunnel->getMode() == STAGE_MODE_RECESS || tunnel->getMode() == STAGE_MODE_TEACHING)
        sess.nback = 0;
    else
        // nback variable in tunnel not accurate due to holdout
        // Examine nback on a criteria will do for now.
        sess.nback = tunnel->getNBack();
    // Redundant to set speed here, the speed slider may adjust value making this inaccurate.
    // It is instead updated in set starting speed
    sess.runSpeedIn = baseSpeed;
    sess.runSpeedOut = -1;
    sess.TP = -1;
    sess.FP = -1;
    sess.TN = -1;
    sess.FN = -1;
    sess.obsHit = -1;
    sess.obsAvoided = -1;
    sessions.push_back(sess);
    
    logged = false;
    
    OgreFramework::getSingletonPtr()->m_pSoundMgr->resumeAllPausedSounds();
    OgreFramework::getSingletonPtr()->m_pSoundMgr->stopAllSounds();
    
    if (nameMusic != "" && (!soundMusic || nameMusic != soundMusic->getName()))
    {
        if (soundMusic) OgreFramework::getSingletonPtr()->m_pSoundMgr->destroySound(soundMusic);
        soundMusic = NULL;
        soundMusic = OgreFramework::getSingletonPtr()->m_pSoundMgr->createSound(nameMusic, Util::getMusicFile(nameMusic), true, true, true);
        soundMusic->setVolume(musicVolume);
    }
    if (soundMusic) soundMusic->setVolume(musicVolume);
    
    boostColor = Ogre::ColourValue(0.0, 0.7, 1.0);
    
    // End animation parameters
    flyOutCounter = 0.0f;
    flyOutDuration = 0.25f;
    endAnimationSuccessDuration = 3.0f;
    endAnimationFailDuration = 1.5f;
    endAnimationBegin = false;
    
    flyOutSpeed = 0.0f;
    flyOutIncr = 0.01f;
    flyOutSpeedUp = true;
    
    flyOutCamSpeed = 0.0f;
    flyOutAngleY = 0.0f;
    flyOutAngleX = 0.0f;
    
    flyLeft = true;
    soundStart = false;
    
    endFlag = false;
    fadeMusic = false;
    startMusicTimer = 2.0;
    
    // initalize player ship location
    tunnel->setOffsetIterators(camPos, vineOffset);
    vines[0]->setVisible(true);
    vines[0]->reloadIfNecessary(globals.setVineShip);
    vines[0]->setBoost();
    boostTimer = 0.0f;
    TunnelSlice* closest = tunnel->getCurrentOffset();
    if (closest)
    {
        Vector3 centerPos = closest->getCenter(tunnel->getTLeftOffsetCurrent());
        Vector3 southVec = closest->requestWallDirection(SOUTH);
        Vector3 rightVec = closest->requestWallDirection(EAST);
        vines[0]->loc = SOUTH;
        vines[0]->dest = SOUTH;
        vines[0]->setPos(centerPos + closest->requestWallDirection(vines[0]->loc) * closest->getWallLength() / 1.5);
        vines[0]->setQuaternion(closest->getQuaternion());
        Vector3 vineVec = (vines[0]->getPos() - centerPos).normalisedCopy();
        camRoll = Degree(vineVec.angleBetween(southVec)).valueDegrees();
        if (vineVec.dotProduct(rightVec) > 0)
            camRoll = -camRoll;
        desireRoll = camRoll;
    }
    initToggleBack();
    tunnel->setSpawnCombo(1);
    triggerStartup = true;
    results.clear();
    actions.clear();
}

void Player::startMenu()
{
    std::string nameMusic = "MusicMenu";
    if (nameMusic != "" && (!soundMusic || nameMusic != soundMusic->getName()))
    {
        OgreFramework::getSingletonPtr()->m_pSoundMgr->resumeAllPausedSounds();
        OgreFramework::getSingletonPtr()->m_pSoundMgr->stopAllSounds();
        
        if (soundMusic) OgreFramework::getSingletonPtr()->m_pSoundMgr->destroySound(soundMusic);
        soundMusic = NULL;
        soundMusic = OgreFramework::getSingletonPtr()->m_pSoundMgr->createSound(nameMusic, Util::getMusicFile(nameMusic), true, true, true);
        soundMusic->setVolume(musicVolume);
    }
    if (soundMusic)
    {
        soundMusic->play();
    }
}

// Save speed settings from speed dial
void Player::saveSpeedSettings()
{
    if (levelRequest)
    {
        // If assigned a specific level (via scheduler)
        switch (levelRequest->first.phaseX)
        {
            case PHASE_COLOR_SOUND:
                scheduler->speedA = globals.initCamSpeed;
                scheduler->firstTimeA = false;
                break;
            case PHASE_SHAPE_SOUND:
                scheduler->speedB = globals.initCamSpeed;
                scheduler->firstTimeB = false;
                break;
            case PHASE_SOUND_ONLY:
                scheduler->speedC = globals.initCamSpeed;
                scheduler->firstTimeC = false;
                break;
            case PHASE_ALL_SIGNAL:
                scheduler->speedD = globals.initCamSpeed;
                scheduler->firstTimeD = false;
                break;
            case PHASE_COLLECT:
                scheduler->speedE = globals.initCamSpeed;
                scheduler->firstTimeE = false;
                break;
            default:
                break;
        }
    }
    else
    {
        // Save speed settings for every other stage as well.
        // Players prefer it carries over to every other level instead of just the level played itself.
        for (int i = 0; i < levelProgress.size(); ++i)
            for (int j = 0; j < levelProgress[i].size(); ++j)
            {
                PlayerProgress* levelResult = &(levelProgress[i][j]);
                levelResult->initSpeedSetting = globals.initCamSpeed;
            }
        // Save speed settings for just the stage
        //PlayerProgress* levelResult = &(levelProgress[levelRequestRow][levelRequestCol]);
        //levelResult->initSpeedSetting = globals.initCamSpeed;
    }
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
    q.FromAngleAxis(Degree(camRoll - offsetRoll / 2.0), globals.tunnelReferenceForward);
    return q;
}

Quaternion Player::getCombinedRotAndRoll() const
{
    return camRot * getRoll();
}

void Player::playSound(OgreOggSound::OgreOggISound* sound) const
{
    if (sound)
    {
        sound->setVolume(soundVolume);
        sound->stop();
        sound->play();
    }
}

void Player::reactGUI() const
{
    playSound(soundButtonPress);
}

void Player::playFireworkSound() const
{
    playSound(soundFirework);
}

float Player::getStartMusicTimer() const
{
    return startMusicTimer;
}

void Player::playMusic() const
{
    if (soundMusic)
    {
        soundMusic->play();
    }
}

void Player::stopMusic()
{
    if (soundMusic)
    {
        fadeMusic = true;
    }
}

// Only called as the player adjusts the music volume slider, don't need one for sound volume
void Player::updateMusicVolume()
{
    if (soundMusic) soundMusic->setVolume(musicVolume);
}

void Player::unpause()
{
    revertCam();
}

void Player::pause()
{
    saveCam();
}

// Initializes single item sounds
void Player::setSounds()
{
    soundFeedbackGreat = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGreatFeedback");
    soundFeedbackGood = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundGoodFeedback");
    soundFeedbackBad = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundBadFeedback");
    soundFeedbackMiss = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundMissFeedback");
    soundCollision = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundCollision");
    soundStartup = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundStartup");
    soundBoost = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundBoost");
    soundButtonPress = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundButtonPress");
    soundFirework = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundFirework");
}

void Player::addVine(Vine *vine)
{
	vines.push_back(vine);
    vine->loc = camDir;
}

void Player::removeVines()
{
    for (int i = 0; i < vines.size(); ++i)
        delete vines[i];
    vines.clear();
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
            {
                for (int i = 0; i < hits.size(); ++i)
                {
                    if (hits[i]->getPodTrigger())
                    {
                        if (hits[i]->getMeshType() == POD_HAZARD)
                        {
                            // Hit an obstacle
                            playSound(soundCollision);
                        
                            if (!triggerShields())
                            {
                                updateSpeed(initSpeed, false);
                                speedTimer = 5.0;
                                tunnel->addToTimePenalty(globals.distractorTimePenalty);
                                if (hp >= 0)
                                    hp += globals.HPPositiveDistractor;
                                else
                                    hp += globals.HPNegativeDistractor;
                                hp = Util::clamp(hp, globals.HPNegativeLimit, globals.HPPositiveLimit);
                                tunnel->addToFuelTimer(0.0); // Done to limit fuel to new maximum
                            
                                beginBadFuelPickUp();
                            }
                            numCorrectCombo = 0;
                        }
                        else if (hits[i]->getMeshType() == POD_POWERUP)
                        {
                            // Hit a powerup
                            switch (hits[i]->getPodColor())
                            {
                                case POD_COLOR_PINK:
                                    powerups["TractorBeam"]->available = true;
                                    break;
                                case POD_COLOR_BLUE:
                                    powerups["Shields"]->available = true;
                                    break;
                                case POD_COLOR_GREEN:
                                    powerups["TimeWarp"]->available = true;
                                    performTimeWarp();
                                    break;
                                case POD_COLOR_YELLOW:
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

// Updates final speed based on the mean. (Bigger steps around the mean and smaller steps away from the mean).
// The function also takes in a step parameter where true is an increasing step and false is a decreasing step.
void Player::updateSpeed(int mean, bool step)
{
    float epsilon = 0.1;
    float ds = 0.0;
    if (step)
    {
        // Step function for how speed rises
        float dist = baseSpeed - mean;
        if (dist <= -15 + epsilon)
            ds = 1.5;
        else if (dist <= -7.5 + epsilon)
            ds = 1.5;
        else if (dist <= -5 + epsilon)
            ds = 1.0;
        else if (dist <= -1 + epsilon)
            ds = 0.5;
        else if (dist < 1 + epsilon)
            ds = 0.5;
        else if (dist < 5 + epsilon)
            ds = 0.25;
        else if (dist < 7.5 + epsilon)
            ds = 0.25;
        else if (dist < 15 + epsilon)
            ds = 0.25;
        else
            ds = 0.0;
    }
    else
    {
        // Step function for how speed drops
        float dist = baseSpeed - mean;
        if (dist <= -15 + epsilon)
            ds = 0.0;
        else if (dist <= -7.5 + epsilon)
            ds = -0.25;
        else if (dist <= -5 + epsilon)
            ds = -0.5;
        else if (dist <= -1 + epsilon)
            ds = -0.5;
        else if (dist < 1 + epsilon)
            ds = -0.5   ;
        else if (dist < 5 + epsilon)
            ds = -1.0;
        else if (dist < 7.5 + epsilon)
            ds = -1.0;
        else if (dist < 15 + epsilon)
            ds = -1.5;
        else
            ds = -1.5;
    }
    baseSpeed += ds;
    baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
    //tunnel->updateTSModifier();
}

// Assigns the actual final speed of the ship depending on the state of the game and the player.
// Final speed uses the in-game base speed of the player.
void Player::decideFinalSpeed(float elapsed)
{
    if (tunnel->isDone())
    {
        if (tunnel->getEval() != PASS && tunnel->getEval() != FAIL)
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
}

void Player::addAction(ActionCode actType)
{
    Action act;
    act.eventID = globals.stageID;
    act.levelID = tunnel ? tunnel->getStageNo() : -1;
    act.action = actType;
    act.timestamp = static_cast<int>(OgreFramework::getSingletonPtr()->totalElapsed * 1000);
    act.baseSpeed = baseSpeed;
    act.finalSpeed = finalSpeed;
    actions.push_back(act);
}

void Player::update(float elapsed)
{
    //std::cout << globals.initCamSpeed << std::endl;
    totalElapsed += elapsed;
    
    // Play music at beginning of stage
    if (triggerStartup && soundStartup && !soundStartup->isPlaying())
    {
        playSound(soundStartup);
        triggerStartup = false;
    }
    if (soundMusic)
    {
        startMusicTimer -= elapsed;
        if (startMusicTimer <= 0.0)
            soundMusic->play();
        if (fadeMusic)
        {
            float vol = soundMusic->getVolume();
            vol -= (0.5 * musicVolume) * elapsed;
            if (vol < 0.0) vol = 0.0;
            soundMusic->setVolume(vol);
        }
    }
    
    // Determine the speed of the player for this update
    decideFinalSpeed(elapsed);
    
    updateBadFuelPickUp(elapsed);
    updateBoost(elapsed);
    updateGlowExtraction(elapsed);
#ifdef DEBUG_MODE
    vines[0]->setPowerIndication(godMode);
#endif
    for(std::map<std::string,Powerup*>::iterator it=powerups.begin(); it != powerups.end(); ++it) {
        if( (it->first).compare("TimeWarp") == 0 ) {
            updateTimeWarp(elapsed);
        }
        else if( (it->first).compare("TractorBeam") == 0 ) {
            updateTractorBeam(elapsed);
        }
        else if( (it->first).compare("Shields") == 0 ) {
            updateShields(elapsed);
        }
    }

    //*******//
    // If score is being calculated, do not continue
    if( endFlag )
        return;
    // Winning animation
    if( tunnel->getEval() == PASS && tunnel->getFlyOut() )
    {
        if( endAnimationBegin ) {
            if( flyOutCounter >= endAnimationSuccessDuration ) {
                flyOutCounter = 0.0f;
                endAnimationBegin = false;
                flyOutAngleX = 0.0f;
                flyOutAngleY = 0.0f;
                flyLeft = true;
                
                endFlag = true;
                tutorialMgr->prepareSlides(TutorialManager::TUTORIAL_END_OF_STAGE, 0.0f);
                
                const int SCORE_PER_TICK = 100;
                const int NONZAP_PICKUP = 50;
                int tleft = 0;
                if (tunnel->getEval() == PASS)
                    tleft = tunnel->getStageTime() - tunnel->getTotalElapsed() - tunnel->getTimePenalty();
                int score = getNumCorrectTotal() * getScoring() + getNumPickupsTotal() * NONZAP_PICKUP + SCORE_PER_TICK * tleft;
                setScore(score);
                
                boostTimer = 0.0;
                soundBoost->stop();
                vines[0]->removeBoost();
            }
            else {
                flyOutCounter += elapsed;
                if( flyOutCamSpeed-0.5f < 0.0f ) {
                    flyOutCamSpeed = 0.0f;
                }
                else {
                    flyOutCamSpeed -= 0.5f;
                }
                
                move(getCamForward(true) * flyOutCamSpeed);
                
                Vector3 pos = vines[0]->getEntireVine()->getPosition();
                
                Vector3 moveOffset = /*getCamForward(true)*/Vector3::NEGATIVE_UNIT_Z * globals.globalModifierCamSpeed*finalSpeed*elapsed;
    
                
                // rotate about y
                moveOffset = Vector3(Math::Cos(Degree(flyOutAngleY))*moveOffset.x+Math::Sin(Degree(flyOutAngleY))*moveOffset.z,moveOffset.y,-Math::Sin(Degree(flyOutAngleY))*moveOffset.x+Math::Cos(Degree(flyOutAngleY))*moveOffset.z);
                
                // rotate about x
                moveOffset = Vector3(moveOffset.x,Math::Cos(Degree(flyOutAngleX))*moveOffset.y-Math::Sin(Degree(flyOutAngleX))*moveOffset.z,Math::Sin(Degree(flyOutAngleX))*moveOffset.y+Math::Cos(Degree(flyOutAngleX))*moveOffset.z);
            
                
                // THANKS CALVIN FOR MAKING THIS POSSIBLE... THIS ANIMATION WOULD HAVE NEVER WORKED IF IT WERE NOT FOR YOUR AMAZING AND BRILLIANT MIND!!! -Manny
                vines[0]->move(getCombinedRotAndRoll()*moveOffset); //<---
                
                if( flyLeft ) {
                    if( flyOutAngleY+1.5f > 12.0f ) {
                        flyLeft = false;
                        performBoost();
                    }
                    else {
                        vines[0]->getEntireVine()->rotate(Vector3::UNIT_Y, Degree(1.5f));
                        vines[0]->getEntireVine()->rotate(Vector3::UNIT_X, Degree(-1.2f));
                        flyOutAngleY += 1.5f;
                        flyOutAngleX -= 1.2f;
                    }
                }
                else {
                    finalSpeed++;
                    vines[0]->getEntireVine()->rotate(Vector3::UNIT_Y, Degree(-2.0f));
                    vines[0]->getEntireVine()->rotate(Vector3::UNIT_X, Degree(1.5f));
                    flyOutAngleY -= 2.0f;
                    flyOutAngleX += 1.5f;
                }
            }
        }
        else if( flyOutCounter >= flyOutDuration ) {
            endAnimationBegin = true;
            flyOutCounter = 0.0f;
        }
        else {
            flyOutCounter += elapsed;
            flyOutCamSpeed = globals.globalModifierCamSpeed*finalSpeed*elapsed;
            move(getCamForward(true) * flyOutCamSpeed);
            Vector3 moveOffset = getCamForward(true) * globals.globalModifierCamSpeed*finalSpeed*elapsed;
            vines[0]->move(moveOffset);
        }
    }
    // Losing animation
    else if( tunnel->getEval() == FAIL && tunnel->getFlyOut() )
    {
        if( !soundStart ) {
            OgreOggISound* sound = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("LevelFail");
            playSound(sound);
            soundStart = true;
        }
        
        if( flyOutCounter >= endAnimationFailDuration ) {
            flyOutCounter = 0.0f;
            flyOutSpeedUp = true;
            flyOutSpeed = 0.0f;
            soundStart = false;
            endFlag = true;
            
            const int SCORE_PER_TICK = 100;
            const int NONZAP_PICKUP = 50;
            int tleft = 0;
            if (tunnel->getEval() == PASS)
                tleft = tunnel->getStageTime() - tunnel->getTotalElapsed() - tunnel->getTimePenalty();
            int score = getNumCorrectTotal() * getScoring() + getNumPickupsTotal() * NONZAP_PICKUP + SCORE_PER_TICK * tleft;
            setScore(score);
            
            tutorialMgr->prepareSlides(TutorialManager::TUTORIAL_END_OF_STAGE, 0.0f);
        }
        else {
            flyOutCounter += elapsed;
            if( flyOutSpeedUp ) {
                if( flyOutSpeed >= 0.15f ) {
                    flyOutSpeed = 0.15f;
                    flyOutSpeedUp = false;
                }
                else {
                    flyOutSpeed += flyOutIncr;
                }
            }
            else {
                if( flyOutSpeed <= 0.0f ) {
                    flyOutSpeed = 0.0f;
                }
                else {
                    flyOutSpeed -= flyOutIncr;
                }
            }
            vines[0]->move(getCamUpward() * -flyOutSpeed);
        }
    }
    else if (animationTimer <= 0.0)
    {
        endFlag = true;
        tutorialMgr->prepareSlides(TutorialManager::TUTORIAL_END_OF_STAGE, 0.0f);
        
        const int SCORE_PER_TICK = 100;
        const int NONZAP_PICKUP = 50;
        int tleft = 0;
        if (tunnel->getEval() == PASS)
            tleft = (tunnel->getStageTime() - tunnel->getTimeLeft());
        int score = getNumCorrectTotal() * getScoring() + getNumPickupsTotal() * NONZAP_PICKUP + SCORE_PER_TICK * tleft;
        setScore(score);
        
        animationTimer = 5.0; // Prevent two slides at once
    }
    else // Game is still going
    {
        // Interpolate the camera to get smooth transitions
        TunnelSlice* next = tunnel->getNext(1);
        if (next)
        {
            Vector3 endOfSlice = next->getEnd();
            Vector3 dir = (endOfSlice - camPos).normalisedCopy();
            Vector3 delta = dir * (globals.globalModifierCamSpeed * finalSpeed * elapsed);
            move(delta);
            camRot = oldRot.Slerp(1 - (endOfSlice - camPos).length() / (endOfSlice - oldPos).length(), oldRot, desireRot);
        }
        
        // Orient the ship in front of the camera at all times
        offsetShip(elapsed);
        
        // Check for collisions for player and the tunnel
        checkCollisions();
    }
    // Record the segment info player has passed
    recordInfo();
}

void Player::initPowerUps()
{
    powerups["TractorBeam"] = new TractorBeam();
    powerups["TimeWarp"] = new TimeWarp(0.0f,0.45f,20.0f,75.0f);
    powerups["Shields"] = new Shields();
    
    powerups["TractorBeam"]->available = false;
    powerups["TimeWarp"]->available = false;
    powerups["Shields"]->available = false;
}

void Player::setPowerUp( std::string pwr, bool val )
{
    if( powerups.find(pwr) != powerups.end() )
        powerups[pwr]->available = val;
}

// Is the powerup available for use?
bool Player::isPowerUpAvailable( std::string pwr )
{
    if( powerups.find(pwr) != powerups.end())
        return powerups[pwr]->available;
    
    return false;
}

// Is the powerup executing?
bool Player::isPowerUpActive( std::string pwr )
{
    if( powerups.find(pwr) != powerups.end())
        return powerups[pwr]->active;
    
    return false;
}

Powerup* Player::getPowerUpPtr( std::string pwr )
{
    if( powerups.find(pwr) != powerups.end() )
        return powerups[pwr];
    
    return NULL;
}

void Player::performPowerUp( std::string pwr )
{
    std::cout << "Powerup: " << pwr << std::endl;
    if( isPowerUpAvailable(pwr) ) {
        if( pwr.compare("TimeWarp") == 0 ) {
            performTimeWarp();
        }
        else if( pwr.compare("TractorBeam") == 0 ) {
            performTractorBeam();
        }
        else if( pwr.compare("Shields") == 0 ) {
            performShields();
        }
    }
}

void Player::destroyPowerUps()
{
    for(std::map<std::string,Powerup*>::iterator it=powerups.begin(); it != powerups.end(); ++it) {
        delete it->second;
    }
}

void Player::resetPowerUps()
{
    for(std::map<std::string,Powerup*>::iterator it=powerups.begin(); it != powerups.end(); ++it) {
        it->second->reset();
    }
}

void Player::incrementNumStagesWon()
{
    ++numStagesWon;
}

void Player::calculateNavigationScores()
{
    skillLevel.calculateNavigation();
}

void Player::calculateSpeedScores()
{
    skillLevel.calculateSpeed();
}

std::string Player::getCurrentStats() const
{
    return skillLevel.getCurrentStats();
}

void Player::saveAllResults(Evaluation eval)
{
#define SCORE_PER_SECOND 100
    
    // Calculate total score which is current plus time left
    //
    // This is now done in animation loop, so don't use it
    //score += (static_cast<int>(tunnel->getTimeLeft()) * SCORE_PER_SECOND);
    
    // Assign the correct rating based on tunnel results
    int nrating = -1;
    if (eval == PASS)
    {
        nrating = 5;
        incrementNumStagesWon();
    }
    else
    {
        // Recess is based off of a high number of collection (% collected)
        if (tunnel->getMode() == STAGE_MODE_RECESS)
        {
            float percentComplete = tunnel->getPercentComplete();
            if (percentComplete >= 0.90)
                nrating = 4;
            else if (percentComplete >= 0.75)
                nrating = 3;
            else if (percentComplete >= 0.50)
                nrating = 2;
            else if (percentComplete >= 0.20)
                nrating = 1;
            else
                nrating = 0;
        }
        else
        {
            nrating = tunnel->getStarPhase();
        }
    }
    
    PlayerProgress* levelResult;
    if(levelRequest)
    {
        // If assigned a specific level (via scheduler)
        levelResult = &levelRequest->second;
    }
    else
    {
        // If level played by 2-D grid select
        levelResult = &(levelProgress[levelRequestRow][levelRequestCol]);
    }
    
    // If level has never been done before or we have a new high score, then save stats
    if (levelResult->rating < 0 || score > levelResult->score)
    {
        levelResult->score = score;
        levelResult->time = tunnel->getTimeLeft();
        levelResult->numCorrect = numCorrectTotal;
        levelResult->numWrong = numWrongTotal;
        levelResult->numSafe = numSafeTotal;
        levelResult->numMissed = numMissedTotal;
        levelResult->numPickups = numPickupsTotal;
        levelResult->startSpeed = initSpeed;
        levelResult->exitSpeed = baseSpeed;
        // Assign other level progress info here since it is a new score
        
        // For level scheduler set new speed to be the average of the ending speed and init speed setting
        if (levelRequest)
        {
            switch (levelRequest->first.phaseX)
            {
                case PHASE_COLOR_SOUND:
                {
                    float nspeed = (scheduler->speedA + baseSpeed) / 2;
                    if (nrating >= 5 || nspeed < scheduler->speedA)
                        scheduler->speedA = nspeed;
                    scheduler->speedA = Util::clamp(scheduler->speedA, globals.minCamSpeed, globals.maxCamSpeed);
                    scheduler->firstTimeA = false;
                    break;
                }
                case PHASE_SHAPE_SOUND:
                {
                    float nspeed = (scheduler->speedB + baseSpeed) / 2;
                    if (nrating >= 5 || nspeed < scheduler->speedB)
                        scheduler->speedB = nspeed;
                    scheduler->speedB = Util::clamp(scheduler->speedB, globals.minCamSpeed, globals.maxCamSpeed);
                    scheduler->firstTimeB = false;
                    break;
                }
                case PHASE_SOUND_ONLY:
                {
                    float nspeed = (scheduler->speedC + baseSpeed) / 2;
                    if (nrating >= 5 || nspeed < scheduler->speedC)
                        scheduler->speedC = nspeed;
                    scheduler->speedC = Util::clamp(scheduler->speedC, globals.minCamSpeed, globals.maxCamSpeed);
                    scheduler->firstTimeC = false;
                    break;
                }
                case PHASE_ALL_SIGNAL:
                {
                    float nspeed = (scheduler->speedD + baseSpeed) / 2;
                    if (nrating >= 5 || nspeed < scheduler->speedD)
                        scheduler->speedD = nspeed;
                    scheduler->speedD = Util::clamp(scheduler->speedD, globals.minCamSpeed, globals.maxCamSpeed);
                    scheduler->firstTimeD = false;
                    break;
                }
                case PHASE_COLLECT:
                {
                    float nspeed = (scheduler->speedE + baseSpeed - globals.stageTotalCollections / 3) / 2;
                    
                    if (nrating >= 5 || nspeed < scheduler->speedE)
                        scheduler->speedE = nspeed;
                    scheduler->speedE = Util::clamp(scheduler->speedE, globals.minCamSpeed, globals.maxCamSpeed);
                    scheduler->firstTimeE = false;
                    break;
                }
                default:
                    break;
            }
        }
    }
    // Update other level results/settings
    levelResult->initSpeedSetting = initSpeed; // Done in newTunnel(...) as well, but save here anyway
    levelResult->setRating(nrating); // Assign rating last
    
    setSkillLevel(skillLevel);
    logData();
    saveProgress(globals.savePath);
}

void Player::logData()
{
    if (results.size() > 0 && !logged)
    {
        saveStage(globals.logPath);
        saveActions(globals.actionPath);
        saveSession(globals.sessionPath);
        logged = true;
        globals.stageID++;
    }
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
            out << "% Tunnel Log: " << endl;
            out << "% Recall Log Version 2.0 debug seed: " << seed << endl;
            out << "%" << endl;
            out << "% SegEncNW { '0'=no panel, '1'=empty panel, '2'=player, '3'=match, '4'=nonmatch, '5'=obstacle, '6'=time warp }" << endl;
            out << "% SegEncN" << endl;
            out << "% SegEncNE" << endl;
            out << "% SegEncE" << endl;
            out << "% SegEncSE" << endl;
            out << "% SegEncS" << endl;
            out << "% SegEncSW" << endl;
            out << "% SegEncW" << endl;
            out << "% Event Number { 0, inf }" << endl;
            out << "% Level Number { 0, inf }" << endl;
            out << "% Task Type { 0=Recess, 1=Color/Sound, 2=Shape/Sound, 3=Sound, 4=AllFeatures }" << endl;
            out << "% N-Back { 0, inf }" << endl;
            out << "% Player Roll Base { 0, 359 }" << endl;
            out << "% Player Roll Offset { 0, 359 }" << endl;
            out << "% Player Roll Speed { -inf, inf }" << endl;
            out << "% Player Fuel Timer { -inf, inf }" << endl;
            out << "% Player Fuel Buffer { -inf, inf }" << endl;
            out << "% Player Loc { 0=Northwest ... 7=West }" << endl;
            out << "% Pod Loc { -1=N/A, 0=Northwest ... 7=West }" << endl;
            out << "% Pod Color { -1=N/A, 0-3, 4=holdout, 5/6=special }" << endl;
            out << "% Pod Shape { -1=N/A, 0-3, 4=holdout }" << endl;
            out << "% Pod Sound { -1=N/A, 0-3, 4=holdout }" << endl;
            out << "% Pod Match { -1=N/A, 0=No, 1=Yes }" << endl;
            out << "% Pod Taken { -1=N/A, 0=No, 1=Yes }" << endl;
            out << "% Pod Zapped { -1=N/A, 0=No, 1=Yes }" << endl;
            out << "% Timestamp (ms)" << endl;
            out << "% Num Obstacles { 0, inf }" << endl;
            out << "% Level Ended { 0, 1 }" << endl;
            out << "% Min Speed { 0, inf }" << endl;
            out << "% Max Speed { 0, inf }" << endl;
            out << "% Base Speed { 0, inf }" << endl;
            out << "% Final Speed { 0, inf }" << endl;
            out << "% Segment Direction { 0=Northwest ... 7=West }" << endl;
            out << "% Segment Angle { 0, inf }" << endl;
            out << "% Segment Panels { 0, inf }" << endl;
            out << "%" << endl;
            out << "% SegEncNW SecEncN SegEncNE SegEncE SegEncSE SegEncS SegEncSW SegEncW EventNumber LevelNumber TaskType N-Back PlayerRollBase PlayerRollOffset PlayerRollSpeed PlayerFuelTimer PlayerFuelBuffer PlayerLoc PodLoc PodColor PodShape PodSound PodMatch PodTaken PodZapped Timestamp NumObs LevelEnded MinSpeed MaxSpeed BaseSpeed FinalSpeed SegmentDir SegmentAngle SegmentPanels" << endl;
        }
        
        for (std::list<Result>::iterator it = results.begin(); it != results.end(); ++it) {
            //out << SOUTH << " "
            
            int nobs = 0;
            for (int i = 0; i < NUM_DIRECTIONS; ++i)
            {
                if (it->segmentEncoding[i] == ENC_OBSTACLE) nobs++;
                out << it->segmentEncoding[i] << " ";
            }
            out << it->eventID << " "
            << it->levelID << " "
            << it->taskType << " "
            << it->nback << " "
            << it->playerRollBase << " "
            << it->playerRollOffset << " "
            << it->playerRollSpeed << " "
            << it->playerFuelTimer << " "
            << it->playerFuelBuffer << " "
            << it->playerLoc << " ";
            if (it->podInfo.podExists)
            {
                out << it->podInfo.podLoc << " "
                << it->podInfo.podColor << " "
                << it->podInfo.podShape << " "
                << it->podInfo.podSound << " "
                << it->podInfo.goodPod << " "
                << it->podInfo.podTaken << " "
                << it->podInfo.podZapped << " ";
            }
            else
                out << "-1 -1 -1 -1 -1 -1 -1 ";
            out << it->timestamp << " ";
            out << nobs << " ";
            out << it->levelEnded << " ";
            out << it->minSpeed << " "
            << it->maxSpeed << " "
            << it->baseSpeed << " "
            << it->finalSpeed << " "
            << it->sectionInfo.tunnelDir << " "
            << it->sectionInfo.tunnelDirAngle << " "
            << Util::getNumSides(it->sectionInfo.sidesUsed) << endl;
        }
        out.close();
    }
    else {
        out.close();
        return false;
    }
    
    return true;
}

bool Player::saveActions(std::string file)
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
    
    std::cout << "Saving action log: " << file << std::endl;
    
    if (out.good()) {
        if (newFile) {
            out << "% Action Log: " << endl;
            out << "% Recall Log Version 2.0 debug seed: " << seed << endl;
            out << "%" << endl;
            out << "% Event Number { 0, inf }" << endl;
            out << "% Level Number { 0, inf }" << endl;
            out << "% Action Type { 0=None, 1=SingleTap, 2=DoubleTap, 6=Pinch } (Does not track pan, use log data to check player speed)" << endl;
            out << "% Timestamp (ms)" << endl;
            out << "% Base Speed { 0, inf }" << endl;
            out << "% Final Speed { 0, inf }" << endl;
            out << "%" << endl;
            out << "% EventNumber LevelNumber ActionType Timestamp BaseSpeed FinalSpeed" << endl;
        }
        
        for (std::list<Action>::iterator it = actions.begin(); it != actions.end(); ++it) {
            //out << SOUTH << " "
            
            out << it->eventID << " "
            << it->levelID << " "
            << it->action << " "
            << it->timestamp << " "
            << it->baseSpeed << " "
            << it->finalSpeed << endl;
        }
        out.close();
    }
    else {
        out.close();
        return false;
    }
    
    return true;
}

bool Player::saveSession(std::string file)
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
    
    std::cout << "Saving session log: " << file << std::endl;
    
    // Extract end results
    sessions.back().timestampOut = (int)(OgreFramework::getSingletonPtr()->totalElapsed * 1000);
    sessions.back().runSpeedOut = baseSpeed;
    sessions.back().eval = tunnel->getEval();
    if (numCorrectTotal + numWrongTotal + numMissedTotal > 0)
        sessions.back().accuracy = static_cast<float>(numCorrectTotal) / (numCorrectTotal + numWrongTotal + numMissedTotal);
    else
        sessions.back().accuracy = 0.0f;
    sessions.back().TP = numCorrectTotal;
    sessions.back().FP = numWrongTotal;
    sessions.back().TN = numMissedTotal;
    sessions.back().FN = numSafeTotal;
    sessions.back().ignored = numIgnoredTotal;
    sessions.back().pickups = numPickupsTotal;
    sessions.back().obsHit = numCollisionsTotal;
    sessions.back().obsAvoided = numAvoidancesTotal;
    sessions.back().score = score;
    sessions.back().totalMarbles = scheduler->totalMarbles;
    sessions.back().nbackLevelA = scheduler->nBackLevelA;
    sessions.back().nbackLevelB = scheduler->nBackLevelB;
    sessions.back().nbackLevelC = scheduler->nBackLevelC;
    sessions.back().nbackLevelD = scheduler->nBackLevelD;
    sessions.back().nbackLevelE = scheduler->nBackLevelE;
    sessions.back().scoreCurr = scheduler->scoreCurr;
    sessions.back().currentHoldout = scheduler->currentHoldout;
    sessions.back().holdoutOffsetA = scheduler->holdoutOffsetA;
    sessions.back().holdoutOffsetB = scheduler->holdoutOffsetB;
    sessions.back().holdoutOffsetD = scheduler->holdoutOffsetD;
    sessions.back().speedA = scheduler->speedA;
    sessions.back().speedB = scheduler->speedB;
    sessions.back().speedC = scheduler->speedC;
    sessions.back().speedD = scheduler->speedD;
    sessions.back().speedE = scheduler->speedE;
    sessions.back().musicVolume = musicVolume;
    sessions.back().soundVolume = soundVolume;
    sessions.back().syncDataToServer = syncDataToServer;
    sessions.back().maxVel = maxVel;
    sessions.back().minVelFree = minVelFree;
    sessions.back().minVelStopper = minVelStopper;
    sessions.back().dampingDecayFree = dampingDecayFree;
    sessions.back().dampingDecayStop = dampingDecayStop;
    sessions.back().dampingDropFree = dampingDropFree;
    sessions.back().dampingDropStop = dampingDropStop;
    sessions.back().inverted = inverted;

    if (out.good()) {
        if (newFile) {
            out << "% Session Log: " << endl;
            out << "% Recall Log Version 2.0 debug seed: " << seed << endl;
            out << "%" << endl;
            out << "% Ways to compute:" << endl;
            out << "% Total Zapped - TP + FP" << endl;
            out << "% N-Back Matches Zapped - TP" << endl;
            out << "% N-Back Matches Not Zapped but Grabbed - Ignored" << endl;
            out << "% Total Grabbed - Pickups + Ignored" << endl;
            out << "% N-Back Non-Matches Zapped - FP" << endl;
            out << "% N-Back Non-Matches Not Zapped but Grabbed - Pickups" << endl;
            out << "%" << endl;
            out << "% Session Number { 0, inf }" << endl;
            out << "% Event Number { 0, inf }" << endl;
            out << "% Level Number { 0, inf }" << endl;
            out << "% Task Type { 0=Recess, 1=Color/Sound, 2=Shape/Sound, 3=Sound, 4=AllFeatures }" << endl;
            out << "% Has Holdout { 0, 1 }" << endl;
            out << "% TSin - Timestamp In (ms)" << endl;
            out << "% TSout - Timestamp Out (ms)" << endl;
            out << "% N-Back { 0, inf }" << endl;
            out << "% RunSpeedIn { 0, inf }" << endl;
            out << "% RunSpeedOut { 0, inf }" << endl;
            out << "% LevelEnding { 0=PASS, 1=LOSE_TIME_OR_X, 2=LOSE_OUT_OF_FUEL, 3=UNFINISHED }" << endl;
            out << "% Accuracy - TP / (TP + FP + TN) { 0-1 }" << endl;
            out << "% TP - Total Zapped and Match { 0, inf }" << endl;
            out << "% FP - Total Zapped and Non-Match { 0, inf }" << endl;
            out << "% TN - Total Missed and Match { 0, inf }" << endl;
            out << "% FN - Total Missed and Non-Match { 0, inf }" << endl;
            out << "% Ignored - Total Grabbed and Match { 0, inf }" << endl;
            out << "% Pickups - Total Grabbed and Non-Match { 0, inf }" << endl;
            out << "% ObsHit - Segments with Obstacles Hit { 0, inf }" << endl;
            out << "% ObsAvoid - Segments with Obstacles Avoided { 0, inf }" << endl;
            out << "% Score - Player points earned in level" << endl;
            out << "% Total Marbles - Levels left in the random pool for the scheduler" << endl;
            out << "% NBackLevelA - Color/Sound scheduler skill level" << endl;
            out << "% NBackLevelB - Shape/Sound scheduler skill level" << endl;
            out << "% NBackLevelC - Sound Only  scheduler skill level" << endl;
            out << "% NBackLevelD - All Signals scheduler skill level" << endl;
            out << "% NBackLevelE - Recess      scheduler skill level" << endl;
            out << "% Scheduler Score - Total scheduler score" << endl;
            out << "% Current Holdout - Holdout experience in scheduler" << endl;
            out << "% Holdout OffsetA - Offset to Color/Sound skill level in scheduler" << endl;
            out << "% Holdout OffsetB - Offset to Shape/Sound skill level in scheduler" << endl;
            out << "% Holdout OffsetD - Offset to All Signals skill level in scheduler" << endl;
            out << "% SpeedA - Scheduler recommended speed for Color/Sound" << endl;
            out << "% SpeedB - Scheduler recommended speed for Shape/Sound" << endl;
            out << "% SpeedC - Scheduler recommended speed for Sound Only" << endl;
            out << "% SpeedD - Scheduler recommended speed for All Signals" << endl;
            out << "% SpeedE - Scheduler recommended speed for Recess" << endl;
            out << "% Music Volume - Settings music volume" << endl;
            out << "% Sound Volume - Settings sound volume" << endl;
            out << "% Sync Data To Server - Settings that lets user to sync log data to server" << endl;
            out << "% Max Vel - Controller Settings for maximum angular spin" << endl;
            out << "% Min Vel Free - deprecated" << endl;
            out << "% Min Vel Stopper - Controller Settings for minimum velocity to force stop in free motion" << endl;
            out << "% Damping Decay Free - Controller Settings for a multiplier to slow down in free motion" << endl;
            out << "% Damping Decay Stop - Controller Settings for a multiplier to slow down by held down input" << endl;
            out << "% Damping Drop Free - Controller Settings for a linear slow down in free motion" << endl;
            out << "% Damping Drop Stop - Controller Settings for a linear slow down by held down input" << endl;
            out << "% Inverted - Controller Settings to invert direction" << endl;
            out << "% fuelEnabled - Study Settings to enable fuel usage" << endl;
            out << "% holdoutEnabled - Study Settings to enable holdout" << endl;
            out << "% ofVelocity - Study Settings to initialize starting speed" << endl;
            out << "% manRecessEnabled - Study Settings to enable mandatory recess" << endl;
            out << "% manRecessLevelLimit - Study Settings to set the number of levels before mandatory recess" << endl;
            out << "% newNavEnabled - Study Settings to enable senior navigation" << endl;
            out << "% newNavIncrement - Study Settings to set senior navigation progression" << endl;
            out << "% indRecessEnabled - Study Settings to enable independent recess" << endl;
            out << "% indRecessIncrement - Study Settings to set independent recess increment" << endl;
            out << "% holdoutdelayEnabled - Study Settings to enable delayed holdout" << endl;
            out << "% holdoutdelayNumber - Study Settings to set holdout delay memory level" << endl;
            out << "% newSounds - Study Settings to use new sounds" << endl;
            out << "%" << endl;
            out << "% SessionNumber EventNumber LevelNumber TaskType HasHoldout TSin TSout N-Back RunSpeedIn RunSpeedOut LevelEnding Accuracy TP FP TN FN Ignored Pickups ObsHit ObsAvoid Score TotalMarbles NBackLevelA NBackLevelB NBackLevelC NBackLevelD NBackLevelE SchedulerScore CurrentHoldout HoldoutOffsetA HoldoutOffsetB HoldoutOffsetD SpeedA SpeedB SpeedC SpeedD SpeedE MusicVolume SoundVolume SyncDataToServer MaxVel MinVelFree MinVelStopper DampingDecayFree DampingDecayStop DampingDropFree DampingDropStop Inverted fuelEnabled holdoutEnabled initialVelocity manRecessEnabled manRecessLevelLimit newNavEnabled newNavIncrement indRecessEnabled indRecessIncrement holdoutdelayEnabled holdoutdelayNumber newSounds" << endl;
        }
        
        //Logging Study Settings
        out << sessions.back().sessionID << " "
        << sessions.back().eventID << " "
        << sessions.back().levelID << " "
        << sessions.back().taskType << " "
        << sessions.back().hasHoldout << " "
        << sessions.back().timestampIn << " "
        << sessions.back().timestampOut << " "
        << sessions.back().nback << " "
        << sessions.back().runSpeedIn << " "
        << sessions.back().runSpeedOut << " "
        << sessions.back().eval << " "
        << sessions.back().accuracy << " "
        << sessions.back().TP << " "
        << sessions.back().FP << " "
        << sessions.back().TN << " "
        << sessions.back().FN << " "
        << sessions.back().ignored << " "
        << sessions.back().pickups << " "
        << sessions.back().obsHit << " "
        << sessions.back().obsAvoided << " "
        << sessions.back().score << " "
        << sessions.back().totalMarbles << " "
        << sessions.back().nbackLevelA << " "
        << sessions.back().nbackLevelB << " "
        << sessions.back().nbackLevelC << " "
        << sessions.back().nbackLevelD << " "
        << sessions.back().nbackLevelE << " "
        << sessions.back().scoreCurr << " "
        << sessions.back().currentHoldout << " "
        << sessions.back().holdoutOffsetA << " "
        << sessions.back().holdoutOffsetB << " "
        << sessions.back().holdoutOffsetD << " "
        << sessions.back().speedA << " "
        << sessions.back().speedB << " "
        << sessions.back().speedC << " "
        << sessions.back().speedD << " "
        << sessions.back().speedE << " "
        << sessions.back().musicVolume << " "
        << sessions.back().soundVolume << " "
        << sessions.back().syncDataToServer << " "
        << sessions.back().maxVel << " "
        << sessions.back().minVelFree << " "
        << sessions.back().minVelStopper << " "
        << sessions.back().dampingDecayFree << " "
        << sessions.back().dampingDecayStop << " "
        << sessions.back().dampingDropFree << " "
        << sessions.back().dampingDropStop << " "
        << sessions.back().inverted << " "
        << fuelEnabled << " "
        << holdoutEnabled << " "
        << initialVelocity << " "
        << manRecessEnabled << " "
        << manRecessLevelLimit << " "
        << newNavEnabled << " "
        << newNavIncrement << " "
        << indRecessEnabled << " "
        << indRecessIncrement << " "
        << holdoutdelayEnabled << " "
        << holdoutdelayNumber << " "
        << globals.newSounds << "\n";
        
        out.close();
    }
    else {
        out.close();
        return false;
    }
    
    return true;
}

// Save based on player results in level progression
bool Player::saveProgress(std::string file)
{
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    bool ret = true;
    
    out << "V1.5" << std::endl;
    
    out << levelProgress.size() << std::endl;
    for (int i = 0; i < levelProgress.size(); ++i)
    {
        for (int j = 0; j < levelProgress[i].size(); ++j)
            out << "level" << " " << i << " " << j << " " << levelProgress[i][j] << std::endl;
    }
    
    
    out << "sessionID" << " " << sessionID << std::endl;
    out << "tutorial1.0" << " " << (*tutorialMgr) << std::endl;
    out << "scheduler1.0" << " " << (*scheduler) << std::endl;
    out << "rerollCounter" << " " << rerollCounter << std::endl;
    out << "musicVolume" << " " << musicVolume << std::endl;
    out << "soundVolume" << " " << soundVolume << std::endl;
    out << "syncDataToServer" << " " << syncDataToServer << std::endl;
    out << "maxVel" << " " << maxVel << std::endl;
    out << "minVelStopper" << " " << minVelStopper << std::endl;
    out << "holdout" << " " << holdout << std::endl;
    out << "holdoutLB" << " " << holdoutLB << std::endl;
    out << "holdoutUB" << " " << holdoutUB << std::endl;
    out << "dampingDecayFree" << " " << dampingDecayFree << std::endl;
    out << "dampingDecayStop" << " " << dampingDecayStop << std::endl;
    out << "dampingDropFree" << " " << dampingDropFree << std::endl;
    out << "dampingDropStop" << " " << dampingDropStop << std::endl;
    out << "inverted" << " " << inverted << std::endl;
    //Study Settings
    out << "fuelEnabled" << " " << fuelEnabled << std::endl;
    out << "holdoutEnabled" << " " << holdoutEnabled << std::endl;
    out << "initialVelocity" << " " << initialVelocity << std::endl;
    out << "manRecessEnabled" << " " << manRecessEnabled << std::endl;
    out << "manRecessLevelLimit" << " " << manRecessLevelLimit << std::endl;
    out << "newNavEnabled" << " " << newNavEnabled << std::endl;
    out << "newNavIncrement" << " " << newNavIncrement << std::endl;
    out << "indRecessEnabled" << " " << indRecessEnabled << std::endl;
    out << "indRecessIncrement" << " " << indRecessIncrement << std::endl;
    out << "holdoutdelayEnabled" << " " << holdoutdelayEnabled << std::endl;
    out << "holdoutdelayNumber" << " " << holdoutdelayNumber << std::endl;
    out << "newSounds" << " " << globals.newSounds << std::endl;





    
    
    std::cout << "Save Level Progress: " << file << std::endl;
    ret = out.good();
    
    out.close();
    return ret;
}

// Load based on player results in level progression
// Version 1.5
bool Player::loadProgress1_5(std::string savePath)
{
    std::ifstream saveFile (savePath.c_str());
    
    if (saveFile.good()) {
        std::string input;
        saveFile >> input; // Receive version string
        
        std::map<std::string, bool> ignoreList;
        while (saveFile >> input)
        {
            setSaveValue(saveFile, input, ignoreList);
        }
    }
    return saveFile.eof();
}

// Loads player progress. First it decides which version,
// the save file is and calls the correct function
bool Player::loadProgress(std::string savePath)
{
    std::ifstream saveFile (savePath.c_str());
    
    if (saveFile.good()) {
        std::string input;
        saveFile >> input;
        
        saveFile.close();
        if (input == "V1.5")
            return loadProgress1_5(savePath);
        else
            return false;
    }
    return false;
}

std::istream& Player::setSaveValue(std::istream& in, std::string paramName, std::map<std::string, bool> ignoreList)
{
    if (paramName == "sessionID")
        in >> sessionID;
    if (paramName == "tutorial1.0")
        in >> (*tutorialMgr);
    else if (paramName == "scheduler1.0")
        in >> (*scheduler);
    else if (paramName == "rerollCounter")
        in >> rerollCounter;
    else if (paramName == "levelSize")
    {
        int size;
        in >> size;
        
        levelProgress = std::vector< std::vector<PlayerProgress> >(size);
        for (int i = 0; i < levelProgress.size(); ++i)
            levelProgress[i] = std::vector<PlayerProgress>(NUM_TASKS);
    }
    else if (paramName == "level")
    {
        int row, col;
        in >> row;
        in >> col;
        in >> levelProgress[row][col];
    }
    else if (paramName == "soundVolume")
        in >> soundVolume;
    else if (paramName == "musicVolume")
        in >> musicVolume;
    else if (paramName == "syncDataToServer")
        in >> syncDataToServer;
    else if (paramName == "maxVel")
        in >> maxVel;
    else if (paramName == "minVelStopper")
        in >> minVelStopper;
    else if (paramName == "holdout")
        in >> holdout;
    else if (paramName == "holdoutLB")
        in >> holdoutLB;
    else if (paramName == "holdoutUB")
        in >> holdoutUB;
    else if (paramName == "dampingDecayFree")
        in >> dampingDecayFree;
    else if (paramName == "dampingDecayStop")
        in >> dampingDecayStop;
    else if (paramName == "dampingDropFree")
        in >> dampingDropFree;
    else if (paramName == "dampingDropStop")
        in >> dampingDropStop;
    else if (paramName == "inverted")
        in >> inverted;
    else if(paramName == "fuelEnabled")
        in >> fuelEnabled;
    else if(paramName == "holdoutEnabled")
        in >> holdoutEnabled;
    else if(paramName == "initialVelocity")
        in >> initialVelocity;
    else if(paramName == "manRecessEnabled")
        in >> manRecessEnabled;
    else if(paramName == "manRecessLevelLimit")
        in >> manRecessLevelLimit;
    else if(paramName == "newNavEnabled")
        in >> newNavEnabled;
    else if(paramName == "newNavIncrement")
        in >> newNavIncrement;
    else if(paramName == "indRecessEnabled")
        in >> indRecessEnabled;
    else if(paramName == "indRecessIncrement")
        in >> indRecessIncrement;
    else if(paramName == "holdoutdelayEnabled")
        in >> holdoutdelayEnabled;
    else if(paramName == "holdoutdelayNumber")
        in >> holdoutdelayNumber;
    else if (paramName == "newSounds")
        in >> globals.newSounds;
    return in;
    

    
}

// Initializes control settings
void Player::initSettings()
{
    //maxVel = 4500.0f;   // Maximum motion velocity                  original: 4500.0f
    //minVelStopper = 1000.0f;    // Stop gliding at center of panels       original2 : 900.0f
    //dampingDecayFree = 0.9500f; // Free Motion damping multiplier          original: 0.9661f
    //dampingDecayStop = 0.5000f; // Stop Motion damping multiplier           original: 0.1000f
    //dampingDropFree = 50.0f;    // Free Motion damping linear drop          original2 : 50.0f
    //dampingDropStop = 50.0f;    // Stop Motion damping linear drop          original2 : 50.0f
    
    // VBZ
    maxVel = 4500.0f;   // Maximum motion velocity
    minVelFree = 200.0f;
    minVelStopper = 1150.0f;    // Stop gliding at center of panels
    dampingDecayFree = 0.937f; // Free Motion damping multiplier
    dampingDecayStop = 0.500f; // Stop Motion damping multiplier
    dampingDropFree = 25.0f;    // Free Motion damping linear drop
    dampingDropStop = 50.0f;    // Stop Motion damping linear drop
}

void Player::feedLevelRequestFromSchedule()
{
    std::vector< std::pair<StageRequest, PlayerProgress> > choices = scheduler->generateChoices(holdoutEnabled,newNavEnabled,indRecessEnabled,indRecessNBackLevel,holdoutdelayEnabled,holdoutdelayNumber);
    scheduleChoice1 = choices[0];
    scheduleChoice2 = choices[1];
    scheduleChoice3 = choices[2];
    scheduleManRecessLevel = choices[3];
//    // assign the level to be played in the same position as the iterator or the schedule
//    levelRequest = &scheduler->schedule[0][0].first;
//    
//    // Increment to the next stagerequest
//    scheduler->scheduleIt++;
//    
//    // if the schedule reaches the end, wrap it back to beginning
//    if( scheduler->scheduleIt == scheduler->schedule.end() )
//        scheduler->scheduleIt = scheduler->schedule.begin();
}

// Returns a multiplier when incrementing or decrementing memory level during assessment
float Player::obtainDifficultyWeight(StageRequest level, PlayerProgress assessment, float nBackDelta)
{
    float valMemory = 1.0;
    
    float nBackChallenge = getMemoryChallenge(level, assessment);
    if (level.phaseX == PHASE_COLLECT)
    {
        valMemory = 0.0;
        if(indRecessEnabled)
        {
            valMemory = 1.0;
        }
        
        //valMemory = 1.0;    // Don't penalize or benefit on memory if it's recess
        //valMemory = 0.0;    // Recess is now worth jack
    }
    else if ( nBackChallenge < -0.5 )
    {
        // easy memory
        if (nBackDelta >= 0.0)
            valMemory = 1.0;    // Winning an easy mem adds to nav score, so don't zero it out
        else
            valMemory = 1.5;
    }
    else if ( nBackChallenge < 0.5 )
    {
        // normal memory
        valMemory = 1.0;
    }
    else //if ( nBackChallenge >= 0.5 )
    {
        // hard memory
        if (nBackDelta >= 0.0)
            valMemory = 1.5;    // hard memory
        else
            valMemory = 0.0;
    }

    return valMemory;
}


// Returns a multiplier when incrementing or decrementing memory level during assessment
float Player::obtainSamplingWeight(StageRequest level, PlayerProgress assessment)
{
    float valLength = 1.0;
    
    // Not only is it shorter times and stuff, the accuracy requirement
    // for passing is lower for easy time. So it is a very strong multiplier
    if (level.phaseX == PHASE_COLLECT)
        valLength = 1.0;
    else if (level.collectionCriteria.size() <= 4)
        valLength = 0.6;    // easy time
    else if (level.collectionCriteria.size() <= 8)
        valLength = 0.8;    // normal time
    else //if (level.collectionCriteria.size() <= 13)
        valLength = 1.0;    // hard time
    
    return valLength;
}

float Player::getMemoryChallenge(StageRequest level, PlayerProgress assessment) const
{
    float nBackChallenge = level.nback - assessment.nBackSkill;
    if (level.hasHoldout())
        nBackChallenge -= assessment.nBackOffset;
    return nBackChallenge;
}

float Player::modifyNBackDelta(StageRequest level, PlayerProgress assessment, float accuracy, bool exclude)
{
    //If Ind Recess Enabled, Use other increment!
    
    
    
    
    // Base nBackDelta increment/decrement (-0.35 <= nBackDelta <= 0.35)
    float softcaptemp;
    float nBackChallenge = getMemoryChallenge(level, assessment);
    bool tooEasy = nBackChallenge < -0.5;
    if(indRecessEnabled && level.phaseX == PHASE_COLLECT)
    {
        
        softcaptemp = indRecessIncrement;
    }
    else if(newNavEnabled)
    {
        
        
        if(tooEasy)
        {
            softcaptemp = newNavIncrement;
            
        }
        else
        {
            softcaptemp = 0.35;

        }
    }
    else
    {
        softcaptemp = 0.35;
    }
    const float SOFT_CAP = softcaptemp;
    const float HARD_CAP = 1.0;
    
    // Anything inbetween these two bounds are considered in the "Dead Zone" where no change happens to nBackDelta
    const float UPPER_BOUND = 0.75; // the threshold to get a positive nBackDelta
    const float LOWER_BOUND = 0.65; // the threshold to get a negative nBackDelta
    
    double nBackDelta = 0.0;
    if (accuracy > UPPER_BOUND) {
        nBackDelta = (accuracy - UPPER_BOUND) / 0.25;
    }
    else if (accuracy < LOWER_BOUND) {
        nBackDelta = -(LOWER_BOUND - accuracy) / 0.25;
    }
    else {
        nBackDelta = 0.0;
    }
    nBackDelta *= SOFT_CAP;
    
    // In case the nBackDelta is lower than the softcap.
    if (nBackDelta < -SOFT_CAP) {
        nBackDelta = -SOFT_CAP;
    }
    
    /*// Taken out. No longer using accuracy range to calculate nBackDelta
     double accuracyRange = 0.25;
     double nBackDelta = SOFT_CAP * (accuracy - (1 - accuracyRange)) / accuracyRange;
     if ( nBackDelta < -SOFT_CAP ) nBackDelta = -SOFT_CAP;
     if ( nBackDelta > SOFT_CAP ) nBackDelta = SOFT_CAP;
     */
    
    float difficultyWeight = obtainDifficultyWeight(level, assessment, nBackDelta);
    float samplingWeight = obtainSamplingWeight(level, assessment);
    const float PERFECT_MULTIPLIER = 1;
    if ( nBackDelta < 0.0 )
    {
        if (assessment.rating >= 5 && !exclude ) // If the player completed the level, don't decrease despite accuracy
            nBackDelta = 0.0;
        
        nBackDelta *= difficultyWeight;
        nBackDelta *= samplingWeight;
    }
    else
    {
        if (assessment.rating < 5 && !exclude ) // If the player didn't complete the level, don't increase despite accuracy
            nBackDelta = 0.0;
        nBackDelta *= difficultyWeight; // apply multiplier to positive base value
        nBackDelta *= samplingWeight;
        if (accuracy >= 1.00 - Util::EPSILON && !exclude    ) // If player has perfect performance, grant a bonus to memory score
            nBackDelta *= PERFECT_MULTIPLIER;
    }
    if ( nBackDelta <= -HARD_CAP) nBackDelta = -HARD_CAP;
    if ( nBackDelta >= HARD_CAP) nBackDelta = HARD_CAP;
    return nBackDelta;
}

// Grades level and updates nBackLevel of scheduler using the accuracy formula
void Player::assessLevelPerformance(std::pair<StageRequest, PlayerProgress>* levelToGrade)
{
    // first and second parts of the explicit pair
    StageRequest level = levelToGrade->first;
    PlayerProgress assessment = levelToGrade->second;
    
    // Formula for accuracy = TP / TP + TN + FP
    double accuracy = 0.0f;
    if (assessment.numCorrect + assessment.numMissed + assessment.numWrong > 0)
        accuracy = assessment.numCorrect / (float)(assessment.numCorrect + assessment.numMissed + assessment.numWrong);
    
    float nBackDelta = modifyNBackDelta(level, assessment, accuracy, false);
    
    // If tutorial level, zero it out if they played one of those types already
    if (level.stageNo < 0)
    {
        if (scheduler->scheduleHistoryD.size() > 0 && level.phaseX == PHASE_ALL_SIGNAL)
            nBackDelta = 0.0;
        if (scheduler->scheduleHistoryE.size() > 0 && level.phaseX == PHASE_COLLECT)
            nBackDelta = 0.0;
    }
    
    float nBackChallenge = getMemoryChallenge(level, assessment);
    bool tooEasy = nBackChallenge < -0.5;
    
    double playerSkill;
    double playerOffset;
    double holdoutDelta = 0.0;
    // Find out what phase they're in and update their skill level for that phase.
    //
    // If the level has holdout, the update is affected into a holdout skill offset
    // for that phase. Additionally, if the holdout skill offset ends up being positive,
    // it is zero'd out and the difference is added into skill level.
    switch (level.phaseX) {
        case PHASE_COLLECT:
            //Added this: Do i need to update playerskill?
            if(manRecessEnabled)
            {
                manRecessCount = 0;
            }
            indRecessNBackLevel += nBackDelta;
            //std::cout<<"RECESS: NBackDelta: "<<nBackDelta<<std::endl;
            scheduler->nBackLevelE += nBackDelta;
            if (scheduler->nBackLevelE < 0.0)
            {
                scheduler->nBackLevelE = 0.0;
            }
            playerSkill = scheduler->nBackLevelE;
            playerOffset = 0.0;
            break;
        case PHASE_COLOR_SOUND:
            if (tooEasy)
                scheduler->nBackLevelE += nBackDelta;
            if (!tooEasy || nBackDelta < 0.0)
            {
                if (level.hasHoldout())
                {
                    scheduler->holdoutOffsetA += nBackDelta;     //get nbackdelta
                    if(scheduler->holdoutOffsetA>0) {
                        holdoutDelta = nBackDelta - scheduler->holdoutOffsetA;
                        nBackDelta = scheduler->holdoutOffsetA;
                        
                        scheduler->nBackLevelA += scheduler->holdoutOffsetA;
                        scheduler->holdoutOffsetA = 0;              //reset offset
                    }
                    else {
                        holdoutDelta = nBackDelta;
                        nBackDelta = 0.0;
                    }
                }
                else
                {
                    scheduler->nBackLevelA += nBackDelta;
                }
                if (scheduler->nBackLevelA < 0.0) scheduler->nBackLevelA = 0.0;
            }
            playerSkill = scheduler->nBackLevelA;
            playerOffset = scheduler->holdoutOffsetA;
            break;
        case PHASE_SHAPE_SOUND:
            if (tooEasy)
                scheduler->nBackLevelE += nBackDelta;
            if (!tooEasy || nBackDelta < 0.0)
            {
                if (level.hasHoldout())
                {
                    scheduler->holdoutOffsetB += nBackDelta;     //get nbackdelta
                    if(scheduler->holdoutOffsetB>0) {
                        holdoutDelta = nBackDelta - scheduler->holdoutOffsetB;
                        nBackDelta = scheduler->holdoutOffsetB;
                    
                        scheduler->nBackLevelB += scheduler->holdoutOffsetB;
                        scheduler->holdoutOffsetB = 0;              //reset offset
                    }
                    else {
                        holdoutDelta = nBackDelta;
                        nBackDelta = 0.0;
                    }
                }
                else
                {
                    scheduler->nBackLevelB += nBackDelta;
                }
                if (scheduler->nBackLevelB < 0.0) scheduler->nBackLevelB = 0.0;
            }
            playerSkill = scheduler->nBackLevelB;
            playerOffset = scheduler->holdoutOffsetB;
            break;
        case PHASE_SOUND_ONLY:
            if (tooEasy)
                scheduler->nBackLevelE += nBackDelta;
            if (!tooEasy || nBackDelta < 0.0)
            {
                scheduler->nBackLevelC += nBackDelta;
                if (scheduler->nBackLevelC < 0.0) scheduler->nBackLevelC = 0.0;
            }
            playerSkill = scheduler->nBackLevelC;
            playerOffset = 0.0;
            break;
        case PHASE_ALL_SIGNAL:
            if (tooEasy)
                scheduler->nBackLevelE += nBackDelta;
            if (!tooEasy || nBackDelta < 0.0)
            {
                if (nBackChallenge < -0.5)
                {
                    scheduler->nBackLevelE += nBackDelta;
                }
                else if (level.hasHoldout())
                {
                    scheduler->holdoutOffsetD += nBackDelta;     //get nbackdelta
                    if(scheduler->holdoutOffsetD>0) {
                        holdoutDelta = nBackDelta - scheduler->holdoutOffsetD;
                        nBackDelta = scheduler->holdoutOffsetD;
                    
                        scheduler->nBackLevelD += scheduler->holdoutOffsetD;
                        scheduler->holdoutOffsetD = 0;              //reset offset
                    }
                    else {
                        holdoutDelta = nBackDelta;
                        nBackDelta = 0.0;
                    }
                }
                else
                {
                    scheduler->nBackLevelD += nBackDelta;
                }
                if (scheduler->nBackLevelD < 0.0) scheduler->nBackLevelD = 0.0;
            }
            playerSkill = scheduler->nBackLevelD;       //set nbacklevel to playerskill
            playerOffset = scheduler->holdoutOffsetD;
            break;
        default:
            break;
    }
    // Update total score
    scheduler->scoreCurr += score;
    
    // Update holdout experience based on performance
    if(nBackDelta > 0 && level.hasHoldout() && scheduler->currentHoldout < 6.0)
    {
        scheduler->currentHoldout+=1.0;
        if (scheduler->currentHoldout > 6.0)
            scheduler->currentHoldout = 6.0;
    }
    
    // Record for reporting purposes
    levelToGrade->second.accuracy = accuracy;
    levelToGrade->second.nBackDelta = nBackDelta;
    levelToGrade->second.nBackReturn = holdoutDelta;
    levelToGrade->second.nBackNoffset = playerOffset;
    levelToGrade->second.nBackResult = playerSkill;
    
    std::cout << "N-Back Delta: " << nBackDelta << std::endl;
    
    scheduler->removeBin(level.phaseX, level.difficultyX, level.durationX, level.hasHoldout());

    // Continue to increment a play count for each non-recess played so that we can track
    // every 5 levels to throw in a recess that they can play.
    if (level.phaseX != PHASE_COLLECT)
    {
        scheduler->playCount++;
        manRecessCount++;
        
    }
    
    // Record in the scheduler for each task column the latest level played
    switch (level.phaseX)
    {
        case PHASE_COLOR_SOUND:
            scheduler->scheduleHistoryA.push_back(*levelRequest);
            break;
        case PHASE_SHAPE_SOUND:
            scheduler->scheduleHistoryB.push_back(*levelRequest);
            break;
        case PHASE_SOUND_ONLY:
            scheduler->scheduleHistoryC.push_back(*levelRequest);
            break;
        case PHASE_ALL_SIGNAL:
            scheduler->scheduleHistoryD.push_back(*levelRequest);
            break;
        case PHASE_COLLECT:
            scheduler->scheduleHistoryE.push_back(*levelRequest);
            break;
        default:
            break;
    }
}

Player::~Player()
{
    destroyPowerUps();
    delete tutorialMgr;
}
