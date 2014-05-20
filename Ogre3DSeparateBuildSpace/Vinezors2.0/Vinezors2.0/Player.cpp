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
: seed(0), name(""), hp(globals.startingHP), numCorrectTotal(0), numSafeTotal(0), numMissedTotal(0), numWrongTotal(0), numAvoidancesTotal(0), numCollisionsTotal(0), numCorrectBonus(0), numCorrectCombo(0), numWrongCombo(0), score(0.0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), keySpace(false), vines(), movementMode(MOVEMENT_ROTATING), showCombo(true), camDir(SOUTH), mousePos(), oldPos(), camPos(), oldRot(), oldRoll(0), camRot(), camRoll(0), desireRot(), desireRoll(0), baseSpeed(0.0), bonusSpeed(0.0), finalSpeed(0.0), initSpeed(0.0), minSpeed(0.0), maxSpeed(0.0), vineOffset(0), lookback(NULL), selectedTarget(NULL), glowSpeed(0.0), toggleBack(0), results(), actions(), sessions(), skillLevel(), totalElapsed(0), totalDistanceTraveled(0.0), animationTimer(0.0), speedTimer(0.0), badFuelPickUpTimer(0.0), boostTimer(0.0), selectTimerFlag(false), selectTimer(0.0), startMusicTimer(0.0), godMode(false), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundPods(NUM_POD_SIGNALS), triggerStartup(true), numStagesWon(0), levelRequestRow(0), levelRequestCol(0), menuRowIndex(0), levelProgress(), tutorialMgr(NULL), offsetRoll(0.0), offsetRollDest(0.0)
{
    tunnel = NULL;
    for (int i = 0; i < soundPods.size(); ++i)
        soundPods[i] = NULL;
    levelProgress = std::vector<std::vector<PlayerProgress> >(NUM_LEVELS, std::vector<PlayerProgress>(NUM_TASKS));
    initPowerUps();
    tutorialMgr = new TutorialManager();
}

Player::Player(const std::string & name, Vector3 camPos, Quaternion camRot, float camSpeed, float offset, unsigned seed, const std::string & filename)
: seed(seed), name(name), hp(globals.startingHP), numCorrectTotal(0), numSafeTotal(0), numCorrectBonus(0), numMissedTotal(0), numWrongTotal(0), numAvoidancesTotal(0), numCollisionsTotal(0), numCorrectCombo(0), numWrongCombo(0), score(0.0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), keySpace(false), vines(), movementMode(MOVEMENT_ROTATING), showCombo(true), camDir(SOUTH), mousePos(), oldPos(camPos), camPos(camPos), oldRot(camRot), oldRoll(0), camRot(camRot), camRoll(0), desireRot(camRot), desireRoll(0), baseSpeed(camSpeed), bonusSpeed(0.0), finalSpeed(camSpeed), initSpeed(0.0), minSpeed(0.0), maxSpeed(0.0), vineOffset(offset), lookback(NULL), selectedTarget(NULL), glowSpeed(0.0), toggleBack(0), results(), actions(), sessions(), skillLevel(), totalElapsed(0), totalDistanceTraveled(0.0), animationTimer(0.0), speedTimer(0.0), badFuelPickUpTimer(0.0), boostTimer(0.0), selectTimerFlag(false), selectTimer(0.0), startMusicTimer(0.0), godMode(false), soundMusic(NULL), soundFeedbackGood(NULL), soundFeedbackBad(NULL), soundPods(NUM_POD_SIGNALS), triggerStartup(true), numStagesWon(0), levelRequestRow(0), levelRequestCol(0), menuRowIndex(0), levelProgress(), tutorialMgr(NULL), offsetRoll(0.0), offsetRollDest(0.0)
{
    levels = new LevelSet();
    levels->initializeLevelSet();
    tunnel = NULL;
    for (int i = 0; i < soundPods.size(); ++i)
        soundPods[i] = NULL;
    levelProgress = std::vector<std::vector<PlayerProgress> >(NUM_LEVELS, std::vector<PlayerProgress>(NUM_TASKS));
    initPowerUps();
    tutorialMgr = new TutorialManager();
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
    float currentTotal = globals.speedMap[baseSpeed] + bonusSpeed;
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
{
    /*
     // Linear progression
    if (!levels->hasLevel(level))
        return false;
    if (level <= 0) return true;
    if (!hasLevelProgress(level - 1))
        return false;
    PlayerProgress progress = getLevelProgress(level - 1);
    return progress.rating > 0;
     */
    
    // Satisfy previous row star total and
    // previous column level must be 3 stars
    if (!levels->hasLevel(level))
        return false;
    int levelRow = levels->getLevelRow(level);
    int levelCol = levels->getLevelCol(level);
    
    int totalRatingCur = getTotalLevelRating(levelRow);
    int rowRequirementCur = levels->getTotalRowRequirement(levelRow);
    
    if (!levels->hasLevel(levelRow - 1, levelCol))
        return (levelCol != 5 || totalRatingCur >= rowRequirementCur - 3);
    if (!hasLevelProgress(levelRow - 1, levelCol))
        return false;
    int totalRatingPrev = getTotalLevelRating(levelRow - 1);
    int previousRating = getLevelProgress(levelRow - 1, levelCol).rating;
    int rowRequirementPrev = levels->getTotalRowRequirement(levelRow - 1);
    
    //std::cout << levelRow << "," << levelCol << " " << rowRequirementCur << " " << totalRatingCur << std::endl;
    if (totalRatingPrev >= rowRequirementPrev && previousRating >= 3)
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

int Player::getMenuRowIndex() const
{
    return menuRowIndex;
}

// Returns the total rating of a row-set of levels
int Player::getTotalLevelRating(int row) const
{
    if (row < 0 && row >= levelProgress.size()) return 0;
    int total = 0;
    for (int col = 0; col < levelProgress[row].size(); ++col)
        if (levelProgress[row][col].rating >= 0)
            total += levelProgress[row][col].rating;
    return total;
}

// Returns the total score of a row-set of levels
float Player::getTotalLevelScore(int row) const
{
    if (row < 0 && row >= levelProgress.size()) return 0.0;
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
    
    int nvalue = tunnel->getNBackToggle();
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
    
    if (selectedTarget && t && t->active)
    {
        SceneNode* glowNode = selectedTarget->getGlowNode();
        
        if (selectedTarget->isPodTaken() && glowNode)
        {
            Vector3 vinePos = vines[0]->entireVine->_getDerivedPosition();
            Vector3 podPos = selectedTarget->getHead()->_getDerivedPosition();
            
            Vector3 rotator = (podPos - vinePos).normalisedCopy();
            
            Quaternion q = getCamForward(false).getRotationTo(rotator);
            
            float distance = (podPos - vinePos).length();
            t->tractorBeamEffect->getEmitter(0)->setParameter("depth", Util::toStringFloat(distance-7.0f));
            t->tractorBeamNode->setPosition(0,0,-distance/2.0-7.0f);
            
            t->tractorBeamRotatorNode->setPosition(vines[0]->entireVine->_getDerivedPosition());
            t->tractorBeamRotatorNode->setOrientation(getCamRot() * q);
            
            t->tractorBeamNode->roll(Radian(Degree(15.0f)));
        }
    }
    else if( t && t->active && t->tractorBeamEffect ) {
        t->tractorBeamNode->detachObject(t->tractorBeamEffect);
        t->tractorBeamNode->getCreator()->destroyParticleSystem(t->tractorBeamEffect);
        t->tractorBeamEffect = NULL;
        t->tractorBeamRotatorNode->removeAndDestroyChild("BeamNode");
        t->tractorBeamNode = NULL;
        OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->removeAndDestroyChild("BeamRotatorNode");
        t->tractorBeamRotatorNode = NULL;
        t->active = false;
    }
}

void Player::performTractorBeam()
{
    TractorBeam* t = dynamic_cast<TractorBeam*>(powerups["TractorBeam"]);
    
    if (!selectedTarget && t && !t->active)
    {
        selectedTarget = tunnel->getNearestPod(globals.tunnelSegmentsPerPod);
        if (selectedTarget && !selectedTarget->getGlowNode() && !selectedTarget->isPodTaken())
        {
            selectedTarget->takePod();
            setGlowGrabParameters();
            //if (selectTimer <= 0.0)
            //    selectTimer = 0.1;//1.0 / (finalSpeed / globals.initCamSpeed);
            
            t->active = true;
            t->available = false;
            t->tractorBeamRotatorNode = OgreFramework::getSingletonPtr()->m_pSceneMgrMain->getRootSceneNode()->createChildSceneNode("BeamRotatorNode");
            t->tractorBeamNode = t->tractorBeamRotatorNode->createChildSceneNode("BeamNode");
            t->tractorBeamEffect = t->tractorBeamNode->getCreator()->createParticleSystem("Beam", "General/Beam");
            t->tractorBeamEffect->getEmitter(0)->setParameter("depth", "0");
            t->tractorBeamNode->attachObject(t->tractorBeamEffect);
            
        } else
            selectedTarget = NULL;
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
                    
                    t->timeBonusNode = vines[0]->entireVine->createChildSceneNode("timeBonusNode");
                    t->timeBonusNode->translate(Vector3(0.0,5.0f,5.0f));
                    
                    float timeBonusWidth = 8.0f;
                    float timeBonusHeight = 4.0f;
                    Ogre::Plane timeBonusPlane;
                    timeBonusPlane.normal = Ogre::Vector3::UNIT_Z;
                    timeBonusPlane.d = 0;
                    Ogre::MeshPtr timeBonusPlaneMeshPtr = Ogre::MeshManager::getSingleton().createPlane("timeBonusPlane",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, timeBonusPlane, timeBonusWidth, timeBonusHeight, 10, 10, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
                    
                    t->timeBonusEntity = t->timeBonusNode->getCreator()->createEntity("timeBonusEntity", timeBonusPlaneMeshPtr);
                    t->timeBonusNode->attachObject(t->timeBonusEntity);
                    
                    t->timeBonusEntity->getSubEntity(0)->setMaterialName("General/TimeBonusMaterial");
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
                if (!tunnel->isDone()) tunnel->addToTimePenalty(-t->timeBonus);
                
                t->zoomIn = 0;
                t->active = false;
                
                t->timeBonusNode->detachObject(t->timeBonusEntity);
                t->timeBonusNode->getCreator()->destroyEntity(t->timeBonusEntity);
                t->timeBonusEntity = NULL;
                vines[0]->entireVine->removeAndDestroyChild("timeBonusNode");
                t->timeBonusNode = NULL;
                
                tutorialMgr->setSlides(TutorialManager::TUTORIAL_SLIDES_TIME_WARP);
            }
            else {
                t->mainTimer += elapsed;
                t->timeBonusNode->translate(Vector3(t->xFlySpeed,t->yFlySpeed,0.0f));
                t->timeBonusNode->scale(Vector3(t->scaleFactor,t->scaleFactor,0.0f));
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
        Camera* cam = OgreFramework::getSingletonPtr()->m_pCameraMain;
        
        t->origFov = Degree(cam->getFOVy());
        t->origAngle = t->origFov.valueDegrees();
        t->currentAngle = t->origFov.valueDegrees();
        
        t->origOffset = vineOffset;
        t->currentOffset = t->origOffset;
        
        t->mainTimer = 0.0f;
        t->active = true;
        t->available = false;
    }
}

void Player::updateBoost(float elapsed)
{
    float timeRange = 0.5;
    if (boostTimer > 0.0)
    {
        boostTimer -= elapsed;
        if (boostTimer <= 0.0)
            boostTimer = 0.0;
        else if (boostTimer <= timeRange)
        {
            soundBoost->stop();
            vines[0]->removeBoost();
        }
    }
}

void Player::performBoost()
{
    float timeRange = 0.5; // Range to linearly decrease back to normal speed
    if (boostTimer <= timeRange)
    {
        boostTimer = 2.0;
        vines[0]->setBoost();
        if (soundBoost)
        {
            soundBoost->stop();
            soundBoost->play();
        }
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

void Player::setGlowGrabParameters()
{
    // Extract a glow that is in the form of a sphere always, not shape of pod.
    selectedTarget->removeGlow();
    selectedTarget->generateGlow(selectedTarget->getPodInfo().podColor, POD_SHAPE_SPHERE);
    
    // Assign a glow speed towards the player
    if (selectedTarget->getGlowNode())
    {
        glowSpeed = 2 * (vines[0]->getEntireVine()->_getDerivedPosition() - selectedTarget->getGlowNode()->_getDerivedPosition()).length();
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
    
    bool goodPod = tunnel->getPodIsGood();

    // Determine whether the player got it right or not
    if (goodPod && test->isPodTaken()) {
        if (tunnel->getMode() == STAGE_MODE_TEACHING || tunnel->getMode() == STAGE_MODE_RECESS)
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

        if (tunnel->satisfyCriteria(tunnel->getNBackToggle()) || tunnel->getMode() == STAGE_MODE_RECESS)
        {
            baseSpeed += globals.stepsizeSpeedUp;
            baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
        }
        
        if (getToggleBack() == 0)
        {
            if (hp >= 0) hp += globals.HPPositiveCorrectAnswer;
            else hp += globals.HPNegativeCorrectAnswer;
            hp = Util::clamp(hp, globals.HPNegativeLimit, globals.HPPositiveLimit);
        }
        ++numCorrectCombo;
        numWrongCombo = 0;
        
        if (tunnel->getMode() == STAGE_MODE_PROFICIENCY &&
            numCorrectCombo % globals.numToSpeedUp == 0)
        {
            baseSpeed += globals.stepsizeSpeedUp;
            baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
        }
        
        // Determine Score
        score += getScoring();
    }
    else if (!goodPod && test->isPodTaken())
    {
        if (soundFeedbackBad)
        {
            soundFeedbackBad->stop();
            soundFeedbackBad->play();
        }
        ++numWrongTotal;
        
        //tunnel->loseRandomCriteria();
        //beginBadFuelPickUp();
        
        if (hp >= 0) hp += globals.HPPositiveWrongAnswer;
        else hp += globals.HPNegativeWrongAnswer;
        hp = Util::clamp(hp, globals.HPNegativeLimit, globals.HPPositiveLimit);
        
        tunnel->addToTimePenalty(globals.wrongAnswerTimePenalty);
        
        numCorrectCombo = 0;
        ++numWrongCombo;
        if (numWrongCombo % globals.numToSpeedDown == 0)
        {
            baseSpeed += globals.stepsizeSpeedDown;
            baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
        }
        numCorrectBonus = 0;
        
        //score -= std::pow(5.0, 4);
        //if (score < 0.0) score = 0.0;
    }
    else if (!goodPod && !test->isPodTaken())
    {
        numSafeTotal++;
        numCorrectBonus++;
    }
    else if (goodPod && !test->isPodTaken()) // Missed good
    {
        ++numMissedTotal;
    }
    // Check for combo mode
    //if (tunnel->getMode() == STAGE_MODE_PROFICIENCY) determineSpawnCombo();
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
            std::vector<Pod*> pods = targetSlice->getPods();
            PodInfo targetinfo;
            SectionInfo sliceInfo = targetSlice->getSectionInfo();
            targetinfo.podExists = false;
            char segmentEncoding[NUM_DIRECTIONS];
            // Set segment information
            for (int i = 0;i < NUM_DIRECTIONS; ++i)
                segmentEncoding[i] = sliceInfo.sidesUsed[i] ? ENC_YES_PANEL : ENC_NO_PANEL;
            char playerEnc = ENC_PLAYER_STATIC;
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
                        if (selectTimer <= 0.0) // Make sure it's not being grabbed already
                            setGlowGrabParameters();
                        if (selectTimer <= 0.0 || selectTimer > 0.1)
                            selectTimer = 0.1;
                    }
                    
                    segmentEncoding[test->getLoc()] = test->isPodGood() ? ENC_TARGET : ENC_FAKE;
                    if (test->isPodGood() && test->isPodTaken())
                        playerEnc = ENC_PLAYER_CORRECT;
                    else if ((test->isPodGood() && !test->isPodTaken()) ||
                             (!test->isPodGood() && test->isPodTaken()))
                        playerEnc = ENC_PLAYER_WRONG;
                }
                // Set obstacle info, assuming the first index can sometimes be signal
                for (; ind < pods.size(); ++ind)
                {
                    bool avoided = true;
                    if (pods[ind]->getPodTrigger())
                    {
                        if (pods[ind]->isPodTaken())
                        {
                            avoided = false;
                            segmentEncoding[pods[ind]->getLoc()] = ENC_OBSTACLE_YES_HIT;
                        }
                        else
                            segmentEncoding[pods[ind]->getLoc()] = ENC_OBSTACLE_NO_HIT;
                    }
                    if (avoided) numAvoidancesTotal++;
                    else numCollisionsTotal++;
                }
            }
            segmentEncoding[vines[0]->transition < 0.50 ? vines[0]->loc : vines[0]->dest] = playerEnc;
            
            // This code block is to record data of the pods
            Result result;
            memcpy(result.segmentEncoding, segmentEncoding, NUM_DIRECTIONS);
            result.eventID = tunnel->getStageNo();
            result.taskType = tunnel->getPhase() - 'A';
            result.nback = tunnel->getNBack();
            result.navigation = tunnel->getCurrentNavLevel();
            result.playerLoc = vines[0]->transition < 0.50 ? vines[0]->loc : vines[0]->dest;
            result.podInfo = targetinfo;
            result.sectionInfo = sliceInfo;
            result.timestamp = (int)(OgreFramework::getSingletonPtr()->totalElapsed * 1000);
            result.minSpeed = minSpeed;
            result.maxSpeed = maxSpeed;
            result.baseSpeed = baseSpeed;
            result.finalSpeed = finalSpeed;
            result.navScore = skillLevel.navigation;
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

void Player::setToggleBack(int value)
{
    toggleBack = value;
}

void Player::setGodMode(bool value)
{
    godMode = value;
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
    int minNBack = tunnel->getLowestCriteria();
    int toggle = 0;
    if (minNBack <= 0) // 0-backs always the last toggle
        toggle = 3;
    else
        toggle = tunnel->getNBack() - minNBack;
    
    setToggleBack(toggle);
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
    sess.sessionNo = skillLevel.sessionID;
    sess.eventID = tunnel->getStageNo();
    sess.taskType = tunnel->getPhase() - 'A';
    sess.stageTime = tunnel->getStageTime();
    sess.timestampIn = (int)(OgreFramework::getSingletonPtr()->totalElapsed * 1000);
    sess.timestampOut = -1;
    if (tunnel->getMode() == STAGE_MODE_RECESS || tunnel->getMode() == STAGE_MODE_TEACHING)
        sess.nback = 0;
    else
        sess.nback = tunnel->getNBack();
    if (tunnel->getPhase() == 'A')
        sess.rep = skillLevel.set1Rep;
    else if (tunnel->getPhase() == 'B')
        sess.rep = skillLevel.set2Rep;
    else if (tunnel->getPhase() == 'C')
        sess.rep = skillLevel.set3Rep;
    else
        sess.rep = -1;
    sess.runSpeedIn = baseSpeed;
    sess.runSpeedOut = -1;
    sess.maxSpeed = maxSpeed;
    sess.navScore = skillLevel.navigation;
    sess.TP = -1;
    sess.FP = -1;
    sess.TN = -1;
    sess.FN = -1;
    sess.obsHit = -1;
    sess.obsAvoided = -1;
    sessions.push_back(sess);
    
    OgreFramework::getSingletonPtr()->m_pSoundMgr->resumeAllPausedSounds();
    OgreFramework::getSingletonPtr()->m_pSoundMgr->stopAllSounds();
    
    OgreOggSound::OgreOggISound* soundMusicTemp = NULL;
    if (nameMusic != "")
        soundMusicTemp = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound(nameMusic);
    if (soundMusic != soundMusicTemp)
    {
        if (soundMusic) soundMusic->stop();
        soundMusic = soundMusicTemp;
        startMusicTimer = 2.0;
    }

    if (soundMusic) soundMusic->setVolume(globals.volumeMusic);
    if (soundPods[POD_SIGNAL_1]) soundPods[POD_SIGNAL_1]->setVolume(globals.volumeSignal1);
    if (soundPods[POD_SIGNAL_2]) soundPods[POD_SIGNAL_2]->setVolume(globals.volumeSignal2);
    if (soundPods[POD_SIGNAL_3]) soundPods[POD_SIGNAL_3]->setVolume(globals.volumeSignal3);
    if (soundPods[POD_SIGNAL_4]) soundPods[POD_SIGNAL_4]->setVolume(globals.volumeSignal4);
    if (soundFeedbackGreat) soundFeedbackGreat->setVolume(globals.volumeFeedbackGood);
    if (soundFeedbackGood) soundFeedbackGood->setVolume(globals.volumeFeedbackGood);
    if (soundFeedbackBad) soundFeedbackBad->setVolume(globals.volumeFeedbackBad);
    if (soundCollision) soundCollision->setVolume(globals.volumeFeedbackCollision);
    if (soundBoost) soundBoost->setVolume(globals.volumeBoost);
    if (soundStartup) soundStartup->setVolume(globals.volumeStartup);
    
    // initalize player ship location
    tunnel->setOffsetIterators(camPos, vineOffset);
    vines[0]->setVisible(true);
    vines[0]->reloadIfNecessary(globals.setVineShip);
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
    OgreOggSound::OgreOggISound* soundMusicTemp = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("MusicMenu");
    if (soundMusic != soundMusicTemp)
    {
        OgreFramework::getSingletonPtr()->m_pSoundMgr->resumeAllPausedSounds();
        OgreFramework::getSingletonPtr()->m_pSoundMgr->stopAllSounds();
        
        soundMusic = soundMusicTemp;
        if (soundMusic) soundMusic->play();
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
        /*
        // Stop any other sounds so that the player gets to hear the next pod signal sound clearly
        // ---------------------------------------------------------------------------------------
        if (soundFeedbackGreat) soundFeedbackGreat->stop();
        if (soundFeedbackGood) soundFeedbackGood->stop();
        if (soundFeedbackBad) soundFeedbackBad->stop();
        if (soundCollision) soundCollision->stop();
        for (int i = 0; i < soundPods.size(); ++i)
            if (soundPods[i]) soundPods[i]->stop();
        // ---------------------------------------------------------------------------------------
         */
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
    if (soundFeedbackGreat)
        soundFeedbackGreat->pause();
    if (soundFeedbackGood)
        soundFeedbackGood->pause();
    if (soundFeedbackBad)
        soundFeedbackBad->pause();
    if (soundPods[POD_SIGNAL_1])
        soundPods[POD_SIGNAL_1]->pause();
    if (soundPods[POD_SIGNAL_2])
        soundPods[POD_SIGNAL_2]->pause();
    if (soundPods[POD_SIGNAL_3])
        soundPods[POD_SIGNAL_3]->pause();
    if (soundPods[POD_SIGNAL_4])
        soundPods[POD_SIGNAL_4]->pause();
    if (soundPods[POD_SIGNAL_HOLDOUT])
        soundPods[POD_SIGNAL_HOLDOUT]->pause();
    if (soundPods[POD_SIGNAL_UNKNOWN])
        soundPods[POD_SIGNAL_UNKNOWN]->pause();
    if (soundCollision)
        soundCollision->pause();
    if (soundStartup)
        soundStartup->pause();
    if (soundBoost)
        soundBoost->pause();
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
        soundPods[POD_SIGNAL_HOLDOUT] = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("HoldoutPod");
        soundPods[POD_SIGNAL_UNKNOWN] = NULL;
        soundCollision = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundCollision");
        soundStartup = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundStartup");
        soundBoost = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundBoost");
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
        soundBoost = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("SoundBoost");
    }
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
                            if (soundCollision)
                            {
                                soundCollision->stop();
                                soundCollision->play();
                            }
                        
                            if (!triggerShields())
                            {
                                baseSpeed -= globals.distractorSpeedPenalty;
                                baseSpeed = Util::clamp(baseSpeed, minSpeed, maxSpeed);
                                speedTimer = 5.0;
                                tunnel->addToTimePenalty(globals.distractorTimePenalty);
                                if (hp >= 0)
                                    hp += globals.HPPositiveDistractor;
                                else
                                    hp += globals.HPNegativeDistractor;
                                hp = Util::clamp(hp, globals.HPNegativeLimit, globals.HPPositiveLimit);
                            
                                beginBadFuelPickUp();
                            }
                            numCorrectCombo = 0;
                        }
                        else if (hits[i]->getMeshType() == POD_POWERUP)
                        {
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
    act.eventID = tunnel ? tunnel->getStageNo() : -1;
    act.action = actType;
    act.timestamp = static_cast<int>(OgreFramework::getSingletonPtr()->totalElapsed * 1000);
    act.baseSpeed = baseSpeed;
    act.finalSpeed = finalSpeed;
    actions.push_back(act);
}

float flyOutCounter = 0.0f;
float flyOutDuration = 0.25f;
float endAnimationSuccessDuration = 3.0f;
float endAnimationFailDuration = 1.5f;
bool endAnimationBegin = false;

float flyOutSpeed = 0.0f;
float flyOutIncr = 0.01f;
bool flyOutSpeedUp = true;

float flyOutCamSpeed = 0.0f;
float flyOutAngleY = 0.0f;
float flyOutAngleX = 0.0f;

bool flyLeft = true;

bool soundStart = false;

void Player::update(float elapsed)
{
    totalElapsed += elapsed;
    
    // Play music at beginning of stage
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
    
    
    
    
    //*******//
    if( tunnel->getEval() == PASS && tunnel->getFlyOut() ) {
        if( endAnimationBegin ) {
            if( flyOutCounter >= endAnimationSuccessDuration ) {
                flyOutCounter = 0.0f;
                endAnimationBegin = false;
                flyOutAngleX = 0.0f;
                flyOutAngleY = 0.0f;
                flyLeft = true;
                tunnel->setCleaning(true);
                
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
                
                move(getCamForward() * flyOutCamSpeed);
                Vector3 moveOffset = getCamForward() * globals.globalModifierCamSpeed*finalSpeed*elapsed;
                
                // rotate about y
                moveOffset = Vector3(Math::Cos(Degree(flyOutAngleY))*moveOffset.x+Math::Sin(Degree(flyOutAngleY))*moveOffset.z,moveOffset.y,-Math::Sin(Degree(flyOutAngleY))*moveOffset.x+Math::Cos(Degree(flyOutAngleY))*moveOffset.z);
                
                // rotate about x
                moveOffset = Vector3(moveOffset.x,Math::Cos(Degree(flyOutAngleX))*moveOffset.y-Math::Sin(Degree(flyOutAngleX))*moveOffset.z,Math::Sin(Degree(flyOutAngleX))*moveOffset.y+Math::Cos(Degree(flyOutAngleX))*moveOffset.z);
            
                
                vines[0]->move(moveOffset);
                
                
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
            move(getCamForward() * flyOutCamSpeed);
            Vector3 moveOffset = getCamForward() * globals.globalModifierCamSpeed*finalSpeed*elapsed;
            vines[0]->move(moveOffset);
        }
        return;
    }
    else if( tunnel->getEval() == FAIL && tunnel->getFlyOut() ) {
        if( !soundStart ) {
            OgreOggISound* sound = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("LevelFail");
            sound->play();
            soundStart = true;
        }
        
        if( flyOutCounter >= endAnimationFailDuration ) {
            flyOutCounter = 0.0f;
            flyOutSpeedUp = true;
            flyOutSpeed = 0.0f;
            soundStart = false;
            tunnel->setCleaning(true);
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
        return;
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
    
    if (!tunnel->isDone())
    {
        // Check for collisions for player and the tunnel
        checkCollisions();
        
        // Record the segment info player has passed
        recordInfo();
    }
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
    score += (static_cast<int>(tunnel->getTimeLeft()) * SCORE_PER_SECOND);
    
    // Assign the correct rating based on tunnel results
    int nrating = -1;
    if (eval == PASS)
    {
        nrating = 3;
        incrementNumStagesWon();
    }
    else
    {
        // Recess is based off of distance (number of signals left to pass)
        // Every other is a collection task which we will use the default 3, 5, 8
        if (tunnel->getMode() == STAGE_MODE_RECESS)
        {
            float percentComplete = tunnel->getPercentComplete();
            if (percentComplete >= 0.9)
                nrating = 2;
            else if (percentComplete >= 0.7)
                nrating = 1;
            else
                nrating = 0;
        }
        else
        {
            int collected = tunnel->getNumSatisfiedCriteria();
            if (collected >= 5)
                nrating = 2;
            else if (collected >= 3)
                nrating = 1;
            else
                nrating = 0;
        }
    }
    PlayerProgress* levelResult = &(levelProgress[levelRequestRow][levelRequestCol]);
    // If level has never been done before or we have a new high score, then save stats
    if (levelResult->rating < 0 || score > levelResult->score)
    {
        levelResult->score = score;
        levelResult->time = tunnel->getTimeLeft();
        levelResult->numCorrect = numCorrectTotal;
        levelResult->numWrong = numWrongTotal;
        levelResult->numSafe = numSafeTotal;
        levelResult->numMissed = numMissedTotal;
        levelResult->startSpeed = initSpeed;
        levelResult->exitSpeed = baseSpeed;
        // Assign other level progress info here since it is a new score
    }
    // Update other level results/settings
    levelResult->initSpeedSetting = initSpeed;
    levelResult->setRating(nrating); // Assign rating last
    
    setSkillLevel(skillLevel);
    saveStage(globals.logPath);
    saveActions(globals.actionPath);
    saveSession(globals.sessionPath);
    saveProgress(globals.savePath);
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
            out << "% debug seed: " << seed << endl;
            out << "%" << endl;
            out << "% SegEncNW { '0'=no panel, '1'=panel, '4'=obstacle, '5'=collided obstacle, '6'=player, '7'=player right, '8'=player wrong, '2'=target, '3'=fake }" << endl;
            out << "% SegEncN" << endl;
            out << "% SegEncNE" << endl;
            out << "% SegEncE" << endl;
            out << "% SegEncSE" << endl;
            out << "% SegEncS" << endl;
            out << "% SegEncSW" << endl;
            out << "% SegEncW" << endl;
            out << "% Event Number { 0, inf }" << endl;
            out << "% Task Type { 0=Color/Sound, 1=Shape/Sound, 2=Sound, 3=Navigation, 4=Speed, 5=Training 6=Recess, 7=Special 2-Back }" << endl;
            out << "% N-Back { 0, inf }" << endl;
            out << "% Navigation Level { 0, inf }" << endl;
            out << "% Player Loc { 0=Northwest ... 7=West }" << endl;
            out << "% Pod Loc { -1=N/A, 0=Northwest ... 7=West }" << endl;
            out << "% Pod Color { -1, inf }" << endl;
            out << "% Pod Shape { -1, inf }" << endl;
            out << "% Pod Sound { -1, inf }" << endl;
            out << "% Pod Match { -1=N/A, 0=No, 1=Yes }" << endl;
            out << "% Pod Taken { -1=N/A, 0=No, 1=Yes }" << endl;
            out << "% Timestamp (ms)" << endl;
            out << "% Num Obstacles { 0, inf }" << endl;
            out << "% Min Speed { 0, inf }" << endl;
            out << "% Max Speed { 0, inf }" << endl;
            out << "% Base Speed { 0, inf }" << endl;
            out << "% Final Speed { 0, inf }" << endl;
            out << "% Navigation Score { 0, inf }" << endl;
            out << "% Segment Direction { 0=Northwest ... 7=West }" << endl;
            out << "% Segment Angle { 0, inf }" << endl;
            out << "% Segment Panels { 0, inf }" << endl;
            out << "%" << endl;
            out << "% SegEncNW SecEncN SegEncNE SegEncE SegEncSE SegEncS SegEncSW SegEncW EventNumber TaskType N-Back Navigation PlayerLoc PodLoc PodColor PodShape PodSound PodMatch PodTaken Timestamp NumObs MinSpeed MaxSpeed BaseSpeed FinalSpeed NavScore SegmentDir SegmentAngle SegmentPanels" << endl;
        }
        
        for (std::list<Result>::iterator it = results.begin(); it != results.end(); ++it) {
            //out << SOUTH << " "
            
            int nobs = 0;
            for (int i = 0; i < NUM_DIRECTIONS; ++i)
            {
                if (it->segmentEncoding[i] == ENC_OBSTACLE_NO_HIT || it->segmentEncoding[i] == ENC_OBSTACLE_YES_HIT) nobs++;
                out << it->segmentEncoding[i] << " ";
            }
            out << it->eventID << " "
            << it->taskType << " "
            << it->nback << " "
            << it->navigation << " "
            << it->playerLoc << " ";
            if (it->podInfo.podExists)
            {
                out << it->podInfo.podLoc << " "
                << it->podInfo.podColor << " "
                << it->podInfo.podShape << " "
                << it->podInfo.podSound << " "
                << it->podInfo.goodPod << " "
                << it->podInfo.podTaken << " ";
            }
            else
                out << "-1 -1 -1 -1 -1 -1 ";
            out << nobs << " ";
            out << it->timestamp << " ";
            out << it->minSpeed << " "
            << it->maxSpeed << " "
            << it->baseSpeed << " "
            << it->finalSpeed << " "
            << it->navScore << " "
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
            out << "% debug seed: " << seed << endl;
            out << "%" << endl;
            out << "% Event Number { 0, inf }" << endl;
            out << "% Action Type { 0=None, 1=SingleTap, 2=DoubleTap, 3=HoldTap, 4=SwipeLeft, 5=SwipeRight, 6=Pinch }" << endl;
            out << "% Timestamp (ms)" << endl;
            out << "% Base Speed { 0, inf }" << endl;
            out << "% Final Speed { 0, inf }" << endl;
            out << "%" << endl;
            out << "% EventNumber ActionType Timestamp BaseSpeed FinalSpeed" << endl;
        }
        
        for (std::list<Action>::iterator it = actions.begin(); it != actions.end(); ++it) {
            //out << SOUTH << " "
            
            out << it->eventID << " "
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
    sessions.back().TP = numCorrectTotal;
    sessions.back().FP = numWrongTotal;
    sessions.back().TN = numMissedTotal;
    sessions.back().FN = numSafeTotal;
    sessions.back().obsHit = numCollisionsTotal;
    sessions.back().obsAvoided = numAvoidancesTotal;
    
    if (out.good()) {
        if (newFile) {
            out << "% Session Log: " << endl;
            out << "% debug seed: " << seed << endl;
            out << "%" << endl;
            out << "% Session Number { 0, inf }" << endl;
            out << "% Event Number { 0, inf }" << endl;
            out << "% Task Type { 0=Color/Sound, 1=Shape/Sound, 2=Sound, 3=Navigation, 4=Speed, 5=Training 6=Recess, 7=Special 2-Back }" << endl;
            out << "% Intended Stage Duration (s)" << endl;
            out << "% TSin - Timestamp In (ms)" << endl;
            out << "% TSout - Timestamp Out (ms)" << endl;
            out << "% N-Back { 0, inf }" << endl;
            out << "% Rep { -1, inf }" << endl;
            out << "% RunSpeedIn { 0, inf }" << endl;
            out << "% RunSpeedOut { 0, inf }" << endl;
            out << "% MaxSpeed { 0, inf }" << endl;
            out << "% NavScore { 0, inf }" << endl;
            out << "% TP - Total Picked and Match { 0, inf }" << endl;
            out << "% FP - Total Picked and Non-Match { 0, inf }" << endl;
            out << "% TN - Total Missed and Match { 0, inf }" << endl;
            out << "% FN - Total Missed and Non-Match { 0, inf }" << endl;
            out << "% ObsHit - Segments with Obstacles Hit { 0, inf }" << endl;
            out << "% ObsAvoid - Segments with Obstacles Avoided { 0, inf }" << endl;
            out << "%" << endl;
            out << "% SessionNumber EventNumber TaskType Duration TSin TSout N-Back Rep RunSpeedIn RunSpeedOut MaxSpeed NavScore TP FP TN FN ObsHit ObsAvoid " << endl;
        }
        
        out << sessions.back().sessionNo << " "
        << sessions.back().eventID << " "
        << sessions.back().taskType << " "
        << sessions.back().stageTime << " "
        << sessions.back().timestampIn << " "
        << sessions.back().timestampOut << " "
        << sessions.back().nback << " "
        << sessions.back().rep << " "
        << sessions.back().runSpeedIn << " "
        << sessions.back().runSpeedOut << " "
        << sessions.back().maxSpeed << " "
        << sessions.back().navScore << " "
        << sessions.back().TP << " "
        << sessions.back().FP << " "
        << sessions.back().TN << " "
        << sessions.back().FN << " "
        << sessions.back().obsHit << " "
        << sessions.back().obsAvoided << endl;
        
        out.close();
    }
    else {
        out.close();
        return false;
    }
    
    return true;
}

/*
// Save based on adaptive player skill level from study
bool Player::saveProgress(std::string file, bool updateSessionID)
{
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    bool ret = false;
    
    if (updateSessionID)
        skillLevel.sessionID++;
    
    out << skillLevel.sessionID << std::endl;
    out << skillLevel.set1 << std::endl;
    out << skillLevel.set2 << std::endl;
    out << skillLevel.set3 << std::endl;
    out << skillLevel.set1Rep << std::endl;
    out << skillLevel.set2Rep << std::endl;
    out << skillLevel.set3Rep << std::endl;
    out << skillLevel.set1Notify << std::endl;
    out << skillLevel.set2Notify << std::endl;
    out << skillLevel.set3Notify << std::endl;
    out << skillLevel.navigation << std::endl;
    out << skillLevel.minSpeed << std::endl;
    out << skillLevel.averageSpeed << std::endl;
    out << skillLevel.maxSpeed << std::endl;
    out << skillLevel.runSpeed1 << std::endl;
    out << skillLevel.runSpeed2 << std::endl;
    out << skillLevel.runSpeed3 << std::endl;
    std::cout << "Writing Stage ID: " << file << std::endl;
    ret = out.good();
    
    out.close();
    return ret;
}
 
// Load based on adaptive player skill level from study
bool Player::loadProgress(std::string savePath)
{
    std::ifstream saveFile (savePath.c_str());
    bool ret = false;
    
    if (saveFile.good()) {
        saveFile >> skillLevel.sessionID;
        saveFile >> skillLevel.set1;
        saveFile >> skillLevel.set2;
        saveFile >> skillLevel.set3;
        saveFile >> skillLevel.set1Rep;
        saveFile >> skillLevel.set2Rep;
        saveFile >> skillLevel.set3Rep;
        saveFile >> skillLevel.set1Notify;
        saveFile >> skillLevel.set2Notify;
        saveFile >> skillLevel.set3Notify;
        saveFile >> skillLevel.navigation;
        saveFile >> skillLevel.minSpeed;
        saveFile >> skillLevel.averageSpeed;
        saveFile >> skillLevel.maxSpeed;
        saveFile >> skillLevel.runSpeed1;
        saveFile >> skillLevel.runSpeed2;
        saveFile >> skillLevel.runSpeed3;
        
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
*/

// Save based on player results in level progression
bool Player::saveProgress(std::string file)
{
    std::ofstream out;
    out.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    bool ret = true;
    
    out << levelProgress.size() << std::endl;
    for (int i = 0; i < levelProgress.size(); ++i)
    {
        for (int j = 0; j < levelProgress[i].size(); ++j)
            out << levelProgress[i][j] << std::endl;
    }
    
    out << (*tutorialMgr) << std::endl;
    
    std::cout << "Save Level Progress: " << file << std::endl;
    ret = out.good();
    
    out.close();
    return ret;
}

// Load based on player results in level progression
bool Player::loadProgress(std::string savePath)
{
    std::ifstream saveFile (savePath.c_str());
    bool ret = false;
    
    if (saveFile.good()) {
        int size;
        saveFile >> size;
        
        levelProgress = std::vector< std::vector<PlayerProgress> >(size);
        for (int i = 0; i < levelProgress.size(); ++i)
        {
            levelProgress[i] = std::vector<PlayerProgress>(NUM_TASKS);
            for (int j = 0; j < levelProgress[i].size(); ++j)
            {
                std::cout << "Level: " << i << "," << j << std::endl;
                saveFile >> levelProgress[i][j];
                std::cout << levelProgress[i][j] << std::endl;
            }
        }
        
        saveFile >> (*tutorialMgr);
        
        globals.setMessage("Loaded Save " + globals.playerName + "\nSwipe to Continue", MESSAGE_NORMAL);
        ret = true;
    } else {
        globals.setMessage("New Save " + globals.playerName + "\nSwipe to Continue", MESSAGE_NORMAL);
        ret = false;
    }
    
    tutorialMgr->setSlides(TutorialManager::TUTORIAL_SLIDES_WELCOME);
    
    saveFile.close();
    return ret;
}

Player::~Player()
{
    destroyPowerUps();
    delete tutorialMgr;
}
