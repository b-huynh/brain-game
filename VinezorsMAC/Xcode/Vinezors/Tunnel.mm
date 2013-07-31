#include "Tunnel.h"

#include <cstdlib>
#include <iostream>

const Number infinityDepth = 1024;

Tunnel::Tunnel()
: scene(NULL), start(), end(), segments(), current(), segmentWidth(0.0), segmentDepth(0.0)
{
}

Tunnel::Tunnel(CollisionScene *scene, Vector3 start, Number segmentWidth, Number segmentDepth)
: scene(scene), start(start), end(start), segments(), current(), segmentWidth(segmentWidth), segmentDepth(segmentDepth)
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
        it++;
        if (it == segments.end())
            return NULL;
    }
	return *it;
}

Number Tunnel::getSegmentWidth() const
{
	return segmentWidth;
}

Number Tunnel::getSegmentDepth() const
{
	return segmentDepth;
}

void Tunnel::addSegment(PodType type, Direction loc)
{
    Quaternion rot;
    Vector3 forward;
    Vector3 upward;
    Vector3 right;
    if (segments.size() > 0) {
        rot = segments.back()->getQuaternion();
        forward = segments.back()->getForward();
        upward = segments.back()->getUpward();
        right = segments.back()->getRight();
        
        int dir = rand() % 9;
        double degrees = randRangeInt(0, 10);
        Quaternion rot1;
        Quaternion rot2;
        switch (dir)
        {
            case 0:
                rot1.createFromAxisAngle(right.x, right.y, right.z, degrees);
                rot = rot1 * rot;
                break;
            case 1:
                rot1.createFromAxisAngle(right.x, right.y, right.z, -degrees);
                rot = rot1 * rot;
                break;
            case 2:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, degrees);
                rot = rot1 * rot;
                break;
            case 3:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, -degrees);
                rot = rot1 * rot;
                break;
            case 4:              
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, degrees);
                rot2.createFromAxisAngle(right.x, right.y, right.z, degrees);
                rot = rot2 * rot1 * rot;
                break;
            case 5:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, -degrees);
                rot2.createFromAxisAngle(right.x, right.y, right.z, degrees);
                rot = rot2 * rot1 * rot;
                break;
            case 6:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, degrees);
                rot2.createFromAxisAngle(right.x, right.y, right.z, -degrees);
                rot = rot2 * rot1 * rot;
                break;
            case 7:
                rot1.createFromAxisAngle(upward.x, upward.y, upward.z, -degrees);
                rot2.createFromAxisAngle(right.x, right.y, right.z, -degrees);
                rot = rot2 * rot1 * rot;
                break;
            default:
                // no rotation
                break;
        }
    } else {
        rot.set(1, 0, 0, 0);
        forward = TUNNEL_REFERENCE_FORWARD;
        upward = TUNNEL_REFERENCE_UPWARD;
        right = TUNNEL_REFERENCE_RIGHT;
    }
    
    forward = rot.applyTo(TUNNEL_REFERENCE_FORWARD);
    right = rot.applyTo(TUNNEL_REFERENCE_RIGHT);
    upward = rot.applyTo(TUNNEL_REFERENCE_UPWARD);
    Vector3 stepend = end + forward * (segmentDepth + TUNNEL_ANGLE_BUFFER);
	TunnelSlice* newSegment = new TunnelSlice(scene, NORMAL_WITH_PODS, (end + stepend) / 2, rot, segmentWidth, segmentDepth);

	int numPods = rand() % 1 + 1;

	for (int i = 0; i < numPods; ++i)
	{
		int randPod = rand() % 5;
        randPod = (int)type;
        
		newSegment->addPod(scene, loc, (PodType)randPod);
	}
	end = stepend;
	if (segments.size() <= 0) // Init TunnelSlice iterator
    {
        segments.push_back(newSegment);
		current = segments.begin();
    }
    else {
        segments.back()->connect(scene, newSegment);
        segments.push_back(newSegment);
    }
}

void Tunnel::addSegment(PodType type)
{
    addSegment(type, randDirection());
}

void Tunnel::removeSegment()
{
	start += segments.front()->getForward() * (segmentDepth + TUNNEL_ANGLE_BUFFER);
	segments.front()->removeFromCollisionScene(scene);
	segments.pop_front();
}

// Moves a segment in front to the back of the list.
// The infinite segment is maintained to be the last element.
void Tunnel::renewSegment()
{
	TunnelSlice *frontTunnel = segments.front();
    Vector3 endStep = end + segments.front()->getForward() * (segmentDepth * TUNNEL_ANGLE_BUFFER);
	frontTunnel->move((end + endStep) / 2 - frontTunnel->getCenter());
	segments.pop_front();
	segments.push_back(frontTunnel);
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
		++current;
        //removeSegment();
		//renewSegment();
		//(*current)->changeWallTexture();
        
        return true;
	}
    return false;
}

void Tunnel::constructTunnel(int size, int nback)
{
    if (size <= nback) return;
    
    vector<PodType> colors (size);
    for (int i = 0; i < size; ++i) {
        colors[i] = (PodType)(rand() % 5);
        while (colors[i] == POD_YELLOW)
            colors[i] = (PodType)(rand() % 5);
    }
    
    for (int i = 0; i < size - nback - 1; ++i) {
        if (colors[i] != POD_YELLOW && colors[i] == colors[i + nback])
            colors[i + nback + 1] = POD_YELLOW;
    }
    
    cout << "Unknown is:" << POD_UNKNOWN << endl;
    cout << "Yellow is:" << POD_YELLOW << endl;
    
    Direction dir = randDirection();
    for (int i = 0; i < size; ++i) {
        cout << colors[i] << std::endl;
        if (colors[i] == POD_YELLOW) {
            addSegment(colors[i], dir);
        } else {
            dir = randDirection();
            addSegment(colors[i], dir);
        }
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
		segments.pop_front();
	}
}
