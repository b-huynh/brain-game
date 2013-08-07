#include "Tunnel.h"

#include <cstdlib>
#include <iostream>

const Number infinityDepth = 1024;

Tunnel::Tunnel()
: scene(NULL), start(), end(), segments(), current(), segmentWidth(0.0), segmentDepth(0.0), sections(), types(), sectionSize(0), podSegmentSize(0), podIndex(0), sectionIndex(0), renewalSectionCounter(0), renewalPodCounter(0), nback(1), sinceLastnback(0), done(false)
{
}

Tunnel::Tunnel(CollisionScene *scene, Vector3 start, Number segmentWidth, Number segmentDepth, int sectionSize, int podSegmentSize)
: scene(scene), start(start), end(start), segments(), current(), segmentWidth(segmentWidth), segmentDepth(segmentDepth), sections(), types(), sectionSize(sectionSize), podSegmentSize(podSegmentSize), sectionIndex(0), podIndex(0), renewalSectionCounter(0), renewalPodCounter(0), nback(1), sinceLastnback(0), done(false)
{
	current = segments.end();
}

CollisionScene *Tunnel::getScene() const
{
	return scene;
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

list<TunnelSlice*>::iterator Tunnel::getCurrentIterator()
{
    return current;
}

list<TunnelSlice*>::iterator Tunnel::getBeginIterator()
{
    return segments.begin();
}

list<TunnelSlice*>::iterator Tunnel::getEndIterator()
{
    return segments.end();
}

// An estimate of which Segment contains the position with a certain linear interpolated offset
TunnelSlice* Tunnel::findSliceFromCurrent(Vector3 pos, Number tOffset) const
{
    list<TunnelSlice*>::iterator it = current;
    
    Number t = 0;
    while (it != segments.end())
    {
        
        t = (*it)->getT(pos) + tOffset;
        if (t <= 1)
        {
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

// Get the ith segment from current. Returns NULL if no ith segment exists
TunnelSlice* Tunnel::getNext(int i) const
{
    list<TunnelSlice*>::iterator it = current;
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

Number Tunnel::getSegmentWidth() const
{
	return segmentWidth;
}

Number Tunnel::getSegmentDepth() const
{
	return segmentDepth;
}

Quaternion Tunnel::getNewSegmentQuaternion(Direction dir, int degrees)
{
    Quaternion rot = endRot;
    /*
    if (segments.size() > 0) {
        rot = segments.back()->getQuaternion();
    } else {
        rot.set(1, 0, 0, 0);
    }
     */
    
    Quaternion rot1;
    Quaternion rot2;
    switch (dir)
    {
        case NORTH:
            rot1.createFromAxisAngle(TUNNEL_REFERENCE_RIGHT.x, TUNNEL_REFERENCE_RIGHT.y, TUNNEL_REFERENCE_RIGHT.z, degrees);
            rot = rot * rot1;
            break;
        case SOUTH:
            rot1.createFromAxisAngle(TUNNEL_REFERENCE_RIGHT.x, TUNNEL_REFERENCE_RIGHT.y, TUNNEL_REFERENCE_RIGHT.z, -degrees);
            rot = rot * rot1;
            break;
        case WEST:
            rot1.createFromAxisAngle(TUNNEL_REFERENCE_UPWARD.x, TUNNEL_REFERENCE_UPWARD.y, TUNNEL_REFERENCE_UPWARD.z, degrees);
            rot = rot * rot1;
            break;
        case EAST:
            rot1.createFromAxisAngle(TUNNEL_REFERENCE_UPWARD.x, TUNNEL_REFERENCE_UPWARD.y, TUNNEL_REFERENCE_UPWARD.z, -degrees);
            rot = rot * rot1;
            break;
        case NORTHWEST:
            rot1.createFromAxisAngle(TUNNEL_REFERENCE_UPWARD.x, TUNNEL_REFERENCE_UPWARD.y, TUNNEL_REFERENCE_UPWARD.z, degrees);
            rot2.createFromAxisAngle(TUNNEL_REFERENCE_RIGHT.x, TUNNEL_REFERENCE_RIGHT.y, TUNNEL_REFERENCE_RIGHT.z, degrees);
            rot = rot * rot1 * rot2;
            break;
        case NORTHEAST:
            rot1.createFromAxisAngle(TUNNEL_REFERENCE_UPWARD.x, TUNNEL_REFERENCE_UPWARD.y, TUNNEL_REFERENCE_UPWARD.z, -degrees);
            rot2.createFromAxisAngle(TUNNEL_REFERENCE_RIGHT.x, TUNNEL_REFERENCE_RIGHT.y, TUNNEL_REFERENCE_RIGHT.z, degrees);
            rot = rot * rot1 * rot2;
            break;
        case SOUTHWEST:
            rot1.createFromAxisAngle(TUNNEL_REFERENCE_UPWARD.x, TUNNEL_REFERENCE_UPWARD.y, TUNNEL_REFERENCE_UPWARD.z, degrees);
            rot2.createFromAxisAngle(TUNNEL_REFERENCE_RIGHT.x, TUNNEL_REFERENCE_RIGHT.y, TUNNEL_REFERENCE_RIGHT.z, -degrees);
            rot = rot * rot1 * rot2;
            break;
        case SOUTHEAST:
            rot1.createFromAxisAngle(TUNNEL_REFERENCE_UPWARD.x, TUNNEL_REFERENCE_UPWARD.y, TUNNEL_REFERENCE_UPWARD.z, -degrees);
            rot2.createFromAxisAngle(TUNNEL_REFERENCE_RIGHT.x, TUNNEL_REFERENCE_RIGHT.y, TUNNEL_REFERENCE_RIGHT.z, -degrees);
            rot = rot * rot1 * rot2;
            break;
        default:
            // no rotation
            break;
    }
    /*
        Quaternion rot1;
        Quaternion rot2;
        switch (dir)
        {
            case NORTH:
                rot1.createFromAxisAngle(right.x, right.y, right.z, degrees);
                rot = rot1 * rot;
                break;
            case SOUTH:
                rot1.createFromAxisAngle(right.x, right.y, right.z, -degrees);
                rot = rot1 * rot;
                break;
            case WEST:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, degrees);
                rot = rot1 * rot;
                break;
            case EAST:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, -degrees);
                rot = rot1 * rot;
                break;
            case NORTHWEST:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, degrees);
                rot2.createFromAxisAngle(right.x, right.y, right.z, degrees);
                rot = rot2 * rot1 * rot;
                break;
            case NORTHEAST:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, -degrees);
                rot2.createFromAxisAngle(right.x, right.y, right.z, degrees);
                rot = rot2 * rot1 * rot;
                break;
            case SOUTHWEST:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, degrees);
                rot2.createFromAxisAngle(right.x, right.y, right.z, -degrees);
                rot = rot2 * rot1 * rot;
                break;
            case SOUTHEAST:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, -degrees);
                rot2.createFromAxisAngle(right.x, right.y, right.z, -degrees);
                rot = rot2 * rot1 * rot;
                break;
            default:
                // no rotation
                break;
        }
    */
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

int Tunnel::getNBack() const
{
    return nback;
}

bool Tunnel::isDone() const
{
    return done;
}

void Tunnel::setDone(bool value)
{
    done = value;
}

void Tunnel::removeSegment()
{
	start += segments.front()->getForward() * (segmentDepth + TUNNEL_SEGMENT_BUFFER);
	segments.front()->removeFromCollisionScene(scene);
	segments.pop_front();
}

SectionInfo Tunnel::getNextSectionInfo()
{
    TunnelType segmentType = NORMAL_WITH_ONE_POD;
    Direction segmentDir = randDirection();
    int segmentTurnAngle = randRangeInt(TUNNEL_MIN_ANGLE_TURN, TUNNEL_MAX_ANGLE_TURN);
    
    return SectionInfo(segmentType, segmentDir, segmentTurnAngle);
}

PodInfo Tunnel::getNextPodInfo(SectionInfo & sectionInfo)
{
    PodType podType = (PodType)(rand() % NUM_POD_TYPES);
    Direction podLoc = randDirection();
    
    if (types.size() - nback >= 0) {
        int r = rand() % 100;
        if (r <= 5 * sinceLastnback)
            podType = types[types.size() - nback].podType;
    }
    
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
            /*
             // Guarantee an nback if a yellow nback appears
             podType = types[types.size() - nback].podType;
             podLoc = types[types.size() - 1].podLoc;
             */
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
    if (NBack == POD_NONE) {
        ++sinceLastnback;
    }
    else
        sinceLastnback = 0;
    
    return PodInfo(podType, podLoc);
}

void Tunnel::addSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc)
{
    Quaternion rot;
    Vector3 forward;
    Vector3 upward;
    Vector3 right;
    
    rot = getNewSegmentQuaternion(segmentTurn, turnDegrees);
    
    forward = rot.applyTo(TUNNEL_REFERENCE_FORWARD);
    right = rot.applyTo(TUNNEL_REFERENCE_RIGHT);
    upward = rot.applyTo(TUNNEL_REFERENCE_UPWARD);
    Vector3 stepend = end + forward * (segmentDepth + TUNNEL_SEGMENT_BUFFER);
	TunnelSlice* nsegment = new TunnelSlice(scene, segmentType, (end + stepend) / 2, rot, segmentWidth, segmentDepth);
    
    switch (segmentType)
    {
        case NORMAL_WITH_ONE_POD:
            nsegment->addPod(scene, podLoc, podType);
            break;
        case NORMAL_WITH_THREE_PODS:
            nsegment->addPod(scene, leftOf(podLoc), podType);
            nsegment->addPod(scene, podLoc, podType);
            nsegment->addPod(scene, rightOf(podLoc), podType);
            break;
        case NORMAL_WITH_FIVE_PODS:
            nsegment->addPod(scene, leftOf(leftOf(podLoc)), podType);
            nsegment->addPod(scene, leftOf(podLoc), podType);
            nsegment->addPod(scene, podLoc, podType);
            nsegment->addPod(scene, rightOf(podLoc), podType);
            nsegment->addPod(scene, rightOf(rightOf(podLoc)), podType);
            break;
        case NORMAL_WITH_MANY_PODS:
        {
            for (int d = 0; d < 8; ++d) {
                if ((Direction)(d) != podLoc)
                    nsegment->addPod(scene, (Direction)(d), podType);
            }
            break;
        }
        default:
            break;
    }

	end = stepend;
    if (segments.size() <= 0) // Init TunnelSlice iterator
    {
        segments.push_back(nsegment);
        current = segments.begin();
    }
    else
    {
        segments.back()->connect(scene, nsegment);
        segments.push_back(nsegment);
    }
}

// Moves a segment in front to the back of the list.
// The infinite segment is maintained to be the last element.
void Tunnel::renewSegment(TunnelType segmentType, Direction segmentTurn, int turnDegrees, PodType podType, Direction podLoc)
{
    if (segments.size() <= 0)
        return;
    
    Quaternion rot;
    Vector3 forward;
    Vector3 upward;
    Vector3 right;
    rot = getNewSegmentQuaternion(segmentTurn, turnDegrees);
    
    forward = rot.applyTo(TUNNEL_REFERENCE_FORWARD);
    right = rot.applyTo(TUNNEL_REFERENCE_RIGHT);
    upward = rot.applyTo(TUNNEL_REFERENCE_UPWARD);
    Vector3 stepend = end + forward * (segmentDepth + TUNNEL_SEGMENT_BUFFER);
    
	TunnelSlice *nsegment = segments.front();
    nsegment->rejuvenate(scene, segmentType, (end + stepend) / 2, rot, segmentWidth, segmentDepth);
    
    switch (segmentType)
    {
        case NORMAL_WITH_ONE_POD:
            nsegment->addPod(scene, podLoc, podType);
            break;
        case NORMAL_WITH_THREE_PODS:
            nsegment->addPod(scene, leftOf(podLoc), podType);
            nsegment->addPod(scene, podLoc, podType);
            nsegment->addPod(scene, rightOf(podLoc), podType);
            break;
        case NORMAL_WITH_FIVE_PODS:
            nsegment->addPod(scene, leftOf(leftOf(podLoc)), podType);
            nsegment->addPod(scene, leftOf(podLoc), podType);
            nsegment->addPod(scene, podLoc, podType);
            nsegment->addPod(scene, rightOf(podLoc), podType);
            nsegment->addPod(scene, rightOf(rightOf(podLoc)), podType);
            break;
        case NORMAL_WITH_MANY_PODS:
        {
            for (int d = 0; d < 8; ++d) {
                if ((Direction)(d) != podLoc) 
                    nsegment->addPod(scene, (Direction)(d), podType);
            }
            break;
        }
        default:
            break;
    }
    
    end = stepend;
    if (segments.size() > 1)
    {
        nsegment->disconnect(scene);
        segments.back()->connect(scene, nsegment);
        segments.push_back(nsegment);
        segments.pop_front();
    }
}

void Tunnel::addSection(SectionInfo newSection)
{
    if (newSection.tunnelType == CHECKPOINT) {
        
        for (int i = 0; i < sectionSize; ++i)
            if (i == sectionSize - 1)
                addSegment(CHECKPOINT, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION);
            else
                addSegment(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION);
        return;
    }
    for (int i = 0; i < sectionSize; ++i)
    {
        if (newSection.tunnelType != NORMAL_BLANK)
            ++renewalPodCounter;
        if (renewalPodCounter >= podSegmentSize)
        {
            PodInfo newPod = getNextPodInfo(newSection);
            addSegment(newSection.tunnelType, newSection.tunnelDir, newSection.tunnelDirAngle, newPod.podType, newPod.podLoc);
            renewalPodCounter = 0;
            
            types.push_back(newPod);
        }
        else
        {
            addSegment(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION);
        }
    }
    sections.push_back(newSection);
}

void Tunnel::renewSection(SectionInfo newSection)
{
    if (newSection.tunnelType == CHECKPOINT) {
        
        for (int i = 0; i < sectionSize; ++i)
            if (i == sectionSize - 1)
                renewSegment(CHECKPOINT, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION);
            else
                renewSegment(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION);
        return;
    }
    for (int i = 0; i < sectionSize; ++i)
    {
        if (newSection.tunnelType != NORMAL_BLANK)
            ++renewalPodCounter;
        if (renewalPodCounter >= podSegmentSize)
        {
            PodInfo newPod = getNextPodInfo(newSection);
            addSegment(newSection.tunnelType, newSection.tunnelDir, newSection.tunnelDirAngle, newPod.podType, newPod.podLoc);
            renewalPodCounter = 0;
            
            types.push_back(newPod);
        }
        else
        {
            addSegment(NORMAL_BLANK, newSection.tunnelDir, newSection.tunnelDirAngle, POD_NONE, NO_DIRECTION);
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
    Vector3 endOfSlice = currentSlice->getCenter() + (currentSlice->getForward() * segmentDepth / 2);
    if ((checkPos - endOfSlice).dot(currentSlice->getForward()) >= 0)
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
            if (renewalSectionCounter >= sectionSize)
            {
                SectionInfo info = getNextSectionInfo();
                renewSection(info);
                renewalSectionCounter = 0;
                ++sectionIndex;
            }
        }
        
        //if (current != segments.end())
        //    (*current)->changeWallTexture();
        
        return true;
	}
    return false;
}

void Tunnel::constructTunnel(int size, int nback, Quaternion q)
{
    this->endRot = q;
    this->nback = nback;
    
    for (int i = 0; i < INITIATION_SECTIONS; ++i) {
        SectionInfo info = SectionInfo(NORMAL_BLANK, NO_DIRECTION, 0);
        addSection(info);
    }
        
    for (int i = 0; i < size - INITIATION_SECTIONS; ++i) {
        SectionInfo info = getNextSectionInfo();
        addSection(info);
    }
}

vector<Pod *> Tunnel::findPodCollisions(CollisionScene *scene, SceneEntity *entity)
{
    vector<Pod *> collisions;
    list<TunnelSlice *>::iterator it = segments.begin();
    for (it = current; it != segments.end(); ++it) {
        
        //Fog3 Size Hardcoded... FIX!
        Number entDist = entity->getPosition().z + ((5.0 * 5.0)/2);
        
        Pod * pod = NULL;
        if ( (*it)->getPods().size() > 0 )
            pod = (*it)->getPods()[0];
    
        if (pod && entDist < pod->getHead()->getPosition().z && !pod->getPastFog()) {
            pod->setPastFog(true);
            collisions.push_back(pod);
        }
        
        if (collisions.size() > 0) return collisions;
    }
    return collisions;
}

Tunnel::~Tunnel()
{
	while (!segments.empty())
	{
		segments.front()->removeFromCollisionScene(scene);
        delete segments.front();
		segments.pop_front();
	}
}
