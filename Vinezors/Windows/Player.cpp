#include "Player.h"

Player::Player()
	: name(""), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), vines(), dir(NO_DIRECTION), mousePos(), camPos()
{}

Player::Player(const string & name, Vector3 camPos)
	: name(name), mouseLeft(false), keyUp(false), keyDown(false), keyLeft(false), keyRight(false), vines(), dir(NO_DIRECTION), mousePos(), camPos(camPos)
{}

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

void Player::update(Number elapsed)
{
	const double CAM_SPEED = 5.0;

	move(Vector3(0, 0, -CAM_SPEED * elapsed));

	// Speed Up/Down keys
	if (keyUp)
		move(Vector3(0, 0, -CAM_SPEED * elapsed));
	if (keyDown)
		move(Vector3(0, 0, CAM_SPEED * elapsed));

	// *** TODO: Vines still need to be able to be directed to move somewhere.
}