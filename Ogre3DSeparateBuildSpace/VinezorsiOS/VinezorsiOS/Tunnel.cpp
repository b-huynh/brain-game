//
//  Tunnel.cpp
//  Testing
//
//  Created by Calvin Phung on 9/10/13.
//
//

#include "Tunnel.h"
#include "Player.h"
#include <cstdlib>

using namespace std;

extern Util::ConfigGlobal globals;

const float infinityDepth = 1024;

static int tunnelID = 0;

Tunnel::Tunnel()
    : player(NULL), parentNode(NULL), mainTunnelNode(NULL), start(), end(), segments(), tLeftPrevious(0.0), tLeftCurrent(0.0), previous(), current(), tLeftOffsetPrevious(0.0), tLeftOffsetCurrent(0.0), previousOffset(), currentOffset(), segmentCounter(0), segmentWidth(0.0), segmentDepth(0.0), sections(), types(), targets(), sectionSize(0), podSegmentSize(0), distractorSegmentSize(0), powerupSegmentSize(0), spawnIndex(0), spawnCombo(0), podIndex(0), sectionIndex(0), renewalSectionCounter(0), renewalPodCounter(0), renewalDistractorCounter(0), renewalPowerupCounter(0), spawnLimit(-1), numTargets(0), activePods(), playNo(0), stageNo(0), mode(STAGE_MODE_PROFICIENCY), phase(' '), stageTime(0.0), totalElapsed(0.0), timePenalty(0.0), nback(1), control(0), basis(NO_DIRECTION), sidesUsed(), materialNames(), eval(EVEN), signalTypes(), navPhase(0), catchupPhase(0), tunnelSectionsPerNavigationUpgrade(10), navCheckpoint(0), navLevels(), propagateCounters(), guide(NO_DIRECTION), collectionCriteria(), powerups(), hasHoldout(false), holdoutCounter(100), holdoutPod(0), holdoutIndex(0), holdoutFrequency(4), trackNBackA(0), trackNBackB(0), trackNBackC(0), done(false), cleanup(false), gateSlice(NULL), tVal(0.0f), gateOpen(false), activateGreen(false), tSpeedOpen(10.0f), tSpeed(0.0f), tAccel(2.0f), gateDelayTimer(0.0f), gateDelay(0.5f), flyOut(false)
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = true;
}

Tunnel::Tunnel(Ogre::SceneNode* parentNode, Vector3 start, Quaternion rot, float segmentWidth, float segmentDepth, int segmentMinAngleTurn, int segmentMaxAngleTurn, int playNo, int stageNo, StageMode mode, char phase, int nback, float stageTime, Direction sloc, int sectionSize, int podSegmentSize, int distractorSegmentSize, int powerupSegmentSize, const std::vector<std::vector<PodInfo> > & signalTypes, const std::vector<PowerupType> & powerups)
: player(NULL), parentNode(parentNode), mainTunnelNode(NULL), start(start), end(start), segments(), tLeftPrevious(0.0), tLeftCurrent(0.0), previous(), current(), tLeftOffsetPrevious(0.0), tLeftOffsetCurrent(0.0), previousOffset(), currentOffset(), segmentCounter(0), segmentWidth(segmentWidth), segmentDepth(segmentDepth), segmentMinAngleTurn(segmentMinAngleTurn), segmentMaxAngleTurn(segmentMaxAngleTurn), endRot(rot), sections(), types(), targets(), sectionSize(sectionSize), podSegmentSize(podSegmentSize), distractorSegmentSize(distractorSegmentSize), powerupSegmentSize(powerupSegmentSize), sectionIndex(0), spawnIndex(0), spawnCombo(0), podIndex(0), renewalSectionCounter(0), renewalPodCounter(0), renewalDistractorCounter(0), renewalPowerupCounter(0), spawnLimit(-1), numTargets(0), activePods(), playNo(playNo), stageNo(stageNo), mode(mode), phase(phase), stageTime(stageTime), totalElapsed(0.0), timePenalty(0.0), nback(nback), basis(sloc), sidesUsed(), materialNames(), eval(EVEN), signalTypes(signalTypes), navPhase(0), catchupPhase(0), tunnelSectionsPerNavigationUpgrade(10), navCheckpoint(0), navLevels(), propagateCounters(), guide(NO_DIRECTION), collectionCriteria(), powerups(powerups), hasHoldout(false), holdoutCounter(100), holdoutPod(0), holdoutIndex(0), holdoutFrequency(4), trackNBackA(0), trackNBackB(0), trackNBackC(0), done(false), cleanup(false), gateSlice(NULL), tVal(0.0f), gateOpen(false), activateGreen(false), tSpeedOpen(10.0f), tSpeed(0.0f), tAccel(2.0f), gateDelayTimer(0.0f), gateDelay(0.5f), flyOut(false)
{
    mainTunnelNode = parentNode->createChildSceneNode("mainTunnelNode" + Util::toStringInt(tunnelID));
	current = segments.end();
    setNewControl(control);
    
    // Add time based on n-back since players need more time before they can get targets
    this->stageTime += 2 * nback;
}

SceneNode* Tunnel::getMainTunnelNode() const
{
    return mainTunnelNode;
}

Vector3 Tunnel::getStart() const
{
	return start;
}

Vector3 Tunnel::getEnd() const
{
	return end;
}

Vector3 Tunnel::getCenter() const
{
	return (start + end) / 2;
}

float Tunnel::getTLeftPrevious() const
{
    return tLeftPrevious;
}

float Tunnel::getTLeftCurrent() const
{
    return tLeftCurrent;
}

float Tunnel::getTLeftOffsetPrevious() const
{
    return tLeftOffsetPrevious;
}

float Tunnel::getTLeftOffsetCurrent() const
{
    return tLeftOffsetCurrent;
}

std::list<TunnelSlice*>::const_iterator Tunnel::getPreviousIterator() const
{
    return previous;
}

std::list<TunnelSlice*>::const_iterator Tunnel::getCurrentIterator() const
{
    return current;
}

std::list<TunnelSlice*>::const_iterator Tunnel::getBeginIterator() const
{
    return segments.begin();
}

std::list<TunnelSlice*>::const_iterator Tunnel::getEndIterator() const
{
    return segments.end();
}

std::vector<TunnelSlice*> Tunnel::findSlicesSincePrevious() const
{
    std::vector<TunnelSlice*> ret;
    
    std::list<TunnelSlice*>::iterator it = previous;
    while (it != segments.end())
    {
        ret.push_back(*it);
        if (it == current)
            break;
        ++it;
    }
    return ret;
    
}

std::vector<TunnelSlice*> Tunnel::findSlicesSincePreviousOffset() const
{
    std::vector<TunnelSlice*> ret;
    
    std::list<TunnelSlice*>::iterator it = previousOffset;
    while (it != segments.end())
    {
        ret.push_back(*it);
        if (it == currentOffset)
            break;
        ++it;
    }
    return ret;
    
}

void Tunnel::setOffsetIterators(Vector3 pos, float tOffset)
{
    std::list<TunnelSlice*>::iterator it = current;
    
    previousOffset = currentOffset;
    tLeftOffsetPrevious = tLeftOffsetCurrent;
    float t = 0;
    while (it != segments.end())
    {
        
        t = (*it)->getT(pos) + tOffset;
        if (t <= 1)
        {
            tLeftOffsetCurrent = t;
            break;;
        }
        ++it;
    }
    currentOffset = it;
}

// An estimate of which Segment contains the position with a certain linear interpolated offset
TunnelSlice* Tunnel::findSliceFrom(std::list<TunnelSlice*>::const_iterator stit, Vector3 pos, float tOffset, float & tLeft) const
{
    std::list<TunnelSlice*>::const_iterator it = stit;
    
    float t = 0;
    while (it != segments.end())
    {
        
        t = (*it)->getT(pos) + tOffset;
        if (t <= 1)
        {
            tLeft = t;
            return *it;
        }
        ++it;
    }
    return NULL;
}

TunnelSlice* Tunnel::getPrevious() const
{
	if (previous == segments.end())
		return NULL;
	return *previous;
}

TunnelSlice* Tunnel::getCurrent() const
{
	if (current == segments.end())
		return NULL;
	return *current;
}
TunnelSlice* Tunnel::getPreviousOffset() const
{
	if (previousOffset == segments.end())
		return NULL;
	return *previousOffset;
}

TunnelSlice* Tunnel::getCurrentOffset() const
{
	if (currentOffset == segments.end())
		return NULL;
	return *currentOffset;
}
TunnelSlice* Tunnel::getFront() const
{
    return segments.front();
}
TunnelSlice* Tunnel::getBack() const
{
    return segments.back();
}

// Get the ith segment from current. Returns NULL if no ith segment exists
TunnelSlice* Tunnel::getNext(int i) const
{
    std::list<TunnelSlice*>::iterator it = current;
    for (int n = 0; n < i; ++n)
    {
        if (it == segments.end())
            return NULL;
        it++;
    }
    if (it == segments.end())
        return NULL;
	return *it;
}

// Note: includes current tunnel slice
std::vector<TunnelSlice*> Tunnel::getNSlices(int n) const
{
    std::vector<TunnelSlice*> ret;
    std::list<TunnelSlice*>::iterator it = current;
    for (int i = 0; i < n; ++i)
    {
        if (it == segments.end())
            break;;
        ret.push_back(*it);
        it++;
    }
	return ret;
}

int Tunnel::getSpawnIndex() const
{
    return spawnIndex;
}

int Tunnel::getSpawnCombo() const
{
    return spawnCombo;
}

int Tunnel::getSpawnLimit() const
{
    return spawnLimit;
}

int Tunnel::getNumTargets() const
{
    return numTargets;
}

int Tunnel::getSignalsLeft() const
{
    return spawnLimit - podIndex;
}

int Tunnel::getSectionIndex() const
{
    return sectionIndex;
}

int Tunnel::getPodIndex() const
{
    return podIndex;
}

Quaternion Tunnel::getQuaternion() const
{
    return mainTunnelNode->getOrientation();
}

Quaternion Tunnel::getCombinedQuaternion(TunnelSlice* slice) const
{
    return mainTunnelNode->getOrientation() * slice->getQuaternion();
}

Evaluation Tunnel::getEval() const
{
    return eval;
}

Pod* Tunnel::getNearestPod(int numSlices) const
{
    std::list<TunnelSlice*>::iterator it = current;
    int count = 0;
    while (count < numSlices && it != segments.end())
    {
        if (!(*it)->isInfoStored())
        {
            std::vector<Pod*> pods = (*it)->getPods();
            for (int i = 0; i < pods.size(); ++i)
                if (!pods[i]->getPodTrigger())
                    return pods[i];
        }
        ++count;
        ++it;
    }
    return NULL;
}

void Tunnel::getSpanFrom(int span, Direction base, const bool sides[NUM_DIRECTIONS], bool ret[NUM_DIRECTIONS])
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        ret[i] = false;
    if (base == NO_DIRECTION)
        return;
    
    int count = 0;
    // If panel exists for current location, enable it as a possible direction
    if (sides[base])
    {
        ret[base] = true;
        ++count;
    }
    // Determine total steps left and right that should be enabled as a possible direction
    Direction left = Util::leftOf(base);
    Direction right = Util::rightOf(base);
    for (int i = 0; i < span; ++i)
    {
        if (sides[left])
        {
            if (sides[left])
            {
                ret[left] = true;
                ++count;
                left = Util::leftOf(left);
            }
        }
        if (sides[right])
        {
            if (sides[right])
            {
                ret[right] = true;
                ++count;
                right = Util::rightOf(right);
            }
        }
    }
    // count is used to ensure that at least one panel is a possible direction.
    while (count <= 0 && left != right && Util::leftOf(left) != right)
    {
        if (sides[left])
        {
            ret[left] = true;
            ++count;
        }
        left = Util::leftOf(left);
        if (sides[right])
        {
            ret[right] = true;
            ++count;
        }
        right = Util::rightOf(right);
    }
}

Direction Tunnel::getRandPossibleDirection(SectionInfo segmentInfo, bool assignGuide)
{
    // Uses the current segment info (that the player is on) such that it would
    // be possible for the player to reach this Direction returned
    SectionInfo checkInfo = getCurrent()->getSectionInfo();
    
    bool panels[NUM_DIRECTIONS];
    // ALwyas use less panels
    if (Util::getNumSides(checkInfo.sidesUsed) < Util::getNumSides(segmentInfo.sidesUsed))
        Util::copySides(panels, checkInfo.sidesUsed);
    else
        Util::copySides(panels, segmentInfo.sidesUsed);
    
    bool set[NUM_DIRECTIONS];
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        set[i] = false;
    
    Direction vineLoc = player->getVineDest();
    // Guide uses the previous direction (obstacle opening) and assumes the best case the player
    // chooses the guide as the previous destination.
    if (guide != NO_DIRECTION)
        vineLoc = guide;
    int span = globals.span;
    int count = 0;
    // If panel exists for current location, enable it as a possible direction
    if (panels[vineLoc])
    {
        set[vineLoc] = true;
        ++count;
    }
    // Determine total steps left and right that should be enabled as a possible direction
    Direction left = Util::leftOf(vineLoc);
    Direction right = Util::rightOf(vineLoc);
    for (int i = 0; i < span; ++i)
    {
        if (panels[left])
        {
            if (panels[left])
            {
                set[left] = true;
                ++count;
                left = Util::leftOf(left);
            }
        }
        if (panels[right])
        {
            if (panels[right])
            {
                set[right] = true;
                ++count;
                right = Util::rightOf(right);
            }
        }
    }
    // count is used to ensure that at least one panel is a possible direction.
    while (count <= 0 && left != right && Util::leftOf(left) != right)
    {
        if (panels[left])
        {
            set[left] = true;
            ++count;
        }
        left = Util::leftOf(left);
        if (panels[right])
        {
            set[right] = true;
            ++count;
        }
        right = Util::rightOf(right);
    }
    if (Util::getNumSides(set) <= 0)
    {
        std::cout << "WARNING: FOUND NO SIDES IN FINDING RANDOM SPAWN LOCATION\n";
        return Util::randDirection(panels);
    }
    if (assignGuide)
    {
        guide = Util::randDirection(set);
        return guide;
    } else return Util::randDirection(set);
    /*
     // Always use less panels for a random location
     if (Util::getNumSides(checkInfo.sidesUsed) < Util::getNumSides(segmentInfo.sidesUsed))
     return Util::randDirection(checkInfo.sidesUsed);
     else
     return Util::randDirection(segmentInfo.sidesUsed);
     */
}

float Tunnel::getSegmentWidth() const
{
	return segmentWidth;
}

float Tunnel::getSegmentDepth() const
{
	return segmentDepth;
}

std::vector<SectionInfo> Tunnel::getSectionInfo() const
{
    return sections;
}

std::vector<PodInfo> Tunnel::getPodInfo() const
{
    return types;
}

Quaternion Tunnel::getNewSegmentQuaternion(Direction dir, int degrees) const
{
    Quaternion rot = endRot;
    
    Quaternion rot1;
    Quaternion rot2;
    switch (dir)
    {
        case NORTH:
            rot1.FromAngleAxis(Degree(degrees), globals.tunnelReferenceRight);
            rot = rot * rot1;
            break;
        case SOUTH:
            rot1.FromAngleAxis(Degree(-degrees), globals.tunnelReferenceRight);
            rot = rot * rot1;
            break;
        case WEST:
            rot1.FromAngleAxis(Degree(degrees), globals.tunnelReferenceUpward);
            rot = rot * rot1;
            break;
        case EAST:
            rot1.FromAngleAxis(Degree(-degrees), globals.tunnelReferenceUpward);
            rot = rot * rot1;
            break;
        case NORTHWEST:
            rot1.FromAngleAxis(Degree(degrees), globals.tunnelReferenceUpward);
            rot2.FromAngleAxis(Degree(degrees), globals.tunnelReferenceRight);
            rot = rot * rot1;
            rot = rot * rot1 * rot2;
            break;
        case NORTHEAST:
            rot1.FromAngleAxis(Degree(-degrees), globals.tunnelReferenceUpward);
            rot2.FromAngleAxis(Degree(degrees), globals.tunnelReferenceRight);
            rot = rot * rot1 * rot2;
            break;
        case SOUTHWEST:
            rot1.FromAngleAxis(Degree(degrees), globals.tunnelReferenceUpward);
            rot2.FromAngleAxis(Degree(-degrees), globals.tunnelReferenceRight);
            rot = rot * rot1 * rot2;
            break;
        case SOUTHEAST:
            rot1.FromAngleAxis(Degree(-degrees), globals.tunnelReferenceUpward);
            rot2.FromAngleAxis(Degree(-degrees), globals.tunnelReferenceRight);
            rot = rot * rot1 * rot2;
            break;
        default:
            // no rotation
            break;
    }
    
    return rot;
}

bool decide(bool m1, bool m2, bool m3, int track1, int track2, int track3)
{
	if (m1)
	{
		return (!m2 || track1 <= track2) && (!m3 || track1 <= track3);
	}
	return false;
}

// If numtimes is 2, that means the signal appears 3 times in a row
bool Tunnel::testForRepeatSignal(PodSignal ps, int numtimes)
{
    for (int i = 1; i <= numtimes; ++i)
    {
        if (getNBackTest(i, ps) != ps)
            return false;
    }
    return true;
}

// Returns the test for whether the next pod would cause a n-back match of numtimes
bool Tunnel::testForRepeatMatch(int nvalue, PodSignal ps, int numtimes)
{
    for (int i = 1; i <= numtimes - 1; ++i)
    {
        if (getNBackTest(types.size() - i, nvalue) == POD_SIGNAL_UNKNOWN)
            return false;
    }
    return getNBackTest(nvalue, ps) == ps;
}

// Generate a signal using some rules to make sure no repetitiveness
PodSignal Tunnel::generateItem(int nbackA, int nbackB, int nbackC, int trackA, int trackB, int trackC)
{
    std::vector<PodSignal> candidates;
	std::vector<PodSignal> priorities;
	
	bool m1 = (rand() % 100) < 33;
	bool m2 = (rand() % 100) < 33;
	bool m3 = (rand() % 100) < 33;
	for (int i = 0; i < signalTypes.size(); ++i)
	{
        PodSignal ps = (PodSignal)i;
        /*
        std::cout << testForRepeatSignal(ps, 2) << " "
            << testForRepeatMatch(nbackA, ps, 2) << " "
            << testForRepeatMatch(nbackB, ps, 2) << " "
        << testForRepeatMatch(nbackC, ps, 2) << std::endl;
        */
		// Test for 2 in a row and never let 3 in a row happen
		// Also never let the same n-back three in a row
		if (!testForRepeatSignal(ps, 2) &&
            !testForRepeatMatch(nbackA, ps, 2) &&
            !testForRepeatMatch(nbackB, ps, 2) &&
            !testForRepeatMatch(nbackC, ps, 2))
		{
			// Guarantee it if it should be that n-back with no double in a row of the same n-back
			if ((decide(m1, m2, m3, trackA, trackB, trackC) && getNBackTest(nbackA, ps) == ps && !testForRepeatMatch(nbackA, ps, 2)) ||
				(decide(m2, m1, m3, trackB, trackA, trackC) && getNBackTest(nbackB, ps) == ps && !testForRepeatMatch(nbackB, ps, 2)) ||
				(decide(m3, m1, m2, trackC, trackA, trackB) && getNBackTest(nbackC, ps) == ps && !testForRepeatMatch(nbackC, ps, 2)))
			{
                // Don't guarantee it if it causes a repeat signal
                priorities.push_back(ps);
			}
			candidates.push_back(ps);
		}
	}
	if (candidates.size() <= 0)
	{
		cout << "WARNING: NO AVAILABLE CANDIDATES TO PUSH BACK\n";
		return POD_SIGNAL_UNKNOWN;
	}
	if (priorities.size() > 0)
		return priorities[rand() % priorities.size()];
	else
		return candidates[rand() % candidates.size()];
}

// Returns the signal if the next signal added would be a match
// If no match, return POD_SIGNAL_UNKNOWN
PodSignal Tunnel::getNBackTest(int nvalue, PodSignal test) const
{
    if ((int)(types.size()) - nvalue >= 0 && types[types.size() - nvalue].podSignal == test)
        return test;
    else
        return POD_SIGNAL_UNKNOWN;
}

// Returns the type of signal that matched the n-back at the specified index
// If no match, return POD_SIGNAL_UNKNOWN
PodSignal Tunnel::getNBackTest(int ind, int nvalue) const
{
    if (ind - nvalue >= 0 && types[ind - nvalue].podSignal == types[ind].podSignal)
        return types[ind].podSignal;
    else
        return POD_SIGNAL_UNKNOWN;
}

// Tests for n-back at the podIndex (i.e. where the player is)
PodSignal Tunnel::getNBackTest(int nvalue) const
{
    return getNBackTest(podIndex, nvalue);
}

bool Tunnel::getPodIsGood(int index) const
{
    bool goodPod = getNBackTest(index, getNBackToggle()) != POD_SIGNAL_UNKNOWN;
    if (getMode() == STAGE_MODE_RECESS || getMode() == STAGE_MODE_TEACHING) goodPod = true;
    return goodPod;
}

// Returns true for the next pod if it is good depending on the Toggle of N-Back
bool Tunnel::getPodIsGood() const
{
    return getPodIsGood(podIndex);
}

// Returns the n-back based on the player's current toggle value.
// if there is only one n-back in a stage, this toggle value should be fixed
int Tunnel::getNBackToggle() const
{
    if (player->getToggleBack() == 3)
        return 0;
    return Util::clamp(getNBack() - player->getToggleBack(), 0, getNBack());
}

StageMode Tunnel::getMode() const
{
    return mode;
}

float Tunnel::getStageTime() const
{
    return stageTime;
}

float Tunnel::getTotalElapsed() const
{
    return totalElapsed;
}

float Tunnel::getTimePenalty() const
{
    return timePenalty;
}

float Tunnel::getTimeLeft() const
{
    return stageTime - getTotalElapsed() - getTimePenalty();
}

// Return percent complete based off the number of signals left to pass
float Tunnel::getPercentComplete() const
{
    float percentComplete = static_cast<float>(player->getNumCorrectTotal()) / getNumTargets();
    //float percentComplete = static_cast<float>((getSpawnLimit() - getSignalsLeft())) / getSpawnLimit();
    percentComplete = Util::clamp(percentComplete, 0.0, 100.0);
    return percentComplete;
}

int Tunnel::getNBack() const
{
    return nback;
}

int Tunnel::getControl() const
{
    return control;
}

Direction Tunnel::getBasis() const
{
    return basis;
}

bool Tunnel::hasAvailableSide(Direction side) const
{
    if (side == NO_DIRECTION)
        return false;
    return sidesUsed[side];
}

int Tunnel::getPlayNo() const
{
    return playNo;
}

int Tunnel::getStageNo() const
{
    return stageNo;
}

char Tunnel::getPhase() const
{
    // Always return capital case
    if (phase >= 'a' && phase <= 'z')
        return phase - ('a' - 'A');
    return phase;
}

void Tunnel::determineMaterial()
{
    // Determines material based on what phase it is
    materialNames.clear();
    switch (getPhase())
    {
        case 'A':
            materialNames.push_back("General/WallBindingA");
            break;
        case 'B':
            materialNames.push_back("General/WallBindingB");
            break;
        case 'C':
            materialNames.push_back("General/WallBindingC");
            break;
        case 'D':
            materialNames.push_back("General/WallBindingD");
            break;
        case 'E':
            materialNames.push_back("General/WallBindingE");
            break;
        case 'F':
            materialNames.push_back("General/Wall1");
            break;
        case 'G':
            materialNames.push_back("General/WallBindingG");
            break;
        default:
            materialNames.push_back("General/Wall1");
            break;
    }
}

std::string Tunnel::getMaterialName() const
{
    if (materialNames.size() <= 0)
        return "General/Wall0";
    return materialNames[rand() % materialNames.size()];
}

std::vector<NavigationLevel> Tunnel::getNavLevels() const
{
    return navLevels;
}

int Tunnel::getNumNavLevels() const
{
    return navLevels.size();
}

int Tunnel::getBuildingNavLevel() const
{
    return navLevels[navPhase].level;
}

int Tunnel::getCurrentNavLevel() const
{
    return navLevels[catchupPhase].level;
}

// Removes the first occurence of the type parameter specified and returns true
// If it is not found, it returns false
bool Tunnel::extractPowerup(PowerupType type)
{
    for (int i = 0; i < powerups.size(); ++i)
        if (powerups[i] == type)
        {
            powerups[i] = powerups[powerups.size() - 1];
            powerups.pop_back();
            return true;
        }
    return false;
}

std::vector<CollectionCriteria> Tunnel::getCollectionCriteria() const
{
    return collectionCriteria;
}

void Tunnel::checkIfDone()
{
    // Determine whether a stage has completed
    if (!isDone())
    {
        if (getMode() == STAGE_MODE_PROFICIENCY ||
            getMode() == STAGE_MODE_COLLECTION ||
            getMode() == STAGE_MODE_TEACHING)
        {
            if (isCriteriaSatisfied())
                setDone(PASS);
            //else if (player->getHP() >= globals.HPPositiveLimit)
            //    setDone(PASS);
            else if (spawnLimit > 0 && getSignalsLeft() <= 0)
                setDone(FAIL);//setDone(EVEN);
            else if (stageTime > 0 && getTimeLeft() <= 0)
                setDone(FAIL);//setDone(EVEN);
            else if (player->getHP() <= globals.HPNegativeLimit)
                setDone(FAIL);
        }
        else //if (getMode() == STAGE_MODE_RECESS)
        {
            if (player->getNumCorrectTotal() >= getNumTargets())
                setDone(PASS);
            //if (spawnLimit > 0 && getSignalsLeft() <= 0)
            //    setDone(PASS);
            else if (stageTime > 0 && getTimeLeft() <= 0)
                setDone(FAIL);//setDone(EVEN);
        }
    }
}

bool Tunnel::isDone() const
{
    return done;
}

void Tunnel::setDone(Evaluation eval)
{
    this->eval = eval;
    for (int i = 0; i < globals.initiationSections; ++i) {
        SectionInfo info = SectionInfo(BLANK, NO_DIRECTION, 0, endRot, sidesUsed);
        if (eval == PASS)
            info.tunnelType = CHECKPOINT_PASS;
        else if (eval == FAIL)
            info.tunnelType = CHECKPOINT_FAIL;
        else
            info.tunnelType = CHECKPOINT_EVEN;
        addSection(info);
    }
    done = true;
}

void Tunnel::setSpawnCombo(int level)
{
    spawnCombo = level;
}

void Tunnel::setSpawnLimit(int value)
{
    spawnLimit = value;
}

void Tunnel::setNumTargets(int value)
{
    numTargets = value;
}

// Used in Time/Speed Trial and is called everytime a new section is added
void Tunnel::upgradeControl()
{
    ++navCheckpoint;
    if (navCheckpoint >= tunnelSectionsPerNavigationUpgrade)
    {
        if (navPhase + 1 < navLevels.size())
        {
            navCheckpoint = 0;
            ++navPhase;
        
            // Counter to notify when new set of distractors obtained from new control level should be generated
            int propagateCounter = segments.size();// - renewalSectionCounter;
            for (std::list<int>::const_iterator it = propagateCounters.begin(); it != propagateCounters.end(); ++it)
                propagateCounter -= (*it);
            propagateCounters.push_back(propagateCounter);
            setNewControl(navLevels[navPhase].control);
        }
    }
}

void Tunnel::addToTimePenalty(float value)
{
    timePenalty += value;
}

void Tunnel::setVisible(bool value)
{
    mainTunnelNode->setVisible(value);
}

void Tunnel::setCleaning(bool value)
{
    cleanup = value;
}

bool Tunnel::needsCleaning() const
{
    return cleanup;
}

// Sets the boolean values for each discrete panel (panel or no panel)
void Tunnel::setNewControl(int control)
{
    this->control = control;
    Util::setSides(sidesUsed, control, basis);
}

// Sets navigation levels based on player nav level and nav limit
void Tunnel::setNavigationLevels(int tunnelSectionsPerNavLevel)
{
    navLevels.clear();
    PlayerLevel skill = player->getSkillLevel();
    int playerNavLevel = skill.navigation;
    if (playerNavLevel < 2) playerNavLevel = 2;

    setNavigationLevels(playerNavLevel - 2, skill.getNavLimit(), tunnelSectionsPerNavLevel);
    std::cout << "Nav Limit: " << skill.getNavLimit() << std::endl;
    
    navPhase = 0;
    catchupPhase = 0;
    navCheckpoint = 0;
    tunnelSectionsPerNavigationUpgrade = tunnelSectionsPerNavLevel;
    setNewControl(navLevels[navPhase].control);
    globals.stageTotalDistractorsMin = navLevels[navPhase].obstacles;
    globals.stageTotalDistractorsMax = navLevels[navPhase].obstacles;
}

// Sets navigation levels based on provided parameter nav level and clamps at provided nav limit
// Refers to globals.navLevels as the ordering of nav levels
void Tunnel::setNavigationLevels(int startingNavLevel, int navLimit, int tunnelSectionsPerNavLevel)
{
    navLevels.clear();
    int dn = 0;
    for (int i = 0; i < 25; ++i)
    {
        navLevels.push_back(globals.navMap[Util::clamp(startingNavLevel + dn, 0, navLimit)]);
        dn += 1;
    }

    std::cout << "Nav Levels: " << std::endl;
    for (int i = 0; i < navLevels.size(); ++i)
        std::cout << navLevels[i].level << " " << navLevels[i].control << " " << navLevels[i].obstacles << std::endl;
    
    navPhase = 0;
    catchupPhase = 0;
    navCheckpoint = 0;
    tunnelSectionsPerNavigationUpgrade = tunnelSectionsPerNavLevel;
    setNewControl(navLevels[navPhase].control);
    globals.stageTotalDistractorsMin = navLevels[navPhase].obstacles;
    globals.stageTotalDistractorsMax = navLevels[navPhase].obstacles;
}

// Sets navigation levels based on the parameters
void Tunnel::setNavigationLevels(const std::vector<NavigationLevel> & preset, int tunnelSectionsPerNavLevel)
{
    navLevels.clear();
    for (int i = 0; i < 25; ++i) // Duplicate copies for repeat
    {
        //std::cout << "test: " << preset[i % preset.size()].control << " " << preset[i % preset.size()].obstacles << std::endl;
        navLevels.push_back(preset[i % preset.size()]);
    }
    navPhase = 0;
    catchupPhase = 0;
    navCheckpoint = 0;
    tunnelSectionsPerNavigationUpgrade = tunnelSectionsPerNavLevel;
    setNewControl(navLevels[navPhase].control);
    globals.stageTotalDistractorsMin = navLevels[navPhase].obstacles;
    globals.stageTotalDistractorsMax = navLevels[navPhase].obstacles;
}

void Tunnel::setCollectionCriteria(const std::vector<CollectionCriteria> & value)
{
    collectionCriteria = value;
    
    // Determine whether it is a multi-type-collection task
    int minback = getLowestCriteria();
    int maxback = getHighestCriteria();
    multiCollectionTask = minback != maxback;
}

// Collects the first occurence of the n-back value in the collection criteria list
bool Tunnel::satisfyCriteria(int n)
{
    for (int i = 0; i < collectionCriteria.size(); ++i)
    {
        if (n == collectionCriteria[i].nback && !collectionCriteria[i].collected)
        {
            collectionCriteria[i].collected = true;
            return true;
        }
    }
    return false;
}

// Randomly set a collected item to uncollected and returns that index
// If no item is collected, -1 is returned
int Tunnel::loseRandomCriteria()
{
    std::vector<int> collectedRefs;
    for (int i = 0; i < collectionCriteria.size(); ++i)
    {
        if (collectionCriteria[i].collected)
            collectedRefs.push_back(i);
    }
    if (collectedRefs.size() > 0)
    {
        int ind = rand() % collectedRefs.size();
        collectionCriteria[collectedRefs[ind]].collected = false;
        return ind;
    }
    else
        return -1;
}

// Checks whether all collection criterias are satisfied
bool Tunnel::isCriteriaSatisfied() const
{
    for (int i = 0; i < collectionCriteria.size(); ++i)
    {
        if (!collectionCriteria[i].collected)
            return false;
    }
    return true;
}

// Returns true if the task has different types to collect
bool Tunnel::isMultiCollectionTask() const
{
    return multiCollectionTask;
}

// Sets all criteria to not collected or collected.
// Good for cheat debugging
bool Tunnel::setAllCriteriaTo(bool value)
{
    for (int i = 0; i < collectionCriteria.size(); ++i)
        collectionCriteria[i].collected = value;
}

// Returns the lowest n-back criteria required
int Tunnel::getLowestCriteria() const
{
    int ret = nback;
    for (int i = 0; i < collectionCriteria.size(); ++i)
    {
        if (collectionCriteria[i].nback < ret)
            ret = collectionCriteria[i].nback;
    }
    return ret;
}

// Returns the highest n-back criteria required
int Tunnel::getHighestCriteria() const
{
    int ret = 0;
    for (int i = 0; i < collectionCriteria.size(); ++i)
    {
        if (collectionCriteria[i].nback > ret)
            ret = collectionCriteria[i].nback;
    }
    return ret;
}

// Return the n-back of the first item in the collection criteria list
int Tunnel::getFirstCriteria() const
{
    return collectionCriteria.size() > 0 ? collectionCriteria[0].nback : 0;
}

// Returns the number of required collection criterias
int Tunnel::getNumRequiredCriteria() const
{
    return collectionCriteria.size();
}

// Returns the number of satisfied elements
int Tunnel::getNumSatisfiedCriteria() const
{
    int ret = 0;
    for (int i = 0; i < collectionCriteria.size(); ++i)
        if (collectionCriteria[i].collected)
            ret++;
    return ret;
}

void Tunnel::removeSegment()
{
	start += segments.front()->getForward() * (segmentDepth + globals.tunnelSegmentBuffer);
	segments.front()->removeFromScene();
	segments.pop_front();
}

SectionInfo Tunnel::getNextSectionInfo() const
{
    SectionInfo ret;
    TunnelType segmentType = NORMAL;
    Direction segmentDir = Util::randDirection();
    int segmentTurnAngle = Util::randRangeInt(segmentMinAngleTurn, segmentMaxAngleTurn);
    Quaternion segmentRot = getNewSegmentQuaternion(segmentDir, segmentTurnAngle);
    
    ret = SectionInfo(segmentType, segmentDir, segmentTurnAngle, segmentRot, sidesUsed);
    
    return ret;
}

SectionInfo Tunnel::getNextSegmentInfo(SectionInfo sectionInfo) const
{
    // Segment info is same as section
    SectionInfo ret = sectionInfo;
    return ret;
}

PodInfo Tunnel::getNextPodInfoAt(SectionInfo segmentInfo, SetPodTarget setting)
{
    PodInfo ret;
    int index = types.size();
    if (getMode() == STAGE_MODE_TEACHING || getMode() == STAGE_MODE_RECESS)
    {
        ret.podSignal = POD_SIGNAL_UNKNOWN;
        ret.meshType = POD_FUEL;
        ret.podColor = POD_COLOR_PURPLE;
        ret.podSound = POD_SOUND_UNKNOWN;
        ret.podShape = POD_SHAPE_UNKNOWN;
        ret.podLoc = getRandPossibleDirection(segmentInfo);
        ret.goodPod = true;
        ret.podTrigger = false;
        types.push_back(ret);
    }
    else if (nback <= 0)
    {
        Direction podLoc = getRandPossibleDirection(segmentInfo);
        int signalIndex = rand() % signalTypes.size();
        
        ret = signalTypes[signalIndex][rand() % signalTypes[signalIndex].size()];
        ret.podLoc = podLoc;
        ret.goodPod = true;
        ret.podTrigger = false;
        types.push_back(ret);
    }
    else // If not spawn one now
    {
        Direction podLoc = getRandPossibleDirection(segmentInfo);
        
        PodSignal final = POD_SIGNAL_UNKNOWN;
        if (!isMultiCollectionTask())
        {
            // If it is not a multi-collection task, do this pod gen since it's more accurate
            int testedNBack = getLowestCriteria();
        
            PodSignal repeat1 = POD_SIGNAL_UNKNOWN; // Two repeated signals
            PodSignal repeat2 = POD_SIGNAL_UNKNOWN; // Three repeated signals
            PodSignal repeat3 = POD_SIGNAL_UNKNOWN; // Four repeated signals! Unused
            // Obtain repeated signals
            if (types.size() > 2 && types[index - 1].podSignal == types[index - 2].podSignal)
                repeat1 = types[index - 1].podSignal;
            if (types.size() > 3 && repeat1 != POD_SIGNAL_UNKNOWN && types[index - 2].podSignal == types[index - 3].podSignal)
                repeat2 = types[index - 2].podSignal;
            if (types.size() > 4 && repeat2 != POD_SIGNAL_UNKNOWN && types[index - 3].podSignal == types[index - 4].podSignal)
                repeat3 = types[index - 3].podSignal;
        
            PodSignal guarantee = POD_SIGNAL_UNKNOWN;
            if (types.size() >= testedNBack && testedNBack > 0)
                guarantee = types[index - testedNBack].podSignal;
        
            if (setting == BAD_TARGET)
            {
                std::vector<PodSignal> candidates;
                
                if(candidates.size()!=1)
                {
                    for (int i = 0; i < signalTypes.size(); ++i)
                        if ((types.size() <= 0 || (PodSignal)i != types[index - 1].podSignal || (signalTypes.size() < 3 || testedNBack >= 2 )) && // Prevent repeated signals for nbacks less than 2 unless theres not enough supported signals
                            ((PodSignal)i != repeat1 || testedNBack > 3) && // If there's two in a row already, don't allow possibly three for nback 3
                            (PodSignal)i != repeat2 && // If there's already three in a row... don't even try 4
                            (PodSignal)i != guarantee) // This is supposed to be bad, it shouldn't be a target
                            candidates.push_back((PodSignal)i);
                    
                    // Reroll the next pod if it happens to be a repeat.
                    PodSignal podType = candidates[rand() % candidates.size()];
                    if (types.size() > 0 && types[index - 1].podSignal == podType)
                        podType = candidates[rand() % candidates.size()];
                    if (podType == repeat1)
                    {
                        podType = candidates[rand() % candidates.size()];
                    }
                    final = podType;
                }
                else final = (PodSignal)0; //added line?
            }
            else if (setting == GOOD_TARGET)
            {
                final = guarantee;
            }
            else //if (setting != UNKNOWN)
            {
                // Determine whether the next pod is a good target
                int r = rand() % 100 + 1;
                bool nbackGuarantee = r <= globals.podNBackChance;
                
                // If the next pod should be a target (rolled and no more than 3 in a row)
                if (nbackGuarantee && guarantee != repeat2)
                    final = guarantee;
        
                // Randomly assign next pod not a target
                if (final == POD_SIGNAL_UNKNOWN)
                {
                    std::vector<PodSignal> candidates;
                    for (int i = 0; i < signalTypes.size(); ++i)
                        if ((PodSignal)i != repeat2 && (PodSignal)i != guarantee)
                            candidates.push_back((PodSignal)i);
                    
                    // Reroll the next pod if it happens to be a repeat.
                    PodSignal podType = candidates[rand() % candidates.size()];
                    if (types.size() > 0 && types[index - 1].podSignal == podType)
                        podType = candidates[rand() % candidates.size()];
                    if (podType == repeat1)
                    {
                        podType = candidates[rand() % candidates.size()];
                    }
                    final = podType;
                }
            }
        }
        else
        {
            // If it is a multi-collection task do this generation task instead
            int nvalueA = nback - 2 > 0 ? nback - 2 : nback;
            int nvalueB = nback - 1 > 0 ? nback - 1 : nback;
            int nvalueC = nback;
            final = generateItem(nvalueA, nvalueB, nvalueC, trackNBackA, trackNBackB, trackNBackC);
            if (final == POD_SIGNAL_UNKNOWN)
                final = (PodSignal)(rand() % signalTypes.size());
            if (getNBackTest(nvalueA, final) != POD_SIGNAL_UNKNOWN) trackNBackA++;
            if (getNBackTest(nvalueB, final) != POD_SIGNAL_UNKNOWN) trackNBackB++;
            if (getNBackTest(nvalueC, final) != POD_SIGNAL_UNKNOWN) trackNBackC++;
        }
        
        ret = signalTypes[final][rand() % signalTypes[final].size()];
        
        ret.podLoc = podLoc;
        // Determine NBack of next pod is good
        ret.goodPod = (nback <= 0 || (types.size() >= nback && types[index - nback].podSignal == final));
        ret.podTrigger = false;
        
        bool timevar = false;
        float timefreq;
        
        //Percentage of stage time being withheld
        float starttime=player->holdoutLB;
        float endtime=player->holdoutUB;
        StageRequest level = player->getLevels()->retrieveLevel(player->getLevelRequestRow(), player->getLevelRequestCol());
        
        if ( level.holdoutStart!=level.holdoutEnd) {
            starttime = level.holdoutStart;
            endtime = level.holdoutEnd;
        }
        
        level = player->getLevels()->retrieveLevel(player->getLevelRequestRow(), player->getLevelRequestCol());
        
        
        //Holdout time bounds
        float holdouttimelb = stageTime - stageTime*starttime;
        float holdouttimeub = stageTime - stageTime*endtime;
        float transitiontime = abs(holdouttimeub - holdouttimelb);
        float quartertime = transitiontime/4;
        
        float frequencyquarter = 0;

        frequencyquarter = player->holdout*100/4;
        
        //std::cout<<"                Frequency quarter: "<<frequencyquarter<<std::endl;
        
        
        std::cout<<"           Holdout Progression Interval: ("<<holdouttimelb<<","<<holdouttimeub<<")"<<std::endl;
        
        if(getTimeLeft()<=holdouttimelb-quartertime*3) {
            setHoldout(true, 100/(frequencyquarter*4));
            std::cout<<"                        HOLDOUT Progression 100% --->"<< Tunnel::holdoutFrequency<<std::endl;
        }
        else if(getTimeLeft()<=holdouttimelb-quartertime*2) {
            setHoldout(true, 100/frequencyquarter*3);
            std::cout<<"                        HOLDOUT Progression 50%-75% --->"<<Tunnel::holdoutFrequency <<std::endl;
        }
        else if(getTimeLeft()<=holdouttimelb-quartertime) {
            setHoldout(true, 100/(frequencyquarter*2));
            std::cout<<"                        HOLDOUT Progression 25%-50% --->"<< Tunnel::holdoutFrequency<<std::endl;
        }
        else if(getTimeLeft()<=holdouttimelb) {
            setHoldout(true, 100/frequencyquarter);
            std::cout<<"                        HOLDOUT Progression 0-25%. --->"<<Tunnel::holdoutFrequency<<std::endl;
        }
        else if (getTimeLeft()>=holdouttimelb) {
            setHoldout(false);
            //std::cout<<"                        HOLDOUT IS OFF. --->"<<Tunnel::holdoutFrequency <<std::endl;
        }
        
        if(getTimeLeft()<=holdouttimelb) {
            setHoldout(true);
        }
        else {
            setHoldout(false);
        }
        
        std::cout<<"                Time Remaining: ("<<getTimeLeft()<<")"<<std::endl;
        std::cout<<"                Holdout Active: "<<hasHoldout<<std::endl;
        //<< " Level: " << player->getLevels()->holdoutFreqvar
        if( hasHoldout ) {
            if( holdoutCounter >= holdoutFrequency ) {
                float rand = Ogre::Math::UnitRandom();
                float incr = 1.0f / (float)holdoutFrequency;
                holdoutPod = (int)(rand/incr);
                std::cout << "Plan to holdout pod at position " << holdoutPod+1 << " of " << holdoutFrequency << " pods" << std::endl;
                holdoutCounter = 0;
                holdoutIndex = 0;
            }
            
            ++holdoutCounter;
        
            if( holdoutIndex == holdoutPod ) {
                ret.performHoldout(phase, player->soundVolume > 0.0);
            }
            
            ++holdoutIndex;
        }
        
        types.push_back(ret);
    }
    return ret;
}

std::vector<PodInfo> Tunnel::getNextDistractorInfo(SectionInfo segment, const std::vector<PodInfo> & signals)
{
    std::vector<PodInfo> ret = signals;
    
    bool availability[NUM_DIRECTIONS];
    Util::copySides(availability, segment.sidesUsed);   // Set tunnel segment panel availability
    for (int i = 0; i < ret.size(); ++i)
        availability[ret[i].podLoc] = false;            // Set existing pod locations to unavailable
    
    std::vector<int> availDirs;
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        if (availability[i]) availDirs.push_back(i);
    
    int count = Util::randRangeInt(globals.stageTotalDistractorsMin, globals.stageTotalDistractorsMax);
    int c = 0;
    while (c < count && availDirs.size() > 1)
    {
        int rind = rand() % availDirs.size();
        PodInfo newDistractor = PodInfo(POD_SIGNAL_UNKNOWN, POD_HAZARD, POD_COLOR_UNKNOWN, POD_SHAPE_SPHERE, POD_SOUND_UNKNOWN, (Direction)availDirs[rind], false, true, false);
        ret.push_back(newDistractor);
        
        availDirs[rind] = availDirs[availDirs.size() - 1];
        availDirs.pop_back();
        ++c;
    }
    if (c > 0)
        globals.numSegmentsWithObstacles++; // Used for statistics
    return ret;
}

std::vector<PodInfo> Tunnel::getNextPowerupInfo(SectionInfo segment, const std::vector<PodInfo> & signals)
{
    std::vector<PodInfo> ret = signals;
    
    bool availability[NUM_DIRECTIONS];
    Util::copySides(availability, segment.sidesUsed);   // Set tunnel segment panel availability
    for (int i = 0; i < ret.size(); ++i)
        availability[ret[i].podLoc] = false;            // Set existing pod locations to unavailable
    
    std::vector<int> availDirs;
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        if (availability[i]) availDirs.push_back(i);
    
    
    PowerupType power = POWERUP_NONE;
    //if (powerups.size() <= 0) return ret;
    //power = powerups[rand() % powerups.size()];
    
    // Assumes number of satisfied criteria is always 8
    if (getTimeLeft() >= 5.0 && getTimeLeft() <= 25.0 &&
        player->getNumCorrectTotal() >= 2 * player->getNumWrongTotal())
        power = POWERUP_TIME_WARP;
    
    if (availDirs.size() > 0 && extractPowerup(power))
    {
        int rind = rand() % availDirs.size();
        
        PodColor pcolor = POD_COLOR_UNKNOWN;
        switch (power)
        {
            case POWERUP_TRACTOR_BEAM:
                pcolor = POD_COLOR_PINK;
                break;
            case POWERUP_TIME_WARP:
                pcolor = POD_COLOR_GREEN;
                break;
            case POWERUP_SHIELDS:
                pcolor = POD_COLOR_BLUE;
                break;
            default:
                break;
        }
        
        PodInfo newPowerup = PodInfo(POD_SIGNAL_UNKNOWN, POD_POWERUP, pcolor, POD_SHAPE_SPHERE, POD_SOUND_UNKNOWN, (Direction)availDirs[rind], false, true, false);
        ret.push_back(newPowerup);
    }
    
    return ret;
}

PodInfo Tunnel::getNextPodInfo(SectionInfo segmentInfo, SetPodTarget setting)
{
    PodInfo ret = getNextPodInfoAt(segmentInfo, setting);
    ++spawnIndex;
    return ret;
}

PodInfo Tunnel::getNextPodInfo(SectionInfo segmentInfo)
{
    PodInfo ret;
    if (spawnIndex < targets.size()) // Should be a target
    {
        switch (spawnCombo)
        {
            case 1:
                ret = getNextPodInfoAt(segmentInfo, targets[spawnIndex].level1 ? GOOD_TARGET : BAD_TARGET);
                break;
            case 2:
                ret = getNextPodInfoAt(segmentInfo, targets[spawnIndex].level2 ? GOOD_TARGET : BAD_TARGET);
                break;
            case 3:
                ret = getNextPodInfoAt(segmentInfo, targets[spawnIndex].level3 ? GOOD_TARGET : BAD_TARGET);
                break;
            default:
                ret = getNextPodInfoAt(segmentInfo, targets[spawnIndex].level1 ? GOOD_TARGET : BAD_TARGET);
                break;
        }
        ++spawnIndex;
    }
    else
    {
        ret = getNextPodInfoAt(segmentInfo, UNKNOWN);
        ++spawnIndex;
    }
    return ret;
}

void Tunnel::setPods(TunnelSlice* segment, const std::vector<PodInfo> & podInfos)
{
    for (int i = 0; i < podInfos.size(); ++i)
        segment->addPod(podInfos[i]);
    
    std::vector<Pod*> pods = segment->getPods();
    for (int i = 0; i < pods.size(); ++i)
    {
        activePods.push_back(pods[i]);
    }
}

void Tunnel::addSegment(SectionInfo sectionInfo)
{
	TunnelSlice* nsegment = new TunnelSlice(mainTunnelNode, segmentCounter, sectionInfo, end, segmentWidth, segmentDepth, getMaterialName());
    ++segmentCounter;
    
	end = end + nsegment->getForward() * (segmentDepth + globals.tunnelSegmentBuffer);
    endRot = sectionInfo.tunnelRot;
    if (segments.size() <= 0) // Init TunnelSlice iterator
    {
        segments.push_back(nsegment);
        current = segments.begin();
    }
    else
    {
        segments.back()->connect(nsegment);
        segments.push_back(nsegment);
    }
}

// Moves a segment in front to the back of the list.
// The infinite segment is maintained to be the last element.
void Tunnel::renewSegment(SectionInfo sectionInfo)
{
    if (segments.size() <= 0)
        return;
    
	TunnelSlice *nsegment = segments.front();
    
    int oldNumPods = nsegment->getPods().size();
    for (int i = 0; i < oldNumPods && activePods.size() > 0; ++i)
        activePods.pop_front();
    
    nsegment->makeDecreasingTransition = false;
    if( Util::getNumSides(segments.back()->getSectionInfo().sidesUsed) == 5 && Util::getNumSides(sectionInfo.sidesUsed) == 3 )
        nsegment->makeDecreasingTransition = true;
    else if( Util::getNumSides(segments.back()->getSectionInfo().sidesUsed) == 7 && Util::getNumSides(sectionInfo.sidesUsed) == 5 )
        nsegment->makeDecreasingTransition = true;
    else if( Util::getNumSides(segments.back()->getSectionInfo().sidesUsed) == 8 && Util::getNumSides(sectionInfo.sidesUsed) == 7 )
        nsegment->makeDecreasingTransition = true;
    
    nsegment->rejuvenate(segmentCounter, sectionInfo, end, segmentWidth, segmentDepth, getMaterialName());
    ++segmentCounter;
    
    end = end + nsegment->getForward() * (segmentDepth + globals.tunnelSegmentBuffer);
    endRot = sectionInfo.tunnelRot;
    if (segments.size() > 1)
    {
        nsegment->disconnect();
        segments.back()->connect(nsegment);
        segments.push_back(nsegment);
        segments.pop_front();
    }
}

void Tunnel::addSection(SectionInfo newSection)
{
    if (newSection.tunnelType > NORMAL) {
        
        //for (int i = 0; i < sectionSize; ++i)
        addSegment(newSection);
        return;
    }
    for (int i = 0; i < sectionSize; ++i)
    {
        addSegment(getNextSegmentInfo(newSection));
    }
    sections.push_back(newSection);
    upgradeControl();
}

void Tunnel::renewSection(SectionInfo newSection)
{
    if (newSection.tunnelType > NORMAL) {
        
        for (int i = 0; i < sectionSize; ++i)
            renewSegment(newSection);
        return;
    }
    for (int i = 0; i < sectionSize; ++i)
    {
        renewSegment(getNextSegmentInfo(newSection));
    }
    sections.push_back(newSection);
    upgradeControl();
}

bool Tunnel::updateIterators(Vector3 checkPos)
{
	if (current == segments.end())
		return false;

    previous = current;
    tLeftPrevious = tLeftCurrent;
    float t = (*current)->getT(checkPos);
    while (t > 1)
    {
        std::vector<Pod*> pods = (*current)->getPods();
        if (!isDone() && pods.size() > 0 && !pods[0]->getPodTrigger())
        {
            ++podIndex;
        }
        // Update the pod type index if we have just passed a segment with a pod
        ++current;
        
        if (!done)
        {
            ++renewalSectionCounter;
            if (propagateCounters.size() > 0)
            {
                propagateCounters.front()--;
                while (propagateCounters.size() > 0 && propagateCounters.front() <= 0)
                {
                    propagateCounters.pop_front();
                    ++catchupPhase; // Guaranteed to be less than navPhase
                    globals.stageTotalDistractorsMin = navLevels[catchupPhase].obstacles;
                    globals.stageTotalDistractorsMax = navLevels[catchupPhase].obstacles;
                }
            }
        }
        t = (*current)->getT(checkPos);
    }
    tLeftCurrent = t;
    
    return previous != current;
}

// Determines whether the current segment should be updated depending on the position compared.
// Returns true if a renew was necessary
bool Tunnel::renewIfNecessary()
{
    bool renewed = false;
    if (!done)
    {
        // Update the section index and recycle sections
        while (renewalSectionCounter >= globals.tunnelSegmentsBeforeRefresh)
        {
            
            SectionInfo info = getNextSectionInfo();
            renewSection(info);
            ++sectionIndex;
            renewalSectionCounter -= globals.tunnelSegmentsPerSection;
            
            renewed = true;
        }
    }
    return renewed;
}

void Tunnel::unlink()
{
    this->player = NULL;
}

void Tunnel::link(Player* player)
{
    this->player = player;
}

void Tunnel::presetTargets(int level)
{
    int testedNBack = getLowestCriteria();
    
    // The following uses a binning algorithm originated from the famous pigeonhole principle
    int TOTAL_SIGNALS = globals.stageTotalSignals;
    int TOTAL_ITEMS = 0;
    int BIN_SIZE = 0;;
    switch (level)
    {
        case 1:
            TOTAL_ITEMS = globals.stageTotalTargets1 + Util::randRangeInt(-globals.stageTotalTargetsVariance, globals.stageTotalTargetsVariance);
            BIN_SIZE = globals.podBinSize1;
            break;
        case 2:
            TOTAL_ITEMS = globals.stageTotalTargets2 + Util::randRangeInt(-globals.stageTotalTargetsVariance, globals.stageTotalTargetsVariance);
            BIN_SIZE = globals.podBinSize2;
            break;
        case 3:
            TOTAL_ITEMS = globals.stageTotalTargets3 + Util::randRangeInt(-globals.stageTotalTargetsVariance, globals.stageTotalTargetsVariance);
            BIN_SIZE = globals.podBinSize3;
            break;
    }
    
    // How many bins do we need
    int NUM_BINS = TOTAL_SIGNALS % BIN_SIZE ?
    TOTAL_SIGNALS / BIN_SIZE + 1 :
    TOTAL_SIGNALS / BIN_SIZE;
    
    int NUM_ASSIGNED_PER_BIN_DEFAULT = TOTAL_ITEMS / NUM_BINS;
    
    int itemsLeft = TOTAL_ITEMS - NUM_ASSIGNED_PER_BIN_DEFAULT * NUM_BINS;
    std::vector<int> binItemCounts(NUM_BINS, NUM_ASSIGNED_PER_BIN_DEFAULT);
    
    // Allocate remaining items to random bins
    std::vector<int> binIndices(NUM_BINS);
    for (int i = 0; i < binIndices.size(); ++i)
        binIndices[i] = i;
    while (itemsLeft > 0)
    {
        int rind = rand() % binIndices.size();
        binItemCounts[binIndices[rind]]++;
        
        binIndices[rind] = binIndices[binIndices.size() - 1];
        binIndices.pop_back();
        --itemsLeft;
    }
    
    // Now that we have our bins, we assign positions in those bins for good signals
    std::vector<bool> signalTargets(TOTAL_SIGNALS + testedNBack, false);
    for (int n = 0; n < binItemCounts.size(); ++n)
    {
        int signalStartIndex = testedNBack + n * BIN_SIZE;
        
        std::vector<int> signalSlotIndices(BIN_SIZE);
        for (int i = 0; i < signalSlotIndices.size(); ++i)
            signalSlotIndices[i] = signalStartIndex + i;
        
        while (binItemCounts[n] > 0)
        {
            int rind = rand() % signalSlotIndices.size();
            
            int signalIndex = signalSlotIndices[rind];
            if (signalIndex < signalTargets.size())
            {
                signalTargets[signalIndex] = true;
            }
            
            signalSlotIndices[rind] = signalSlotIndices[signalSlotIndices.size() - 1];
            signalSlotIndices.pop_back();
            
            binItemCounts[n]--;
        }
    }
    
    
    switch (level)
    {
        case 1:
        {
            for (int i = 0; i < signalTargets.size(); ++i)
                targets[i].level1 = signalTargets[i];
            break;
        }
        case 2:
        {
            for (int i = 0; i < signalTargets.size(); ++i)
                targets[i].level2 = signalTargets[i];
            spawnLimit = signalTargets.size();
            break;
        }
        case 3:
        {
            for (int i = 0; i < signalTargets.size(); ++i)
                targets[i].level3 = signalTargets[i];
            break;
        }
    }
    numTargets = globals.stageTotalCollections;
    
    /*
    // Plague spreading method
    for (int i = 0; i < signalTargets.size(); ++i)
    {
        TargetInfo tinfo;
        tinfo.level1 = signalTargets[i];
        tinfo.level2 = (i + 1 < signalTargets.size() && i + 1 >= nback && signalTargets[i + 1]) || tinfo.level1;
        tinfo.level3 = (i - 1 >= nback && signalTargets[i - 1]) || tinfo.level2;
        targets.push_back(tinfo);
     
     }
     spawnLimit = signalTargets.size();
     numTargets = TOTAL_ITEMS;
     */
}

void Tunnel::constructTunnel(const std::string & nameTunnelTile, int size)
{
    // Pregenerate some targets
    if (nback > 0 && getMode() != STAGE_MODE_RECESS && !isMultiCollectionTask())
    {
        int testedNBack = getLowestCriteria();
        
        targets = std::vector<TargetInfo>(globals.stageTotalSignals + testedNBack);
        presetTargets(1);
        presetTargets(2);
        presetTargets(3);
        
        for (int i = 0; i < targets.size(); ++i)
            std::cout << targets[i].level1;
        std::cout << std::endl;
        for (int i = 0; i < targets.size(); ++i)
            std::cout << targets[i].level2;
        std::cout << std::endl;
        for (int i = 0; i < targets.size(); ++i)
            std::cout << targets[i].level3;
        std::cout << std::endl;
        int podInd = 0;
    }
    else
    {
        numTargets = globals.stageTotalCollections;
        //spawnLimit = globals.stageTotalCollections;
    }
    
    //determineMaterial();
    materialNames.push_back(nameTunnelTile);
    
    // Output strands
    for (int i = 0; i < globals.initiationSections; ++i) {
        SectionInfo info = SectionInfo(BLANK, NO_DIRECTION, 0, endRot, sidesUsed);
        addSection(info);
    }
    
    renewalPodCounter = -globals.initialSegmentsFirstPod;
    renewalDistractorCounter = -globals.initialSegmentsFirstDistractors;
    renewalPowerupCounter = -globals.initialSegmentsFirstPowerup;
    for (int i = 0; i < size - globals.initiationSections; ++i) {
        SectionInfo info = getNextSectionInfo();
        addSection(info);
    }
    
    std::list<TunnelSlice*>::iterator it = current;
    for (int i = 0; i <= globals.podAppearance && it != segments.end(); ++i)
    {
        (*it)->updateGrowth(1.0);
        std::vector<Pod*> pods = (*it)->getPods();
        for (int j = 0; j < pods.size(); ++j)
        {
            pods[j]->uncloakPod();
#ifdef DEBUG_MODE
            if (!pods[j]->getPodTrigger())
            {
                pods[j]->generateIndicator();
                pods[j]->setVisibleIndicator(getPodIsGood() && player->getGodMode());
            }
#endif
            if (pods[j]->getPodTrigger() && pods[j]->getMeshType() == POD_HAZARD)
                player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_OBSTACLE);
        }
        ++it;
    }
}

void Tunnel::update(float elapsed)
{
    if (!isDone() && !player->isPowerUpActive("TimeWarp"))
        totalElapsed += elapsed;
    
    // Animate Pod Growing outwards or Growing inwards
    const float GROWTH_SPEED = player->getFinalSpeed() / 10.0;
    TunnelSlice* nextSliceM = getNext(globals.podAppearance);
    if (nextSliceM) {
        if (!isDone())
            nextSliceM->updateGrowth(GROWTH_SPEED * elapsed);
        else
            nextSliceM->updateGrowth(-2 * GROWTH_SPEED * elapsed);
    }
    nextSliceM = getNext(globals.podAppearance + 1);
    if (nextSliceM) {
        if (!isDone())
            nextSliceM->updateGrowth(GROWTH_SPEED * elapsed);
        else
            nextSliceM->updateGrowth(-2 * GROWTH_SPEED * elapsed);
    }
    
    // Check to see if we need to recycle tunnel segments
    if (!flyOut && updateIterators(player->getCamPos())) {
        renewIfNecessary();
        
        // Set new camera slerp goal
        player->saveCam();
        
        TunnelSlice* nextSlice1 = getNext(1);
        if (nextSlice1) player->setDesireRot(nextSlice1->getQuaternion());
        
        // Flag to notify main app this tunnel is completely over
        flyOut = !nextSlice1;//****//cleanup = !nextSlice1;
        
        // Show Pod Color and Play Sound
        TunnelSlice* nextSliceN = getNext(globals.podAppearance);
        if (nextSliceN && !isDone())
        {
            std::vector<Pod*> pods = nextSliceN->getPods();
            for (int i = 0; i < pods.size(); ++i) {
                pods[i]->uncloakPod();
                player->playPodSound(pods[i]->getPodSound());
                //pods[i]->setRotateSpeed(Vector3(5.0, 5.0, 5.0));
                
#ifdef DEBUG_MODE
                if (!pods[i]->getPodTrigger())
                {
                    pods[i]->generateIndicator();
                    pods[i]->setVisibleIndicator(getPodIsGood() && player->getGodMode());
                }
#endif
                if (pods[i]->getPodTrigger() && pods[i]->getMeshType() == POD_HAZARD)
                    player->getTutorialMgr()->setSlides(TutorialManager::TUTORIAL_SLIDES_OBSTACLE);
            }
        }
        nextSliceN = getNext(globals.podAppearance + 1);
        if (nextSliceN && !isDone())
        {
            // Determine whether should be added to this segment
            ++renewalPodCounter;
            ++renewalDistractorCounter;
            ++renewalPowerupCounter;
            PodInfo signalInfo;
            std::vector<PodInfo> podInfos;
            if (renewalPodCounter >= podSegmentSize && (spawnLimit < 0 || spawnIndex < spawnLimit))
            {
                PodInfo signalInfo = getNextPodInfo(nextSliceN->getSectionInfo());
                if (signalInfo.podLoc != NO_DIRECTION)
                    podInfos.push_back(signalInfo);
                renewalPodCounter = 0;
            }
            if (renewalDistractorCounter >= distractorSegmentSize && (spawnLimit < 0 || spawnIndex < spawnLimit))
            {
                podInfos = getNextDistractorInfo(nextSliceN->getSectionInfo(), podInfos);
                renewalDistractorCounter = 0;
            }
            if (renewalPowerupCounter >= powerupSegmentSize && (spawnLimit < 0 || spawnIndex < spawnLimit))
            {
                podInfos = getNextPowerupInfo(nextSliceN->getSectionInfo(), podInfos);
                renewalPowerupCounter = 0;
            }
            setPods(nextSliceN, podInfos);
        }
    }
    
    std::list<Pod *>::iterator it;
    for (it = activePods.begin(); it != activePods.end(); ++it)
    {
        (*it)->update(elapsed);
    }
    checkIfDone();
    player->update(elapsed);
    
    cleanup = !cleanup ? (isDone() && player->getAnimationTimer() <= 0.0) : cleanup;
    
    nextSliceM = getNext(globals.podAppearance+4);
    if ( !gateSlice && nextSliceM->getType() == CHECKPOINT_PASS ) {
        gateSlice = nextSliceM;
    }
    else if( !gateSlice && nextSliceM->getType() == CHECKPOINT_FAIL ) {
        gateSlice = nextSliceM;
        tVal = 4.0f;
    }
    gateAnimation(elapsed);
}

void Tunnel::gateAnimation(float elapsed)
{
    if( gateSlice && gateSlice->getType() == CHECKPOINT_PASS ) {
        if( !gateOpen ) {
            if( !activateGreen ) {
                if( gateDelayTimer >= gateDelay ) {
                    activateGreen = true;
                    OgreOggISound* sound = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("GateOpen");
                    sound->setVolume(player->soundVolume);
                    sound->play();
                    player->stopMusic();
                }
                else {
                    gateDelayTimer += elapsed;
                    gateSlice->gateEntity->getSubEntity(1)->setMaterialName("General/WallCheckpointPass");
                }
            }
            else {
                if( tVal + tSpeed*elapsed >= 4.0f ) {
                    tVal = 4.0f;
                    if( gateDelayTimer >= gateDelay ) {
                        gateOpen = true;
                        gateDelayTimer = 0.0f;
                        tSpeed = 0.0f;
                        OgreOggISound* sound = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("LevelPass");
                        sound->setVolume(player->musicVolume);
                        sound->play();
                    }
                    else {
                        gateDelayTimer += elapsed;
                    }
                }
                else {
                    if( tSpeed+tAccel >= tSpeedOpen ) {
                        tSpeed = tSpeedOpen;
                    }
                    else {
                        tSpeed += tAccel;
                    }
                    tVal += tSpeed*elapsed;
                }
            
                for( int i = 0; i < 8; ++i ) {
                    Vector3 t = Vector3(0,tVal,0);
                    Vector3 r = Vector3(Math::Cos(Degree(45*i))*t.x - Math::Sin(Degree(45*i))*t.y,Math::Sin(Degree(45*i))*t.x + Math::Cos(Degree(45*i))*t.y,0);
                    gateSlice->gateDoorNodes[i]->setPosition(r);
                }
            }
        }
    }
    if( gateSlice && gateSlice->getType() == CHECKPOINT_FAIL ) {
        if( !gateOpen ) {
            if( !activateGreen ) {
                if( gateDelayTimer >= gateDelay ) {
                    activateGreen = true;
                    OgreOggISound* sound = OgreFramework::getSingletonPtr()->m_pSoundMgr->getSound("GateClose");
                    sound->setVolume(player->soundVolume);
                    sound->play();
                }
                else {
                    gateDelayTimer += elapsed;
                    gateSlice->gateEntity->getSubEntity(1)->setMaterialName("General/WallCheckpointFail");
                }
            }
            else {
                if( tVal - tSpeed*elapsed <= 0.0f ) {
                    tVal = 0.0f;
                    if( gateDelayTimer >= gateDelay ) {
                        gateOpen = true;
                        gateDelayTimer = 0.0f;
                        tSpeed = 0.0f;
                    }
                    else {
                        gateDelayTimer += elapsed;
                    }
                }
                else {
                    if( tSpeed+tAccel >= tSpeedOpen ) {
                        tSpeed = tSpeedOpen;
                    }
                    else {
                        tSpeed += tAccel;
                    }
                    tVal -= tSpeed*elapsed;
                }
                
                for( int i = 0; i < 8; ++i ) {
                    Vector3 t = Vector3(0,tVal,0);
                    Vector3 r = Vector3(Math::Cos(Degree(45*i))*t.x - Math::Sin(Degree(45*i))*t.y,Math::Sin(Degree(45*i))*t.x + Math::Cos(Degree(45*i))*t.y,0);
                    gateSlice->gateDoorNodes[i]->setPosition(r);
                }
            }
        }
    }
}

bool Tunnel::getFlyOut()
{
    return flyOut;
}

void Tunnel::respondToToggleCheat()
{
#ifdef DEBUG_MODE
    Pod* pod = getNearestPod(globals.podAppearance + 1);
    if (pod)
        pod->setVisibleIndicator(getPodIsGood() && player->getGodMode());
#endif
}

void Tunnel::setHoldout(bool val, int freq)
{
    hasHoldout = val;
    
    holdoutFrequency = freq;
    
}

void Tunnel::setHoldout(bool val)
{
    hasHoldout = val;

}


void Tunnel::setHoldout( int freq)
{
    holdoutFrequency = freq;
}


Tunnel::~Tunnel()
{
	while (!segments.empty())
	{
		segments.front()->removeFromScene();
        delete segments.front();
		segments.pop_front();
	}
    
    mainTunnelNode->removeAndDestroyAllChildren();
    mainTunnelNode->getCreator()->destroySceneNode(mainTunnelNode);
    mainTunnelNode = NULL;
}
