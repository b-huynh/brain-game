#include "Tunnel.h"

#include <cstdlib>

TunnelSlice::TunnelSlice()
	: center(), width(0), depth(0),
	topLeftWall(NULL), topWall(NULL), topRightWall(NULL), rightWall(NULL), bottomRightWall(NULL), bottomWall(NULL), bottomLeftWall(NULL), leftWall(NULL), 
	pods()
{}

TunnelSlice::TunnelSlice(CollisionScene *scene, Vector3 center, Number width, Number depth)
	: center(center), width(width), depth(depth),
	topLeftWall(NULL), topWall(NULL), topRightWall(NULL), rightWall(NULL), bottomRightWall(NULL), bottomWall(NULL), bottomLeftWall(NULL), leftWall(NULL), 
	pods()
{
	Number wallLength = width / (2 * cos(PI / 4) + 1);

	topLeftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topLeftWall->setPosition(center.x - (width + wallLength) / 4, center.y + (width + wallLength) / 4, center.z);
	topLeftWall->setRoll(225);
	//topLeftWall->setMaterialByName("WallMaterial");
	topLeftWall->loadTexture("resources/yellow_solid.png");
	
	topWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topWall->setPosition(center.x, center.y + width / 2, center.z);
	topWall->setRoll(180);
	//topWall->setMaterialByName("WallMaterial");
	topWall->loadTexture("resources/yellow_solid.png");
	
	topRightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topRightWall->setPosition(center.x + (width + wallLength) / 4, center.y + (width + wallLength) / 4, center.z);
	topRightWall->setRoll(135);
	//topRightWall->setMaterialByName("WallMaterial");
	topRightWall->loadTexture("resources/yellow_solid.png");

	rightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	rightWall->setPosition(center.x + width / 2, center.y, center.z);
	rightWall->setRoll(90);
	//rightWall->setMaterialByName("WallMaterial");
	rightWall->loadTexture("resources/yellow_solid.png");

	bottomRightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomRightWall->setPosition(center.x + (width + wallLength) / 4, center.y - (width + wallLength) / 4, center.z);
	bottomRightWall->setRoll(45);
	//bottomRightWall->setMaterialByName("WallMaterial");
	bottomRightWall->loadTexture("resources/yellow_solid.png");

	bottomWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomWall->setPosition(center.x, center.y - width / 2, center.z);
	bottomWall->setRoll(0);
	//bottomWall->setMaterialByName("WallMaterial");
	bottomWall->loadTexture("resources/yellow_solid.png");
	
	bottomLeftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomLeftWall->setPosition(center.x - (width + wallLength) / 4, center.y - (width + wallLength) / 4, center.z);
	bottomLeftWall->setRoll(-45);
	//bottomLeftWall->setMaterialByName("WallMaterial");
	bottomLeftWall->loadTexture("resources/yellow_solid.png");

	leftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	leftWall->setPosition(center.x - width / 2, center.y, center.z);
	leftWall->setRoll(-90);
	//leftWall->setMaterialByName("WallMaterial");
	leftWall->loadTexture("resources/yellow_solid.png");
	
	//light = new SceneLight(SceneLight::AREA_LIGHT, scene, intensity);
	//light->setPosition(center);

	addToCollisionScene(scene);
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
		pods[i].move(delta);
}

void TunnelSlice::addPod(CollisionScene *scene, Direction loc)
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

	pods.push_back(Pod(scene, base, head, STEM_RADIUS, HEAD_RADIUS));
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

	//scene->addLight(light);
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

	//scene->removeLight(light);
	
	//delete light; light = NULL;

	for (int i = 0; i < pods.size(); ++i)
		pods[i].removeFromCollisionScene(scene);
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

const Number infinityDepth = 1024;

Tunnel::Tunnel()
	: scene(NULL), start(), end(), segments(), segmentWidth(0.0), segmentDepth(0.0)
{
}

Tunnel::Tunnel(CollisionScene *scene, Vector3 start, Number segmentWidth, Number segmentDepth)
	: scene(scene), start(start), end(start), segments(), segmentWidth(segmentWidth), segmentDepth(segmentDepth)
{
	segments.push_back(new TunnelSlice(scene, end + Vector3(0, 0, (segmentDepth - infinityDepth) / 2), segmentWidth, infinityDepth));
}

void Tunnel::addSegment()
{
	TunnelSlice *backTunnel = segments.back();

	TunnelSlice *newSegment = new TunnelSlice(scene, end, segmentWidth, segmentDepth);

	vector<Direction> dirs(8);
	dirs[0] = NORTHWEST;
	dirs[1] = NORTH;
	dirs[2] = NORTHEAST;
	dirs[3] = EAST;
	dirs[4] = SOUTHEAST;
	dirs[5] = SOUTH;
	dirs[6] = SOUTHWEST;
	dirs[7] = WEST;
	int numPods = rand() % dirs.size();

	for (int i = 0; i < numPods; ++i)
	{
		int randDirIndex = rand() % dirs.size();
		newSegment->addPod(scene, dirs[randDirIndex]);
		dirs[randDirIndex] = dirs[dirs.size() - 1];
		dirs.pop_back();
	}
	segments.back() = newSegment;
	
	end += Vector3(0, 0, -segmentDepth);
	segments.push_back(backTunnel);
	segments.back()->move(Vector3(0, 0, -segmentDepth));
}

void Tunnel::removeSegment()
{
	segments.front()->removeFromCollisionScene(scene);
	segments.pop_front();
	start += Vector3(0, 0, -segmentDepth);
}

// Determines whether a new segment should be updated to look like the tunnel is infinitely long.
// However, this is not used anymore due to inefficiency. Everything currently is pre-allocated
void Tunnel::renewIfNecessary(Vector3 checkPos)
{
	if (checkPos.z < start.z - segmentDepth)
	{
		addSegment();
		//removeSegment();
	}
}

Tunnel::~Tunnel()
{
	while (!segments.empty())
	{
		segments.front()->removeFromCollisionScene(scene);
		segments.pop_front();
	}
}
