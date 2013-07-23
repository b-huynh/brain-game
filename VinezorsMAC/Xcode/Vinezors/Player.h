#pragma once

#include <vector>
#include "Util.h"
#include "Vine.h"
#include "Tunnel.h"

using namespace std;

class Player
{
private:
	string name;
	int score;
	bool mouseLeft;
	bool keyUp;
	bool keyDown;
	bool keyLeft;
	bool keyRight;
	
	ScenePrimitive *cursor;
	vector<Vine *> vines;

	Direction dir;
	Vector2 mousePos;
	Vector3 camPos;
	Vector3 vineOffset; // offset to camPos
    
    SceneLight * light;
    SceneLight * light2;
    SceneLight * light3;
    
public:
	Player();
	Player(CollisionScene *scene, const string & name, Vector3 camPos, Vector3 offset);
	
	int getScore() const;
	bool getMouseLeft() const;
	Direction getDir() const;
	Vector2 getMousePos() const;
	Vector3 getCamPos() const;
	Vector3 getVineOffset() const;
	
	Vector3 requestTunnelPosition(Tunnel *tunnel, Direction dir) const;

	void setScore(int value);
	void setMouseLeft(bool value);
	void setKeyUp(bool value);
	void setKeyDown(bool value);
	void setKeyLeft(bool value);
	void setKeyRight(bool value);
	
	void setDir(Direction value);
	void setMousePos(Vector2 value);
	void setCamPos(Vector3 value);

	void move(Vector3 delta);
	void addVine(Vine *vine);
	void checkCollisions(Tunnel *tunnel);

	void update(Number elapsed, Tunnel *tunnel);
};