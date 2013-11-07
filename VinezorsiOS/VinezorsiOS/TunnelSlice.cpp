//
//  TunnelSlice.cpp
//  Testing
//
//  Created by Calvin Phung on 9/9/13.
//
//

#include "TunnelSlice.h"

#include <cstdlib>

using namespace std;

extern Util::ConfigGlobal globals;

static int wallID = 0;
static int intermediateMeshID = 0;

TunnelSlice::TunnelSlice()
: parentNode(NULL), center(), rot(), width(0), depth(0), type(NORMAL_BLANK), materialName(""), entireWall(NULL),
topLeftWall(NULL), topWall(NULL), topRightWall(NULL), rightWall(NULL), bottomRightWall(NULL), bottomWall(NULL), bottomLeftWall(NULL), leftWall(NULL), entireIntermediate(NULL), topLeftIntermediate(NULL), topIntermediate(NULL), topRightIntermediate(NULL), rightIntermediate(NULL), bottomRightIntermediate(NULL), bottomIntermediate(NULL), bottomLeftIntermediate(NULL), leftIntermediate(NULL),
pods(), growthT(0), prerangeT(0), sidesUsed(), podHistory(false), infoStored(false)
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = false;
}

TunnelSlice::TunnelSlice(Ogre::SceneNode* parentNode, TunnelType type, Vector3 center, Quaternion rot, double width, double depth, const std::string & material, const bool sides[NUM_DIRECTIONS])
: parentNode(parentNode), center(center), rot(rot), width(width), depth(depth), type(type), materialName(material), entireWall(NULL),
topLeftWall(NULL), topWall(NULL), topRightWall(NULL), rightWall(NULL), bottomRightWall(NULL), bottomWall(NULL), bottomLeftWall(NULL), leftWall(NULL), entireIntermediate(NULL),topLeftIntermediate(NULL), topIntermediate(NULL), topRightIntermediate(NULL), rightIntermediate(NULL), bottomRightIntermediate(NULL), bottomIntermediate(NULL), bottomLeftIntermediate(NULL), leftIntermediate(NULL), pods(), growthT(0), prerangeT(0), sidesUsed(), podHistory(false), infoStored(false)
{
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = sides[i];
    initWalls();
}

void TunnelSlice::initWalls()
{
	double wallLength = getWallLength();
    
    double angle;
    Quaternion q;
    Vector3 move;
    
    entireWall = parentNode->createChildSceneNode("entireWallNode" + Util::toStringInt(wallID));
    
    if (type == CHECKPOINT)
        materialName = "General/WallCheckpoint";
    
    if (sidesUsed[NORTHWEST]) {
        topLeftWall = entireWall->createChildSceneNode("topLeftWallNode" + Util::toStringInt(wallID));
        Entity* topLeftWallEntity = topLeftWall->getCreator()->createEntity("topLeftWallEntity" + Util::toStringInt(wallID), "wallTileMesh");
        topLeftWall->attachObject(topLeftWallEntity);
        move = Vector3(-(width + wallLength) / 4, (width + wallLength) / 4, 0);
        topLeftWall->translate(move);
        topLeftWall->roll(Degree(225));
        topLeftWall->scale(wallLength, 1.0, depth);
        topLeftWallEntity->setMaterialName(materialName);
    }
    
    if (sidesUsed[NORTH]) {
        topWall = entireWall->createChildSceneNode("topWallNode" + Util::toStringInt(wallID));
        Entity* topWallEntity = topWall->getCreator()->createEntity("topWallEntity" + Util::toStringInt(wallID), "wallTileMesh");
        topWall->attachObject(topWallEntity);
        move = Vector3(0, width / 2, 0);
        topWall->translate(move);
        topWall->roll(Degree(180));
        topWall->scale(wallLength, 1.0, depth);
        topWallEntity->setMaterialName(materialName);
    }
    
    if (sidesUsed[NORTHEAST]) {
        topRightWall = entireWall->createChildSceneNode("topRightWallNode" + Util::toStringInt(wallID));
        Entity* topRightWallEntity = topRightWall->getCreator()->createEntity("topRightWallEntity" + Util::toStringInt(wallID), "wallTileMesh");
        topRightWall->attachObject(topRightWallEntity);
        move = Vector3((width + wallLength) / 4, (width + wallLength) / 4, 0);
        topRightWall->translate(move);
        topRightWall->roll(Degree(135));
        topRightWall->scale(wallLength, 1.0, depth);
        topRightWallEntity->setMaterialName(materialName);
    }
    
    if (sidesUsed[EAST]) {
        rightWall = entireWall->createChildSceneNode("rightWallNode" + Util::toStringInt(wallID));
        Entity* rightWallEntity = rightWall->getCreator()->createEntity("rightWallEntity" + Util::toStringInt(wallID), "wallTileMesh");
        rightWall->attachObject(rightWallEntity);
        move = Vector3(width / 2, 0, 0);
        rightWall->translate(move);
        rightWall->roll(Degree(90));
        rightWall->scale(wallLength, 1.0, depth);
        rightWallEntity->setMaterialName(materialName);
    }
    
    if (sidesUsed[SOUTHEAST]) {
        bottomRightWall = entireWall->createChildSceneNode("bottomRightWallNode" + Util::toStringInt(wallID));
        Entity* bottomRightWallEntity = bottomRightWall->getCreator()->createEntity("bottomRightWallEntity" + Util::toStringInt(wallID), "wallTileMesh");
        bottomRightWall->attachObject(bottomRightWallEntity);
        move = Vector3((width + wallLength) / 4, -(width + wallLength) / 4, 0);
        bottomRightWall->translate(move);
        bottomRightWall->roll(Degree(45));
        bottomRightWall->scale(wallLength, 1.0, depth);
        bottomRightWallEntity->setMaterialName(materialName);
    }
    
    if (sidesUsed[SOUTH]) {
        bottomWall = entireWall->createChildSceneNode("bottomWallNode" + Util::toStringInt(wallID));
        Entity* bottomWallEntity = bottomWall->getCreator()->createEntity("bottomWallEntity" + Util::toStringInt(wallID), "wallTileMesh");
        bottomWall->attachObject(bottomWallEntity);
        move = Vector3(0, -width / 2, 0);
        bottomWall->translate(move);
        bottomWall->roll(Degree(0));
        bottomWall->scale(wallLength, 1.0, depth);
        bottomWallEntity->setMaterialName(materialName);
    }
    
    if (sidesUsed[SOUTHWEST]) {
        bottomLeftWall = entireWall->createChildSceneNode("bottomLeftWallNode" + Util::toStringInt(wallID));
        Entity* bottomLeftWallEntity = bottomLeftWall->getCreator()->createEntity("bottomLeftWallEntity" + Util::toStringInt(wallID), "wallTileMesh");
        bottomLeftWall->attachObject(bottomLeftWallEntity);
        move = Vector3(-(width + wallLength) / 4, -(width + wallLength) / 4, 0);
        bottomLeftWall->translate(move);
        bottomLeftWall->roll(Degree(315));
        bottomLeftWall->scale(wallLength, 1.0, depth);
        bottomLeftWallEntity->setMaterialName(materialName);
    }
    
    if (sidesUsed[WEST]) {
        leftWall = entireWall->createChildSceneNode("leftWallNode" + Util::toStringInt(wallID));
        Entity* leftWallEntity = leftWall->getCreator()->createEntity("leftWallEntity" + Util::toStringInt(wallID), "wallTileMesh");
        leftWall->attachObject(leftWallEntity);
        move = Vector3(-width / 2, 0, 0);
        leftWall->translate(move);
        leftWall->roll(Degree(270));
        leftWall->scale(wallLength, 1.0, depth);
        leftWallEntity->setMaterialName(materialName);
    }
    
    entireWall->setPosition(center);
    entireWall->setOrientation(rot);

    ++wallID;
}

double TunnelSlice::getWallLength() const
{
	return width / (2 * Math::Cos(Ogre::Radian(Math::PI) / 4) + 1);
}

TunnelType TunnelSlice::getType() const
{
    return type;
}

Quaternion TunnelSlice::getQuaternion() const
{
    return rot;
}

Vector3 TunnelSlice::getStart() const
{
    return center - getForward() * (depth / 2);
}

Vector3 TunnelSlice::getEnd() const
{
    return center + getForward() * (depth / 2);
}

Vector3 TunnelSlice::getCenter() const
{
	return center;
}

Vector3 TunnelSlice::getCenter(double t) const
{
    Vector3 start = getStart();
    Vector3 end = getEnd();
    
    return start + (end - start) * t;
}

// Since slices have an axis for its center, we project pos onto the axis
double TunnelSlice::getT(Vector3 pos) const
{
    Vector3 start = getStart();
    Vector3 end = getEnd();
    return getForward().dotProduct(pos - start) / (end - start).length();
}

double TunnelSlice::getPrerangeT() const
{
    return prerangeT;
}


Vector3 TunnelSlice::getForward() const
{
    Quaternion forward = Quaternion(0, 0, 0, -1);
    forward = rot * forward * rot.Inverse();
    return Vector3(forward.x, forward.y, forward.z);
}
Vector3 TunnelSlice::getUpward() const
{
    Quaternion upward = Quaternion(0, 0, 1, 0);
    upward = rot * upward * rot.Inverse();
    return Vector3(upward.x, upward.y, upward.z);
}
Vector3 TunnelSlice::getRight() const
{
    Quaternion right = Quaternion(0, 1, 0, 0);
    right = rot * right * rot.Inverse();
    return Vector3(right.x, right.y, right.z);
}

std::vector<Pod*> TunnelSlice::getPods() const
{
    return pods;
}

SectionInfo TunnelSlice::getSectionInfo() const
{
    return sectionInfo;
}

PodInfo TunnelSlice::getPodInfo() const
{
    return podInfo;
}

bool TunnelSlice::isPodHistory() const
{
    return podHistory;
}

bool TunnelSlice::isInfoStored() const
{
    return infoStored;
}

bool TunnelSlice::hasAvailableSide(Direction side) const
{
    if (side == NO_DIRECTION)
        return false;
    return sidesUsed[side];
}

std::vector<Pod*> TunnelSlice::findCollisions(SceneNode* ent) const
{
    std::vector<Pod*> ret;
    
	for (int i = 0; i < pods.size(); ++i)
	{
		if (ent->_getWorldAABB().intersects(
                pods[i]->getHead()->_getWorldAABB()))
			ret.push_back(pods[i]);
	}
    
	return ret;
}

std::vector<Pod*> TunnelSlice::findCollisions(Vine* vine) const
{
    std::vector<Pod*> ret;
    
	for (int i = 0; i < pods.size(); ++i)
	{
        if (vine->loc == pods[i]->getLoc())
        {
            //std::cout << vine->aftert << " " << vine->previoust << std::endl;
            if ((vine->aftert >= 0.46 && vine->aftert <= 0.54) ||
                (vine->previoust < 0.46 && vine->aftert > 0.54))
                ret.push_back(pods[i]);
        }
         /*
        double maxR = vine->getRadius() + pods[i]->getHeadRadius() * 1.5;

        double dx = vine->getTip()->getPosition().x - pods[i]->getHead()->_getDerivedPosition().x;
        double dy = vine->getTip()->getPosition().y - pods[i]->getHead()->_getDerivedPosition().y;
        double dz = vine->getTip()->getPosition().z - pods[i]->getHead()->_getDerivedPosition().z;
		if (dx * dx + dy * dy + dz * dz <= maxR * maxR)
			ret.push_back(pods[i]);
         */
	}
    
	return ret;
}

Vector3 TunnelSlice::requestWallDistance(Direction dir) const
{
	double wallLength = getWallLength();
    Vector3 move;
	switch (dir)
	{
        case NORTHWEST:
            move = Vector3(-(width + wallLength) / 4, (width + wallLength) / 4, 0);
            break;
        case NORTH:
            move = Vector3(0, width / 2, 0);
            break;
        case NORTHEAST:
            move = Vector3((width + wallLength) / 4, (width + wallLength) / 4, 0);
            break;
        case EAST:
            move = Vector3(width / 2, 0, 0);
            break;
        case SOUTHEAST:
            move = Vector3((width + wallLength) / 4, -(width + wallLength) / 4, 0);
            break;
        case SOUTH:
            move = Vector3(0, -width / 2, 0);
            break;
        case SOUTHWEST:
            move = Vector3(-(width + wallLength) / 4, -(width + wallLength) / 4, 0);
            break;
        case WEST:
            move = Vector3(-width / 2, 0, 0);
            break;
        default:
            // No Direction
            break;
	}
    return move;
}

Vector3 TunnelSlice::requestMove(Direction dir) const
{
	double wallLength = getWallLength();
	const double WALL_OFFSET = wallLength / 2;
    
    Vector3 move = rot * requestWallDistance(dir);
    move = move * ((move.length() - WALL_OFFSET) / move.length());
    
    return move;
}

Vector3 TunnelSlice::requestPosition(Vector3 cur, Direction dir) const
{
    return cur + requestMove(dir);
}

void TunnelSlice::setSectionInfo(const SectionInfo & value)
{
    sectionInfo = value;
}

void TunnelSlice::setPodInfo(const PodInfo & value)
{
    podInfo = value;
}

void TunnelSlice::setPodHistory(bool value)
{
    podHistory = value;
}

void TunnelSlice::setInfoStored(bool value)
{
    infoStored = value;
}

void TunnelSlice::setPrerangeT(double value)
{
    prerangeT = value;
}

void TunnelSlice::move(Vector3 delta)
{
	center.x += delta.x;
	center.y += delta.y;
	center.z += delta.z;
	
    entireWall->translate(delta);
	//topLeftWall->translate(delta);
	//topWall->translate(delta);
	//topRightWall->translate(delta);
	//rightWall->translate(delta);
	//bottomRightWall->translate(delta);
	//bottomWall->translate(delta);
	//bottomLeftWall->translate(delta);
	//leftWall->translate(delta);
    if (entireIntermediate)
        entireIntermediate->translate(delta);
    
	for (int i = 0; i < pods.size(); ++i)
		pods[i]->move(delta);
}

void TunnelSlice::addPod(Direction loc, PodType type)
{
	double wallLength = getWallLength();
	const double STEM_RADIUS = globals.podStemRadius;
	const double HEAD_RADIUS = globals.podHeadRadius;
	const double STEM_LENGTH = globals.podStemLength;
    
    Vector3 base;
    Vector3 head;
    Vector3 move = requestWallDistance(loc);
    //base = center + move;
    //move = move * ((move.length() - STEM_LENGTH) / move.length());
    //head = center + move;
    base = move;
    move = move * ((move.length() - STEM_LENGTH) / move.length());
    head = move;
    
	pods.push_back(new Pod(entireWall, base, head, type, STEM_RADIUS, HEAD_RADIUS, loc));
}

void TunnelSlice::setIntermediateWall(SceneNode* entire, Direction dir, Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4)
{
    std::string meshName = "intermediateMesh" + Util::toStringInt(intermediateMeshID);
    ManualObject * manual = parentNode->getCreator()->createManualObject(meshName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    Vector3 n = (p2 - p1).crossProduct(p4 - p1).normalisedCopy();
    
    Vector3 d1 = (p1 - p3).normalisedCopy();
    Vector3 d2 = (p2 - p4).normalisedCopy();
    
    manual->position(p1 + d1 * Util::EPSILON);
    manual->normal(n);
    manual->textureCoord(1, 1);
    manual->position(p2 + d2 * Util::EPSILON);
    manual->normal(n);
    manual->textureCoord(1, 0);
    manual->position(p3 + d1 * -Util::EPSILON);
    manual->normal(n);
    manual->textureCoord(0, 0);
    manual->position(p4 + d2 * -Util::EPSILON);
    manual->normal(n);
    manual->textureCoord(0, 1);
    manual->quad(0, 1, 2, 3);
    
    manual->end();
    MeshPtr mesh = manual->convertToMesh(meshName);
    
    Vector3 bl = p1;
    Vector3 tr = p1;
    bl = Vector3(min(bl.x, p2.x), min(bl.y, p2.y), min(bl.z, p2.z));
    bl = Vector3(min(bl.x, p3.x), min(bl.y, p3.y), min(bl.z, p3.z));
    bl = Vector3(min(bl.x, p4.x), min(bl.y, p4.y), min(bl.z, p4.z));
    tr = Vector3(max(tr.x, p2.x), max(tr.y, p2.y), max(tr.z, p2.z));
    tr = Vector3(max(tr.x, p3.x), max(tr.y, p3.y), max(tr.z, p3.z));
    tr = Vector3(max(tr.x, p4.x), max(tr.y, p4.y), max(tr.z, p4.z));
    mesh->_setBounds( AxisAlignedBox( bl, tr ), true );
    
    double l1 = (p3 - p1).length();// / 2;
    double l2 = (p4 - p2).length();// / 2;
    mesh->_setBoundingSphereRadius(l1 > l2 ? l1 : l2);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
    
    SceneNode* target = entire->createChildSceneNode("intermediateSegmentSubNode" + Util::toStringInt(intermediateMeshID));
    
    Entity* intermediateSegmentEntity = entireIntermediate->getCreator()->createEntity("intermediateSegmentEntity" + Util::toStringInt(intermediateMeshID), meshName);
    intermediateSegmentEntity->setMaterialName(materialName);
    
    switch (dir)
    {
        case NORTHWEST:
            topLeftIntermediate = target;
            break;
        case NORTH:
            topIntermediate = target;
            break;
        case NORTHEAST:
            topRightIntermediate = target;
            break;
        case EAST:
            rightIntermediate = target;
            break;
        case SOUTHEAST:
            bottomRightIntermediate = target;
            break;
        case SOUTH:
            bottomIntermediate = target;
            break;
        case SOUTHWEST:
            bottomLeftIntermediate = target;
            break;
        case WEST:
            leftIntermediate = target;
            break;
        default:
            break;
    }
    
    target->attachObject(intermediateSegmentEntity);
    
    intermediateMeshID++;
}

void TunnelSlice::connect(TunnelSlice* next)
{
	double wallLength1 = getWallLength();
	double wallLength2 = next->getWallLength();
    
    entireIntermediate = parentNode->createChildSceneNode("intermediateSegmentNode" + Util::toStringInt(intermediateMeshID));
    
    Quaternion q1 = getQuaternion();
    Quaternion q2 = next->getQuaternion();
    
    Vector3 start = center + getForward() * (depth / 2);
    Vector3 end = next->center - next->getForward() * (next->depth / 2);
    
    Vector3 move;
    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
    Vector3 p4;
    
    move = Vector3(-wallLength1 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), wallLength1 / 2, 0);
    move = q1 * move;
    p1 = start + move;
    move = Vector3(-wallLength2 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), wallLength2 / 2, 0);
    move = q2 * move;
    p2 = end + move;
    move = Vector3(-wallLength2 / 2, wallLength2 * (0.5 + Math::Sin(Ogre::Radian(Math::PI) / 4)), 0);
    move = q2 * move;
    p3 = end + move;
    move = Vector3(-wallLength1 / 2, wallLength1 * (0.5 + Math::Sin(Ogre::Radian(Math::PI) / 4)), 0);
    move = q1 * move;
    p4 = start + move;
    if (sidesUsed[NORTHWEST])
        setIntermediateWall(entireIntermediate, NORTHWEST, p1, p2, p3, p4);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(wallLength2 / 2, wallLength2 * (0.5 + Math::Sin(Ogre::Radian(Math::PI) / 4)), 0);
    move = q2 * move;
    p3 = end + move;
    move = Vector3(wallLength1 / 2, wallLength1 * (0.5 + Math::Sin(Ogre::Radian(Math::PI) / 4)), 0);
    move = q1 * move;
    p4 = start + move;
    if (sidesUsed[NORTH])
        setIntermediateWall(entireIntermediate, NORTH, p1, p2, p3, p4);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(wallLength2 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), wallLength2 / 2, 0);
    move = q2 * move;
    p3 = end + move;
    move = Vector3(wallLength1 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), wallLength1 / 2, 0);
    move = q1 * move;
    p4 = start + move;
    if (sidesUsed[NORTHEAST])
        setIntermediateWall(entireIntermediate, NORTHEAST, p1, p2, p3, p4);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(wallLength2 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), -wallLength2 / 2, 0);
    move = q2 * move;
    p3 = end + move;
    move = Vector3(wallLength1 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), -wallLength1 / 2, 0);
    move = q1 * move;
    p4 = start + move;
    if (sidesUsed[EAST])
        setIntermediateWall(entireIntermediate, EAST, p1, p2, p3, p4);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(wallLength2 / 2, -wallLength2 * (0.5 + Math::Sin(Ogre::Radian(Math::PI) / 4)), 0);
    move = q2 * move;
    p3 = end + move;
    move = Vector3(wallLength1 / 2, -wallLength1 * (0.5 + Math::Sin(Ogre::Radian(Math::PI) / 4)), 0);
    move = q1 * move;
    p4 = start + move;
    if (sidesUsed[SOUTHEAST])
        setIntermediateWall(entireIntermediate, SOUTHEAST, p1, p2, p3, p4);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(-wallLength2 / 2, -wallLength2 * (0.5 + Math::Sin(Ogre::Radian(Math::PI) / 4)), 0);
    move = q2 * move;
    p3 = end + move;
    move = Vector3(-wallLength1 / 2, -wallLength1 * (0.5 + Math::Sin(Ogre::Radian(Math::PI) / 4)), 0);
    move = q1 * move;
    p4 = start + move;
    if (sidesUsed[SOUTH])
        setIntermediateWall(entireIntermediate, SOUTH, p1, p2, p3, p4);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(-wallLength2 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), -wallLength2 / 2, 0);
    move = q2 * move;
    p3 = end + move;
    move = Vector3(-wallLength1 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), -wallLength1 / 2, 0);
    move = q1 * move;
    p4 = start + move;
    if (sidesUsed[SOUTHWEST])
        setIntermediateWall(entireIntermediate, SOUTHWEST, p1, p2, p3, p4);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(-wallLength1 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), wallLength1 / 2, 0);
    move = q2 * move;
    p3 = end + move;
    move = Vector3(-wallLength2 * (0.5 + Math::Cos(Ogre::Radian(Math::PI) / 4)), wallLength2 / 2, 0);
    move = q1 * move;
    p4 = start + move;
    if (sidesUsed[WEST])
        setIntermediateWall(entireIntermediate, WEST, p1, p2, p3, p4);    
}

void TunnelSlice::disconnect()
{
    if (!entireIntermediate)
        return;
    
    if (topLeftIntermediate)
        topLeftIntermediate->getCreator()->destroyMovableObject(topLeftIntermediate->getAttachedObject(0)); // Assuming only one entity
    if (topIntermediate)
        topIntermediate->getCreator()->destroyMovableObject(topIntermediate->getAttachedObject(0));
    if (topRightIntermediate)
        topRightIntermediate->getCreator()->destroyMovableObject(topRightIntermediate->getAttachedObject(0));
    if (rightIntermediate)
        rightIntermediate->getCreator()->destroyMovableObject(rightIntermediate->getAttachedObject(0));
    if (bottomRightIntermediate)
        bottomRightIntermediate->getCreator()->destroyMovableObject(bottomRightIntermediate->getAttachedObject(0));
    if (bottomIntermediate)
        bottomIntermediate->getCreator()->destroyMovableObject(bottomIntermediate->getAttachedObject(0));
    if (bottomLeftIntermediate)
        bottomLeftIntermediate->getCreator()->destroyMovableObject(bottomLeftIntermediate->getAttachedObject(0));
    if (leftIntermediate)
        leftIntermediate->getCreator()->destroyMovableObject(leftIntermediate->getAttachedObject(0));
    entireIntermediate->removeAndDestroyAllChildren();
    entireIntermediate->getCreator()->destroySceneNode(entireIntermediate);
    entireIntermediate = NULL;
    topLeftIntermediate = NULL;
    topIntermediate = NULL;
    topRightIntermediate = NULL;
    rightIntermediate = NULL;
    bottomRightIntermediate = NULL;
    bottomIntermediate = NULL;
    bottomLeftIntermediate = NULL;
    leftIntermediate = NULL;
}

void TunnelSlice::clearPods()
{
    for (int i = 0; i < pods.size(); ++i) {
        pods[i]->removeFromScene();
        delete pods[i];
    }
    pods.clear();
}

void TunnelSlice::updateGrowth(double nt)
{
    growthT += nt;
    if (growthT > 1) growthT = 1;
    if (growthT < 0) growthT = 0;
    for (int i = 0; i < pods.size(); ++i)
        pods[i]->setToGrowth(growthT);
}

void TunnelSlice::rejuvenate(TunnelType type, Vector3 center, Quaternion rot, double width, double depth, const std::string & material, const bool sides[NUM_DIRECTIONS])
{
    this->type = type;
    this->center = center;
    this->rot = rot;
    this->width = width;
    this->depth = depth;
    this->materialName = material;
    for (int i = 0; i < NUM_DIRECTIONS; ++i)
        sidesUsed[i] = sides[i];
    growthT = 0;
    prerangeT = 0;
    infoStored = false;
    
    removeFromScene(); //also clears pods and disconnects
    initWalls();
}

void TunnelSlice::removeFromScene()
{
	for (int i = 0; i < pods.size(); ++i)
		pods[i]->removeFromScene();
    
    if (topLeftWall)
        topLeftWall->getCreator()->destroyMovableObject(topLeftWall->getAttachedObject(0)); // Assuming only one entity
    if (topWall)
        topWall->getCreator()->destroyMovableObject(topWall->getAttachedObject(0));
    if (topRightWall)
        topRightWall->getCreator()->destroyMovableObject(topRightWall->getAttachedObject(0));
    if (rightWall)
        rightWall->getCreator()->destroyMovableObject(rightWall->getAttachedObject(0));
    if (bottomRightWall)
        bottomRightWall->getCreator()->destroyMovableObject(bottomRightWall->getAttachedObject(0));
    if (bottomWall)
        bottomWall->getCreator()->destroyMovableObject(bottomWall->getAttachedObject(0));
    if (bottomLeftWall)
        bottomLeftWall->getCreator()->destroyMovableObject(bottomLeftWall->getAttachedObject(0));
    if (leftWall)
        leftWall->getCreator()->destroyMovableObject(leftWall->getAttachedObject(0));
    
    entireWall->removeAndDestroyAllChildren();
    entireWall->getCreator()->destroySceneNode(entireWall);
    
    disconnect();
	
	for (int i = 0; i < pods.size(); ++i)
        delete pods[i];
	topLeftWall = NULL;
	topWall = NULL;
	topRightWall = NULL;
	rightWall = NULL;
	bottomRightWall = NULL;
	bottomWall = NULL;
	bottomLeftWall = NULL;
	leftWall = NULL;
    entireWall = NULL;
	pods.clear();
}
