//
//  Tunnel.cpp
//  Testing
//
//  Created by Calvin Phung on 9/10/13.
//
//

#include "Tunnel.h"
#include "Player.h"
#include "Hud.h"
#include <cstdlib>

using namespace std;

extern Util::ConfigGlobal globals;

const double infinityDepth = 1024;

static int tunnelID = 0;

Tunnel::Tunnel()
: parentNode(NULL), mainTunnelNode(NULL), start(), end(), segments(), tLeftPrevious(0.0), tLeftCurrent(0.0), previous(), current(), tLeftOffsetPrevious(0.0), tLeftOffsetCurrent(0.0), previousOffset(), currentOffset(), segmentCounter(0), segmentWidth(0.0), segmentDepth(0.0), sections(), types(), sectionSize(0), podSegmentSize(0), spawnIndex(0), podIndex(0), sectionIndex(0), renewalSectionCounter(0), renewalPodCounter(0), spawnLimit(-1), numTargets(0), activePods(), mode(GAME_NORMAL), totalElapsed(0.0), nback(1), control(0), history(NULL), basis(NO_DIRECTION), sidesUsed(), testType(POD_TYPE_COLOR), setColors(false), setSounds(false), setShapes(false), done(false), cleanup(false)
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = true;
}

Tunnel::Tunnel(Ogre::SceneNode* parentNode, Vector3 start, double segmentWidth, double segmentDepth, int segmentMinAngleTurn, int segmentMaxAngleTurn, GameMode mode, int nback, int control, Direction sloc, int sectionSize, int podSegmentSize, PodType testType, bool setColors, bool setSounds, bool setShapes)
: parentNode(parentNode), mainTunnelNode(NULL), start(start), end(start), segments(), tLeftPrevious(0.0), tLeftCurrent(0.0), previous(), current(), tLeftOffsetPrevious(0.0), tLeftOffsetCurrent(0.0), previousOffset(), currentOffset(), segmentCounter(0), segmentWidth(segmentWidth), segmentDepth(segmentDepth), segmentMinAngleTurn(segmentMinAngleTurn), segmentMaxAngleTurn(segmentMaxAngleTurn), sections(), types(), sectionSize(sectionSize), podSegmentSize(podSegmentSize), sectionIndex(0), spawnIndex(0), podIndex(0), renewalSectionCounter(0), renewalPodCounter(0), spawnLimit(-1), numTargets(0), activePods(), mode(mode), totalElapsed(0.0), nback(nback), control(control), history(NULL), basis(sloc), sidesUsed(), testType(testType), setColors(setColors), setSounds(setSounds), setShapes(setShapes), done(false), cleanup(false)
{
    mainTunnelNode = parentNode->createChildSceneNode("mainTunnelNode" + Util::toStringInt(tunnelID));
    history = new History(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, nback);
	current = segments.end();
    
    setNewControl(control);
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

double Tunnel::getTLeftPrevious() const
{
    return tLeftPrevious;
}

double Tunnel::getTLeftCurrent() const
{
    return tLeftCurrent;
}

double Tunnel::getTLeftOffsetPrevious() const
{
    return tLeftOffsetPrevious;
}

double Tunnel::getTLeftOffsetCurrent() const
{
    return tLeftOffsetCurrent;
}

std::list<TunnelSlice*>::const_iterator Tunnel::getPreviousIterator() const
{
    return current;
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

void Tunnel::setOffsetIterators(Vector3 pos, double tOffset)
{
    std::list<TunnelSlice*>::iterator it = current;
    
    previousOffset = currentOffset;
    tLeftOffsetPrevious = tLeftOffsetCurrent;
    double t = 0;
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
TunnelSlice* Tunnel::findSliceFrom(std::list<TunnelSlice*>::const_iterator stit, Vector3 pos, double tOffset, double & tLeft) const
{
    std::list<TunnelSlice*>::const_iterator it = stit;
    
    double t = 0;
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

int Tunnel::getSpawnIndex() const
{
    return spawnIndex;
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

History* Tunnel::getHistory() const
{
    return history;
}

double Tunnel::getSegmentWidth() const
{
	return segmentWidth;
}

double Tunnel::getSegmentDepth() const
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

Quaternion Tunnel::getNewSegmentQuaternion(Direction dir, int degrees)
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
    
    endRot = rot;
    
    return rot;
}

PodSignal Tunnel::getNBackTest(int ind) const
{
    if (ind - nback >= 0 && types[ind - nback].podSignal == types[ind].podSignal)
        return types[ind].podSignal;
    else
        return POD_SIGNAL_UNKNOWN;
}

GameMode Tunnel::getMode() const
{
    return mode;
}

double Tunnel::getTotalElapsed() const
{
    return totalElapsed;
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

std::string Tunnel::determineMaterial() const
{
    if (nback == 1)
        return "General/Wall1";
    switch ((nback - 2) % 6)
    {
        case 0:
            return "General/Wall2";
            //return "TextureModColor";
        case 1:
            return "General/Wall3";
        case 2:
            return "General/Wall4";
        case 3:
            return "General/Wall5";
        case 4:
            return "General/Wall6";
        case 5:
            return "General/Wall7";
        case 6:
            return "General/Wall8";
        default:
            return "General/Wall0";
    }
}

bool Tunnel::isDone() const
{
    return done;
}

void Tunnel::setDone(Evaluation eval)
{
    // Generate a CHECKPOINT tunnel section
    for (int i = 0; i < globals.initiationSections; ++i) {
        SectionInfo info = SectionInfo(NORMAL_BLANK, NO_DIRECTION, 0);
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

void Tunnel::upgradeControl()
{
    // Measure by num signals
    int currentControlLevel = control;
    if ((currentControlLevel == 1 && getSignalsLeft() <= spawnLimit - 10 - nback) ||
        (currentControlLevel == 2 && getSignalsLeft() <= spawnLimit - 25 - nback) ||
        (currentControlLevel == 3 && getSignalsLeft() <= spawnLimit - 50 - nback))
    {
        currentControlLevel++;
        SectionInfo info = SectionInfo(CHECKPOINT_PASS, NO_DIRECTION, 0);
        addSegment(info,
                   PodInfo(POD_SIGNAL_UNKNOWN, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_UNKNOWN, NO_DIRECTION, false));
        setNewControl(currentControlLevel);
    }
    
}

bool Tunnel::needsCleaning() const
{
    return cleanup;
}

void Tunnel::setNewControl(int control)
{
    this->control = control;
    Util::setSides(sidesUsed, control, basis);
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
    TunnelType segmentType = NORMAL_WITH_ONE_POD;
    Direction segmentDir = Util::randDirection();
    int segmentTurnAngle = Util::randRangeInt(segmentMinAngleTurn, segmentMaxAngleTurn);
    ret = SectionInfo(segmentType, segmentDir, segmentTurnAngle);
    return ret;
}


PodInfo Tunnel::getNextPodInfoAt(SetPodTarget setting, int index)
{
    PodInfo ret;
    if (index < types.size()) // Has the pods been predefined?
    {
        ret = types[spawnIndex];
    }
    else // If not spawn one now
    {
        Direction podLoc = Util::randDirection(sidesUsed);
        PodSignal final = POD_SIGNAL_UNKNOWN;
        
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
        if (types.size() >= nback && nback > 0)
            guarantee = types[index - nback].podSignal;
        
        if (setting == BAD_TARGET)
        {
            std::vector<PodSignal> candidates;
            for (int i = 0; i < 4; ++i)
                if ((types.size() <= 0 || (PodSignal)i != types[index - 1].podSignal || nback > 2) && // Prevent repeated signals for nbacks less than 2
                    ((PodSignal)i != repeat1 || nback > 3) && // If there's two in a row already, don't allow possibly three for nback 3
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
        else if (setting == GOOD_TARGET)
        {
            final = guarantee;
        }
        else //if (setting == UNKNOWN)
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
                for (int i = 0; i < 4; ++i)
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
        
        ret.podSignal = final;
    
        // Match Color to signal
        if (testType == POD_TYPE_COLOR || setColors)
            ret.podColor = (PodColor)final;
    
        // Match Sound to signal
        if (testType == POD_TYPE_SOUND || setSounds)
            ret.podSound = (PodSound)final;
        
        // Match Shape to signal
        if (testType == POD_TYPE_SHAPE || setShapes)
            ret.podShape = (PodShape)final;
        
        ret.podLoc = podLoc;
    
        // Determine NBack of next pod is good
        ret.goodPod = (nback <= 0 || (types.size() >= nback && types[index - nback].podSignal == final));
        
        types.push_back(ret);
    }
    return ret;
}

PodInfo Tunnel::getNextPodInfo(SetPodTarget setting)
{
    PodInfo ret = getNextPodInfoAt(setting, spawnIndex);
    return ret;
}

void Tunnel::addSegment(SectionInfo sectionInfo, PodInfo podInfo)
{
    Quaternion rot;
    rot = getNewSegmentQuaternion(sectionInfo.tunnelDir, sectionInfo.tunnelDirAngle);
    
    Vector3 forward = rot * globals.tunnelReferenceForward;
    Vector3 stepend = end + forward * (segmentDepth + globals.tunnelSegmentBuffer);
	TunnelSlice* nsegment = new TunnelSlice(mainTunnelNode, segmentCounter, sectionInfo.tunnelType, (end + stepend) / 2, rot, segmentWidth, segmentDepth, determineMaterial(), sidesUsed);
    ++segmentCounter;
    
    switch (sectionInfo.tunnelType)
    {
        case NORMAL_WITH_ONE_POD:
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, podInfo.podLoc, podInfo.goodPod));
            break;
        case NORMAL_WITH_THREE_PODS:
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::leftOf(podInfo.podLoc), podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, podInfo.podLoc, podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::rightOf(podInfo.podLoc), podInfo.goodPod));
            break;
        case NORMAL_WITH_FIVE_PODS:
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::leftOf(Util::leftOf(podInfo.podLoc)), podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::leftOf(podInfo.podLoc), podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, podInfo.podLoc, podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::rightOf(podInfo.podLoc), podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::rightOf(Util::rightOf(podInfo.podLoc)), podInfo.goodPod));
            break;
        case NORMAL_WITH_MANY_PODS:
        {
            for (int d = 0; d < 8; ++d) {
                if ((Direction)(d) != podInfo.podLoc)
                    nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, (Direction)(d), podInfo.goodPod));
            }
            break;
        }
        default:
            break;
    }
    
    std::vector<Pod*> pods = nsegment->getPods();
    for (int i = 0; i < pods.size(); ++i)
        activePods.push_back(pods[i]);
    
    nsegment->setSectionInfo(sectionInfo);
    
	end = stepend;
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
void Tunnel::renewSegment(SectionInfo sectionInfo, PodInfo podInfo)
{
    if (segments.size() <= 0)
        return;
    
    Quaternion rot;
    rot = getNewSegmentQuaternion(sectionInfo.tunnelDir, sectionInfo.tunnelDirAngle);
    
    Vector3 forward = rot * globals.tunnelReferenceForward;
    Vector3 stepend = end + forward * (segmentDepth + globals.tunnelSegmentBuffer);
    
	TunnelSlice *nsegment = segments.front();
    
    if (nsegment->getType() == NORMAL_WITH_ONE_POD && activePods.size() > 0)
        activePods.pop_front();
    
    nsegment->rejuvenate(segmentCounter, sectionInfo.tunnelType, (end + stepend) / 2, rot, segmentWidth, segmentDepth, determineMaterial(), sidesUsed);
    ++segmentCounter;
    
    switch (sectionInfo.tunnelType)
    {
        case NORMAL_WITH_ONE_POD:
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, podInfo.podLoc, podInfo.goodPod));
            break;
        case NORMAL_WITH_THREE_PODS:
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::leftOf(podInfo.podLoc), podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, podInfo.podLoc, podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::rightOf(podInfo.podLoc), podInfo.goodPod));
            break;
        case NORMAL_WITH_FIVE_PODS:
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::leftOf(Util::leftOf(podInfo.podLoc)), podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::leftOf(podInfo.podLoc), podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, podInfo.podLoc, podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::rightOf(podInfo.podLoc), podInfo.goodPod));
            nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, Util::rightOf(Util::rightOf(podInfo.podLoc)), podInfo.goodPod));
            break;
        case NORMAL_WITH_MANY_PODS:
        {
            for (int d = 0; d < 8; ++d) {
                if ((Direction)(d) != podInfo.podLoc)
                    nsegment->addPod(PodInfo(podInfo.podSignal, podInfo.podColor, podInfo.podShape, podInfo.podSound, (Direction)(d), podInfo.goodPod));
            }
            break;
        }
        default:
            break;
    }

    
    std::vector<Pod*> pods = nsegment->getPods();
    for (int i = 0; i < pods.size(); ++i)
        activePods.push_back(pods[i]);
    
    nsegment->setSectionInfo(sectionInfo);
    
    end = stepend;
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
    if (newSection.tunnelType >= CHECKPOINT_PASS) {
        
        for (int i = 0; i < sectionSize; ++i)
            addSegment(newSection,
                       PodInfo(POD_SIGNAL_UNKNOWN, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_UNKNOWN, NO_DIRECTION, false));
        return;
    }
    for (int i = 0; i < sectionSize; ++i)
    {
        if (newSection.tunnelType != NORMAL_BLANK)
            ++renewalPodCounter;
        if (renewalPodCounter >= podSegmentSize && (spawnLimit < 0 || spawnIndex < spawnLimit))
        {
            PodInfo newPodInfo = getNextPodInfo(UNKNOWN);
            ++spawnIndex;
            
            addSegment(newSection, newPodInfo);
            renewalPodCounter = 0;
        
            if (mode == GAME_TIMED) upgradeControl();
        }
        else
        {
            addSegment(SectionInfo(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle),
                       PodInfo(POD_SIGNAL_UNKNOWN, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_UNKNOWN, NO_DIRECTION, false));
        }
    }
    sections.push_back(newSection);
}

void Tunnel::renewSection(SectionInfo newSection)
{
    if (newSection.tunnelType >= CHECKPOINT_PASS) {
        
        for (int i = 0; i < sectionSize; ++i)
            renewSegment(newSection,
                       PodInfo(POD_SIGNAL_UNKNOWN, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_UNKNOWN, NO_DIRECTION, false));
        return;
    }
    for (int i = 0; i < sectionSize; ++i)
    {
        if (newSection.tunnelType != NORMAL_BLANK)
            ++renewalPodCounter;
        if (renewalPodCounter >= podSegmentSize && (spawnLimit < 0 || spawnIndex < spawnLimit))
        {
            PodInfo newPodInfo = getNextPodInfo(UNKNOWN);
            ++spawnIndex;
            
            renewSegment(newSection, newPodInfo);
            renewalPodCounter = 0;
            
            if (mode == GAME_TIMED) upgradeControl();
        }
        else
        {
            renewSegment(SectionInfo(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle),
                       PodInfo(POD_SIGNAL_UNKNOWN, POD_COLOR_UNKNOWN, POD_SHAPE_UNKNOWN, POD_SOUND_UNKNOWN, NO_DIRECTION, false));
        }
    }
    sections.push_back(newSection);
}

bool Tunnel::updateIterators(Vector3 checkPos)
{
	if (current == segments.end())
		return false;

    previous = current;
    tLeftPrevious = tLeftCurrent;
    double t = (*current)->getT(checkPos);
    while (t > 1)
    {
        // Update the pod type index if we have just passed a segment with a pod
        ++current;
        if ((*current)->getPods().size() > 0)
        {
            ++podIndex;
        }
        
        if (!done)
            ++renewalSectionCounter;
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

void Tunnel::constructTunnel(int size, Quaternion q, bool pregenPods)
{
    this->endRot = q;
    
    // Pregenerate some pods
    if (nback != 0 && pregenPods)
    {
        // The following uses a binning algorithm originated from the famous pigeonhole principle
        
        // How many good pods do we want?
        const int TOTAL_SIGNALS = mode == GAME_TIMED ? 60 : globals.stageTotalSignals;
        const int TOTAL_ITEMS = mode == GAME_TIMED ? 20 : globals.stageTotalTargets + Util::randRangeInt(-2, 2);
        const int BIN_SIZE = 4;
        
        // How many bins do we need
        const int NUM_BINS = TOTAL_SIGNALS % BIN_SIZE ?
            TOTAL_SIGNALS / BIN_SIZE + 1 :
            TOTAL_SIGNALS / BIN_SIZE;
        
        const int NUM_ASSIGNED_PER_BIN_DEFAULT = TOTAL_ITEMS / NUM_BINS;
        
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
        std::vector<bool> signalTargets(TOTAL_SIGNALS, false);
        for (int n = 0; n < binItemCounts.size(); ++n)
        {
            int signalStartIndex = n * BIN_SIZE;
            
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
        
        int podInd = 0;
        for (int i = 0; i < nback; ++i)
        {
            getNextPodInfoAt(BAD_TARGET, podInd);
            ++podInd;
        }
        
        // Assign random signals.
        for (int i = 0; i < signalTargets.size(); ++i)
        {
            getNextPodInfoAt(signalTargets[i] ? GOOD_TARGET : BAD_TARGET, podInd);
            ++podInd;
        }
        spawnLimit = nback + signalTargets.size();
        numTargets = TOTAL_ITEMS;
        
        for (int i = 0; i < types.size(); ++i)
            std::cout << types[i].goodPod;
        std::cout << "\n";
        for (int i = 0; i < types.size(); ++i)
        {
            switch (types[i].podSignal)
            {
                case POD_SIGNAL_1:
                    std::cout << 'B';
                    break;
                case POD_SIGNAL_2:
                    std::cout << 'G';
                    break;
                case POD_SIGNAL_3:
                    std::cout << 'R';
                    break;
                case POD_SIGNAL_4:
                    std::cout << 'Y';
                    break;
                default:
                    std::cout << 'U';
            }
        }
        std::cout << "\n";
    }
    
    // Output strands
    for (int i = 0; i < globals.initiationSections; ++i) {
        SectionInfo info = SectionInfo(NORMAL_BLANK, NO_DIRECTION, 0);
        addSection(info);
    }
    for (int i = 0; i < size - globals.initiationSections; ++i) {
        SectionInfo info = getNextSectionInfo();
        addSection(info);
    }
}

std::vector<Pod *> Tunnel::findPodCollisions(SceneNode *ent)
{
    std::vector<Pod*> collisions;
    std::list<TunnelSlice *>::iterator it = segments.begin();
    for (it = current; it != segments.end(); ++it) {
        
        //Fog3 Size Hardcoded... FIX!
        double entDist = ent->getPosition().z + ((5.0 * 5.0)/2);
        
        Pod* pod = NULL;
        if ( (*it)->getPods().size() > 0 )
            pod = (*it)->getPods()[0];
        
        if (pod && entDist < pod->getHead()->getPosition().z) {
            collisions.push_back(pod);
        }
        
        if (collisions.size() > 0) return collisions;
    }
    return collisions;
}

void Tunnel::update(Player* player, Hud* hud, double elapsed)
{
    // Determine whether a stage has completed
    if (!isDone())
    {
        totalElapsed += elapsed;
        if (getMode() == GAME_NORMAL)
        {
            if (spawnLimit >= 0)
            {
                if (podIndex >= spawnLimit)
                    setDone(player->getEvaluation(this));
            }
            else
            {
                if (player->getHP() >= globals.HPPositiveLimit ||
                    player->getHP() <= globals.HPNegativeLimit)
                    setDone(player->getEvaluation(this));
            }
        }
        else
        {
            if (getSignalsLeft() <= 0)
                setDone(PASS);
            if (getTotalElapsed() > globals.timedRunTimer)
                setDone(EVEN);
        }
    }
    
    // For timed trials, increase panels based on progress
    /*
    if (mode == GAME_TIMED)
    {
         // Measure by distance
        int currentControlLevel = control;
        if ((currentControlLevel == 1 && player->getTotalDistanceTraveled() >= globals.timedRunControlUpDist1) ||
            (currentControlLevel == 2 && player->getTotalDistanceTraveled() >= globals.timedRunControlUpDist2) ||
            (currentControlLevel == 3 && player->getTotalDistanceTraveled() >= globals.timedRunControlUpDist3))
        {
            currentControlLevel++;
            setNewControl(currentControlLevel);
        }
    }
    */
    
    // Animate Pod Growing outwards or Growing inwards
    const double GROWTH_SPEED = player->getCamSpeed() / 10.0;
    TunnelSlice* nextSliceM = getNext(globals.podAppearance);
    if (nextSliceM) {
        if (!isDone())
            nextSliceM->updateGrowth(GROWTH_SPEED * elapsed);
        else
            nextSliceM->updateGrowth(-GROWTH_SPEED * elapsed);
    }
    nextSliceM = getNext(globals.podAppearance + 1);
    if (nextSliceM) {
        if (!isDone())
            nextSliceM->updateGrowth(GROWTH_SPEED * elapsed);
        else
            nextSliceM->updateGrowth(-GROWTH_SPEED * elapsed);
    }
    
    // Check to see if we need to recycle tunnel segments
    if (updateIterators(player->getCamPos())) {
        renewIfNecessary();
        
        // Set new camera slerp goal
        player->saveCam();
        
        TunnelSlice* nextSlice1 = getNext(1);
        if (nextSlice1)
        {
            player->setDesireRot(nextSlice1->getQuaternion());
        }
        else {
            cleanup = true; // Flag to notify main app this tunnel is completely over
        }
        
        // Show Pod Color and Play Sound
        TunnelSlice* nextSliceN = getNext(globals.podAppearance);
        if (nextSliceN && !isDone())
        {
            std::vector<Pod*> pods = nextSliceN->getPods();
            for (int i = 0; i < pods.size(); ++i) {
                pods[i]->revealPod();
            player->playPodSound(pods[i]->getPodSound());
                //activePods.push_back(pods[i]);
                //pods[i]->setRotateSpeed(Vector3(5.0, 5.0, 5.0));
            }
        }
    }
    std::list<Pod *>::iterator it;
    for (it = activePods.begin(); it != activePods.end(); ++it)
    {
        (*it)->update(elapsed);
    }
    if (history) history->update(elapsed);
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
    
    if (history) delete history;
}
