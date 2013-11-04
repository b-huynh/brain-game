//
//  Tunnel.cpp
//  Testing
//
//  Created by Calvin Phung on 9/10/13.
//
//

#include "Tunnel.h"

#include <cstdlib>

using namespace std;

extern Util::ConfigGlobal globals;

const double infinityDepth = 1024;

static int tunnelID = 0;

Tunnel::Tunnel()
: parentNode(NULL), mainTunnelNode(NULL), start(), end(), segments(), current(), segmentWidth(0.0), segmentDepth(0.0), sections(), types(), sectionSize(0), podSegmentSize(0), podIndex(0), sectionIndex(0), renewalSectionCounter(0), renewalPodCounter(0), mode(GAME_NORMAL), totalElapsed(0.0), nback(1), control(0), history(NULL), basis(NO_DIRECTION), sidesUsed(), done(false)
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = true;
}

Tunnel::Tunnel(Ogre::SceneNode* parentNode, Vector3 start, double segmentWidth, double segmentDepth, int segmentMinAngleTurn, int segmentMaxAngleTurn, GameMode mode, int nback, int control, Direction sloc, int sectionSize, int podSegmentSize)
: parentNode(parentNode), mainTunnelNode(NULL), start(start), end(start), segments(), current(), segmentWidth(segmentWidth), segmentDepth(segmentDepth), segmentMinAngleTurn(segmentMinAngleTurn), segmentMaxAngleTurn(segmentMaxAngleTurn), sections(), types(), sectionSize(sectionSize), podSegmentSize(podSegmentSize), sectionIndex(0), podIndex(0), renewalSectionCounter(0), renewalPodCounter(0), mode(mode), totalElapsed(0.0), nback(nback), control(control), history(NULL), basis(sloc), sidesUsed(), done(false)
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

// An estimate of which Segment contains the position with a certain linear interpolated offset
TunnelSlice* Tunnel::findSliceFromCurrent(Vector3 pos, double tOffset, double & tLeft) const
{
    std::list<TunnelSlice*>::iterator it = current;
    
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

TunnelSlice* Tunnel::getCurrent() const
{
	if (current == segments.end())
		return NULL;
	return *current;
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

PodType Tunnel::getNBackTest(int ind) const
{
    if (ind - nback >= 0 && types[ind - nback].podType == types[ind].podType)
        return types[ind].podType;
    else
        return POD_NONE;
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
    switch (nback)
    {
        case 2:
            return "General/Wall1";
        case 3:
            return "General/Wall2";
        case 4:
            return "General/Wall3";
        case 5:
            return "General/Wall4";
        case 6:
            return "General/Wall5";
        case 7:
            return "General/Wall6";
        case 8:
            return "General/Wall7";
        case 9:
            return "General/Wall8";
        default:
            return "General/Wall0";
    }
}


bool Tunnel::isDone() const
{
    return done;
}

void Tunnel::setDone(bool value)
{
    done = value;
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
    TunnelType segmentType = NORMAL_WITH_ONE_POD;
    Direction segmentDir = Util::randDirection();
    int segmentTurnAngle = Util::randRangeInt(segmentMinAngleTurn, segmentMaxAngleTurn);
    
    return SectionInfo(segmentType, segmentDir, segmentTurnAngle);
}

PodInfo Tunnel::getNextPodInfo(SectionInfo & sectionInfo) const
{
    Direction podLoc = Util::randDirection(sidesUsed);
    
    /*
    PodType podType = (PodType)(rand() % 4);
    // Do a reroll if pod is the same type as last one
    if (nback != 1 && types.size() > 0 && types[types.size() - 1].podType == podType)
        podType = (PodType)(rand() % 4);
    else if (nback == 1 && types.size() > 1 && // Nback == 1 has a different case
             types[types.size() - 1].podType == podType && types[types.size() - 2].podType == podType)
        podType = (PodType)(rand() % 4);
    */
    
    // Next pod has a 1/3 chance to be a good pod
    PodType podType;
    if (types.size() >= nback && nback > 0)
    {
        podType = types[types.size() - nback].podType;
        int r = rand() % 3;
        if (r > 0)
        {
            std::vector<PodType> candidates;
            for (int i = 0; i < 4; ++i)
                if (i != podType)
                    candidates.push_back((PodType)i);
            podType = candidates[rand() % candidates.size()];
        }
    }
    else
        podType = (PodType)(rand() % 4);
    
    // The following deals with throwing special settings for certain NBacks occuring before this one
    PodType NBack = getNBackTest(types.size() - 1);
    switch (NBack)
    {
        case POD_PINK:
            //sectionInfo.tunnelType = NORMAL_WITH_ONE_POD;
            break;
        case POD_BLUE:
            //sectionInfo.tunnelType = NORMAL_WITH_ONE_POD;
            break;
        case POD_GREEN:
            //sectionInfo.tunnelType = NORMAL_WITH_ONE_POD;
            break;
        case POD_YELLOW:
            //sectionInfo.tunnelType = NORMAL_WITH_ONE_POD;
            
            // Guarantee an nback if a yellow nback appears
            //podType = types[types.size() - nback].podType;
            //podLoc = types[types.size() - 1].podLoc;
            
            break;
        case POD_BLACK:
            //sectionInfo.tunnelType = NORMAL_WITH_MANY_PODS;
            //podLoc = types[types.size() - 1].podLoc;
            break;
        case POD_NONE:
            // No NBack at the end
            //sectionInfo.tunnelType = NORMAL_WITH_ONE_POD;
            break;
    }
    
    // Determine NBack of next pod is good
    bool good = false;
    if (nback <= 0 || (types.size() >= nback && types[types.size() - nback].podType == podType))
        good = true;
    return PodInfo(podType, podLoc, good);
}

void Tunnel::addSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc, bool podGood)
{
    Quaternion rot;
    
    rot = getNewSegmentQuaternion(segmentTurn, turnDegrees);
    
    Vector3 forward = rot * globals.tunnelReferenceForward;
    Vector3 stepend = end + forward * (segmentDepth + globals.tunnelSegmentBuffer);
	TunnelSlice* nsegment = new TunnelSlice(mainTunnelNode, segmentType, (end + stepend) / 2, rot, segmentWidth, segmentDepth, determineMaterial(), sidesUsed);
    
    switch (segmentType)
    {
        case NORMAL_WITH_ONE_POD:
            nsegment->addPod(podLoc, podType);
            break;
        case NORMAL_WITH_THREE_PODS:
            nsegment->addPod(Util::leftOf(podLoc), podType);
            nsegment->addPod(podLoc, podType);
            nsegment->addPod(Util::rightOf(podLoc), podType);
            break;
        case NORMAL_WITH_FIVE_PODS:
            nsegment->addPod(Util::leftOf(Util::leftOf(podLoc)), podType);
            nsegment->addPod(Util::leftOf(podLoc), podType);
            nsegment->addPod(podLoc, podType);
            nsegment->addPod(Util::rightOf(podLoc), podType);
            nsegment->addPod(Util::rightOf(Util::rightOf(podLoc)), podType);
            break;
        case NORMAL_WITH_MANY_PODS:
        {
            for (int d = 0; d < 8; ++d) {
                if ((Direction)(d) != podLoc)
                    nsegment->addPod((Direction)(d), podType);
            }
            break;
        }
        default:
            break;
    }
    nsegment->setSectionInfo(SectionInfo(segmentType, segmentTurn, turnDegrees));
    nsegment->setPodInfo(PodInfo(podType, podLoc, podGood));
    
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
void Tunnel::renewSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc, bool podGood)
{
    if (segments.size() <= 0)
        return;
    
    Quaternion rot;
    rot = getNewSegmentQuaternion(segmentTurn, turnDegrees);
    
    Vector3 forward = rot * globals.tunnelReferenceForward;
    Vector3 stepend = end + forward * (segmentDepth + globals.tunnelSegmentBuffer);
    
	TunnelSlice *nsegment = segments.front();
    nsegment->rejuvenate(segmentType, (end + stepend) / 2, rot, segmentWidth, segmentDepth, determineMaterial(), sidesUsed);
    
    switch (segmentType)
    {
        case NORMAL_WITH_ONE_POD:
            nsegment->addPod(podLoc, podType);
            break;
        case NORMAL_WITH_THREE_PODS:
            nsegment->addPod(Util::leftOf(podLoc), podType);
            nsegment->addPod(podLoc, podType);
            nsegment->addPod(Util::rightOf(podLoc), podType);
            break;
        case NORMAL_WITH_FIVE_PODS:
            nsegment->addPod(Util::leftOf(Util::leftOf(podLoc)), podType);
            nsegment->addPod(Util::leftOf(podLoc), podType);
            nsegment->addPod(podLoc, podType);
            nsegment->addPod(Util::rightOf(podLoc), podType);
            nsegment->addPod(Util::rightOf(Util::rightOf(podLoc)), podType);
            break;
        case NORMAL_WITH_MANY_PODS:
        {
            for (int d = 0; d < 8; ++d) {
                if ((Direction)(d) != podLoc)
                    nsegment->addPod((Direction)(d), podType);
            }
            break;
        }
        default:
            break;
    }
    nsegment->setSectionInfo(SectionInfo(segmentType, segmentTurn, turnDegrees));
    nsegment->setPodInfo(PodInfo(podType, podLoc, podGood));
    
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
    if (newSection.tunnelType == CHECKPOINT) {
        
        for (int i = 0; i < sectionSize; ++i)
            if (i == sectionSize - 1)
                addSegment(CHECKPOINT, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION, false);
            else
                addSegment(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION, false);
        return;
    }
    for (int i = 0; i < sectionSize; ++i)
    {
        if (newSection.tunnelType != NORMAL_BLANK)
            ++renewalPodCounter;
        if (renewalPodCounter >= podSegmentSize)
        {
            PodInfo newPod = getNextPodInfo(newSection);
            addSegment(newSection.tunnelType, newSection.tunnelDir, newSection.tunnelDirAngle, newPod.podType, newPod.podLoc, newPod.goodPod);
            renewalPodCounter = 0;
            
            types.push_back(newPod);
        }
        else
        {
            addSegment(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION, false);
        }
    }
    sections.push_back(newSection);
}

void Tunnel::renewSection(SectionInfo newSection)
{
    if (newSection.tunnelType == CHECKPOINT) {
        
        for (int i = 0; i < sectionSize; ++i)
            if (i == sectionSize - 1)
                renewSegment(CHECKPOINT, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION, false);
            else
                renewSegment(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION, false);
        return;
    }
    for (int i = 0; i < sectionSize; ++i)
    {
        if (newSection.tunnelType != NORMAL_BLANK)
            ++renewalPodCounter;
        if (renewalPodCounter >= podSegmentSize)
        {
            PodInfo newPod = getNextPodInfo(newSection);
            renewSegment(newSection.tunnelType, newSection.tunnelDir, newSection.tunnelDirAngle, newPod.podType, newPod.podLoc, newPod.goodPod);
            renewalPodCounter = 0;
            
            types.push_back(newPod);
        }
        else
        {
            renewSegment(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION, false);
        }
    }
    sections.push_back(newSection);
}

// Determines whether the current segment should be updated depending on the position compared.
// Returns true if a renew was necessary
bool Tunnel::renewIfNecessary(Vector3 checkPos)
{
	if (current == segments.end())
		return false;
    TunnelSlice* currentSlice = *current;
    Vector3 endOfSlice = currentSlice->getCenter() + (currentSlice->getForward() * (segmentDepth) / 2);
    if ((checkPos - endOfSlice).dotProduct(currentSlice->getForward()) >= 0)
	{
        // Update the pod type index if we have just passed a segment with a po
        ++current;
        if ((*current)->getType() != NORMAL_BLANK)
        {
            ++podIndex;
        }
        
        if (!done)
        {
            // Update the section index and recycle sections
            ++renewalSectionCounter;
            if (renewalSectionCounter >= globals.tunnelSegmentsBeforeRefresh)
            {
                SectionInfo info = getNextSectionInfo();
                renewSection(info);
                ++sectionIndex;
                renewalSectionCounter -= globals.tunnelSegmentsPerSection;
            }
        }
        
        //if (current != segments.end())
        //    (*current)->changeWallTexture();
        
        return true;
	}
    return false;
}

void Tunnel::constructTunnel(int size, Quaternion q)
{
    this->endRot = q;
    
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

void Tunnel::update(double elapsed)
{
    totalElapsed += elapsed;
    history->update(elapsed);
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
    
    delete history;
}
