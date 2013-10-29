//
//  Player.h
//  Testing
//
//  Created by Calvin Phung on 9/13/13.
//
//

#ifndef __Testing__Player__
#define __Testing__Player__

#pragma once

#include <vector>
#include <fstream>
#include "Util.h"
#include "Vine.h"
#include "Tunnel.h"

class Player
{
private:
    unsigned seed;
    
    std::string name;
    int hp;
	double score;
	bool mouseLeft;
	bool keyUp;
	bool keyDown;
	bool keyLeft;
	bool keyRight;
	
    std::vector<Vine*> vines;
    
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
    int camSpeed;
	double vineOffset; // offset to camPos in direction of forward
    
    SceneNode* light;
    SceneNode* light2;
    SceneNode* light3;
    
    struct Result {
        int timestamp;
        SectionInfo sectionInfo;
        PodInfo podInfo;
        bool playerTookPod;
        int nback;
        bool goodPod;
    };
    std::vector<Result> results;
    
    double totalElapsed;
    TunnelSlice* vineSlice;
    double vineT;
public:
    
	Player();
	Player(const std::string & name, Vector3 camPos, Quaternion camRot, int camSpeed, double  offset, unsigned seed, const std::string & filename);
	
    unsigned getSeed() const;
    std::string getName() const;
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
	int getCamSpeed() const;
	Vector3 getVineOffset();
	double getTotalElapsed() const;
    
    void setSeed(unsigned value);
    void setName(const std::string & name);
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
    void setCamSpeed(int value);
	Vector3 getCamForward() const;
	Vector3 getCamUpward() const;
	Vector3 getCamRight() const;
    Quaternion getCombinedRotAndRoll() const;
    
    void newTunnel(Tunnel* tunnel);
    
	void move(Vector3 delta);
	void addVine(Vine* vine);
	void checkCollisions(Tunnel* tunnel);
    
	void update(double elapsed, Tunnel* tunnel);
    
    void reportResult() const;
    bool saveProgress(std::string file);
    
    ~Player();
};

#endif /* defined(__Testing__Player__) */
