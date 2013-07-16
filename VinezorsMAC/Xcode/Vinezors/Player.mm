#include "Player.h"

Player::Player()
	: name(""), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), cursor(NULL), vines(), dir(NORTH), mousePos(), camPos(), vineOffset()
{}

Player::Player(CollisionScene *scene, const string & name, Vector3 camPos, Vector3 offset)
	: name(name), score(0), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), cursor(NULL), vines(), dir(NORTH), mousePos(), camPos(camPos), vineOffset(offset)
{
	cursor = new ScenePrimitive(ScenePrimitive::TYPE_SPHERE, 0.5, 5, 5);
	cursor->renderWireframe = true;
	cursor->setColor(1.0, 0.0, 0.0, 1.0);
	scene->addChild(cursor);
}

int Player::getScore() const
{
	return score;
}

Direction Player::getDir() const
{
	return dir;
}

bool Player::getMouseLeft() const
{
	return mouseLeft;
}

Vector2 Player::getMousePos() const
{
	return mousePos;
}

Vector3 Player::getCamPos() const
{
	return camPos;
}

Vector3 Player::getVineOffset() const
{
	return vineOffset;
}

Vector3 Player::requestTunnelPosition(Tunnel *tunnel, Direction dir) const
{
	Number width = tunnel->getSegmentWidth();
	Number depth = tunnel->getSegmentDepth();
	Number wallLength = width / (2 * cos(PI / 4) + 1);;
	const Number STEM_RADIUS = width / 100;
	const Number HEAD_RADIUS = width / 25;
	const Number STEM_LENGTH = wallLength / 3;

	Vector3 base;
	Vector3 head;
	Vector3 center = tunnel->getCenter();
	switch (dir)
	{
	case NORTHWEST:
		base = Vector3(center.x - (width + wallLength) / 4, center.y + (width + wallLength) / 4, camPos.z + vineOffset.z);
		head = Vector3(base.x - cos(3 * PI / 4) * STEM_LENGTH, base.y - sin(3 * PI / 4) * STEM_LENGTH, base.z);
		break;
	case NORTH:
		base = Vector3(center.x, center.y + width / 2, camPos.z + vineOffset.z);
		head = Vector3(base.x - cos(PI / 2) * STEM_LENGTH, base.y - sin(PI / 2) * STEM_LENGTH, base.z);
		break;
	case NORTHEAST:
		base = Vector3(center.x + (width + wallLength) / 4, center.y + (width + wallLength) / 4, camPos.z + vineOffset.z);
		head = Vector3(base.x - cos(PI / 4) * STEM_LENGTH, base.y - sin(PI / 4) * STEM_LENGTH, base.z);
		break;
	case EAST:
		base = Vector3(center.x + width / 2, center.y, camPos.z + vineOffset.z);
		head = Vector3(base.x - cos(0.0) * STEM_LENGTH, base.y - sin(0.0) * STEM_LENGTH, base.z);
		break;
	case SOUTHEAST:
		base = Vector3(center.x + (width + wallLength) / 4, center.y - (width + wallLength) / 4, camPos.z + vineOffset.z);
		head = Vector3(base.x - cos(-PI / 4) * STEM_LENGTH, base.y - sin(-PI / 4) * STEM_LENGTH, base.z);
		break;
	case SOUTH:
		base = Vector3(center.x, center.y - width / 2, camPos.z + vineOffset.z);
		head = Vector3(base.x - cos(-PI / 2) * STEM_LENGTH, base.y - sin(-PI / 2) * STEM_LENGTH, base.z);
		break;
	case SOUTHWEST:
		base = Vector3(center.x - (width + wallLength) / 4, center.y - (width + wallLength) / 4, camPos.z + vineOffset.z);
		head = Vector3(base.x - cos(-3 * PI / 4) * STEM_LENGTH, base.y - sin(-3 * PI / 4) * STEM_LENGTH, base.z);
		break;
	case WEST:
		base = Vector3(center.x - width / 2, center.y, camPos.z + vineOffset.z);
		head = Vector3(base.x - cos(-PI) * STEM_LENGTH, base.y - sin(-PI) * STEM_LENGTH, base.z);
		break;
	default:
		// No Direction
		break;
	}
	return head;
}

void Player::setScore(int value)
{
	score = value;
}

void Player::setMouseLeft(bool value)
{
	mouseLeft = value;
}

void Player::setKeyUp(bool value)
{
	keyUp = value;
}

void Player::setKeyDown(bool value)
{
	keyDown = value;
}

void Player::setKeyLeft(bool value)
{
	keyLeft = value;
}

void Player::setKeyRight(bool value)
{
	keyRight = value;
}

void Player::setDir(Direction value)
{
	dir = value;
}

void Player::setMousePos(Vector2 value)
{
	mousePos = value;
}

void Player::setCamPos(Vector3 value)
{
	camPos = value;
}

void Player::move(Vector3 delta)
{
	camPos += delta; 
	for (int i = 0; i < vines.size(); ++i)
		vines[i]->move(delta);
}

void Player::addVine(Vine *vine)
{
	vines.push_back(vine);
}

void Player::checkCollisions(Tunnel *tunnel)	
{
	if (tunnel->getCurrent() == NULL)
		return;
	for (int i = 0; i < vines.size(); ++i)
	{
		vector<Pod *> collided = tunnel->getCurrent()->findCollisions(tunnel->getScene(), vines[i]->getTip());
		for (int j = 0; j < collided.size(); ++j)
		{
			if (collided[j]->getHead()->enabled && collided[j]->getType() == POD_YELLOW)
				++score;
			collided[j]->hidePod();
		}
	}
}

void Player::update(Number elapsed, Tunnel *tunnel)
{
	const double CAM_SPEED = 14.0;

	move(Vector3(0, 0, -CAM_SPEED * elapsed));
	
	// Speed Up/Down keys
	if (keyUp)
		move(Vector3(0, 0, -CAM_SPEED * elapsed));
	if (keyDown)
		move(Vector3(0, 0, CAM_SPEED * elapsed));
	
	Vector3 targetPos = requestTunnelPosition(tunnel, dir);
	for (int i = 0; i < vines.size(); ++i)
	{
		vines[i]->setDest(targetPos);
	}
	cursor->setPosition(targetPos);

	for (int i = 0; i < vines.size(); ++i)
		vines[i]->update(elapsed);	
}