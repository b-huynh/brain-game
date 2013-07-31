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

	Direction dir; // offset  on tunnel for vine
	Vector2 mousePos;
    Vector3 oldPos;
	Vector3 camPos;
    Quaternion oldRot;
    Quaternion camRot;
    Quaternion desireRot;
	Number vineOffset; // offset to camPos in direction of forward
    
    SceneLight * light;
    SceneLight * light2;
    SceneLight * light3;
    
public:
	Player();
	Player(CollisionScene *scene, const string & name, Vector3 camPos, Quaternion camRot, Number offset);
	
	int getScore() const;
	bool getMouseLeft() const;
	bool getKeyUp() const;
	bool getKeyDown() const;
	bool getKeyLeft() const;
	bool getKeyRight() const;
	Direction getDir() const;
	Vector2 getMousePos() const;
	Vector3 getOldPos() const;
	Vector3 getCamPos() const;
	Quaternion getOldRot() const;
	Quaternion getCamRot() const;
	Quaternion getDesireRot() const;
	Vector3 getVineOffset() const;

	void setScore(int value);
	void setMouseLeft(bool value);
	void setKeyUp(bool value);
	void setKeyDown(bool value);
	void setKeyLeft(bool value);
	void setKeyRight(bool value);
	
	void setDir(Direction value);
	void setMousePos(Vector2 value);
	void setOldPos(Vector3 value);
	void setCamPos(Vector3 value);
	void setOldRot(Quaternion value);
	void setCamRot(Quaternion value);
	void setDesireRot(Quaternion value);
	Vector3 getCamForward() const;
	Vector3 getCamUpward() const;
	Vector3 getCamRight() const;

	void move(Vector3 delta);
	void addVine(Vine *vine);
	void checkCollisions(Tunnel *tunnel);

	void update(Number elapsed, Tunnel *tunnel);
};