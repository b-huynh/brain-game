#include "Tunnel.h"

#include <cstdlib>
#include <iostream>

const Number infinityDepth = 1024;

TunnelSlice::TunnelSlice()
	: center(), width(0), depth(0), type(NORMAL_BLANK),
	topLeftWall(NULL), topWall(NULL), topRightWall(NULL), rightWall(NULL), bottomRightWall(NULL), bottomWall(NULL), bottomLeftWall(NULL), leftWall(NULL), 
	pods()
{}

TunnelSlice::TunnelSlice(CollisionScene *scene, TunnelType type, Vector3 center, Number width, Number depth)
	: center(center), width(width), depth(depth), type(type),
	topLeftWall(NULL), topWall(NULL), topRightWall(NULL), rightWall(NULL), bottomRightWall(NULL), bottomWall(NULL), bottomLeftWall(NULL), leftWall(NULL), 
	pods()
{
	string filePathWallTexture;
	if (type == NORMAL_WITH_PODS)
		filePathWallTexture = "resources/yellow_solid_circle.png";
	else if (type == NORMAL_BLANK)
		filePathWallTexture = "resources/yellow_solid.png";
	else if (type == NORMAL_ELONGATED)
		filePathWallTexture = "resources/yellow_solid_ellipse.png";

	Number wallLength = width / (2 * cos(PI / 4) + 1);

	topLeftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topLeftWall->setPosition(center.x - (width + wallLength) / 4, center.y + (width + wallLength) / 4, center.z);
	topLeftWall->setRoll(225);
	topLeftWall->loadTexture(filePathWallTexture);
	
	topWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topWall->setPosition(center.x, center.y + width / 2, center.z);
	topWall->setRoll(180);
	topWall->loadTexture(filePathWallTexture);
	
	topRightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topRightWall->setPosition(center.x + (width + wallLength) / 4, center.y + (width + wallLength) / 4, center.z);
	topRightWall->setRoll(135);
	topRightWall->loadTexture(filePathWallTexture);

	rightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	rightWall->setPosition(center.x + width / 2, center.y, center.z);
	rightWall->setRoll(90);
	rightWall->loadTexture(filePathWallTexture);

	bottomRightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomRightWall->setPosition(center.x + (width + wallLength) / 4, center.y - (width + wallLength) / 4, center.z);
	bottomRightWall->setRoll(45);
	bottomRightWall->loadTexture(filePathWallTexture);

	bottomWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomWall->setPosition(center.x, center.y - width / 2, center.z);
	bottomWall->setRoll(0);
	bottomWall->loadTexture(filePathWallTexture);
	
	bottomLeftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomLeftWall->setPosition(center.x - (width + wallLength) / 4, center.y - (width + wallLength) / 4, center.z);
	bottomLeftWall->setRoll(-45);
	bottomLeftWall->loadTexture(filePathWallTexture);

	leftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	leftWall->setPosition(center.x - width / 2, center.y, center.z);
	leftWall->setRoll(-90);
	leftWall->loadTexture(filePathWallTexture);

	addToCollisionScene(scene);
}

Vector3 TunnelSlice::getCenter() const
{
	return center;
}

vector<Pod *> TunnelSlice::getPods()
{
    return pods;
}

vector<Pod *> TunnelSlice::findCollisions(CollisionScene *scene, SceneEntity *ent) const
{
	vector<Pod *> ret;
	for (int i = 0; i < pods.size(); ++i)
	{
		if (scene->testCollision(ent,  pods[i]->getHead()).collided)
			ret.push_back(pods[i]);
	}
	return ret;
}

void TunnelSlice::move(Vector3 delta)
{
	center.x += delta.x;
	center.y += delta.y;
	center.z += delta.z;
	
	topLeftWall->Translate(delta);
	topWall->Translate(delta);
	topRightWall->Translate(delta);
	rightWall->Translate(delta);
	bottomRightWall->Translate(delta);
	bottomWall->Translate(delta);
	bottomLeftWall->Translate(delta);
	leftWall->Translate(delta);

	for (int i = 0; i < pods.size(); ++i)
		pods[i]->move(delta);
}

void TunnelSlice::changeWallTexture()
{
	string filePathWallTexture;
	if (type == NORMAL_WITH_PODS)
		filePathWallTexture = "resources/red_solid_circle.png";
	else if (type == NORMAL_BLANK)
		filePathWallTexture = "resources/red_solid.png";
	else if (type == NORMAL_ELONGATED)
		filePathWallTexture = "resources/red_solid_ellipse.png";
	topLeftWall->loadTexture(filePathWallTexture);
	topWall->loadTexture(filePathWallTexture);
	topRightWall->loadTexture(filePathWallTexture);
	rightWall->loadTexture(filePathWallTexture);
	bottomRightWall->loadTexture(filePathWallTexture);
	bottomWall->loadTexture(filePathWallTexture);
	bottomLeftWall->loadTexture(filePathWallTexture);
	leftWall->loadTexture(filePathWallTexture);
}

void TunnelSlice::addPod(CollisionScene *scene, Direction loc, PodType type)
{
	Number wallLength = width / (2 * cos(PI / 4) + 1);;
	const Number STEM_RADIUS = width / 100;
	const Number HEAD_RADIUS = width / 25;
	const Number STEM_LENGTH = wallLength / 3;

	Vector3 base;
	Vector3 head;
	switch (loc)
	{
	case NORTHWEST:
		base = Vector3(center.x - (width + wallLength) / 4, center.y + (width + wallLength) / 4, center.z);
		head = Vector3(base.x - cos(3 * PI / 4) * STEM_LENGTH, base.y - sin(3 * PI / 4) * STEM_LENGTH, base.z);
		break;
	case NORTH:
		base = Vector3(center.x, center.y + width / 2, center.z);
		head = Vector3(base.x - cos(PI / 2) * STEM_LENGTH, base.y - sin(PI / 2) * STEM_LENGTH, base.z);
		break;
	case NORTHEAST:
		base = Vector3(center.x + (width + wallLength) / 4, center.y + (width + wallLength) / 4, center.z);
		head = Vector3(base.x - cos(PI / 4) * STEM_LENGTH, base.y - sin(PI / 4) * STEM_LENGTH, base.z);
		break;
	case EAST:
		base = Vector3(center.x + width / 2, center.y, center.z);
		head = Vector3(base.x - cos(0.0) * STEM_LENGTH, base.y - sin(0.0) * STEM_LENGTH, base.z);
		break;
	case SOUTHEAST:
		base = Vector3(center.x + (width + wallLength) / 4, center.y - (width + wallLength) / 4, center.z);
		head = Vector3(base.x - cos(-PI / 4) * STEM_LENGTH, base.y - sin(-PI / 4) * STEM_LENGTH, base.z);
		break;
	case SOUTH:
		base = Vector3(center.x, center.y - width / 2, center.z);
		head = Vector3(base.x - cos(-PI / 2) * STEM_LENGTH, base.y - sin(-PI / 2) * STEM_LENGTH, base.z);
		break;
	case SOUTHWEST:
		base = Vector3(center.x - (width + wallLength) / 4, center.y - (width + wallLength) / 4, center.z);
		head = Vector3(base.x - cos(-3 * PI / 4) * STEM_LENGTH, base.y - sin(-3 * PI / 4) * STEM_LENGTH, base.z);
		break;
	case WEST:
		base = Vector3(center.x - width / 2, center.y, center.z);
		head = Vector3(base.x - cos(-PI) * STEM_LENGTH, base.y - sin(-PI) * STEM_LENGTH, base.z);
		break;
	default:
		// No Direction
		return;
	}

	pods.push_back(new Pod(scene, type, base, head, STEM_RADIUS, HEAD_RADIUS));
}

void TunnelSlice::addToCollisionScene(CollisionScene *scene)
{
	scene->addCollisionChild(topLeftWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(topWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(topRightWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(rightWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(bottomRightWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(bottomWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(bottomLeftWall, CollisionSceneEntity::SHAPE_PLANE);
	scene->addCollisionChild(leftWall, CollisionSceneEntity::SHAPE_PLANE);
}

void TunnelSlice::removeFromCollisionScene(CollisionScene * scene)
{
	scene->removeEntity(topLeftWall);
	scene->removeEntity(topWall);
	scene->removeEntity(topRightWall);
	scene->removeEntity(rightWall);
	scene->removeEntity(bottomRightWall);
	scene->removeEntity(bottomWall);
	scene->removeEntity(bottomLeftWall);
	scene->removeEntity(leftWall);
	
	for (int i = 0; i < pods.size(); ++i)
		pods[i]->removeFromCollisionScene(scene);
	delete topLeftWall; topLeftWall = NULL;
	delete topWall; topWall = NULL;
	delete topRightWall; topRightWall = NULL;
	delete rightWall; rightWall = NULL;
	delete bottomRightWall; bottomRightWall = NULL;
	delete bottomWall; bottomWall = NULL;
	delete bottomLeftWall; bottomLeftWall = NULL;
	delete leftWall; leftWall = NULL;

	pods.clear();
}

PodType TunnelSlice::getPodType()
{
    for (int i = 0; i < pods.size(); ++i) {
        return pods[i]->getPodType();
    }
    return POD_UNKNOWN;
}


Tunnel::Tunnel()
	: scene(NULL), start(), end(), segments(), current(), segmentWidth(0.0), segmentDepth(0.0)
{
}

Tunnel::Tunnel(CollisionScene *scene, Vector3 start, Number segmentWidth, Number segmentDepth)
	: scene(scene), start(start), end(start), segments(), current(), segmentWidth(segmentWidth), segmentDepth(segmentDepth)
{
	segments.push_back(new TunnelSlice(scene, NORMAL_WITH_PODS, end + Vector3(0, 0, (segmentDepth - infinityDepth) / 2), segmentWidth, infinityDepth));
	current = segments.begin();
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

TunnelSlice *Tunnel::getCurrent() const
{
	if (current == segments.end())
		return NULL;
	return *current;
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
	TunnelSlice *newSegment = new TunnelSlice(scene, NORMAL_WITH_PODS, end, segmentWidth, segmentDepth);
	if (segments.size() <= 0)
	{
		segments.push_back(newSegment);
		current = segments.begin();
		return;
	}
	TunnelSlice *backTunnel = segments.back();


	int numPods = rand() % 1 + 1;

	for (int i = 0; i < numPods; ++i)
	{
		int randPod = rand() % 5;
        randPod = (int)type;
        
		newSegment->addPod(scene, loc, (PodType)randPod);
	}
	segments.back() = newSegment;
	if (segments.size() == 1)
		newSegment->changeWallTexture();
	end += Vector3(0, 0, -segmentDepth);
	segments.push_back(backTunnel);
	segments.back()->move(Vector3(0, 0, -segmentDepth));
}

void Tunnel::addSegment(PodType type)
{
    addSegment(type, randDirection());
}

void Tunnel::removeSegment()
{
	segments.front()->removeFromCollisionScene(scene);
	segments.pop_front();
	start += Vector3(0, 0, -segmentDepth);
}

// Moves a segment in front to the back of the list.
// The infinite segment is maintained to be the last element.
void Tunnel::renewSegment()
{
	TunnelSlice *backTunnel = segments.back();
	segments.pop_back();
	TunnelSlice *frontTunnel = segments.front();
	frontTunnel->move(segments.back()->getCenter() - frontTunnel->getCenter() + Vector3(0, 0, -segmentDepth));
	segments.pop_front();
	segments.push_back(frontTunnel);
	backTunnel->move(Vector3(0, 0, -segmentDepth));
	segments.push_back(backTunnel);

}

// Determines whether the current segment should be updated depending on the position compared.
void Tunnel::renewIfNecessary(Vector3 checkPos)
{
	if (current == segments.end())
		return;
	if (checkPos.z < (*current)->getCenter().z - segmentDepth)
	{
		++current;
		renewSegment();
		//(*current)->changeWallTexture();
	}
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
            addSegment(colors[i],dir);
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
