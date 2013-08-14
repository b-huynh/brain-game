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
    int hp;
	double score;
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
    int oldRoll;
    Quaternion camRot;
    int camRoll;
    Quaternion desireRot;
    int desireRoll;
    double camSpeed;
	Number vineOffset; // offset to camPos in direction of forward
    
    SceneLight * light;
    SceneLight * light2;
    SceneLight * light3;
    
    struct Result {
        int timestamp;
        SectionInfo sectionInfo;
        PodInfo podInfo;
        bool playerTookPod;
        int nback;
        bool goodPod;
    };
    vector<Result> results;
    
    Number totalElapsed;
public:
    
	Player();
	Player(CollisionScene *scene, const string & name, Vector3 camPos, Quaternion camRot, double camSpeed, Number offset);
	
    int getHP() const;
	double getScore() const;
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
    int getOldRoll() const;
	Quaternion getCamRot() const;
	int getCamRoll() const;
	Quaternion getDesireRot() const;
	int getDesireRoll() const;
	double getCamSpeed() const;
	Vector3 getVineOffset();
	Number getTotalElapsed() const;

    void setHP(int value);
	void setScore(double value);
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
    void setOldRoll(int value);
	void setCamRot(Quaternion value);
    void setCamRoll(int value);
	void setDesireRot(Quaternion value);
    void setDesireRoll(int value);
    void setCamSpeed(double value);
    void updateRot(double t);
	Vector3 getCamForward();
	Vector3 getCamUpward();
	Vector3 getCamRight();
    Quaternion getCombinedRotAndRoll();

	void move(Vector3 delta);
	void addVine(Vine *vine);
	void checkCollisions(Tunnel *tunnel);

	void update(Number elapsed, Tunnel *tunnel);
    
    bool saveProgress(std::string file);
};