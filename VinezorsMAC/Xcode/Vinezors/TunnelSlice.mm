#include "TunnelSlice.h"

#include <cstdlib>

TunnelSlice::TunnelSlice()
: center(), rot(), width(0), depth(0), type(NORMAL_BLANK),
topLeftWall(NULL), topWall(NULL), topRightWall(NULL), rightWall(NULL), bottomRightWall(NULL), bottomWall(NULL), bottomLeftWall(NULL), leftWall(NULL), intermediateSegment(NULL),
pods()
{}

TunnelSlice::TunnelSlice(CollisionScene *scene, TunnelType type, Vector3 center, Quaternion rot, Number width, Number depth)
: center(center), rot(rot), width(width), depth(depth), type(type),
topLeftWall(NULL), topWall(NULL), topRightWall(NULL), rightWall(NULL), bottomRightWall(NULL), bottomWall(NULL), bottomLeftWall(NULL), leftWall(NULL), intermediateSegment(NULL),
pods()
{
	String filePathWallTexture;
	if (type == NORMAL_WITH_PODS)
		filePathWallTexture = "resources/metal.png";
	else if (type == NORMAL_BLANK)
		filePathWallTexture = "resources/yellow_solid.png";
	else if (type == NORMAL_ELONGATED)
		filePathWallTexture = "resources/yellow_solid_ellipse.png";
    
	Number wallLength = width / (2 * cos(PI / 4) + 1);
    
    double angle;
    Quaternion q;
    Vector3 move;
    //cout << axis.x << "," << axis.y << "," << axis.z << endl;
    //cout << angle << endl;
    //cout << rot.w << "," << rot.x << "," << rot.y << "," << rot.z << endl;
    
    //move = Vector3((-width + wallLength) / 4, (width + wallLength) / 4, 0);
    
    //cout << move.x << "," << move.y << "," << move.z << endl;
    //move = rot.applyTo(move);
    //cout << move.x << "," << move.y << "," << move.z << endl;
    
    angle = 225;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, angle);
    q = q.Inverse();
    q = rot * q;
    move = Vector3(-(width + wallLength) / 4, (width + wallLength) / 4, 0);
    move = rot.applyTo(move);
	topLeftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topLeftWall->setPosition(center + move);
    topLeftWall->setRotationQuat(q.w, q.x, q.y, q.z);
	topLeftWall->setMaterialByName("WallMaterial");
    //topLeftWall->loadTexture("resources/red_solid.png");
    
    angle = 180;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, angle);
    q = q.Inverse();
    q = rot * q;
    move = Vector3(0, width / 2, 0);
    move = rot.applyTo(move);
	topWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topWall->setPosition(center + move);
    topWall->setRotationQuat(q.w, q.x, q.y, q.z);
	topWall->setMaterialByName("WallMaterial");
	//topWall->loadTexture("resources/yellow_solid.png");
    
    angle = 135;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, angle);
    q = q.Inverse();
    q = rot * q;
    move = Vector3((width + wallLength) / 4, (width + wallLength) / 4, 0);
    move = rot.applyTo(move);
	topRightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	topRightWall->setPosition(center + move);
    topRightWall->setRotationQuat(q.w, q.x, q.y, q.z);
	topRightWall->setMaterialByName("WallMaterial");
    
    angle = 90;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, angle);
    q = q.Inverse();
    q = rot * q;
    move = Vector3(width / 2, 0, 0);
    move = rot.applyTo(move);
	rightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	rightWall->setPosition(center + move);
    rightWall->setRotationQuat(q.w, q.x, q.y, q.z);
	rightWall->setMaterialByName("WallMaterial");
    
    angle = 45;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, angle);
    q = q.Inverse();
    q = rot * q;
    move = Vector3((width + wallLength) / 4, -(width + wallLength) / 4, 0);
    move = rot.applyTo(move);
	bottomRightWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomRightWall->setPosition(center + move);
    bottomRightWall->setRotationQuat(q.w, q.x, q.y, q.z);
	bottomRightWall->setMaterialByName("WallMaterial");
    
    angle = 0;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, angle);
    q = q.Inverse();
    q = rot * q;
    move = Vector3(0, -width / 2, 0);
    move = rot.applyTo(move);
	bottomWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomWall->setPosition(center + move);
    bottomWall->setRotationQuat(q.w, q.x, q.y, q.z);
	bottomWall->setMaterialByName("WallMaterial");
	
    angle = 315;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, angle);
    q = q.Inverse();
    q = rot * q;
    move = Vector3(-(width + wallLength) / 4, -(width + wallLength) / 4, 0);
    move = rot.applyTo(move);
	bottomLeftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	bottomLeftWall->setPosition(center + move);
    bottomLeftWall->setRotationQuat(q.w, q.x, q.y, q.z);
	bottomLeftWall->setMaterialByName("WallMaterial");
    
    angle = 270;
    q.createFromAxisAngle(TUNNEL_REFERENCE_FORWARD.x, TUNNEL_REFERENCE_FORWARD.y, TUNNEL_REFERENCE_FORWARD.z, angle);
    q = q.Inverse();
    q = rot * q;
    move = Vector3(-width / 2, 0, 0);
    move = rot.applyTo(move);
	leftWall = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, wallLength, depth);
	leftWall->setPosition(center + move);
    leftWall->setRotationQuat(q.w, q.x, q.y, q.z);
	leftWall->setMaterialByName("WallMaterial");
    
    /*
     topLeftWall->backfaceCulled = false;
     topWall->backfaceCulled = false;
     topRightWall->backfaceCulled = false;
     rightWall->backfaceCulled = false;
     bottomRightWall->backfaceCulled = false;
     bottomWall->backfaceCulled = false;
     bottomLeftWall->backfaceCulled = false;
     leftWall->backfaceCulled = false;
     */
    
	addToCollisionScene(scene);
}

Quaternion TunnelSlice::getQuaternion()
{
    return rot;
    /*
     Quaternion rot;
     Vector3 axis = forward.crossProduct(TUNNEL_REFERENCE_FORWARD);
     if (axis == Vector3(0,0,0)) axis = TUNNEL_REFERENCE_FORWARD;
     axis.Normalize();
     Number angle = forward.angleBetween(TUNNEL_REFERENCE_FORWARD) * (180.0/ PI);
     if (forward.dot(Vector3(1,1,1)) >= 0) angle += 180;
     rot.createFromAxisAngle(axis.x, axis.y, axis.z, angle);
     //if (angle <= 180)
     rot = rot.Inverse();
     return rot;
     */
}

Vector3 TunnelSlice::getCenter()
{
	return center;
}

Vector3 TunnelSlice::getCenter(Number t)
{
    Vector3 start = center - getForward() * (depth / 2);
    Vector3 end = center + getForward() * (depth / 2);
    
    return start + (end - start) * t;
}

// Since slices have an axis for its center, we project pos onto the axis
Number TunnelSlice::getT(Vector3 pos)
{
    Vector3 start = center - getForward() * (depth / 2);
    Vector3 end = center + getForward() * (depth / 2);
    return getForward().dot(pos - start) / (end - start).length();
}

Vector3 TunnelSlice::getForward()
{
    Quaternion forward;
    forward.set(0, 0, 0, -1);
    forward = rot * forward * rot.Inverse();
    return Vector3(forward.x, forward.y, forward.z);
}
Vector3 TunnelSlice::getUpward()
{
    Quaternion upward;
    upward.set(0, 0, 1, 0);
    upward = rot * upward * rot.Inverse();
    return Vector3(upward.x, upward.y, upward.z);
}
Vector3 TunnelSlice::getRight()
{
    Quaternion right;
    right.set(0, 1, 0, 0);
    right = rot * right * rot.Inverse();
    return Vector3(right.x, right.y, right.z);
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

Vector3 TunnelSlice::requestPosition(Vector3 cur, Direction dir) const
{
	Number wallLength = width / (2 * cos(PI / 4) + 1);
	const Number STEM_LENGTH = wallLength / 3;
    
    Vector3 move;
    
    Vector3 ret;
	Vector3 base;
	Vector3 head;
	switch (dir)
	{
        case NORTHWEST:
            move = Vector3(-(width + wallLength) / 4, (width + wallLength) / 4, 0);
            move = rot.applyTo(move);
            base = Vector3(cur + move);
            move = Vector3(-cos(3 * PI / 4) * STEM_LENGTH, -sin(3 * PI / 4) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case NORTH:
            move = Vector3(0, width / 2, 0);
            move = rot.applyTo(move);
            base = Vector3(cur + move);
            move = Vector3(-cos(PI / 2) * STEM_LENGTH, -sin(PI / 2) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case NORTHEAST:
            move = Vector3((width + wallLength) / 4, (width + wallLength) / 4, 0);
            move = rot.applyTo(move);
            base = Vector3(cur + move);
            move = Vector3(-cos(PI / 4) * STEM_LENGTH, -sin(PI / 4) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case EAST:
            move = Vector3(width / 2, 0, 0);
            move = rot.applyTo(move);
            base = Vector3(cur + move);
            move = Vector3(-cos(0.0) * STEM_LENGTH, -sin(0.0) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case SOUTHEAST:
            move = Vector3((width + wallLength) / 4, -(width + wallLength) / 4, 0);
            move = rot.applyTo(move);
            base = Vector3(cur + move);
            move = Vector3(-cos(-PI / 4) * STEM_LENGTH, -sin(-PI / 4) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case SOUTH:
            move = Vector3(0, -width / 2, 0);
            move = rot.applyTo(move);
            base = Vector3(cur + move);
            move = Vector3(-cos(-PI / 2) * STEM_LENGTH, -sin(-PI / 2) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case SOUTHWEST:
            move = Vector3(-(width + wallLength) / 4, -(width + wallLength) / 4, 0);
            move = rot.applyTo(move);
            base = Vector3(cur + move);
            move = Vector3(-cos(-3 * PI / 4) * STEM_LENGTH, -sin(-3 * PI / 4) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case WEST:
            move = Vector3(-width / 2, 0, 0);
            move = rot.applyTo(move);
            base = Vector3(cur + move);
            move = Vector3(-cos(-PI) * STEM_LENGTH, -sin(-PI) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        default:
            // No Direction
            break;
	}
    ret = head;
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
	String filePathWallTexture;
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
    
    Vector3 move;
    
	Vector3 base;
	Vector3 head;
	switch (loc)
	{
        case NORTHWEST:
            move = Vector3(-(width + wallLength) / 4, (width + wallLength) / 4, 0);
            move = rot.applyTo(move);
            base = Vector3(center + move);
            move = Vector3(-cos(3 * PI / 4) * STEM_LENGTH, -sin(3 * PI / 4) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case NORTH:
            move = Vector3(0, width / 2, 0);
            move = rot.applyTo(move);
            base = Vector3(center + move);
            move = Vector3(-cos(PI / 2) * STEM_LENGTH, -sin(PI / 2) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case NORTHEAST:
            move = Vector3((width + wallLength) / 4, (width + wallLength) / 4, 0);
            move = rot.applyTo(move);
            base = Vector3(center + move);
            move = Vector3(-cos(PI / 4) * STEM_LENGTH, -sin(PI / 4) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case EAST:
            move = Vector3(width / 2, 0, 0);
            move = rot.applyTo(move);
            base = Vector3(center + move);
            move = Vector3(-cos(0.0) * STEM_LENGTH, -sin(0.0) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case SOUTHEAST:
            move = Vector3((width + wallLength) / 4, -(width + wallLength) / 4, 0);
            move = rot.applyTo(move);
            base = Vector3(center + move);
            move = Vector3(-cos(-PI / 4) * STEM_LENGTH, -sin(-PI / 4) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case SOUTH:
            move = Vector3(0, -width / 2, 0);
            move = rot.applyTo(move);
            base = Vector3(center + move);
            move = Vector3(-cos(-PI / 2) * STEM_LENGTH, -sin(-PI / 2) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case SOUTHWEST:
            move = Vector3(-(width + wallLength) / 4, -(width + wallLength) / 4, 0);
            move = rot.applyTo(move);
            base = Vector3(center + move);
            move = Vector3(-cos(-3 * PI / 4) * STEM_LENGTH, -sin(-3 * PI / 4) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        case WEST:
            move = Vector3(-width / 2, 0, 0);
            move = rot.applyTo(move);
            base = Vector3(center + move);
            move = Vector3(-cos(-PI) * STEM_LENGTH, -sin(-PI) * STEM_LENGTH, 0);
            move = rot.applyTo(move);
            head = Vector3(base + move);
            break;
        default:
            // No Direction
            return;
	}
    
	pods.push_back(new Pod(scene, center, rot, type, base, head, STEM_RADIUS, HEAD_RADIUS));
}

void TunnelSlice::connect(CollisionScene* scene, TunnelSlice* next)
{
	Number wallLength1 = width / (2 * cos(PI / 4) + 1);
	Number wallLength2 = next->width / (2 * cos(PI / 4) + 1);
    
    Vector3 move;
    Quaternion rot1 = getQuaternion();
    Quaternion rot2 = next->getQuaternion();
    Vector3 start = center + getForward() * (depth / 2);
    Vector3 end = next->center - next->getForward() * (next->depth / 2);
    
    intermediateSegment = new SceneMesh(Mesh::QUAD_MESH);
    Polycode::Polygon* topLeft = new Polycode::Polygon();
    Polycode::Polygon* top = new Polycode::Polygon();
    Polycode::Polygon* topRight = new Polycode::Polygon();
    Polycode::Polygon* right = new Polycode::Polygon();
    Polycode::Polygon* bottomRight = new Polycode::Polygon();
    Polycode::Polygon* bottom = new Polycode::Polygon();
    Polycode::Polygon* bottomLeft = new Polycode::Polygon();
    Polycode::Polygon* left = new Polycode::Polygon();
    
    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
    Vector3 p4;
    
    move = Vector3(-wallLength1 * (0.5 + cos(PI / 4)), wallLength1 / 2, 0);
    move = rot1.applyTo(move);
    p1 = start + move;
    move = Vector3(-wallLength2 * (0.5 + cos(PI / 4)), wallLength2 / 2, 0);
    move = rot2.applyTo(move);
    p2 = end + move;
    move = Vector3(-wallLength2 / 2, wallLength2 * (0.5 + sin(PI / 4)), 0);
    move = rot2.applyTo(move);
    p3 = end + move;
    move = Vector3(-wallLength1 / 2, wallLength1 * (0.5 + sin(PI / 4)), 0);
    move = rot1.applyTo(move);
    p4 = start + move;
    topLeft->addVertex(p1.x, p1.y, p1.z);
    topLeft->addVertex(p2.x, p2.y, p2.z);
    topLeft->addVertex(p3.x, p3.y, p3.z);
    topLeft->addVertex(p4.x, p4.y, p4.z);
    intermediateSegment->getMesh()->addPolygon(topLeft);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(wallLength2 / 2, wallLength2 * (0.5 + sin(PI / 4)), 0);
    move = rot2.applyTo(move);
    p3 = end + move;
    move = Vector3(wallLength1 / 2, wallLength1 * (0.5 + sin(PI / 4)), 0);
    move = rot1.applyTo(move);
    p4 = start + move;
    top->addVertex(p1.x, p1.y, p1.z);
    top->addVertex(p2.x, p2.y, p2.z);
    top->addVertex(p3.x, p3.y, p3.z);
    top->addVertex(p4.x, p4.y, p4.z);
    intermediateSegment->getMesh()->addPolygon(top);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(wallLength2 * (0.5 + cos(PI / 4)), wallLength2 / 2, 0);
    move = rot2.applyTo(move);
    p3 = end + move;
    move = Vector3(wallLength1 * (0.5 + cos(PI / 4)), wallLength1 / 2, 0);
    move = rot1.applyTo(move);
    p4 = start + move;
    topRight->addVertex(p1.x, p1.y, p1.z);
    topRight->addVertex(p2.x, p2.y, p2.z);
    topRight->addVertex(p3.x, p3.y, p3.z);
    topRight->addVertex(p4.x, p4.y, p4.z);
    intermediateSegment->getMesh()->addPolygon(topRight);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(wallLength2 * (0.5 + cos(PI / 4)), -wallLength2 / 2, 0);
    move = rot2.applyTo(move);
    p3 = end + move;
    move = Vector3(wallLength1 * (0.5 + cos(PI / 4)), -wallLength1 / 2, 0);
    move = rot1.applyTo(move);
    p4 = start + move;
    right->addVertex(p1.x, p1.y, p1.z);
    right->addVertex(p2.x, p2.y, p2.z);
    right->addVertex(p3.x, p3.y, p3.z);
    right->addVertex(p4.x, p4.y, p4.z);
    intermediateSegment->getMesh()->addPolygon(right);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(wallLength2 / 2, -wallLength2 * (0.5 + sin(PI / 4)), 0);
    move = rot2.applyTo(move);
    p3 = end + move;
    move = Vector3(wallLength1 / 2, -wallLength1 * (0.5 + sin(PI / 4)), 0);
    move = rot1.applyTo(move);
    p4 = start + move;
    bottomRight->addVertex(p1.x, p1.y, p1.z);
    bottomRight->addVertex(p2.x, p2.y, p2.z);
    bottomRight->addVertex(p3.x, p3.y, p3.z);
    bottomRight->addVertex(p4.x, p4.y, p4.z);
    intermediateSegment->getMesh()->addPolygon(bottomRight);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(-wallLength2 / 2, -wallLength2 * (0.5 + sin(PI / 4)), 0);
    move = rot2.applyTo(move);
    p3 = end + move;
    move = Vector3(-wallLength1 / 2, -wallLength1 * (0.5 + sin(PI / 4)), 0);
    move = rot1.applyTo(move);
    p4 = start + move;
    bottom->addVertex(p1.x, p1.y, p1.z);
    bottom->addVertex(p2.x, p2.y, p2.z);
    bottom->addVertex(p3.x, p3.y, p3.z);
    bottom->addVertex(p4.x, p4.y, p4.z);
    intermediateSegment->getMesh()->addPolygon(bottom);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(-wallLength2 * (0.5 + cos(PI / 4)), -wallLength2 / 2, 0);
    move = rot2.applyTo(move);
    p3 = end + move;
    move = Vector3(-wallLength1 * (0.5 + cos(PI / 4)), -wallLength1 / 2, 0);
    move = rot1.applyTo(move);
    p4 = start + move;
    bottomLeft->addVertex(p1.x, p1.y, p1.z);
    bottomLeft->addVertex(p2.x, p2.y, p2.z);
    bottomLeft->addVertex(p3.x, p3.y, p3.z);
    bottomLeft->addVertex(p4.x, p4.y, p4.z);
    intermediateSegment->getMesh()->addPolygon(bottomLeft);
    
    p1 = p4;
    p2 = p3;
    move = Vector3(-wallLength1 * (0.5 + cos(PI / 4)), wallLength1 / 2, 0);
    move = rot2.applyTo(move);
    p3 = end + move;
    move = Vector3(-wallLength2 * (0.5 + cos(PI / 4)), wallLength2 / 2, 0);
    move = rot1.applyTo(move);
    p4 = start + move;
    left->addVertex(p1.x, p1.y, p1.z);
    left->addVertex(p2.x, p2.y, p2.z);
    left->addVertex(p3.x, p3.y, p3.z);
    left->addVertex(p4.x, p4.y, p4.z);
    intermediateSegment->getMesh()->addPolygon(left);
    
    //intermediateSegment->setMaterialByName("WallMaterial");
    scene->addCollisionChild(intermediateSegment, CollisionSceneEntity::SHAPE_MESH);
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
	if (intermediateSegment) scene->removeEntity(intermediateSegment);
	
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
    delete intermediateSegment; intermediateSegment = NULL;
    
	pods.clear();
}
