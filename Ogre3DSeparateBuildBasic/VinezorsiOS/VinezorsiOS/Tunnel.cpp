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

const float infinityDepth = 1024;

static int tunnelID = 0;

Tunnel::Tunnel()
:  player(NULL), hud(NULL), parentNode(NULL), mainTunnelNode(NULL), start(), end(), segments(), tLeftPrevious(0.0), tLeftCurrent(0.0), previous(), current(), tLeftOffsetPrevious(0.0), tLeftOffsetCurrent(0.0), previousOffset(), currentOffset(), segmentCounter(0), segmentWidth(0.0), segmentDepth(0.0), sections(), types(), targets(), sectionSize(0), podSegmentSize(0), distractorSegmentSize(0), spawnIndex(0), spawnCombo(0), podIndex(0), sectionIndex(0), renewalSectionCounter(0), renewalPodCounter(0), renewalDistractorCounter(0), spawnLimit(-1), numTargets(0), activePods(), stageNo(0), mode(GAME_PROFICIENCY), phase(' '), totalElapsed(0.0), timePenalty(0.0), nback(1), control(0), history(NULL), basis(NO_DIRECTION), sidesUsed(), materialNames(), eval(EVEN), signalTypes(), navPhase(0), catchupPhase(0), navCheckpoint(0), navLevels(), propagateCounters(), guide(NO_DIRECTION), done(false), cleanup(false)
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = true;
}

Tunnel::Tunnel(Ogre::SceneNode* parentNode, Vector3 start, Quaternion rot, float segmentWidth, float segmentDepth, int segmentMinAngleTurn, int segmentMaxAngleTurn, int stageNo, GameMode mode, char phase, int nback, int control, Direction sloc, int sectionSize, int podSegmentSize, int distractorSegmentSize, const std::vector<std::vector<PodInfo> > & signalTypes)
:  player(NULL), hud(NULL), parentNode(parentNode), mainTunnelNode(NULL), start(start), end(start), segments(), tLeftPrevious(0.0), tLeftCurrent(0.0), previous(), current(), tLeftOffsetPrevious(0.0), tLeftOffsetCurrent(0.0), previousOffset(), currentOffset(), segmentCounter(0), segmentWidth(segmentWidth), segmentDepth(segmentDepth), segmentMinAngleTurn(segmentMinAngleTurn), segmentMaxAngleTurn(segmentMaxAngleTurn), endRot(rot), sections(), types(), targets(), sectionSize(sectionSize), podSegmentSize(podSegmentSize), distractorSegmentSize(distractorSegmentSize), sectionIndex(0), spawnIndex(0), spawnCombo(0), podIndex(0), renewalSectionCounter(0), renewalPodCounter(0), renewalDistractorCounter(0), spawnLimit(-1), numTargets(0), activePods(), stageNo(stageNo), mode(mode), phase(phase), totalElapsed(0.0), timePenalty(0.0), nback(nback), control(control), history(NULL), basis(sloc), sidesUsed(), materialNames(), eval(EVEN), signalTypes(signalTypes), navPhase(0), catchupPhase(0), navCheckpoint(0), navLevels(), propagateCounters(), guide(NO_DIRECTION), done(false), cleanup(false)
{
    mainTunnelNode = parentNode->createChildSceneNode("mainTunnelNode" + Util::toStringInt(tunnelID));
    //history = new History(OgreFramework::getSingletonPtr()->m_pSceneMgrSide, nback);
	current = segments.end();
    
    globals.stageTime = 140 + 3 * nback + 2;
    
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

int Tunnel::getTotalCollections() const
{
    return globals.stageTotalCollections;
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
    /*
     SectionInfo checkInfo = getCurrent()->getSectionInfo();
     
     bool panels[NUM_DIRECTIONS];
     // ALwyas use less panels
     if (Util::getNumSides(checkInfo.sidesUsed) < Util::getNumSides(segmentInfo.sidesUsed))
     Util::copySides(panels, checkInfo.sidesUsed);
     else
     Util::copySides(panels, segmentInfo.sidesUsed);
     
     bool set1[NUM_DIRECTIONS];
     bool set2[NUM_DIRECTIONS];
     
     getSpanFrom(2, player->getVineDest(), panels, set1);
     getSpanFrom(2, guide, panels, set2);
     
     // Intersection
     bool set3[NUM_DIRECTIONS];
     for (int i = 0; i < NUM_DIRECTIONS; ++i)
     {
     set3[i] = (set1[i] && set2[i]);
     }
     if (Util::getNumSides(set3) > 0)
     guide = Util::randDirection(set3);
     else
     guide = Util::randDirection(set1);
     return guide;
     */
    
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
    return globals.stageTime - getTotalElapsed() - getTimePenalty();
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
        case 'H':
            materialNames.push_back("General/WallBindingA");
            break;
        default:
            materialNames.push_back("General/Wall1");
            break;
    }
    /*
    if (nback <= 1)
    {
        materialNames.push_back("General/Wall1");
        return;
    }

    if (globals.setWallPanelTexture)
    {
        switch ((nback - 2) % 4)
        {
            case 0:
                materialNames.push_back("General/WallCartoon2");
                break;
            case 1:
                materialNames.push_back("General/WallCartoon3");
                break;
            case 2:
                materialNames.push_back("General/WallCartoon4");
                break;
            case 3:
                materialNames.push_back("General/WallCartoon5");
                break;
            default:
                materialNames.push_back("General/Wall1");
                break;
        }
    }
    else
    {
        switch ((nback - 2) % 6)
        {
            case 0:
            {
                std::string materialName;
                int r = rand() % 2;
                materialName = "General/Wall2";
                materialName += char('a' + r);
                materialNames.push_back(materialName);
                break;
            }
            case 1:
            {
                std::string materialName;
                int r = rand() % 2;
                materialName = "General/Wall3";
                materialName += char('a' + r);
                materialNames.push_back(materialName);
                break;
            }
            case 2:
            {
                std::string materialName;
                int r = rand() % 2;
                materialName = "General/Wall4";
                materialName += char('a' + r);
                materialNames.push_back(materialName);
                break;
            }
            case 3:
            {
                std::string materialName;
                int r = rand() % 3;
                materialName = "General/Wall5";
                materialName += char('a' + r);
                materialNames.push_back(materialName);
                break;
            }
            case 4:
            {
                std::string materialName;
            
                int r = rand() % 3;
                materialName = "General/Wall6";
                materialName += char('a' + r);
                if (materialName == "General/Wall6a")
                {
                    materialNames.push_back(materialName + "1");
                    materialNames.push_back(materialName + "2");
                    materialNames.push_back(materialName + "3");
                }
                else if (materialName == "General/Wall6b")
                {
                    materialNames.push_back(materialName + "1");
                    materialNames.push_back(materialName + "2");
                    materialNames.push_back(materialName + "3");
                }
                else
                    materialNames.push_back(materialName);
                break;
            }
            case 5:
            {
                std::string materialName;
                int r = rand() % 2;
                materialName = "General/Wall7";
                materialName += char('a' + r);
                materialNames.push_back(materialName);
                break;
            }
            default:
                materialNames.push_back("General/Wall1");
                break;
        }
    }
     */
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

void Tunnel::checkIfDone()
{
    // Determine whether a stage has completed
    if (!isDone())
    {
        if (getMode() == GAME_PROFICIENCY)
        {
            if (player->getHP() >= globals.HPPositiveLimit)
                setDone(PASS);
            else if (spawnLimit > 0 && getSignalsLeft() <= 0)
                setDone(EVEN);
            else if (globals.stageTime > 0 && getTimeLeft() <= 0)
                setDone(EVEN);
        }
        else if (getMode() == GAME_TIMED)
        {
            if (spawnLimit > 0 && getSignalsLeft() <= 0)
                setDone(PASS);
            else if (globals.stageTime > 0 && getTimeLeft() <= 0)
                setDone(EVEN);
        }
        else if (getMode() == GAME_NAVIGATION)
        {
            if (globals.stageTime > 0 && getTimeLeft() <= 0)
                setDone(PASS);
            else if (player->getHP() <= globals.HPNegativeLimit)
                setDone(EVEN);
        }
        else //if (getMode() == GAME_TEACHING || getMode() == GAME_RECESS)
        {
            if (player->getNumCorrectTotal() >= getNumTargets())
                setDone(PASS);
            else if (globals.stageTime > 0 && getTimeLeft() <= 0)
                setDone(EVEN);
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
    if (eval == PASS)
        player->incrementNumStagesWon();
    done = true;
}

void Tunnel::setSpawnCombo(int level)
{
    spawnCombo = level;
}

// Used in Time/Speed Trial and is called everytime a new section is added
void Tunnel::upgradeControl()
{
    if (getMode() == GAME_TEACHING)
    {
        float percentComplete = static_cast<float>(player->getNumCorrectTotal()) / getNumTargets();
        if (percentComplete >= 0.50 && navPhase + 1 < navLevels.size())
        {
            ++navPhase;
            
            // No need for propagate counters, we want the distractors asap
            setNewControl(navLevels[navPhase].control);
            globals.stageTotalDistractorsMin = navLevels[navPhase].obstacles;
            globals.stageTotalDistractorsMax = navLevels[navPhase].obstacles;
        }
    }
    else if (getMode() != GAME_PROFICIENCY)
    {
        ++navCheckpoint;
        if (navCheckpoint >= globals.tunnelSectionsPerNavigationUpgrade)
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
    /*
    else if (getMode() == GAME_TIMED)
    {
        // Measure the distance traveled by the number of signals passed, if enough passed, upgrade the controls
        //
        // Assumed to be 4 phases in these stages
        if ((navPhase == 0 && getSignalsLeft() <= spawnLimit - globals.stageTimeThreshold1 - nback) ||
            (navPhase == 1 && getSignalsLeft() <= spawnLimit - globals.stageTimeThreshold2 - nback) ||
            (navPhase == 2 && getSignalsLeft() <= spawnLimit - globals.stageTimeThreshold3 - nback))
        {
            navPhase++;
            setNewControl(navLevels[navPhase].control);
        }
    }
     */
}

void Tunnel::addToTimePenalty(float value)
{
    timePenalty += value;
}

void Tunnel::setCleaning(bool value)
{
    cleanup = value;
}

bool Tunnel::needsCleaning() const
{
    return cleanup;
}

void Tunnel::setNewControl(int control)
{
    this->control = control;
    //SectionInfo info = SectionInfo(CHECKPOINT_PASS, NO_DIRECTION, 0, sidesUsed);
    //addSegment(info);
    Util::setSides(sidesUsed, control, basis);
}

void Tunnel::setNavigationLevels()
{
    navLevels.clear();
    int playerNavLevel = player->getSkillLevel().navigation;
    if (getMode() == GAME_PROFICIENCY)
    {
        int tunnelNavLevel = playerNavLevel + Util::randRangeInt(-2, 0);
        tunnelNavLevel = Util::clamp(tunnelNavLevel, 0, player->getSkillLevel().getNavLimit());
        navLevels.push_back(globals.navMap[tunnelNavLevel]);
    }
    else if (getMode() == GAME_TIMED)
    {
        if (playerNavLevel < 2) playerNavLevel = 2;
        if (globals.tunnelSectionsPerNavigationUpgrade > 0)
        {
            int tunnelNavLevel;
            int dn = -2;
            for (int i = 0; i < globals.numTimePhases; ++i)
            {
                tunnelNavLevel = playerNavLevel + dn;
                tunnelNavLevel = Util::clamp(tunnelNavLevel, 0, player->getSkillLevel().getNavLimit());
                navLevels.push_back(globals.navMap[tunnelNavLevel]);
                dn += 1;
            }
        }
        else
        {
            // Old time trial mode where only panels grow from 3 to 8
            navLevels.push_back(NavigationLevel(0, 1, 0));
            navLevels.push_back(NavigationLevel(1, 2, 0));
            navLevels.push_back(NavigationLevel(2, 3, 0));
            navLevels.push_back(NavigationLevel(3, 4, 0));
        }
    }
    else if (getMode() == GAME_NAVIGATION || getMode() == GAME_RECESS)
    {
        if (playerNavLevel < 2) playerNavLevel = 2;
        if (globals.tunnelSectionsPerNavigationUpgrade > 0)
        {
            int tunnelNavLevel;
            int dn = -2;
            for (int i = 0; i < 20; ++i)
            {
                tunnelNavLevel = playerNavLevel + dn;
                tunnelNavLevel = Util::clamp(tunnelNavLevel, 0, player->getSkillLevel().getNavLimit());
                navLevels.push_back(globals.navMap[tunnelNavLevel]);
                dn += 1;
            }
        }
    }
    else //if (getMode() == GAME_TEACHING)
    {
        navLevels.push_back(NavigationLevel(0, 1, 0));
        navLevels.push_back(NavigationLevel(0, 1, 1));
    }
    std::cout << "Nav Levels: " << std::endl;
    for (int i = 0; i < navLevels.size(); ++i)
        std::cout << navLevels[i].level << " " << navLevels[i].control << " " << navLevels[i].obstacles << std::endl;
    std::cout << "Nav Limit: " << player->getSkillLevel().getNavLimit() << std::endl;
    
    navPhase = 0;
    catchupPhase = 0;
    navCheckpoint = 0;
    setNewControl(navLevels[navPhase].control);
    globals.stageTotalDistractorsMin = navLevels[navPhase].obstacles;
    globals.stageTotalDistractorsMax = navLevels[navPhase].obstacles;
}

void Tunnel::setNavigationLevels(const std::vector<NavigationLevel> & preset)
{
    navLevels = preset;
    navPhase = 0;
    catchupPhase = 0;
    navCheckpoint = 0;
    setNewControl(navLevels[navPhase].control);
    globals.stageTotalDistractorsMin = navLevels[navPhase].obstacles;
    globals.stageTotalDistractorsMax = navLevels[navPhase].obstacles;
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
    if (getMode() == GAME_NAVIGATION || getMode() == GAME_TEACHING || getMode() == GAME_RECESS)
    {
        ret.podSignal = POD_SIGNAL_UNKNOWN;
        ret.meshType = POD_FUEL;
        ret.podColor = POD_COLOR_PURPLE;
        ret.podSound = POD_SOUND_UNKNOWN;
        ret.podShape = POD_SHAPE_UNKNOWN;
        ret.podLoc = SOUTHEAST;
        ret.goodPod = true;
        ret.podTrigger = false;
        types.push_back(ret);
    }
    else if (nback <= 0)
    {
        Direction podLoc = SOUTHEAST;
        int signalIndex = rand() % signalTypes.size();
        
        ret = signalTypes[signalIndex][rand() % signalTypes[signalIndex].size()];
        ret.podLoc = podLoc;
        ret.goodPod = true;
        ret.podTrigger = false;
        types.push_back(ret);
    }
    else // If not spawn one now
    {
        Direction podLoc = SOUTHEAST;
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
            for (int i = 0; i < signalTypes.size(); ++i)
                if ((types.size() <= 0 || (PodSignal)i != types[index - 1].podSignal || (signalTypes.size() < 3 || nback > 2)) && // Prevent repeated signals for nbacks less than 2 unless theres not enough supported signals
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
        else //if (setting != UNKNOWN) // C.P. Removed Redundancy if check
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
    
        ret = signalTypes[final][rand() % signalTypes[final].size()];
        
        ret.podLoc = podLoc;
        // Determine NBack of next pod is good
        ret.goodPod = (nback <= 0 || (types.size() >= nback && types[index - nback].podSignal == final));
        ret.podTrigger = false;
        
        types.push_back(ret);
    }
    return ret;
}

std::vector<PodInfo> Tunnel::getNextDistractorInfo(SectionInfo segment, PodInfo signal)
{
    std::vector<PodInfo> ret;
    if (signal.podLoc != NO_DIRECTION)
        ret.push_back(signal);
    
    // Possible that signal loc and safe loc are the same, however, for this current build
    // distractors and signals do not appear on same segment. Otherwise this needs to check
    Direction safeLoc = getRandPossibleDirection(segment, false);
    
    std::vector<int> availDirs;
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        if (segment.sidesUsed[i] &&
            (Direction)(i) != signal.podLoc &&
            (Direction)(i) != safeLoc) availDirs.push_back(i);
    
    int count = Util::randRangeInt(globals.stageTotalDistractorsMin, globals.stageTotalDistractorsMax);
    if (count > 0)
        globals.numSegmentsWithObstacles++; // Used for statistics
    while (count > 0 && availDirs.size() > 0)
    {
        int rind = rand() % availDirs.size();
        PodInfo newDistractor = PodInfo(POD_SIGNAL_UNKNOWN, POD_HAZARD, POD_COLOR_UNKNOWN, POD_SHAPE_SPHERE, POD_SOUND_UNKNOWN, (Direction)availDirs[rind], false, true, false);
        ret.push_back(newDistractor);
        
        availDirs[rind] = availDirs[availDirs.size() - 1];
        availDirs.pop_back();
        --count;
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
        
        for (int i = 0; i < sectionSize; ++i)
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
                    if (getMode() == GAME_NAVIGATION) player->setHP(globals.startingHP);
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
    this->hud = NULL;
}

void Tunnel::link(Player* player, Hud* hud)
{
    this->player = player;
    this->hud = hud;
}

void Tunnel::presetTargets(int level)
{
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
    std::vector<bool> signalTargets(TOTAL_SIGNALS + nback, false);
    for (int n = 0; n < binItemCounts.size(); ++n)
    {
        int signalStartIndex = nback + n * BIN_SIZE;
        
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

void Tunnel::constructTunnel(int size) // Removed preset param C.P.
{
    // Pregenerate some targets
    if (nback > 0 && getMode() != GAME_NAVIGATION && getMode() != GAME_TEACHING && getMode() != GAME_RECESS)
    {
        targets = std::vector<TargetInfo>(globals.stageTotalSignals + nback);
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
        /*
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
        */
    }
    else
    {
        numTargets = globals.stageTotalCollections;
    }
    
    determineMaterial();
    
    // Output strands
    for (int i = 0; i < globals.initiationSections; ++i) {
        SectionInfo info = SectionInfo(BLANK, NO_DIRECTION, 0, endRot, sidesUsed);
        addSection(info);
    }
    
    renewalPodCounter = -globals.initialSegmentsFirstPod;
    renewalDistractorCounter = -globals.initialSegmentsFirstDistractors;
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
            pods[j]->uncloakPod();
        ++it;
    }
}

std::vector<Pod *> Tunnel::findPodCollisions(SceneNode *ent)
{
    std::vector<Pod*> collisions;
    std::list<TunnelSlice *>::iterator it = segments.begin();
    for (it = current; it != segments.end(); ++it) {
        
        //Fog3 Size Hardcoded... FIX!
        float entDist = ent->getPosition().z + ((5.0 * 5.0)/2);
        
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

void Tunnel::update(float elapsed)
{
    totalElapsed += elapsed;
    // Animate Pod Growing outwards or Growing inwards
    const float GROWTH_SPEED = 1;
    for (int i = 0; i <= globals.podAppearance + 1; ++i)
    {
        TunnelSlice* nextSliceM = getNext(i);
        if (nextSliceM) {
            if (!isDone() && !nextSliceM->isGrowthFlip())
                nextSliceM->updateGrowth(GROWTH_SPEED * elapsed);
            else
                nextSliceM->updateGrowth(-GROWTH_SPEED * elapsed);
        }
    }
    
    // Check to see if we need to recycle tunnel segments
    if (updateIterators(player->getCamPos())) {
        renewIfNecessary();
        
        // Set new camera slerp goal
        player->saveCam();
        
        TunnelSlice* nextSlice1 = getNext(1);
        if (nextSlice1) player->setDesireRot(nextSlice1->getQuaternion());
        
        // Flag to notify main app this tunnel is completely over
        cleanup = !nextSlice1;
        
        // Show Pod Color and Play Sound
        TunnelSlice* nextSliceN = getNext(globals.podAppearance);
        if (nextSliceN && !nextSliceN->isGrowthFlip() && !isDone())
        {
            nextSliceN->setGrowthFlip();
            std::vector<Pod*> pods = nextSliceN->getPods();
            for (int i = 0; i < pods.size(); ++i) {
                pods[i]->uncloakPod();
            player->playPodSound(pods[i]->getPodSound());
            }
        }
        nextSliceN = getNext(globals.podAppearance + 1);
        if (nextSliceN && !isDone())
        {
            // Determine whether should be added to this segment
            ++renewalPodCounter;
            ++renewalDistractorCounter;
            PodInfo signalInfo;
            std::vector<PodInfo> podInfos;
            if (renewalPodCounter >= podSegmentSize && (spawnLimit < 0 || spawnIndex < spawnLimit))
            {
                signalInfo = getNextPodInfo(nextSliceN->getSectionInfo());
                renewalPodCounter = 0;
            }
            if (renewalDistractorCounter >= distractorSegmentSize && (spawnLimit < 0 || spawnIndex < spawnLimit))
            {
                podInfos = getNextDistractorInfo(nextSliceN->getSectionInfo(), signalInfo);
                renewalDistractorCounter = 0;
            }
            else if (signalInfo.podLoc != NO_DIRECTION) podInfos.push_back(signalInfo);
            setPods(nextSliceN, podInfos);
        }
    }
    
    std::list<Pod *>::iterator it;
    for (it = activePods.begin(); it != activePods.end(); ++it)
    {
        (*it)->update(elapsed);
    }
    
    checkIfDone();

    if (history) history->update(elapsed);
    
    cleanup = !cleanup ? (isDone() && player->getAnimationTimer() <= 0.0) : cleanup;
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
